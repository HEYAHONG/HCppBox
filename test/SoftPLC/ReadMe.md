# 说明

本目录为用于测试软件PLC的示例程序。

本目录采用[CMake](https://cmake.org)作为构建系统。

# MATIEC

本目录的所有代码均需要正确安装[matiec](https://github.com/thiagoralves/matiec.git)才能正常编译。

## 源代码安装

在`Linux`、`Cygwin`、`MSYS2 MSYS`等类`unix`环境下，推荐采用源代码安装,编译安装的指令如下：

```bash

#使用git下载源代码
git clone https://github.com/thiagoralves/matiec

#进入matiec目录
cd matiec

#生成configure
autoreconf -i

#生成Makefile
./configure

#编译安装（可能需要sudo提升为root权限）
make install
```

## 二进制安装

在Windows环境（MinGW）下，推荐采用二进制安装，即下载编译好的二进制文件并将`iec2c.exe`所在目录添加至`PATH`环境变量。

# 注意

- **本目录中的程序应当采用GCC作为C编译器，MSVC可能不能正常通过编译。**
- **某些环境下(如Cygwin、MSYS2 MSYS)可能不兼容。**

#  目录说明

- [blink](blink)：闪灯程序
- [trafic_light](trafic_light)：交通灯程序

