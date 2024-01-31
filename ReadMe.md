# 说明

本工程为工具集,主要实现一些本人常用的C++/C功能。


## 使用

本工程主要支持CMake构建的工程,其它类型项目请直接添加相应组件的C++/C源代码文件,并设置相关头文件搜索目录,并视情况定义相关宏定义。

对于基于CMake的工程，在将本工程顶级目录使用`add_subdirectory`命令添加为子目录后，可使用以下自定义命令:

- hcppbox_enable(targetname):为目标名为targetname的目标启用HCppBox。
- hbox_enable(targetname):为目标名为targetname的目标启用HBox。
- hrc_enable(targetname):为目标名为targetname的目标启用HRC。

代码示例请参考[test](test)目录。

# 组件

## hbox

hbox 为以C语言为主要语言的组件，详情参考[hbox](hbox)目录。

本组件主要用于嵌入式编程，通常只依赖于C运行库与部分OS接口,尽量使用较少的依赖以适应各种编译环境。

编译器需支持的最低标准:

- C语言标准:C99
- C++语言标准:C++11

## hcppbox

hcppbox为以C++为主要语言的组件,详情参考[hcppbox](hcppbox)目录。

本组件不可单独使用,依赖于hbox组件。

本组件主要用于一般编程,主要依赖标准C++运行库(如线程、锁等)，通常运行于Windows、Linux等支持完整C++运行库的操作系统,一般不支持RTOS(一般不支持完整的标准C++库，某些组件不可用)。

编译器需支持的最低标准:

- C语言标准:C99
- C++语言标准:C++23

## hrc

RC(Resource Compiler)主要用于将各种文件资源编译到应用程序。

本组件类似于桌面程序的资源文件，主要针对嵌入式编程。

源代码实现的目录为 [hrc](hrc)。

在固件编写中，很多时候需要大量的固定数据，直接手工嵌入到C文件里比较麻烦。

通过读取文件转换到对应C文件可大大节省时间，可添加常用的文件（如各种证书）或者不适宜放在可读写的文件系统中的文件(如需要在格式化中保留或者初始参数)。转换程序源代码为[hrc/fsgen.cpp](hrc/fsgen.cpp)。

使用步骤如下:

- 将待添加的文件放入 [hrc/fs](hrc/fs)目录下。

- 使用文件名调用以下函数(需包含相应头文件hrc.h):

  ```c++
  //通过名称获取资源大小
  size_t RCGetSize(const char * name);
  
  //通过名称获取资源指针
  const unsigned char * RCGetHandle(const char * name);
  ```

本组件对构建主机(build host)的要求如下：

- Windows主机(x86、x86_64)。
- 安装有g++的其它操作系统。

编译器需支持的最低标准:

- C语言标准:C99
- C++语言标准:C++11