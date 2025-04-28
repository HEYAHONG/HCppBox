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
CheckTool find
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

#从系统文件中获取32位系统调用号
for i in `find /usr/include -name unistd_32.h`
do
    cp "$i" hdefaults_os_linux_syscall32.h
    sed -i "s/__NR/HDEFAULTS_OS_LINUX_SYSCALL32/g" hdefaults_os_linux_syscall32.h
    sed -i "s/ASM_UNISTD/HDEFAULTS_OS_LINUX_SYSCALL/g" hdefaults_os_linux_syscall32.h
done

#从系统文件中获取64位系统调用号
for i in `find /usr/include -name unistd_64.h`
do
    cp "$i" hdefaults_os_linux_syscall64.h
    sed -i "s/__NR/HDEFAULTS_OS_LINUX_SYSCALL64/g" hdefaults_os_linux_syscall64.h
    sed -i "s/ASM_UNISTD/HDEFAULTS_OS_LINUX_SYSCALL/g" hdefaults_os_linux_syscall64.h
done


popd
