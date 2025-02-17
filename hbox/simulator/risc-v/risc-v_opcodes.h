/***************************************************************
 * Name:      risc-v_opcodes.h
 * Purpose:   声明risc-v_opcodes接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-02-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HS_RISC_V_OPCODES_H__
#define __HS_RISC_V_OPCODES_H__
#include "stdint.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

struct hs_risc_v_opcodes_insn;
typedef struct hs_risc_v_opcodes_insn hs_risc_v_opcodes_insn_t;

struct  hs_risc_v_opcodes_insn
{
    const char *name;   /**< 指令名称 */
    uint32_t    match;  /**< 指令匹配值 */
    uint32_t    mask;   /**< 指令匹配掩码 */
};


/** \brief 声明要使用的INSN
 *
 *
 */
#ifdef __cplusplus
#define HS_RISC_V_OPCODES_INSN_DECLARE(NAME) \
extern "C" const hs_risc_v_opcodes_insn_t hs_risc_v_opcodes_insn_##NAME
#else
#define HS_RISC_V_OPCODES_INSN_DECLARE(NAME) \
extern const hs_risc_v_opcodes_insn_t hs_risc_v_opcodes_insn_##NAME
#endif


/** \brief 获取INSN的名称
 *
 *
 */
#define HS_RISC_V_OPCODES_INSN_NAME(NAME) (hs_risc_v_opcodes_insn_##NAME)

struct hs_risc_v_opcodes_csr;
typedef struct hs_risc_v_opcodes_csr hs_risc_v_opcodes_csr_t;

struct  hs_risc_v_opcodes_csr
{
    const char *name;   /**< CSR名称 */
    uint16_t    addr;  /**< CSR地址 */
};

/** \brief 声明要使用的CSR
 *
 *
 */
#ifdef __cplusplus
#define HS_RISC_V_OPCODES_CSR_DECLARE(NAME) \
extern "C" const hs_risc_v_opcodes_csr_t hs_risc_v_opcodes_csr_##NAME
#else
#define HS_RISC_V_OPCODES_CSR_DECLARE(NAME) \
extern const hs_risc_v_opcodes_csr_t hs_risc_v_opcodes_csr_##NAME
#endif

/** \brief 获取CSR的名称
 *
 *
 */
#define HS_RISC_V_OPCODES_CSR_NAME(NAME) (hs_risc_v_opcodes_csr_##NAME)

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __HS_RISC_V_OPCODES_H__

