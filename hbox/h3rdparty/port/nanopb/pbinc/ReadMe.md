# 说明

本目录用于预先定义一些Protocol Buffers文件。

Protocol Buffers是Google公司开发的一种数据描述语言，类似于XML能够将结构化数据序列化，可用于数据存储、通信协议等方面。

Protocol Buffers相对于JSON 、XML等格式，可节省大量的空间。

Protocol Buffers可配合[MQTT](https://mqtt.org/)应用于物联网应用，通过MQTT的主题识别消息类型（即识别不同的消息），使用负载数据传输实际的消息数据。

每个Protocol Buffers具有以下3类文件：

- `.proto`:Protocol Buffers源代码。可通过`protoc`工具生成其它语言(如C++、C#、JAVA、Kotlin、PHP、python等)的文件，本目录只提供适用于nanopb的头文件及源代码文件。
- `.pb.h`:nanopb所用到的头文件。可在多个文件包含。注意:此文件只适用于nanopb。
- `.pb.c`:nanopb所用到的C语言源代码文件。只能在一处C语言源代码文件中包含。注意:此文件只适用于于nanopb。

 C语言源代码包含时需要添加目录名`pbinc`，如包含`main_entry.pb.h`的代码为`#include "pbinc/main_entry.pb.h"`。

# 列表

## [main_entry](main_entry)

（C语言代码）主入口main的参数。
