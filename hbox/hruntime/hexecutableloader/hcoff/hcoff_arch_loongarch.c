/***************************************************************
 * Name:      hcoff_arch_loongarch.c
 * Purpose:   实现hcoff_loongarch接口(LoongArch COFF)
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-11-20
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hcoff_arch_loongarch.h"

static const hcoff_arch_relocation_info_t loongarch_absolute= {"IMAGE_REL_LOONGARCH_ABSOLUTE",HCOFF_SECTION_RELOCATION_R_TYPE_LOONGARCH_ABSOLUTE,0,HCOFF_ARCH_RELOCATION_FLAG_IGNORED};
static const hcoff_arch_relocation_info_t loongarch_addr32= {"IMAGE_REL_LOONGARCH_ADDR32",HCOFF_SECTION_RELOCATION_R_TYPE_LOONGARCH_ADDR32,4,0};
static const hcoff_arch_relocation_info_t loongarch_addr32nb= {"IMAGE_REL_LOONGARCH_ADDR32NB",HCOFF_SECTION_RELOCATION_R_TYPE_LOONGARCH_ADDR32NB,4,0};
static const hcoff_arch_relocation_info_t loongarch_branch26= {"IMAGE_REL_LOONGARCH_BRANCH26",HCOFF_SECTION_RELOCATION_R_TYPE_LOONGARCH_BRANCH26,4,HCOFF_ARCH_RELOCATION_FLAG_PC_RELATIVE};
static const hcoff_arch_relocation_info_t loongarch_pagebase_hi20= {"IMAGE_REL_LOONGARCH_PAGEBASE_HI20",HCOFF_SECTION_RELOCATION_R_TYPE_LOONGARCH_PAGEBASE_HI20,4,0};
static const hcoff_arch_relocation_info_t loongarch_pageoffset_lo12_i= {"IMAGE_REL_LOONGARCH_PAGEOFFSET_LO12_I",HCOFF_SECTION_RELOCATION_R_TYPE_LOONGARCH_PAGEOFFSET_LO12_I,4,0};
static const hcoff_arch_relocation_info_t loongarch_pageoffset_lo12_s= {"IMAGE_REL_LOONGARCH_PAGEOFFSET_LO12_S",HCOFF_SECTION_RELOCATION_R_TYPE_LOONGARCH_PAGEOFFSET_LO12_S,4,0};
static const hcoff_arch_relocation_info_t loongarch_hi20= {"IMAGE_REL_LOONGARCH_HI20",HCOFF_SECTION_RELOCATION_R_TYPE_LOONGARCH_HI20,4,0};
static const hcoff_arch_relocation_info_t loongarch_lo12_i= {"IMAGE_REL_LOONGARCH_LO12_I",HCOFF_SECTION_RELOCATION_R_TYPE_LOONGARCH_LO12_I,4,0};
static const hcoff_arch_relocation_info_t loongarch_lo12_s= {"IMAGE_REL_LOONGARCH_LO12_S",HCOFF_SECTION_RELOCATION_R_TYPE_LOONGARCH_LO12_S,4,0};
static const hcoff_arch_relocation_info_t loongarch_rel32= {"IMAGE_REL_LOONGARCH_REL32",HCOFF_SECTION_RELOCATION_R_TYPE_LOONGARCH_REL32,4,HCOFF_ARCH_RELOCATION_FLAG_PC_RELATIVE};

const hcoff_loongarch_section_relocation_info_t * hcoff_loongarch_section_relocation_info_get(const hcoff_loongarch_section_relocation_t *relocation)
{
    if(relocation==NULL)
    {
        return &loongarch_absolute;
    }
    switch(relocation->r_type)
    {
    case HCOFF_SECTION_RELOCATION_R_TYPE_LOONGARCH_ABSOLUTE:
        return &loongarch_absolute;
    case HCOFF_SECTION_RELOCATION_R_TYPE_LOONGARCH_ADDR32:
        return &loongarch_addr32;
    case HCOFF_SECTION_RELOCATION_R_TYPE_LOONGARCH_ADDR32NB:
        return &loongarch_addr32nb;
    case HCOFF_SECTION_RELOCATION_R_TYPE_LOONGARCH_BRANCH26:
        return &loongarch_branch26;
    case HCOFF_SECTION_RELOCATION_R_TYPE_LOONGARCH_PAGEBASE_HI20:
        return &loongarch_pagebase_hi20;
    case HCOFF_SECTION_RELOCATION_R_TYPE_LOONGARCH_PAGEOFFSET_LO12_I:
        return &loongarch_pageoffset_lo12_i;
    case HCOFF_SECTION_RELOCATION_R_TYPE_LOONGARCH_PAGEOFFSET_LO12_S:
        return &loongarch_pageoffset_lo12_s;
    case HCOFF_SECTION_RELOCATION_R_TYPE_LOONGARCH_HI20:
        return &loongarch_hi20;
    case HCOFF_SECTION_RELOCATION_R_TYPE_LOONGARCH_LO12_I:
        return &loongarch_lo12_i;
    case HCOFF_SECTION_RELOCATION_R_TYPE_LOONGARCH_LO12_S:
        return &loongarch_lo12_s;
    case HCOFF_SECTION_RELOCATION_R_TYPE_LOONGARCH_REL32:
        return &loongarch_rel32;
    default:
        break;
    }
    return NULL;
}

static const hcoff_arch_relocation_info_t *hcoff_loongarch_arch_relocation_info_get(uint16_t type)
{
    hcoff_section_relocation_t relocation= {0};
    relocation.r_type=type;
    return hcoff_loongarch_section_relocation_info_get(&relocation);
}

static void hcoff_loongarch_apply_hi20(uint8_t *addr,uint64_t symbol_addr)
{
    uint32_t hi=(uint32_t)((symbol_addr+0x800)>>12);
    uint32_t inst=hcoff_get_uint32_le(addr);
    inst=(inst & 0xFFC0001FU) | ((hi & 0xFFFFFU) << 5);
    hcoff_put_uint32_le(addr,inst);
}

static void hcoff_loongarch_apply_lo12_i(uint8_t *addr,uint64_t symbol_addr)
{
    uint32_t lo=(uint32_t)symbol_addr & 0xFFFU;
    uint32_t inst=hcoff_get_uint32_le(addr);
    inst=(inst & 0xFFC003FFU) | (lo << 10);
    hcoff_put_uint32_le(addr,inst);
}

static void hcoff_loongarch_apply_lo12_s(uint8_t *addr,uint64_t symbol_addr)
{
    uint32_t lo=(uint32_t)symbol_addr & 0xFFFU;
    uint32_t inst=hcoff_get_uint32_le(addr);
    inst=(inst & 0xFFC003FFU) | (lo << 10);
    hcoff_put_uint32_le(addr,inst);
}

bool hcoff_loongarch_section_relocation_doit(uint8_t *relocation_addr,uint64_t symbol_addr,uintptr_t pc_address,uintptr_t image_base,const hcoff_loongarch_section_relocation_t *relocation)
{
    if(relocation_addr==NULL || relocation==NULL)
    {
        return false;
    }
    switch(relocation->r_type)
    {
    case HCOFF_SECTION_RELOCATION_R_TYPE_LOONGARCH_ABSOLUTE:
        return true;
    case HCOFF_SECTION_RELOCATION_R_TYPE_LOONGARCH_ADDR32:
        hcoff_put_uint32_le(relocation_addr,(uint32_t)symbol_addr);
        return true;
    case HCOFF_SECTION_RELOCATION_R_TYPE_LOONGARCH_ADDR32NB:
        hcoff_put_uint32_le(relocation_addr,(uint32_t)(symbol_addr-(uint64_t)image_base));
        return true;
    case HCOFF_SECTION_RELOCATION_R_TYPE_LOONGARCH_BRANCH26:
    {
        int64_t diff=(int64_t)symbol_addr-(int64_t)pc_address;
        uint32_t inst=hcoff_get_uint32_le(relocation_addr);
        inst=(inst & 0xFC000000U) | (((uint32_t)(diff>>2)) & 0x03FFFFFFU);
        hcoff_put_uint32_le(relocation_addr,inst);
        return true;
    }
    case HCOFF_SECTION_RELOCATION_R_TYPE_LOONGARCH_PAGEBASE_HI20:
    case HCOFF_SECTION_RELOCATION_R_TYPE_LOONGARCH_HI20:
        hcoff_loongarch_apply_hi20(relocation_addr,symbol_addr);
        return true;
    case HCOFF_SECTION_RELOCATION_R_TYPE_LOONGARCH_PAGEOFFSET_LO12_I:
    case HCOFF_SECTION_RELOCATION_R_TYPE_LOONGARCH_LO12_I:
        hcoff_loongarch_apply_lo12_i(relocation_addr,symbol_addr);
        return true;
    case HCOFF_SECTION_RELOCATION_R_TYPE_LOONGARCH_PAGEOFFSET_LO12_S:
    case HCOFF_SECTION_RELOCATION_R_TYPE_LOONGARCH_LO12_S:
        hcoff_loongarch_apply_lo12_s(relocation_addr,symbol_addr);
        return true;
    case HCOFF_SECTION_RELOCATION_R_TYPE_LOONGARCH_REL32:
    {
        int64_t diff=(int64_t)symbol_addr-(int64_t)pc_address;
        hcoff_put_uint32_le(relocation_addr,(uint32_t)diff);
        return true;
    }
    default:
        break;
    }
    return false;
}

static bool hcoff_loongarch_arch_relocation_doit(uint8_t *relocation_addr,uint64_t symbol_addr,uintptr_t pc_address,uintptr_t image_base,uint16_t type)
{
    hcoff_section_relocation_t relocation= {0};
    relocation.r_type=type;
    return hcoff_loongarch_section_relocation_doit(relocation_addr,symbol_addr,pc_address,image_base,&relocation);
}

const hcoff_arch_ops_t hcoff_loongarch_arch_ops=
{
    HCOFF_FILEHEADER_F_MAGIC_LOONGARCH64,
    "loongarch64",
    true,
    14,
    hcoff_loongarch_arch_relocation_doit,
    hcoff_loongarch_arch_relocation_info_get
};

static bool hcoff_loongarch_relocatable_machine_get(hcoff_file_input_t *input_file,uint16_t *machine)
{
    if(input_file==NULL || machine==NULL)
    {
        return false;
    }
    hcoff_fileheader_t fileheader= {0};
    if(!hcoff_fileheader_read_at(&fileheader,input_file,0))
    {
        return false;
    }
    if(fileheader.f_magic!=HCOFF_FILEHEADER_F_MAGIC_LOONGARCH64)
    {
        return false;
    }
    *machine=fileheader.f_magic;
    return true;
}

size_t hcoff_loongarch_relocatable_image_size_get(hcoff_file_input_t *input_file)
{
    uint16_t machine=HCOFF_FILEHEADER_F_MAGIC_UNKNOWN;
    if(!hcoff_loongarch_relocatable_machine_get(input_file,&machine))
    {
        return 0;
    }
    return hcoff_relocatable_image_size_get(input_file,machine);
}

bool hcoff_loongarch_relocatable_init(hcoff_loongarch_relocatable_t *loongarch_relocatable,hcoff_file_input_t *input_file,void *buffer,size_t buffer_size,hcoff_loongarch_external_symbol_find_with_usr_t extern_symbol,void *usr)
{
    uint16_t machine=HCOFF_FILEHEADER_F_MAGIC_UNKNOWN;
    if(!hcoff_loongarch_relocatable_machine_get(input_file,&machine))
    {
        return false;
    }
    return hcoff_relocatable_init(loongarch_relocatable,input_file,machine,buffer,buffer_size,(hcoff_relocatable_external_symbol_find_t)extern_symbol,usr);
}

void hcoff_loongarch_relocatable_deinit(hcoff_loongarch_relocatable_t * loongarch_relocatable)
{
    hcoff_relocatable_deinit(loongarch_relocatable);
}

static const struct hruntime_symbol *hcoff_loongarch_external_symbol_find_with_usr_adapter(const char *name,void *usr)
{
    hcoff_loongarch_external_symbol_find_t extern_symbol=(hcoff_loongarch_external_symbol_find_t)usr;
    if(extern_symbol!=NULL)
    {
        return extern_symbol(name);
    }
    return NULL;
}

const hcoff_loongarch_relocatable_t *hcoff_loongarch_relocatable_new(hcoff_file_input_t *input_file,hcoff_loongarch_external_symbol_find_t extern_symbol)
{
    return hcoff_loongarch_relocatable_new_with_usr(input_file,hcoff_loongarch_external_symbol_find_with_usr_adapter,(void *)extern_symbol);
}

const hcoff_loongarch_relocatable_t *hcoff_loongarch_relocatable_new_with_usr(hcoff_file_input_t *input_file,hcoff_loongarch_external_symbol_find_with_usr_t extern_symbol,void *usr)
{
    uint16_t machine=HCOFF_FILEHEADER_F_MAGIC_UNKNOWN;
    if(!hcoff_loongarch_relocatable_machine_get(input_file,&machine))
    {
        return NULL;
    }
    return hcoff_relocatable_new_with_usr(input_file,machine,(hcoff_relocatable_external_symbol_find_t)extern_symbol,usr);
}

void hcoff_loongarch_relocatable_delete(const hcoff_loongarch_relocatable_t * loongarch_relocatable)
{
    hcoff_relocatable_delete(loongarch_relocatable);
}
