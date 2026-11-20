# 说明

本组件用于加载可执行代码到内存中(并不负责执行这些代码)。  

 **注意：本组件加载的代码可供本地执行，也可供模拟器使用。**

#  组件

## hintelhex

读取或者写入intelhex文件。

intelhex格式通常用于嵌入式领域,其可用于存储程序文件(如MCU)也可用于存储数据(如EEPROM数据)。

具体实现见[hintelhex](hintelhex)。

## hsrecord

读取或者写入Motorola S记录(srecord)文件。

srecord格式通常用于嵌入式领域,其可用于存储程序文件(如MCU)也可用于存储数据(如EEPROM数据)。

具体实现见[hsrecord](hsrecord)。

## hcoff

辅助读取COFF/PE文件。

**注意:**

- COFF文件一般用于存储可执行文件映像或者对象文件,是一种可跨平台的文件格式。
- 随着的发展，现在常见于在Windows下编译C语言源代码所产生的对象文件（无论是MSVC、GCC还是Clang）。
- 本组件可用于处理COFF可重定位对象文件(可用于操作系统无关的模块，如各种内核、Bootloader的模块)。
- 本组件同时支持读取PE格式的动态链接库(DLL)与可执行文件(EXE),包括PE32与PE32+,可解析文件头、节表、导出表、导入表、基址重定位、TLS目录等,并可将PE映像映射到内存、解析导入符号。
- 已支持的可重定位对象架构:i386、AMD64、AArch64、ARM/Thumb、RISC-V 32/64、LoongArch64。

具体实现见[hcoff](hcoff)。

## helf

辅助读取ELF文件。

具体实现见[helf](helf)。

