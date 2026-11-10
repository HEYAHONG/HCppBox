/*
rvjit_la64.h - RVJIT loongarch64 Backend
Copyright (C) 2026 Yao Zi <me@ziyao.cc>

This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include "bit_ops.h"
#include "mem_ops.h"
#include "rvjit.h"
#include "utils.h"

#ifndef RVJIT_LA64_H
#define RVJIT_LA64_H

/*
 * Calling convention
 *
 * - $r0		constant zero ($zero)
 * - $r1		return address ($ra)
 * - $r2		thread pointer ($tp)
 * - $r3		stack pointer ($sp)
 * - $r4 - $r11		argument registers ($a0 - $a7), values are returned in
 *			$r4 and $r5.
 * - $12 - $r20		temporary registers ($t0 - $t8)
 * - $r21		reserved, non-allocatable
 * - $r22 - $r31	static registers, ($s0 - $s9), $r31/$s9 may also be
 *			used as framepointer, in which case it's called $fp.
 */

/* ABI definitions */
#define REG_ZERO   0
#define REG_RA     1
#define REG_TP     2
#define REG_SP     3
#define REG_Ax(n)  (4 + (n))
#define REG_Tx(n)  (12 + (n))
#define REG_Sx(n)  (22 + (n))

#define VM_PTR_REG REG_Ax(0)

static inline size_t rvjit_native_default_hregmask(void)
{
    return (1 << REG_Tx(0)) | (1 << REG_Tx(1)) | (1 << REG_Tx(2)) | (1 << REG_Tx(3)) | (1 << REG_Tx(4))
         | (1 << REG_Tx(5)) | (1 << REG_Tx(6)) | (1 << REG_Tx(7)) | (1 << REG_Tx(8)) | (1 << REG_Ax(1))
         | (1 << REG_Ax(2)) | (1 << REG_Ax(3)) | (1 << REG_Ax(4)) | (1 << REG_Ax(5)) | (1 << REG_Ax(6))
         | (1 << REG_Ax(7));
}

static inline size_t rvjit_native_abireclaim_hregmask(void)
{
    return (1 << REG_Sx(0)) | (1 << REG_Sx(1)) | (1 << REG_Sx(2)) | (1 << REG_Sx(3)) | (1 << REG_Sx(4))
         | (1 << REG_Sx(5)) | (1 << REG_Sx(6)) | (1 << REG_Sx(7)) | (1 << REG_Sx(8)) | (1 << REG_Sx(9));
}

/* Some convenient opcode */
#define OP_lu12i_w    0x14000000
#define OP_lu32i_d    0x16000000
#define OP_lu52i_d    0x03000000
#define OP_ori        0x03800000
#define OP_addi_d     0x02c00000
#define OP_srai_d     0x00490000
#define OP_add_w      0x00100000
#define OP_add_d      0x00108000
#define OP_mul_d      0x001d8000
#define OP_mulh_du    0x001e8000
#define OP_or         0x00150000
#define OP_nor        0x00140000
#define OP_jirl       0x4c000000
#define OP_b          0x50000000
#define OP_beq        0x58000000
#define OP_beqz       0x40000000
#define OP_bne        0x5c000000
#define OP_st_d       0x29c00000
#define OP_ld_d       0x28c00000
#define OP_bstrpick_d 0x00c00000

/* Helpers for encoding instrunctions */
static inline void rvjit_la_emit_djk(rvjit_block_t* block, uint32_t opcode, regid_t rd, regid_t rj, regid_t rk)
{
    uint32_t code = opcode | (rd << 0) | (rj << 5) | (rk << 10);
    rvjit_put_code(block, &code, sizeof(code));
}

static inline void rvjit_la_emit_djk12(rvjit_block_t* block, uint32_t opcode, regid_t rd, regid_t rj, uint32_t k12)
{
    uint32_t code  = opcode | (rd << 0) | (rj << 5);
    code          |= (k12 & bit_mask(12)) << 10;
    rvjit_put_code(block, &code, sizeof(code));
}

