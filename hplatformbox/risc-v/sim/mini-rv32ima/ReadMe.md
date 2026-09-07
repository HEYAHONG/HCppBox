# 说明

本rom均适用于mini-rv32ima。

- 64MB内存:地址0x80000000,长度0x4000000(其中0x83FFC000~0x84000000用于存储设备树)
- 8250串口：地址0x10000000
- syscon:地址0x11100000,用于处理重启(0x7777)与关机(0x5555)
- clint: 地址0x1100000

注意:mini-rv32ima使用的是bin文件，不是elf文件。

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

# 使用

编译完成后，可得到bin二进制文件，可使用以下指令启动测试:

```bash
#使用hmini-rv32ima加载
hmini-rv32ima -f bin文件

#使用hmini-rv32imav2加载
hmini-rv32imav2 bin文件
```



# 目录说明

- [rv32i](rv32i):RISC-V 32位ROM代码.

