#!/bin/sh
# 统一 SCons 构建的 PIE 设置，避免 GCC 与链接器默认配置不一致。
# 编译时加 -fno-pie，链接时加 -no-pie；构建共享库时保持原样。
set -eu

real="$1"
shift

shared=0
compile=0
for arg in "$@"; do
    case "$arg" in
        -shared)
            shared=1
            ;;
        -c|-S|-E)
            compile=1
            ;;
    esac
done

if [ "$shared" -eq 1 ]; then
    exec "$real" "$@"
fi

if [ "$compile" -eq 1 ]; then
    exec "$real" -fno-pie "$@"
fi

exec "$real" -no-pie "$@"
