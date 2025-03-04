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

/** \brief 32位指令基础操作码。基础操作码主要指指令低7位。
 *  注意：当基础操作码低2位不为3时，表示该指令为16位指令，应当按照16位指令解析。
 *
 */
typedef enum
{
    HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_LOAD             =   0b0000011,
    HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_STORE            =   0b0100011,
    HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_MADD             =   0b1000011,
    HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_BRANCH           =   0b1100011,
    HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_LOAD_FP          =   0b0000111,
    HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_STORE_FP         =   0b0100111,
    HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_MSUB             =   0b1000111,
    HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_JALR             =   0b1100111,
    HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_CUSTOM_0         =   0b0001011,
    HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_CUSTOM_1         =   0b0101011,
    HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_NMSUB            =   0b1001011,
    HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_RESERVED         =   0b1101011,
    HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_MISC_MEM         =   0b0001111,
    HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_AMO              =   0b0101111,
    HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_NMADD            =   0b1001111,
    HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_JAL              =   0b1101111,
    HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_OP_IMM           =   0b0010011,
    HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_OP               =   0b0110011,
    HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_OP_FP            =   0b1010011,
    HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_SYSTEM           =   0b1110011,
    HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_AUIPC            =   0b0010111,
    HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_LUI              =   0b0110111,
    HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_OP_V             =   0b1010111,
    HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_OP_VE            =   0b1110111,
    HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_OP_IMM_32        =   0b0011011,
    HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_OP_32            =   0b0111011,
    HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_CUSTOM_3         =   0b1011011,
    HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_CUSTOM_4         =   0b1111011,
    /*
     *  下列编码并非操作码，一般用于指示该指令为更高位数的指令
     */
    HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_TO_48BIT_0                   =   0b0011111,
    HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_TO_64BIT                     =   0b0111111,
    HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_TO_48BIT_1                   =   0b1011111,
    HS_RISC_V_COMMON_INSTRUCTION_32BIT_BASE_OPCODE_TO_80BIT_OR_HIGHER           =   0b1111111,
} hs_risc_v_common_instruction_32bit_base_opcode;

/** \brief 将12位数扩展为32位有符号数
 *
 * \param data uint16_t 12位数(以无符号数形式存储)
 * \return int32_t  32位有符号数(可通过直接赋值转换64位有符号数,C语言支持的类型可直接符号扩展)
 *
 */
int32_t hs_risc_v_common_sign_extend_uint12(uint16_t data);


/** \brief 将32位数扩展为64位有符号数
 *
 * \param data uint16_t 32位数(以无符号数形式存储)
 * \return int32_t  64位有符号数
 *
 */
int64_t hs_risc_v_common_sign_extend_uint32(uint32_t data);


typedef union
{
    uint8_t bytes[1];
    uint8_t value;
    int8_t  s_value;
}   hs_risc_v_common_memory_byte_t;         /**< 字节类型 */


typedef union
{
    uint8_t  bytes[2];
    uint16_t value;
    int16_t  s_value;
}   hs_risc_v_common_memory_halfword_t;     /**< 半字类型 */

typedef union
{
    uint8_t  bytes[4];
    uint32_t value;
    int32_t  s_value;
}   hs_risc_v_common_memory_word_t;         /**< 字类型 */

typedef union
{
    uint8_t  bytes[8];
    uint64_t value;
    int64_t  s_value;
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
    if(m_hs_byteorder.bytes[0]==0 && (sizeof(Data) > 1))        \
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

    //"F"扩展：单精度浮点，依赖"Zicsr"扩展
    HS_RISC_V_COMMON_INSTRUCTION_SET_RV32F= (1ULL << (7)),              /**< RV32F扩展指令集 */
    HS_RISC_V_COMMON_INSTRUCTION_SET_RV64F= (1ULL << (7)),              /**< RV64F扩展指令集 */

    //"D"扩展：双精度浮点，依赖"F"扩展
    HS_RISC_V_COMMON_INSTRUCTION_SET_RV32D= (1ULL << (8)),              /**< RV32D扩展指令集 */
    HS_RISC_V_COMMON_INSTRUCTION_SET_RV64D= (1ULL << (8)),              /**< RV64D扩展指令集 */


    //"Zicsr"扩展：控制状态寄存器
    HS_RISC_V_COMMON_INSTRUCTION_SET_RV32ZICSR= (1ULL << (26+1)),                 /**< RV32Zicsr扩展指令集 */
    HS_RISC_V_COMMON_INSTRUCTION_SET_RV64ZICSR= (1ULL << (26+1)),                 /**< RV64Zicsr扩展指令集 */

    //"Zifencei"扩展：指令获取屏障
    HS_RISC_V_COMMON_INSTRUCTION_SET_RV32ZIFENCEI= (1ULL << (26+2)),              /**< RV32Zicsr扩展指令集 */
    HS_RISC_V_COMMON_INSTRUCTION_SET_RV64ZIFENCEI= (1ULL << (26+2)),              /**< RV64Zicsr扩展指令集 */

} hs_risc_v_common_instruction_set_t;                                   /**< 指令集类型 */


