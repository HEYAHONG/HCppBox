# 说明

hbox意为HYH的工具箱。

实现一些在开发过程中的常用函数及相关结构。

主要用于嵌入式编程，通常只依赖于C运行库与部分OS接口,尽量使用较少的依赖以适应各种编译环境。

![HBox_Structure](../doc/HBox_Structure.png)

# 硬件要求

主要支持32位及更高位宽的硬件平台,包括但不限于嵌入式硬件平台:

- 32位通用硬件平台
- 32位嵌入式硬件平台(如单片机等)
- 64位通用硬件平台
- 64位嵌入式硬件平台

8位/16位平台的C编译器基本有其独特的扩展，C语言代码专用性比较强，如需使用相关组件，一般需要进行一些针对性适配。

# 宏定义

本章节所指宏定义主要指用户传递的宏定义，并非组件内部的宏定义。

宏定义通常是通过编译器命令行参数传递的,也可能是编译器预定义的,也可通过特殊的头文件(如config.h)定义。

不同的宏定义将触发不同的行为。

|        宏定义         |                             说明                             |                             备注                             |
| :-------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: |
|    `__RTTHREAD__`     |                    处于RT-Thread操作系统                     |                     RT-Thread 4.0.3+有效                     |
|      `__NuttX__`      |                      处于NuttX操作系统                       |                                                              |
|        `WIN32`        |                        处于windows中                         |                                                              |
|      `__unix__`       |                        处于(类)unix中                        | 若处于linux中，可使用`__linux__` 判断。在(类)unix中将使用pthread及其相关库（有些可能需要手动启用）。 |
|     `__FreeBSD__`     |                        处于FreeBSD中                         |         通常也会定义`__unix__`，即处于(类) unix中。          |
|      `__linux__`      |                         处于linux中                          |         通常也会定义`__unix__`，即处于(类) unix中。          |
|     `__ANDROID__`     |                        处于Android中                         | 通常也会定义`__unix__`、`__linux__`，即处于(类) unix、linux中。 |
|     `__CYGWIN__`      |                         处于Cygwin中                         |         通常也会定义`__unix__`，即处于(类) unix中。          |
|   `__EMSCRIPTEN__`    |                       处于emscripten中                       |         通常也会定义`__unix__`，即处于(类) unix中。          |
| `__STDC_NO_ATOMICS__` | 当此宏定义被定义时，C不包含`_Atomic`原子类型名,即不支持原子操作。 |                            C11起                             |
|    `HAVE_CONFIG_H`    |          `config.h`存在,当定义存在时包含`config.h`           |                    此宏定义一般由用户定义                    |
| `HBOX_CONFIG_HEADER`  | `HBOX_CONFIG_HEADER`表示hbox的配置文件路径(不含引号)，若定义将由hdefaults包含该配置文件。 |                    此宏定义一般由用户定义                    |

# C++组件

此C++组件主要用于嵌入式代码,一般不依赖某些在嵌入式中一般不移植的C++运行库(如线程、互斥等),依赖于本工具箱的C代码。

此C++组件具体实现与说明见[cpp](cpp)目录。

# 组件

## hcompiler

主要提供一些编译器相关特性的宏定义。

具体实现见[hcompiler](hcompiler)目录

## hdefaults

本组件主要提供一些定义及函数。

本组件主要用于提供统一访问OS接口的接口,其它组件一般不直接与OS接口打交道。

具体实现见[hdefaults](hdefaults)目录

##  hevent

本组件主要用于辅助进行事件处理。

具体实现见[hevent](hevent)目录

## hdriverframework

本组件主要用于辅助进行驱动编写。

具体实现见[hdriverframework](hdriverframework)。

## hmemory

本组件主要用于辅助进行内存管理。

具体实现见[hmemory](hmemory)目录

## hlocale

本组件主要用于辅助处理区域设置(如字符编码等)。

具体实现见[hlocale](hlocale)目录

## hnet

本目录主要用于辅助实现网络相关代码。

具体实现见[hnet](hnet)目录

## h3rdparty

有很多成熟的第三方库也适合本库的应用场景，在此模块上添加这些库。

具体实现见[h3rdparty](h3rdparty),由于第三方源代码(有些静态变量、宏定义会冲突)不能直接放在一个C源代码中，[h3rdparty](h3rdparty)中的文件应当添加至构建的源代码列表。

## hgui

在嵌入式场景中辅助实现GUI界面。

实现在GUI界面中一些常用的功能,提供一些接口模板。

具体实现见[hgui](hgui)目录

## hsimulator

辅助实现一些可在嵌入式平台(如MCU)上运行的模拟器。

具体实现见[hsimulator](hsimulator)。

模拟器相对真机的优点是可轻松控制运行状态（监控、复位、调试、升级等），缺点是一般来说模拟器的性能不如真机。

如果是物联网应用，模拟器还可远程控制运行状态。

## hshell

 通常用于运行简易命令，通常用于调试。

具体实现见[hshell](hshell)目录。

使用本组件实现的shell推荐使用支持终端转义序列的软件（如[putty](https://www.chiark.greenend.org.uk/~sgtatham/putty/)、minicom）打开，不推荐直接使用串口调试工具打开。

## huuid

辅助处理UUID。

具体实现见[huuid](huuid)目录。

## hcrypto

加密组件。

一般提供软件实现的加密算法及校验算法。

具体实现见[hcrypto](hcrypto)目录。

## hruntime

辅助处理运行时相关工作。

具体实现见[hruntime](hruntime)目录。

