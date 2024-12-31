# 说明

hbox意为HYH的工具箱。

实现一些在开发过程中的常用函数及相关结构。

主要用于嵌入式编程，通常只依赖于C运行库与部分OS接口,尽量使用较少的依赖以适应各种编译环境。

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
|        `WIN32`        |                        处于windows中                         |                                                              |
|      `__unix__`       |                        处于(类)unix中                        | 若处于linux中，可使用`__linux__` 判断。在(类)unix中将使用pthread及其相关库（有些可能需要手动启用）。 |
| `__STDC_NO_ATOMICS__` | 当此宏定义被定义时，C不包含`_Atomic`原子类型名,即不支持原子操作。 |                            C11起                             |
|    `HAVE_CONFIG_H`    |          `config.h`存在,当定义存在时包含`config.h`           |                    此宏定义一般由用户定义                    |
| `HBOX_CONFIG_HEADER`  | `HBOX_CONFIG_HEADER`表示hbox的配置文件路径(不含引号)，若定义将由hdefaults包含该配置文件。 |                    此宏定义一般由用户定义                    |

# C++组件

此C++组件主要用于嵌入式代码,一般不依赖某些在嵌入式中一般不移植的C++运行库(如线程、互斥等),依赖于本工具箱的C代码。

此C++组件具体实现与说明见[cpp](cpp)目录。

# 组件

## hcompiler

主要提供一些编译器相关特性的宏定义。

|         宏定义         |      说明      |                             备注                             |
| :--------------------: | :------------: | :----------------------------------------------------------: |
|        `__ASM`         |      汇编      |                                                              |
|       `__INLINE`       |      内联      |                                                              |
|   `__STATIC_INLINE`    |    静态内联    |                                                              |
| `__STATIC_FORCEINLINE` |  静态强制内联  |                                                              |
|     `__NO_RETURN`      |     无返回     |                                                              |
|        `__USED`        |  标记已被使用  |                                                              |
|        `__WEAK`        |     弱定义     | 一般用于库函数编写。注意:在gcc中必须指定`-Wl,--whole-archive`才能确保库的弱定义一定生效 |
|       `__PACKED`       |  取消对齐优化  |                                                              |
|      `__ALIGNED`       |    对齐设置    |                                                              |
|      `__RESTRICT`      | restrict限定符 |                                                              |
|      `__NO_INIT`       |  不进行初始化  | 通常用于支持SRAM在复位时不复位的单片机使用。可减少异常复位带来的数据损失。 |
|       `__ALIAS`        |      别名      |                                                              |
|      `__SECTION`       |     设置节     |                     通常配合链接脚本使用                     |

## hdefaults

本组件主要提供一些定义及函数。本工具箱的可移植部分通常在本组件中进行。

|                        函数                         |      说明      |                             备注                             |
| :-------------------------------------------------: | :------------: | :----------------------------------------------------------: |
|     `hdefaults_tick_t hdefaults_tick_get(void)`     |  默认节拍获取  |                                                              |
| `void * hdefaults_malloc(size_t nBytes,void *usr);` |  默认内存分配  |                                                              |
|     `void hdefaults_free(void *ptr,void *usr);`     |  默认内存释放  |                                                              |
|      `void  hdefaults_mutex_lock(void *usr);`       | 默认互斥锁加锁 | 通常在实现时使用临界区实现,如需使用互斥锁，必须使用支持递归的互斥锁 |
|     `void  hdefaults_mutex_unlock(void *usr);`      | 默认互斥锁解锁 | 通常在实现时使用临界区实现，如需使用互斥锁，必须使用支持递归的互斥锁 |

提供的宏定义如下:

