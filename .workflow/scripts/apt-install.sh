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

export DEBIAN_FRONTEND=noninteractive

$SUDO apt-get update
if ! $SUDO apt-get install -y -f --no-install-recommends "$@"; then
    if [ "${APT_OPTIONAL:-0}" = "1" ]; then
        echo "可选软件包安装失败，已跳过：$*" >&2
        exit 0
    fi
    exit 1
fi
