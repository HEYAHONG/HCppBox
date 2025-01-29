/***************************************************************
 * Name:      risc-v.h
 * Purpose:   声明risc-v接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-01-29
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HS_RISC_V_H__
#define __HS_RISC_V_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/*
 *  由riscv-opcodes（https://github.com/riscv/riscv-opcodes.git）生成的C语言头文件。
 *  如需使用，需要在包含头文件前定义HS_RISC_V_BUILD_C_SOURCE
 */
#ifdef HS_RISC_V_BUILD_C_SOURCE
#include "encoding.out.h"
#endif

#include "risc-v_common.h"

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __HS_RISC_V_H__
