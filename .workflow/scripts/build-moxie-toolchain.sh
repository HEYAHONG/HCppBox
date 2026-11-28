#!/usr/bin/env bash
# Build the moxie-unknown-elf cross toolchain with crosstool-NG.
#
# The crosstool-NG sample "moxie-unknown-elf" does not enable C++ by default,
# so this script loads the sample and appends CT_CC_LANG_CXX=y before building.
# The toolchain build is retried up to 3 times: crosstool-NG downloads the
# component tarballs (binutils, gcc, newlib, gdb, ...) during the build and a
# transient network failure should not abort the whole CI job.
#
# The toolchain is installed into a dedicated prefix,
# ${MOXIE_TOOLCHAIN_PREFIX:-/opt/moxie-unknown-elf}, so moxie-unknown-elf-gcc
# lands in /opt/moxie-unknown-elf/bin. The workflows add that directory to PATH.
#
# CT_RM_RF_PREFIX_DIR is disabled (see below): crosstool-NG would otherwise
# delete the whole installation prefix at the start of a build. Keep the prefix
# dedicated anyway; do not install into /usr/local, where unrelated files such
# as the CI runner's Node.js binaries live.
set -euo pipefail

# crosstool-NG refuses to build when running as root unless this override is
# set. All CI jobs run in ephemeral containers (or VMs) as root, so allow it.
export CT_ALLOW_BUILD_AS_ROOT_SURE=1

CTNG_VERSION="${CTNG_VERSION:-1.29.0}"
INSTALL_PREFIX="${MOXIE_TOOLCHAIN_PREFIX:-/opt/moxie-unknown-elf}"
TARGET="moxie-unknown-elf"
GCC="$INSTALL_PREFIX/bin/$TARGET-gcc"

case "$INSTALL_PREFIX" in
    /|/usr|/usr/local|/usr/local/)
        echo "Refusing to install the toolchain into ${INSTALL_PREFIX}:" >&2
        echo "use a dedicated directory such as /opt/${TARGET} instead." >&2
        exit 1
        ;;
esac

if [ -x "$GCC" ]; then
    echo "moxie toolchain already installed: $GCC"
    exit 0
fi

mkdir -p "$INSTALL_PREFIX"
workdir="$(mktemp -d)"
trap 'rm -rf "$workdir"' EXIT

# --- fetch crosstool-NG -----------------------------------------------------
ctng_dir=""

download() {
    local url="$1" out="$2"
    echo "Downloading ${url}"
    curl -fsSL --retry 5 --retry-delay 5 --connect-timeout 20 --max-time 600 \
        -o "$out" "$url"
}

tarball="$workdir/crosstool-ng-${CTNG_VERSION}.tar.xz"
fetched=""
for url in \
    "https://github.com/crosstool-ng/crosstool-ng/releases/download/crosstool-ng-${CTNG_VERSION}/crosstool-ng-${CTNG_VERSION}.tar.xz" \
    "http://crosstool-ng.org/download/crosstool-ng/crosstool-ng-${CTNG_VERSION}.tar.xz"
do
    if download "$url" "$tarball"; then
        fetched="yes"
        break
    fi
done

if [ -n "$fetched" ]; then
    mkdir -p "$workdir/ctng"
    tar -xJf "$tarball" -C "$workdir/ctng" --strip-components=1
    ctng_dir="$workdir/ctng"
else
    echo "Release downloads failed, falling back to git" >&2
    for attempt in 1 2 3; do
        rm -rf "$workdir/ctng"
        if git clone --depth 1 --branch "crosstool-ng-${CTNG_VERSION}" \
            https://github.com/crosstool-ng/crosstool-ng.git "$workdir/ctng"; then
            ctng_dir="$workdir/ctng"
            break
        fi
        sleep $((attempt * 10))
    done
    if [ -z "$ctng_dir" ]; then
        echo "Failed to fetch crosstool-NG ${CTNG_VERSION}" >&2
        exit 1
    fi
fi

# --- build crosstool-NG -----------------------------------------------------
cd "$ctng_dir"
if [ ! -x ./configure ]; then
    # Git checkouts do not ship the generated configure script.
    ./bootstrap
fi
./configure --enable-local
make -j"$(nproc)"

# --- configure the moxie-unknown-elf toolchain ------------------------------
mkdir -p "$workdir/work"
cd "$workdir/work"

defconfig="$workdir/moxie-unknown-elf.defconfig"
local_tarballs="$workdir/local-tarballs"
mkdir -p "$local_tarballs"
cp "$ctng_dir/samples/moxie-unknown-elf/crosstool.config" "$defconfig"
# The sample defconfig leaves C++ disabled; append the override so Kconfig
# enables C++ (and libstdc++ for the target). Install into a dedicated prefix,
# never remove that prefix before building (CT_RM_RF_PREFIX_DIR=n, so a stale
# prefix cannot make crosstool-NG delete unrelated files), and keep the tree
# writable (do not "render the toolchain read-only").
printf '%s\n' 'CT_CC_LANG_CXX=y' 'CT_PREFIX_DIR_RO=n' 'CT_RM_RF_PREFIX_DIR=n' \
    "CT_PREFIX_DIR=\"${INSTALL_PREFIX}\"" \
    "CT_LOCAL_TARBALLS_DIR=\"${local_tarballs}\"" >> "$defconfig"
DEFCONFIG="$defconfig" "$ctng_dir/ct-ng" defconfig

