# 说明

本组件主要实现常用的C++功能。

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