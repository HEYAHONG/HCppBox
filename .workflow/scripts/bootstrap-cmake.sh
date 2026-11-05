#!/usr/bin/env bash
# 确保 CMake 版本满足项目要求。
#
# Gitee Go 的 build@gcc 构建镜像当前基于 Ubuntu 22.04(jammy)，自带 CMake
# 3.22，仍低于本项目根 CMakeLists.txt 要求的 3.25。仅在版本不足时添加
# Kitware 官方 APT 源安装新版 CMake，避免无必要的第三方依赖。
set -euo pipefail

required="${CMAKE_MIN_VERSION:-3.25}"
# 固定使用 CMake 3.31.x，避免 CMake 4 对旧版 cmake_minimum_required 的兼容性变化。
pin_series="${CMAKE_PIN_SERIES:-3.31}"

if [ "$(id -u)" -eq 0 ]; then
    SUDO=""
else
    SUDO="sudo"
fi

cmake_version() {
    cmake --version 2>/dev/null | awk 'NR == 1 { print $3 }'
}

version_ge() {
    [ "$(printf '%s\n%s\n' "$1" "$2" | sort -V | head -n 1)" = "$1" ]
}

if command -v cmake >/dev/null 2>&1 && version_ge "$required" "$(cmake_version)"; then
    echo "CMake $(cmake_version) >= ${required}，无需升级"
    exit 0
fi

export DEBIAN_FRONTEND=noninteractive

$SUDO apt-get update
$SUDO apt-get install -y --no-install-recommends ca-certificates curl gnupg

. /etc/os-release
codename="${UBUNTU_CODENAME:-${VERSION_CODENAME:-jammy}}"

curl -fsSL https://apt.kitware.com/keys/kitware-archive-latest.asc \
    | gpg --dearmor \
    | $SUDO tee /usr/share/keyrings/kitware-archive-keyring.gpg >/dev/null

echo "deb [signed-by=/usr/share/keyrings/kitware-archive-keyring.gpg] https://apt.kitware.com/ubuntu/ ${codename} main" \
    | $SUDO tee /etc/apt/sources.list.d/kitware.list >/dev/null

$SUDO apt-get update

pin_version="$($SUDO apt-cache madison cmake | awk '{ print $3 }' | grep "^${pin_series}\\." | sort -V | tail -n 1 || true)"
if [ -n "$pin_version" ]; then
    # cmake 依赖同版本的 cmake-data，而 APT 默认只会为依赖选择候选版本，
    # 因此这里同时显式指定两个包的版本。
    $SUDO apt-get install -y --no-install-recommends "cmake=${pin_version}" "cmake-data=${pin_version}"
else
    $SUDO apt-get install -y --no-install-recommends cmake
fi

installed="$(cmake_version)"
if ! version_ge "$required" "$installed"; then
    echo "CMake 升级失败：当前 ${installed}，要求 >= ${required}" >&2
    exit 1
fi

echo "CMake ${installed} 安装完成"
