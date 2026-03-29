# 简介

## HBox

hbox意为HYH的工具箱。

实现一些在开发过程中的常用函数及相关结构。

主要用于嵌入式编程，通常只依赖于C运行库与部分OS接口,尽量使用较少的依赖以适应各种编译环境。

![HBox_Structure](HBox_Structure.png)

默认情况下，调用层次如下：

- C++语言组件(如[hcpprt_init](@ref hcpprt_init)、[hcpprt_loop](@ref hcpprt_loop)):C++组件为可选组件。若支持C++则应当使用C++组件(其内部自动调用C语言组件，用户无需重复调用)，否则应当直接使用C语言组件。
- C语言组件(如[hruntime_init](@ref hruntime_init)、[hruntime_loop](@ref hruntime_loop))：C语言组件中，由hruntime组件管理运行时，其它组件的初始化及循环调用默认情况下均由hruntime管理，也可使用宏定义单独管理某个组件的调用。
- OS/裸机（如C运行库、系统调用）:一般情况下由hdefaults组件管理。

## HCPPBox

本组件主要实现常用的C++功能。

主要用于一般编程,主要依赖标准C++运行库(如线程、锁等)，通常运行于Windows、Linux等支持完整C++运行库的操作系统,一般不支持RTOS(一般不支持完整的标准C++库，某些组件不可用)。