static inline void rvjit_la_emit_djk16(rvjit_block_t* block, uint32_t opcode, regid_t rd, regid_t rj, uint32_t k16)
{
    uint32_t code  = opcode | (rd << 0) | (rj << 5);
    code          |= (k16 & bit_mask(16)) << 10;
    rvjit_put_code(block, &code, sizeof(code));
}

static inline void rvjit_la_emit_dj20(rvjit_block_t* block, uint32_t opcode, regid_t rd, uint32_t j20)
{
    uint32_t code  = opcode | (rd << 0);
    code          |= (j20 & bit_mask(20)) << 5;
    rvjit_put_code(block, &code, sizeof(code));
}

static inline void rvjit_la_emit_djk5(rvjit_block_t* block, uint32_t opcode, regid_t rd, uint32_t rj, uint8_t k5)
{
    uint32_t code  = opcode | (rd << 0) | (rj << 5);
    code          |= (k5 & bit_mask(5)) << 10;
    rvjit_put_code(block, &code, sizeof(code));
}

static inline void rvjit_la_emit_djk6(rvjit_block_t* block, uint32_t opcode, regid_t rd, uint32_t rj, uint8_t k6)
{
    uint32_t code  = opcode | (rd << 0) | (rj << 5);
    code          |= (k6 & bit_mask(6)) << 10;
    rvjit_put_code(block, &code, sizeof(code));
}

static inline void rvjit_la_emit_d15(rvjit_block_t* block, uint32_t opcode, int32_t d15)
{
    uint32_t code = opcode | (d15 << 0);
    rvjit_put_code(block, &code, sizeof(code));
}

static inline void rvjit_la_emit_djm6k6(rvjit_block_t* block, uint32_t opcode, regid_t rd, regid_t rj, uint8_t m6,
                                        uint8_t k6)
{
    uint32_t code  = opcode | (rd << 0) | (rj << 5);
    code          |= (k6 << 10) | (m6 << 16);
    rvjit_put_code(block, &code, sizeof(code));
}

static inline void rvjit_la_emit_d10k16(rvjit_block_t* block, uint32_t opcode, uint32_t d10k16)
{
    uint32_t code  = opcode;
    code          |= (d10k16 >> 16) & bit_mask(10);
    code          |= (d10k16 & bit_mask(16)) << 10;
    rvjit_put_code(block, &code, sizeof(code));
}

static inline void rvjit_la_emit_jd5k16(rvjit_block_t* block, uint32_t opcode, regid_t rj, uint32_t d5k16)
{
    uint32_t code  = opcode | (rj << 5);
    code          |= (d5k16 >> 16) & bit_mask(5);
    code          |= (d5k16 & bit_mask(16)) << 10;
    rvjit_put_code(block, &code, sizeof(code));
}

/* Helpers for encoding instruction sequences */
static inline void rvjit_la_load_imm64(rvjit_block_t* block, regid_t hreg, uint64_t imm)
{
    /*
     * TODO: Optimize the loading sequence
     *	lu12i.w	hreg,	imm[31:12]
     *	ori	hreg,	hreg,	imm[11:0]
     *	lu32i.d	hreg,	imm[51:32]
     *	lu52i.d hreg,	imm[63:52]
     */
    rvjit_la_emit_dj20(block, OP_lu12i_w, hreg, imm >> 12);
    rvjit_la_emit_djk12(block, OP_ori, hreg, hreg, imm);
    rvjit_la_emit_dj20(block, OP_lu32i_d, hreg, imm >> 32);
    rvjit_la_emit_djk12(block, OP_lu52i_d, hreg, hreg, imm >> 52);
}

static int rvjit_la_simm_in_range(uint64_t imm, int bits)
{
    return ((imm + (1 << (bits - 1))) & ~bit_mask(bits)) == 0;
}

