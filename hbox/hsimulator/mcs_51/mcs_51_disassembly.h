/***************************************************************
 * Name:      mcs_51_disassembly.h
 * Purpose:   声明mcs_51_disassembly接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-02-14
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HS_MCS_51_DISASSEMBLY_H__
#define __HS_MCS_51_DISASSEMBLY_H__
#include "stdint.h"
#include "stdlib.h"
#include "stdbool.h"
#ifdef __cplusplus
extern "C"
{
#endif

struct hs_mcs_51_disassembly_instruction;
typedef struct hs_mcs_51_disassembly_instruction hs_mcs_51_disassembly_instruction_t;

struct hs_mcs_51_disassembly_instruction
{
    uint8_t code;         /**<指令代码，通常为指令第1字节  */
    uint8_t mask;         /**<掩码，通常使用mask=0表示反汇编表结束  */
    int8_t  branch;       /**<分支类型 */
    uint8_t length;       /**<指令长度  */
    const char *mnemonic; /**<汇编助记符 */
    bool is_call;         /**<是否为调用 */
};

/** \brief 反汇编表，共257项，前256项可使用MCS-51指令第一字节引索
 *
 *
 */
extern const hs_mcs_51_disassembly_instruction_t hs_mcs_51_disassembly_instruction_table[];

/** \brief 从反汇编表中读取指令
 *
 * \param instruction const uint8_t* 指令数组指针
 * \return const hs_mcs_51_disassembly_instruction_t* 已读取的指令
 *
 */
const hs_mcs_51_disassembly_instruction_t *hs_mcs_51_disassembly_instruction_table_get(const uint8_t *instruction);

/** \brief 从反汇编表中读取指令长度
 *
 * \param instruction uint8_t* 指令数组指针
 * \return size_t 指令长度，失败返回0
 *
 */
size_t  hs_mcs_51_disassembly_instruction_length(const uint8_t *instruction);


/** \brief 获取程序代码中的指令条数
 *
 * \param code uint8_t* 程序代码指针
 * \param length size_t 程序代码长度
 * \return size_t   程序代码中指令的条数
 *
 */
size_t  hs_mcs_51_disassembly_code_instruction_count(const uint8_t *code,size_t length);


/** \brief 获取程序代码中的指令类型数量
 *
 * \param code uint8_t* 程序代码指针
 * \param length size_t 程序代码长度
 * \return size_t   程序代码中指令类型数量
 *
 */
size_t  hs_mcs_51_disassembly_code_instruction_type_count(const uint8_t *code,size_t length);



/** \brief 打印指令信息
 *
 * \param instruction const uint8_t* 指令数据指针
 * \param length size_t 指令数据长度
 * \param char_output void (*)(const char ch,void *usr) 字符输出函数
 * \param usr void*  用户参数，用于字符输出
 *
 */
void hs_mcs_51_disassembly_print(const uint8_t *instruction,size_t length,void (*char_output)(const char ch,void *usr),void *usr);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __HS_MCS_51_DISASSEMBLY_H__
