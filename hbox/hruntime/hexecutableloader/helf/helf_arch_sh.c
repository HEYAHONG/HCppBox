/***************************************************************
 * Name:      helf_arch_sh.c
 * Purpose:   实现helf_arch_sh接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "helf_arch_sh.h"

static const uint16_t helf_sh_machines[]=
{
    HELF_EM_SH,
};

bool helf_sh_is_sh(helf_file_input_t *input_file)
{
    return helf_arch_is(input_file,helf_sh_machines,sizeof(helf_sh_machines)/sizeof(helf_sh_machines[0]),HELF_ELFCLASS_32);
}

bool helf_sh_is_big_endian(helf_file_input_t *input_file)
{
    if(!helf_sh_is_sh(input_file))
    {
        return false;
    }
    return helf_file_input_is_big_endian(input_file);
}

bool helf_sh_is_executable(helf_file_input_t *input_file)
{
    return helf_arch_is_executable(input_file,helf_sh_machines,sizeof(helf_sh_machines)/sizeof(helf_sh_machines[0]),HELF_ELFCLASS_32);
}

bool helf_sh_entry_get(helf_file_input_t *input_file,uint32_t *entry)
{
    if(entry==NULL)
    {
        return false;
    }
    if(!helf_sh_is_sh(input_file))
    {
        return false;
    }
    uint64_t elf_entry=0;
    if(!helf_arch_entry_get(input_file,&elf_entry))
    {
        return false;
    }
    *entry=(uint32_t)elf_entry;
    return true;
}

size_t helf_sh_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_sh_is_sh(input_file))
    {
        return 0;
    }
    return helf_arch_section_count_get(input_file);
}

bool helf_sh_section_info_get(helf_file_input_t *input_file,size_t index,helf_sh_section_info_t *info)
{
    if(!helf_sh_is_sh(input_file))
    {
        return false;
    }
    return helf_arch_section_info_get(input_file,index,info);
}

bool helf_sh_is_rx_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_sh_is_sh(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_section(index,input_file);
}

intptr_t helf_sh_rx_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_sh_is_rx_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rx_section_offset_get(index,input_file);
}

size_t helf_sh_rx_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_sh_is_sh(input_file))
    {
        return 0;
    }
    return helf_arch_rx_section_size_get(input_file);
}

bool helf_sh_is_ro_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_sh_is_sh(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_section(index,input_file);
}

intptr_t helf_sh_ro_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_sh_is_ro_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_ro_section_offset_get(index,input_file);
}

size_t helf_sh_ro_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_sh_is_sh(input_file))
    {
        return 0;
    }
    return helf_arch_ro_section_size_get(input_file);
}

bool helf_sh_is_rw_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_sh_is_sh(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_section(index,input_file);
}

intptr_t helf_sh_rw_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_sh_is_rw_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rw_section_offset_get(index,input_file);
}

size_t helf_sh_rw_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_sh_is_sh(input_file))
    {
        return 0;
    }
    return helf_arch_rw_section_size_get(input_file);
}

bool helf_sh_is_bss_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_sh_is_sh(input_file))
    {
        return false;
    }
    return helf_arch_is_bss_section(index,input_file);
}

size_t helf_sh_program_header_count_get(helf_file_input_t *input_file)
{
    if(!helf_sh_is_sh(input_file))
    {
        return 0;
    }
    return helf_arch_program_header_count_get(input_file);
}

bool helf_sh_program_header_get(helf_file_input_t *input_file,size_t index,helf_elf32_program_header_t *programheader)
{
    if(programheader==NULL)
    {
        return false;
    }
    if(!helf_sh_is_sh(input_file))
    {
        return false;
    }
    if(index>=helf_arch_program_header_count_get(input_file))
    {
        return false;
    }
    return helf_file_input_32_bits_program_header_get(input_file,index,programheader);
}

size_t helf_sh_segment_count_get(helf_file_input_t *input_file)
{
    return helf_sh_program_header_count_get(input_file);
}

bool helf_sh_segment_info_get(helf_file_input_t *input_file,size_t index,helf_sh_segment_info_t *info)
{
    if(!helf_sh_is_sh(input_file))
    {
        return false;
    }
    return helf_arch_segment_info_get(input_file,index,info);
}

bool helf_sh_is_load_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_sh_is_sh(input_file))
    {
        return false;
    }
    return helf_arch_is_load_segment(index,input_file);
}

bool helf_sh_is_rx_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_sh_is_sh(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_segment(index,input_file);
}

bool helf_sh_is_ro_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_sh_is_sh(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_segment(index,input_file);
}

bool helf_sh_is_rw_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_sh_is_sh(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_segment(index,input_file);
}

size_t helf_sh_load_segment_count_get(helf_file_input_t *input_file)
{
    if(!helf_sh_is_sh(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_count_get(input_file);
}

bool helf_sh_load_segment_info_get(helf_file_input_t *input_file,size_t index,helf_sh_segment_info_t *info)
{
    if(!helf_sh_is_sh(input_file))
    {
        return false;
    }
    return helf_arch_load_segment_info_get(input_file,index,info);
}

size_t helf_sh_load_segment_total_filesz_get(helf_file_input_t *input_file)
{
    if(!helf_sh_is_sh(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_filesz_get(input_file);
}

size_t helf_sh_load_segment_total_memsz_get(helf_file_input_t *input_file)
{
    if(!helf_sh_is_sh(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_memsz_get(input_file);
}

size_t helf_sh_relocation_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_sh_is_sh(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_section_count_get(input_file);
}

bool helf_sh_relocation_section_info_get(helf_file_input_t *input_file,size_t index,helf_arch_relocation_section_info_t *info)
{
    if(!helf_sh_is_sh(input_file))
    {
        return false;
    }
    return helf_arch_relocation_section_info_get(input_file,index,info);
}

size_t helf_sh_relocation_count_get(helf_file_input_t *input_file,size_t section_index)
{
    if(!helf_sh_is_sh(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_count_get(input_file,section_index);
}

bool helf_sh_relocation_get(helf_file_input_t *input_file,size_t section_index,size_t index,helf_arch_relocation_info_t *info)
{
    if(!helf_sh_is_sh(input_file))
    {
        return false;
    }
    if(!helf_arch_relocation_get(input_file,section_index,index,info))
    {
        return false;
    }
    return true;
}

bool helf_sh_relocation_symbol_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_symbol_info_t *info)
{
    if(!helf_sh_is_sh(input_file))
    {
        return false;
    }
    return helf_arch_relocation_symbol_info_get(input_file,relocation,info);
}

static const helf_arch_relocation_type_name_t helf_sh_relocation_type_names[]=
{
    {0,0,"R_SH_NONE"},
    {1,4,"R_SH_DIR32"},
    {2,4,"R_SH_REL32"},
    {3,0,"R_SH_DIR8WPN"},
    {4,0,"R_SH_IND12W"},
    {5,0,"R_SH_DIR8WPL"},
    {6,0,"R_SH_DIR8WPZ"},
    {7,0,"R_SH_DIR8BP"},
    {8,0,"R_SH_DIR8W"},
    {9,0,"R_SH_DIR8L"},
    {10,0,"R_SH_LOOP_START"},
    {11,0,"R_SH_LOOP_END"},
    {22,0,"R_SH_GNU_VTINHERIT"},
    {23,0,"R_SH_GNU_VTENTRY"},
    {24,1,"R_SH_SWITCH8"},
    {25,2,"R_SH_SWITCH16"},
    {26,4,"R_SH_SWITCH32"},
    {27,0,"R_SH_USES"},
    {28,0,"R_SH_COUNT"},
    {29,0,"R_SH_ALIGN"},
    {30,0,"R_SH_CODE"},
    {31,0,"R_SH_DATA"},
    {32,0,"R_SH_LABEL"},
    {33,2,"R_SH_DIR16"},
    {34,1,"R_SH_DIR8"},
    {35,0,"R_SH_DIR8UL"},
    {36,0,"R_SH_DIR8UW"},
    {37,0,"R_SH_DIR8U"},
    {38,0,"R_SH_DIR8SW"},
    {39,0,"R_SH_DIR8S"},
    {40,0,"R_SH_DIR4UL"},
    {41,0,"R_SH_DIR4UW"},
    {42,0,"R_SH_DIR4U"},
    {43,0,"R_SH_PSHA"},
    {44,0,"R_SH_PSHL"},
    {45,0,"R_SH_DIR5U"},
    {46,0,"R_SH_DIR6U"},
    {47,0,"R_SH_DIR6S"},
    {48,0,"R_SH_DIR10S"},
    {49,0,"R_SH_DIR10SW"},
    {50,0,"R_SH_DIR10SL"},
    {51,0,"R_SH_DIR10SQ"},
    {53,0,"R_SH_DIR16S"},
    {144,4,"R_SH_TLS_GD_32"},
    {145,4,"R_SH_TLS_LD_32"},
    {146,4,"R_SH_TLS_LDO_32"},
    {147,4,"R_SH_TLS_IE_32"},
    {148,4,"R_SH_TLS_LE_32"},
    {149,4,"R_SH_TLS_DTPMOD32"},
    {150,4,"R_SH_TLS_DTPOFF32"},
    {151,4,"R_SH_TLS_TPOFF32"},
    {160,4,"R_SH_GOT32"},
    {161,4,"R_SH_PLT32"},
    {162,0,"R_SH_COPY"},
    {163,0,"R_SH_GLOB_DAT"},
    {164,0,"R_SH_JMP_SLOT"},
    {165,0,"R_SH_RELATIVE"},
    {166,0,"R_SH_GOTOFF"},
    {167,0,"R_SH_GOTPC"},
    {168,4,"R_SH_GOTPLT32"},
    {169,2,"R_SH_GOT_LOW16"},
    {170,2,"R_SH_GOT_MEDLOW16"},
    {171,2,"R_SH_GOT_MEDHI16"},
    {172,2,"R_SH_GOT_HI16"},
    {173,2,"R_SH_GOTPLT_LOW16"},
    {174,2,"R_SH_GOTPLT_MEDLOW16"},
    {175,2,"R_SH_GOTPLT_MEDHI16"},
    {176,2,"R_SH_GOTPLT_HI16"},
    {177,2,"R_SH_PLT_LOW16"},
    {178,2,"R_SH_PLT_MEDLOW16"},
    {179,2,"R_SH_PLT_MEDHI16"},
    {180,2,"R_SH_PLT_HI16"},
    {181,2,"R_SH_GOTOFF_LOW16"},
    {182,2,"R_SH_GOTOFF_MEDLOW16"},
    {183,2,"R_SH_GOTOFF_MEDHI16"},
    {184,2,"R_SH_GOTOFF_HI16"},
    {185,2,"R_SH_GOTPC_LOW16"},
    {186,2,"R_SH_GOTPC_MEDLOW16"},
    {187,2,"R_SH_GOTPC_MEDHI16"},
    {188,2,"R_SH_GOTPC_HI16"},
    {189,0,"R_SH_GOT10BY4"},
    {190,0,"R_SH_GOTPLT10BY4"},
    {191,1,"R_SH_GOT10BY8"},
    {192,1,"R_SH_GOTPLT10BY8"},
    {193,8,"R_SH_COPY64"},
    {194,8,"R_SH_GLOB_DAT64"},
    {195,8,"R_SH_JMP_SLOT64"},
    {196,8,"R_SH_RELATIVE64"},
    {201,0,"R_SH_GOT20"},
    {202,0,"R_SH_GOTOFF20"},
    {203,0,"R_SH_GOTFUNCDESC"},
    {204,0,"R_SH_GOTFUNCDESC20"},
    {205,0,"R_SH_GOTOFFFUNCDESC"},
    {206,0,"R_SH_GOTOFFFUNCDESC20"},
    {207,0,"R_SH_FUNCDESC"},
    {208,0,"R_SH_FUNCDESC_VALUE"},
    {242,0,"R_SH_SHMEDIA_CODE"},
    {243,2,"R_SH_PT_16"},
    {244,2,"R_SH_IMMS16"},
    {245,2,"R_SH_IMMU16"},
    {246,2,"R_SH_IMM_LOW16"},
    {247,0,"R_SH_IMM_LOW16_PCREL"},
    {248,2,"R_SH_IMM_MEDLOW16"},
    {249,0,"R_SH_IMM_MEDLOW16_PCREL"},
    {250,2,"R_SH_IMM_MEDHI16"},
    {251,0,"R_SH_IMM_MEDHI16_PCREL"},
    {252,2,"R_SH_IMM_HI16"},
    {253,0,"R_SH_IMM_HI16_PCREL"},
    {254,8,"R_SH_64"},
    {255,0,"R_SH_64_PCREL"},
};

const char *helf_sh_relocation_type_name_get(uint32_t type)
{
    return helf_arch_relocation_type_name_get(helf_sh_relocation_type_names,sizeof(helf_sh_relocation_type_names)/sizeof(helf_sh_relocation_type_names[0]),type);
}

uint8_t helf_sh_relocation_size_get(uint32_t type)
{
    return helf_arch_relocation_type_size_get(helf_sh_relocation_type_names,sizeof(helf_sh_relocation_type_names)/sizeof(helf_sh_relocation_type_names[0]),type);
}

const char *helf_sh_section_type_name_get(uint32_t type)
{
    (void)type;
    return NULL;
}

bool helf_sh_relocation_address_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_relocation_address_info_t *info)
{
    if(!helf_sh_is_sh(input_file) || relocation==NULL)
    {
        return false;
    }
    uint8_t size=helf_sh_relocation_size_get(relocation->type);
    if(size==0)
    {
        return false;
    }
    return helf_arch_relocation_address_info_get(input_file,relocation,size,info);
}

size_t helf_sh_relocation_data_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,void *buffer,size_t buffer_length)
{
    if(!helf_sh_is_sh(input_file) || relocation==NULL)
    {
        return 0;
    }
    uint8_t size=helf_sh_relocation_size_get(relocation->type);
    if(size==0)
    {
        return 0;
    }
    return helf_arch_relocation_data_get(input_file,relocation,size,buffer,buffer_length);
}

bool helf_sh_is_shared_object(helf_file_input_t *input_file)
{
    return helf_arch_is_shared_object(input_file,helf_sh_machines,sizeof(helf_sh_machines)/sizeof(helf_sh_machines[0]),HELF_ELFCLASS_32);
}

bool helf_sh_is_relocatable(helf_file_input_t *input_file)
{
    return helf_arch_is_relocatable(input_file,helf_sh_machines,sizeof(helf_sh_machines)/sizeof(helf_sh_machines[0]),HELF_ELFCLASS_32);
}

size_t helf_sh_dynamic_count_get(helf_file_input_t *input_file)
{
    if(!helf_sh_is_sh(input_file))
    {
        return 0;
    }
    return helf_arch_dynamic_count_get(input_file);
}

bool helf_sh_dynamic_info_get(helf_file_input_t *input_file,size_t index,helf_arch_dynamic_info_t *info)
{
    if(!helf_sh_is_sh(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_info_get(input_file,index,info);
}

bool helf_sh_dynamic_string_get(helf_file_input_t *input_file,uint64_t offset,char *buffer,size_t buffer_length)
{
    if(!helf_sh_is_sh(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_string_get(input_file,offset,buffer,buffer_length);
}
