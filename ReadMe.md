# 说明

本工程主要实现一些本人常用的C++功能。


## 使用


本工程主要支持CMake构建的工程,其它类型项目请直接添加[src](src)下的cpp文件。

代码示例请参考[test](test)目录。

# 组件

## HCPPObject

本组件一般用作其它组件的基类，提供一些基础的功能。

功能如下:

- 通过重载operator new与operator delete实现对变量是否在堆上分配的判断

### 文件列表

- HCPPObject.cpp  

- HCPPObject.h  

### 测试程序截图

![test](doc/test.png)

## HCPPObjectPool

本组件主要提供一些全局指针的存储。

功能如下:

- 提供一个对象池用于使用字符串访问对象指针。

### 文件列表

- HCPPObjectPool.cpp  
- HCPPObjectPool.h