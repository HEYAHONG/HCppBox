# 说明

本目录为测试目录,可直接通过CMake编译。

若本目录的子目录包含CMakeLists.txt,将会被自动加入。

# 编译

确保安装好编译环境（编译器+CMake+Ninja/Make/NMake)。

以下为bash(Msys2或者Linux的shell程序)下编译过程:

```bash
#创建目录
mkdir build
#进入目录
cd build
#使用CMake生成构建文件
cmake ../
#编译,具体根据环境的不同而不同
cmake --build .
#编译完成后即可在子目录中找到每个测试程序的可执行文件
```

# scons

在当前目录执行`scons`可通过[scons](https://scons.org)编译。

注意:scons构建并非主要构建，一般是为了测试[RT-Thread](https://www.rt-thread.org/)的scons构建。

构建完成后可在build目录找到测试程序。

# Android

Android测试见目录[Android](Android)。Android测试项目需要Android Studio及最新的Android SDK,一般情况不与其它平台通用。