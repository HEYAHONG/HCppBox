# ***尚未实现*** 

# 说明

本目录主要用于实现MCS-51模拟器。

对于非嵌入式平台可采用SDCC自带的[ucsim](https://www.ucsim.hu/)工具。

本目录参考以下模拟器代码：

- [ucsim](https://www.ucsim.hu/)
- [emu8051](https://github.com/jarikomppa/emu8051.git)

# 编译器/开发环境

- Keil C51:MCS-51 集成开发环境。
- [SDCC](https://sdcc.sourceforge.net/)：支持MCS-51的编译器。
- [mcu8051ide](https://sourceforge.net/projects/mcu8051ide/):MCS-51 集成开发环境,采用SDCC，支持模拟器，支持外设（如LED显示）。

编译器推荐使用SDCC，可采用部分较新的C语言特性。

# 模型

注意：此章节的说明适用于SDCC,不适用于Keil C51。

## 地址空间

![sdcc_address_spaces](sdcc_address_spaces.png)

- 内部RAM地址空间：`__data、__idata`
- 外部RAM地址空间:`__xdata、__pdata`
- 程序ROM地址空间:`__code`

## 内存模型

内存模型只影响默认的变量分配（如参数与局部变量），若用户指定变量存储位置，则不受影响。

### 极简模型

- 128字节RAM(只可直接寻址)
- 128字节SFR(只可直接寻址)
- ROM空间不限（小于64KB）

此模型没有特定的编译器选项，可使用`--model-small  --iram-size 128`指定此模型。

### model-small

- 256字节RAM（高128字节需要间接寻址,低128字节可直接寻址也可间接寻址）
- 128字节SFR(只可直接寻址)
- ROM空间不限（小于64KB）

可使用`--model-small  --iram-size 256`指定此模型。

### model-medium

- 256字节RAM（高128字节需要间接寻址,低128字节可直接寻址也可间接寻址）
- 256字节外部RAM(也可小于256字节)
- 128字节SFR(只可直接寻址)
- ROM空间不限（小于64KB）

可使用`--model-medium  --xram-size 外部RAM字节数`指定此模型。

### model-large

- 256字节RAM（高128字节需要间接寻址,低128字节可直接寻址也可间接寻址）
- 64KB外部RAM(可小于64KB)
- 128字节SFR(只可直接寻址)
- ROM空间不限（小于64KB）

可使用`--model-large  --xram-size 外部RAM字节数`指定此模型。

### model-huge

- 256字节RAM（高128字节需要间接寻址,低128字节可直接寻址也可间接寻址）
- 64KB外部RAM(可小于64KB)
- 128字节SFR(只可直接寻址)
- ROM空间不限（大于64KB，ROM空间被分为两块，0-0x7FFF的映射不可改变,0x8000-0xFFFF可通过某个SFR切换映射关系以突破64KB限制）

可使用`--model-huge  --xram-size 外部RAM字节数`指定此模型。

## 时钟

默认情况下，主时钟频率为11.0592M。

用户可通过宏定义`HS_MCS_51_COMMON_CLK_FREQ` 修改此值。

注意：此时钟仅用于某些外设（如串口、定时器等）的计算，并不要求模拟器的时钟节拍的调用频率一定为此时钟频率。

## 基础外设(8031/8051外设)

MCS-51的经典外设如下:

- 定时器：至少2个16位定时器。
- 串口:具有一个串口外设。

对于各种基于8051内核的单片机而言，或许有更多类型的外设，但不在此章节描述。

### 串口

#### 寄存器概述

| 名称 | SFR地址 | 说明                                         |
| :--: | :-----: | :------------------------------------------- |
| SCON |   98H   | 串口控制                                     |
| PCON |   87H   | 电源控制                                     |
|  IE  |   A8H   | 中断使能                                     |
|  IP  |   B8H   | 中断优先级                                   |
| SBUF |   99H   | 串口缓冲，写入/读取此寄存器表示发送/接收数据 |

#### SCON

| 名称 | 位(相对与寄存器) | 说明                                                         |
| :--: | :--------------: | ------------------------------------------------------------ |
| SM0  |        7         | 与SM1共同选择工作模式。                                      |
| SM1  |        6         | 与SM0共同选择工作模式。模式0=同步位移模式，模式1=8位UART(波特率可变)，模式2=9位UART，模式3=9位UART(波特率可变)。可变波特率时由定时器决定波特率。 |
| SM2  |        5         | 在工作模式2与3时，选择多机通信还是单机通信，SM2=1时选择多机模式，SM2=0时选择双机模式。模式0需要清0。多机模式下，只接收RB8=1的地址数据。 |
| REN  |        4         | 接收使能。REN=1时允许接收。                                  |
| TB8  |        3         | 发送数据第9位。                                              |
| RB8  |        2         | 接收数据第9位。                                              |
|  TI  |        1         | 发送中断请求。TI=1时有中断请求。                             |
|  RI  |        0         | 接收中断请求。RI=1时有中断请求。                             |

注意:

- 双机（一对一）模式下,SM2必须清零。
- 多机(一主多从)模式下,主机发送地址数据时,SM2必须置位,接收数据时SM2必须清零，从机必须先将SM2置位接收地址数据，当接收到正确的地址后，从机才可以将SM2清零接收普通数据。

#### PCON

| 名称 | 位(相对于寄存器) | 说明                 |
| :--: | :--------------: | -------------------- |
| SMOD |        7         | SMOD=1，波特率加倍。 |

注意:

- 只描述串口相关位

#### IE

| 名称 | 位(相对于寄存器) | 说明                             |
| :--: | :--------------: | -------------------------------- |
|  EA  |        7         | 全局中断使能。EA=1全局开中断。   |
|  ES  |        4         | 串口中断使能。ES=1使能串口中断。 |

注意:

- 只描述串口相关位

#### IP

| 名称 | 位(相对于寄存器) | 说明                                       |
| :--: | :--------------: | ------------------------------------------ |
|  PS  |        4         | 串口中断优先级。PS=1时串口为高优先级中断。 |

注意:

- 只描述串口相关位

#### SBUF

8位串口数据。写入代表发送数据，读取表示接收数据。

### 外部中断与定时器0/1

#### 寄存器概述

| 名称 | SFR地址 | 说明                  |
| :--: | :-----: | :-------------------- |
| TCON |   88H   | 定时器/计数器控制     |
| TMOD |   89H   | 定时器/计数器模式控制 |
| TL0  |   8AH   | 定时器0低8位          |
| TL1  |   8BH   | 定时器1低8位          |
| TH0  |   8CH   | 定时器0高8位          |
| TH1  |   8DH   | 定时器1高8位          |
|  IE  |   A8H   | 中断使能              |
|  IP  |   B8H   | 中断优先级            |

#### TCON

| 名称 | 位(相对于寄存器) | 说明                                                 |
| :--: | :--------------: | ---------------------------------------------------- |
| TF1  |        7         | 定时器1溢出标志。当中断过程结束时自动清0。           |
| TR1  |        6         | 定时器1运行控制。                                    |
| TF0  |        5         | 定时器0溢出标志。当中断过程结束时自动清0。           |
| TR0  |        4         | 定时器0运行控制。                                    |
| IE1  |        3         | 外部中断1标志。当中断过程结束时自动清0(边沿触发时)。 |
| IT1  |        2         | 外部中断1类型控制。1=下降沿，0=低电平。              |
| IE0  |        1         | 外部中断0标志。当中断过程结束时自动清0(边沿触发时)。 |
| IT0  |        0         | 外部中断0类型控制。1=下降沿，0=低电平。              |

#### TMOD

| 名称 | 位(相对于寄存器) | 说明                                                         |
| :--: | :--------------: | ------------------------------------------------------------ |
|  G1  |        7         | 定时器1门控位。1=定时器由TR1与INT1引脚(高电平有效)共同控制(与)，0=定时器由TR1控制。 |
| CT1  |        6         | 定时器1功能选择位。1=计数器(对外部信号脉冲计数)，0=定时器(对时钟信号计数)。 |
| M11  |        5         | 定时器1工作模式位1,与M10共同选择工作模式。                   |
| M10  |        4         | 定时器1工作模式位0,与M11共同选择工作模式。0=13位定时器/计数器,1=16位定时器/计数器,2=8位自动重装载定时器/计数器,3=两个8位定时器/计数器。 |
|  G0  |        3         | 定时器0门控位。1=定时器由TR0与INT0引脚(高电平有效)共同控制(与)，0=定时器由TR0控制。 |
| CT0  |        2         | 定时器0功能选择位。1=计数器(对外部信号脉冲计数)，0=定时器(对时钟信号计数)。 |
| M01  |        1         | 定时器0工作模式位1,与M00共同选择工作模式。                   |
| M00  |        0         | 定时器0工作模式位0,与M01共同选择工作模式。0=13位定时器/计数器,1=16位定时器/计数器,2=8位自动重装载定时器/计数器,3=定时器停止。 |

#### TL0

定时器0低八位。

#### TL1

定时器1低八位。

#### TH0

定时器0高八位。当工作在工作模式2时，TH0为自动重装载的值。

#### TH1

定时器1高八位。当工作在工作模式2时，TH1为自动重装载的值。

#### IE

| 名称 | 位(相对于寄存器) | 说明                            |
| :--: | :--------------: | ------------------------------- |
|  EA  |        7         | 全局中断使能。EA=1全局开中断。  |
| ET1  |        3         | 定时器1中断使能。1=启用中断。   |
| EX1  |        2         | 外部中断1中断使能。1=启用中断。 |
| ET0  |        1         | 定时器0中断使能。1=启用中断。   |
| EX0  |        0         | 外部中断0中断使能。1=启用中断。 |

注意:

- 只描述本章节相关位

#### IP

| 名称 | 位(相对于寄存器) | 说明                              |
| :--: | :--------------: | --------------------------------- |
| PT1  |        3         | 定时器1中断优先级。1=高优先级。   |
| PX1  |        2         | 外部中断1中断优先级。1=高优先级。 |
| PT0  |        1         | 定时器0中断优先级。1=高优先级。   |
| PX0  |        0         | 外部中断0中断优先级。1=高优先级。 |

注意:

- 只描述本章节相关位

### 中断

#### 中断号

`MCS-51中断地址=(中断号*8)+3`,当需要执行中断过程时需要跳到中断地址。

| 名称 | 中断号 | 说明      |
| :--: | :----: | --------- |
| IE0  |   0    | 外部中断0 |
| TF0  |   1    | 定时器0   |
| IE1  |   2    | 外部中断1 |
| TF1  |   3    | 定时器1   |
| SI0  |   4    | 串口0中断 |

注意：

- 地址0为复位时跳转的地址(类似其它架构的复位中断)，一般在此放一条长跳转指令。

#### 中断优先级

中断优先级由相应寄存器(IP)设置。

最终的优先级顺序：

- 高优先级中断高于低优先级中断。
- 同一级优先级中断的执行顺序由硬件扫描顺序决定。

中断执行规则：

- 中断不能被低优先级/同一优先级中断打断。
- 高优先级中断可打断低优先级中断。

### IO端口

| 名称 | sfr地址 | 说明                                      |
| :--: | :-----: | ----------------------------------------- |
|  P0  |   80H   | 此IO端口可位寻址,即单独对某一个引脚操作。 |
|  P1  |   90H   | 此IO端口可位寻址,即单独对某一个引脚操作。 |
|  P2  |   A0H   | 此IO端口可位寻址,即单独对某一个引脚操作。 |
|  P3  |   B0H   | 此IO端口可位寻址,即单独对某一个引脚操作。 |

#### 8位数据总线

P0口可作为8位数据总线。

#### 16位地址总线

P0口可复用为16位地址总线低8位。

P2口可复用为16地址总线的高8位。

#### 特殊功能复用

| 功能 | 位地址 | 说明      |
| :--: | :----: | --------- |
|  RD  |  B7H   | P3口引脚7 |
|  WR  |  B6H   | P3口引脚6 |
|  T1  |  B5H   | P3口引脚5 |
|  T0  |  B4H   | P3口引脚4 |
| INT1 |  B3H   | P3口引脚3 |
| INT0 |  B2H   | P3口引脚2 |
| TXD  |  B1H   | P3口引脚1 |
| RXD  |  B0H   | P3口引脚0 |

### 80C51增强功能

#### PCON 增强功能

PCON的SFR地址为87H。

| 名称 | 位(相对于寄存器) | 说明                                                |
| :--: | :--------------: | --------------------------------------------------- |
| GF1  |        3         | 通用标志1，由用户决定用途。                         |
| GF0  |        2         | 通用标志2，由用户决定用途。                         |
|  PD  |        1         | 掉电模式。1=进入掉电模式。由复位清除。              |
| IDL  |        0         | 空闲模式标志。1=空闲模式，当有中断/复位发生时清除。 |

注意：

- 只描述增强的功能。

## 8032/8052外设

除了基础外设之外,8032/8052新增如下功能：

- 定时器2

注意：本章节只描述新增的功能。

### 定时器2

#### 寄存器概览

|  名称  | sfr地址 | 说明                       |
| :----: | :-----: | -------------------------- |
| T2CON  |   C8H   | 定时器/计数器2控制器寄存器 |
| RCAP2L |   CAH   | 捕获/重装载值低8位         |
| RCAP2H |   CBH   | 捕获/重装载值高8位         |
|  TL2   |   CCH   | 定时器2低8位               |
|  TH2   |   CDH   | 定时器2高8位               |
|   IE   |   A8H   | 中断使能                   |
|   IP   |   B8H   | 中断优先级                 |

#### T2CON

| 名称  | 位(相对于寄存器) | 说明                                                         |
| :---: | :--------------: | ------------------------------------------------------------ |
|  TF2  |        7         | 定时器2溢出标志                                              |
| EXF2  |        6         | 通过T2EX的负跳变触发捕获/重装时置位，将触发定时器2中断（若使能了定时器2中断）。 |
| RCLK  |        5         | 接收时钟。1=替代定时器1作为串口接收时钟。                    |
| TCLK  |        4         | 发送时钟。1=替代定时器1作为发送接收时钟。                    |
| EXEN2 |        3         | 定时器外部使能。0=忽略T2EX引脚，1=当未被作为串口时钟时，由T2EX引脚触发捕获/重装载。 |
|  TR2  |        2         | 定时器2运行控制。                                            |
|  CT2  |        1         | 定时器2功能选择位。1=计数器(对外部信号脉冲计数)，0=定时器(对时钟信号计数)。 |
| CPRL2 |        0         | 定时器2捕获/重装控制。当定时器2作为串口时钟时此位被忽略。0=当定时器溢出/T2EX负跳变时自动重装，1=当T2EX负跳变时捕获（若使能了EXEN2）。 |

#### RCAP2L

捕获/重装载值低8位

#### RCAP2H

捕获/重装载值高8位

#### TL2

定时器2低8位

#### TH2

 定时器2高8位

#### IE

| 名称 | 位(相对于寄存器) | 说明                           |
| :--: | :--------------: | ------------------------------ |
|  EA  |        7         | 全局中断使能。EA=1全局开中断。 |
| ET2  |        5         | 定时器2中断使能。1=启用中断。  |

注意:

- 只描述本章节相关位

#### IP

| 名称 | 位(相对于寄存器) | 说明                            |
| :--: | :--------------: | ------------------------------- |
| PT2  |        5         | 定时器2中断优先级。1=高优先级。 |

注意:

- 只描述本章节相关位

### 中断

#### 中断号

`MCS-51中断地址=(中断号*8)+3`,当需要执行中断过程时需要跳到中断地址。

| 名称 | 中断号 | 说明    |
| :--: | :----: | ------- |
| TF2  |   5    | 定时器2 |

### IO端口

#### 特殊功能复用

| 功能 | 位地址 | 说明      |
| :--: | :----: | --------- |
| T2EX |  91H   | P1口引脚1 |
|  T2  |  90H   | P1口引脚0 |

注意：

- 只描述新增的特殊功能复用。

# 指令表

```txt
指令	   汇编				字节		时钟周期数（针对原版MCS-51,不针对1T8051）
0x00	NOP					1		12
0x01	AJMP addr			2		24
0x02	LJMP addr			3		24
0x03	RR A				1		12
0x04	INC A				1		12
0x05	INC addr			2		12
0x06	INC @R0				1		12
0x07	INC @R1				1		12
0x08	INC R0				1		12
0x09	INC R1				1		12
0x0a	INC R2				1		12
0x0b	INC R3				1		12
0x0c	INC R4				1		12
0x0d	INC R5				1		12
0x0e	INC R6				1		12
0x0f	INC R7				1		12
0x10	JBC bit,addr		3		12
0x11	ACALL addr			2		24
0x12	LCALL addr			3		24
0x13	RRC A				1		12
0x14	DEC A				1		12
0x15	DEC addr			2		12
0x16	DEC @R0				1		12
0x17	DEC @R1				1		12
0x18	DEC R0				1		12
0x19	DEC R1				1		12
0x1a	DEC R2				1		12
0x1b	DEC R3				1		12
0x1c	DEC R4				1		12
0x1d	DEC R5				1		12
0x1e	DEC R6				1		12
0x1f	DEC R7				1		12
0x20	JB bit,addr			3		24
0x21	AJMP addr			2		24
0x22	RET					1		24
0x23	RL A				1		12
0x24	ADD A,#data			2		12
0x25	ADD A,addr			2		12
0x26	ADD A,@R0			1		12
0x27	ADD A,@R1			1		12
0x28	ADD A,R0			1		12
0x29	ADD A,R1			1		12
0x2a	ADD A,R2			1		12
0x2b	ADD A,R3			1		12
0x2c	ADD A,R4			1		12
0x2d	ADD A,R5			1		12
0x2e	ADD A,R6			1		12
0x2f	ADD A,R7			1		12
0x30	JNB bit,addr		3		12(?)
0x31	ACALL addr			2		24
0x32	RETI				1		24
0x33	RLC A				1		12
0x34	ADDC A,#data		2		12
0x35	ADDC A,addr			2		12
0x36	ADDC A,@R0			1		12
0x37	ADDC A,@R1			1		12
0x38	ADDC A,R0			1		12
0x39	ADDC A,R1			1		12
0x3a	ADDC A,R2			1		12
0x3b	ADDC A,R3			1		12
0x3c	ADDC A,R4			1		12
0x3d	ADDC A,R5			1		12
0x3e	ADDC A,R6			1		12
0x3f	ADDC A,R7			1		12
0x40	JC addr				2		24
0x41	AJMP addr			2		24
0x42	ORL addr,A			2		12
0x43	ORL addr,#data		3		24
0x44	ORL A,#data			2		12
0x45	ORL A,addr			2		12
0x46	ORL A,@R0			1		12
0x47	ORL A,@R1			1		12
0x48	ORL A,R0			1		12
0x49	ORL A,R1			1		12
0x4a	ORL A,R2			1		12
0x4b	ORL A,R3			1		12
0x4c	ORL A,R4			1		12
0x4d	ORL A,R5			1		12
0x4e	ORL A,R6			1		12
0x4f	ORL A,R7			1		12
0x50	JNC addr			2		24
0x51	ACALL addr			2		24
0x52	ANL addr,A			2		12
0x53	ANL addr,#data		3		24
0x54	ANL A,#data			2		12
0x55	ANL A,addr			2		12
0x56	ANL A,@R0			1		12
0x57	ANL A,@R1			1		12
0x58	ANL A,R0			1		12
0x59	ANL A,R1			1		12
0x5a	ANL A,R2			1		12
0x5b	ANL A,R3			1		12
0x5c	ANL A,R4			1		12
0x5d	ANL A,R5			1		12
0x5e	ANL A,R6			1		12
0x5f	ANL A,R7			1		12
0x60	JZ addr				2		24
0x61	AJMP addr			2		24
0x62	XRL addr,A			2		12
0x63	XRL addr,#data		3		24
0x64	XRL A,#data			2		12
0x65	XRL A,addr			2		12
0x66	XRL A,@R0			1		12
0x67	XRL A,@R1			1		12
0x68	XRL A,R0			1		12
0x69	XRL A,R1			1		12
0x6a	XRL A,R2			1		12
0x6b	XRL A,R3			1		12
0x6c	XRL A,R4			1		12
0x6d	XRL A,R5			1		12
0x6e	XRL A,R6			1		12
0x6f	XRL A,R7			1		12
0x70	JNZ addr			2		24
0x71	ACALL addr			2		24
0x72	ORL C,addr			2		24
0x73	JMP @A+DPTR			1		24
0x74	MOV A,#data			2		12
0x75	MOV addr,#data		3		24
0x76	MOV @R0,#data		2		12
0x77	MOV @R1,#data		2		12
0x78	MOV R0,#data		2		12
0x79	MOV R1,#data		2		12
0x7a	MOV R2,#data		2		12
0x7b	MOV R3,#data		2		12
0x7c	MOV R4,#data		2		12
0x7d	MOV R5,#data		2		12
0x7e	MOV R6,#data		2		12
0x7f	MOV R7,#data		2		12
0x80	SJMP addr			2		24
0x81	AJMP addr			2		24
0x82	ANL C,addr			2		24
0x83	MOVC A,@A+PC		1		24
0x84	DIV AB				1		48
0x85	MOV addr,addr		3		24
0x86	MOV addr,@R0		2		24
0x87	MOV addr,@R1		2		24
0x88	MOV addr,R0			2		24
0x89	MOV addr,R1			2		24
0x8a	MOV addr,R2			2		24
0x8b	MOV addr,R3			2		24
0x8c	MOV addr,R4			2		24
0x8d	MOV addr,R5			2		24
0x8e	MOV addr,R6			2		24
0x8f	MOV addr,R7			2		24
0x90	MOV DPTR,#data		3		24
0x91	ACALL addr			2		24
0x92	MOV addr,C			2		24
0x93	MOVC A,@A+DPTR		1		24
0x94	SUBB A,#data		2		12
0x95	SUBB A,addr			2		12
0x96	SUBB A,@R0			1		12
0x97	SUBB A,@R1			1		12
0x98	SUBB A,R0			1		12
0x99	SUBB A,R1			1		12
0x9a	SUBB A,R2			1		12
0x9b	SUBB A,R3			1		12
0x9c	SUBB A,R4			1		12
0x9d	SUBB A,R5			1		12
0x9e	SUBB A,R6			1		12
0x9f	SUBB A,R7			1		12
0xa0	ORL C,/addr			2		24
0xa1	AJMP addr			2		24
0xa2	MOV C,addr			2		12
0xa3	INC DPTR			1		24
0xa4	MUL AB				1		48
0xa5	****************Breakpoint
0xa6	MOV @R0,addr		2		24
0xa7	MOV @R1,addr		2		24
0xa8	MOV R0,addr			2		24
0xa9	MOV R1,addr			2		24
0xaa	MOV R2,addr			2		24
0xab	MOV R3,addr			2		24
0xac	MOV R4,addr			2		24
0xad	MOV R5,addr			2		24
0xae	MOV R6,addr			2		24
0xaf	MOV R7,addr			2		24
0xb0	ANL C,/addr			2		24
0xb1	ACALL addr			2		24
0xb2	CPL bitaddr			2		12
0xb3	CPL C				1		12
0xb4	CJNE A,#data,addr	3		24
0xb5	CJNE A,addr,addr	3		24
0xb6	CJNE @R0,#data,addr	3		24
0xb7	CJNE @R1,#data,addr	3		24
0xb8	CJNE R0,#data,addr	3		24
0xb9	CJNE R1,#data,addr	3		24
0xba	CJNE R2,#data,addr	3		24
0xbb	CJNE R3,#data,addr	3		24
0xbc	CJNE R4,#data,addr	3		24
0xbd	CJNE R5,#data,addr 	3		24
0xbe	CJNE R6,#data,addr	3		24
0xbf	CJNE R7,#data,addr	3		24
0xc0	PUSH addr			2		24
0xc1	AJMP addr			2		24
0xc2	CLR bitaddr			2		12
0xc3	CLR C				1		12
0xc4	SWAP A				1		12
0xc5	XCH A,addr			2		12
0xc6	XCH A,@R0			1		12
0xc7	XCH A,@R1			1		12
0xc8	XCH A,R0			1		12
0xc9	XCH A,R1			1		12
0xca	XCH A,R2			1		12
0xcb	XCH A,R3			1		12
0xcc	XCH A,R4			1		12
0xcd	XCH A,R5			1		12
0xce	XCH A,R6			1		12
0xcf	XCH A,R7			1		12
0xd0	POP addr			2		24
0xd1	ACALL addr			2		24
0xd2	SETB addr			2		12
0xd3	SETB C				1		12
0xd4	DA A				1		12
0xd5	DJNZ addr,addr		3		24
0xd6	XCHD A,@R0			1		12
0xd7	XCHD A,@R1			1		12
0xd8	DJNZ R0,addr		2		24
0xd9	DJNZ R1,addr		2		24
0xda	DJNZ R2,addr		2		24
0xdb	DJNZ R3,addr		2		24
0xdc	DJNZ R4,addr		2		24
0xdd	DJNZ R5,addr		2		24
0xde	DJNZ R6,addr		2		24
0xdf	DJNZ R7,addr		2		24
0xe0	MOVX A,@DPTR		1		24
0xe1	AJMP addr			2		24
0xe2	MOVX A,@R0			1		24
0xe3	MOVX A,@R1			1		24
0xe4	CLR A				1		12
0xe5	MOV A,addr			2		12
0xe6	MOV A,@R0			1		12
0xe7	MOV A,@R1			1		12
0xe8	MOV A,R0			1		12
0xe9	MOV A,R1			1		12
0xea	MOV A,R2			1		12
0xeb	MOV A,R3			1		12
0xec	MOV A,R4			1		12
0xed	MOV A,R5			1		12
0xee	MOV A,R6			1		12
0xef	MOV A,R7			1		12
0xf0	MOVX @DPTR,A		1		24
0xf1	ACALL addr			2		24
0xf2	MOVX @R0,A			1		24
0xf3	MOVX @R1,A			1		24
0xf4	CPL A				1		12
0xf5	MOV addr,A			2		12
0xf6	MOV @R0,A			1		12
0xf7	MOV @R1,A			1		12
0xf8	MOV R0,A			1		12
0xf9	MOV R1,A			1		12
0xfa	MOV R2,A			1		12
0xfb	MOV R3,A			1		12
0xfc	MOV R4,A			1		12
0xfd	MOV R5,A			1		12
0xfe	MOV R6,A			1		12
0xff	MOV R7,A			1		12
```

# ROM

rom在此处指主要程序ROM,对于MCS-51而言，主要指一些demo。

见[rom](rom)目录,通常需要单独编译。

