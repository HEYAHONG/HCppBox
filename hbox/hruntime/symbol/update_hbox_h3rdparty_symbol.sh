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
CheckTool bc
[ $? -eq 0 ] || exit;
CheckTool cmake
[ $? -eq 0 ] || exit;
CheckTool gcc
[ $? -eq 0 ] || exit;
CheckTool objdump
[ $? -eq 0 ] || exit;
CheckTool realpath
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
pushd "${script_dir}"

filename="hbox_h3rdparty_symbol.c"

#初始化文件
echo "#include \"hbox.h\"" > ${filename}
echo "static const hruntime_symbol_t hruntime_hbox_h3rdparty_symbol[]=" >>  ${filename}
echo "{" >>  ${filename}

#扫描hbox目录
HBOX_ROOT=`realpath ${script_dir}/../../`
for i in `ls ${script_dir}/../../h3rdparty/*.c`
do
    echo $i
    FILE_PATH=`realpath --relative-base="${HBOX_ROOT}" ${i}`
    echo "//${FILE_PATH}" >> ${filename}
    gcc -c -DHRUNTIME_SYMBOL_SCAN -I${script_dir}/../../cpp  -I${script_dir}/../../ $i -o $i.o
    for symbol in `objdump -t $i.o | grep -v "\.local" | grep -v "*UND*" | grep -v "*ABS*"  | awk '{if( $2 == "g") { print $0}}' | awk '{ print $6 }'`
    do
       cat << EOF > testsymbol.c
#include "hbox.h"
void * testsymbol(void)
{
    return &${symbol};
}
EOF
       gcc -c testsymbol.c -I${script_dir}/../../ -I${script_dir}/../../cpp/  -o testsymbol.o 2>/dev/null >/dev/null
       if [ "$?" -eq 0 ]
       then
           echo "{hdefaults_str(${symbol}),(uintptr_t)&${symbol}}," >> ${filename}
       fi
       rm testsymbol.*
    done
    rm $i.o
done

#文件结束
echo "};" >>  ${filename}

#使用astyle整理文件
astyle -n ${filename} 2> /dev/null >/dev/null

popd
