#!/usr/bin/env bash
# Install the Android SDK packages needed by the test/Android projects.
#
# This file is meant to be *sourced* by the CI job:
#     source .workflow/scripts/setup-android-sdk.sh
# so that ANDROID_SDK_ROOT, ANDROID_HOME, JAVA_HOME and PATH stay exported for
# the subsequent build steps. When GITHUB_ENV/GITHUB_PATH are available (GitHub
# Actions, Gitea Actions) they are updated as well.
set -euo pipefail

: "${ANDROID_COMPILE_SDK:=34}"
: "${ANDROID_BUILD_TOOLS:=34.0.0}"
: "${ANDROID_NDK:=26.1.10909125}"
: "${ANDROID_CMAKE:=3.31.1}"

# --- JDK --------------------------------------------------------------------
if [ -z "${JAVA_HOME:-}" ] || [ ! -x "${JAVA_HOME}/bin/java" ]; then
    for candidate in /usr/lib/jvm/java-17-openjdk-* /usr/lib/jvm/java-17-*; do
        if [ -x "${candidate}/bin/java" ]; then
            JAVA_HOME="${candidate}"
            export JAVA_HOME
            break
        fi
    done
fi
if [ -n "${JAVA_HOME:-}" ]; then
    export PATH="${JAVA_HOME}/bin:${PATH}"
fi
if ! command -v java >/dev/null 2>&1; then
    echo "java not found; install openjdk-17-jdk-headless before sourcing this script" >&2
    return 1
fi

# --- Android SDK ------------------------------------------------------------
sdk_root=""
for candidate in "${ANDROID_SDK_ROOT:-}" "${ANDROID_HOME:-}" "$HOME/Android/Sdk" "$HOME/android-sdk"; do
    if [ -n "${candidate}" ] && [ -w "${candidate}" ] \
        && [ -x "${candidate}/cmdline-tools/latest/bin/sdkmanager" ]; then
        sdk_root="${candidate}"
        break
    fi
done

if [ -z "${sdk_root}" ]; then
    sdk_root="${ANDROID_SDK_ROOT:-$HOME/android-sdk}"
    mkdir -p "${sdk_root}/cmdline-tools"
    if [ ! -x "${sdk_root}/cmdline-tools/latest/bin/sdkmanager" ]; then
        archive="${sdk_root}/commandlinetools-linux.zip"
        downloaded=0
        for url in \
            "https://dl.google.com/android/repository/commandlinetools-linux-11076708_latest.zip" \
            "https://dl.google.com/android/repository/commandlinetools-linux-13114758_latest.zip"
        do
            echo "Downloading ${url}"
            if curl -fsSL --retry 5 --retry-delay 5 --connect-timeout 20 --max-time 600 \
                -o "${archive}" "${url}"; then
                downloaded=1
                break
            fi
        done
        if [ "${downloaded}" -ne 1 ]; then
            echo "Failed to download the Android command-line tools" >&2
            return 1
        fi
        rm -rf "${sdk_root}/cmdline-tools/latest" "${sdk_root}/cmdline-tools/cmdline-tools"
        unzip -q "${archive}" -d "${sdk_root}/cmdline-tools"
        mv "${sdk_root}/cmdline-tools/cmdline-tools" "${sdk_root}/cmdline-tools/latest"
        rm -f "${archive}"
    fi
fi

export ANDROID_SDK_ROOT="${sdk_root}"
export ANDROID_HOME="${sdk_root}"
export PATH="${sdk_root}/cmdline-tools/latest/bin:${sdk_root}/platform-tools:${PATH}"

# --- packages ---------------------------------------------------------------
yes | sdkmanager --licenses >/dev/null 2>&1 || true
sdkmanager --install \
    "platform-tools" \
    "platforms;android-${ANDROID_COMPILE_SDK}" \
    "build-tools;${ANDROID_BUILD_TOOLS}" \
    "ndk;${ANDROID_NDK}" \
    "cmake;${ANDROID_CMAKE}"

# Persist the environment for later CI steps where supported.
if [ -n "${GITHUB_ENV:-}" ]; then
    {
        echo "ANDROID_SDK_ROOT=${ANDROID_SDK_ROOT}"
        echo "ANDROID_HOME=${ANDROID_HOME}"
        if [ -n "${JAVA_HOME:-}" ]; then
            echo "JAVA_HOME=${JAVA_HOME}"
        fi
    } >> "${GITHUB_ENV}"
    echo "${ANDROID_SDK_ROOT}/cmdline-tools/latest/bin" >> "${GITHUB_PATH}"
    echo "${ANDROID_SDK_ROOT}/platform-tools" >> "${GITHUB_PATH}"
    if [ -n "${JAVA_HOME:-}" ]; then
        echo "${JAVA_HOME}/bin" >> "${GITHUB_PATH}"
    fi
fi

echo "Android SDK: ${ANDROID_SDK_ROOT}"
echo "Android NDK: ${ANDROID_NDK}"
echo "Android CMake: ${ANDROID_CMAKE}"
