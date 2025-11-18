# 说明

本目录主要用于存放用于[CMake](https://cmake.org)的模块文件

# 模块

## [FindMATIEC.cmake](FindMATIEC.cmake)

用于设置MATIEC相关变量，提供的变量如下：

| 名称                   | 说明                              |                                      |
| ---------------------- | --------------------------------- | ------------------------------------ |
| `MATIEC_IEC2C_FOUND`   | 已找到IEC2C,值为ON或者OFF         |                                      |
| `IEC2C`                | iec2c可执行文件路径               | 仅当`MATIEC_IEC2C_FOUND`为ON时有效   |
| `MATIEC_IEC2IEC_FOUND` | 已找到IEC2IEC,值为ON或者OFF       |                                      |
| `IEC2IEC`              | iec2iec可执行文件路径             | 仅当`MATIEC_IEC2IEC_FOUND`为ON时有效 |
| `MATIEC_HEADER_FOUND`  | 已找到MATIEC头文件，值为ON或者OFF |                                      |
| `MATIEC_C_HEADER`      | MATIEC的C语言头文件路径           | 仅当`MATIEC_HEADER_FOUND`为ON时有效  |
| `MATIEC_IEC_HEADER`    | MATIEC的IEC 61131-3标准头文件路径 | 仅当`MATIEC_HEADER_FOUND`为ON时有效  |

