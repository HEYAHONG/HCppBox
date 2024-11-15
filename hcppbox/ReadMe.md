# 说明

本组件主要实现常用的C++功能。

主要用于一般编程,主要依赖标准C++运行库(如线程、锁等)，通常运行于Windows、Linux等支持完整C++运行库的操作系统,一般不支持RTOS(一般不支持完整的标准C++库，某些组件不可用)。

# 硬件要求

主要支持32位及更高位宽的通用硬件平台:

- 32位通用硬件平台
- 64位通用硬件平台

通用硬件平台一般安装有操作系统,包括但不限Windows、Linux等

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

- HCPPObjectPool.cpp  
- HCPPObjectPool.h

## HCPPThread

本组件主要对标准C++库的线程进行一定封装，使其可用作可执行对象的父对象，并使其自动执行。

### 文件列表

- HCPPThread.cpp
- HCPPThread.h

## HCPPTimer

采用线程实现的定时器(非精确定时)。

### 文件列表

- HCPPTimer.cpp
- HCPPTimer.h

## HCPPInit

辅助实现动态注册初始化函数且按照一定顺序初始化。

### 文件列表

- HCPPInit.cpp
- HCPPInit.h