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
CheckTool rsync
[ $? -eq 0 ] || exit;
CheckTool file
[ $? -eq 0 ] || exit;
CheckTool iconv
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

echo 当前目录为${script_dir}.

if [ -d "${script_dir}/simplegui" ]
then
	pushd "${script_dir}/simplegui"
	git pull
	popd
else
	git clone  https://gitee.com/Polarix/simplegui "${script_dir}/simplegui"
fi


if [ -f "${script_dir}/simplegui/LICENSE" ]
then
	rsync -rl --progress  "${script_dir}/simplegui/LICENSE" "${script_dir}/"
	rsync -rl --progress  --include="*.h" --include="*.c" "${script_dir}/simplegui/GUI/inc/" "${script_dir}/"
	rsync -rl --progress  --include="*.h" --include="*.c" "${script_dir}/simplegui/GUI/src/" "${script_dir}/"
	rsync -rl --progress  --include="*.h" --include="*.c" "${script_dir}/simplegui/HMI/inc/" "${script_dir}/"
	rsync -rl --progress  --include="*.h" --include="*.c" "${script_dir}/simplegui/HMI/src/" "${script_dir}/"
	for c_file in `ls *.c`
	do
		cp "simplegui_template_c" "../simplegui_${c_file}"
		sed -i "s/simplegui_filename/${c_file}/g"  "../simplegui_${c_file}"
		iconv -f gbk -t utf8 ${c_file} > ${c_file}.utf8
		mv ${c_file}.utf8 ${c_file}
	done
	sed -i '/\#include <windows.h>/d' SGUI_Interface.c
	sed -i '/\#include "RTC.h"/d' SGUI_Interface.c
	sed -i 's/\#define _SIMPLE_GUI_IN_VIRTUAL_SDK_/\/\/\#define _SIMPLE_GUI_IN_VIRTUAL_SDK_/g' SGUI_Config.h
fi
