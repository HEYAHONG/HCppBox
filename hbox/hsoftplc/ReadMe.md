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