|         宏定义          |               说明               | 备注 |
| :---------------------: | :------------------------------: | :--: |
|   `hdefaults_xstr(s)`   | 宏函数,将符号s的内容转换为字符串 |      |
|   `hdefaults_str(s)`    |    宏函数,将符号s转换为字符串    |      |
| `HDEFAULTS_OS_RTTHREAD` |         处于RT-Thread中          |      |
| `HDEFAULTS_OS_WINDOWS`  |          处于Windows中           |      |
|   `HDEFAULTS_OS_UNIX`   |        处于UNIX(类UNIX中)        |      |

可外部配置的宏定义如下:

|          宏定义          |                             说明                             |                             备注                             |
| :----------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: |
|   `HDEFAULTS_TICK_GET`   |               默认获取节拍(毫秒)函数名称宏定义               |                                                              |
|    `HDEFAULTS_MALLOC`    |                  默认内存分配函数名称宏定义                  |                                                              |
|     `HDEFAULTS_FREE`     |                  默认内存释放函数名称宏定义                  |                                                              |
|  `HDEFAULTS_MUTEX_LOCK`  | 默认互斥锁加锁函数(无参数，无返回值)名称宏定义,要求锁支持递归。 | 一般用于嵌入式编程,一般使用临界区实现。对于无操作系统的环境,可采用计数+开中断的方式实现(参考rt-thread)。 |
| `HDEFAULTS_MUTEX_UNLOCK` | 默认互斥锁解锁函数(无参数，无返回值)名称宏定义，要求锁支持递归。 | 一般用于嵌入式编程,一般使用临界区实现。对于无操作系统的环境,可采用计数+关中断的方式实现(参考rt-thread)。 |

##  heventloop

本组件用于辅助实现事件循环功能，通常用于将一些事件发送到其它线程执行。

通常用法为在某个固定的线程或者位置周期调用事件处理函数,在其它位置添加事件。

当所有的事件均在固定位置处理(如仅在某一个线程处理对某类资源的访问，其它线程均通过添加事件的方式访问)时，可减少多个线程需同步时锁的使用。

## heventslots

本组件通常用于实现一般回调函数功能。本组件主要定义以下语义:

- 信号:调用回调函数的参数。
- 槽:回调函数
- 发送信号:使用信号作为参数依次调用回调函数。

通常回调函数可由函数指针(及其数组)简易实现，函数指针(及其数组)优点是简单可靠，缺点是不能动态添加(非设置指针),其数量是固定的。

本组件的特点为可动态注册/反注册槽。

## heventchain

本组件通常用于实现Hook回调函数功能。

本组件中回调函数是链式的,当排在前面的回调函数成功处理后将不再向后传递,如同windows的钩子一样，优先级较高的钩子可截断某些操作。

本组件相对于heventslots的特点为调用者可获取参数是否最终处理，并支持优先级(优先级高的钩子可截断数据的处理)。

## hwatchdog

本组件通常用于辅助实现看门狗,包括软件实现的看门狗及硬件看门狗喂狗。

需要使用看门狗时需要使用`HWATCHDOG_ADD_WATCH(check,timeout_ms,usr)`在需要检查的地方添加看门狗检查,当看门狗喂狗被调用时将执行检查函数。

喂狗操作通过调用`HWATCHDOG_FEED()`完成。

当本组件用于RTOS时,一般在RTOS的空闲线程中空闲钩子(若有)调用喂狗。

当本组件用于通用编程(如Windows、Linux等操作系统下的编程)时,一般需要单独开一个线程喂狗。

当本组件用于无操作系统时，一般需要在主循环中周期喂狗。

### 软件看门狗

软件看门狗由纯软件实现,其原理为在用户喂狗时进行检查,当检查不过时,调用特定函数。对于软件看门狗而言,需要实现以下函数并通过`hwatchdog_setup_software_dog`设置看门狗:

|              函数              |                             说明                             |
| :----------------------------: | :----------------------------------------------------------: |
|        void sys_reset()        | 系统复位。对于单片机而言为单片机软件复位，对于一般应用而言为重启自身。当此函数无效时必须实现硬件看门狗。 |
| hwatchdog_tick_t sys_tick_ms() | 获取系统时钟节拍(单位:ms)。此函数必须实现，否则看门狗将不可用。注意:返回值位宽由宏定义决定，默认为hdefaults组件的hdefaults_tick_t类型。 |

