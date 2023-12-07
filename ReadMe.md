# 说明

本工程主要实现一些本人常用的C++/C功能。

C相关代码放置于[hbox](hbox)，通常只依赖于C运行库与部分OS接口,尽量使用较少的依赖以适应各种编译环境。

C++相关代码放置于[src](src),通常运行于Windows与Linux,一般不支持RTOS。


## 使用

本工程主要支持CMake构建的工程,其它类型项目请直接添加[src](src)下的cpp文件与[hbox](hbox)的C语言文件,并设置相关头文件搜索目录。

代码示例请参考[test](test)目录。

对于基于CMake的工程，在将本工程顶级目录使用`add_subdirectory`命令添加为子目录后，可使用以下自定义命令:

- hcppbox_enable(targetname):为目标名为targetname的目标启用C++相关组件。
- hbox_enable(targetname):为目标名为targetname的目标启用HBox(C相关组件)。

# 组件

## hbox

hbox 为C语言组件，详情参考[hbox](hbox)目录。

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

![test](doc/test.png)

## HCPPObjectPool

本组件主要提供一些全局指针的存储。注意：用户需要自行处理指针的有效性。

功能如下:

- 提供一个对象池用于使用字符串访问对象指针。

### 文件列表

- HCPPObjectPool.cpp  
- HCPPObjectPool.h