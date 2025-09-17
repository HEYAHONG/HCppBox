# 说明

本rom代码主要运行于[opensbi](https://github.com/riscv-software-src/opensbi)环境，可使用[rvvm](https://github.com/LekKit/RVVM.git)或者[qemu](https://www.qemu.org/)（机器选项指定virt）测试。

本rom需要在加载[opensbi](https://github.com/riscv-software-src/opensbi)的同时直接加载至内存中。

本目录的代码均需要单独编译.

本目录代码中，采用[CMake](https://cmake.org)作为源代码管理工具。

# 编译

## 编译环境

rom代码采用`riscv64-unknown-elf-gcc`作为C编译器,采用CMake作为源代码管理工具.

在ubuntu24.04及更新版本的系统,可使用以下命令安装编译环境:

```bash
sudo apt-get install build-essential cmake gcc-riscv64-unknown-elf gcc g++
```

对于Windows用户,可尝试安装[MSYS2](http://msys2.org)并手动安装`riscv64-unknown-elf-gcc`、`gcc`、`g++`、`cmake`等工具。

## 编译ROM代码

与其它CMake工程一样，编译ROM代码分为以下步骤：

- 进入代码目录并创建构建目录。
- 配置CMake工程。
- 构建工程。

以32位RISC-V代码为例，具体命令如下：

```bash
#进入代码目录并创建构建目录
cd rv32i
mkdir build
cd build

#配置CMake工程
cmake ../

#构建工程
cmake --build .
```



# 目录说明

- [rv32i](rv32i):RISC-V 32位ROM代码.
- [rv64i](rv64i):RISC-V 64位ROM代码.

