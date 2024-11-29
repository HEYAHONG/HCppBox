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
    HS_MCS_51_IO_BREAKPOINT             //由MCS-51的保留指令（0xA5）触发，可用于自定义的指令(默认这是一条单周期单字节指令)，通过地址传入PC的值（下一条指令地址），通过数据传出相对跳转的地址(有符号数)。
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


//SFR地址
typedef enum
{
    HS_MCS_51_SFR_ACC=0xe0,         /* Accumulator */
    HS_MCS_51_SFR_B=0xf0,           /* B register (secondary accumulator) */
    HS_MCS_51_SFR_PSW=0xd0,         /* Program Status Word */
    HS_MCS_51_SFR_SP=0x81,          /* Stack Pointer */
    HS_MCS_51_SFR_DPL=0x82,         /* Data Pointer Low byte */
    HS_MCS_51_SFR_DPH=0x83,         /* Data Pointer High byte */
    HS_MCS_51_SFR_DPL1=0x84,        /* 2nd Data Pointer Low byte */
    HS_MCS_51_SFR_DPH1=0x85,        /* 2nd Data Pointer High byte */
    HS_MCS_51_SFR_DPS=0x86,         /* DPS 1H=DPTR is DPL1/DPH1, 2H=AUTO DPTR INCR */
    HS_MCS_51_SFR_P0=0x80,          /* Port #0 */
    HS_MCS_51_SFR_P1=0x90,          /* Port #1 */
    HS_MCS_51_SFR_P2=0xa0,          /* Port #2 */
    HS_MCS_51_SFR_P3=0xb0,          /* Port #3 */
    HS_MCS_51_SFR_IP=0xb8,          /* Interrupt Priority */
    HS_MCS_51_SFR_IE=0xa8,          /* Interrupt Enable */
    HS_MCS_51_SFR_TMOD=	0x89,       /* Timer MODe */
    HS_MCS_51_SFR_TCON=0x88,        /* Timer CONtrol */
    HS_MCS_51_SFR_T2CON=0xc8,       /* Timer #2 CONtrol */
    HS_MCS_51_SFR_TH0=0x8c,         /* Timer #0 High byte */
    HS_MCS_51_SFR_TL0=0x8a,         /* Timer #0 Low byte */
    HS_MCS_51_SFR_TH1=0x8d,         /* Timer #1 High byte */
    HS_MCS_51_SFR_TL1=0x8b,         /* Timer #1 Low byte */
    HS_MCS_51_SFR_SCON=0x98,        /* Serial line CONtrol */
    HS_MCS_51_SFR_TH2=0xcd,         /* Timer #2 High byte */
    HS_MCS_51_SFR_TL2=0xcc,         /* Timer #2 Low byte */
    HS_MCS_51_SFR_RCAP2H=0xcb,      /* Capture Register of Timer #2 High byte */
    HS_MCS_51_SFR_RCAP2L=0xca,      /* Capture Register of Timer #2 Low byte */
    HS_MCS_51_SFR_SBUF=0x99,        /* Serial line BUFfer */
    HS_MCS_51_SFR_PCON=0x87,        /* Power CONtrol */
    HS_MCS_51_SFR_AUXR=0x8e,        /* Auxiliary Register */
    HS_MCS_51_SFR_AUXR1=0xa2,       /* Secondary Aux Register */
    HS_MCS_51_SFR_P4=0x80,
    HS_MCS_51_SFR_DPXL=0x84,        /* */
    HS_MCS_51_SFR_CKCON=0x8E,       /* Clock Control */
    HS_MCS_51_SFR_EXIF=0x91,        /* */
    HS_MCS_51_SFR_P4CNT=0x92,       /* */
    HS_MCS_51_SFR_DPX =0x93,        /* Data Pointer HHigh byte */
    HS_MCS_51_SFR_DPX1 =0x95,       /* Data Pointer HHigh byte */
    HS_MCS_51_SFR_R51_ESP=0x9b,     /* Extended Stack Pointer */
    HS_MCS_51_SFR_AP=0x9c,          /* Address Page */
    HS_MCS_51_SFR_ACON =0x9d,       /* */
    HS_MCS_51_SFR_P5 =0xa1,
    HS_MCS_51_SFR_P5CNT =0xa2,      /* */
    HS_MCS_51_SFR_C0C =0xa3,        /* */
    HS_MCS_51_SFR_WDTRST=0xa6,      /* */
    HS_MCS_51_SFR_IE0=0xa8,         /* */
    HS_MCS_51_SFR_SADDR=0xa9,       /* */
    HS_MCS_51_SFR_IPH0=0xb7,        /* */
    HS_MCS_51_SFR_IPH=0xb7,
    HS_MCS_51_SFR_IPL0=0xb8,        /* */
    HS_MCS_51_SFR_SADEN=0xb9,       /* */
    HS_MCS_51_SFR_SPH=0xbd,         /* */
    HS_MCS_51_SFR_PMR=0xc4,         /* */
    HS_MCS_51_SFR_MCON=0xc6,        /* */
    HS_MCS_51_SFR_TA=0xc7,          /* */
    HS_MCS_51_SFR_T2MOD	=0xc9,      /* */
    HS_MCS_51_SFR_COR=0xce,         /* */
    HS_MCS_51_SFR_PSW1=0xd1,        /* */
    HS_MCS_51_SFR_MCNT0=0xd1,
    HS_MCS_51_SFR_MCNT1=0xd2,
    HS_MCS_51_SFR_MA=0xd3,          /* MA register from math accelerator */
    HS_MCS_51_SFR_MB=0xd4,          /* MB register from math accelerator */
    HS_MCS_51_SFR_MC=0xd5,          /* MC register from math accelerator */
    HS_MCS_51_SFR_CCON=0xd8,        /* */
    HS_MCS_51_SFR_CMOD=0xd9,        /* */
    HS_MCS_51_SFR_WDCON=0xd8,       /* */
    HS_MCS_51_SFR_CCAPM0=0xda,      /* */
    HS_MCS_51_SFR_CCAPM1=0xdb,      /* */
    HS_MCS_51_SFR_CCAPM2=0xdc,      /* */
    HS_MCS_51_SFR_CCAPM3=0xdd,      /* */
    HS_MCS_51_SFR_CCAPM4=0xde,      /* */
    HS_MCS_51_SFR_C1C=0xe3,         /* */
    HS_MCS_51_SFR_CL=0xe9,          /* */
    HS_MCS_51_SFR_CCAP0L=0xea,      /* */
    HS_MCS_51_SFR_CCAP1L=0xeb,      /* */
    HS_MCS_51_SFR_CCAP2L=0xec,      /* */
    HS_MCS_51_SFR_CCAP3L=0xed,      /* */
    HS_MCS_51_SFR_CCAP4L=0xee,      /* */
    HS_MCS_51_SFR_CH=0xf9,          /* */
    HS_MCS_51_SFR_CCAP0H=0xfa,      /* */
    HS_MCS_51_SFR_CCAP1H=0xfb,      /* */
    HS_MCS_51_SFR_CCAP2H=0xfc,      /* */
    HS_MCS_51_SFR_CCAP3H=0xfd,      /* */
    HS_MCS_51_SFR_CCAP4H=0xfe       /* */
} hs_mcs_51_sfr_addr_t;