/** \brief RISC-V 32位通用指令集RV32G（包括IMAFDZicsr_Zifencei）
 *
 *
 */
#define HS_RISC_V_COMMON_INSTRUCTION_SET_RV32G  (\
                                                 HS_RISC_V_COMMON_INSTRUCTION_SET_RV32I |\
                                                 HS_RISC_V_COMMON_INSTRUCTION_SET_RV32M |\
                                                 HS_RISC_V_COMMON_INSTRUCTION_SET_RV32A |\
                                                 HS_RISC_V_COMMON_INSTRUCTION_SET_RV32F |\
                                                 HS_RISC_V_COMMON_INSTRUCTION_SET_RV32D |\
                                                 HS_RISC_V_COMMON_INSTRUCTION_SET_RV32ZICSR |\
                                                 HS_RISC_V_COMMON_INSTRUCTION_SET_RV32ZIFENCEI )

/** \brief 指令集集合中是否含有32位通用指令集
 *
 * \param sets uint32_t 指令集集合
 * \return bool 是否有指令集
 *
 */
bool hs_risc_v_common_instruction_set_sets_has_rv32g(uint32_t sets);


/** \brief RISC-V 64位通用指令集RV64G（包括IMAFDZicsr_Zifencei）
 *
 *
 */
#define HS_RISC_V_COMMON_INSTRUCTION_SET_RV64G  (\
                                                 HS_RISC_V_COMMON_INSTRUCTION_SET_RV64I |\
                                                 HS_RISC_V_COMMON_INSTRUCTION_SET_RV64M |\
                                                 HS_RISC_V_COMMON_INSTRUCTION_SET_RV64A |\
                                                 HS_RISC_V_COMMON_INSTRUCTION_SET_RV64F |\
                                                 HS_RISC_V_COMMON_INSTRUCTION_SET_RV64D |\
                                                 HS_RISC_V_COMMON_INSTRUCTION_SET_RV64ZICSR |\
                                                 HS_RISC_V_COMMON_INSTRUCTION_SET_RV64ZIFENCEI )

/** \brief 指令集集合中是否含有64位通用指令集
 *
 * \param sets uint32_t 指令集集合
 * \return bool 是否有指令集
 *
 */
bool hs_risc_v_common_instruction_set_sets_has_rv64g(uint32_t sets);


/** \brief 指令集集合中是否有某个指令集
 *
 * \param sets uint32_t 指令集集合
 * \param instruction_set hs_risc_v_common_instruction_set_t  指令集类型
 * \return bool 是否有指令集
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

/** \brief 指令集集合添加指令集类型
 *
 * \param sets uint32_t 指令集集合
 * \param instruction_set hs_risc_v_common_instruction_set_t 指令集类型
 * \return uint32_t 添加后的指令集集合
 *
 */
uint32_t hs_risc_v_common_instruction_set_sets_set_set(uint32_t sets,hs_risc_v_common_instruction_set_t instruction_set);

/** \brief 指令集集合删除指令集类型。注意：只可以删除扩展指令且仅当没有指令依赖待删除的指令时可成功删除
 *
 * \param sets uint32_t 指令集集合
 * \param instruction_set hs_risc_v_common_instruction_set_t 指令集类型
 * \return uint32_t  清理后的指令集集合
 *
 */
uint32_t hs_risc_v_common_instruction_set_sets_clear_set(uint32_t sets,hs_risc_v_common_instruction_set_t instruction_set);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __HS_RISC_V_COMMON_H__
