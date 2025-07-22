# 说明

本组件主要实现常用的C++功能。

主要用于一般编程,主要依赖标准C++运行库(如线程、锁等)，通常运行于Windows、Linux等支持完整C++运行库的操作系统,一般不支持RTOS(一般不支持完整的标准C++库，某些组件不可用)。

# 硬件要求

主要支持32位及更高位宽的通用硬件平台:

- 32位通用硬件平台
- 64位通用硬件平台

通用硬件平台一般安装有操作系统,包括但不限Windows、Linux等

# 宏定义

本章节所指宏定义主要指用户传递的宏定义，并非组件内部的宏定义。

宏定义通常是通过编译器命令行参数传递的,也可能是编译器预定义的,也可通过特殊的头文件(如config.h)定义。

不同的宏定义将触发不同的行为。

| 宏定义      | 说明                                          | 备注                  |
| ----------- | --------------------------------------------- | --------------------- |
| `HAVE_SDL`  | 为1时表示外部有SDL(1.x)库,某些组件将使用SDL。 | 一般由HCPPGui组件使用 |
| `HAVE_SDL2` | 为1时表示外部有SDL2库,某些组件将使用SDL2。    |                       |

# CMake变量

本章节的变量主要指CMake中的相关变量，一般由用户设定而非CMake内部变量。

变量仅当使用CMake进行构建时有效。

| 变量名                       | 说明                                                         | 备注                         |
| ---------------------------- | ------------------------------------------------------------ | ---------------------------- |
| `HCPPBOX_USE_PURE_CPLUSPLUS` | 当此变量被定义时，将不主动链接第三方库。此变量常用于可能出现冲突的场景，用户需要使用SDL2,而本组件却链接了SDL(1.x)。 | 定义同名的环境变量效果相同。 |

# 组件

## HCPPObject

本组件一般用作其它组件的基类，提供一些基础的功能。

功能如下:

- 通过重载operator new与operator delete实现对变量是否在堆上分配的判断
- 提供数据类型模板。
- 可创建对象树（父对象与子对象），当父对象被删除时自动删除在堆上分配的子对象。

### 文件列表

- HCPPObject.cpp  

- HCPPObject.h  

### 测试程序截图

![test](../doc/test.png)

## HCPPObjectPool

本组件主要提供一些全局指针的存储。注意：用户需要自行处理指针的有效性。

功能如下:

- 提供一个对象池用于使用字符串访问对象指针。

### 文件列表

- [HCPPObjectPool.cpp](HCPPObjectPool.cpp)  
- [HCPPObjectPool.h](HCPPObjectPool.h)

## HCPPThread

本组件主要对标准C++库的线程进行一定封装，使其可用作可执行对象的父对象，并使其自动执行。

### 文件列表

- [HCPPThread.cpp](HCPPThread.cpp)
- [HCPPThread.h](HCPPThread.h)

## HCPPTimer

采用线程实现的定时器(非精确定时)。

### 文件列表

- [HCPPTimer.cpp](HCPPTimer.cpp)
- [HCPPTimer.h](HCPPTimer.h)

##  HCPPSocket

对各种操作系统的套接字进行一定的封装，用于编写更加通用的套接字程序。

### 文件列表

- [HCPPSocket.cpp](HCPPSocket.cpp)
- [HCPPSocketCygwinHelper.cpp](HCPPSocketCygwinHelper.cpp)

- [HCPPSocket.h](HCPPSocket.h)

## HCPPInit

辅助实现动态注册初始化函数且按照一定顺序初始化。

### 文件列表

- [HCPPInit.cpp](HCPPInit.cpp)
- [HCPPInit.h](HCPPInit.h)

## HCPPGui

测试嵌入式GUI，非通用GUI框架。

此组件主要依赖以下第三方库：

- [SDL](https://libsdl.org/)：用于创建显示窗口，默认使用SDL1,Debian系Linux可使用`sudo apt-get install libsdl-dev`安装sdl1.2库。

### 文件列表

- [HCPPGui.cpp](HCPPGui.cpp)
- [HCPPGui.h](HCPPGui.h)

### 宏定义

可外部配置的宏定义如下:

| 宏定义           | 说明                  | 备注                                                 |
| ---------------- | --------------------- | ---------------------------------------------------- |
| `HCPPGUI_NO_GUI` | 默认不使用HCPPGui组件 | 推荐在不使用HCPPGui时(尤其是使用其它GUI框架时)定义。 |