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

echo 当前目录为${script_dir}.

pushd ${script_dir}

if [ -d "${script_dir}/Z80" ]
then
	pushd "${script_dir}/Z80"
	git pull
	popd
else
	git clone https://github.com/redcode/Z80.git "${script_dir}/Z80"
fi


if [ -f "${script_dir}/Z80/COPYING" ]
then
	rsync -rl --progress  "${script_dir}/Z80/COPYING" "${script_dir}/"
	rsync -rl --progress  --include="*.h" --include="*.c" --exclude="*" "${script_dir}/Z80/API/" "${script_dir}/"
	rsync -rl --progress  --include="*.h" --include="Z80*.c" --exclude="*" "${script_dir}/Z80/sources/" "${script_dir}/"
	for c_file in `ls Z80.c`
	do
		cp "Z80_template_c" "../../h3rdparty_Z80_${c_file}"
		sed -i "s/Z80_filename/${c_file}/g"  "../..//h3rdparty_Z80_${c_file}"
	done
fi

if [ -d "${script_dir}/Zeta" ]
then
	pushd "${script_dir}/Zeta"
	git pull
	popd
else
	git clone https://github.com/redcode/Zeta.git "${script_dir}/Zeta"
fi


if [ -f "${script_dir}/Zeta/COPYING" ]
then
	rsync -rl --progress   "${script_dir}/Zeta/API/" "${script_dir}/"
	for header_file in $(find  Z/ -name "*.h")
	do
		sed -i "s/Z\//h3rdparty\/3rdparty\/Z80\/Z\//g" ${header_file}
	done
	for header_file in $(find  Z/ -name "*.hpp")
	do
		sed -i "s/Z\//h3rdparty\/3rdparty\/Z80\/Z\//g" ${header_file}
	done
fi

popd
