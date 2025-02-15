# 说明

本目录主要用于实现树莓派PIO状态机。

优点：

- PIO专为IO接口优化。可通过编程可实现多种IO标准，其介于硬件设计与软件设计之间，编程更贴合硬件。
- 可移植性强。可在MCU上运行，可辅助实现一个可编程的硬件接口。可使用针对硬件设计的PIO程序（具体效果跟软件实现有关）。
- 无需具体硬件。几乎不受硬件限制，只要内存足够，就可使用。

缺点：

- 效果跟软件实现（如io操作的实现情况、节拍的调用时序）有关。甚至需要根据具体程序进行调整。
- 速度较慢。由于是软件模拟，实现高速的硬件接口有一定困难。速度较高是硬件PIO的优点。


# 数据手册

- [RP2040](https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf)
- [RP2350](https://datasheets.raspberrypi.com/rp2350/rp2350-datasheet.pdf)

可查看数据手册的PIO章节。

# PIO程序

## 编译

- [pico-sdk](https://github.com/raspberrypi/pico-sdk.git):此为树莓派官方Pico SDK，其tools/pioasm目录为pioasm源代码目录。
- [pioasm-wasm](https://github.com/wokwi/pioasm-wasm.git)：此为第三方工具，可使用[https://wokwi.com/tools/pioasm](https://wokwi.com/tools/pioasm)在线访问。

## 示例

官方示例链接：[https://github.com/raspberrypi/pico-examples/pio](https://github.com/raspberrypi/pico-examples/tree/master/pio)

### hello_pio

```pio
;
; Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
;
; SPDX-License-Identifier: BSD-3-Clause
;

.program hello

; Repeatedly get one word of data from the TX FIFO, stalling when the FIFO is
; empty. Write the least significant bit to the OUT pin group.

loop:
    pull
    out pins, 1
    jmp loop
```

