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
CheckTool tr
[ $? -eq 0 ] || exit;
CheckTool sort
[ $? -eq 0 ] || exit;
CheckTool uniq
[ $? -eq 0 ] || exit;
CheckTool cat
[ $? -eq 0 ] || exit;
CheckTool mv
[ $? -eq 0 ] || exit;
CheckTool bc
[ $? -eq 0 ] || exit;
CheckTool cmake
[ $? -eq 0 ] || exit;
CheckTool gcc
[ $? -eq 0 ] || exit;
CheckTool objdump
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
echo "此工具仅支持在Ubuntu运行，其它系统下运行有可能出错。"
pushd "${script_dir}"

filename="libc_symbol.c"

#初始化文件
echo "#include \"hbox.h\"" > ${filename}
echo "#include \"stdlib.h\"" >> ${filename}
echo "#include \"stdio.h\"" >> ${filename}
echo "#include \"string.h\"" >> ${filename}
echo "#include \"setjmp.h\"" >> ${filename}
echo "#include \"ctype.h\"" >> ${filename}
echo "#include \"time.h\"" >> ${filename}
echo "static const hruntime_symbol_t hruntime_libc_symbol[]=" >>  ${filename}
echo "{" >>  ${filename}

symbol=""

#扫描hbox目录
for i in `ls ${script_dir}/../../*.c`
do
	echo ${i}
    gcc -c -DHRUNTIME_SYMBOL_SCAN -I${script_dir}/../../cpp  -I${script_dir}/../../ $i -o $i.o
    symbol=" `objdump -t $i.o | grep -v "\.local" | grep  "*UND*" | grep -v "*ABS*" | grep -v "__" |  awk '{ print $4 }'` ${symbol}"
    rm $i.o
done

#扫描hbox_h3rdparty目录
for i in `ls ${script_dir}/../../h3rdparty/*.c`
do
	echo ${i}
    gcc -c -DHRUNTIME_SYMBOL_SCAN -I${script_dir}/../../cpp  -I${script_dir}/../../ $i -o $i.o
    symbol=" `objdump -t $i.o | grep -v "\.local" | grep  "*UND*" | grep -v "*ABS*" | grep -v "__" |  awk '{ print $4 }'` ${symbol}"
    rm $i.o
done

#查找libc库
libc=`ldd /bin/sh | grep libc | awk  '{ print $3 }'`

echo C运行库为${libc}

#查找C库中的符号
libc_symbol=""
if [ -f ${libc} ]
then
    for i in `objdump -T ${libc} | awk '{ print $7 }'`
    do
      if [ -n "`echo ${symbol} | grep -w $i`" ]
      then
        libc_symbol="$i $libc_symbol"
      fi
    done
fi

#过滤掉某些C库函数
symbol="${libc_symbol}"
libc_symbol=""
##去掉带下划线的C运行库函数
for i in `echo ${symbol}`
do
	if [ -n "`echo $i | grep -v "_" `" ]
    then
      libc_symbol="$i ${libc_symbol}"
    fi
done
##去掉排除文件的内容
symbol="${libc_symbol}"
libc_symbol=""
symbol_exclude=`cat libc_symbol.exclude.list `
for i in `echo ${symbol}`
do
    if [ -z "`echo ${symbol_exclude} | grep -w $i`" ]
    then
      libc_symbol="$i ${libc_symbol}"
    fi
done

#去重
libc_symbol=`echo ${libc_symbol} | tr ' ' '\n ' | sort | uniq`

#输出符号
for symbol in `echo ${libc_symbol}`
do
   echo "{hdefaults_str(${symbol}),(uintptr_t)&${symbol}}," >> ${filename}
done


#文件结束
echo "};" >>  ${filename}


#使用astyle整理文件
astyle -n ${filename} 2> /dev/null >/dev/null

popd
