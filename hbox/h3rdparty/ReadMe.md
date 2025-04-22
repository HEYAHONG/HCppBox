# 说明

第三方库的源代码及其许可见[3rdparty](3rdparty)目录（注意：此目录的源代码无需添加构建的源代码列表，由h3rdparty_*.c引入）。

现有以下第三方库：

- [cJSON](https://github.com/DaveGamble/cJSON/tree/master):JSON 解析器。
- [printf](https://github.com/mpaland/printf.git)：嵌入式系统sprintf/printf实现。
- [nanopb](https://github.com/nanopb/nanopb.git)：嵌入式系统Protocol Buffers。
- [paho.mqtt.embedded-c](https://github.com/eclipse-paho/paho.mqtt.embedded-c)：嵌入式MQTT C/C++客户端。
- [softfp](https://bellard.org/softfp/)：软件浮点库。

 提供的配置宏定义如下：

|                        宏定义                        |               说明               |                             备注                             |
| :--------------------------------------------------: | :------------------------------: | :----------------------------------------------------------: |
|            `H3RDPARTY_USING_SYSTEM_CJSON`            |         使用系统cJSON库          |     需要手动引入第三方库，只定义此宏定义可能不能通过编译     |
|           `H3RDPARTY_USING_SYSTEM_NANOPB`            |         使用系统nanopb库         |     需要手动引入第三方库，只定义此宏定义可能不能通过编译     |
| `H3RDPARTY_USING_SYSTEM_PAHO_MQTT_EMBEDDED_C_PACKET` | 使用系统`paho.mqtt.embedded-c`库 | 某些环境内置`paho.mqtt.embedded-c`库，可使用此宏定义防止冲突 |
|           `H3RDPARTY_USING_SYSTEM_SOFTFP`            |        使用系统`softfp`库        |        某些环境下函数可能冲突，可使用此宏定义防止冲突        |

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