### 硬件看门狗

软件看门狗的缺点为当用户因异常不能喂狗时看门狗将失效。使用硬件看门狗可弥补软件看门狗的不足。

对于硬件看门狗的初始化，由其它代码实现。如需启用硬件看门狗,需要实现以下函数并`hwatchdog_set_hardware_dog_feed`设置喂狗函数:

|      函数      |                         说明                          |
| :------------: | :---------------------------------------------------: |
| void hw_feed() | 硬件看门狗喂狗。注意:硬件看门狗初始化由其它代码实现。 |

### 看门狗使用

可使用以下方式使用:

- 在需要添加检查的地方添加检查宏定义，一般配合lambda表达式使用。
- 使用软狗(softdog),软狗拥有与硬件看门狗相似的特性，软狗各自独立,软狗需要执行自己的喂狗(除开总的看门狗喂狗)。

## hmemoryheap

本组件主要用于实现一个简易内存堆。在操作系统不提供动态内存分配时使用。 

可管理的内存参数如下:

|          项目          |               值(字节)               |               经典值（对齐为4,32位系统）               |
| :--------------------- | :----------------------------: | ---------------------- |
| 可管理的最大内存字节数 | `2^31 - HMEMORYHEAP_ALIGNED_SIZE` | 0x7FFF FFFC Bytes |
| 可管理的最小内存字节数(无法分配任何内存，仅供创建堆) | `sizeof(hmemoryheap_pool_block_t)+sizeof(hmemoryheap_pool_t)` | 32 Bytes |
| 推荐的最小内存字节数 |  | 256 Bytes |
| 每分配一个指针额外消耗的最小内存 | `sizeof(hmemoryheap_pool_block_t)` | 4 Bytes |
| 每分配一个指针额外消耗的最大内存 | `sizeof(hmemoryheap_pool_block_t)*2+（HMEMORYHEAP_ALIGNED_SIZE-1）` | 11 Bytes |

可配置的宏定义如下:

|             宏定义              |                             说明                             |            备注             |
| :-----------------------------: | :----------------------------------------------------------: | :-------------------------: |
|   `HMEMORYHEAP_ALIGNED_SIZE`    |                  对齐大小。默认为指针大小。                  | 需根据对应架构的要求修改。  |
|   `HMEMORYHEAP_MAGIC_NUMBER`    |         魔数，用作标记是否初始化。默认为0xcccc55aa。         |                             |
|     `HMEMORYHEAP_PTR_SIZE`      |               指针大小。默认不定义(默认为4)。                | 对于64位系统一定要指定为8。 |
| `HMEMORYHEAP_DEFAULT_POOL_SIZE` |            默认堆大小。至少256字节。默认不定义。             |                             |
|       `USING_HMEMORYHEAP`       | 启用默认堆(默认256字节)并将其作为本工具箱默认内存分配函数。默认不定义 |                             |

## hobject

本组件主要用于实现一些基于C的对象的相关操作。

本组件主要用于事件处理中数据的传递。

利用C语言结构体地址与结构体第一个成员的地址相同的特性,可实现类似于C++类的继承的特性,不过C语言只能实现单继承,且没有权限控制。

对于带管理的对象,添加了实现类似于C++的析构功能的回调函数指针，带管理的对象在释放数据前，应调用`hobject_cleanup`调用清理函数后再使用内存释放函数(如`free`)释放内存。

注意:对于结构体对象(包括带管理的结构体对象)，当架构(或者操作系统)不支持非对齐访问且对齐字节大于4字节时,可能出现异常。

## hringbuf

本组件主要提供简易环形缓冲区。

环形缓冲区常用来做FIFO,对数据做一些缓存。

