/***************************************************************
 * Name:      mcs_51_core.h
 * Purpose:   声明mcs_51_core接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-11-21
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HS_MCS_51_CORE_H__
#define __HS_MCS_51_CORE_H__
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#include "string.h"
#ifdef __cplusplus
extern "C"
{
#endif

struct hs_mcs_51_core;
typedef struct hs_mcs_51_core hs_mcs_51_core_t;

/** \brief hs_mcs_51_core_t结构体大小 通常用于创建hs_mcs_51_core_t结构体
 *
 * \return size_t hs_mcs_51_core_t结构体大小
 *
 */
size_t hs_mcs_51_core_size(void);

#ifndef HS_MCS_51_INSTRUCTION_MAX_LENGTH
#define HS_MCS_51_INSTRUCTION_MAX_LENGTH 3  /**< 指令最长字节数,默认每次读取此长度的指令 */
#endif // HS_MCS_51_INSTRUCTION_MAX_SIZE

typedef enum
{
    HS_MCS_51_IO_RESET,                 //IO复位
    HS_MCS_51_IO_READ_ROM,              //读取ROM
    HS_MCS_51_IO_READ_RAM_SFR,          //读取内部低128字节RAM与SFR
    HS_MCS_51_IO_WRITE_RAM_SFR,         //写入内部低128字节RAM与SFR
    HS_MCS_51_IO_READ_HIGH_RAM,         //读取内部RAM(包括低128B与高128B)
    HS_MCS_51_IO_WRITE_HIGH_RAM,        //写入内部RAM(包括低128B与高128B)
    HS_MCS_51_IO_READ_EXTERNAL_RAM,     //读取外部RAM(最高64KB)
    HS_MCS_51_IO_WRITE_EXTERNAL_RAM,    //写入外部RAM(最高64KB)
} hs_mcs_51_io_opt_t;

/** \brief MCS-51 IO操作
 *
 * \param core hs_mcs_51_core_t*    MCS-51内核指针
 * \param address uint16_t          地址
 * \param opt hs_mcs_51_io_opt_t    IO操作选项
 * \param data uint8_t*             数据长度
 * \param length uint16_t           数据长度
 * \param usr void*                 用户指针
 * \return bool 是否操作成功
 *
 */
typedef bool (*hs_mcs_51_io_t)(hs_mcs_51_core_t *core,hs_mcs_51_io_opt_t opt,uint16_t address,uint8_t *data,uint16_t length,void *usr);


/** \brief  初始化 hs_mcs_51_io_t
 *
 * \param mem void* 大小至少为hs_mcs_51_core_size()返回的大小的内存的指针
 * \param io hs_mcs_51_io_t MCS-51 内核IO
 * \param usr void* 用户指针 用户指针
 * \return hs_mcs_51_core_t* 成功返回地址，失败返回NULL
 *
 */
hs_mcs_51_core_t *hs_mcs_51_core_init(void *mem,hs_mcs_51_io_t io,void *usr);

/** \brief MCS-51 内核节拍，注意：此函数不是线程安全的，必要时需要加锁。
 *
 * \param core hs_mcs_51_core_t* MCS-51 内核指针
 * \param cycles size_t 周期数
 *
 */
void hs_mcs_51_core_tick(hs_mcs_51_core_t * core,size_t cycles);

/** \brief MCS-51内核复位，注意：此函数不是线程安全的，必要时需要加锁。
 *
 * \param core hs_mcs_51_core_t* MCS-51 内核指针
 *
 */
void hs_mcs_51_core_reset(hs_mcs_51_core_t * core);

//中断表（中断入口地址=(中断号 *8)+3）
typedef enum
{
    HS_MCS_51_INTERRUPT_0=0,
    HS_MCS_51_INTERRUPT_1=1,
    HS_MCS_51_INTERRUPT_2=2,
    HS_MCS_51_INTERRUPT_3=3,
    HS_MCS_51_INTERRUPT_4=4,
    HS_MCS_51_INTERRUPT_5=5,
    HS_MCS_51_INTERRUPT_6=6,
    HS_MCS_51_INTERRUPT_7=7,
    HS_MCS_51_INTERRUPT_8=8,
    HS_MCS_51_INTERRUPT_9=9,
    HS_MCS_51_INTERRUPT_10=10,
    HS_MCS_51_INTERRUPT_11=11,
    HS_MCS_51_INTERRUPT_12=12,
    HS_MCS_51_INTERRUPT_13=13,
    HS_MCS_51_INTERRUPT_14=14,
    HS_MCS_51_INTERRUPT_15=15,
    HS_MCS_51_INTERRUPT_16=16,
    HS_MCS_51_INTERRUPT_17=17,
    HS_MCS_51_INTERRUPT_18=18,
    HS_MCS_51_INTERRUPT_19=19,
    HS_MCS_51_INTERRUPT_20=20,
    HS_MCS_51_INTERRUPT_21=21,
    HS_MCS_51_INTERRUPT_22=22,
    HS_MCS_51_INTERRUPT_23=23,
    HS_MCS_51_INTERRUPT_24=24,
    HS_MCS_51_INTERRUPT_25=25,
    HS_MCS_51_INTERRUPT_26=26,
    HS_MCS_51_INTERRUPT_27=27,
    HS_MCS_51_INTERRUPT_28=28,
    HS_MCS_51_INTERRUPT_29=29,
    HS_MCS_51_INTERRUPT_30=30,
    HS_MCS_51_INTERRUPT_31=31,
    HS_MCS_51_INTERRUPT_8051_IE0=HS_MCS_51_INTERRUPT_0, //外部中断0
    HS_MCS_51_INTERRUPT_8051_TF0=HS_MCS_51_INTERRUPT_1, //定时器中断0
    HS_MCS_51_INTERRUPT_8051_IE1=HS_MCS_51_INTERRUPT_2, //外部中断1
    HS_MCS_51_INTERRUPT_8051_TF1=HS_MCS_51_INTERRUPT_3, //定时器中断1
    HS_MCS_51_INTERRUPT_8051_SI0=HS_MCS_51_INTERRUPT_4, //串口中断0
} hs_mcs_51_interrupt_number_t;

/** \brief MCS-51内核设置中断到中断扫描表（将在下一个周期执行中断），执行后自动清除，一般由外设调用，注意：此函数不是线程安全的，必要时需要加锁。
 *
 * \param core hs_mcs_51_core_t* MCS-51 内核指针
 * \param number hs_mcs_51_interrupt_number_t 中断号
 * \param is_high_priority bool 是否是高优先级中断
 *
 */
void hs_mcs_51_core_interrupt_set(hs_mcs_51_core_t * core,hs_mcs_51_interrupt_number_t number,bool is_high_priority);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // __HS_MCS_51_CORE_H__
