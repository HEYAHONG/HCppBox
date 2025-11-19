# 说明

辅助实现软件PLC功能。

通常配合[matiec](https://github.com/thiagoralves/matiec)使用。

# 外部配置宏定义

| 宏定义                                  | 说明                                                     | 备注                                                       |
| --------------------------------------- | -------------------------------------------------------- | ---------------------------------------------------------- |
| `HSOFTPLC`                              | 启用本组件，启用时需要定义为1                            | 默认不启用。                                               |
| `HSOFTPLC_NO_DYNAMIC_LOCATED_VARIABLES` | 不启用动态生成的变量。若启用，用户需要手工定义相关变量。 | 默认不启用。推荐使用弱符号预定义相关变量而不是启用此选项。 |

宏定义可由hdefaults组件引入，也可以在C编译器的命令行上定义。

# 注意

- 本组件默认不开启，需要用户在构建软件PLC时手动启用。
- 本组件的兼容性依赖于matiec的C语言头文件的兼容，不保证在所有编译环境下可用(如MSVC可能不能正确编译本组件)。

# 工具

- [matiec](https://github.com/thiagoralves/matiec)：IEC 61131-3 编译器
- [OpenPLC_Editor](https://github.com/thiagoralves/OpenPLC_Editor.git)：OpenPLC IDE，可用于生成软件PLC程序。

## matiec

IEC 61131-3编译器，可将IEC 61131-3中定义的编程语言（通常用于PLC）转化为C语言代码。

一般情况下，PLC的编程更加注重控制逻辑的实现，一般称为组态。

matiec编译器生成的C语言文件编译时需要指定库的头文件路径，此时可使用本库内置的头文件路径（**需要手动指定**），见[3rdparty/matiec_c_header/](3rdparty/matiec_c_header/)

通常情况下，可使用`iec2c`命令将IEC 61131-3程序文件转化为C语言文件，如：

```bash
iec2c  -f -l -p -r -R -a -I [头文件路径] -T [输出目录] [ST程序文件]
```

生成的C语言文件包含多个C语言代码文件与C语言头文件，但一般情况下，只需要编译组态(config)文件与资源(resource)文件。

matiec编译器生成的C语言代码包含以下入口：

- `void config_init__(void);`:组态初始化，一般只调用一次。
- `void config_run__(unsigned long tick);`：组态运行，多次调用，参数为节拍，每次调用时加一,节拍调用的间隔由变量`common_ticktime__`指定。

matiec编译器生成的C语言代码的组态逻辑与其它C语言代码的交互主要通过变量实现，即组态运行前更新变量，运行后根据变量的变化执行相应的操作。

如对于使用到定时器的代码，应当定义**`TIME __CURRENT_TIME;`**全局变量，并在调用组态运行前更新此变量。

对于指定地址(如输入位、输出位)的变量等，需要在其它C语言代码定义该变量(注意：组态中使用的变量通常为指针)，通常使用的代码如下：

```c
#define __LOCATED_VAR(type, name, ...) type __##name;
#include "LOCATED_VARIABLES.h"
#undef __LOCATED_VAR
#define __LOCATED_VAR(type, name, ...) type* name = &__##name;
#include "LOCATED_VARIABLES.h"
#undef __LOCATED_VAR
```

以上代码可动态定义已使用的变量，实际上也可以固定定义一些C语言变量（如8个输入位、8个输出位），C语言变量名称由IEC61131-3的变量地址决定。

通常情况下，IEC61131-3的变量地址为`%<存储区><数据长度><地址>`。

变量地址的存储区类型如下(只列举常用的存储区类型)：

- I:输入
- Q:输出
- M:内存/全局变量

变量地址的数据长度如下:

- X:1位
- B:8位
- W:16位
- D:32位
- L:64位

变量地址的地址为数字。

变量地址与C语言中的变量名可进行一定转换，通常情况下，C语言变量名为`__<存储区><数据长度><地址>`，如:

- %IX0.0：**输入位 0.0**（第 0 字节的第 0 位),C语言变量名为`__IX0_0`。
- %QX1.5：**输出位 1.5**（第 1 字节的第 5 位),C语言变量名为`__QX1_5`。

## OpenPLC_Editor

OpenPLC IDE，可用于生成软件PLC程序(通常是ST程序，可用于matiec转换为C语言代码)。