static inline void rvjit_la_sext_imm(rvjit_block_t* block, uint32_t opcode_imm, uint32_t opcode_reg, regid_t rd,
                                     regid_t rj, int64_t imm)
{
    if (rvjit_la_simm_in_range(imm, 12)) {
        rvjit_la_emit_djk12(block, opcode_imm, rd, rj, imm);
    } else {
        regid_t rimm = rvjit_claim_hreg(block);

        rvjit_la_load_imm64(block, rimm, imm);
        rvjit_la_emit_djk(block, opcode_reg, rd, rj, rimm);

        rvjit_free_hreg(block, rimm);
    }
}

static inline void rvjit_la_zext_imm(rvjit_block_t* block, uint32_t opcode_imm, uint32_t opcode_reg, regid_t rd,
                                     regid_t rj, int64_t imm)
{
    uint64_t immbits = imm;

    if (immbits >> 12) {
        regid_t rimm = rvjit_claim_hreg(block);

        rvjit_la_load_imm64(block, rimm, imm);
        rvjit_la_emit_djk(block, opcode_reg, rd, rj, rimm);

        rvjit_free_hreg(block, rimm);
    } else {
        rvjit_la_emit_djk12(block, opcode_imm, rd, rj, imm);
    }
}

static inline void rvjit_la_patch_rel_jump(rvjit_block_t* block, branch_t handle, int64_t offset)
{
    uint64_t offbits = offset;
    uint32_t code;

    if (unlikely(!rvjit_la_simm_in_range(offset, 28))) {
        rvvm_fatal("Relative jump offset 0x%lx truncates to fit", offset);
    }

    offbits >>= 2;

    code  = read_uint32_le_m(&block->code[handle]);
    code &= ~bit_mask(26);
    code |= (offbits >> 16) & bit_mask(10);
    code |= (offbits & bit_mask(16)) << 10;
    write_uint32_le_m(&block->code[handle], code);
}

static inline void rvjit_la_patch_branch_reg(rvjit_block_t* block, branch_t handle, int64_t offset)
{
    rvjit_la_patch_rel_jump(block, handle + 4, offset - 4);
}

static inline branch_t rvjit_la_branch_reg(rvjit_block_t* block, uint32_t opcode, uint32_t opcode_inverted, regid_t rs1,
                                           regid_t rs2, branch_t handle, bool target)
{
    UNUSED(opcode);

    /* TODO: Generate a single conditional branch if reachable */
    if (target == BRANCH_ENTRY) {
        branch_t tmp = block->size;

        /*
         * Emit a branch, the target/offset is only available when
         * branching backwards (handle != BRANCH_NEW), we only relocate
         * it immediately in such case.
         *
         *	binverted $rs1, $rs2, 1f
         *	b target (might not available yet)
         * 1:
         */
        rvjit_la_emit_djk16(block, opcode_inverted, rs1, rs2, 2);
        rvjit_la_emit_d10k16(block, OP_b, 0x0);

        if (handle == BRANCH_NEW) {
            return tmp;
        } else {
            rvjit_la_patch_branch_reg(block, tmp, handle - tmp);
            return BRANCH_NEW;
        }
    } else {
        /*
         * Relocate a forward branch (handle != BRANCH_NEW). Backward
         * branches are relocated immediately after emitting.
         */
        if (handle == BRANCH_NEW) {
            return block->size;
        } else {
            rvjit_la_patch_branch_reg(block, handle, block->size - handle);
            return BRANCH_NEW;
        }
    }
}

