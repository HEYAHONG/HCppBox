/***************************************************************
 * Name:      rp_pio_sm.h
 * Purpose:   声明rp_pio_sm接口,注意：rp_pio_sm主要用于模拟pio的状态机而非硬件，即主要用于模拟运行pio程序(视具体实现可能和实际硬件有所有差异)。
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HSIMLULATOR_RP_PIO_SM_H__
#define __HSIMLULATOR_RP_PIO_SM_H__
#include "stdbool.h"
#include "stdint.h"
#include "stdlib.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

struct hs_rp_pio_sm;
typedef struct hs_rp_pio_sm hs_rp_pio_sm_t;

/** \brief hs_rp_pio_sm_t结构体大小 通常用于创建hs_rp_pio_sm_t结构体
 *
 * \return size_t hs_rp_pio_sm_t
 *
 */
size_t hs_rp_pio_sm_size(void);

typedef enum
{
    HS_RP_PIO_SM_IO_READ_INSTRUCTION,//指令读取，传入PC的值(通过*val访问)，传出指令（低16位）
    HS_RP_PIO_SM_IO_PUSH_RX_FIFO,//写RX_FIFO，写32位数据
    HS_RP_PIO_SM_IO_READ_MOV_RX_FIFO_0,//读RX_FIFO，写32位数据
    HS_RP_PIO_SM_IO_READ_MOV_RX_FIFO_1,//读RX_FIFO，写32位数据
    HS_RP_PIO_SM_IO_READ_MOV_RX_FIFO_2,//读RX_FIFO，写32位数据
    HS_RP_PIO_SM_IO_READ_MOV_RX_FIFO_3,//读RX_FIFO，写32位数据
    HS_RP_PIO_SM_IO_WRITE_MOV_RX_FIFO_0,//写RX_FIFO，写32位数据
    HS_RP_PIO_SM_IO_WRITE_MOV_RX_FIFO_1,//写RX_FIFO，写32位数据
    HS_RP_PIO_SM_IO_WRITE_MOV_RX_FIFO_2,//写RX_FIFO，写32位数据
    HS_RP_PIO_SM_IO_WRITE_MOV_RX_FIFO_3,//写RX_FIFO，写32位数据
    HS_RP_PIO_SM_IO_PULL_TX_FIFO,//读TX_FIFO，读32位数据
    HS_RP_PIO_SM_IO_READ_PINS,//读PINS,32位数据，每一位表示一个PIN
    HS_RP_PIO_SM_IO_READ_JMP_PIN,//读取用于JMP的PIN,32位数据，每一位表示一个PIN，位0表示JMP指令使用的PIN
    HS_RP_PIO_SM_IO_READ_GPIO,//读GPIO(未经映射)，32位数据，每一位表示一个GPIO。
    HS_RP_PIO_SM_IO_WRITE_PINS,//写PINS,32位数据，每一位表示一个PIN
    HS_RP_PIO_SM_IO_READ_PINDIRS,//读PINDIR,32位数据，每一位表示一个PINDIR
    HS_RP_PIO_SM_IO_WRITE_PINDIRS,//写PINDIR,32位数据，每一位表示一个PINDIR
    HS_RP_PIO_SM_IO_WRITE_SIDESET,//SIDESET可参考HS_RP_PIO_SM_IO_WRITE_PINS,低位有效
    HS_RP_PIO_SM_IO_READ_STATUS,//读STATUS,32位数据，通常是全0或者全1，由MOV指令使用
    HS_RP_PIO_SM_IO_READ_IRQ,//读IRQ,32位数据，位地址由IdxMode(2bit)+Index(3bit)组成
    HS_RP_PIO_SM_IO_WRITE_IRQ,//写IRQ,32位数据，位地址由IdxMode(2bit)+Index(3bit)组成,通常用于清除中断。

} hs_rp_pio_sm_io_opt_t;

/** \brief 状态机IO,状态机与与其他的部分的交互均使用此回调函数。
 *
 * \param sm hs_rp_pio_sm_t* 状态机
 * \param opt hs_rp_pio_sm_io_opt_t IO选项
 * \param val uint32_t* 值，注意：传入传出均使用此指针
 * \param usr void* 用户指针
 * \return bool 是否成功执行
 *
 */
typedef bool (*hs_rp_pio_io_t)(hs_rp_pio_sm_t *sm,hs_rp_pio_sm_io_opt_t opt,uint32_t *val,void *usr);

/** \brief 初始化 hs_rp_pio_sm_t;
 *
 * \param mem void* 大小至少为hs_rp_pio_sm_size()返回的大小的内存的指针
 * \param io hs_rp_pio_io_t 状态机IO
 * \param usr void* 用户指针
 * \return hs_rp_pio_sm_t* 成功返回地址，失败返回NULL
 *
 */
hs_rp_pio_sm_t *hs_rp_pio_sm_init(void *mem,hs_rp_pio_io_t io,void *usr);



typedef enum
{
    HS_RP_PIO_SM_PULL_THRESH,
    HS_RP_PIO_SM_PUSH_THRESH,
    HS_RP_PIO_SM_OUT_SHIFTDIR,
    HS_RP_PIO_SM_IN_SHIFTDIR,
    HS_RP_PIO_SM_AUTOPULL,
    HS_RP_PIO_SM_AUTOPUSH,
    HS_RP_PIO_SM_SIDESET_CNT
} hs_rp_pio_sm_cfg_opt_t;

/** \brief 配置状态机，注意：此函数不是线程安全的，必要时需要加锁。
 *
 * \param sm hs_rp_pio_sm_t* 状态机指针
 * \param opt hs_rp_pio_sm_cfg_opt_t 配置项
 * \param val uint32_t 值
 *
 */