/** \brief MCS-51内核读SFR，注意：此函数不是线程安全的，必要时需要加锁。
 *
 * \param core hs_mcs_51_core_t* MCS-51 内核指针
 * \param addr hs_mcs_51_sfr_addr_t SFR地址
 * \param data uint8_t* 数据指针
 * \return bool 是否成功
 *
 */
bool hs_mcs_51_sfr_read(hs_mcs_51_core_t * core,hs_mcs_51_sfr_addr_t addr,uint8_t *data);

/** \brief MCS-51内核写SFR，注意：此函数不是线程安全的，必要时需要加锁。
 *
 * \param core hs_mcs_51_core_t* MCS-51 内核指针
 * \param addr hs_mcs_51_sfr_addr_t SFR地址
 * \param data uint8_t 数据
 * \return bool 是否成功
 *
 */
bool hs_mcs_51_sfr_write(hs_mcs_51_core_t * core,hs_mcs_51_sfr_addr_t addr,uint8_t data);


/** \brief MCS-51内核获取PC值，注意：此函数不是线程安全的，必要时需要加锁。
 *
 * \param core hs_mcs_51_core_t* MCS-51 内核指针
 * \return uint16_t PC值
 *
 */
uint16_t hs_mcs_51_pc_get(hs_mcs_51_core_t * core);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // __HS_MCS_51_CORE_H__
