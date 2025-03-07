# 说明

本rom均采用基本指令集编译,尽可能采用较小的内存,主要目标为实现一些辅助启动的功能.

本目录的代码均需要单独编译.

默认情况下,rom配置如下:

- Flash空间:   起始地址=0xD0000000,大小=2MB
- RAM空间:    起始地址=0x20000000,大小=8KB

本目录代码中，采用[CMake](https://cmake.org)作为源代码管理工具。

# RISC-V官方模拟器

本ROM已经修改了相关地址，无法在RISC-V官方模拟器中运行。

如需在官方模拟器中运行，需要进行以下修改：

- 修改Flash在地址至0x80000000。
- 修改RAM地址到Flash后，若Flash大小为0x00200000则RAM地址为0x80200000。
- 在模拟器的命令参数中指定PC值为0x80000400。

# 向量表

向量表通常存放中断向量表，在本代码中也存放库函数相关配置（可存放库函数参数/或者函数地址）。

本ROM代码中，向量表共256项，其中前64项为中断向量表，其余为库函数相关配置。

向量表的实现见各个ROM代码的vector.cpp。

注意：

- 除标记为裸函数（自行采用汇编处理）的函数外，陷入函数与中断服务函数需要标记为中断，即需要使用`__attribute__((interrupt))`标记函数。根据特权级别的不同，可将中断分别标记为机器级别(`interrupt ("machine")` )、监管者级别(`interrupt ("supervisor")` )、用户级别(`interrupt ("user")` )。
- 中断向量表中通常存放`J`指令。

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

