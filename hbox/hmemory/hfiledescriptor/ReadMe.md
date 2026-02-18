# 说明

自行实现的文件描述符，一般情况下用于无操作系统环境。

# 可配置的宏定义

|                    宏定义                     |                       说明                        |                             备注                             |
| :-------------------------------------------: | :-----------------------------------------------: | :----------------------------------------------------------: |
|     `HFILEDESCRIPTOR_COMMON_TABLE_SIZE `      |             文件描述符表中的项目个数              |                                                              |
|   `HFILEDESCRIPTOR_COMMON_TABLE_ATTRIBUTE`    |                文件描述符表的属性                 | 通常用于指定section信息，可用于在启用`MMU`时将文件描述符表存放至指定区域。 |
| `HFILEDESCRIPTOR_COMMON_TABLE_FLAG_ATTRIBUTE` | 文件描述符表标志的属性,用于存储文件描述符分配信息 | 通常用于指定section信息，可用于在启用`MMU`时将文件描述符表标志存放至指定区域。 |
|            `HFILEDESCRIPTOR_OPEN`             |         用户实现的`hfiledescriptor_open`          |            参数与返回值同``hfiledescriptor_open``            |
|           `HFILEDESCRIPTOR_OPENAT`            |        用户实现的`hfiledescriptor_openat`         |           参数与返回值同``hfiledescriptor_openat`            |
|           `HFILEDESCRIPTOR_SOCKET`            |        用户实现的`hfiledescriptor_socket`         |           参数与返回值同``hfiledescriptor_socket``           |

