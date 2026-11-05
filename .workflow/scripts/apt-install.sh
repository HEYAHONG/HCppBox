#!/usr/bin/env bash
# 在 Gitee Go 构建环境中安装 APT 软件包。
# 构建容器可能以 root 或普通用户运行，这里自动判断是否需要 sudo。
set -euo pipefail

if [ "$#" -eq 0 ]; then
    exit 0
fi

if [ "$(id -u)" -eq 0 ]; then
    SUDO=""
else
    SUDO="sudo"
fi

# 将 Ubuntu APT 源替换为中科大镜像，加速国内构建环境下载。
# 兼容旧式 /etc/apt/sources.list 与 Ubuntu 24.04+ 的 deb822(ubuntu.sources)。
ubuntu_mirror="http://mirrors.ustc.edu.cn"

rewrite_ubuntu_mirror() {
    if [ ! -r /etc/os-release ]; then
        return 0
    fi
    # shellcheck disable=SC1091
    . /etc/os-release
    if [ "${ID:-}" != "ubuntu" ]; then
        return 0
    fi

    for file in /etc/apt/sources.list /etc/apt/sources.list.d/*.list /etc/apt/sources.list.d/*.sources; do
        [ -f "$file" ] || continue
        sed -i -E \
            -e "s#https?://[^[:space:]]*ubuntu\.com/ubuntu-ports/?#${ubuntu_mirror}/ubuntu-ports/#g" \
            -e "s#https?://[^[:space:]]*ubuntu\.com/ubuntu/?#${ubuntu_mirror}/ubuntu/#g" \
            "$file"
    done
}

rewrite_ubuntu_mirror

export DEBIAN_FRONTEND=noninteractive

$SUDO apt-get update
if ! $SUDO apt-get install -y -f --no-install-recommends "$@"; then
    if [ "${APT_OPTIONAL:-0}" = "1" ]; then
        echo "可选软件包安装失败，已跳过：$*" >&2
        exit 0
    fi
    exit 1
fi
