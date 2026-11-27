# 说明

本组件主要包含一个可替换的C运行库，用于替换编译器自带的C语言库或者为无C语言支持库的环境添加C运行库支持。

主要基于[picolibc](https://github.com/picolibc/picolibc),见[picolibc](picolibc)目录。

对于posix标准，一般也归为广义的C运行库，本组件中的posix兼容层参考Linux的接口(头文件)，一般用于增强picolibc，具体见[posix](posix)。

