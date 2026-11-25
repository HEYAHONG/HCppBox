/***************************************************************
 * Name:      moxie_core.h
 * Purpose:   声明moxie内核接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-16
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HS_MOXIE_CORE_H__
#define __HS_MOXIE_CORE_H__
#include "stdlib.h"
#include "stdint.h"
#include "stdbool.h"
#include "string.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

struct hs_moxie_core;
typedef struct hs_moxie_core hs_moxie_core_t;

/** \brief hs_moxie_core_t结构体大小 通常用于创建hs_moxie_core_t结构体
 *
 * \return size_t hs_moxie_core_t结构体大小
 *
 */
size_t hs_moxie_core_size(void);

#ifndef HS_MOXIE_CORE_SIZE
/** \brief hs_moxie_core_t结构体大小,一般用于静态分配，注意:可能大于hs_moxie_core_size()返回的值。
 *
 *
 */
#define HS_MOXIE_CORE_SIZE()  (sizeof(uintptr_t)*2+sizeof(uint32_t)*12+sizeof(uint64_t)*3)
#endif // HS_MOXIE_CORE_SIZE

/*
 * 注意:本文件为大端序moxie内核。除特殊说明外，IO操作中的多字节数据均为大端序。
 * 即地址较低处存放数据的高字节。
 */
typedef enum
{
    HS_MOXIE_CORE_IO_RESET=0,                                           /**< IO初始化，用户应在此选项中将通用寄存器、特殊寄存器清零并设置初始状态 */
    HS_MOXIE_CORE_IO_ENTRY_PC_READ,                                     /**< 入口PC值读取，必须实现，否则复位状态将异常 */
    HS_MOXIE_CORE_IO_INITIAL_SP_READ,                                   /**< 初始SP值读取，若读取的值为0表示未实现此选项，用户应自行在软件的启动代码中设置SP */
    HS_MOXIE_CORE_IO_MEMORY_READ,                                       /**< 内存读取，内存包括数据与程序，数据为大端序 */
    HS_MOXIE_CORE_IO_MEMORY_WRITE,                                      /**< 内存写入，内存包括数据与程序，数据为大端序 */
    HS_MOXIE_CORE_IO_SREG_READ,                                         /**< 特殊寄存器读取，地址为特殊寄存器编号(0~255)，数据为大端序 */
    HS_MOXIE_CORE_IO_SREG_WRITE,                                        /**< 特殊寄存器写入，地址为特殊寄存器编号(0~255)，数据为大端序 */
    HS_MOXIE_CORE_IO_REGISTER_READ,                                     /**< 通用寄存器读取，地址为寄存器编号(0~15)，数据为大端序 */
    HS_MOXIE_CORE_IO_REGISTER_WRITE,                                    /**< 通用寄存器写入，地址为寄存器编号(0~15)，数据为大端序 */
    HS_MOXIE_CORE_IO_PC_REGISTER_READ,                                  /**< PC寄存器读取，数据为大端序 */
    HS_MOXIE_CORE_IO_PC_REGISTER_WRITE,                                 /**< PC寄存器写入，数据为大端序 */
    HS_MOXIE_CORE_IO_INSTRUCTION_ENTER,                                 /**< 指令进入,开始执行指令时调用。地址为当前PC值,数据为已经读取的指令(类型为uint32_t，低16位为首个16位指令字，高16位为后续16位指令字(若有))。 */
    HS_MOXIE_CORE_IO_INSTRUCTION_EXIT,                                  /**< 指令退出,结束执行指令时调用。地址为当前PC值,数据为已经读取的指令(类型为uint32_t，低16位为首个16位指令字，高16位为后续16位指令字(若有))。 */
    HS_MOXIE_CORE_IO_TICK_ENTER,                                        /**< 节拍进入,时钟节拍开始时调用。地址为当前PC值,数据为剩余节拍数(类型为size_t)。 */
    HS_MOXIE_CORE_IO_TICK_EXIT,                                         /**< 节拍退出,时钟节拍结束时调用。地址为当前PC值,数据为剩余节拍数(类型为size_t)。 */
    HS_MOXIE_CORE_IO_SWI,                                               /**< 软件中断(系统调用)处理。地址为swi中断号。若返回true表示已由用户处理，内核将继续执行下一条指令；返回false表示未处理，内核将按体系结构进入异常处理流程。 */
} hs_moxie_core_io_opt_t;                                               /**< moxie IO选项。IO选项通常占32位中的低24位 */

