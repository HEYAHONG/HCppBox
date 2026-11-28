#!/usr/bin/env bash
# Build every Android project under test/Android.
#
# A subdirectory is considered an Android project when it contains a Gradle
# settings file, a Gradle build file and the Gradle wrapper. Each project is
# built with ABI splits for the common Android architectures and the resulting
# APKs are copied into ./dist as:
#
#   <project>-<abi>-snapshot-debug.apk
#   <project>-<abi>-snapshot-release.apk
#
# Unsigned release APKs are signed with the local Android debug keystore.
#
# This file is meant to be *sourced* after setup-android-sdk.sh:
#     source .workflow/scripts/build-android-projects.sh
set -euo pipefail

script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
repo_root="$(cd "${script_dir}/../.." && pwd)"
android_dir="${repo_root}/test/Android"
dist_dir="${repo_root}/dist"
abis="${ANDROID_ABIS:-armeabi-v7a arm64-v8a x86 x86_64}"

: "${ANDROID_SDK_ROOT:?ANDROID_SDK_ROOT is not set; source setup-android-sdk.sh first}"
: "${ANDROID_BUILD_TOOLS:=34.0.0}"

mkdir -p "${dist_dir}"
rm -f "${dist_dir}"/*.apk

work_dir="$(mktemp -d)"
trap 'rm -rf "${work_dir}"' EXIT

abi_include=""
for abi in ${abis}; do
    abi_include="${abi_include} '${abi}',"
done
abi_include="${abi_include%,}"

cat > "${work_dir}/abi-splits.gradle" <<GRADLE
gradle.projectsLoaded {
    gradle.rootProject.allprojects { project ->
        project.plugins.withId('com.android.application') {
            project.android.splits.abi {
                enable true
                reset()
                include ${abi_include}
                universalApk false
            }
        }
    }
}
GRADLE

# A debug keystore is used to sign the snapshot release APKs. Gradle creates
# this keystore while building the debug variants; create it explicitly so the
# signing step does not depend on that.
keystore="${HOME}/.android/debug.keystore"
if [ ! -f "${keystore}" ]; then
    mkdir -p "$(dirname "${keystore}")"
    keytool -genkeypair -keystore "${keystore}" \
        -storepass android -keypass android \
        -alias androiddebugkey \
        -dname "CN=Android Debug,O=Android,C=US" \
        -keyalg RSA -keysize 2048 -validity 10000 >/dev/null 2>&1
fi
apksigner="${ANDROID_SDK_ROOT}/build-tools/${ANDROID_BUILD_TOOLS}/apksigner"

project_count=0
apk_count=0

for project_dir in "${android_dir}"/*/; do
    project_dir="${project_dir%/}"
    project_name="$(basename "${project_dir}")"

    # An Android project has a Gradle settings file, a build file and the
    # Gradle wrapper.
    if [ ! -f "${project_dir}/settings.gradle" ] && [ ! -f "${project_dir}/settings.gradle.kts" ]; then
        continue
    fi
    if [ ! -f "${project_dir}/build.gradle" ] && [ ! -f "${project_dir}/build.gradle.kts" ]; then
        continue
    fi
    if [ ! -f "${project_dir}/gradlew" ]; then
        continue
    fi

    project_count=$((project_count + 1))
    echo "== Building Android project: ${project_name}"
    chmod +x "${project_dir}/gradlew"

    built=0
    for attempt in 1 2 3; do
        echo "Gradle build attempt ${attempt} of 3"
        if (cd "${project_dir}" && ./gradlew --no-daemon \
            --init-script "${work_dir}/abi-splits.gradle" \
            assembleDebug assembleRelease); then
            built=1
            break
        fi
        sleep $((attempt * 20))
    done
    if [ "${built}" -ne 1 ]; then
        echo "Failed to build Android project ${project_name}" >&2
        exit 1
    fi

    for abi in ${abis}; do
        debug_apk="$(find "${project_dir}" -type f \
            -path "*/build/outputs/apk/debug/*-${abi}-debug.apk" -print -quit)"
        release_apk="$(find "${project_dir}" -type f \
            -path "*/build/outputs/apk/release/*-${abi}-release.apk" -print -quit)"
        unsigned_apk="$(find "${project_dir}" -type f \
            -path "*/build/outputs/apk/release/*-${abi}-release-unsigned.apk" -print -quit)"

        if [ -n "${debug_apk}" ]; then
            cp "${debug_apk}" "${dist_dir}/${project_name}-${abi}-snapshot-debug.apk"
            apk_count=$((apk_count + 1))
        fi

        if [ -n "${release_apk}" ]; then
            cp "${release_apk}" "${dist_dir}/${project_name}-${abi}-snapshot-release.apk"
            apk_count=$((apk_count + 1))
        elif [ -n "${unsigned_apk}" ]; then
            signed_apk="${work_dir}/${project_name}-${abi}-release.apk"
            "${apksigner}" sign --ks "${keystore}" \
                --ks-pass pass:android --key-pass pass:android \
                --out "${signed_apk}" "${unsigned_apk}"
            mv "${signed_apk}" "${dist_dir}/${project_name}-${abi}-snapshot-release.apk"
            apk_count=$((apk_count + 1))
        fi
    done
done

if [ "${project_count}" -eq 0 ]; then
    echo "No Android projects found under ${android_dir}" >&2
    exit 1
fi
if [ "${apk_count}" -eq 0 ]; then
    echo "No APKs were produced" >&2
    exit 1
fi

echo "Built ${apk_count} APK(s):"
ls -l "${dist_dir}"
