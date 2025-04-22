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

static bool hs_rp_pio_hello_pio_io_process(hs_rp_pio_sm_t *sm,hs_rp_pio_sm_io_opt_t opt,uint32_t *val,void *usr)
{
    hs_rp_pio_hello_pio_t *pio=(hs_rp_pio_hello_pio_t *)sm;
    if(pio->io!=NULL)
    {
        pio->io(pio,(hs_rp_pio_sm_hello_pio_io_opt_t)(int)opt,val,usr);
    }
    return true;
}

void hs_rp_pio_hello_pio_init(hs_rp_pio_hello_pio_t *pio,hs_rp_pio_hello_pio_io_t hook,void *usr)
{
    if(pio==NULL)
    {
        return;
    }
    pio->io=hook;
    hs_rp_pio_init(&pio->pio,hs_rp_pio_hello_pio_io_process,usr);
    pio->pio.memory=hs_rp_pio_sm_rom_hello_pio;
}

void hs_rp_pio_hello_pio_tick(hs_rp_pio_hello_pio_t* pio,size_t cycles)
{
    if(pio!=NULL)
    {
        hs_rp_pio_tick(&pio->pio,cycles);
    }
}

void hs_rp_pio_hello_pio_reset(hs_rp_pio_hello_pio_t *pio)
{
    if(pio!=NULL)
    {
        hs_rp_pio_reset(&pio->pio);
    }
}

bool hs_rp_pio_hello_pio_push(hs_rp_pio_hello_pio_t *pio,uint32_t data)
{
    if(pio!=NULL)
    {
        return hs_rp_pio_push(&pio->pio,data);
    }
    return false;
}
