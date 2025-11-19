# 说明

第三方库的源代码及其许可见[3rdparty](3rdparty)目录（注意：此目录的源代码无需添加构建的源代码列表，由h3rdparty_*.c引入）。

现有以下第三方库：

- [cJSON](https://github.com/DaveGamble/cJSON/tree/master):JSON 解析器。
- [printf](https://github.com/mpaland/printf.git)：嵌入式系统sprintf/printf实现。
- [nanopb](https://github.com/nanopb/nanopb.git)：嵌入式系统Protocol Buffers。
- [paho.mqtt.embedded-c](https://github.com/eclipse-paho/paho.mqtt.embedded-c)：嵌入式MQTT C/C++客户端。
- [softfp](https://bellard.org/softfp/)：软件浮点库。
- [zlib](https://github.com/madler/zlib):zlib压缩库
- [lz4](https://github.com/lz4/lz4):lz4压缩库
- [libqrencode](https://github.com/fukuchi/libqrencode.git):二维码编码库
- [argtable3](https://github.com/argtable/argtable3.git):命令行解析工具
- [tlsf](https://github.com/mattconte/tlsf.git):TLSF 内存分配算法实现
- [libfdt](https://git.kernel.org/pub/scm/utils/dtc/dtc.git):设备树操作库
- [FreeRTOS](www.freertos.org):FreeRTOS操作系统
- [mini-rv32ima](https://github.com/cnlohr/mini-rv32ima.git):C语言编写的极小的RISC-V模拟器
- [matiec](https://github.com/thiagoralves/matiec.git):IEC 61131-3 编译器，注意:本库仅集成matiec编译器的内置库的C语言头文件,而非编译本身。

 提供的配置宏定义如下：

|                        宏定义                        |               说明               |                             备注                             |
| :--------------------------------------------------: | :------------------------------: | :----------------------------------------------------------: |
|            `H3RDPARTY_USING_SYSTEM_CJSON`            |         使用系统cJSON库          |     需要手动引入第三方库，只定义此宏定义可能不能通过编译     |
|           `H3RDPARTY_USING_SYSTEM_NANOPB`            |         使用系统nanopb库         |     需要手动引入第三方库，只定义此宏定义可能不能通过编译     |
| `H3RDPARTY_USING_SYSTEM_PAHO_MQTT_EMBEDDED_C_PACKET` | 使用系统`paho.mqtt.embedded-c`库 | 某些环境内置`paho.mqtt.embedded-c`库，可使用此宏定义防止冲突 |
|           `H3RDPARTY_USING_SYSTEM_SOFTFP`            |        使用系统`softfp`库        |        某些环境下函数可能冲突，可使用此宏定义防止冲突        |
|            `H3RDPARTY_USING_SYSTEM_ZLIB`             |           使用系统zlib           |      链接到系统的zlib(动态库)时,可使用此宏定义防止冲突       |
|             `H3RDPARTY_USING_SYSTEM_LZ4`             |           使用系统lz4            |       链接到系统的lz4(动态库)时,可使用此宏定义防止冲突       |
|         `H3RDPARTY_USING_SYSTEM_LIBQRENCODE`         |       使用系统libqrencode        |   链接到系统的libqrencode(动态库)时,可使用此宏定义防止冲突   |
|          `H3RDPARTY_USING_SYSTEM_ARGTABLE3`          |        使用系统argtable3         |    链接到系统的argtable3(动态库)时,可使用此宏定义防止冲突    |
|            `H3RDPARTY_USING_SYSTEM_TLSF`             |           使用系统tlsf           |          链接到系统的tlsf时,可使用此宏定义防止冲突           |
|           `H3RDPARTY_USING_SYSTEM_LIBFDT`            |          使用系统libfdt          |         链接到系统的libfdt时,可使用此宏定义防止冲突          |
|                  `FREERTOS_KERNEL`                   |       启用FreeRTOS内核代码       | 若启用FreeRTOS内核代码，必须完成FreeRTOS的移植，否则将编译不过。 |

# nanopb

嵌入式系统Protocol Buffers。

本组件预定义一些Protocol Buffers，可用于测试。具体见[pbinc](pbinc)。

使用nanopb时，需要先使用Protocol Buffers消息文件(`.proto`文件)通过`nanopb_generator`工具生成`.pb.h`与`.pb.c`文件。

nanopb_generator可通过以下方式获取：

- 直接下载[nanopb](https://github.com/nanopb/nanopb)源代码,使用源代码中的`generator/nanopb_generator.py`。
- 通过[python](https://www.python.org/)的pip工具安装nanopb包，安装完成后若PATH变量设置正确则可直接使用`nanopb_generator`。此方法适用于所有支持python且支持pip的平台。
- 通过系统的软件包管理工具安装nanopb软件包。此方法适用于python软件包被系统管理的情况，如ubuntu可使用`sudo apt-get install nanopb`安装相应工具，安装完成即可使用`nanopb_generator`。

# paho.mqtt.embedded-c

嵌入式系统[MQTT](https://mqtt.org/)库，主要使用其MQTT数据包解析功能。

由于[paho.mqtt.embedded-c](https://github.com/eclipse-paho/paho.mqtt.embedded-c)的头文件中的枚举值没有加前缀，容易与其它库冲突，因此默认不包含其头文件。

若要使用相应库功能，请使用以下代码：

```C++
//引入MQTT包解析功能
#include H3RDPARTY_PAHO_MQTT_EMBEDDED_C_PACKET_HEADER

```

# softfp

对IEEE 754-2008 32/64/128位浮点支持，主要用于hsimulator组件。

若要使用相应库功能，请使用以下代码：

```c++
#include H3RDPARTY_SOFTFP_HEADER
#include H3RDPARTY_SOFTFLOAT_HEADER
```

# zlib

zlib压缩库。

若要使用相应库功能，请使用以下代码：

```c++
#include H3RDPARTY_ZLIB_HEADER
```

注意:此库需要根据使用情况添加C语言文件(h3rdparty_zlib_*.c),某些C文件可能在某些场景下不能正常编译。

# lz4

lz4压缩库。

若要使用相应库功能，请使用以下代码：

```c++
#include H3RDPARTY_LZ4_HEADER
#include H3RDPARTY_LZ4HC_HEADER
#include H3RDPARTY_LZ4FRAME_HEADER
#include H3RDPARTY_LZ4FILE_HEADER
```

注意:此库需要根据使用情况添加C语言文件(h3rdparty_lz4_*.c),某些C文件可能在某些场景下不能正常编译。

# libqrencode

二维码编码库。

若要使用相应库功能，请使用以下代码：

```c++
#include H3RDPARTY_LIBQRENCODE_HEADER
```

注意:此库需要根据使用情况添加C语言文件(h3rdparty_libqrencode_*.c),某些C文件可能在某些场景下不能正常编译。

# argtable3

命令行解析库。

若要使用相应库功能，请使用以下代码：

```c++
#include H3RDPARTY_ARGTABLE3_HEADER
```

注意:此库需要根据使用情况添加C语言文件(h3rdparty_argtable3_*.c),某些C文件可能在某些场景下不能正常编译。

# tlsf

TLSF内存分配算法实现。

若要使用相应库功能，请使用以下代码：

```c++
#include H3RDPARTY_TLSF_HEADER
```

注意:此库需要根据使用情况添加C语言文件(h3rdparty_tlsf_*.c),某些C文件可能在某些场景下不能正常编译。

# libfdt

设备树操作库。

若要使用相应库功能，请使用以下代码：

```c++
#include H3RDPARTY_LIBFDT_HEADER
```

注意:此库需要根据使用情况添加C语言文件(h3rdparty_libfdt_*.c),某些C文件可能在某些场景下不能正常编译。

# FreeRTOS

FreeRTOS操作系统

若要使用相应API，请使用以下代码：

```c
#include FREERTOS_KERNEL_FREERTOS_HEADER
#include FREERTOS_KERNEL_TASK_HEADER
#include FREERTOS_KERNEL_TIMERS_HEADER
#include FREERTOS_KERNEL_QUEUE_HEADER
#include FREERTOS_KERNEL_SEMPHR_HEADER
#include FREERTOS_KERNEL_CROUTINE_HEADER
#include FREERTOS_KERNEL_LIST_HEADER
#include FREERTOS_KERNEL_EVENT_GROUPS_HEADER
```

 提供的配置宏定义如下：

|                  宏定义                  |                             说明                             |       备注       |
| :--------------------------------------: | :----------------------------------------------------------: | :--------------: |
|      `FREERTOS_KERNEL_MEMMANG_HEAP`      | 配置FreeRTOS内核所使用的heap，可选1、2、3、4、5，分别使用heap_1.c、heap_2.c、heap_3.c、heap_4.c、heap_5.c、 | 默认使用heap_3.c |
| `FREERTOS_KERNEL_NO_AUTO_INCLUDE_HEADER` |                   不自动包含FreeRTOS头文件                   |    默认不定义    |

# mini-rv32ima

C语言编写的极小的RISC-V模拟器。

此模拟器几乎可以运行在支持C语言的任何平台(包括各种单片机，如[esp32s3](https://github.com/ohdarling/linux-esp32s3.git)、[Atmega328p](https://github.com/raspiduino/arv32-opt.git)、[ch32v003](https://github.com/tvlad1234/linux-ch32v003.git)等)，不过需要移植I/O部分,如果本身内存不够，可通过外扩RAM解决。

此模拟器可加载nommu版本的Linux。

若要使用相应库功能，请使用以下代码：

```c++
/*
 * 定义移植mini-rv32ima需要的宏定义(需要根据具体移植修改)
 */
#define MINI_RV32_RAM_SIZE ram_amt
#define MINIRV32_IMPLEMENTATION


/*
 * 包含mini-rv32ima头文件
 */
#include H3RDPARTY_MINI_RV32IMA_HEADER
#include H3RDPARTY_MINI_RV32IMA_64MB_DTB_HEADER
```

注意：

- 本组件中只包含核心的模拟器代码。其它代码(如测试代码)请见 [mini-rv32ima](https://github.com/cnlohr/mini-rv32ima.git)源代码。

# matiec

IEC 61131-3编译器，可将IEC 61131-3中定义的编程语言（通常用于PLC）转化为C语言代码。

一般情况下，PLC的编程更加注重控制逻辑的实现，一般称为组态。

matiec编译器生成的C语言文件编译时需要指定库的头文件路径，此时可使用本库内置的头文件路径（**需要手动指定**），见[3rdparty/matiec_c_header/](3rdparty/matiec_c_header/)
