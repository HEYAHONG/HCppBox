/***************************************************************
 * Name:      risc-v_core_rv32.h
 * Purpose:   声明risc-v_core_rv32接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-02-21
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HS_RISC_V_CORE_RV32_H__
#define __HS_RISC_V_CORE_RV32_H__
#include "risc-v_common.h"
#include "risc-v_opcodes.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

struct hs_risc_v_core_rv32;
typedef struct hs_risc_v_core_rv32 hs_risc_v_core_rv32_t;

/** \brief hs_risc_v_core_rv32_t结构体大小 通常用于创建hs_risc_v_core_rv32_t结构体
 *
 * \return size_t hs_risc_v_core_rv32_t结构体大小
 *
 */
size_t hs_risc_v_core_rv32_size(void);

#define HS_RISC_V_CORE_RV32_ALIGNED_FILL_SIZE(MAXTYPE,TYPE) (sizeof(MAXTYPE)>(sizeof(TYPE))?sizeof(MAXTYPE)-(sizeof(TYPE)):0)

#ifndef HS_RISC_V_CORE_RV32_SIZE
/** \brief hs_risc_v_core_rv32_t结构体大小,一般用于静态分配，注意:可能大于hs_risc_v_core_rv32_size()返回的值。
 *
 *
 */
#define HS_RISC_V_CORE_RV32_SIZE()  (sizeof(uintptr_t)*2+sizeof(uint32_t)*3+HS_RISC_V_CORE_RV32_ALIGNED_FILL_SIZE(uintptr_t,uint32_t))
#endif // HS_RISC_V_CORE_RV32_SIZE


typedef enum
{
    HS_RISC_V_CORE_RV32_IO_RESET,                                       /**< IO初始化 */
    HS_RISC_V_CORE_RV32_IO_ENTRY_PC_READ,                               /**< 入口PC值读取，必须实现，否则复位状态将异常*/
    HS_RISC_V_CORE_RV32_IO_MEMORY_READ,                                 /**< 内存读取，内存包括数据与程序 */
    HS_RISC_V_CORE_RV32_IO_MEMORY_WRITE,                                /**< 内存写入，内存包括数据与程序 */
    HS_RISC_V_CORE_RV32_IO_X_REGISTER_READ,                             /**< X寄存器读取，此选项必须实现 */
    HS_RISC_V_CORE_RV32_IO_X_REGISTER_WRITE,                            /**< X寄存器写入，此选项必须实现 */
    HS_RISC_V_CORE_RV32_IO_PC_REGISTER_READ,                            /**< PC寄存器读取，此选项必须实现 */
    HS_RISC_V_CORE_RV32_IO_PC_REGISTER_WRITE,                           /**< PC寄存器写入，此选项必须实现 */
    HS_RISC_V_CORE_RV32_IO_INSTRUCTION_ENTER,                           /**< 指令进入,开始执行指令时调用。通常用于调试或者用户处理指令。地址为当前PC值,数据为已经执行的指令(类型为uint32_t)。*/
    HS_RISC_V_CORE_RV32_IO_INSTRUCTION_EXIT,                            /**< 指令退出,结束执行指令时调用。通常用于调试或者用户处理指令。地址为当前PC值(可能已被指令修改),数据为已经执行的指令(类型为uint32_t)。*/
    HS_RISC_V_CORE_RV32_IO_TICK_ENTER,                                  /**< 节拍进入,时钟节拍开始时调用。地址为当前PC值,数据为剩余节拍数(类型为size_t)。 */
    HS_RISC_V_CORE_RV32_IO_TICK_EXIT,                                   /**< 节拍退出,时钟节拍开始时调用。地址为当前PC值,数据为剩余节拍数(类型为size_t)。 */
    HS_RISC_V_CORE_RV32_IO_CUSTOM_INSTRUCTION_EXEC,                     /**< custom指令（RISC-V预留了部分自定义指令空间）执行。地址为下一条指令PC值,数据为待执行的指令(类型为uint32_t)。 */
    HS_RISC_V_CORE_RV32_IO_INSTRUCTION_FENCE_I_EXEC,                    /**< fence_i指令(Zifencei扩展指令集)执行。地址为下一条指令PC值,数据为待执行的指令(类型为uint32_t)。 */
} hs_risc_v_core_rv32_io_opt_t;                                         /**< RISC-V RV32 IO选项 */


