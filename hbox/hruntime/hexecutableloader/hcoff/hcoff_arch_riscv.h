/***************************************************************
 * Name:      hcoff_arch_riscv.h
 * Purpose:   声明hcoff_riscv接口(RISC-V COFF)
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-11-20
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HCOFF_RISCV_H__
#define __HCOFF_RISCV_H__
#include "stdint.h"
#include "stdbool.h"
#include "hcoff_common.h"
#include "hcoff_arch.h"
#include "hcoff_relocatable.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#define HCOFF_SECTION_RELOCATION_R_TYPE_RISCV_ABSOLUTE      0x0000
#define HCOFF_SECTION_RELOCATION_R_TYPE_RISCV_ADDR32        0x0001
#define HCOFF_SECTION_RELOCATION_R_TYPE_RISCV_ADDR32NB      0x0002
#define HCOFF_SECTION_RELOCATION_R_TYPE_RISCV_BRANCH        0x0003
#define HCOFF_SECTION_RELOCATION_R_TYPE_RISCV_JAL           0x0004
#define HCOFF_SECTION_RELOCATION_R_TYPE_RISCV_HIGH20        0x0005
#define HCOFF_SECTION_RELOCATION_R_TYPE_RISCV_LOW12I        0x0006
#define HCOFF_SECTION_RELOCATION_R_TYPE_RISCV_LOW12S        0x0007
#define HCOFF_SECTION_RELOCATION_R_TYPE_RISCV_REL32         0x0008

typedef hcoff_section_relocation_t hcoff_riscv_section_relocation_t;
typedef hcoff_arch_relocation_info_t hcoff_riscv_section_relocation_info_t;

const hcoff_riscv_section_relocation_info_t * hcoff_riscv_section_relocation_info_get(const hcoff_riscv_section_relocation_t *relocation);

bool hcoff_riscv_section_relocation_doit(uint8_t *relocation_addr,uint64_t symbol_addr,uintptr_t pc_address,uintptr_t image_base,const hcoff_riscv_section_relocation_t *relocation);

typedef hcoff_relocatable_t hcoff_riscv_relocatable_t;
typedef const struct hruntime_symbol *(*hcoff_riscv_external_symbol_find_t)(const char *name);
typedef const struct hruntime_symbol *(*hcoff_riscv_external_symbol_find_with_usr_t)(const char *name,void *usr);

size_t hcoff_riscv_relocatable_image_size_get(hcoff_file_input_t *input_file);
bool hcoff_riscv_relocatable_init(hcoff_riscv_relocatable_t *riscv_relocatable,hcoff_file_input_t *input_file,void *buffer,size_t buffer_size,hcoff_riscv_external_symbol_find_with_usr_t extern_symbol,void *usr);
void hcoff_riscv_relocatable_deinit(hcoff_riscv_relocatable_t * riscv_relocatable);

const hcoff_riscv_relocatable_t *hcoff_riscv_relocatable_new(hcoff_file_input_t *input_file,hcoff_riscv_external_symbol_find_t extern_symbol);
const hcoff_riscv_relocatable_t *hcoff_riscv_relocatable_new_with_usr(hcoff_file_input_t *input_file,hcoff_riscv_external_symbol_find_with_usr_t extern_symbol,void *usr);
void hcoff_riscv_relocatable_delete(const hcoff_riscv_relocatable_t * riscv_relocatable);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HCOFF_RISCV_H__
