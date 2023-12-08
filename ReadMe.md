# 说明

本工程主要实现一些本人常用的C++/C功能。

C相关代码放置于[hbox](hbox)，通常只依赖于C运行库与部分OS接口,尽量使用较少的依赖以适应各种编译环境。

C++相关代码放置于[hcppbox](hcppbox),通常运行于Windows与Linux,一般不支持RTOS。


## 使用

本工程主要支持CMake构建的工程,其它类型项目请直接添加[hcppbox](hcppbox)下的cpp文件与[hbox](hbox)的C语言文件,并设置相关头文件搜索目录。

代码示例请参考[test](test)目录。

对于基于CMake的工程，在将本工程顶级目录使用`add_subdirectory`命令添加为子目录后，可使用以下自定义命令:

- hcppbox_enable(targetname):为目标名为targetname的目标启用C++相关组件。
- hbox_enable(targetname):为目标名为targetname的目标启用HBox(C相关组件)。

# 组件

## hbox

hbox 为C语言组件，详情参考[hbox](hbox)目录。

## hcppbox

hcppbox为C语言组件,详情参考[hcppbox](hcppbox)目录。本组件不可单独使用,依赖于hbox组件。