/** \brief RISC-V RV32 IO操作
 *
 * \param core hs_risc_v_core_rv32_t*           RISC-V内核指针
 * \param opt hs_risc_v_core_rv32_io_opt_t      IO选项
 * \param address uint32_t                      地址
 * \param data uint8_t*                         数据指针,若IO选项未特殊说明数据类型，IO操作读写的数据均为小端序（若在大端序平台上运行，需要先转化为小端序再返回），若已说明数据类型，可直接强制转换指针类型然后访问。
 * \param len size_t                            数据长度
 * \param usr void*                             用户参数
 * \return bool 是否操作成功。注意：对于某些必须成功的操作（如读写X寄存器与PC寄存器）无需判断返回值。
 *
 */
typedef bool (*hs_risc_v_core_rv32_io_t)(hs_risc_v_core_rv32_t *core,hs_risc_v_core_rv32_io_opt_t opt,uint32_t address,uint8_t *data,size_t len,void *usr);


/** \brief  初始化 hs_risc_v_core_rv32_io_t
 *
 * \param mem void* 大小至少为hs_risc_v_core_rv32_size()返回的大小的内存的指针
 * \param io hs_risc_v_core_rv32_io_t RISC-V RV32 内核IO
 * \param usr void* 用户指针 用户指针
 * \return hs_risc_v_core_rv32_t* 成功返回地址，失败返回NULL
 *
 */
hs_risc_v_core_rv32_t *hs_risc_v_core_rv32_init(void *mem,hs_risc_v_core_rv32_io_t io,void *usr);

/** \brief RISC-V RV32 内核节拍，注意：此函数不是线程安全的，必要时需要加锁。
 *
 * \param core hs_risc_v_core_rv32_t* RISC-V RV32 内核指针
 * \param cycles size_t 周期数
 *
 */
void hs_risc_v_core_rv32_tick(hs_risc_v_core_rv32_t * core,size_t cycles);

/** \brief RISC-V RV32内核复位，注意：此函数不是线程安全的，必要时需要加锁。
 *
 * \param core hs_risc_v_core_rv32_t* RISC-V RV32 内核指针
 *
 */
void hs_risc_v_core_rv32_reset(hs_risc_v_core_rv32_t * core);


/** \brief RISC-V RV32 执行IO操作
 *
 * \param core hs_risc_v_core_rv32_t*           RISC-V内核指针
 * \param opt hs_risc_v_core_rv32_io_opt_t      IO选项
 * \param address uint32_t                      地址
 * \param data uint8_t*                         数据指针
 * \param len size_t                            数据长度
 * \return bool 是否操作成功。注意：对于某些必须成功的操作（如读写X寄存器与PC寄存器）无需判断返回值。
 *
 */
bool hs_risc_v_core_rv32_io(hs_risc_v_core_rv32_t *core,hs_risc_v_core_rv32_io_opt_t opt,uint32_t address,uint8_t *data,size_t len);

/** \brief  RISC-V RV32  获取支持的指令集集合
 *
 * \param core hs_risc_v_core_rv32_t*   RISC-V内核指针
 * \return uint32_t 支持的指令集集合（见hs_risc_v_common_instruction_set_t）
 *
 */
uint32_t hs_risc_v_core_rv32_sets_get(hs_risc_v_core_rv32_t *core);


/** \brief  RISC-V RV32  关闭某个指令集（此时用户可通过HS_RISC_V_CORE_RV32_IO_CUSTOM_INSTRUCTION_EXEC执行原指令集的工作），注意：复位后此操作的设置将失效
 *
 * \param core hs_risc_v_core_rv32_t*                   RISC-V内核指针
 * \param ins_set hs_risc_v_common_instruction_set_t    指令集类型，注意：需要先关闭依赖的指令才能关闭被依赖的指令
 * \return uint32_t 支持的指令集集合（见hs_risc_v_common_instruction_set_t）
 *
 */
uint32_t hs_risc_v_core_rv32_sets_disable_set(hs_risc_v_core_rv32_t *core,hs_risc_v_common_instruction_set_t ins_set);


/** \brief  RISC-V RV32 异常提升
 *
 * \param core hs_risc_v_core_rv32_t*   RISC-V内核指针
 * \param cause int                     case值。当为狭义的异常时，可见HS_RISC_V_OPCODES_EXCEPTION_*，否则表示中断号(注意：中断使能与中断优先级由用户实现。)。在本模拟器中，RV32一般不小于0且不应超过31。
 * \param interrupt bool                是否为中断。RISC-V将狭义的异常与中断统称为异常。当此值为false时表示狭义的异常，否则表示中断
 * \return bool 是否成功进入处理流程（如设置中断请求标志），若未进入处理流程，用户可选择重试
 *
 */
bool hs_risc_v_core_rv32_exception_raise(hs_risc_v_core_rv32_t *core,int cause,bool interrupt);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __HS_RISC_V_CORE_RV32_H__
