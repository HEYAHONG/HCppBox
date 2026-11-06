#!/usr/bin/env bash
# 下载 Gitee 上 testing 发行版的现有注记，作为临时 release body。
# release@gitee 插件要求 body 非空，但通常不希望每次发布都改写发行注记，
# 因此先下载现有注记，再原样交给插件。
# 若发行版不存在或注记为空，则写入默认注记，保证插件可以正常执行。
set -euo pipefail

tag="${RELEASE_TAG:-testing}"
out="${1:-release-body.txt}"
api="${GITEE_API_URL:-https://gitee.com/api/v5}"

owner="${GITEE_OWNER:-}"
repo="${GITEE_REPO:-}"

# GITEE_REPO 可能为 owner/repo 形式，也可能只是仓库名。
case "$repo" in
    */*)
        owner="${repo%%/*}"
        repo="${repo##*/}"
        ;;
esac

if [ -z "$owner" ] || [ -z "$repo" ]; then
    remote="$(git config --get remote.origin.url 2>/dev/null || true)"
    remote="${remote%.git}"
    remote="${remote%/}"
    if [ -n "$remote" ]; then
        repo="${remote##*/}"
        owner="${remote%/*}"
        owner="${owner##*[:/]}"
    fi
fi

if [ -z "$owner" ] || [ -z "$repo" ]; then
    echo "无法确定仓库地址，请设置 GITEE_OWNER 和 GITEE_REPO" >&2
    exit 1
fi

tmp="$(mktemp)"
trap 'rm -f "$tmp"' EXIT

if curl -fsS "$api/repos/$owner/$repo/releases/tags/$tag" -o "$tmp"; then
    python3 - "$tmp" "$out" <<'PY'
import json
import sys

with open(sys.argv[1], encoding='utf-8') as f:
    data = json.load(f)
body = data.get('body') or ''
with open(sys.argv[2], 'w', encoding='utf-8') as f:
    f.write(body)
PY
else
    echo "未找到 Release '$tag'，使用默认注记" >&2
    : > "$out"
fi

# release@gitee 要求 release body 非空。
if [ ! -s "$out" ]; then
    printf 'Latest snapshot builds\n' > "$out"
fi
