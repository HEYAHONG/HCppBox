# 说明

本目录主要用于辅助实现[simplegui](https://gitee.com/Polarix/simplegui.git)。

# 目录说明

- [3rdparty](3rdparty):第三方源代码。
- [port](port):移植代码。

# 头文件目录

若不采用CMake，需要将下列目录添加至头文件搜索目录：

-  [port](port)
- [3rdparty/source](3rdparty/source)

# 源代码文件目录

若不采用CMake，需要将下列目录的文件（不包括子目录）添加至源代码树。

-  [port](port)
- [3rdparty](3rdparty)

# 可配置的宏定义

|             宏定义             |       说明        |                         备注                         |
| :----------------------------: | :---------------: | :--------------------------------------------------: |
| `HSIMPLEGUI_USING_SYSTEM_SIMPLEGUI` | 使用系统simplegui库 | 需要手动引入第三方库，只定义此宏定义可能不能通过编译 |

