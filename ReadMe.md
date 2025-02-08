# 说明

本工程为工具集,主要实现一些本人常用的C++/C功能。


## 使用

本工程主要支持CMake构建的工程,其它类型项目请直接添加相应组件的C++/C源代码文件,并设置相关头文件搜索目录,并视情况定义相关宏定义。

对于基于CMake的工程，在将本工程顶级目录使用`add_subdirectory`命令添加为子目录后，可使用以下自定义命令:

- hcppbox_enable(targetname):为目标名为targetname的目标启用HCppBox。
- hbox_enable(targetname):为目标名为targetname的目标启用HBox。
- hrc_enable(targetname):为目标名为targetname的目标启用HRC。

## 编译器支持

注意:应当尽可能使用最新版编译器。

- GCC4.9.0及更新版本:https://gcc.gnu.org/
- Clang 3.4 及更新版本:https://clang.llvm.org/

### 可测试的其他编译器/IDE环境

通过CMake,可生成其它编译器/IDE环境的工程文件，如MSVC。

#### MSVC

MSVC的编译器对C/C++标准支持随着版本的升级而逐渐完善，较为古老的IDE环境可能不支持本工程。

- 推荐测试需要C99标准的IDE：VS2015及更新版本的IDE。
- 推荐测试需要C++11标准的IDE：VS 2019 16.8及更新版本的IDE.

# 测试

如需使用各种IDE/编辑器（如[vscode](https://code.visualstudio.com/)）打开源代码，请不要直接打开当前目录，而是打开测试目录，这样才可以顺利运行测试例程。

测试目录见[test](test)目录。

# 组件

## hbox

hbox 为以C语言为主要语言的组件，详情参考[hbox](hbox)目录。

本组件主要用于嵌入式编程，通常只依赖于C运行库与部分OS接口,尽量使用较少的依赖以适应各种编译环境。

编译器需支持的最低标准:

- C语言标准:C99 (某些特性需要C11支持)
- C++语言标准:C++11

## hcppbox

hcppbox为以C++为主要语言的组件,详情参考[hcppbox](hcppbox)目录。

本组件不可单独使用,依赖于hbox组件。

本组件主要用于一般编程,主要依赖标准C++运行库(如线程、锁等)，通常运行于Windows、Linux等支持完整C++运行库的操作系统,一般不支持RTOS(一般不支持完整的标准C++库，某些组件不可用)。

编译器需支持的最低标准:

- C语言标准:C23
- C++语言标准:C++23

## hrc

RC(Resource Compiler)主要用于将各种文件资源编译到应用程序。

本组件类似于桌面程序的资源文件，主要针对嵌入式编程。

源代码实现的目录为 [hrc](hrc)。

编译器需支持的最低标准:

- C语言标准:C99
- C++语言标准:C++11