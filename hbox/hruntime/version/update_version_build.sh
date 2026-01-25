#!/bin/bash

#检查工具是否存在,$1为待检查的工具名。
function CheckTool
{
	[  -n "$1"  ] ||
	{
		echo -e  "\033[41;37mCheckTool 参数错误!!\r\n\033[40;37m";
		return 255;
	};
	ToolPath=`which $1`;
	[ -e "$ToolPath" ] ||
	{
		 echo -e "\033[41;37m$1 不存在，请先安装此工具\r\n\033[40;37m";
		 return 255;
	};
	return 0;
}

CheckTool git
[ $? -eq 0 ] || exit;
CheckTool mkdir
[ $? -eq 0 ] || exit;
CheckTool sed
[ $? -eq 0 ] || exit;
CheckTool awk
[ $? -eq 0 ] || exit;
CheckTool grep
[ $? -eq 0 ] || exit;
CheckTool realpath
[ $? -eq 0 ] || exit;
CheckTool md5sum
[ $? -eq 0 ] || exit;

#获取当前目录
slef_path=
# shellcheck disable=SC2128  # ignore array expansion warning
if [ -n "${BASH_SOURCE-}" ]
then
        self_path="${BASH_SOURCE}"
elif [ -n "${ZSH_VERSION-}" ]
then
        self_path="${(%):-%x}"
else
        return 1
fi
# shellcheck disable=SC2169,SC2169,SC2039  # unreachable with 'dash'
if [[ "$OSTYPE" == "darwin"* ]]; then
        # convert possibly relative path to absolute
        script_dir="$(realpath_int "${self_path}")"
        # resolve any ../ references to make the path shorter
        script_dir="$(cd "${script_dir}" || exit 1; pwd)"
else
        # convert to full path and get the directory name of that
        script_name="$(readlink -f "${self_path}")"
        script_dir="$(dirname "${script_name}")"
fi
echo "此工具仅支持在Linux运行，其它系统下运行有可能出错。"
pushd "${script_dir}" 2>/dev/null 1>/dev/null

md5sum -c build_hash 2>/dev/null 1>/dev/null
if [ $? -ne 0 ]
then
    echo build version update!
    old_build_version=`cat build`
    echo $[${old_build_version}+1] > build
    md5sum ../symbol/*.c > build_hash
fi
echo build version `cat build`
 
popd 2>/dev/null 1>/dev/null

