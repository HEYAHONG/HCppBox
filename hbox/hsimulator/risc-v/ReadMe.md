# ***尚未实现*** 

TODO:

- 特权指令
- 浮点指令集

# 说明

本目录主要用于实现对RISC-V指令集的模拟，主要实现在低端嵌入式平台下对RISC-V裸机环境(主要偏向模拟指令执行情况而非实际硬件)的模拟。

对于非低端嵌入式平台上可采用[riscv-isa-sim](https://github.com/riscv-software-src/riscv-isa-sim.git)工具，本目录也参考其实现相应功能。

对于系统级模拟(一般运行在非低端嵌入式平台)，推荐采用[qemu](https://www.qemu.org/)中对riscv32与riscv64的支持。

RISC-V的指令集分为基本指令集+扩展指令集，可相对灵活地选择自己的实现。

# 编译器/开发环境

对于RISC-V指令集的程序开发，主要采用以下编译器：

- [GCC](https://gcc.gnu.org/)
- [LLVM](https://llvm.org/)

## 裸机开发

裸机开发时，无论是32位开发还是64位开发，推荐采用目标为riscv64-unknown-elf的gcc工具链，此时C编译器的命令为`riscv64-unknown-elf-gcc`。C库采用[picolibc](https://github.com/picolibc/picolibc.git)或者Newlib。注意：如果是自己制作编译工具链，需要编译32位库支持，否则可能不能使用C库（可能可以编译无需C库的代码）。

在ubuntu24.04及更新的ubuntu系统可采用以下命令安装：

```bash
sudo apt-get install gcc-riscv64-unknown-elf
```

对于GCC而言(通常Clang兼容相应选项)，需要使用以下选项指定相应的指令集：

- **`-march`**:指定指令集，通常是基本指令集+扩展指令集，如rv32i指RV32I指令集，rv32ia值RV32I指令集+RV32A扩展指令集。
- **`-mcpu`**：指定处理器，通常用于具体的硬件处理器（或者处理器系列）。
- **`-mtune`**:指定处理器优化，通常用于具体的硬件处理器。
- **`-mabi`**:指定ABI，在RISC-V上常见选项为ilp32、ilp32d、ilp32e、ilp32f、lp64、lp64d、lp64e、lp64f。此选项需要配合指令集使用，如64位的ABI通常需要64位的指令集，当进行32位程序编写时需要指定32位的ABI。需要浮点运算的ABI需要浮点运算的扩展指令集。

# 模型

## 地址空间

RISC-V通常采用统一编址，即所有主内存、I/O设备均在一块以字节编址的空间上。

地址空间的大小由XLEN决定，如32位系统的寻址空间大小为4GB，64位系统的寻址空间大小为16EB。

按照一般经验,地址的高位(根据实际需求决定数量)通常可用于特殊用途(如区分主内存与I/O设备、对于64位系统而言，也可用作特殊标志位),剩余低位用作具体内存或I/O设备的地址。

## 中断(异常)

RISC-V将狭义的异常与中断均称为异常，在本章节中若未特殊说明，异常指狭义的异常,即不包括中断。

注意：对于RISC-V的向量表而言，与其它指令集不同，存储的不是函数地址，而是指令（通常是`J`指令）。

中断与异常的区别之一时，异常是同步的(通常当一条指令产生异常时，需要立即进行处理)，中断通常是异步的(即中断无需在当前指令执行后立即处理)

RISC-V中断模式可分为以下模式(向量基地址寄存器`mtvec`低两位用于编码模式，其余位标识基地址(4字节对齐)):

- 模式0:直接模式。只有一个陷入函数用于处理中断(异常)，具体的中断号在软件中判断。中断陷入函数地址存储在向量基地址。
- 模式1:向量模式。具有一个4字节对齐的向量表。具体中断函数地址存储在向量基地址+4*(中断异常情况编码),而狭义的异常处理函数地址则由存储在向量基地址(即中断号为0的情况)。

中断号(中断异常情况编码)的分配如下:

- 中断号0到中断号15由RISC-V标准使用,共16个中断号。
- 中断号16到中断号23由（硬件）平台使用,共8个中断号。
- 中断号24到中断号31由用户自定义使用,共8个中断号。
- 中断号32到中断号47由（硬件）平台使用，一般在64位及更高位数的系统中有效,共16个中断号。
- 中断号48及更大的中断号由用户自定义使用，一般在64位及更高位数的系统中有效。

## 虚拟CPU模型

**注意:若未说明，本章节内容默认为自定义内容，而非RISC-V标准内容**

### 基础模型

本模型主要用于模拟RISC-V指令的执行。主要特点如下:

- 无内存管理单元(MMU)。
- 仅工作在机器模式(M-Mode)。
- 主要运行裸机环境代码。
- 内存占用小，可运行于低端嵌入式环境。

#### 地址空间

无论是32位系统还是64位系统，均采用32位地址空间(64位系统忽略高32位地址)。

若未特殊说明，默认将采用小端模式。

地址高4位用于划分地址用途，4G地址空间被划分为16块，具体划分如下:

| 高4位值(HEX值) |    地址范围(HEX值)    | 说明                                                     |
| :------------: | :-------------------: | -------------------------------------------------------- |
|       0        | 0x00000000-0x0FFFFFFF | 可映射的程序代码块。可映射为Flash或者内部ROM(如果有的话) |
|       1        | 0x10000000-0x1FFFFFFF | Flash映射地址。                                          |
|       2        | 0x20000000-0x2FFFFFFF | RAM映射地址。                                            |
|       D        | 0xD0000000-0xDFFFFFFF | 内部ROM(如果有的话)映射地址                              |
|       E        | 0xE0000000-0xEFFFFFFF | I/O设备映射地址。                                        |
|       F        | 0xF0000000-0xFFFFFFFF | 虚拟CPU信息地址，如一些内部的寄存器。                    |

##### 可映射的程序代码块

可选映射为Flash或者内部ROM(如果有的话)。

Flash主要存放用户程序代码，其中内部ROM可以存放bootloader也可以同时存放内部库函数。

##### Flash

Flash主要存放用户程序代码,可映射到0x00000000。

##### RAM

RAM主要用于存储数据。

##### 内部ROM

可映射到0x00000000。

可不实现。

可以存放bootloader也可以同时存放内部库函数。

当存放内部库函数，需要在中断向量表后接库函数向量表。

##### I/O设备

I/O设备地址主要用于访问外设的寄存器。

寄存器的地址通常是4字节对齐的，即寄存器的大小为32位及其整倍数。

I/O设备地址低16位用于外设内部寄存器地址，剩余高16位(最高4位固定为E)用于外设基址。

##### 虚拟CPU信息

提供内部寄存器的统一访问方式或者其它信息。

当多个虚拟CPU使用使用同一个地址空间时，此部分需要单独实现（不与其它CPU共享）

寄存器的地址为16字节对齐（为RV128I预留），小端模式，未使用的字节默认读为0，不可写。

|       地址范围        | 说明                                                         | 备注                                                         |
| :-------------------: | ------------------------------------------------------------ | ------------------------------------------------------------ |
| 0xF0000000-0xF00001FF | X0~X31寄存器。共32个寄存器。位[0 3]为用于寄存器内部寻址,位[4 8]为寄存器地址。 | 必须实现。通常可读写。                                       |
| 0xF0000200-0xF000020F | PC寄存器。                                                   | 必须实现。通常可读写。                                       |
| 0xF0000210-0xF000020F | XLEN寄存器。                                                 | 必须实现。通常只读。                                         |
| 0xF0001000-0xF00011FF | f0~f31寄存器。共32个寄存器。位[0 3]为用于寄存器内部寻址,位[4 8]为寄存器地址。 | 可选实现。通常可读写。                                       |
| 0xF0001200-0xF000120F | FCSR寄存器。                                                 | 可选实现。通常可读写。                                       |
| 0xF0001210-0xF000120F | FLEN寄存器。                                                 | 可选实现。通常只读。                                         |
| 0xF1000000-0xF100FFFF | CSR寄存器，共4096个寄存器。位[0 3]为用于寄存器内部寻址,位[4 15]为CSR寄存器地址。 | 可根据实际需要部分实现。                                     |
| 0xF2000000-0xF2FFFFFF | 设备树(Device Tree )二进制存放地址。用于描述I/O设备与虚拟CPU信息。 | 可选实现。由用户提供，通常只读。可配合Device Tree Overlay技术用于加载相应驱动。 |



#### 中断

向量基地址固定为0x00000001,即基地址为0，默认为向量模式,不可修改。

为简化中断的处理，中断服务程序的返回必须使用`MRET`指令或者`SRET`指令，如果未实现相应特权级别，不产生与特权级别相关的异常（RISC-V标准中规定的异常），`MRET`指令或者`SRET`指令仅用于标记中断服务程序的结束且返回正常状态。

在基础模型中，最少需要实现异常处理与复位中断。

| 中断号 |       说明       | 备注                                               |
| :----: | :--------------: | -------------------------------------------------- |
|   0    |     异常处理     | 在RISC-V标准中为保留，本模型中实际为异常处理函数。 |
|   1    |   监管者软中断   | RISC-V标准                                         |
|   3    |    机器软中断    | RISC-V标准                                         |
|   5    | 监管者定时器中断 | RISC-V标准                                         |
|   7    |  机器定时器中断  | RISC-V标准                                         |
|   9    |  监管者外部中断  | RISC-V标准                                         |
|   11   |   机器外部中断   | RISC-V标准                                         |
|   16   |     复位中断     | 复位时进入此向量所指的函数。                       |

#### 初始化状态

- SP(X2)寄存器的值需要初始化到内存末尾,即由硬件将栈指针设置到内存末尾。

# ROM

rom指内置的程序，见[rom](rom)目录,通常需要单独编译。

rom均采用基本指令集编译尽可能采用较小的内存,主要目标为实现一些辅助启动的功能.

# 官方资料

- 官网：https://riscv.org/
- [riscv-opcodes](https://github.com/riscv/riscv-opcodes)
- [riscv-isa-manual](https://github.com/riscv/riscv-isa-manual)

