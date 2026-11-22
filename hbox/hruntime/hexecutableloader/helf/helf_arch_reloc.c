/***************************************************************
 * Name:      helf_arch_reloc.c
 * Purpose:   架构重定位描述与应用分发
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-21
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "helf_arch.h"
#include "string.h"

extern bool helf_aarch64_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_alpha_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_amd64_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_amdgpu_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_arc_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_arc64_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_arm_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_avr_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_avr32_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_bfin_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_bpf_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_ceva_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_cr16_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_cris_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_crx_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_csky_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_d10v_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_d30v_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_dlx_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_epiphany_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_fr30_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_frv_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_ft32_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_h8300_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_hppa_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_hppa64_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_i386_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_ia64_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_iamcu_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_ip2k_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_iq2000_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_kvx_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_lanai_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_lm32_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_loongarch32_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_loongarch64_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_m16c_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_m32c_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_m32r_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_m68hc11_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_m68hc12_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_m68k_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_mcore_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_mep_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_metag_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_microblaze_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_mips_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_mips64_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_mmix_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_mn10200_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_mn10300_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_msp430_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_mt_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_nds32_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_nfp_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_nios2_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_or1k_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_pdp11_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_pj_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_ppc_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_ppc64_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_pru_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_riscv32_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_riscv64_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_rl78_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_rx_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_s12z_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_s390_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_s390x_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_score_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_sh_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_sparc_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_sparc64_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_spu_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_tachyum_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_tic6x_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_tile64_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_tilegx_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_tilepro_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_v850_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_vax_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_ve_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_visium_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_wasm32_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_xc16x_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_xgate_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_xstormy16_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_xtensa_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);
extern bool helf_z80_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto);

bool helf_arch_relocation_howto_get(uint16_t machine,uint8_t elfclass,uint32_t type,helf_arch_relocation_howto_t *howto)
{
    if(machine==HELF_EM_386 && elfclass==HELF_ELFCLASS_32)
    {
        return helf_i386_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_IAMCU && elfclass==HELF_ELFCLASS_32)
    {
        return helf_iamcu_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_ARM && elfclass==HELF_ELFCLASS_32)
    {
        return helf_arm_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_MIPS && elfclass==HELF_ELFCLASS_32)
    {
        return helf_mips_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_MIPS_RS3_LE && elfclass==HELF_ELFCLASS_32)
    {
        return helf_mips_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_MIPS_RS4_BE && elfclass==HELF_ELFCLASS_32)
    {
        return helf_mips_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_PPC && elfclass==HELF_ELFCLASS_32)
    {
        return helf_ppc_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_PPC_OLD && elfclass==HELF_ELFCLASS_32)
    {
        return helf_ppc_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_CYGNUS_POWERPC && elfclass==HELF_ELFCLASS_32)
    {
        return helf_ppc_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_SPARC && elfclass==HELF_ELFCLASS_32)
    {
        return helf_sparc_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_SPARC32PLUS && elfclass==HELF_ELFCLASS_32)
    {
        return helf_sparc_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_68K && elfclass==HELF_ELFCLASS_32)
    {
        return helf_m68k_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_68HC11 && elfclass==HELF_ELFCLASS_32)
    {
        return helf_m68hc11_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_68HC12 && elfclass==HELF_ELFCLASS_32)
    {
        return helf_m68hc12_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_SH && elfclass==HELF_ELFCLASS_32)
    {
        return helf_sh_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_V850 && elfclass==HELF_ELFCLASS_32)
    {
        return helf_v850_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_CYGNUS_V850 && elfclass==HELF_ELFCLASS_32)
    {
        return helf_v850_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_AVR && elfclass==HELF_ELFCLASS_32)
    {
        return helf_avr_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_AVR_OLD && elfclass==HELF_ELFCLASS_32)
    {
        return helf_avr_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_AVR32 && elfclass==HELF_ELFCLASS_32)
    {
        return helf_avr32_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_MSP430 && elfclass==HELF_ELFCLASS_32)
    {
        return helf_msp430_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_MSP430_OLD && elfclass==HELF_ELFCLASS_32)
    {
        return helf_msp430_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_MICROBLAZE && elfclass==HELF_ELFCLASS_32)
    {
        return helf_microblaze_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_MICROBLAZE_OLD && elfclass==HELF_ELFCLASS_32)
    {
        return helf_microblaze_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_XTENSA && elfclass==HELF_ELFCLASS_32)
    {
        return helf_xtensa_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_XTENSA_OLD && elfclass==HELF_ELFCLASS_32)
    {
        return helf_xtensa_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_ARC && elfclass==HELF_ELFCLASS_32)
    {
        return helf_arc_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_ARC_COMPACT && elfclass==HELF_ELFCLASS_32)
    {
        return helf_arc_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_ARC_COMPACT2 && elfclass==HELF_ELFCLASS_32)
    {
        return helf_arc_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_ARC_COMPACT3 && elfclass==HELF_ELFCLASS_32)
    {
        return helf_arc_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_ALTERA_NIOS2 && elfclass==HELF_ELFCLASS_32)
    {
        return helf_nios2_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_NIOS32 && elfclass==HELF_ELFCLASS_32)
    {
        return helf_nios2_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_BLACKFIN && elfclass==HELF_ELFCLASS_32)
    {
        return helf_bfin_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_M32R && elfclass==HELF_ELFCLASS_32)
    {
        return helf_m32r_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_CYGNUS_M32R && elfclass==HELF_ELFCLASS_32)
    {
        return helf_m32r_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_MN10300 && elfclass==HELF_ELFCLASS_32)
    {
        return helf_mn10300_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_CYGNUS_MN10300 && elfclass==HELF_ELFCLASS_32)
    {
        return helf_mn10300_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_MN10200 && elfclass==HELF_ELFCLASS_32)
    {
        return helf_mn10200_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_CYGNUS_MN10200 && elfclass==HELF_ELFCLASS_32)
    {
        return helf_mn10200_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_CRIS && elfclass==HELF_ELFCLASS_32)
    {
        return helf_cris_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_CR16 && elfclass==HELF_ELFCLASS_32)
    {
        return helf_cr16_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_CRX && elfclass==HELF_ELFCLASS_32)
    {
        return helf_crx_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_DLX && elfclass==HELF_ELFCLASS_32)
    {
        return helf_dlx_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_CYGNUS_FRV && elfclass==HELF_ELFCLASS_32)
    {
        return helf_frv_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_OR1K && elfclass==HELF_ELFCLASS_32)
    {
        return helf_or1k_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_OPENRISC && elfclass==HELF_ELFCLASS_32)
    {
        return helf_or1k_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_CSKY && elfclass==HELF_ELFCLASS_32)
    {
        return helf_csky_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_CSKY_OLD && elfclass==HELF_ELFCLASS_32)
    {
        return helf_csky_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_RX && elfclass==HELF_ELFCLASS_32)
    {
        return helf_rx_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_NDS32 && elfclass==HELF_ELFCLASS_32)
    {
        return helf_nds32_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_SCORE && elfclass==HELF_ELFCLASS_32)
    {
        return helf_score_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_SCORE_OLD && elfclass==HELF_ELFCLASS_32)
    {
        return helf_score_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_FT32 && elfclass==HELF_ELFCLASS_32)
    {
        return helf_ft32_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_VISIUM && elfclass==HELF_ELFCLASS_32)
    {
        return helf_visium_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_Z80 && elfclass==HELF_ELFCLASS_32)
    {
        return helf_z80_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_H8_300 && elfclass==HELF_ELFCLASS_32)
    {
        return helf_h8300_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_H8_300H && elfclass==HELF_ELFCLASS_32)
    {
        return helf_h8300_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_H8S && elfclass==HELF_ELFCLASS_32)
    {
        return helf_h8300_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_H8_500 && elfclass==HELF_ELFCLASS_32)
    {
        return helf_h8300_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_MCORE && elfclass==HELF_ELFCLASS_32)
    {
        return helf_mcore_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_RCE && elfclass==HELF_ELFCLASS_32)
    {
        return helf_mcore_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_VAX && elfclass==HELF_ELFCLASS_32)
    {
        return helf_vax_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_PARISC && elfclass==HELF_ELFCLASS_32)
    {
        return helf_hppa_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_S390 && elfclass==HELF_ELFCLASS_32)
    {
        return helf_s390_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_S390_OLD && elfclass==HELF_ELFCLASS_32)
    {
        return helf_s390_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_LATTICEMICO32 && elfclass==HELF_ELFCLASS_32)
    {
        return helf_lm32_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_WEBASSEMBLY && elfclass==HELF_ELFCLASS_32)
    {
        return helf_wasm32_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_TILEPRO && elfclass==HELF_ELFCLASS_32)
    {
        return helf_tilepro_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_ADAPTEVA_EPIPHANY && elfclass==HELF_ELFCLASS_32)
    {
        return helf_epiphany_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_CYGNUS_MEP && elfclass==HELF_ELFCLASS_32)
    {
        return helf_mep_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_METAG && elfclass==HELF_ELFCLASS_32)
    {
        return helf_metag_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_MT && elfclass==HELF_ELFCLASS_32)
    {
        return helf_mt_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_PJ && elfclass==HELF_ELFCLASS_32)
    {
        return helf_pj_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_PJ_OLD && elfclass==HELF_ELFCLASS_32)
    {
        return helf_pj_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_SPU && elfclass==HELF_ELFCLASS_32)
    {
        return helf_spu_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_XGATE && elfclass==HELF_ELFCLASS_32)
    {
        return helf_xgate_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_XSTORMY16 && elfclass==HELF_ELFCLASS_32)
    {
        return helf_xstormy16_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_D10V && elfclass==HELF_ELFCLASS_32)
    {
        return helf_d10v_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_CYGNUS_D10V && elfclass==HELF_ELFCLASS_32)
    {
        return helf_d10v_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_D30V && elfclass==HELF_ELFCLASS_32)
    {
        return helf_d30v_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_CYGNUS_D30V && elfclass==HELF_ELFCLASS_32)
    {
        return helf_d30v_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_FR30 && elfclass==HELF_ELFCLASS_32)
    {
        return helf_fr30_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_CYGNUS_FR30 && elfclass==HELF_ELFCLASS_32)
    {
        return helf_fr30_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_IP2K && elfclass==HELF_ELFCLASS_32)
    {
        return helf_ip2k_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_IP2K_OLD && elfclass==HELF_ELFCLASS_32)
    {
        return helf_ip2k_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_IQ2000 && elfclass==HELF_ELFCLASS_32)
    {
        return helf_iq2000_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_M32C && elfclass==HELF_ELFCLASS_32)
    {
        return helf_m32c_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_M32C_OLD && elfclass==HELF_ELFCLASS_32)
    {
        return helf_m32c_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_M16C && elfclass==HELF_ELFCLASS_32)
    {
        return helf_m16c_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_TI_PRU && elfclass==HELF_ELFCLASS_32)
    {
        return helf_pru_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_RL78 && elfclass==HELF_ELFCLASS_32)
    {
        return helf_rl78_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_S12Z && elfclass==HELF_ELFCLASS_32)
    {
        return helf_s12z_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_TI_C6000 && elfclass==HELF_ELFCLASS_32)
    {
        return helf_tic6x_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_TI_C2000 && elfclass==HELF_ELFCLASS_32)
    {
        return helf_tic6x_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_TI_C5500 && elfclass==HELF_ELFCLASS_32)
    {
        return helf_tic6x_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_XC16X && elfclass==HELF_ELFCLASS_32)
    {
        return helf_xc16x_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_C166 && elfclass==HELF_ELFCLASS_32)
    {
        return helf_xc16x_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_LANAI && elfclass==HELF_ELFCLASS_32)
    {
        return helf_lanai_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_CEVA && elfclass==HELF_ELFCLASS_32)
    {
        return helf_ceva_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_CEVA_X2 && elfclass==HELF_ELFCLASS_32)
    {
        return helf_ceva_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_PDP11 && elfclass==HELF_ELFCLASS_32)
    {
        return helf_pdp11_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_X86_64 && elfclass==HELF_ELFCLASS_64)
    {
        return helf_amd64_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_AMD64 && elfclass==HELF_ELFCLASS_64)
    {
        return helf_amd64_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_K1OM && elfclass==HELF_ELFCLASS_64)
    {
        return helf_amd64_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_L1OM && elfclass==HELF_ELFCLASS_64)
    {
        return helf_amd64_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_AARCH64 && elfclass==HELF_ELFCLASS_64)
    {
        return helf_aarch64_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_PPC64 && elfclass==HELF_ELFCLASS_64)
    {
        return helf_ppc64_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_SPARCV9 && elfclass==HELF_ELFCLASS_64)
    {
        return helf_sparc64_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_OLD_SPARCV9 && elfclass==HELF_ELFCLASS_64)
    {
        return helf_sparc64_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_MIPS && elfclass==HELF_ELFCLASS_64)
    {
        return helf_mips64_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_RISCV && elfclass==HELF_ELFCLASS_32)
    {
        return helf_riscv32_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_RISCV && elfclass==HELF_ELFCLASS_64)
    {
        return helf_riscv64_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_LOONGARCH && elfclass==HELF_ELFCLASS_32)
    {
        return helf_loongarch32_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_LOONGARCH && elfclass==HELF_ELFCLASS_64)
    {
        return helf_loongarch64_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_S390 && elfclass==HELF_ELFCLASS_64)
    {
        return helf_s390x_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_S390_OLD && elfclass==HELF_ELFCLASS_64)
    {
        return helf_s390x_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_PARISC && elfclass==HELF_ELFCLASS_64)
    {
        return helf_hppa64_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_IA_64 && elfclass==HELF_ELFCLASS_64)
    {
        return helf_ia64_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_ALPHA && elfclass==HELF_ELFCLASS_64)
    {
        return helf_alpha_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_OLD_ALPHA && elfclass==HELF_ELFCLASS_64)
    {
        return helf_alpha_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_ALPHA_STD && elfclass==HELF_ELFCLASS_64)
    {
        return helf_alpha_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_TILEGX && elfclass==HELF_ELFCLASS_64)
    {
        return helf_tilegx_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_TILE64 && elfclass==HELF_ELFCLASS_64)
    {
        return helf_tile64_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_MMIX && elfclass==HELF_ELFCLASS_64)
    {
        return helf_mmix_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_BPF && elfclass==HELF_ELFCLASS_64)
    {
        return helf_bpf_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_AMDGPU && elfclass==HELF_ELFCLASS_64)
    {
        return helf_amdgpu_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_NFP && elfclass==HELF_ELFCLASS_64)
    {
        return helf_nfp_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_KVX && elfclass==HELF_ELFCLASS_64)
    {
        return helf_kvx_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_TACHYUM && elfclass==HELF_ELFCLASS_64)
    {
        return helf_tachyum_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_VE && elfclass==HELF_ELFCLASS_64)
    {
        return helf_ve_relocation_howto_get(type,howto);
    }
    if(machine==HELF_EM_ARC_COMPACT3_64 && elfclass==HELF_ELFCLASS_64)
    {
        return helf_arc64_relocation_howto_get(type,howto);
    }
    return false;
}

extern bool helf_amd64_relocation_apply(uint32_t type,uint8_t *target,uintptr_t place,uintptr_t symbol_address,int64_t addend,const helf_arch_relocation_context_t *context);
extern bool helf_aarch64_relocation_apply(uint32_t type,uint8_t *target,uintptr_t place,uintptr_t symbol_address,int64_t addend,const helf_arch_relocation_context_t *context);
extern bool helf_riscv32_relocation_apply(uint32_t type,uint8_t *target,uintptr_t place,uintptr_t symbol_address,int64_t addend,const helf_arch_relocation_context_t *context);
extern bool helf_riscv64_relocation_apply(uint32_t type,uint8_t *target,uintptr_t place,uintptr_t symbol_address,int64_t addend,const helf_arch_relocation_context_t *context);
extern bool helf_ppc64_relocation_apply(uint32_t type,uint8_t *target,uintptr_t place,uintptr_t symbol_address,int64_t addend,const helf_arch_relocation_context_t *context);
extern bool helf_loongarch32_relocation_apply(uint32_t type,uint8_t *target,uintptr_t place,uintptr_t symbol_address,int64_t addend,const helf_arch_relocation_context_t *context);
extern bool helf_loongarch64_relocation_apply(uint32_t type,uint8_t *target,uintptr_t place,uintptr_t symbol_address,int64_t addend,const helf_arch_relocation_context_t *context);
extern bool helf_mips_relocation_apply(uint32_t type,uint8_t *target,uintptr_t place,uintptr_t symbol_address,int64_t addend,const helf_arch_relocation_context_t *context);
extern bool helf_mips64_relocation_apply(uint32_t type,uint8_t *target,uintptr_t place,uintptr_t symbol_address,int64_t addend,const helf_arch_relocation_context_t *context);
extern bool helf_ppc64_relocation_symbol_address(uint32_t type,uintptr_t *address,const helf_arch_relocation_context_t *context);
extern size_t helf_ppc64_relocation_toc_size_get(helf_file_input_t *input_file);
extern bool helf_ppc64_relocation_section_is_priority(const char *name);

bool helf_arch_relocation_apply(uint16_t machine,uint8_t elfclass,uint32_t type,uint8_t *target,uintptr_t place,uintptr_t symbol_address,int64_t addend,const helf_arch_relocation_context_t *context)
{
    if(machine==HELF_EM_X86_64 && elfclass==HELF_ELFCLASS_64)
    {
        return helf_amd64_relocation_apply(type,target,place,symbol_address,addend,context);
    }
    if(machine==HELF_EM_AARCH64 && elfclass==HELF_ELFCLASS_64)
    {
        return helf_aarch64_relocation_apply(type,target,place,symbol_address,addend,context);
    }
    if(machine==HELF_EM_RISCV && elfclass==HELF_ELFCLASS_32)
    {
        return helf_riscv32_relocation_apply(type,target,place,symbol_address,addend,context);
    }
    if(machine==HELF_EM_RISCV && elfclass==HELF_ELFCLASS_64)
    {
        return helf_riscv64_relocation_apply(type,target,place,symbol_address,addend,context);
    }
    if(machine==HELF_EM_PPC64 && elfclass==HELF_ELFCLASS_64)
    {
        return helf_ppc64_relocation_apply(type,target,place,symbol_address,addend,context);
    }
    if(machine==HELF_EM_LOONGARCH && elfclass==HELF_ELFCLASS_32)
    {
        return helf_loongarch32_relocation_apply(type,target,place,symbol_address,addend,context);
    }
    if(machine==HELF_EM_LOONGARCH && elfclass==HELF_ELFCLASS_64)
    {
        return helf_loongarch64_relocation_apply(type,target,place,symbol_address,addend,context);
    }
    if(machine==HELF_EM_MIPS && elfclass==HELF_ELFCLASS_32)
    {
        return helf_mips_relocation_apply(type,target,place,symbol_address,addend,context);
    }
    if(machine==HELF_EM_MIPS && elfclass==HELF_ELFCLASS_64)
    {
        return helf_mips64_relocation_apply(type,target,place,symbol_address,addend,context);
    }
    return false;
}

bool helf_arch_relocation_symbol_address(uint16_t machine,uint8_t elfclass,uint32_t type,uintptr_t *address,const helf_arch_relocation_context_t *context)
{
    if(machine==HELF_EM_PPC64 && elfclass==HELF_ELFCLASS_64)
    {
        return helf_ppc64_relocation_symbol_address(type,address,context);
    }
    return false;
}

size_t helf_arch_relocation_toc_size_get(uint16_t machine,uint8_t elfclass,helf_file_input_t *input_file)
{
    if(machine==HELF_EM_PPC64 && elfclass==HELF_ELFCLASS_64)
    {
        return helf_ppc64_relocation_toc_size_get(input_file);
    }
    return 0;
}

bool helf_arch_relocation_section_is_priority(uint16_t machine,uint8_t elfclass,const char *name)
{
    if(machine==HELF_EM_PPC64 && elfclass==HELF_ELFCLASS_64)
    {
        return helf_ppc64_relocation_section_is_priority(name);
    }
    return false;
}