void hs_rp_pio_sm_cfg(hs_rp_pio_sm_t *sm,hs_rp_pio_sm_cfg_opt_t opt,uint32_t val);

typedef enum
{
    HS_RP_PIO_SM_OSR_STATUS,
    HS_RP_PIO_SM_ISR_STATUS,
    HS_RP_PIO_SM_X_STATUS,
    HS_RP_PIO_SM_Y_STATUS,
    HS_RP_PIO_SM_PC_STATUS,
    HS_RP_PIO_SM_DELAY_STATUS,
    HS_RP_PIO_SM_OSR_SHIFT_CNT_STATUS,
    HS_RP_PIO_SM_ISR_SHIFT_CNT_STATUS,
    HS_RP_PIO_SM_PULL_THRESH_STATUS,
    HS_RP_PIO_SM_PUSH_THRESH_STATUS,
    HS_RP_PIO_SM_OUT_SHIFTDIR_STATUS,
    HS_RP_PIO_SM_IN_SHIFTDIR_STATUS,
    HS_RP_PIO_SM_AUTOPULL_STATUS,
    HS_RP_PIO_SM_AUTOPUSH_STATUS,
    HS_RP_PIO_SM_SIDESET_CNT_STATUS,
    HS_RP_PIO_SM_STALL_STATUS
} hs_rp_pio_sm_status_opt_t;

/** \brief 状态机状态,注意：此函数不是线程安全的，必要时需要加锁。
 *
 * \param sm hs_rp_pio_sm_t* 状态机指针
 * \param opt hs_rp_pio_sm_status_opt_t 状态项
 * \param val uint32_t* 值指针
 *
 */
void hs_rp_pio_sm_status(hs_rp_pio_sm_t *sm,hs_rp_pio_sm_status_opt_t opt,uint32_t *val);

enum
{
    HS_RP_PIO_SM_INS_CLASS_JMP=0,           //JMP指令
    HS_RP_PIO_SM_INS_CLASS_WAIT=1,          //WAIT指令
    HS_RP_PIO_SM_INS_CLASS_IN=2,            //IN指令
    HS_RP_PIO_SM_INS_CLASS_OUT=3,           //OUT指令
    HS_RP_PIO_SM_INS_CLASS_PUSH_MOV_PULL=4, //PUSH、MOV、PULL指令
    HS_RP_PIO_SM_INS_CLASS_MOV=5,           //MOV指令
    HS_RP_PIO_SM_INS_CLASS_IRQ=6,           //IRQ指令
    HS_RP_PIO_SM_INS_CLASS_SET=7            //SET指令
};

typedef union
{
    uint16_t Instruction;
    struct
    {
        uint16_t resv:8;
        uint16_t Delay_SideSet:5;
        uint16_t Instruction_Class:3;//HS_RP_PIO_SM_INS_CLASS_*
    };
    struct
    {
        uint16_t Address:5;
        uint16_t Condition:3;
        uint16_t resv:8;//见Delay_SideSet与Ins_Class,下同
    } INS_JMP;
    struct
    {
        uint16_t Index:5;
        uint16_t Source:2;
        uint16_t Pol:1;
        uint16_t resv:8;
    } INS_WAIT;
    struct
    {
        uint16_t Bit_count:5;
        uint16_t Source:3;
        uint16_t resv:8;
    } INS_IN;
    struct
    {
        uint16_t Bit_count:5;
        uint16_t Destination:3;
        uint16_t resv:8;
    } INS_OUT;
    struct
    {
        uint16_t Index:2;//MOV指令
        uint16_t resv1:1;
        uint16_t Idxl:1;//MOV指令
        uint16_t Mov:1;//1=MOV指令,0=Push/Pull指令
        uint16_t Blk:1;//PUSH/PULL指令
        uint16_t ifF_ifE:1;//PUSH/PULL指令
        uint16_t Pull:1;//1=Pull,0=Push/Mov
        uint16_t resv2:8;
    } INS_PUSH_MOV_PULL;
    struct
    {
        uint16_t Source:3;
        uint16_t Op:2;
        uint16_t Destination:3;
        uint16_t resv:8;
    } INS_MOV;
    struct
    {
        uint16_t Index:3;
        uint16_t IdxMode:2;
        uint16_t Wait:1;
        uint16_t Clr:1;
        uint16_t resv1:1;
        uint16_t resv2:8;
    } INS_IRQ;
    struct
    {
        uint16_t Data:5;
        uint16_t Destination:3;
        uint16_t resv:8;
    } INS_SET;
} hs_rp_pio_sm_instruction_t;

/** \brief 状态机执行指令，注意：此函数不是线程安全的，必要时需要加锁。
 *
 * \param sm hs_rp_pio_sm_t* 状态机指针
 * \param instruction hs_rp_pio_sm_instruction_t 指令
 *
 */
void hs_rp_pio_sm_exec(hs_rp_pio_sm_t *sm,hs_rp_pio_sm_instruction_t instruction);


/** \brief 状态机节拍,由于pio对时序要求比较高，推荐在定时器中调用此函数。注意：此函数不是线程安全的，必要时需要加锁。
 *
 * \param sm hs_rp_pio_sm_t* 状态机指针
 * \param cycles size_t 周期数,0等于无效,如需精确的时序，通常使用1作为周期数，并使用定时器调用此函数。
 *
 */
void hs_rp_pio_sm_tick(hs_rp_pio_sm_t *sm,size_t cycles);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __RP_PIO_SM_H__