常见的缓冲区(如数组)可通过`hringbuf_get`函数转化为环形缓冲区。

当运行环境不是无操作系统环境时或者需要设置用户指针时，需使用`hringbuf_set_lock`设置相应的锁(只需要设置一次)，否则可能工作异常。

## hunicode

 本组件主要用于辅助处理Unicode编码，主要用用于辅助处理`wchar_t`、UTF-8字符串与Unicode编码之间的转换。

本组件的主要作用是处理中文字符。

当需要使用UTF-8字符串时，需要把C/C++源代码文件的编码改为UTF-8。

若C/C++源代码文件编码不为UTF-8（常见于Windows）,定义的常量字符串编码将不是UTF-8字符串，无法使用本组件。

## hstacklesscoroutine

本组件用于辅助编写简易无栈协程。无栈协程可用于实现简易的多任务系统。

无栈协程的特点如下：

- 协程没有自己的栈，直接使用调用协程的线程（进程）栈。
- 让出控制权时局部变量将失效，因此状态保存只能使用非局部变量（全局变量或者静态变量）。
- 可移植性强，几乎不依赖特定的操作系统特性或者硬件特性。
- 只能由用户控制是否让出控制权,没有抢占等特性。

### 定义协程块

定义协程块主要使用以下宏定义：

- `HSTACKLESSCOROUTINE_BLOCK_START(NAME)`
- `HSTACKLESSCOROUTINE_BLOCK_END(NAME)`

协程块的形式如下:

```c++
HSTACKLESSCOROUTINE_BLOCK_START(协程名称)
 .
 . //协程代码
 .
HSTACKLESSCOROUTINE_BLOCK_END(协程名称)
```

在协程代码中，可使用以下以下函数进行操作：

- `HSTACKLESSCOROUTINE_GET_CURRENT_CCB()`：获取当前协程控制块指针
- `HSTACKLESSCOROUTINE_GET_CURRENT_EVENT()`：获取当前事件指针
- `hstacklesscoroutine_yield()` ：让出控制权，下次将从此开始运行。
- `hstacklesscoroutine_yield_with_label(N)`：让出控制权并且使用标签（正整数），下次将从此开始运行。
- `hstacklesscoroutine_return()`：协程返回，常用于条件不满足时不继续运行，并从上一次让出控制权的地方重新运行。
- `hstacklesscoroutine_goto_label(N)`：跳转至特定标签，可配合`hstacklesscoroutine_yield_with_label(N)`使用。
- `hstacklesscoroutine_await(AWAITER)`:协程等待，与协程返回不同，协程等待条件满足前不会再运行任何用户代码

### 调用协程

所用的协程都需要一个或者多个线程运行。

对于MCU等嵌入式环境，无操作系统时可直接在`main()`函数中调用协程，有操作系统时可使用单独线程调用协程或者使用软件定时器调用协程。

当定义协程块与调用协程不在同一个文件时，需要使用`HSTACKLESSCOROUTINE_DECLARE_COROUTINE(NAME)`先声明协程。

调用协程可使用以下入口函数（任选其一）:

- `HSTACKLESSCOROUTINE_ENTRY(NAME)`
- `HSTACKLESSCOROUTINE_ENTRY_WITH_EVENT(NAME,EVENT)`
- `HSTACKLESSCOROUTINE_ENTRY_WITH_CCB_AND_EVENT(NAME,CCB,EVENT)`

以上函数的区别仅仅是是否使用自定义的协程控制块（CCB）与事件（EVENT）,入口函数应该多次调用，直到协程完成。

协程的全局协程控制块可使用`HSTACKLESSCOROUTINE_GET_GLOBAL_CCB(NAME)`获取，可使用协程控制块获取协程是否完成。

## hmodbus

