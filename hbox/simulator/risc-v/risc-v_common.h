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
({                                                              \
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
})
#endif // HS_RISC_V_COMMOM_MEMORY_BYTEORDER_FIX

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __HS_RISC_V_COMMON_H__
