/***************************************************************
 * Name:      risc-v_common.h
 * Purpose:   声明risc-v common接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-01-29
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HS_RISC_V_COMMON_H__
#define __HS_RISC_V_COMMON_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "stdlib.h"
#include "stdint.h"
#include "stdbool.h"
#include "string.h"


/** \brief 获取指令长度,通常情况下，risc-v的指令长度为32位/16位，但risc-v也规定了指令长度（一般是16位的整倍数）的编码。
 *
 *
 * \param instruction uint32_t 指令（超过32位取低32位）
 * \return size_t 指令长度，字节数
 *
 */
size_t hs_risc_v_common_instruction_length(uint32_t instruction);


typedef union
{
    uint8_t bytes[1];
    uint8_t value;
}   hs_risc_v_common_memory_byte_t;         /**< 字节类型 */


typedef union
{
    uint8_t  bytes[2];
    uint16_t value;
}   hs_risc_v_common_memory_halfword_t;     /**< 半字类型 */

typedef union
{
    uint8_t  bytes[4];
    uint32_t value;
}   hs_risc_v_common_memory_word_t;         /**< 字类型 */

typedef union
{
    uint8_t  bytes[8];
    uint64_t value;
}   hs_risc_v_common_memory_doubleword_t;   /**< 双字类型 */

typedef union
{
    uint8_t  bytes[16];
}   hs_risc_v_common_memory_quadword_t;   /**< 四字类型 */

#ifndef HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX
/*
 *  对半字、字、双字的大小端进行修复。在读取字节数据与写入字节数据时进行调用。第一次调用后，确保value成员可用，第二次调用时又回复原有字节序，即bytes成员可用。
 *  参数Data为变量名称。
 */
#define HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX(Data)             \
{                                                               \
    hs_risc_v_common_memory_halfword_t m_hs_byteorder;          \
    m_hs_byteorder.value=0x01;                                  \
    if(m_hs_byteorder.bytes[0]==0)                              \
    {                                                           \
        for(size_t i=0;i<sizeof(Data)/2;i++)                    \
        {                                                       \
            uint8_t temp=(Data).bytes[i];                       \
            (Data).bytes[i]=(Data).bytes[sizeof(Data)-1-i];     \
            (Data).bytes[sizeof(Data)-1-i]=temp;                \
        }                                                       \
    };                                                          \
}
#endif // HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX

typedef enum
{
    /*
     *  基本指令集（整数）。注意：基本指令集不可组合，分配低4位作为基本指令集的标识,扩展指令集每个扩展占1位。
     */
    HS_RISC_V_COMMON_INSTRUCTION_SET_RV32I= (0ULL << (0)),               /**< RV32I基本指令集 */
    HS_RISC_V_COMMON_INSTRUCTION_SET_RV32E= (1ULL << (0)),               /**< RV32E基本指令集 */
    HS_RISC_V_COMMON_INSTRUCTION_SET_RV64I= (2ULL << (0)),               /**< RV64I基本指令集 */
    HS_RISC_V_COMMON_INSTRUCTION_SET_RV64E= (3ULL << (0)),               /**< RV64E基本指令集 */
    HS_RISC_V_COMMON_INSTRUCTION_SET_RV128I=(4ULL << (0)),               /**< RV128I基本指令集 */

    /*
     * 扩展指令集。扩展指令集通常可组合（有些扩展指令值相同表示需要需要根据基本指令集确定最终的指令集）
     */

    //"C"扩展：压缩指令
    HS_RISC_V_COMMON_INSTRUCTION_SET_RVC=   (1ULL << (4)),              /**< RVC扩展指令集 */
    HS_RISC_V_COMMON_INSTRUCTION_SET_RV32C= (1ULL << (4)),              /**< RV32C扩展指令集 */
    HS_RISC_V_COMMON_INSTRUCTION_SET_RV64C= (1ULL << (4)),              /**< RV64C扩展指令集 */
    HS_RISC_V_COMMON_INSTRUCTION_SET_RV128C=(1ULL << (4)),              /**< RV128C扩展指令集 */

    //"A"扩展：原子操作指令
    HS_RISC_V_COMMON_INSTRUCTION_SET_RV32A= (1ULL << (5)),              /**< RV32A扩展指令集 */
    HS_RISC_V_COMMON_INSTRUCTION_SET_RV64A= (1ULL << (5)),              /**< RV64A扩展指令集 */

    //"M"扩展：整数乘除
    HS_RISC_V_COMMON_INSTRUCTION_SET_RV32M= (1ULL << (6)),              /**< RV32M扩展指令集 */
    HS_RISC_V_COMMON_INSTRUCTION_SET_RV64M= (1ULL << (6)),              /**< RV64M扩展指令集 */

} hs_risc_v_common_instruction_set_t;                                   /**< 指令集类型 */

/** \brief 指令集集合中是否有某个指令集
 *
 * \param sets uint32_t 指令集集合
 * \param instruction_set hs_risc_v_common_instruction_set_t  指令集类型
 * \return bool 是否指令集
 *
 */
bool hs_risc_v_common_instruction_set_sets_has_set(uint32_t sets,hs_risc_v_common_instruction_set_t instruction_set);


/** \brief 指令集集合格式化（去除其中不合理的值如不存在的基本指令集、添加某些指令集依赖（指一个指令集依赖另一个指令集））
 *
 * \param sets uint32_t 指令集集合
 * \return uint32_t 格式化后的指令集集合
 *
 */
uint32_t hs_risc_v_common_instruction_set_sets_format(uint32_t sets);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __HS_RISC_V_COMMON_H__
