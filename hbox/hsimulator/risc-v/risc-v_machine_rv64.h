/***************************************************************
 * Name:      risc-v_machine_rv64.h
 * Purpose:   声明risc-v_machine_rv64接口(虚拟CPU模型-hsimulator)
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-12
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HS_RISC_V_MACHINE_RV64_H__
#define __HS_RISC_V_MACHINE_RV64_H__
#include "risc-v_core_rv64.h"
#include "stdbool.h"
#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

struct hs_risc_v_machine_rv64;
typedef struct hs_risc_v_machine_rv64 hs_risc_v_machine_rv64_t;

/*
 * 控制台客户CSR
 *   CONSOLE_OUT    : 写低8位到控制台输出
 *   CONSOLE_IN     : 读取控制台输入字节
 *   CONSOLE_STATUS : 读状态，bit0=有输入数据，bit1=输出就绪
 */
/*
 * 控制台CSR地址可由用户统一修改:
 *   在包含本头文件前定义以下宏，或者通过编译器-D选项定义，例如:
 *   -DHS_RISC_V_MACHINE_CSR_CONSOLE_OUT=0x810
 */
#ifndef HS_RISC_V_MACHINE_CSR_CONSOLE_OUT
#define HS_RISC_V_MACHINE_CSR_CONSOLE_OUT      0x800
#endif
#ifndef HS_RISC_V_MACHINE_CSR_CONSOLE_IN
#define HS_RISC_V_MACHINE_CSR_CONSOLE_IN       0x801
#endif
#ifndef HS_RISC_V_MACHINE_CSR_CONSOLE_STATUS
#define HS_RISC_V_MACHINE_CSR_CONSOLE_STATUS   0x802
#endif

#ifndef HS_RISC_V_MACHINE_RV64_CSR_CONSOLE_OUT
#define HS_RISC_V_MACHINE_RV64_CSR_CONSOLE_OUT      HS_RISC_V_MACHINE_CSR_CONSOLE_OUT
#endif
#ifndef HS_RISC_V_MACHINE_RV64_CSR_CONSOLE_IN
#define HS_RISC_V_MACHINE_RV64_CSR_CONSOLE_IN       HS_RISC_V_MACHINE_CSR_CONSOLE_IN
#endif
#ifndef HS_RISC_V_MACHINE_RV64_CSR_CONSOLE_STATUS
#define HS_RISC_V_MACHINE_RV64_CSR_CONSOLE_STATUS   HS_RISC_V_MACHINE_CSR_CONSOLE_STATUS
#endif

/*
 * 内存区域(Flash/RAM/ROM)
 * 注意:地址为CPU地址，回调函数应自行根据base计算实际偏移。
 */
typedef struct hs_risc_v_machine_rv64_memory hs_risc_v_machine_rv64_memory_t;
struct hs_risc_v_machine_rv64_memory
{
    uint32_t    base;       /**< 区域起始地址(CPU地址) */
    size_t      size;       /**< 区域大小 */
    void       *usr;        /**< 用户指针 */
    /** \brief 从区域读取数据
     *
     * \param mem const hs_risc_v_machine_rv64_memory_t* 区域指针
     * \param address uint32_t CPU地址
     * \param data void* 数据指针，小端序
     * \param len size_t 数据长度
     * \return size_t 已读取长度
     *
     */
    size_t      (*read)(const hs_risc_v_machine_rv64_memory_t *mem,uint32_t address,void *data,size_t len);
    /** \brief 向区域写入数据
     *
     * \param mem const hs_risc_v_machine_rv64_memory_t* 区域指针
     * \param address uint32_t CPU地址
     * \param data const void* 数据指针，小端序
     * \param len size_t 数据长度
     * \return size_t 已写入长度
     *
     */
    size_t      (*write)(const hs_risc_v_machine_rv64_memory_t *mem,uint32_t address,const void *data,size_t len);
};