本组件用于辅助实现[modbus](https://modbus.org),标准文档为[Modbus_Application_Protocol_V1_1b3.pdf](https://modbus.org/docs/Modbus_Application_Protocol_V1_1b3.pdf)。

现可实现以下功能：

- 精简版modbus rtu从机:实现简单的modbus从机功能，仅实现线圈、离散输入、输入寄存器、保持寄存器相关功能。
- modbus tcp网关服务器：实现modbus tcp到modbus rtu的转换。
- modbus tcp客户端：实现对modbus tcp服务器的访问。

注意：本组件只包含对相应模式（如 modbus tcp、modbus rtu）下的数据包的处理，不包含具体io操作(如串口读写、tcp读写)。

[modbus](https://modbus.org)支持以下数据类型：

| 名称       | 说明                  | 备注                                   |
| ---------- | --------------------- | -------------------------------------- |
| 线圈       | 可读写的位（1bit）    |                                        |
| 离散输入   | 只读的位（1bit）      |                                        |
| 保持寄存器 | 可读写的数据（16bit） | 超过16位的数据通常需要多个寄存器组合。 |
| 输入寄存器 | 只读的数据（16bit）   | 超过16位的数据通常需要多个寄存器组合。 |

实际应用中，需要将这些数据类型映射到实际的功能上。

具体实现见[modbus](modbus)目录（注意：此目录的源代码需要添加构建的源代码列表）

## h3rdparty

有很多成熟的第三方库也适合本库的应用场景，在此模块上添加这些库。

第三方库的源代码及其许可见[3rdparty](3rdparty)目录（注意：此目录的源代码无需添加构建的源代码列表，由h3rdparty_*.c引入）。

现有以下第三方库：

- [cJSON](https://github.com/DaveGamble/cJSON/tree/master):JSON 解析器。
- [printf](https://github.com/mpaland/printf.git)：嵌入式系统sprintf/printf实现。
- [nanopb](https://github.com/nanopb/nanopb.git)：嵌入式系统Protocol Buffers。

 提供的配置宏定义如下：

|             宏定义              |       说明       |                         备注                         |
| :-----------------------------: | :--------------: | :--------------------------------------------------: |
| `H3RDPARTY_USING_SYSTEM_CJSON`  | 使用系统cJSON库  | 需要手动引入第三方库，只定义此宏定义可能不能通过编译 |
| `H3RDPARTY_USING_SYSTEM_NANOPB` | 使用系统nanopb库 | 需要手动引入第三方库，只定义此宏定义可能不能通过编译 |

### nanopb

嵌入式系统Protocol Buffers。

本组件预定义一些Protocol Buffers，可用于测试。具体见[pbinc](pbinc)。

使用nanopb时，需要现使用Protocol Buffers消息文件通过`nanopb_generator`工具生成`.pb.h`与`.pb.c`文件。

nanopb_generator可通过以下方式获取：

- 直接下载[nanopb](https://github.com/nanopb/nanopb)源代码,使用源代码中的`generator/nanopb_generator.py`。
- 通过[python](https://www.python.org/)的pip工具安装nanopb包，安装完成后若PATH变量设置正确则可直接使用`nanopb_generator`。此方法适用于所有支持python且支持pip的平台。
- 通过系统的软件包管理工具安装nanopb软件包。此方法适用于python软件包被系统管理的情况，如ubuntu可使用`sudo apt-get install nanopb`安装相应工具，安装完成即可使用`nanopb_generator`。

## hbase64

简易Base64编解码。可用于在Json中嵌入二进制编码。

## hgui

在嵌入式场景中辅助实现GUI界面。

实现在GUI界面中一些常用的功能,提供一些接口模板。

具体实现见[gui](gui)目录（注意：此目录的源代码需要添加构建的源代码列表）

## hsimulator

辅助实现一些可在嵌入式平台(如MCU)上运行的模拟器。

具体实现见[simulator](simulator)。

模拟器相对真机的优点是可轻松控制运行状态（监控、复位、调试、升级等），缺点是一般来说模拟器的性能不如真机。

如果是物联网应用，模拟器还可远程控制运行状态。
