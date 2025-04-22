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

struct hs_rp_pio_hello_pio;
typedef struct hs_rp_pio_hello_pio hs_rp_pio_hello_pio_t;

typedef enum
{
    HS_RP_PIO_SM_HELLO_PIO_IO_RESET                 =       HS_RP_PIO_SM_IO_RESET,
    HS_RP_PIO_SM_HELLO_PIO_IO_WRITE_PINS            =       HS_RP_PIO_SM_IO_WRITE_PINS,
} hs_rp_pio_sm_hello_pio_io_opt_t;

typedef bool (*hs_rp_pio_hello_pio_io_t)(hs_rp_pio_hello_pio_t *sm,hs_rp_pio_sm_hello_pio_io_opt_t opt,uint32_t *val,void *usr);

struct  hs_rp_pio_hello_pio
{
    hs_rp_pio_t pio;
    hs_rp_pio_hello_pio_io_t io;
};

/** \brief hello_pio PIO初始化
 *
 * \param pio hs_rp_pio_hello_pio_t*      hello_pio pio指针
 * \param hook hs_rp_pio_hello_pio_io_t   Hook函数
 * \param usr void* 用户参数
 *
 */
void hs_rp_pio_hello_pio_init(hs_rp_pio_hello_pio_t *pio,hs_rp_pio_hello_pio_io_t hook,void *usr);

/** \brief hello_pio PIO节拍,由于pio对时序要求比较高，推荐在定时器中调用此函数。注意：此函数不是线程安全的，必要时需要加锁。
 *
 * \param pio hs_rp_pio_hello_pio_t*      hello_pio pio指针
 * \param cycles size_t 周期数,0等于无效,如需精确的时序，通常使用1作为周期数，并使用定时器调用此函数。
 *
 */
void hs_rp_pio_hello_pio_tick(hs_rp_pio_hello_pio_t* pio,size_t cycles);


/** \brief hello_pio PIO复位(恢复默认状态与配置)，注意：此函数不是线程安全的，必要时需要加锁。
 *
 * \param pio hs_rp_pio_hello_pio_t*      hello_pio pio指针
 *
 */
void hs_rp_pio_hello_pio_reset(hs_rp_pio_hello_pio_t *pio);

/** \brief hello_pio PIO PUSH数据，注意：此函数不是线程安全的，必要时需要加锁。
 *
 * \param pio hs_rp_pio_hello_pio_t*      hello_pio pio指针
 * \param data uint32_t 数据
 * \return bool 是否成功
 *
 */
bool hs_rp_pio_hello_pio_push(hs_rp_pio_hello_pio_t *pio,uint32_t data);



#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __HSIMLULATOR_RP_PIO_ROM_H__