/*
 * 可选实现回调
 */
typedef bool (*hs_risc_v_machine_rv64_csr_read_t)(hs_risc_v_machine_rv64_t *machine,void *usr,uint32_t csr,uint64_t *value);
typedef bool (*hs_risc_v_machine_rv64_csr_write_t)(hs_risc_v_machine_rv64_t *machine,void *usr,uint32_t csr,uint64_t *value);

typedef size_t (*hs_risc_v_machine_rv64_device_tree_read_t)(hs_risc_v_machine_rv64_t *machine,void *usr,uint32_t address,void *data,size_t len);

typedef size_t (*hs_risc_v_machine_rv64_io_read_t)(hs_risc_v_machine_rv64_t *machine,void *usr,uint32_t address,void *data,size_t len);
typedef size_t (*hs_risc_v_machine_rv64_io_write_t)(hs_risc_v_machine_rv64_t *machine,void *usr,uint32_t address,const void *data,size_t len);

typedef void (*hs_risc_v_machine_rv64_instruction_enter_t)(hs_risc_v_machine_rv64_t *machine,void *usr,uint32_t pc,uint32_t instruction);
typedef void (*hs_risc_v_machine_rv64_instruction_exit_t)(hs_risc_v_machine_rv64_t *machine,void *usr,uint32_t pc,uint32_t instruction);

typedef bool (*hs_risc_v_machine_rv64_console_put_t)(hs_risc_v_machine_rv64_t *machine,void *usr,uint8_t data);
typedef bool (*hs_risc_v_machine_rv64_console_get_t)(hs_risc_v_machine_rv64_t *machine,void *usr,uint8_t *data);
typedef bool (*hs_risc_v_machine_rv64_console_has_data_t)(hs_risc_v_machine_rv64_t *machine,void *usr);

/*
 * 虚拟CPU模型
 */
struct hs_risc_v_machine_rv64
{
    /*
     * 必须实现的内部寄存器
     */
    uint64_t    x[32];                  /**< X0~X31寄存器 */
    uint64_t    pc;                     /**< PC寄存器 */
    uint64_t    xlen;                   /**< XLEN寄存器(只读，固定为64) */

    /*
     * 浮点寄存器
     */
    uint64_t    f[32];                  /**< F0~F31寄存器 */
    uint32_t    fcsr;                   /**< FCSR寄存器 */
    uint32_t    flen;                   /**< FLEN寄存器(只读) */

    /*
     * 复位参数
     */
    uint32_t    hart_id;                /**< hart id，复位时写入A0(X10) */
    uint32_t    dtb_address;            /**< 设备树地址，复位时写入A1(X11) */
    uint32_t    entry;                  /**< 入口PC，为0时使用复位向量(0x40) */

    /*
     * 内存区域(地址空间高4位)
     * 0x00000000: 可映射程序代码块(优先flash，其次rom)
     * 0x10000000: Flash
     * 0x20000000: RAM
     * 0xD0000000: 内部ROM
     */
    hs_risc_v_machine_rv64_memory_t flash;
    hs_risc_v_machine_rv64_memory_t ram;
    hs_risc_v_machine_rv64_memory_t rom;

    /*
     * 可选实现(回调函数)
     */
    void       *csr_usr;
    hs_risc_v_machine_rv64_csr_read_t   csr_read;
    hs_risc_v_machine_rv64_csr_write_t  csr_write;

    void       *device_tree_usr;
    hs_risc_v_machine_rv64_device_tree_read_t   device_tree_read;

    void       *io_usr;
    hs_risc_v_machine_rv64_io_read_t            io_read;
    hs_risc_v_machine_rv64_io_write_t           io_write;

    void       *instruction_usr;
    hs_risc_v_machine_rv64_instruction_enter_t  instruction_enter;
    hs_risc_v_machine_rv64_instruction_exit_t   instruction_exit;