typedef enum
{
    HS_MOXIE_CORE_IO_OPERATOR_CORE=       (0UL << 24),                            /**< 操作者为内核，通常表示是由指令操作的 */
    HS_MOXIE_CORE_IO_OPERATOR_HW=         (1UL << 24),                            /**< 操作者为硬件实现，表示该操作由硬件自动操作时调用。 */
    HS_MOXIE_CORE_IO_OPERATOR_EXTERN=     (2UL << 24),                            /**< 操作者为外部操作，表示该操作由外部调用者调用，通常用于外部获取内核状态信息*/
    HS_MOXIE_CORE_IO_OPERATOR_DEFAULT=HS_MOXIE_CORE_IO_OPERATOR_EXTERN,          /**< 默认操作者 */
} hs_moxie_core_io_operator_t;                                                    /**< moxie IO操作者选项。IO选项通常占32位中的高8位 */

/** \brief moxie IO操作
 *
 * \param core hs_moxie_core_t*                 moxie内核指针
 * \param opt uint32_t                          IO选项，可对hs_moxie_core_io_opt_t与hs_moxie_core_io_operator_t进行按位或后传入。
 * \param address uint32_t                      地址
 * \param data uint8_t*                         数据指针,除特殊说明外，IO操作读写的多字节数据均为大端序。
 * \param len size_t                            数据长度
 * \param usr void*                             用户参数
 * \return bool 是否操作成功。注意：对于某些必须成功的操作（如读写通用寄存器与PC寄存器）无需判断返回值。
 *
 */
typedef bool (*hs_moxie_core_io_t)(hs_moxie_core_t *core,uint32_t opt,uint32_t address,uint8_t *data,size_t len,void *usr);

/*
 * Sv32页表寄存器(satp)定义。
 * 仅支持Sv32模式(MODE=1)；MODE=0时按无MMU处理。
 */
#define HS_MOXIE_CORE_SATP_MODE         (1UL << 31)        /**< Sv32模式位 */
#define HS_MOXIE_CORE_SATP_ASID         (0x7FC00000UL)     /**< ASID */
#define HS_MOXIE_CORE_SATP_PPN          (0x003FFFFFUL)     /**< 根页表PPN */
#define HS_MOXIE_CORE_SATP_MODE_BARE    0                   /**< 无MMU */
#define HS_MOXIE_CORE_SATP_MODE_SV32    1                   /**< Sv32 */

/*
 * MIE寄存器位定义。
 * 当前仅定时器中断(MTIE)参与中断处理，其余位保留。
 */
#define HS_MOXIE_CORE_MIE_SSIE          (1UL << 1)
#define HS_MOXIE_CORE_MIE_MSIE          (1UL << 3)
#define HS_MOXIE_CORE_MIE_STIE          (1UL << 5)
#define HS_MOXIE_CORE_MIE_MTIE          (1UL << 7)
#define HS_MOXIE_CORE_MIE_SEIE          (1UL << 9)
#define HS_MOXIE_CORE_MIE_MEIE          (1UL << 11)

/*
 * mip寄存器位定义。
 * MSIP可由程序写入以触发软件中断；MTIP/MEIP为只读状态位。
 */
#define HS_MOXIE_CORE_MIP_MSIP          (1UL << 3)
#define HS_MOXIE_CORE_MIP_MTIP          (1UL << 7)
#define HS_MOXIE_CORE_MIP_MEIP          (1UL << 11)

/*
 * mstatus寄存器位定义。
 * MIE用于全局中断使能，仅影响中断，不影响swi与其它异常。
 */
#define HS_MOXIE_CORE_MSTATUS_MIE       (1UL << 3)
#define HS_MOXIE_CORE_MSTATUS_DEFAULT   HS_MOXIE_CORE_MSTATUS_MIE

/** \brief 定时器中断附带值 */
#define HS_MOXIE_CORE_INTERRUPT_TIMER   7
/** \brief 软件中断附带值 */
#define HS_MOXIE_CORE_INTERRUPT_SOFTWARE 3
/** \brief 外部中断附带值 */
#define HS_MOXIE_CORE_INTERRUPT_EXTERNAL 11

/** \brief moxie异常原因
 *
 */
typedef enum
{
    HS_MOXIE_CORE_EXCEPTION_DIV0=0,         /**< 除0错误 */
    HS_MOXIE_CORE_EXCEPTION_BAD=1,          /**< 指令错误(非法指令) */
    HS_MOXIE_CORE_EXCEPTION_IRQ=2,          /**< 中断请求 */
    HS_MOXIE_CORE_EXCEPTION_SWI=3,          /**< 软件中断 */
    HS_MOXIE_CORE_EXCEPTION_MMU_MISS=4,     /**< MMU未命中/内存访问错误 */
    HS_MOXIE_CORE_EXCEPTION_BREAK=16,       /**< 断点 */
} hs_moxie_core_exception_t;                /**< moxie异常原因 */

/** \brief moxie特殊寄存器编号
 *
 */