if ! grep -q '^CT_CC_LANG_CXX=y$' .config; then
    echo "Failed to enable C++ for ${TARGET}" >&2
    exit 1
fi
if ! grep -Eq '^(CT_RM_RF_PREFIX_DIR=n|# CT_RM_RF_PREFIX_DIR is not set)$' .config; then
    echo "Failed to disable removal of the install prefix" >&2
    exit 1
fi
if ! grep -q "^CT_PREFIX_DIR=\"${INSTALL_PREFIX}\"$" .config; then
    echo "Failed to set the install prefix to ${INSTALL_PREFIX}" >&2
    exit 1
fi
if ! grep -q "^CT_LOCAL_TARBALLS_DIR=\"${local_tarballs}\"$" .config; then
    echo "Failed to set the local tarballs directory" >&2
    exit 1
fi

# --- prefetch and verify all component tarballs -----------------------------
# crosstool-NG downloads each component from a list of mirrors and aborts on
# the first digest mismatch without trying the next mirror. Some mirrors
# (e.g. invisible-mirror.net for ncurses) answer with a challenge page instead
# of the tarball. Download every tarball ourselves, verify it against the
# SHA512 in crosstool-NG's packages/<pkg>/<version>/chksum, and put it into the
# local tarballs directory so the build uses verified local copies.
config_value() {
    sed -n "s/^$1=\"\\(.*\\)\"$/\\1/p" .config | head -n 1
}

pkg_mirrors() {
    local pkg="$1"
    (
        set +eu
        CT_Mirrors() {
            case "$1" in
                GNU)
                    printf 'https://ftpmirror.gnu.org/gnu/%s\n' "$2"
                    printf 'https://ftp.gnu.org/gnu/%s\n' "$2"
                    ;;
                sourceware)
                    printf 'https://sourceware.org/%s/ftp\n' "$2"
                    printf 'https://mirrors.kernel.org/sourceware/%s\n' "$2"
                    ;;
                *)
                    :
                    ;;
            esac
        }
        # shellcheck disable=SC1091
        . "$PWD/.config"
        eval "printf '%s\n' \"\${CT_${pkg}_MIRRORS}\""
    )
}

prefetch_component_tarballs() {
    local pkg version pkg_name template formats ext basename filename digest
    local chksum_file tmp url actual
    for pkg in $(sed -n 's/^CT_\([A-Z0-9_]*\)_ARCHIVE_FILENAME=.*/\1/p' .config); do
        version="$(config_value "CT_${pkg}_VERSION")"
        pkg_name="$(config_value "CT_${pkg}_PKG_NAME")"
        template="$(config_value "CT_${pkg}_ARCHIVE_FILENAME")"
        formats="$(config_value "CT_${pkg}_ARCHIVE_FORMATS")"
        if [ -z "${version}" ] || [ -z "${pkg_name}" ] || [ -z "${template}" ]; then
            continue
        fi
        chksum_file="$ctng_dir/packages/$(printf '%s' "${pkg}" | tr 'A-Z' 'a-z')/${version}/chksum"
        if [ ! -r "${chksum_file}" ]; then
            continue
        fi
        basename="${template//@\{pkg_name\}/${pkg_name}}"
        basename="${basename//@\{version\}/${version}}"
        filename=""
        for ext in ${formats}; do
            # lzip is not installed by the CI, so skip the .tar.lz format.
            if [ "${ext}" = ".tar.lz" ] && ! command -v lzip >/dev/null 2>&1; then
                continue
            fi
            if grep -q "^sha512 ${basename}${ext} " "${chksum_file}"; then
                filename="${basename}${ext}"
                break
            fi
        done
        if [ -z "${filename}" ] || [ -f "${local_tarballs}/${filename}" ]; then
            continue
        fi
        digest="$(awk -v f="${filename}" '$1 == "sha512" && $2 == f { print $3 }' "${chksum_file}")"
        if [ -z "${digest}" ]; then
            continue
        fi
        echo "Prefetching ${filename}"
        tmp="${local_tarballs}/${filename}.part"
        for url in $(pkg_mirrors "${pkg}"); do
            rm -f "${tmp}"
            if ! curl -fsSL --retry 3 --retry-delay 5 --connect-timeout 20 --max-time 900 \
                -o "${tmp}" "${url}/${filename}"; then
                continue
            fi
            actual="$(sha512sum "${tmp}" | awk '{ print $1 }')"
            if [ "${actual}" = "${digest}" ]; then
                mv "${tmp}" "${local_tarballs}/${filename}"
                break
            fi
            echo "Bad SHA512 for ${url}/${filename}; trying the next mirror" >&2
            rm -f "${tmp}"
        done
        if [ ! -f "${local_tarballs}/${filename}" ]; then
            # Let crosstool-NG try; it may know a mirror that is not listed here.
            echo "WARNING: failed to prefetch ${filename}; crosstool-NG will download it" >&2
        fi
    done
}

prefetch_component_tarballs

# --- build the toolchain ----------------------------------------------------
attempt=1
while [ "$attempt" -le 3 ]; do
    echo "Building ${TARGET}: attempt ${attempt} of 3"
    if "$ctng_dir/ct-ng" "build.$(nproc)"; then
        break
    fi
    if [ "$attempt" -eq 3 ]; then
        echo "Failed to build ${TARGET} after 3 attempts" >&2
        exit 1
    fi
    attempt=$((attempt + 1))
    sleep $((attempt * 30))
done

# --- verify -----------------------------------------------------------------
"$GCC" --version
