# 说明

本组件用于辅助实现[modbus](https://modbus.org),标准文档为[Modbus_Application_Protocol_V1_1b3.pdf](https://modbus.org/docs/Modbus_Application_Protocol_V1_1b3.pdf)。

现可实现以下功能：

- 精简版modbus rtu从机:实现简单的modbus从机功能，仅实现线圈、离散输入、输入寄存器、保持寄存器相关功能。
- modbus tcp网关服务器：实现modbus tcp到modbus rtu的转换。
- modbus tcp客户端：实现对modbus tcp服务器的访问。

注意：本组件只包含对相应模式（如 modbus tcp、modbus rtu）下的数据包的处理，不包含具体io操作(如串口读写、tcp读写)。

[modbus](https://modbus.org)支持以下数据类型：

| 名称       | 说明                  | 备注                                   |
| ---------- | --------------------- | -------------------------------------- |
| 线圈       | 可读写的位（1bit）    |                                        |
| 离散输入   | 只读的位（1bit）      |                                        |
| 保持寄存器 | 可读写的数据（16bit） | 超过16位的数据通常需要多个寄存器组合。 |
| 输入寄存器 | 只读的数据（16bit）   | 超过16位的数据通常需要多个寄存器组合。 |

实际应用中，需要将这些数据类型映射到实际的功能上。

