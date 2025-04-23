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


pushd "${script_dir}"

#uint128为基础模板

##uint256
cp huint128.h huint256.h
cp huint128.c huint256.c
sed -i "s/128/256/g" huint256.*

##uint512
cp huint128.h huint512.h
cp huint128.c huint512.c
sed -i "s/128/512/g" huint512.*

##uint1024
cp huint128.h huint1024.h
cp huint128.c huint1024.c
sed -i "s/128/1024/g" huint1024.*


##uint2048
cp huint128.h huint2048.h
cp huint128.c huint2048.c
sed -i "s/128/2048/g" huint2048.*

##uint4096
cp huint128.h huint4096.h
cp huint128.c huint4096.c
sed -i "s/128/4096/g" huint4096.*

##uint8192
cp huint128.h huint8192.h
cp huint128.c huint8192.c
sed -i "s/128/8192/g" huint8192.*


popd
