/***************************************************************
 * Name:      hcoff_arch_arm.c
 * Purpose:   实现hcoff_arm接口(ARM/Thumb COFF)
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-11-20
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hcoff_arch_arm.h"
#include "string.h"

static const hcoff_arch_relocation_info_t arm_absolute=
{
    "IMAGE_REL_ARM_ABSOLUTE",
    HCOFF_SECTION_RELOCATION_R_TYPE_ARM_ABSOLUTE,
    0,
    HCOFF_ARCH_RELOCATION_FLAG_IGNORED
};

static const hcoff_arch_relocation_info_t arm_addr32=
{
    "IMAGE_REL_ARM_ADDR32",
    HCOFF_SECTION_RELOCATION_R_TYPE_ARM_ADDR32,
    4,
    0
};

static const hcoff_arch_relocation_info_t arm_addr32nb=
{
    "IMAGE_REL_ARM_ADDR32NB",
    HCOFF_SECTION_RELOCATION_R_TYPE_ARM_ADDR32NB,
    4,
    0
};

static const hcoff_arch_relocation_info_t arm_branch24=
{
    "IMAGE_REL_ARM_BRANCH24",
    HCOFF_SECTION_RELOCATION_R_TYPE_ARM_BRANCH24,
    4,
    HCOFF_ARCH_RELOCATION_FLAG_PC_RELATIVE
};

static const hcoff_arch_relocation_info_t arm_branch11=
{
    "IMAGE_REL_ARM_BRANCH11",
    HCOFF_SECTION_RELOCATION_R_TYPE_ARM_BRANCH11,
    4,
    HCOFF_ARCH_RELOCATION_FLAG_PC_RELATIVE
};

static const hcoff_arch_relocation_info_t arm_token=
{
    "IMAGE_REL_ARM_TOKEN",
    HCOFF_SECTION_RELOCATION_R_TYPE_ARM_TOKEN,
    4,
    0
};

static const hcoff_arch_relocation_info_t arm_blx24=
{
    "IMAGE_REL_ARM_BLX24",
    HCOFF_SECTION_RELOCATION_R_TYPE_ARM_BLX24,
    4,
    HCOFF_ARCH_RELOCATION_FLAG_PC_RELATIVE
};

static const hcoff_arch_relocation_info_t arm_blx11=
{
    "IMAGE_REL_ARM_BLX11",
    HCOFF_SECTION_RELOCATION_R_TYPE_ARM_BLX11,
    4,
    HCOFF_ARCH_RELOCATION_FLAG_PC_RELATIVE
};

static const hcoff_arch_relocation_info_t arm_rel32=
{
    "IMAGE_REL_ARM_REL32",
    HCOFF_SECTION_RELOCATION_R_TYPE_ARM_REL32,
    4,
    HCOFF_ARCH_RELOCATION_FLAG_PC_RELATIVE
};

static const hcoff_arch_relocation_info_t arm_section=
{
    "IMAGE_REL_ARM_SECTION",
    HCOFF_SECTION_RELOCATION_R_TYPE_ARM_SECTION,
    2,
    0
};

static const hcoff_arch_relocation_info_t arm_secrel=
{
    "IMAGE_REL_ARM_SECREL",
    HCOFF_SECTION_RELOCATION_R_TYPE_ARM_SECREL,
    4,
    0
};

static const hcoff_arch_relocation_info_t arm_mov32=
{
    "IMAGE_REL_ARM_MOV32",
    HCOFF_SECTION_RELOCATION_R_TYPE_ARM_MOV32,
    8,
    0
};

static const hcoff_arch_relocation_info_t arm_thumb_mov32=
{
    "IMAGE_REL_ARM_THUMB_MOV32",
    HCOFF_SECTION_RELOCATION_R_TYPE_ARM_THUMB_MOV32,
    8,
    0
};

const hcoff_arm_section_relocation_info_t * hcoff_arm_section_relocation_info_get(const hcoff_arm_section_relocation_t *relocation)
{
    if(relocation==NULL)
    {
        return &arm_absolute;
    }
    switch(relocation->r_type)
    {
    case HCOFF_SECTION_RELOCATION_R_TYPE_ARM_ABSOLUTE:
        return &arm_absolute;
    case HCOFF_SECTION_RELOCATION_R_TYPE_ARM_ADDR32:
        return &arm_addr32;
    case HCOFF_SECTION_RELOCATION_R_TYPE_ARM_ADDR32NB:
        return &arm_addr32nb;
    case HCOFF_SECTION_RELOCATION_R_TYPE_ARM_BRANCH24:
        return &arm_branch24;
    case HCOFF_SECTION_RELOCATION_R_TYPE_ARM_BRANCH11:
        return &arm_branch11;
    case HCOFF_SECTION_RELOCATION_R_TYPE_ARM_TOKEN:
        return &arm_token;
    case HCOFF_SECTION_RELOCATION_R_TYPE_ARM_BLX24:
        return &arm_blx24;
    case HCOFF_SECTION_RELOCATION_R_TYPE_ARM_BLX11:
        return &arm_blx11;
    case HCOFF_SECTION_RELOCATION_R_TYPE_ARM_REL32:
        return &arm_rel32;
    case HCOFF_SECTION_RELOCATION_R_TYPE_ARM_SECTION:
        return &arm_section;
    case HCOFF_SECTION_RELOCATION_R_TYPE_ARM_SECREL:
        return &arm_secrel;
    case HCOFF_SECTION_RELOCATION_R_TYPE_ARM_MOV32:
        return &arm_mov32;
    case HCOFF_SECTION_RELOCATION_R_TYPE_ARM_THUMB_MOV32:
        return &arm_thumb_mov32;
    default:
        break;
    }
    return NULL;
}

static const hcoff_arch_relocation_info_t *hcoff_arm_arch_relocation_info_get(uint16_t type)
{
    hcoff_section_relocation_t relocation= {0};
    relocation.r_type=type;
    return hcoff_arm_section_relocation_info_get(&relocation);
}

static void hcoff_arm_write_uint32(uint8_t *addr,uint32_t value,bool is_big_endian)
{
    if(is_big_endian)
    {
        hcoff_put_uint32(addr,value,true);
    }
    else
    {
        hcoff_put_uint32_le(addr,value);
    }
}

static uint32_t hcoff_arm_read_uint32(const uint8_t *addr,bool is_big_endian)
{
    return is_big_endian?hcoff_get_uint32_be(addr):hcoff_get_uint32_le(addr);
}

static void hcoff_arm_apply_mov32(uint8_t *addr,uint32_t value,bool is_thumb)
{
    uint32_t insn1=hcoff_get_uint32_le(addr);
    uint32_t insn2=hcoff_get_uint32_le(addr+4);
    if(is_thumb)
    {
        //Thumb-2 MOVW/MOVT
        insn1=(insn1 & 0x8F00FBF0U) | ((value & 0xF) << 0) | (((value >> 4) & 0x7) << 12) | (((value >> 11) & 0x1) << 26);
        insn2=(insn2 & 0x8F00FBF0U) | (((value >> 16) & 0xF) << 0) | (((value >> 20) & 0x7) << 12) | (((value >> 27) & 0x1) << 26);
    }
    else
    {
        //ARM MOVW/MOVT
        insn1=(insn1 & 0xFFF0F000U) | ((value & 0xFFFF) & 0x0FFF) | (((value >> 12) & 0xFFFF) << 16);
        insn2=(insn2 & 0xFFF0F000U) | (((value >> 16) & 0xFFFF) & 0x0FFF) | (((value >> 28) & 0xFFFF) << 16);
    }
    hcoff_put_uint32_le(addr,insn1);
    hcoff_put_uint32_le(addr+4,insn2);
}

bool hcoff_arm_section_relocation_doit(uint8_t *relocation_addr,uint64_t symbol_addr,uintptr_t pc_address,uintptr_t image_base,const hcoff_arm_section_relocation_t *relocation)
{
    if(relocation_addr==NULL || relocation==NULL)
    {
        return false;
    }
    switch(relocation->r_type)
    {
    case HCOFF_SECTION_RELOCATION_R_TYPE_ARM_ABSOLUTE:
        return true;
    case HCOFF_SECTION_RELOCATION_R_TYPE_ARM_ADDR32:
        hcoff_arm_write_uint32(relocation_addr,(uint32_t)symbol_addr,false);
        return true;
    case HCOFF_SECTION_RELOCATION_R_TYPE_ARM_ADDR32NB:
        hcoff_arm_write_uint32(relocation_addr,(uint32_t)(symbol_addr-(uint64_t)image_base),false);
        return true;
    case HCOFF_SECTION_RELOCATION_R_TYPE_ARM_BRANCH24:
    case HCOFF_SECTION_RELOCATION_R_TYPE_ARM_BLX24:
    {
        int64_t diff=(int64_t)symbol_addr-(int64_t)(pc_address+8);
        uint32_t insn=hcoff_arm_read_uint32(relocation_addr,false);
        insn=(insn & 0xFF000000U) | (((uint32_t)(diff>>2)) & 0x00FFFFFFU);
        hcoff_arm_write_uint32(relocation_addr,insn,false);
        return true;
    }
    case HCOFF_SECTION_RELOCATION_R_TYPE_ARM_BRANCH11:
    case HCOFF_SECTION_RELOCATION_R_TYPE_ARM_BLX11:
    {
        int64_t diff=(int64_t)symbol_addr-(int64_t)(pc_address+4);
        uint32_t insn=hcoff_arm_read_uint32(relocation_addr,false);
        uint32_t imm=((uint32_t)(diff>>1)) & 0x7FFU;
        insn=(insn & 0xFFFFF800U) | imm;
        hcoff_arm_write_uint32(relocation_addr,insn,false);
        return true;
    }
    case HCOFF_SECTION_RELOCATION_R_TYPE_ARM_TOKEN:
        hcoff_arm_write_uint32(relocation_addr,(uint32_t)symbol_addr,false);
        return true;
    case HCOFF_SECTION_RELOCATION_R_TYPE_ARM_REL32:
    {
        int64_t diff=(int64_t)symbol_addr-(int64_t)(pc_address+4);
        hcoff_arm_write_uint32(relocation_addr,(uint32_t)diff,false);
        return true;
    }
    case HCOFF_SECTION_RELOCATION_R_TYPE_ARM_MOV32:
        hcoff_arm_apply_mov32(relocation_addr,(uint32_t)symbol_addr,false);
        return true;
    case HCOFF_SECTION_RELOCATION_R_TYPE_ARM_THUMB_MOV32:
        hcoff_arm_apply_mov32(relocation_addr,(uint32_t)symbol_addr,true);
        return true;
    case HCOFF_SECTION_RELOCATION_R_TYPE_ARM_SECTION:
    case HCOFF_SECTION_RELOCATION_R_TYPE_ARM_SECREL:
    default:
        break;
    }
    return false;
}

static bool hcoff_arm_arch_relocation_doit(uint8_t *relocation_addr,uint64_t symbol_addr,uintptr_t pc_address,uintptr_t image_base,uint16_t type)
{
    hcoff_section_relocation_t relocation= {0};
    relocation.r_type=type;
    return hcoff_arm_section_relocation_doit(relocation_addr,symbol_addr,pc_address,image_base,&relocation);
}

const hcoff_arch_ops_t hcoff_arm_arch_ops=
{
    HCOFF_FILEHEADER_F_MAGIC_ARM,
    "arm",
    false,
    10,
    hcoff_arm_arch_relocation_doit,
    hcoff_arm_arch_relocation_info_get
};

static bool hcoff_arm_relocatable_machine_get(hcoff_file_input_t *input_file,uint16_t *machine)
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
    if(fileheader.f_magic!=HCOFF_FILEHEADER_F_MAGIC_ARM &&
            fileheader.f_magic!=HCOFF_FILEHEADER_F_MAGIC_ARMNT &&
            fileheader.f_magic!=HCOFF_FILEHEADER_F_MAGIC_THUMB)
    {
        return false;
    }
    *machine=fileheader.f_magic;
    return true;
}

size_t hcoff_arm_relocatable_image_size_get(hcoff_file_input_t *input_file)
{
    uint16_t machine=HCOFF_FILEHEADER_F_MAGIC_UNKNOWN;
    if(!hcoff_arm_relocatable_machine_get(input_file,&machine))
    {
        return 0;
    }
    return hcoff_relocatable_image_size_get(input_file,machine);
}

bool hcoff_arm_relocatable_init(hcoff_arm_relocatable_t *arm_relocatable,hcoff_file_input_t *input_file,void *buffer,size_t buffer_size,hcoff_arm_external_symbol_find_with_usr_t extern_symbol,void *usr)
{
    uint16_t machine=HCOFF_FILEHEADER_F_MAGIC_UNKNOWN;
    if(!hcoff_arm_relocatable_machine_get(input_file,&machine))
    {
        return false;
    }
    return hcoff_relocatable_init(arm_relocatable,input_file,machine,buffer,buffer_size,(hcoff_relocatable_external_symbol_find_t)extern_symbol,usr);
}

void hcoff_arm_relocatable_deinit(hcoff_arm_relocatable_t * arm_relocatable)
{
    hcoff_relocatable_deinit(arm_relocatable);
}

static const struct hruntime_symbol *hcoff_arm_external_symbol_find_with_usr_adapter(const char *name,void *usr)
{
    hcoff_arm_external_symbol_find_t extern_symbol=(hcoff_arm_external_symbol_find_t)usr;
    if(extern_symbol!=NULL)
    {
        return extern_symbol(name);
    }
    return NULL;
}

const hcoff_arm_relocatable_t *hcoff_arm_relocatable_new(hcoff_file_input_t *input_file,hcoff_arm_external_symbol_find_t extern_symbol)
{
    return hcoff_arm_relocatable_new_with_usr(input_file,hcoff_arm_external_symbol_find_with_usr_adapter,(void *)extern_symbol);
}

const hcoff_arm_relocatable_t *hcoff_arm_relocatable_new_with_usr(hcoff_file_input_t *input_file,hcoff_arm_external_symbol_find_with_usr_t extern_symbol,void *usr)
{
    uint16_t machine=HCOFF_FILEHEADER_F_MAGIC_UNKNOWN;
    if(!hcoff_arm_relocatable_machine_get(input_file,&machine))
    {
        return NULL;
    }
    return hcoff_relocatable_new_with_usr(input_file,machine,(hcoff_relocatable_external_symbol_find_t)extern_symbol,usr);
}

void hcoff_arm_relocatable_delete(const hcoff_arm_relocatable_t * arm_relocatable)
{
    hcoff_relocatable_delete(arm_relocatable);
}
