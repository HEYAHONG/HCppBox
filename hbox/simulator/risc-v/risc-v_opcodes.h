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


/*
 *  RV32I
 */
HS_RISC_V_OPCODES_INSN_DECLARE(lui);
HS_RISC_V_OPCODES_INSN_DECLARE(auipc);
HS_RISC_V_OPCODES_INSN_DECLARE(jal);
HS_RISC_V_OPCODES_INSN_DECLARE(jalr);
HS_RISC_V_OPCODES_INSN_DECLARE(beq);
HS_RISC_V_OPCODES_INSN_DECLARE(bne);
HS_RISC_V_OPCODES_INSN_DECLARE(blt);
HS_RISC_V_OPCODES_INSN_DECLARE(bge);
HS_RISC_V_OPCODES_INSN_DECLARE(bltu);
HS_RISC_V_OPCODES_INSN_DECLARE(bgeu);
HS_RISC_V_OPCODES_INSN_DECLARE(lb);
HS_RISC_V_OPCODES_INSN_DECLARE(lh);
HS_RISC_V_OPCODES_INSN_DECLARE(lw);
HS_RISC_V_OPCODES_INSN_DECLARE(lbu);
HS_RISC_V_OPCODES_INSN_DECLARE(lhu);
HS_RISC_V_OPCODES_INSN_DECLARE(sb);
HS_RISC_V_OPCODES_INSN_DECLARE(sh);
HS_RISC_V_OPCODES_INSN_DECLARE(sw);
HS_RISC_V_OPCODES_INSN_DECLARE(addi);
HS_RISC_V_OPCODES_INSN_DECLARE(slti);
HS_RISC_V_OPCODES_INSN_DECLARE(xori);
HS_RISC_V_OPCODES_INSN_DECLARE(ori);
HS_RISC_V_OPCODES_INSN_DECLARE(andi);
HS_RISC_V_OPCODES_INSN_DECLARE(add);
HS_RISC_V_OPCODES_INSN_DECLARE(sub);
HS_RISC_V_OPCODES_INSN_DECLARE(sll);
HS_RISC_V_OPCODES_INSN_DECLARE(slt);
HS_RISC_V_OPCODES_INSN_DECLARE(sltu);
HS_RISC_V_OPCODES_INSN_DECLARE(xor);
HS_RISC_V_OPCODES_INSN_DECLARE(srl);
HS_RISC_V_OPCODES_INSN_DECLARE(sra);
HS_RISC_V_OPCODES_INSN_DECLARE(or);
HS_RISC_V_OPCODES_INSN_DECLARE(and);
HS_RISC_V_OPCODES_INSN_DECLARE(fence);
HS_RISC_V_OPCODES_INSN_DECLARE(ecall);
HS_RISC_V_OPCODES_INSN_DECLARE(ebreak);
HS_RISC_V_OPCODES_INSN_DECLARE(slli);
HS_RISC_V_OPCODES_INSN_DECLARE(srli);
HS_RISC_V_OPCODES_INSN_DECLARE(srai);
HS_RISC_V_OPCODES_INSN_DECLARE(pause);

/*
 *  RV64I（较RV32I新增）
 */
HS_RISC_V_OPCODES_INSN_DECLARE(lwu);
HS_RISC_V_OPCODES_INSN_DECLARE(ld);
HS_RISC_V_OPCODES_INSN_DECLARE(sd);
HS_RISC_V_OPCODES_INSN_DECLARE(slli);
HS_RISC_V_OPCODES_INSN_DECLARE(srli);
HS_RISC_V_OPCODES_INSN_DECLARE(srai);
HS_RISC_V_OPCODES_INSN_DECLARE(addw);
HS_RISC_V_OPCODES_INSN_DECLARE(subw);
HS_RISC_V_OPCODES_INSN_DECLARE(sllw);
HS_RISC_V_OPCODES_INSN_DECLARE(srlw);
HS_RISC_V_OPCODES_INSN_DECLARE(sraw);

/*
 *  RV32/RV64 Zifencei
 */
HS_RISC_V_OPCODES_INSN_DECLARE(fence_i);


/*
 *  RV32/RV64 Zicsr
 */
HS_RISC_V_OPCODES_INSN_DECLARE(csrrw);
HS_RISC_V_OPCODES_INSN_DECLARE(csrrs);
HS_RISC_V_OPCODES_INSN_DECLARE(csrrc);
HS_RISC_V_OPCODES_INSN_DECLARE(csrrwi);
HS_RISC_V_OPCODES_INSN_DECLARE(csrrsi);
HS_RISC_V_OPCODES_INSN_DECLARE(csrrci);

/*
 *  RV32M
 */
HS_RISC_V_OPCODES_INSN_DECLARE(mul);
HS_RISC_V_OPCODES_INSN_DECLARE(mulh);
HS_RISC_V_OPCODES_INSN_DECLARE(mulhsu);
HS_RISC_V_OPCODES_INSN_DECLARE(mulhu);
HS_RISC_V_OPCODES_INSN_DECLARE(div);
HS_RISC_V_OPCODES_INSN_DECLARE(divu);
HS_RISC_V_OPCODES_INSN_DECLARE(rem);
HS_RISC_V_OPCODES_INSN_DECLARE(remu);

/*
 *  RV64M (较RV32M新增)
 */
HS_RISC_V_OPCODES_INSN_DECLARE(mulw);
HS_RISC_V_OPCODES_INSN_DECLARE(divw);
HS_RISC_V_OPCODES_INSN_DECLARE(divuw);
HS_RISC_V_OPCODES_INSN_DECLARE(remw);
HS_RISC_V_OPCODES_INSN_DECLARE(remuw);

/*
 *  RV32A
 */
HS_RISC_V_OPCODES_INSN_DECLARE(lr_w);
HS_RISC_V_OPCODES_INSN_DECLARE(sc_w);
HS_RISC_V_OPCODES_INSN_DECLARE(amoswap_w);
HS_RISC_V_OPCODES_INSN_DECLARE(amoadd_w);
HS_RISC_V_OPCODES_INSN_DECLARE(amoxor_w);
HS_RISC_V_OPCODES_INSN_DECLARE(amoand_w);
HS_RISC_V_OPCODES_INSN_DECLARE(amoor_w);
HS_RISC_V_OPCODES_INSN_DECLARE(amomin_w);
HS_RISC_V_OPCODES_INSN_DECLARE(amomax_w);
HS_RISC_V_OPCODES_INSN_DECLARE(amominu_w);
HS_RISC_V_OPCODES_INSN_DECLARE(amomaxu_w);


/*
 * RV64A (较RV32A新增)
 */
HS_RISC_V_OPCODES_INSN_DECLARE(lr_d);
HS_RISC_V_OPCODES_INSN_DECLARE(sc_d);
HS_RISC_V_OPCODES_INSN_DECLARE(amoswap_d);
HS_RISC_V_OPCODES_INSN_DECLARE(amoadd_d);
HS_RISC_V_OPCODES_INSN_DECLARE(amoxor_d);
HS_RISC_V_OPCODES_INSN_DECLARE(amoand_d);
HS_RISC_V_OPCODES_INSN_DECLARE(amoor_d);
HS_RISC_V_OPCODES_INSN_DECLARE(amomin_d);
HS_RISC_V_OPCODES_INSN_DECLARE(amomax_d);
HS_RISC_V_OPCODES_INSN_DECLARE(amominu_d);
HS_RISC_V_OPCODES_INSN_DECLARE(amomaxu_d);






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

