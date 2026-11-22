/***************************************************************
 * Name:      helf.h
 * Purpose:   声明helf接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-06-09
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HELF_H__
#define __HELF_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/*
 * 加载器依赖 hmemory/hruntime。为保持仅解析/编辑场景可独立使用，
 * 仅在依赖头文件可用时编译加载器。
 */
#if defined(__HRUNTIME_H__)
#define HELF_LOADER_AVAILABLE 1
#elif defined(__has_include)
#if __has_include("hmemory.h") && __has_include("hruntime.h")
#define HELF_LOADER_AVAILABLE 1
#endif
#endif

#include "helf/helf_common.h"
#include "helf/helf_section.h"
#include "helf/helf_arch.h"
#include "helf/helf_output.h"
#ifdef HELF_LOADER_AVAILABLE
#include "helf/helf_loader.h"
#endif
#include "helf/helf_arch_aarch64.h"
#include "helf/helf_arch_alpha.h"
#include "helf/helf_arch_amd64.h"
#include "helf/helf_arch_amdgpu.h"
#include "helf/helf_arch_arc.h"
#include "helf/helf_arch_arc64.h"
#include "helf/helf_arch_arm.h"
#include "helf/helf_arch_avr.h"
#include "helf/helf_arch_avr32.h"
#include "helf/helf_arch_bfin.h"
#include "helf/helf_arch_bpf.h"
#include "helf/helf_arch_ceva.h"
#include "helf/helf_arch_cr16.h"
#include "helf/helf_arch_cris.h"
#include "helf/helf_arch_crx.h"
#include "helf/helf_arch_csky.h"
#include "helf/helf_arch_d10v.h"
#include "helf/helf_arch_d30v.h"
#include "helf/helf_arch_dlx.h"
#include "helf/helf_arch_epiphany.h"
#include "helf/helf_arch_fr30.h"
#include "helf/helf_arch_frv.h"
#include "helf/helf_arch_ft32.h"
#include "helf/helf_arch_h8300.h"
#include "helf/helf_arch_hppa.h"
#include "helf/helf_arch_hppa64.h"
#include "helf/helf_arch_i386.h"
#include "helf/helf_arch_ia64.h"
#include "helf/helf_arch_iamcu.h"
#include "helf/helf_arch_ip2k.h"
#include "helf/helf_arch_iq2000.h"
#include "helf/helf_arch_kvx.h"
#include "helf/helf_arch_lanai.h"
#include "helf/helf_arch_lm32.h"
#include "helf/helf_arch_loongarch32.h"
#include "helf/helf_arch_loongarch64.h"
#include "helf/helf_arch_m16c.h"
#include "helf/helf_arch_m32c.h"
#include "helf/helf_arch_m32r.h"
#include "helf/helf_arch_m68hc11.h"
#include "helf/helf_arch_m68hc12.h"
#include "helf/helf_arch_m68k.h"
#include "helf/helf_arch_mcore.h"
#include "helf/helf_arch_mep.h"
#include "helf/helf_arch_metag.h"
#include "helf/helf_arch_microblaze.h"
#include "helf/helf_arch_mips.h"
#include "helf/helf_arch_mips64.h"
#include "helf/helf_arch_mmix.h"
#include "helf/helf_arch_mn10200.h"
#include "helf/helf_arch_mn10300.h"
#include "helf/helf_arch_moxie.h"
#include "helf/helf_arch_msp430.h"
#include "helf/helf_arch_mt.h"
#include "helf/helf_arch_nds32.h"
#include "helf/helf_arch_nfp.h"
#include "helf/helf_arch_nios2.h"
#include "helf/helf_arch_or1k.h"
#include "helf/helf_arch_pdp11.h"
#include "helf/helf_arch_pj.h"
#include "helf/helf_arch_ppc.h"
#include "helf/helf_arch_ppc64.h"
#include "helf/helf_arch_pru.h"
#include "helf/helf_arch_riscv32.h"
#include "helf/helf_arch_riscv64.h"
#include "helf/helf_arch_rl78.h"
#include "helf/helf_arch_rx.h"
#include "helf/helf_arch_s12z.h"
#include "helf/helf_arch_s390.h"
#include "helf/helf_arch_s390x.h"
#include "helf/helf_arch_score.h"
#include "helf/helf_arch_sh.h"
#include "helf/helf_arch_sparc.h"
#include "helf/helf_arch_sparc64.h"
#include "helf/helf_arch_spu.h"
#include "helf/helf_arch_tachyum.h"
#include "helf/helf_arch_tic6x.h"
#include "helf/helf_arch_tile64.h"
#include "helf/helf_arch_tilegx.h"
#include "helf/helf_arch_tilepro.h"
#include "helf/helf_arch_v850.h"
#include "helf/helf_arch_vax.h"
#include "helf/helf_arch_ve.h"
#include "helf/helf_arch_visium.h"
#include "helf/helf_arch_wasm32.h"
#include "helf/helf_arch_xc16x.h"
#include "helf/helf_arch_xgate.h"
#include "helf/helf_arch_xstormy16.h"
#include "helf/helf_arch_xtensa.h"
#include "helf/helf_arch_z80.h"

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HELF_H__
