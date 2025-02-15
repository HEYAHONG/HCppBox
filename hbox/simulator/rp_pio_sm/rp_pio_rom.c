/***************************************************************
 * Name:      rp_pio_rom.c
 * Purpose:   实现rp_pio_rom接口。
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-02-16
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "rp_pio_rom.h"


/** \brief 程序，主要将TX FIFO中的数据（无数据则stall）中的最低位通过PINS发送出去。。
 *
 * loop:
 *      pull
 *      out pins, 1
 *      jmp loop
 *
 */
const hs_rp_pio_sm_memory_t hs_rp_pio_sm_rom_hello_pio=
{
    {
        0x80a0, //  0: pull   block
        0x6001, //  1: out    pins, 1
        0x0000  //  2: jmp    0
    },
    {
        0,
        0,
        1,
        1,
        0,
        0,
        0,
        0
    }
};