    void       *console_usr;
    hs_risc_v_machine_rv64_console_put_t        console_put;
    hs_risc_v_machine_rv64_console_get_t        console_get;
    hs_risc_v_machine_rv64_console_has_data_t   console_has_data;

    /*
     * 部分CSR/计时器
     */
    uint64_t    mtime;                  /**< mtime */
    uint64_t    mtimecmp;               /**< mtimecmp */
    uint64_t    cycle;                  /**< cycle/mcycle */
    uint64_t    instret;                /**< instret/minstret */

    /*
     * 标准CSR(机器状态与中断)
     */
    uint64_t    mstatus;
    uint64_t    medeleg;
    uint64_t    mideleg;
    uint64_t    mie;
    uint64_t    mtvec;
    uint64_t    mcounteren;
    uint64_t    mscratch;
    uint64_t    mepc;
    uint64_t    mcause;
    uint64_t    mtval;
    uint64_t    mip;
    uint64_t    mstatush;
    uint64_t    sstatus;
    uint64_t    sie;
    uint64_t    stvec;
    uint64_t    scounteren;
    uint64_t    sscratch;
    uint64_t    sepc;
    uint64_t    scause;
    uint64_t    stval;
    uint64_t    sip;
    uint64_t    satp;
    uint32_t    csr_written;            /**< 已写入CSR标记 */

    /*
     * 运行状态
     */
    struct
    {
        uint32_t    started:1;              /**< 是否已开始执行(首次step时复位) */
        uint32_t    reservation_valid:1;    /**< LR/SC保留标记 */
    } flags;
    uint32_t    reservation_address;    /**< LR/SC保留地址 */

    /*
     * 内核对象存储
     */
    union
    {
        uint8_t     bytes[HS_RISC_V_CORE_RV64_SIZE()];
        uint64_t    align;
    } core;
};

/** \brief hs_risc_v_machine_rv64_t结构体大小
 *
 * \return size_t 结构体大小
 *
 */
size_t hs_risc_v_machine_rv64_size(void);

/** \brief 初始化虚拟CPU模型
 *
 * \param mem void* 大小至少为hs_risc_v_machine_rv64_size()的内存
 * \return hs_risc_v_machine_rv64_t* 成功返回指针，失败返回NULL
 *
 * 注意:初始化后请设置RAM/ROM/Flash回调，并调用hs_risc_v_machine_rv64_check()检查。
 * 首次调用hs_risc_v_machine_rv64_step()时会自动完成复位。
 *
 */
hs_risc_v_machine_rv64_t *hs_risc_v_machine_rv64_init(void *mem);

/** \brief 检查虚拟CPU模型配置
 *
 * \param machine hs_risc_v_machine_rv64_t* 虚拟CPU模型指针
 * \return bool 配置是否有效。RAM读/写回调必须设置，否则返回false。
 *
 */
bool hs_risc_v_machine_rv64_check(hs_risc_v_machine_rv64_t *machine);

/** \brief 虚拟CPU模型步进
 *
 * \param machine hs_risc_v_machine_rv64_t* 虚拟CPU模型指针
 * \param elapsed_us uint32_t 距离上次调用经过的时间(微秒)，用于维护mtime
 * \param count size_t 本次执行的指令条数
 * \return int 0=正常，1=处于WFI状态，-1=参数错误或配置检查失败
 *
 * 注意:首次调用时会自动完成复位。
 *
 */
int hs_risc_v_machine_rv64_step(hs_risc_v_machine_rv64_t *machine,uint32_t elapsed_us,size_t count);

/** \brief 获取内核指针
 *
 * \param machine hs_risc_v_machine_rv64_t* 虚拟CPU模型指针
 * \return hs_risc_v_core_rv64_t* 内核指针
 *
 */
hs_risc_v_core_rv64_t *hs_risc_v_machine_rv64_core(hs_risc_v_machine_rv64_t *machine);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __HS_RISC_V_MACHINE_RV64_H__
