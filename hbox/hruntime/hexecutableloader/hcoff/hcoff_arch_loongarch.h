/***************************************************************
 * Name:      hcoff_arch_loongarch.h
 * Purpose:   声明hcoff_loongarch接口(LoongArch COFF)
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-11-20
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HCOFF_LOONGARCH_H__
#define __HCOFF_LOONGARCH_H__
#include "stdint.h"
#include "stdbool.h"
#include "hcoff_common.h"
#include "hcoff_arch.h"
#include "hcoff_relocatable.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#define HCOFF_SECTION_RELOCATION_R_TYPE_LOONGARCH_ABSOLUTE          0x0000
#define HCOFF_SECTION_RELOCATION_R_TYPE_LOONGARCH_ADDR32            0x0001
#define HCOFF_SECTION_RELOCATION_R_TYPE_LOONGARCH_ADDR32NB          0x0002
#define HCOFF_SECTION_RELOCATION_R_TYPE_LOONGARCH_BRANCH26          0x0003
#define HCOFF_SECTION_RELOCATION_R_TYPE_LOONGARCH_PAGEBASE_HI20     0x0004
#define HCOFF_SECTION_RELOCATION_R_TYPE_LOONGARCH_PAGEOFFSET_LO12_I 0x0005
#define HCOFF_SECTION_RELOCATION_R_TYPE_LOONGARCH_PAGEOFFSET_LO12_S 0x0006
#define HCOFF_SECTION_RELOCATION_R_TYPE_LOONGARCH_HI20              0x0007
#define HCOFF_SECTION_RELOCATION_R_TYPE_LOONGARCH_LO12_I            0x0008
#define HCOFF_SECTION_RELOCATION_R_TYPE_LOONGARCH_LO12_S            0x0009
#define HCOFF_SECTION_RELOCATION_R_TYPE_LOONGARCH_REL32             0x000A

typedef hcoff_section_relocation_t hcoff_loongarch_section_relocation_t;
typedef hcoff_arch_relocation_info_t hcoff_loongarch_section_relocation_info_t;

const hcoff_loongarch_section_relocation_info_t * hcoff_loongarch_section_relocation_info_get(const hcoff_loongarch_section_relocation_t *relocation);

bool hcoff_loongarch_section_relocation_doit(uint8_t *relocation_addr,uint64_t symbol_addr,uintptr_t pc_address,uintptr_t image_base,const hcoff_loongarch_section_relocation_t *relocation);

typedef hcoff_relocatable_t hcoff_loongarch_relocatable_t;
typedef const struct hruntime_symbol *(*hcoff_loongarch_external_symbol_find_t)(const char *name);
typedef const struct hruntime_symbol *(*hcoff_loongarch_external_symbol_find_with_usr_t)(const char *name,void *usr);

size_t hcoff_loongarch_relocatable_image_size_get(hcoff_file_input_t *input_file);
bool hcoff_loongarch_relocatable_init(hcoff_loongarch_relocatable_t *loongarch_relocatable,hcoff_file_input_t *input_file,void *buffer,size_t buffer_size,hcoff_loongarch_external_symbol_find_with_usr_t extern_symbol,void *usr);
void hcoff_loongarch_relocatable_deinit(hcoff_loongarch_relocatable_t * loongarch_relocatable);

const hcoff_loongarch_relocatable_t *hcoff_loongarch_relocatable_new(hcoff_file_input_t *input_file,hcoff_loongarch_external_symbol_find_t extern_symbol);
const hcoff_loongarch_relocatable_t *hcoff_loongarch_relocatable_new_with_usr(hcoff_file_input_t *input_file,hcoff_loongarch_external_symbol_find_with_usr_t extern_symbol,void *usr);
void hcoff_loongarch_relocatable_delete(const hcoff_loongarch_relocatable_t * loongarch_relocatable);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HCOFF_LOONGARCH_H__