typedef enum
{
    HS_MOXIE_CORE_SREG_STATUS=0,                    /**< 状态寄存器，bit0为中断使能 */
    HS_MOXIE_CORE_SREG_EXCEPTION_HANDLER=1,         /**< 异常处理程序地址 */
    HS_MOXIE_CORE_SREG_EXCEPTION_CAUSE=2,           /**< 异常原因 */
    HS_MOXIE_CORE_SREG_SWI_NUMBER=3,                /**< swi中断号 */
    HS_MOXIE_CORE_SREG_SUPERVISOR_SP=4,             /**< 异常处理使用的栈指针 */
    HS_MOXIE_CORE_SREG_EXCEPTION_PC=5,              /**< 进入异常处理时的PC值 */
    HS_MOXIE_CORE_SREG_SIM_RETURN_ADDR=6,           /**< 模拟器返回值地址(moxiebox扩展) */
    HS_MOXIE_CORE_SREG_SIM_RETURN_LEN=7,            /**< 模拟器返回值长度/初始栈地址(moxiebox扩展) */
    HS_MOXIE_CORE_SREG_DEVICE_TREE=9,               /**< 设备树地址 */
    HS_MOXIE_CORE_SREG_SATP=10,                     /**< Sv32页表基址(satp) */
    HS_MOXIE_CORE_SREG_MIE=11,                      /**< 中断使能(mie) */
    HS_MOXIE_CORE_SREG_TIME_LO=12,                  /**< 定时器计数低32位 */
    HS_MOXIE_CORE_SREG_TIME_HI=13,                  /**< 定时器计数高32位 */
    HS_MOXIE_CORE_SREG_TIMECMP_LO=14,               /**< 定时器比较值低32位 */
    HS_MOXIE_CORE_SREG_TIMECMP_HI=15,               /**< 定时器比较值高32位 */
    HS_MOXIE_CORE_SREG_MSTATUS=16,                  /**< 全局中断状态(mstatus) */
    HS_MOXIE_CORE_SREG_MIP=17,                      /**< 中断挂起(mip) */
} hs_moxie_core_sreg_t;                             /**< moxie特殊寄存器编号 */

/** \brief  初始化 hs_moxie_core_io_t (大端序)
 *
 * \param mem void* 大小至少为hs_moxie_core_size()返回的大小的内存的指针
 * \param io hs_moxie_core_io_t moxie 内核IO
 * \param usr void* 用户指针
 * \return hs_moxie_core_t* 成功返回地址，失败返回NULL
 *
 */
hs_moxie_core_t *hs_moxie_core_init(void *mem,hs_moxie_core_io_t io,void *usr);

/** \brief moxie内核节拍，注意：此函数不是线程安全的，必要时需要加锁。
 *
 * \param core hs_moxie_core_t* moxie内核指针
 * \param cycles size_t 周期数
 *
 */
void hs_moxie_core_tick(hs_moxie_core_t * core,size_t cycles);

/** \brief moxie内核复位，注意：此函数不是线程安全的，必要时需要加锁。
 *
 * \param core hs_moxie_core_t* moxie内核指针
 *
 */
void hs_moxie_core_reset(hs_moxie_core_t * core);

/** \brief 获取Sv32 satp寄存器缓存值
 *
 * \param core hs_moxie_core_t* moxie内核指针
 * \return uint32_t satp值
 *
 */
uint32_t hs_moxie_core_satp_get(hs_moxie_core_t *core);

/** \brief 设置Sv32 satp寄存器缓存值
 *
 * \param core hs_moxie_core_t* moxie内核指针
 * \param value uint32_t satp值
 *
 */
void hs_moxie_core_satp_set(hs_moxie_core_t *core,uint32_t value);

/** \brief 获取mstatus缓存值
 *
 * \param core hs_moxie_core_t* moxie内核指针
 * \return uint32_t mstatus值
 *
 */
uint32_t hs_moxie_core_mstatus_get(hs_moxie_core_t *core);

/** \brief 设置mstatus缓存值
 *
 * \param core hs_moxie_core_t* moxie内核指针
 * \param value uint32_t mstatus值
 *
 */
void hs_moxie_core_mstatus_set(hs_moxie_core_t *core,uint32_t value);

/** \brief 获取mip挂起状态
 *
 * \param core hs_moxie_core_t* moxie内核指针
 * \return uint32_t mip值
 *
 */
uint32_t hs_moxie_core_mip_get(hs_moxie_core_t *core);

/** \brief 设置mip挂起状态(仅MSIP可写)
 *
 * \param core hs_moxie_core_t* moxie内核指针
 * \param value uint32_t mip值
 *
 */
void hs_moxie_core_mip_set(hs_moxie_core_t *core,uint32_t value);

/** \brief 获取定时器计数(time)
 *
 * \param core hs_moxie_core_t* moxie内核指针
 * \return uint64_t 定时器计数
 *
 */
uint64_t hs_moxie_core_timer_time_get(hs_moxie_core_t *core);

