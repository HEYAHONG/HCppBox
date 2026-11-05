#!/usr/bin/env bash
# 将构建产物上传到 Gitee 发行版(Release)的指定标签，行为与 Gitea 工作流一致：
# 同名附件会被覆盖，Release 不存在时自动创建。
#
# 需要配置流水线变量：
#   GITEE_TOKEN  拥有仓库写权限的 Gitee 私人令牌
# 可选变量：
#   GITEE_OWNER  仓库所有者；默认从 git remote 解析
#   GITEE_REPO   仓库名；默认从 git remote 解析
#   RELEASE_TAG  Release 标签；默认 testing
set -euo pipefail

asset="${1:-}"
if [ -z "$asset" ] || [ ! -f "$asset" ]; then
    echo "用法: $0 <要上传的制品文件>" >&2
    exit 2
fi

tag="${RELEASE_TAG:-testing}"
api="${GITEE_API_URL:-https://gitee.com/api/v5}"
: "${GITEE_TOKEN:?请先在 Gitee Go 流水线变量中配置 GITEE_TOKEN（拥有仓库写权限的私人令牌）}"

repo="${GITEE_REPO:-}"
owner="${GITEE_OWNER:-}"

if [ -z "$owner" ] || [ -z "$repo" ]; then
    remote="$(git config --get remote.origin.url 2>/dev/null || true)"
    remote="${remote%.git}"
    remote="${remote%/}"
    if [ -n "$remote" ]; then
        parsed_repo="${remote##*/}"
        parsed_owner="${remote%/*}"
        parsed_owner="${parsed_owner##*[:/]}"
        owner="${owner:-$parsed_owner}"
        repo="${repo:-$parsed_repo}"
    fi
fi

# GITEE_REPO 有可能已经是 owner/repo 形式。
case "$repo" in
    */*)
        owner="${repo%%/*}"
        repo="${repo##*/}"
        ;;
esac

if [ -z "$owner" ] || [ -z "$repo" ]; then
    echo "无法确定仓库地址，请设置 GITEE_OWNER 和 GITEE_REPO" >&2
    exit 1
fi

release_url="$api/repos/$owner/$repo/releases"

get_release_id() {
    curl -fsS --get \
        --data-urlencode "access_token=$GITEE_TOKEN" \
        "$release_url/tags/$tag" 2>/dev/null \
        | jq -r '.id // empty'
}

release_id="$(get_release_id || true)"
if [ -z "$release_id" ]; then
    echo "Release '$tag' 不存在，正在创建"
    if ! curl -fsS --request POST \
        --data-urlencode "access_token=$GITEE_TOKEN" \
        --data-urlencode "tag_name=$tag" \
        --data-urlencode "name=$tag" \
        --data-urlencode "body=Latest snapshot builds" \
        --data-urlencode "target_commitish=$(git rev-parse HEAD)" \
        --data-urlencode "prerelease=true" \
        "$release_url" >/dev/null; then
        # 多个流水线可能同时创建同名 Release，等待后重新查询。
        sleep 2
    fi
    release_id="$(get_release_id || true)"
fi

if [ -z "$release_id" ]; then
    echo "无法创建或获取 Release '$tag'" >&2
    exit 1
fi

asset_name="$(basename "$asset")"
attach_url="$release_url/$release_id/attach_files"

existing_ids="$(
    curl -fsS --get \
        --data-urlencode "access_token=$GITEE_TOKEN" \
        --data-urlencode "per_page=100" \
        "$attach_url" \
        | jq -r --arg name "$asset_name" '.[] | select(.name == $name) | .id'
)"

for id in $existing_ids; do
    echo "删除旧附件 ${asset_name} (${id})"
    curl -fsS --request DELETE --get \
        --data-urlencode "access_token=$GITEE_TOKEN" \
        "$attach_url/$id" >/dev/null
done

echo "上传 ${asset_name} 到 Release '${tag}'"
curl -fsS --request POST \
    --form "access_token=$GITEE_TOKEN" \
    --form "file=@$asset" \
    "$attach_url" >/dev/null

echo "已上传 ${asset_name}"
