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

编译完成后,可得rom.bin文件

# 测试

本rom代码需要opensbi环境支持，因此因此用户需要自行准备对应环境的fw_payload.bin与fw_jump.bin.

下列命令均为最少需要使用的选项，用户可根据实际需要增添其他选项。

## rvvm

### 32位RISC-V

```bash
rvvm -rv32 -kernel rom.bin路径 fw_payload.bin路径
```

### 64位RISC-V

```bash
rvvm -kernel rom.bin路径 fw_payload.bin路径
```

## qemu

### 64位RISC-V

```bash
qemu-system-riscv64 -M virt -bios fw_jump.bin路径 -kernel rom.bin路径 -serial stdio
```

# 目录说明

- [rv32i](rv32i):RISC-V 32位ROM代码.
- [rv64i](rv64i):RISC-V 64位ROM代码.

