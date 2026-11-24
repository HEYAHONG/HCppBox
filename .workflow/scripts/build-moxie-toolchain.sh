#!/usr/bin/env bash
# Build the moxie-unknown-elf cross toolchain with crosstool-NG.
#
# The crosstool-NG sample "moxie-unknown-elf" does not enable C++ by default,
# so this script loads the sample and appends CT_CC_LANG_CXX=y before building.
# The toolchain build is retried up to 3 times: crosstool-NG downloads the
# component tarballs (binutils, gcc, newlib, gdb, ...) during the build and a
# transient network failure should not abort the whole CI job.
#
# The toolchain is installed system-wide into ${MOXIE_TOOLCHAIN_PREFIX:-/usr/local}
# (so moxie-unknown-elf-gcc lands in /usr/local/bin, which is already on PATH).
set -euo pipefail

# crosstool-NG refuses to build when running as root unless this override is
# set. All CI jobs run in ephemeral containers (or VMs) as root, so allow it.
export CT_ALLOW_BUILD_AS_ROOT_SURE=1

CTNG_VERSION="${CTNG_VERSION:-1.29.0}"
INSTALL_PREFIX="${MOXIE_TOOLCHAIN_PREFIX:-/usr/local}"
TARGET="moxie-unknown-elf"
GCC="$INSTALL_PREFIX/bin/$TARGET-gcc"

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
cp "$ctng_dir/samples/moxie-unknown-elf/crosstool.config" "$defconfig"
# The sample defconfig leaves C++ disabled; append the override so Kconfig
# enables C++ (and libstdc++ for the target). Install system-wide so the
# toolchain binaries are placed in /usr/local/bin.
printf '%s\n' 'CT_CC_LANG_CXX=y' "CT_PREFIX_DIR=\"${INSTALL_PREFIX}\"" >> "$defconfig"
DEFCONFIG="$defconfig" "$ctng_dir/ct-ng" defconfig

if ! grep -q '^CT_CC_LANG_CXX=y$' .config; then
    echo "Failed to enable C++ for ${TARGET}" >&2
    exit 1
fi
if ! grep -q "^CT_PREFIX_DIR=\"${INSTALL_PREFIX}\"$" .config; then
    echo "Failed to set the install prefix to ${INSTALL_PREFIX}" >&2
    exit 1
fi

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