static inline void rvjit_la_div_rem(rvjit_block_t* block, int bitwidth, uint32_t opcode, regid_t rd, regid_t rs1,
                                    regid_t rs2, bool rem, bool is_signed)
{
    /*
     * overflow_check:
     *	lu52i.d $treg, $zero, 0x800
     *	beq $treg, $rs1, overflow_check_divisor
     *
     * div0_check:
     *	beqz $rs2, div0
     *
     *	# Result of 32-bit division is UNPREDICTABLE if either dividend
     *	# or divisor is not sign-extended to 64-bit.
     *	add.w $rs1, $rs1, $zero
     *	add.w $rs2, $rs2, $zero
     *
     *	opcode $rd, $rs1, $rs2
     *	b out
     *
     * overflow_check_divisor:
     *	nor $treg, $treg, $zero
     *	bne $treg, $rs2, div0_check
     *	if is_rem
     *		ori $rd, $zero, 0
     *	else
     *		ori $rd, $rs1, 0
     *	b out
     *
     * div0:
     *	if is_rem
     *		ori $rd, $rs1, 0
     *	else
     *		nor $rd, $zero, $zero
     * out:
     */

    regid_t treg;

    if (is_signed) {
        treg = rvjit_claim_hreg(block);

        rvjit_la_emit_djk12(block, OP_lu52i_d, treg, REG_ZERO, 0x800);
        rvjit_la_emit_djk16(block, OP_beq, treg, rs1, 4 + (bitwidth == 32) * 2);
    }

    rvjit_la_emit_jd5k16(block, OP_beqz, rs2, (is_signed ? 7 : 3) + (bitwidth == 32) * 2);

    if (bitwidth == 32) {
        rvjit_la_emit_djk(block, OP_add_w, rs1, rs1, REG_ZERO);
        rvjit_la_emit_djk(block, OP_add_w, rs2, rs2, REG_ZERO);
    }

    rvjit_la_emit_djk(block, opcode, rd, rs1, rs2);
    rvjit_la_emit_d10k16(block, OP_b, is_signed ? 6 : 2);

    if (is_signed) {
        rvjit_la_emit_djk(block, OP_nor, treg, treg, REG_ZERO);
        rvjit_la_emit_djk16(block, OP_bne, treg, rs2, (int64_t)(-4 - (bitwidth == 32) * 2));
        rvjit_la_emit_djk12(block, OP_ori, rd, rem ? REG_ZERO : rs1, 0);
        rvjit_la_emit_d10k16(block, OP_b, 2);
    }

    if (rem) {
        rvjit_la_emit_djk12(block, OP_ori, rd, rs1, 0);
    } else {
        rvjit_la_emit_djk(block, OP_nor, rd, REG_ZERO, REG_ZERO);
    }

    if (is_signed) {
        rvjit_free_hreg(block, treg);
    }
}

/* Translation generators */
#define RVJIT_IMM_UTYPE_32  uint32_t
#define RVJIT_IMM_UTYPE_64  uint64_t
#define RVJIT_IMM_UTYPE(bw) RVJIT_IMM_UTYPE_##bw

#define RVJIT_IMM_STYPE_32  int32_t
#define RVJIT_IMM_STYPE_64  int64_t
#define RVJIT_IMM_STYPE(bw) RVJIT_IMM_STYPE_##bw

#define rvjit_la_trans_djk_operands(bw, op, opcode, op1, op2, op3)                                                     \
    static inline void rvjit##bw##_native_##op(rvjit_block_t* block, regid_t hrds, regid_t hrs1, regid_t hrs2)         \
    {                                                                                                                  \
        rvjit_la_emit_djk(block, opcode, op1, op2, op3);                                                               \
    }

#define rvjit_la_trans_djk(bw, op, opcode) rvjit_la_trans_djk_operands(bw, op, opcode, hrds, hrs1, hrs2)

#define rvjit_la_trans_djsk12(bw, op, opcode)                                                                          \
    static inline void rvjit##bw##_native_##op(rvjit_block_t* block, regid_t hrds, regid_t hrs1,                       \
                                               RVJIT_IMM_STYPE(bw) imm)                                                \
    {                                                                                                                  \
        rvjit_la_emit_djk12(block, opcode, hrds, hrs1, imm);                                                           \
    }

#define rvjit_la_trans_sext_imm(bw, op, opcode_imm, opcode_reg)                                                        \
    static inline void rvjit##bw##_native_##op(rvjit_block_t* block, regid_t hrds, regid_t hrs1,                       \
                                               RVJIT_IMM_STYPE(bw) imm)                                                \
    {                                                                                                                  \
        rvjit_la_sext_imm(block, opcode_imm, opcode_reg, hrds, hrs1, imm);                                             \
    }

