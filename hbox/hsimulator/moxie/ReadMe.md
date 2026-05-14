# 说明

[moxie](http://moxielogic.org)是通用嵌入式软核处理器。

其特点如下：

- 16个32位寄存器。
- 最多可访问256个32位特殊寄存器。
- 指令集设计简单，可用于虚拟化环境。
- 可使用GNU 编译器套件。

# 模型

## 寄存器

### 通用寄存器

moxie定义16个寄存器。

| 寄存器引索 | 名称         | 说明       |
| ---------- | ------------ | ---------- |
| 0          | `$fp`        |            |
| 1          | `$sp`        |            |
| 2-15       | `$r0`-`$r13` | 通用寄存器 |

### PC寄存器

32位PC寄存器，理论不能直接访问，可通过某些指令改变。

### 特殊寄存器

特殊寄存器仅可通过`gsr`与`ssr`指令访问，最多可支持256个特殊寄存器。

已定义的特殊寄存器如下:

| 特殊寄存器 | 描述                                                         |
| ---------- | ------------------------------------------------------------ |
| 0          | status register                                              |
| 1          | a pointer to the Exception Handler routine (invoked by `swi`, IRQs, Divide by Zero and illegal instructions (`bad`)) |
| 2          | upon invocation of the Excecption Handler .                  |
| 3          | the `swi` request number (by convention)                     |
| 4          | address of the supervisor mode stack on which exceptions are executed |
| 5          | return address upon entering the exception handler           |
| 6          | reserved                                                     |
| 7          | reserved                                                     |
| 8          | reserved                                                     |
| 9          | an optional non-zero pointer to the Device Tree blob describing this device |

#### 特殊寄存器2

特殊寄存器2用于指示异常原因.

已定义的异常原因如下:

| 名称              | 值   | 说明               |
| ----------------- | ---- | ------------------ |
| MOXIE_EX_DIV0     | 0    | 除0错误            |
| MOXIE_EX_BAD      | 1    | 指令错误(非法指令) |
| MOXIE_EX_IRQ      | 2    | 中断请求           |
| MOXIE_EX_SWI      | 3    | 软件中断           |
| MOXIE_EX_MMU_MISS | 4    | MMU未命中          |
| MOXIE_EX_BREAK    | 16   |                    |

## 指令集

### 指令集格式

moxie指令采用16位宽度，可带16位或者32位立即数。

moxie指令可根据最高2位分为以下格式:

| 指令[14 15]值 | 说明              | 备注 |
| ------------- | ----------------- | ---- |
| 0b11          | 位[10 13]为操作码 |      |
| 0b10          | 位[12 13]为操作码 |      |
| 0b00、0b01    | 位[8 14]为操作码  |      |

### 指令

All instructions are 16-bits long.  Some 16-bit instructions are followed by a 32-bit immediate value.  All of the opcode space not consumed by the encodings below is filled with the `bad` instruction.

具体见[instructions.md](instructions.md)

## 虚拟CPU-moxiebox

[moxiebox](https://github.com/atgreen/moxiebox.git)是一个moxie架构的沙箱。

### 地址空间

moxiebox具有一个空白的的32位地址空间。

所有的代码及数据空间均由ELF可执行文件中的虚拟地址指定。

数据文件中的数据加载在ELF可执行文件后的对齐地址上。

### 中断（异常）

swi指令可触发软中断。但由于moxiebox为沙箱，因此并不执行ELF文件中的中断处理程序,而是由moxiebox模拟实现相应功能。

| swi中断号（系统调用号） | 说明     | 备注                       |
| ----------------------- | -------- | -------------------------- |
| 0                       | 退出     |                            |
| 90                      | 执行mmap | 可用于在沙箱内动态申请内存 |

### 初始状态

- 入口地址由ELF可执行文件的入口地址指定，默认情况下,地址0x00001000用于程序入口地址。
- 特殊寄存器7存储初始栈地址。
- 特殊寄存器6存储内存描述表，内存描述表可作为程序的输入。

# 工具链

虽然在官方的[moxiebox](https://github.com/atgreen/moxiebox.git)中提供了GNU 编译器套件的编译脚本，但也可使用[crosstool-ng](https://github.com/crosstool-ng/crosstool-ng)的moxie-unknown-moxiebox样例直接编译出最新的工具链。

# 官方资料

- http://moxielogic.org/blog/
- https://github.com/atgreen