/** \brief 设置定时器计数(time)
 *
 * \param core hs_moxie_core_t* moxie内核指针
 * \param value uint64_t 定时器计数
 *
 */
void hs_moxie_core_timer_time_set(hs_moxie_core_t *core,uint64_t value);

/** \brief 推进定时器计数(time)
 *
 * \param core hs_moxie_core_t* moxie内核指针
 * \param delta uint64_t 增加的时间
 *
 */
void hs_moxie_core_timer_time_advance(hs_moxie_core_t *core,uint64_t delta);

/** \brief 获取定时器比较值(timecmp)
 *
 * \param core hs_moxie_core_t* moxie内核指针
 * \return uint64_t 定时器比较值
 *
 */
uint64_t hs_moxie_core_timer_timecmp_get(hs_moxie_core_t *core);

/** \brief 设置定时器比较值(timecmp)
 *
 * \param core hs_moxie_core_t* moxie内核指针
 * \param value uint64_t 定时器比较值
 *
 */
void hs_moxie_core_timer_timecmp_set(hs_moxie_core_t *core,uint64_t value);

/** \brief moxie执行IO操作
 *
 * \param core hs_moxie_core_t*                 moxie内核指针
 * \param opt hs_moxie_core_io_opt_t            IO选项
 * \param address uint32_t                      地址
 * \param data uint8_t*                         数据指针
 * \param len size_t                            数据长度
 * \return bool 是否操作成功。
 *
 */
bool hs_moxie_core_io(hs_moxie_core_t *core,hs_moxie_core_io_opt_t opt,uint32_t address,uint8_t *data,size_t len);

/** \brief moxie异常提升
 *
 * \param core hs_moxie_core_t* moxie内核指针
 * \param cause uint32_t 异常原因，见hs_moxie_core_exception_t
 * \param value uint32_t 异常附带的值(如swi中断号、出错地址等)
 * \return bool 是否成功进入异常等待流程
 *
 */
bool hs_moxie_core_exception_raise(hs_moxie_core_t *core,uint32_t cause,uint32_t value);

/** \brief moxie异常清除
 *
 * \param core hs_moxie_core_t* moxie内核指针
 * \return bool 是否清除成功
 *
 */
bool hs_moxie_core_exception_clear(hs_moxie_core_t *core);

/** \brief moxie中断提升
 *
 * \param core hs_moxie_core_t* moxie内核指针
 * \param value uint32_t 中断附带的值
 * \return bool 是否成功进入中断等待流程
 *
 */
bool hs_moxie_core_interrupt_raise(hs_moxie_core_t *core,uint32_t value);

/** \brief moxie中断清除
 *
 * \param core hs_moxie_core_t* moxie内核指针
 * \return bool 是否清除成功
 *
 */
bool hs_moxie_core_interrupt_clear(hs_moxie_core_t *core);

/** \brief 触发外部中断
 *
 * \param core hs_moxie_core_t* moxie内核指针
 * \return bool 是否成功
 *
 */
bool hs_moxie_core_external_interrupt_raise(hs_moxie_core_t *core);

/** \brief 清除外部中断
 *
 * \param core hs_moxie_core_t* moxie内核指针
 * \return bool 是否成功
 *
 */
bool hs_moxie_core_external_interrupt_clear(hs_moxie_core_t *core);

/** \brief 外部中断是否等待处理
 *
 * \param core hs_moxie_core_t* moxie内核指针
 * \return bool 是否等待处理
 *
 */
bool hs_moxie_core_external_interrupt_pending(hs_moxie_core_t *core);

/** \brief 触发软件中断
 *
 * \param core hs_moxie_core_t* moxie内核指针
 * \return bool 是否成功
 *
 */
bool hs_moxie_core_software_interrupt_raise(hs_moxie_core_t *core);

/** \brief 清除软件中断
 *
 * \param core hs_moxie_core_t* moxie内核指针
 * \return bool 是否成功
 *
 */
bool hs_moxie_core_software_interrupt_clear(hs_moxie_core_t *core);

/** \brief 软件中断是否等待处理
 *
 * \param core hs_moxie_core_t* moxie内核指针
 * \return bool 是否等待处理
 *
 */
bool hs_moxie_core_software_interrupt_pending(hs_moxie_core_t *core);

/** \brief moxie是否处于异常等待(无法进入异常处理程序)状态
 *
 * \param core hs_moxie_core_t* moxie内核指针
 * \return bool 是否处于异常等待状态
 *
 */
bool hs_moxie_core_halted(hs_moxie_core_t *core);

/** \brief moxie获取当前条件码
 *
 * \param core hs_moxie_core_t* moxie内核指针
 * \return uint32_t 条件码
 *
 */
uint32_t hs_moxie_core_condition_code(hs_moxie_core_t *core);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __HS_MOXIE_CORE_H__