#define rvjit_la_trans_zext_imm(bw, op, opcode_imm, opcode_reg)                                                        \
    static inline void rvjit##bw##_native_##op(rvjit_block_t* block, regid_t hrds, regid_t hrs1,                       \
                                               RVJIT_IMM_STYPE(bw) imm)                                                \
    {                                                                                                                  \
        rvjit_la_zext_imm(block, opcode_imm, opcode_reg, hrds, hrs1, imm);                                             \
    }

#define rvjit_la_trans_djk5(bw, op, opcode)                                                                            \
    static inline void rvjit##bw##_native_##op(rvjit_block_t* block, regid_t hrds, regid_t hrs1, uint8_t imm)          \
    {                                                                                                                  \
        rvjit_la_emit_djk5(block, opcode, hrds, hrs1, imm);                                                            \
    }

/* Conditional branches in LoongArch have opposite operand order */
#define rvjit_la_trans_branch_reg(bw, op, opcode, opcode_inverted)                                                     \
    static inline branch_t rvjit##bw##_native_##op(rvjit_block_t* block, regid_t rs1, regid_t rs2, branch_t handle,    \
                                                   bool target)                                                        \
    {                                                                                                                  \
        return rvjit_la_branch_reg(block, opcode, opcode_inverted, rs2, rs1, handle, target);                          \
    }

#define rvjit_la_trans_djk6(bw, op, opcode)                                                                            \
    static inline void rvjit##bw##_native_##op(rvjit_block_t* block, regid_t hrds, regid_t hrs1, uint8_t imm)          \
    {                                                                                                                  \
        rvjit_la_emit_djk6(block, opcode, hrds, hrs1, imm);                                                            \
    }

#define rvjit_la_trans_div_rem(bw, op, opcode, rem, signed)                                                            \
    static inline void rvjit##bw##_native_##op(rvjit_block_t* block, regid_t hrds, regid_t hrs1, regid_t hrs2)         \
    {                                                                                                                  \
        rvjit_la_div_rem(block, bw, opcode, hrds, hrs1, hrs2, rem, signed);                                            \
    }

static inline void rvjit_native_push(rvjit_block_t* block, regid_t reg)
{
    /*
     *	addi.d	$sp, $sp, -8
     *	st.d	$reg, $sp, 0
     */
    rvjit_la_emit_djk12(block, OP_addi_d, REG_SP, REG_SP, -8);
    rvjit_la_emit_djk12(block, OP_st_d, reg, REG_SP, 0);
}

static inline void rvjit_native_pop(rvjit_block_t* block, regid_t reg)
{
    /*
     *	ld.d $reg, $sp, 0
     *	addi.d $sp, $sp, 8
     */
    rvjit_la_emit_djk12(block, OP_ld_d, reg, REG_SP, 0);
    rvjit_la_emit_djk12(block, OP_addi_d, REG_SP, REG_SP, 8);
}

static inline void rvjit_native_ret(rvjit_block_t* block)
{
    // jirl $zero, $ra, 0x0
    rvjit_la_emit_djk16(block, OP_jirl, REG_ZERO, REG_RA, 0x0);
}

static inline void rvjit_native_setreg32(rvjit_block_t* block, regid_t reg, uint32_t imm)
{
    rvjit_la_load_imm64(block, reg, (int64_t)(int32_t)imm);
}

static inline void rvjit_native_setreg32s(rvjit_block_t* block, regid_t reg, int32_t imm)
{
    rvjit_la_load_imm64(block, reg, (int64_t)imm);
}

static inline void rvjit_native_zero_reg(rvjit_block_t* block, regid_t reg)
{
    // or $reg, $zero, $zero
    rvjit_la_emit_djk(block, OP_or, reg, REG_ZERO, REG_ZERO);
}

