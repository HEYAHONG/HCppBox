/***************************************************************
 * Name:      helf_arch_or1k.c
 * Purpose:   实现helf_arch_or1k接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "helf_arch_or1k.h"

static const uint16_t helf_or1k_machines[]=
{
    HELF_EM_OR1K,
    HELF_EM_OPENRISC,
};

bool helf_or1k_is_or1k(helf_file_input_t *input_file)
{
    return helf_arch_is(input_file,helf_or1k_machines,sizeof(helf_or1k_machines)/sizeof(helf_or1k_machines[0]),HELF_ELFCLASS_32);
}

bool helf_or1k_is_big_endian(helf_file_input_t *input_file)
{
    if(!helf_or1k_is_or1k(input_file))
    {
        return false;
    }
    return helf_file_input_is_big_endian(input_file);
}

bool helf_or1k_is_executable(helf_file_input_t *input_file)
{
    return helf_arch_is_executable(input_file,helf_or1k_machines,sizeof(helf_or1k_machines)/sizeof(helf_or1k_machines[0]),HELF_ELFCLASS_32);
}

bool helf_or1k_entry_get(helf_file_input_t *input_file,uint32_t *entry)
{
    if(entry==NULL)
    {
        return false;
    }
    if(!helf_or1k_is_or1k(input_file))
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

size_t helf_or1k_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_or1k_is_or1k(input_file))
    {
        return 0;
    }
    return helf_arch_section_count_get(input_file);
}

bool helf_or1k_section_info_get(helf_file_input_t *input_file,size_t index,helf_or1k_section_info_t *info)
{
    if(!helf_or1k_is_or1k(input_file))
    {
        return false;
    }
    return helf_arch_section_info_get(input_file,index,info);
}

bool helf_or1k_is_rx_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_or1k_is_or1k(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_section(index,input_file);
}

intptr_t helf_or1k_rx_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_or1k_is_rx_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rx_section_offset_get(index,input_file);
}

size_t helf_or1k_rx_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_or1k_is_or1k(input_file))
    {
        return 0;
    }
    return helf_arch_rx_section_size_get(input_file);
}

bool helf_or1k_is_ro_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_or1k_is_or1k(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_section(index,input_file);
}

intptr_t helf_or1k_ro_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_or1k_is_ro_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_ro_section_offset_get(index,input_file);
}

size_t helf_or1k_ro_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_or1k_is_or1k(input_file))
    {
        return 0;
    }
    return helf_arch_ro_section_size_get(input_file);
}

bool helf_or1k_is_rw_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_or1k_is_or1k(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_section(index,input_file);
}

intptr_t helf_or1k_rw_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_or1k_is_rw_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rw_section_offset_get(index,input_file);
}

size_t helf_or1k_rw_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_or1k_is_or1k(input_file))
    {
        return 0;
    }
    return helf_arch_rw_section_size_get(input_file);
}

bool helf_or1k_is_bss_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_or1k_is_or1k(input_file))
    {
        return false;
    }
    return helf_arch_is_bss_section(index,input_file);
}

size_t helf_or1k_program_header_count_get(helf_file_input_t *input_file)
{
    if(!helf_or1k_is_or1k(input_file))
    {
        return 0;
    }
    return helf_arch_program_header_count_get(input_file);
}

bool helf_or1k_program_header_get(helf_file_input_t *input_file,size_t index,helf_elf32_program_header_t *programheader)
{
    if(programheader==NULL)
    {
        return false;
    }
    if(!helf_or1k_is_or1k(input_file))
    {
        return false;
    }
    if(index>=helf_arch_program_header_count_get(input_file))
    {
        return false;
    }
    return helf_file_input_32_bits_program_header_get(input_file,index,programheader);
}

size_t helf_or1k_segment_count_get(helf_file_input_t *input_file)
{
    return helf_or1k_program_header_count_get(input_file);
}

bool helf_or1k_segment_info_get(helf_file_input_t *input_file,size_t index,helf_or1k_segment_info_t *info)
{
    if(!helf_or1k_is_or1k(input_file))
    {
        return false;
    }
    return helf_arch_segment_info_get(input_file,index,info);
}

bool helf_or1k_is_load_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_or1k_is_or1k(input_file))
    {
        return false;
    }
    return helf_arch_is_load_segment(index,input_file);
}

bool helf_or1k_is_rx_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_or1k_is_or1k(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_segment(index,input_file);
}

bool helf_or1k_is_ro_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_or1k_is_or1k(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_segment(index,input_file);
}

bool helf_or1k_is_rw_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_or1k_is_or1k(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_segment(index,input_file);
}

size_t helf_or1k_load_segment_count_get(helf_file_input_t *input_file)
{
    if(!helf_or1k_is_or1k(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_count_get(input_file);
}

bool helf_or1k_load_segment_info_get(helf_file_input_t *input_file,size_t index,helf_or1k_segment_info_t *info)
{
    if(!helf_or1k_is_or1k(input_file))
    {
        return false;
    }
    return helf_arch_load_segment_info_get(input_file,index,info);
}

size_t helf_or1k_load_segment_total_filesz_get(helf_file_input_t *input_file)
{
    if(!helf_or1k_is_or1k(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_filesz_get(input_file);
}

size_t helf_or1k_load_segment_total_memsz_get(helf_file_input_t *input_file)
{
    if(!helf_or1k_is_or1k(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_memsz_get(input_file);
}

size_t helf_or1k_relocation_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_or1k_is_or1k(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_section_count_get(input_file);
}

bool helf_or1k_relocation_section_info_get(helf_file_input_t *input_file,size_t index,helf_arch_relocation_section_info_t *info)
{
    if(!helf_or1k_is_or1k(input_file))
    {
        return false;
    }
    return helf_arch_relocation_section_info_get(input_file,index,info);
}

size_t helf_or1k_relocation_count_get(helf_file_input_t *input_file,size_t section_index)
{
    if(!helf_or1k_is_or1k(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_count_get(input_file,section_index);
}

bool helf_or1k_relocation_get(helf_file_input_t *input_file,size_t section_index,size_t index,helf_arch_relocation_info_t *info)
{
    if(!helf_or1k_is_or1k(input_file))
    {
        return false;
    }
    if(!helf_arch_relocation_get(input_file,section_index,index,info))
    {
        return false;
    }
    return true;
}

bool helf_or1k_relocation_symbol_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_symbol_info_t *info)
{
    if(!helf_or1k_is_or1k(input_file))
    {
        return false;
    }
    return helf_arch_relocation_symbol_info_get(input_file,relocation,info);
}

static const helf_arch_relocation_type_name_t helf_or1k_relocation_type_names[]=
{
    {0,0,"R_OR1K_NONE"},
    {1,4,"R_OR1K_32"},
    {2,2,"R_OR1K_16"},
    {3,1,"R_OR1K_8"},
    {4,4,"R_OR1K_LO_16_IN_INSN"},
    {5,4,"R_OR1K_HI_16_IN_INSN"},
    {6,4,"R_OR1K_INSN_REL_26"},
    {7,4,"R_OR1K_GNU_VTENTRY"},
    {8,4,"R_OR1K_GNU_VTINHERIT"},
    {9,4,"R_OR1K_32_PCREL"},
    {10,2,"R_OR1K_16_PCREL"},
    {11,1,"R_OR1K_8_PCREL"},
    {12,4,"R_OR1K_GOTPC_HI16"},
    {13,4,"R_OR1K_GOTPC_LO16"},
    {14,4,"R_OR1K_GOT16"},
    {15,4,"R_OR1K_PLT26"},
    {16,4,"R_OR1K_GOTOFF_HI16"},
    {17,4,"R_OR1K_GOTOFF_LO16"},
    {18,4,"R_OR1K_COPY"},
    {19,4,"R_OR1K_GLOB_DAT"},
    {20,4,"R_OR1K_JMP_SLOT"},
    {21,4,"R_OR1K_RELATIVE"},
    {22,4,"R_OR1K_TLS_GD_HI16"},
    {23,4,"R_OR1K_TLS_GD_LO16"},
    {24,4,"R_OR1K_TLS_LDM_HI16"},
    {25,4,"R_OR1K_TLS_LDM_LO16"},
    {26,4,"R_OR1K_TLS_LDO_HI16"},
    {27,4,"R_OR1K_TLS_LDO_LO16"},
    {28,4,"R_OR1K_TLS_IE_HI16"},
    {29,4,"R_OR1K_TLS_IE_LO16"},
    {30,4,"R_OR1K_TLS_LE_HI16"},
    {31,4,"R_OR1K_TLS_LE_LO16"},
    {32,4,"R_OR1K_TLS_TPOFF"},
    {33,4,"R_OR1K_TLS_DTPOFF"},
    {34,4,"R_OR1K_TLS_DTPMOD"},
    {35,4,"R_OR1K_AHI16"},
    {36,4,"R_OR1K_GOTOFF_AHI16"},
    {37,4,"R_OR1K_TLS_IE_AHI16"},
    {38,4,"R_OR1K_TLS_LE_AHI16"},
    {39,4,"R_OR1K_SLO16"},
    {40,4,"R_OR1K_GOTOFF_SLO16"},
    {41,4,"R_OR1K_TLS_LE_SLO16"},
    {42,4,"R_OR1K_PCREL_PG21"},
    {43,4,"R_OR1K_GOT_PG21"},
    {44,4,"R_OR1K_TLS_GD_PG21"},
    {45,4,"R_OR1K_TLS_LDM_PG21"},
    {46,4,"R_OR1K_TLS_IE_PG21"},
    {47,4,"R_OR1K_LO13"},
    {48,4,"R_OR1K_GOT_LO13"},
    {49,4,"R_OR1K_TLS_GD_LO13"},
    {50,4,"R_OR1K_TLS_LDM_LO13"},
    {51,4,"R_OR1K_TLS_IE_LO13"},
    {52,4,"R_OR1K_SLO13"},
    {53,4,"R_OR1K_PLTA26"},
    {54,4,"R_OR1K_GOT_AHI16"},
};

const char *helf_or1k_relocation_type_name_get(uint32_t type)
{
    return helf_arch_relocation_type_name_get(helf_or1k_relocation_type_names,sizeof(helf_or1k_relocation_type_names)/sizeof(helf_or1k_relocation_type_names[0]),type);
}

uint8_t helf_or1k_relocation_size_get(uint32_t type)
{
    return helf_arch_relocation_type_size_get(helf_or1k_relocation_type_names,sizeof(helf_or1k_relocation_type_names)/sizeof(helf_or1k_relocation_type_names[0]),type);
}

const char *helf_or1k_section_type_name_get(uint32_t type)
{
    (void)type;
    return NULL;
}

bool helf_or1k_relocation_address_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_relocation_address_info_t *info)
{
    if(!helf_or1k_is_or1k(input_file) || relocation==NULL)
    {
        return false;
    }
    uint8_t size=helf_or1k_relocation_size_get(relocation->type);
    if(size==0)
    {
        return false;
    }
    return helf_arch_relocation_address_info_get(input_file,relocation,size,info);
}

size_t helf_or1k_relocation_data_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,void *buffer,size_t buffer_length)
{
    if(!helf_or1k_is_or1k(input_file) || relocation==NULL)
    {
        return 0;
    }
    uint8_t size=helf_or1k_relocation_size_get(relocation->type);
    if(size==0)
    {
        return 0;
    }
    return helf_arch_relocation_data_get(input_file,relocation,size,buffer,buffer_length);
}

bool helf_or1k_is_shared_object(helf_file_input_t *input_file)
{
    return helf_arch_is_shared_object(input_file,helf_or1k_machines,sizeof(helf_or1k_machines)/sizeof(helf_or1k_machines[0]),HELF_ELFCLASS_32);
}

bool helf_or1k_is_relocatable(helf_file_input_t *input_file)
{
    return helf_arch_is_relocatable(input_file,helf_or1k_machines,sizeof(helf_or1k_machines)/sizeof(helf_or1k_machines[0]),HELF_ELFCLASS_32);
}

size_t helf_or1k_dynamic_count_get(helf_file_input_t *input_file)
{
    if(!helf_or1k_is_or1k(input_file))
    {
        return 0;
    }
    return helf_arch_dynamic_count_get(input_file);
}

bool helf_or1k_dynamic_info_get(helf_file_input_t *input_file,size_t index,helf_arch_dynamic_info_t *info)
{
    if(!helf_or1k_is_or1k(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_info_get(input_file,index,info);
}

bool helf_or1k_dynamic_string_get(helf_file_input_t *input_file,uint64_t offset,char *buffer,size_t buffer_length)
{
    if(!helf_or1k_is_or1k(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_string_get(input_file,offset,buffer,buffer_length);
}

/* helf_relocation_howto_begin */
static const helf_arch_relocation_howto_t helf_or1k_reloc_howto[]=
{
    {0x00000000u,0,0,0,0,0x0040,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000001u,4,32,0,0,0x0000,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000002u,2,16,0,0,0x0000,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000003u,1,8,0,0,0x0000,0x0000000000000000ULL,0x00000000000000FFULL},
    {0x00000004u,4,16,0,0,0x0000,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000005u,4,16,16,0,0x0000,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000006u,4,26,2,0,0x0003,0x0000000000000000ULL,0x0000000003FFFFFFULL},
    {0x00000007u,4,0,0,0,0x0010,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000008u,4,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000009u,4,32,0,0,0x0003,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x0000000Au,2,16,0,0,0x0003,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000000Bu,1,8,0,0,0x0003,0x0000000000000000ULL,0x00000000000000FFULL},
    {0x0000000Cu,4,16,16,0,0x0203,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000000Du,4,16,0,0,0x0203,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000000Eu,4,16,0,0,0x0200,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000000Fu,4,26,2,0,0x0003,0x0000000000000000ULL,0x0000000003FFFFFFULL},
    {0x00000010u,4,16,16,0,0x0200,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000011u,4,16,0,0,0x0200,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000012u,4,32,0,0,0x0180,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000013u,4,32,0,0,0x0100,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000014u,4,32,0,0,0x0140,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000015u,4,32,0,0,0x0080,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000016u,4,16,16,0,0x01C0,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000017u,4,16,0,0,0x01C0,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000018u,4,16,16,0,0x01C0,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000019u,4,16,0,0,0x01C0,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000001Au,4,16,16,0,0x01C0,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000001Bu,4,16,0,0,0x01C0,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000001Cu,4,16,16,0,0x01C0,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000001Du,4,16,0,0,0x01C0,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000001Eu,4,16,16,0,0x01C0,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000001Fu,4,16,0,0,0x01C0,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000020u,4,32,0,0,0x01C0,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000021u,4,32,0,0,0x01C0,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000022u,4,32,0,0,0x01C0,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000023u,4,16,16,0,0x0000,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000024u,4,16,16,0,0x0200,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000025u,4,16,16,0,0x01C0,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000026u,4,16,16,0,0x01C0,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000027u,4,16,0,0,0x0000,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000028u,4,16,0,0,0x0200,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000029u,4,16,0,0,0x01C0,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000002Au,4,21,13,0,0x0002,0x0000000000000000ULL,0x00000000001FFFFFULL},
    {0x0000002Bu,4,21,13,0,0x0202,0x0000000000000000ULL,0x00000000001FFFFFULL},
    {0x0000002Cu,4,21,13,0,0x01C2,0x0000000000000000ULL,0x00000000001FFFFFULL},
    {0x0000002Du,4,21,13,0,0x01C2,0x0000000000000000ULL,0x00000000001FFFFFULL},
    {0x0000002Eu,4,21,13,0,0x01C2,0x0000000000000000ULL,0x00000000001FFFFFULL},
    {0x0000002Fu,4,16,0,0,0x0000,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000030u,4,16,0,0,0x0200,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000031u,4,16,0,0,0x01C0,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000032u,4,16,0,0,0x01C0,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000033u,4,16,0,0,0x01C0,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000034u,4,16,0,0,0x0000,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000035u,4,26,2,0,0x0003,0x0000000000000000ULL,0x0000000003FFFFFFULL},
    {0x00000036u,4,16,16,0,0x0200,0x0000000000000000ULL,0x000000000000FFFFULL},
};

bool helf_or1k_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto)
{
    if(howto==NULL)
    {
        return false;
    }
    size_t count=sizeof(helf_or1k_reloc_howto)/sizeof(helf_or1k_reloc_howto[0]);
    if(count==0)
    {
        return false;
    }
    for(size_t i=0;i<count;i++)
    {
        if(helf_or1k_reloc_howto[i].type==type)
        {
            *howto=helf_or1k_reloc_howto[i];
            return true;
        }
    }
    return false;
}
/* helf_relocation_howto_end */


