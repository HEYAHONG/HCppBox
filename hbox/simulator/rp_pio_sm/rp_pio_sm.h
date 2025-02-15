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

#ifndef HS_RP_PIO_SM_SIZE

/** \brief hs_rp_pio_sm_t结构体大小,一般用于静态分配，注意:可能大于hs_rp_pio_sm_size()返回的值。
 *
 *
 */
#define HS_RP_PIO_SM_SIZE() ((((sizeof(uintptr_t)*2+sizeof(uint32_t)*6)/sizeof(uint32_t))+(((sizeof(uintptr_t)*2+sizeof(uint32_t)*6)%sizeof(uint32_t))?1:0))*sizeof(uint32_t))
#endif // HS_RP_PIO_SM_SIZE

typedef enum
{
    HS_RP_PIO_SM_IO_RESET,//IO复位，无参数
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

//注意：此布局为小端模式的布局，大端模式在内部会进行修复
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

/** \brief 状态机复位(恢复默认状态与配置)，注意：此函数不是线程安全的，必要时需要加锁。
 *
 * \param sm hs_rp_pio_sm_t* 状态机指针
 *
 */
void hs_rp_pio_sm_reset(hs_rp_pio_sm_t *sm);


typedef struct
{
    uint32_t fifo[4];//PIO状态机中FIFO深度为4
    struct
    {
        uint32_t read_ptr:2;
        uint32_t write_ptr:2;
        uint32_t is_empty:1;//当读写指针相同时，使用此标志判断是否为空
        uint32_t is_full:1;//当读写指针相同时，使用此标志判断是否为满
        /*
        *关闭FIFO，直接对fifo数组随机访问,
        *一般用于HS_RP_PIO_SM_INS_CLASS_PUSH_MOV_PULL中的MOV指令.
        *io操作对应HS_RP_PIO_SM_IO_READ_MOV_RX_FIFO_0~HS_RP_PIO_SM_IO_READ_MOV_RX_FIFO_3、HS_RP_PIO_SM_IO_WRITE_MOV_RX_FIFO_0~HS_RP_PIO_SM_IO_WRITE_MOV_RX_FIFO_3
        *此时对FIFO的PUSH与PULL均无效
        */
        uint32_t disable_fifo:1;
    };
} hs_rp_pio_sm_fifo_t;

/** \brief 状态机FIFO初始化,注意：此函数不是线程安全的，必要时需要加锁。
 *
 * \param sm_fifo hs_rp_pio_sm_fifo_t* 状态机FIFO指针
 *
 */
void hs_rp_pio_sm_fifo_init(hs_rp_pio_sm_fifo_t *sm_fifo);

/** \brief FIFO是否为空
 *
 * \param sm_fifo hs_rp_pio_sm_fifo_t* 状态机FIFO指针
 * \return bool FIFO是否为空,sm_fifo为NULL时返回false
 *
 */
bool hs_rp_pio_sm_fifo_is_empty(hs_rp_pio_sm_fifo_t *sm_fifo);

/** \brief FIFO是否为满
 *
 * \param sm_fifo hs_rp_pio_sm_fifo_t* 状态机FIFO指针
 * \return bool FIFO是否为满,sm_fifo为NULL时返回true
 *
 */
bool hs_rp_pio_sm_fifo_is_full(hs_rp_pio_sm_fifo_t *sm_fifo);

/** \brief 写FIFO，注意：此函数不是线程安全的，必要时需要加锁。
 *
 * \param sm_fifo hs_rp_pio_sm_fifo_t* 状态机FIFO指针
 * \param data uint32_t 待写入的数据
 * \return bool 是否写入成功
 *
 */
bool hs_rp_pio_sm_fifo_push(hs_rp_pio_sm_fifo_t *sm_fifo,uint32_t data);

/** \brief 读FIFO，注意：此函数不是线程安全的，必要时需要加锁。
 *
 * \param sm_fifo hs_rp_pio_sm_fifo_t* 状态机FIFO指针
 * \param data uint32_t* 待读取的数据指针
 * \return bool 是否读取成功
 *
 */
bool hs_rp_pio_sm_fifo_pull(hs_rp_pio_sm_fifo_t *sm_fifo,uint32_t* data);

typedef struct
{
    uint16_t code[32];//32条程序代码
    struct //程序相关的配置
    {
        uint32_t pull_thresh:5;//PULL指令阈值,0代表阈值为32.
        uint32_t push_thresh:5;//PUSH指令阈值,0代表阈值为32.
        uint32_t out_shiftdir:1;//1=右移，0=左移，默认应当为1
        uint32_t in_shiftdir:1;//1=右移，0=左移，默认应当为1
        uint32_t autopull:1;//1=自动pull
        uint32_t autopush:1;//1=自动push
        uint32_t disable_rxfifo:1;//关闭RX FIFO，使用随机访问
        uint32_t sideset_cnt:3;//,sideset特性，0~5
    };
} hs_rp_pio_sm_memory_t;

/** \brief 程序内存初始化。
 *
 * \param sm_mem hs_rp_pio_sm_memory_t* 状态机Memory指针
 *
 */
void hs_rp_pio_sm_memory_init(hs_rp_pio_sm_memory_t *sm_mem);

/** \brief 加载内存配置，通常在io回调的复位函数中调用
 *
 * \param sm hs_rp_pio_sm_t* 状态机指针
 * \param sm_rxfifo hs_rp_pio_sm_fifo_t* 状态机Rx FIFO指针,可为NULL
 * \param sm_mem hs_rp_pio_sm_memory_t* 状态机Memory指针
 *
 */
void hs_rp_pio_sm_load_memory_cfg(hs_rp_pio_sm_t *sm,hs_rp_pio_sm_fifo_t *sm_rxfifo,const hs_rp_pio_sm_memory_t *sm_mem);


typedef struct
{
    uint32_t sm[HS_RP_PIO_SM_SIZE()/sizeof(uint32_t)];
    hs_rp_pio_sm_memory_t memory;    //程序内存
    hs_rp_pio_sm_fifo_t txfifo;      //用户可使用hs_rp_pio_sm_fifo_push向状态机写入数据。
    hs_rp_pio_sm_fifo_t rxfifo;      //用户可使用hs_rp_pio_sm_fifo_pull读取状态机的数据。对于某些程序而言,可直接访问fifo内的字。
    uint32_t pins;
    uint32_t jmp_pin;               //位0表示JMP指令使用的PIN
    uint32_t gpio;                  //GPIO与PINS不同，通常指未经映射的IO
    uint32_t pindirs;
    uint32_t sideset;               //sideset特性
    uint32_t status;                //状态数据，一般是全0或者全1
    uint32_t irq;                   //IRQ，一般用于状态机之间的同步
    hs_rp_pio_io_t hook;             //Hook函数将在io操作结束时调用,sm指针可转换为pio指针,一般用于同步一些状态。
} hs_rp_pio_t;/**< PIO,注意:此结构体用于内存比较充足的场景，内存不足时需要用户选择性实现状态机部分功能*/

/** \brief PIO初始化
 *
 * \param pio hs_rp_pio_t*pio指针
 * \param hook hs_rp_pio_io_t Hook函数
 * \param usr void* 用户参数
 *
 */
void hs_rp_pio_init(hs_rp_pio_t *pio,hs_rp_pio_io_t hook,void *usr);

/** \brief PIO节拍,由于pio对时序要求比较高，推荐在定时器中调用此函数。注意：此函数不是线程安全的，必要时需要加锁。
 *
 * \param pio hs_rp_pio_t*pio指针
 * \param cycles size_t 周期数,0等于无效,如需精确的时序，通常使用1作为周期数，并使用定时器调用此函数。
 *
 */
void hs_rp_pio_tick(hs_rp_pio_t *pio,size_t cycles);

/** \brief PIO复位(恢复默认状态与配置)，注意：此函数不是线程安全的，必要时需要加锁。
 *
 * \param pio hs_rp_pio_t*pio指针
 *
 */
void hs_rp_pio_reset(hs_rp_pio_t *pio);

/** \brief PIO PUSH数据，注意：此函数不是线程安全的，必要时需要加锁。
 *
 * \param pio hs_rp_pio_t*pio指针
 * \param data uint32_t 数据
 * \return bool 是否成功
 *
 */
bool hs_rp_pio_push(hs_rp_pio_t *pio,uint32_t data);

/** \brief PIO PULL数据，注意：此函数不是线程安全的，必要时需要加锁。
 *
 * \param pio hs_rp_pio_t*pio指针
 * \param data uint32_t* 待读取数据的指针
 * \return bool 是否成功
 *
 */
bool hs_rp_pio_pull(hs_rp_pio_t *pio,uint32_t* data);

/** \brief PIO 直接写入Rx FIFO(用于部分程序)，注意：此函数不是线程安全的，必要时需要加锁。
 *
 * \param pio hs_rp_pio_t* pio指针
 * \param addr uint32_t ，内部地址，范围0～3
 * \param data uint32_t 数据
 * \return bool 是否成功
 *
 */
bool  hs_rp_pio_rxfifo_set(hs_rp_pio_t *pio,uint32_t addr,uint32_t data);



/** \brief PIO 直接读取Rx FIFO(用于部分程序)，注意：此函数不是线程安全的，必要时需要加锁。
 *
 * \param pio hs_rp_pio_t* pio指针
 * \param addr uint32_t ，内部地址，范围0～3
 * \param data uint32_t* 待读取数据的指针
 * \return bool 是否成功
 *
 */
bool  hs_rp_pio_rxfifo_get(hs_rp_pio_t *pio,uint32_t addr,uint32_t* data);



#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __HSIMLULATOR_RP_PIO_SM_H__


