# ***尚未实现*** 

# 说明

本目录主要用于实现对RISC-V指令集的模拟，主要实现对裸机环境的模拟。

RISC-V的指令集分为基本指令集+扩展指令集，可相对灵活地选择自己的实现。

# 编译器/开发环境

对于RISC-V指令集的程序开发，主要采用以下编译器：

- [GCC](https://gcc.gnu.org/)
- [LLVM](https://llvm.org/)

## 裸机开发

裸机开发时，无论是32位开发还是64位开发，推荐采用目标为riscv64-unknown-elf的gcc工具链，此时C编译器的命令为riscv64-unknown-elf-gcc。C库采用[picolibc](https://github.com/picolibc/picolibc.git)或者Newlib。注意：如果是自己制作编译工具链，需要编译32位库支持，否则可能不能使用C库（可能可以编译无需C库的代码）。

在ubuntu24.04及更新的ubuntu系统可采用以下命令安装：

```bash
sudo apt-get install gcc-riscv64-unkown-elf
```

对于GCC而言(通常Clang兼容相应选项)，需要使用以下选项指定相应的指令集：

- **`-march`**:指定指令集，通常是基本指令集+扩展指令集，如rv32i指RV32I指令集，rv32ia值RV32I指令集+RV32A扩展指令集。
- **`-mcpu`**：指定处理器，通常用于具体的硬件处理器（或者处理器系列）。
- **`-mtune`**:指定处理器优化，通常用于具体的硬件处理器。

# 官方资料

- 官网：https://riscv.org/
- [riscv-opcodes](https://github.com/riscv/riscv-opcodes)
- [riscv-isa-manual](https://github.com/riscv/riscv-isa-manual)