// clang-format off
rvjit_la_trans_djk(32, add, 0x00100000)			// add.w
rvjit_la_trans_djk(32, sub, 0x00110000)			// sub.w
rvjit_la_trans_djk(32, or,  0x00150000)			// or
rvjit_la_trans_djk(32, and, 0x00148000)			// and
rvjit_la_trans_djk(32, xor, 0x00158000)			// xor
rvjit_la_trans_djk(32, sra, 0x00180000)			// sra.w
rvjit_la_trans_djk(32, srl, 0x00178000)			// srl.w
rvjit_la_trans_djk(32, sll, 0x00170000)			// sll.w
rvjit_la_trans_sext_imm(32, addi, 0x02800000,		// addi.w
				  0x00100000)		// add.w
rvjit_la_trans_zext_imm(32, ori,  0x03800000,		// ori
				  0x00150000);		// or
rvjit_la_trans_zext_imm(32, andi, 0x03400000,		// andi, or
				  0x00148000)		// and
rvjit_la_trans_zext_imm(32, xori, 0x03c00000,		// xori, or
				  0x00158000)		// xor
rvjit_la_trans_djk5(32, srai, 0x00488000)		// srai.w
rvjit_la_trans_djk5(32, srli, 0x00448000)		// srli.w
rvjit_la_trans_djk5(32, slli, 0x00408000)		// slli.w
rvjit_la_trans_sext_imm(32, slti, 0x02000000,		// slti
				  0x00120000)		// slt
rvjit_la_trans_sext_imm(32, sltiu, 0x02400000,		// sltui
				   0x00128000);		// sltu
rvjit_la_trans_djk(32, slt, 0x00120000)			// slt
rvjit_la_trans_djk(32, sltu, 0x00128000)		// sltu
rvjit_la_trans_sext_imm(32, lb,  0x28000000,		// ld.b
				 0x38000000)		// ldx.b
rvjit_la_trans_sext_imm(32, lbu, 0x2a000000,		// ld.bu
				 0x38200000)		// ldx.bu
rvjit_la_trans_sext_imm(32, lh,  0x28400000,		// ld.h
				 0x38040000)		// ldx.h
rvjit_la_trans_sext_imm(32, lhu, 0x2a400000,		// ld.hu
				 0x38240000)		// ldx.hu
rvjit_la_trans_sext_imm(32, lw,  0x28800000,		// ld.w
				 0x38080000)		// ldx.w
rvjit_la_trans_sext_imm(32, sb,  0x29000000,		// st.b
				 0x38100000)		// stx.b
rvjit_la_trans_sext_imm(32, sh,  0x29400000,		// st.h
				 0x38140000)		// stx.h
rvjit_la_trans_sext_imm(32, sw,  0x29800000,		// st.w
				 0x38180000)		// stx.w
rvjit_la_trans_branch_reg(32, beq, 0x58000000,		// beq
				   0x5c000000);		// bne
rvjit_la_trans_branch_reg(32, bne, 0x5c000000,		// bne
				   0x58000000);		// beq
// clang-format on

/*
 * TODO: translate beqz/bnez to specialized version for larger addressing range
 */
static inline branch_t rvjit32_native_beqz(rvjit_block_t* block, regid_t rs1, branch_t handle, bool target)
{
    // beq/bne
    return rvjit_la_branch_reg(block, OP_beq, OP_bne, rs1, REG_ZERO, handle, target);
}

static inline branch_t rvjit32_native_bnez(rvjit_block_t* block, regid_t rs1, branch_t handle, bool target)
{
    // bne/beq
    return rvjit_la_branch_reg(block, OP_bne, OP_beq, rs1, REG_ZERO, handle, target);
}

// clang-format off
rvjit_la_trans_branch_reg(32, blt,  0x60000000,		// blt
				    0x64000000);	// bge
rvjit_la_trans_branch_reg(32, bge,  0x64000000,		// bge
				    0x60000000);	// blt
rvjit_la_trans_branch_reg(32, bltu, 0x68000000,		// bltu
				    0x6c000000);	// bgeu
