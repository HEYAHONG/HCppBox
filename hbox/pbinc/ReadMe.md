# 说明

此处用于预先定义一些Protocol Buffers文件。

每个Protocol Buffers具有以下3类文件：

- `.proto`:Protocol Buffers源代码。
- `.pb.h`:nanopb所用到的头文件。可在多个文件包含。
- `.pb.c`:nanopb所用到的C语言源代码文件。只能在一处C语言源代码文件中包含。

 C语言源代码包含时需要添加目录名`pbinc`，如包含`main_entry.pb.h`的代码为`#include "pbinc/main_entry.pb.h"`。

# 列表

## main_entry

（C语言代码）主入口main的参数。