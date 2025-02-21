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

#ifndef HS_RISC_V_CORE_RV32_SIZE
/** \brief hs_risc_v_core_rv32_t结构体大小,一般用于静态分配，注意:可能大于hs_risc_v_core_rv32_size()返回的值。
 *
 *
 */
#define HS_RISC_V_CORE_RV32_SIZE()  (sizeof(uintptr_t)*2)
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
    HS_RISC_V_CORE_RV32_IO_TICK_ENTER,                                  /**< 节拍进入,时钟节拍开始时调用。地址为当前PC值,数据为剩余节拍数(类型为size_t)。 */
    HS_RISC_V_CORE_RV32_IO_TICK_EXIT,                                   /**< 节拍退出,时钟节拍开始时调用。地址为当前PC值,数据为剩余节拍数(类型为size_t)。 */
} hs_risc_v_core_rv32_io_opt_t;                                         /**< RISC-V RV32 IO选项 */


/** \brief RISC-V RV32 IO操作
 *
 * \param core hs_risc_v_core_rv32_t*           RISC-V内核指针
 * \param opt hs_risc_v_core_rv32_io_opt_t      IO选项
 * \param address uint32_t                      地址
 * \param data uint8_t*                         数据指针
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

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __HS_RISC_V_CORE_RV32_H__
