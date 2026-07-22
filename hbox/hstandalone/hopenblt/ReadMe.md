# 说明

本目录主要用于辅助实现[openblt](https://www.feaser.com/openblt/)设备端。

主机端的工具见[https://github.com/feaser/openblt](https://github.com/feaser/openblt)。

# 移植

## 主任务

实现文件见[boot.c](3rdparty/boot.c)

openblt的主任务主要采用以下函数：

- `void BootInit(void);`：初始化函数。仅调用一次。
- `void BootTask(void);`：任务函数。需要多次调用且必须在初始化函数之后。

注意：定义`HOPENBLT`为1可由hruntime组件自动调用主任务相关函数。

## 必选移植

必选移植必须根据具体环境实现，否则将无法成功链接。

### cpu

实现CPU相关接口，具体见[cpu.h](3rdparty/cpu.h)。

### timer

实现定时器相关接口，具体见[timer.h](3rdparty/timer.h)。

### nvm

实现存储相关接口，具体见[nvm.h](3rdparty/nvm.h)。

存储主要用于存放用户程序。

**注意:用户需要正确定义`BOOT_NVM_SIZE_KB`。**

## 可选移植

### gateway

实现网关。

定义`ADDON_GATEWAY_MOD_ENABLE`为1时启用。

需要实现`gateway.h`头文件，并实现以下函数:

- `void GatewayInit(void);`:初始化网关
- `void GatewayTask(void);`运行网关。

### 文件

实现文件接口，具体见[file.h](3rdparty/file.h)。

定义`BOOT_FILE_SYS_ENABLE`为1时启用。

**注意:[file.c](3rdparty/file.c)已实现接口,依赖[fatfs](https://github.com/feaser/openblt/tree/master/Target/Source/third_party/fatfs)。**

### 通信接口

实现文件见[com.c](3rdparty/com.c)

#### rs232

 实现串口通信接口，具体见[rs232.h](3rdparty/rs232.h)。

定义`BOOT_COM_RS232_ENABLE`为1时启用。

#### can

 实现can通信接口，具体见[can.h](3rdparty/can.h)。

定义`BOOT_COM_CAN_ENABLE`为1时启用。

#### usb

 实现can通信接口，具体见[usb.h](3rdparty/usb.h)。

定义`BOOT_COM_USB_ENABLE`为1时启用。

#### mb

 实现Modbus RTU通信接口，具体见[mb.h](3rdparty/mb.h)。

定义`BOOT_COM_MBRTU_ENABLE`为1时启用。

  **注意:[mb.c](3rdparty/mb.c)已实现部分接口。**

#### net

 实现网络通信接口，具体见[net.h](3rdparty/net.h)。

定义`BOOT_COM_NET_ENABLE`为1时启用。

 **注意:[net.c](3rdparty/net.c)已实现接口,依赖[uip](https://github.com/feaser/openblt/tree/master/Target/Source/third_party/uip),若为其他环境可能需要重新实现net.c。**

#### 自定义通信

实现自定义通信接口。

定义`BOOT_COM_CUSTOM_ENABLE`为1时启用。

需要实现以下接口（C语言）:

- `void   ComCustomInitHook(void);` 
- `void   ComCustomFreeHook(void);`
- `blt_bool ComCustomReceivePacketHook(blt_int8u *data, blt_int8u *len);`
- `void   ComCustomTransmitPacketHook(blt_int8u *data, blt_int8u len);`

# 目录说明

- [3rdparty](3rdparty):第三方源代码。
- [port](port):移植代码。

