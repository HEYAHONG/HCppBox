/***************************************************************
 * Name:      rp_pio_rom.h
 * Purpose:   声明rp_pio_rom接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-02-16
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HSIMLULATOR_RP_PIO_ROM_H__
#define __HSIMLULATOR_RP_PIO_ROM_H__
#include "stdbool.h"
#include "stdint.h"
#include "stdlib.h"
#include "rp_pio_sm.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus


/** \brief 程序，主要将TX FIFO中的数据（无数据则stall）中的最低位通过PINS发送出去。
 *
 * loop:
 *      pull
 *      out pins, 1
 *      jmp loop
 *
 */
extern const hs_rp_pio_sm_memory_t hs_rp_pio_sm_rom_hello_pio;

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __HSIMLULATOR_RP_PIO_ROM_H__


