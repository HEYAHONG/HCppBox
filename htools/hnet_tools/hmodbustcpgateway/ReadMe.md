# 说明

 独立的`Modbus` TCP网关程序。

可用于后台运行,如在嵌入式Linux后台运行。

# 环境变量

本工程除了支持通过命名参数传递设置之外，也支持通过环境变量传递参数（可方便后台守护进程启动脚本编写）。

| 环境变量名称                   | 说明                                                  | 备注                         |
| ------------------------------ | ----------------------------------------------------- | ---------------------------- |
| `MBTCP_ADDR`                   | 绑定的网卡的地址                                      | 默认为`0.0.0.0`              |
| `MBTCP_PORT`                   | 绑定的服务端口                                        | 默认为502                    |
| `MBTCP_SERIALPORT`             | 转发的串口名称                                        | 默认为成功枚举的最后一个串口 |
| `MBTCP_SERIALPORT_BAUDRATE`    | 串口波特率                                            | 默认为115200                 |
| `MBTCP_SERIALPORT_DATABITS`    | 串口数据位                                            | 默认为8                      |
| `MBTCP_SERIALPORT_PARITY`      | 串口校验（N=None、E=Even、O=Odd、S=Space、M=Mark）    | 默认为N                      |
| `MBTCP_SERIALPORT_STOPBITS`    | 串口停止位                                            | 默认为1                      |
| `MBTCP_SERIALPORT_FLOWCONTROL` | 串口流控(N=None、R=`RTSCTS`、D=`DTRDSR`、X=`XONXOFF`) | 默认为N                      |
| `MBTCP_SERIALPORT_BLOCK`       | 每次串口请求的阻塞事件，单位为毫秒                    | 默认为100ms                  |

# 依赖库

- [libevent](https://libevent.org/)
- [libserialport](https://sigrok.org/wiki/libserialport)