rvjit_la_trans_branch_reg(32, bgeu, 0x6c000000,		// bgeu
				    0x68000000);	// bltu
rvjit_la_trans_djk(32, mul,   0x001c0000);		// mul.w
rvjit_la_trans_djk(32, mulh,  0x001c8000);		// mulh.w
rvjit_la_trans_djk(32, mulhu, 0x001d0000);		// mulh.wu
// clang-format on

static inline void rvjit32_native_mulhsu(rvjit_block_t* block, regid_t rd, regid_t rs1, regid_t rs2)
{
    regid_t treg = rvjit_claim_hreg(block);

    /*
     *	bstrpick.d $treg, $rs2, 31, 0
     *	mul.d $treg, $rs1, $treg
     *	srai.d $rd, $treg, 32
     */
    rvjit_la_emit_djm6k6(block, OP_bstrpick_d, treg, rs2, 31, 0);
    rvjit_la_emit_djk(block, OP_mul_d, treg, rs1, treg);
    rvjit_la_emit_djk6(block, OP_srai_d, rd, treg, 32);

    rvjit_free_hreg(block, treg);
}

// clang-format off
rvjit_la_trans_div_rem(32, div,  0x00200000, false, true);	// div.w
rvjit_la_trans_div_rem(32, divu, 0x00210000, false, false);	// div.wu
rvjit_la_trans_div_rem(32, rem,  0x00208000, true,  true);	// mod.w
rvjit_la_trans_div_rem(32, remu, 0x00218000, true,  false);	// mod.wu

rvjit_la_trans_djk(64, add,  0x00108000);		// add.d
rvjit_la_trans_djk(64, sub,  0x00118000);		// sub.d
rvjit_la_trans_djk(64, addw, 0x00100000);		// add.w
rvjit_la_trans_djk(64, subw, 0x00110000);		// sub.w
rvjit_la_trans_djk(64, or,   0x00150000);		// or
rvjit_la_trans_djk(64, and,  0x00148000);		// and
rvjit_la_trans_djk(64, xor,  0x00158000);		// xor
rvjit_la_trans_djk(64, sra,  0x00198000);		// sra.d
rvjit_la_trans_djk(64, srl,  0x00190000);		// srl.d
rvjit_la_trans_djk(64, sll,  0x00188000);		// sll.d
rvjit_la_trans_djk(64, sraw, 0x00180000);		// sra.w
rvjit_la_trans_djk(64, srlw, 0x00178000);		// srl.w
rvjit_la_trans_djk(64, sllw, 0x00170000);		// sll.w
rvjit_la_trans_sext_imm(64, addi,  0x02c00000,		// addi.d
				   0x00108000);		// add.d
rvjit_la_trans_sext_imm(64, addiw, 0x02800000,		// addi.w
				   0x00100000);		// add.w
rvjit_la_trans_zext_imm(64, ori,   0x03800000,		// ori
				   0x00150000);		// or
rvjit_la_trans_zext_imm(64, andi,  0x03400000,		// andi
				   0x00148000);		// and
rvjit_la_trans_zext_imm(64, xori,  0x03c00000,		// xori
				   0x00158000);		// xor
rvjit_la_trans_djk6(64, srai,  0x00490000);		// srai.d
rvjit_la_trans_djk6(64, srli,  0x00450000);		// srli.d
rvjit_la_trans_djk6(64, slli,  0x00410000);		// slli.d
rvjit_la_trans_djk6(64, sraiw, 0x00488000);		// srai.w
rvjit_la_trans_djk6(64, srliw, 0x00448000);		// srli.w
rvjit_la_trans_djk6(64, slliw, 0x00408000);		// slli.w
rvjit_la_trans_sext_imm(64, slti,  0x02000000,		// slti
				   0x00120000);		// slt
rvjit_la_trans_sext_imm(64, sltiu, 0x02400000,		// sltui
				   0x00128000);		// sltu
