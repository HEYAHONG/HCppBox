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

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __HS_RISC_V_COMMON_H__
