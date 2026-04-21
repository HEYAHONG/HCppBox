/***************************************************************
 * Name:      hminirv32ima.h
 * Purpose:   声明hminirv32ima接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-04-21
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HMINIRV32IMA_H_INCLUDE__
#define __HMINIRV32IMA_H_INCLUDE__
#include "h3rdparty_port_mini_rv32ima.h"
#include "stdbool.h"
#include "stdint.h"
#include "stdlib.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/*
 * CPU状态
 */
struct hminirv32ima_state
{
    uint32_t regs[32];

    uint32_t pc;
    uint32_t mstatus;
    uint32_t cyclel;
    uint32_t cycleh;

    uint32_t timerl;
    uint32_t timerh;
    uint32_t timermatchl;
    uint32_t timermatchh;

    uint32_t mscratch;
    uint32_t mtvec;
    uint32_t mie;
    uint32_t mip;

    uint32_t mepc;
    uint32_t mtval;
    uint32_t mcause;

    // Note: only a few bits are used.  (Machine = 3, User = 0)
    // Bits 0..1 = privilege.
    // Bit 2 = WFI (Wait for interrupt)
    // Bit 3+ = Load/Store reservation LSBs.
    uint32_t extraflags;

    /** \brief 写入CSR（未在hminirv32ima_state_t结构体中）
     *
     * \param state struct hminirv32ima_state* 状态
     * \param csrno uint32_t CSR编号
     * \param csrvalue uint32_t CSR值
     *
     */
    void (*csr_write)(struct hminirv32ima_state *state,uint32_t csrno,uint32_t csrvalue);
    /** \brief  读取CSR（未在hminirv32ima_state_t结构体中）
     *
     * \param state struct hminirv32ima_state* 状态
     * \param csrno uint32_t CSR编号
     * \param csrvalue uint32_t* CSR值指针
     *
     */
    void (*csr_read)(struct hminirv32ima_state *state,uint32_t csrno,uint32_t* csrvalue);
};

typedef struct hminirv32ima_state hminirv32ima_state_t;

/*
 * 内存操作
 */
struct hminirv32ima_memory
{
    uintptr_t ram_base;     /**< 内存起始地址,默认0x80000000，注意此地址非宿主地址 */
    size_t    ram_size;     /**< 内存大小 */
    /** \brief 从内存加载数据
     *
     * \param mem const struct hminirv32ima_memory * 内存指针
     * \param ram_addr uintptr_t 内存地址
     * \param ptr void* 数据指针，注意：若宿主机不是小端模式则需要转换
     * \param length size_t 数据长度
     * \return size_t 已读取的长度
     *
     */
    size_t    (*mem_load)(const struct hminirv32ima_memory *mem,uintptr_t ram_addr,void *ptr,size_t length);
    /** \brief 存储数据到内存
     *
     * \param mem const struct hminirv32ima_memory * 内存指针
     * \param ram_addr uintptr_t 内存地址
     * \param ptr const void* 数据指针，注意：若宿主机不是小端模式则需要转换
     * \param length size_t  数据长度
     * \return size_t 已写入的长度
     *
     */
    size_t    (*mem_store)(const struct hminirv32ima_memory *mem,uintptr_t ram_addr,const void *ptr,size_t length);
};

typedef struct hminirv32ima_memory hminirv32ima_memory_t;

/*
 * MMIO操作，注意：mini-rv32ima只支持按照4字节对齐操作MMIO,若软件中进行非四字节对齐则可能导致异常（如操作地址后面的地址被覆盖）
 */
struct hminirv32ima_mmio
{
    uintptr_t mmio_base;     /**< 内存起始地址,默认0x10000000，注意此地址非宿主地址 */
    size_t    mmio_size;     /**< 内存大小,默认0x2000000 */
    /** \brief 从mmio加载数据
     *
     * \param mem const struct hminirv32ima_mmio * mmio指针
     * \param mmio_addr uintptr_t mmio地址
     * \param ptr void* 数据指针，注意：若宿主机不是小端模式则需要转换
     * \param length size_t 数据长度
     * \return size_t 已读取的长度
     *
     */
    size_t    (*mmio_load)(const struct hminirv32ima_mmio *mmio,uintptr_t mmio_addr,void *ptr,size_t length);
    /** \brief 存储数据到内存
     *
     * \param mmio const struct hminirv32ima_mmio * 内存指针
     * \param mmio_addr uintptr_t mmio地址
     * \param ptr const void* 数据指针,注意：若宿主机不是小端模式则需要转换
     * \param length size_t  数据长度
     * \return size_t 已写入的长度
     *
     */
    size_t    (*mmio_store)(const struct hminirv32ima_mmio *mmio,uintptr_t mmio_addr,const void *ptr,size_t length);
};

typedef struct hminirv32ima_mmio hminirv32ima_mmio_t;

struct hminirv32ima_core
{
    hminirv32ima_state_t    state;
    hminirv32ima_memory_t   memory;
    hminirv32ima_mmio_t     mmio;
};

typedef struct hminirv32ima_core hminirv32ima_core_t;

/** \brief 初始化mini-rv32ima内核
 *
 * \param core hminirv32ima_core_t* mini-rv32ima内核
 * \param ram_size size_t 内存大小
 *
 */
void hminirv32ima_core_init(hminirv32ima_core_t *core,size_t ram_size);


/** \brief mini-rv32ima内核步进
 *
 * \param core hminirv32ima_core_t* mini-rv32ima内核，注意：一定要初始化完成且设置回掉后使用
 * \param elapsedUs uint32_t 上一步到本步间隔的秒数，用于维护RISCV内部的定时器
 * \param count int 执行指令的条数
 * \return int 错误代码，0=正常运行，1=WFI指令,-1=参数错误
 *
 */
int hminirv32ima_core_step(hminirv32ima_core_t *core,uint32_t elapsedUs, int insn_count );

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HMINIRV32IMA_H_INCLUDE__