rvjit_la_trans_djk(64, slt,  0x00120000);		// slt
rvjit_la_trans_djk(64, sltu, 0x00128000);		// sltu
rvjit_la_trans_sext_imm(64, lb,  0x28000000,		// ld.b
				 0x38000000)		// ldx.b
rvjit_la_trans_sext_imm(64, lbu, 0x2a000000,		// ld.bu
				 0x38200000)		// ldx.bu
rvjit_la_trans_sext_imm(64, lh,  0x28400000,		// ld.h
				 0x38040000)		// ldx.h
rvjit_la_trans_sext_imm(64, lhu, 0x2a400000,		// ld.hu
				 0x38240000)		// ldx.hu
rvjit_la_trans_sext_imm(64, lw,  0x28800000,		// ld.w
				 0x38080000)		// ldx.w
rvjit_la_trans_sext_imm(64, lwu, 0x2a800000,		// ld.wu
				 0x38280000);		// ldx.wu
rvjit_la_trans_sext_imm(64, ld,  0x28c00000,		// ld.d
				 0x380c0000);		// ldx.d
rvjit_la_trans_sext_imm(64, sb,  0x29000000,		// st.b
				 0x38100000)		// stx.b
rvjit_la_trans_sext_imm(64, sh,  0x29400000,		// st.h
				 0x38140000)		// stx.h
rvjit_la_trans_sext_imm(64, sw,  0x29800000,		// st.w
				 0x38180000)		// stx.w
rvjit_la_trans_sext_imm(64, sd,  0x29c00000,		// st.d
				 0x381c0000);		// stx.d
#define rvjit64_native_beq  rvjit32_native_beq
#define rvjit64_native_bne  rvjit32_native_bne
#define rvjit64_native_beqz rvjit32_native_beqz
#define rvjit64_native_bnez rvjit32_native_bnez
#define rvjit64_native_blt  rvjit32_native_blt
#define rvjit64_native_bge  rvjit32_native_bge
#define rvjit64_native_bltu rvjit32_native_bltu
#define rvjit64_native_bgeu rvjit32_native_bgeu
rvjit_la_trans_djk(64, mul,   0x001d8000);		// mul.d
rvjit_la_trans_djk(64, mulh,  0x001e0000);		// mulh.d
rvjit_la_trans_djk(64, mulhu, 0x001e8000);		// mulh.du
// clang-format on

static inline void rvjit64_native_mulhsu(rvjit_block_t* block, regid_t rd, regid_t rs1, regid_t rs2)
{
    regid_t tmp  = rvjit_claim_hreg(block);
    regid_t mulh = rvjit_claim_hreg(block);

    /*
     *	srai.d $tmp, $rs1, 63
     *	mulh.du $mulh, $rs1, $rs2
     *	mul.d $tmp, $tmp, $rs2
     *	add.d $rd, $mulh, $tmp
     */
    rvjit_la_emit_djk6(block, OP_srai_d, tmp, rs1, 63);
    rvjit_la_emit_djk(block, OP_mulh_du, mulh, rs1, rs2);
    rvjit_la_emit_djk(block, OP_mul_d, tmp, tmp, rs2);
    rvjit_la_emit_djk(block, OP_add_d, rd, mulh, tmp);

    rvjit_free_hreg(block, tmp);
    rvjit_free_hreg(block, mulh);
}

// clang-format off
rvjit_la_trans_div_rem(64, div,  0x00220000, false, true);	// div.d
rvjit_la_trans_div_rem(64, divu, 0x00230000, false, false);	// div.du
rvjit_la_trans_div_rem(64, rem,  0x00228000, true,  true);	// mod.d
rvjit_la_trans_div_rem(64, remu, 0x00238000, true,  false);	// mod.du
#define rvjit64_native_mulw  rvjit32_native_mul
#define rvjit64_native_divw  rvjit32_native_div
#define rvjit64_native_divuw rvjit32_native_divu
#define rvjit64_native_remw  rvjit32_native_rem
#define rvjit64_native_remuw rvjit32_native_remu
// clang-format on

#endif // RVJIT_LA64_H
