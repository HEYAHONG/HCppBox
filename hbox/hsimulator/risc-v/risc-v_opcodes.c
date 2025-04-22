/***************************************************************
 * Name:      risc-v_opcodes.h
 * Purpose:   实现risc-v opcodes接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-02-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "risc-v_opcodes.h"

/*
 * 用于声明INSN
 */
#ifndef DECLARE_INSN
#define DECLARE_INSN(NAME,MATCH,MASK)   \
extern const hs_risc_v_opcodes_insn_t hs_risc_v_opcodes_insn_##NAME;\
const hs_risc_v_opcodes_insn_t hs_risc_v_opcodes_insn_##NAME={#NAME,MATCH,MASK};
#endif // DECLARE_INSN


/*
 * 用于声明CSR
 */
#ifndef DECLARE_CSR
#define DECLARE_CSR(NAME,ADDR)   \
extern const hs_risc_v_opcodes_csr_t hs_risc_v_opcodes_csr_##NAME;\
const hs_risc_v_opcodes_csr_t hs_risc_v_opcodes_csr_##NAME={#NAME,ADDR};
#endif // DECLARE_CSR

/*
 * 用于声明异常CAUSE
 */
#ifndef DECLARE_CAUSE
#define DECLARE_CAUSE(NAME,VALUE)   \
HS_RISC_V_OPCODES_EXCEPTION_DECLARE(VALUE);\
const int HS_RISC_V_OPCODES_EXCEPTION_##VALUE=VALUE;
#endif // DECLARE_CAUSE

/*
 *  由riscv-opcodes（https://github.com/riscv/riscv-opcodes.git）生成的C语言头文件。
 *
 */
#include "encoding.out.h"
