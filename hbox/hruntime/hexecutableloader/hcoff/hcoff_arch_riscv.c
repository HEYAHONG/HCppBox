/***************************************************************
 * Name:      hcoff_arch_riscv.c
 * Purpose:   实现hcoff_riscv接口(RISC-V COFF)
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-11-20
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hcoff_arch_riscv.h"

static const hcoff_arch_relocation_info_t riscv_absolute= {"IMAGE_REL_RISCV_ABSOLUTE",HCOFF_SECTION_RELOCATION_R_TYPE_RISCV_ABSOLUTE,0,HCOFF_ARCH_RELOCATION_FLAG_IGNORED};
static const hcoff_arch_relocation_info_t riscv_addr32= {"IMAGE_REL_RISCV_ADDR32",HCOFF_SECTION_RELOCATION_R_TYPE_RISCV_ADDR32,4,0};
static const hcoff_arch_relocation_info_t riscv_addr32nb= {"IMAGE_REL_RISCV_ADDR32NB",HCOFF_SECTION_RELOCATION_R_TYPE_RISCV_ADDR32NB,4,0};
static const hcoff_arch_relocation_info_t riscv_branch= {"IMAGE_REL_RISCV_BRANCH",HCOFF_SECTION_RELOCATION_R_TYPE_RISCV_BRANCH,4,HCOFF_ARCH_RELOCATION_FLAG_PC_RELATIVE};
static const hcoff_arch_relocation_info_t riscv_jal= {"IMAGE_REL_RISCV_JAL",HCOFF_SECTION_RELOCATION_R_TYPE_RISCV_JAL,4,HCOFF_ARCH_RELOCATION_FLAG_PC_RELATIVE};
static const hcoff_arch_relocation_info_t riscv_high20= {"IMAGE_REL_RISCV_HIGH20",HCOFF_SECTION_RELOCATION_R_TYPE_RISCV_HIGH20,4,0};
static const hcoff_arch_relocation_info_t riscv_low12i= {"IMAGE_REL_RISCV_LOW12I",HCOFF_SECTION_RELOCATION_R_TYPE_RISCV_LOW12I,4,0};
static const hcoff_arch_relocation_info_t riscv_low12s= {"IMAGE_REL_RISCV_LOW12S",HCOFF_SECTION_RELOCATION_R_TYPE_RISCV_LOW12S,4,0};
static const hcoff_arch_relocation_info_t riscv_rel32= {"IMAGE_REL_RISCV_REL32",HCOFF_SECTION_RELOCATION_R_TYPE_RISCV_REL32,4,HCOFF_ARCH_RELOCATION_FLAG_PC_RELATIVE};

const hcoff_riscv_section_relocation_info_t * hcoff_riscv_section_relocation_info_get(const hcoff_riscv_section_relocation_t *relocation)
{
    if(relocation==NULL)
    {
        return &riscv_absolute;
    }
    switch(relocation->r_type)
    {
    case HCOFF_SECTION_RELOCATION_R_TYPE_RISCV_ABSOLUTE:
        return &riscv_absolute;
    case HCOFF_SECTION_RELOCATION_R_TYPE_RISCV_ADDR32:
        return &riscv_addr32;
    case HCOFF_SECTION_RELOCATION_R_TYPE_RISCV_ADDR32NB:
        return &riscv_addr32nb;
    case HCOFF_SECTION_RELOCATION_R_TYPE_RISCV_BRANCH:
        return &riscv_branch;
    case HCOFF_SECTION_RELOCATION_R_TYPE_RISCV_JAL:
        return &riscv_jal;
    case HCOFF_SECTION_RELOCATION_R_TYPE_RISCV_HIGH20:
        return &riscv_high20;
    case HCOFF_SECTION_RELOCATION_R_TYPE_RISCV_LOW12I:
        return &riscv_low12i;
    case HCOFF_SECTION_RELOCATION_R_TYPE_RISCV_LOW12S:
        return &riscv_low12s;
    case HCOFF_SECTION_RELOCATION_R_TYPE_RISCV_REL32:
        return &riscv_rel32;
    default:
        break;
    }
    return NULL;
}

static const hcoff_arch_relocation_info_t *hcoff_riscv_arch_relocation_info_get(uint16_t type)
{
    hcoff_section_relocation_t relocation= {0};
    relocation.r_type=type;
    return hcoff_riscv_section_relocation_info_get(&relocation);
}

bool hcoff_riscv_section_relocation_doit(uint8_t *relocation_addr,uint64_t symbol_addr,uintptr_t pc_address,uintptr_t image_base,const hcoff_riscv_section_relocation_t *relocation)
{
    if(relocation_addr==NULL || relocation==NULL)
    {
        return false;
    }
    switch(relocation->r_type)
    {
    case HCOFF_SECTION_RELOCATION_R_TYPE_RISCV_ABSOLUTE:
        return true;
    case HCOFF_SECTION_RELOCATION_R_TYPE_RISCV_ADDR32:
        hcoff_put_uint32_le(relocation_addr,(uint32_t)symbol_addr);
        return true;
    case HCOFF_SECTION_RELOCATION_R_TYPE_RISCV_ADDR32NB:
        hcoff_put_uint32_le(relocation_addr,(uint32_t)(symbol_addr-(uint64_t)image_base));
        return true;
    case HCOFF_SECTION_RELOCATION_R_TYPE_RISCV_BRANCH:
    {
        int64_t diff=(int64_t)symbol_addr-(int64_t)pc_address;
        uint32_t imm=(uint32_t)diff;
        uint32_t inst=hcoff_get_uint32_le(relocation_addr);
        inst=(inst & 0x01FFF07FU) |
             (((imm >> 12) & 0x1) << 31) |
             (((imm >> 5) & 0x3F) << 25) |
             (((imm >> 1) & 0xF) << 8) |
             (((imm >> 11) & 0x1) << 7);
        hcoff_put_uint32_le(relocation_addr,inst);
        return true;
    }
    case HCOFF_SECTION_RELOCATION_R_TYPE_RISCV_JAL:
    {
        int64_t diff=(int64_t)symbol_addr-(int64_t)pc_address;
        uint32_t imm=(uint32_t)diff;
        uint32_t inst=hcoff_get_uint32_le(relocation_addr);
        inst=(inst & 0x00000FFFU) |
             (((imm >> 20) & 0x1) << 31) |
             (((imm >> 1) & 0x3FF) << 21) |
             (((imm >> 11) & 0x1) << 20) |
             (((imm >> 12) & 0xFF) << 12);
        hcoff_put_uint32_le(relocation_addr,inst);
        return true;
    }
    case HCOFF_SECTION_RELOCATION_R_TYPE_RISCV_HIGH20:
    {
        uint32_t value=(uint32_t)(symbol_addr+0x800);
        uint32_t inst=hcoff_get_uint32_le(relocation_addr);
        inst=(inst & 0x00000FFFU) | ((value & 0xFFFFF000U));
        hcoff_put_uint32_le(relocation_addr,inst);
        return true;
    }
    case HCOFF_SECTION_RELOCATION_R_TYPE_RISCV_LOW12I:
    {
        uint32_t inst=hcoff_get_uint32_le(relocation_addr);
        inst=(inst & 0x000FFFFFU) | (((uint32_t)symbol_addr & 0xFFFU) << 20);
        hcoff_put_uint32_le(relocation_addr,inst);
        return true;
    }
    case HCOFF_SECTION_RELOCATION_R_TYPE_RISCV_LOW12S:
    {
        uint32_t imm=(uint32_t)symbol_addr & 0xFFFU;
        uint32_t inst=hcoff_get_uint32_le(relocation_addr);
        inst=(inst & 0x01FFF07FU) | (((imm >> 0) & 0x1F) << 7) | (((imm >> 5) & 0x7F) << 25);
        hcoff_put_uint32_le(relocation_addr,inst);
        return true;
    }
    case HCOFF_SECTION_RELOCATION_R_TYPE_RISCV_REL32:
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

static bool hcoff_riscv_arch_relocation_doit(uint8_t *relocation_addr,uint64_t symbol_addr,uintptr_t pc_address,uintptr_t image_base,uint16_t type)
{
    hcoff_section_relocation_t relocation= {0};
    relocation.r_type=type;
    return hcoff_riscv_section_relocation_doit(relocation_addr,symbol_addr,pc_address,image_base,&relocation);
}

const hcoff_arch_ops_t hcoff_riscv_arch_ops=
{
    HCOFF_FILEHEADER_F_MAGIC_RISCV64,
    "riscv",
    true,
    14,
    hcoff_riscv_arch_relocation_doit,
    hcoff_riscv_arch_relocation_info_get
};

static bool hcoff_riscv_relocatable_machine_get(hcoff_file_input_t *input_file,uint16_t *machine)
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
    if(fileheader.f_magic!=HCOFF_FILEHEADER_F_MAGIC_RISCV32 &&
            fileheader.f_magic!=HCOFF_FILEHEADER_F_MAGIC_RISCV64)
    {
        return false;
    }
    *machine=fileheader.f_magic;
    return true;
}

size_t hcoff_riscv_relocatable_image_size_get(hcoff_file_input_t *input_file)
{
    uint16_t machine=HCOFF_FILEHEADER_F_MAGIC_UNKNOWN;
    if(!hcoff_riscv_relocatable_machine_get(input_file,&machine))
    {
        return 0;
    }
    return hcoff_relocatable_image_size_get(input_file,machine);
}

bool hcoff_riscv_relocatable_init(hcoff_riscv_relocatable_t *riscv_relocatable,hcoff_file_input_t *input_file,void *buffer,size_t buffer_size,hcoff_riscv_external_symbol_find_with_usr_t extern_symbol,void *usr)
{
    uint16_t machine=HCOFF_FILEHEADER_F_MAGIC_UNKNOWN;
    if(!hcoff_riscv_relocatable_machine_get(input_file,&machine))
    {
        return false;
    }
    return hcoff_relocatable_init(riscv_relocatable,input_file,machine,buffer,buffer_size,(hcoff_relocatable_external_symbol_find_t)extern_symbol,usr);
}

void hcoff_riscv_relocatable_deinit(hcoff_riscv_relocatable_t * riscv_relocatable)
{
    hcoff_relocatable_deinit(riscv_relocatable);
}

static const struct hruntime_symbol *hcoff_riscv_external_symbol_find_with_usr_adapter(const char *name,void *usr)
{
    hcoff_riscv_external_symbol_find_t extern_symbol=(hcoff_riscv_external_symbol_find_t)usr;
    if(extern_symbol!=NULL)
    {
        return extern_symbol(name);
    }
    return NULL;
}

const hcoff_riscv_relocatable_t *hcoff_riscv_relocatable_new(hcoff_file_input_t *input_file,hcoff_riscv_external_symbol_find_t extern_symbol)
{
    return hcoff_riscv_relocatable_new_with_usr(input_file,hcoff_riscv_external_symbol_find_with_usr_adapter,(void *)extern_symbol);
}

const hcoff_riscv_relocatable_t *hcoff_riscv_relocatable_new_with_usr(hcoff_file_input_t *input_file,hcoff_riscv_external_symbol_find_with_usr_t extern_symbol,void *usr)
{
    uint16_t machine=HCOFF_FILEHEADER_F_MAGIC_UNKNOWN;
    if(!hcoff_riscv_relocatable_machine_get(input_file,&machine))
    {
        return NULL;
    }
    return hcoff_relocatable_new_with_usr(input_file,machine,(hcoff_relocatable_external_symbol_find_t)extern_symbol,usr);
}

void hcoff_riscv_relocatable_delete(const hcoff_riscv_relocatable_t * riscv_relocatable)
{
    hcoff_relocatable_delete(riscv_relocatable);
}
