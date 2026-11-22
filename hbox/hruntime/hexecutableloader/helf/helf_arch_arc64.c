/***************************************************************
 * Name:      helf_arch_arc64.c
 * Purpose:   实现helf_arch_arc64接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "helf_arch_arc64.h"

static const uint16_t helf_arc64_machines[]=
{
    HELF_EM_ARC_COMPACT3_64,
};

bool helf_arc64_is_arc64(helf_file_input_t *input_file)
{
    return helf_arch_is(input_file,helf_arc64_machines,sizeof(helf_arc64_machines)/sizeof(helf_arc64_machines[0]),HELF_ELFCLASS_64);
}

bool helf_arc64_is_big_endian(helf_file_input_t *input_file)
{
    if(!helf_arc64_is_arc64(input_file))
    {
        return false;
    }
    return helf_file_input_is_big_endian(input_file);
}

bool helf_arc64_is_executable(helf_file_input_t *input_file)
{
    return helf_arch_is_executable(input_file,helf_arc64_machines,sizeof(helf_arc64_machines)/sizeof(helf_arc64_machines[0]),HELF_ELFCLASS_64);
}

bool helf_arc64_entry_get(helf_file_input_t *input_file,uint64_t *entry)
{
    if(entry==NULL)
    {
        return false;
    }
    if(!helf_arc64_is_arc64(input_file))
    {
        return false;
    }
    uint64_t elf_entry=0;
    if(!helf_arch_entry_get(input_file,&elf_entry))
    {
        return false;
    }
    *entry=(uint64_t)elf_entry;
    return true;
}

size_t helf_arc64_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_arc64_is_arc64(input_file))
    {
        return 0;
    }
    return helf_arch_section_count_get(input_file);
}

bool helf_arc64_section_info_get(helf_file_input_t *input_file,size_t index,helf_arc64_section_info_t *info)
{
    if(!helf_arc64_is_arc64(input_file))
    {
        return false;
    }
    return helf_arch_section_info_get(input_file,index,info);
}

bool helf_arc64_is_rx_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_arc64_is_arc64(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_section(index,input_file);
}

intptr_t helf_arc64_rx_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_arc64_is_rx_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rx_section_offset_get(index,input_file);
}

size_t helf_arc64_rx_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_arc64_is_arc64(input_file))
    {
        return 0;
    }
    return helf_arch_rx_section_size_get(input_file);
}

bool helf_arc64_is_ro_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_arc64_is_arc64(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_section(index,input_file);
}

intptr_t helf_arc64_ro_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_arc64_is_ro_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_ro_section_offset_get(index,input_file);
}

size_t helf_arc64_ro_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_arc64_is_arc64(input_file))
    {
        return 0;
    }
    return helf_arch_ro_section_size_get(input_file);
}

bool helf_arc64_is_rw_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_arc64_is_arc64(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_section(index,input_file);
}

intptr_t helf_arc64_rw_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_arc64_is_rw_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rw_section_offset_get(index,input_file);
}

size_t helf_arc64_rw_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_arc64_is_arc64(input_file))
    {
        return 0;
    }
    return helf_arch_rw_section_size_get(input_file);
}

bool helf_arc64_is_bss_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_arc64_is_arc64(input_file))
    {
        return false;
    }
    return helf_arch_is_bss_section(index,input_file);
}

size_t helf_arc64_program_header_count_get(helf_file_input_t *input_file)
{
    if(!helf_arc64_is_arc64(input_file))
    {
        return 0;
    }
    return helf_arch_program_header_count_get(input_file);
}

bool helf_arc64_program_header_get(helf_file_input_t *input_file,size_t index,helf_elf64_program_header_t *programheader)
{
    if(programheader==NULL)
    {
        return false;
    }
    if(!helf_arc64_is_arc64(input_file))
    {
        return false;
    }
    if(index>=helf_arch_program_header_count_get(input_file))
    {
        return false;
    }
    return helf_file_input_64_bits_program_header_get(input_file,index,programheader);
}

size_t helf_arc64_segment_count_get(helf_file_input_t *input_file)
{
    return helf_arc64_program_header_count_get(input_file);
}

bool helf_arc64_segment_info_get(helf_file_input_t *input_file,size_t index,helf_arc64_segment_info_t *info)
{
    if(!helf_arc64_is_arc64(input_file))
    {
        return false;
    }
    return helf_arch_segment_info_get(input_file,index,info);
}

bool helf_arc64_is_load_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_arc64_is_arc64(input_file))
    {
        return false;
    }
    return helf_arch_is_load_segment(index,input_file);
}

bool helf_arc64_is_rx_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_arc64_is_arc64(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_segment(index,input_file);
}

bool helf_arc64_is_ro_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_arc64_is_arc64(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_segment(index,input_file);
}

bool helf_arc64_is_rw_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_arc64_is_arc64(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_segment(index,input_file);
}

size_t helf_arc64_load_segment_count_get(helf_file_input_t *input_file)
{
    if(!helf_arc64_is_arc64(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_count_get(input_file);
}

bool helf_arc64_load_segment_info_get(helf_file_input_t *input_file,size_t index,helf_arc64_segment_info_t *info)
{
    if(!helf_arc64_is_arc64(input_file))
    {
        return false;
    }
    return helf_arch_load_segment_info_get(input_file,index,info);
}

size_t helf_arc64_load_segment_total_filesz_get(helf_file_input_t *input_file)
{
    if(!helf_arc64_is_arc64(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_filesz_get(input_file);
}

size_t helf_arc64_load_segment_total_memsz_get(helf_file_input_t *input_file)
{
    if(!helf_arc64_is_arc64(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_memsz_get(input_file);
}

size_t helf_arc64_relocation_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_arc64_is_arc64(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_section_count_get(input_file);
}

bool helf_arc64_relocation_section_info_get(helf_file_input_t *input_file,size_t index,helf_arch_relocation_section_info_t *info)
{
    if(!helf_arc64_is_arc64(input_file))
    {
        return false;
    }
    return helf_arch_relocation_section_info_get(input_file,index,info);
}

size_t helf_arc64_relocation_count_get(helf_file_input_t *input_file,size_t section_index)
{
    if(!helf_arc64_is_arc64(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_count_get(input_file,section_index);
}

bool helf_arc64_relocation_get(helf_file_input_t *input_file,size_t section_index,size_t index,helf_arch_relocation_info_t *info)
{
    if(!helf_arc64_is_arc64(input_file))
    {
        return false;
    }
    if(!helf_arch_relocation_get(input_file,section_index,index,info))
    {
        return false;
    }
    return true;
}

bool helf_arc64_relocation_symbol_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_symbol_info_t *info)
{
    if(!helf_arc64_is_arc64(input_file))
    {
        return false;
    }
    return helf_arch_relocation_symbol_info_get(input_file,relocation,info);
}

static const helf_arch_relocation_type_name_t helf_arc64_relocation_type_names[]=
{
    {0,0,"R_ARC_NONE"},
    {1,1,"R_ARC_8"},
    {2,2,"R_ARC_16"},
    {3,0,"R_ARC_24"},
    {4,4,"R_ARC_32"},
    {8,1,"R_ARC_N8"},
    {9,2,"R_ARC_N16"},
    {10,0,"R_ARC_N24"},
    {11,4,"R_ARC_N32"},
    {12,0,"R_ARC_SDA"},
    {13,0,"R_ARC_SECTOFF"},
    {14,0,"R_ARC_S21H_PCREL"},
    {15,0,"R_ARC_S21W_PCREL"},
    {16,0,"R_ARC_S25H_PCREL"},
    {17,0,"R_ARC_S25W_PCREL"},
    {18,4,"R_ARC_SDA32"},
    {19,0,"R_ARC_SDA_LDST"},
    {20,0,"R_ARC_SDA_LDST1"},
    {21,0,"R_ARC_SDA_LDST2"},
    {22,0,"R_ARC_SDA16_LD"},
    {23,0,"R_ARC_SDA16_LD1"},
    {24,0,"R_ARC_SDA16_LD2"},
    {25,0,"R_ARC_S13_PCREL"},
    {26,0,"R_ARC_W"},
    {27,0,"R_ARC_32_ME"},
    {105,0,"R_ARC_32_ME_S"},
    {28,0,"R_ARC_N32_ME"},
    {29,0,"R_ARC_SECTOFF_ME"},
    {30,0,"R_ARC_SDA32_ME"},
    {31,0,"R_ARC_W_ME"},
    {35,1,"R_AC_SECTOFF_U8"},
    {36,0,"R_AC_SECTOFF_U8_1"},
    {37,0,"R_AC_SECTOFF_U8_2"},
    {38,0,"R_AC_SECTOFF_S9"},
    {39,0,"R_AC_SECTOFF_S9_1"},
    {40,0,"R_AC_SECTOFF_S9_2"},
    {41,0,"R_ARC_SECTOFF_ME_1"},
    {42,0,"R_ARC_SECTOFF_ME_2"},
    {43,0,"R_ARC_SECTOFF_1"},
    {44,0,"R_ARC_SECTOFF_2"},
    {45,0,"R_ARC_SDA_12"},
    {48,0,"R_ARC_SDA16_ST2"},
    {49,0,"R_ARC_32_PCREL"},
    {50,4,"R_ARC_PC32"},
    {59,4,"R_ARC_GOT32"},
    {51,4,"R_ARC_GOTPC32"},
    {52,4,"R_ARC_PLT32"},
    {53,0,"R_ARC_COPY"},
    {54,0,"R_ARC_GLOB_DAT"},
    {55,0,"R_ARC_JMP_SLOT"},
    {56,0,"R_ARC_RELATIVE"},
    {57,0,"R_ARC_GOTOFF"},
    {58,0,"R_ARC_GOTPC"},
    {60,0,"R_ARC_S21W_PCREL_PLT"},
    {61,0,"R_ARC_S25H_PCREL_PLT"},
    {63,0,"R_ARC_JLI_SECTOFF"},
    {66,0,"R_ARC_TLS_DTPMOD"},
    {68,0,"R_ARC_TLS_TPOFF"},
    {69,0,"R_ARC_TLS_GD_GOT"},
    {70,0,"R_ARC_TLS_GD_LD"},
    {71,0,"R_ARC_TLS_GD_CALL"},
    {72,0,"R_ARC_TLS_IE_GOT"},
    {67,0,"R_ARC_TLS_DTPOFF"},
    {73,0,"R_ARC_TLS_DTPOFF_S9"},
    {74,0,"R_ARC_TLS_LE_S9"},
    {75,4,"R_ARC_TLS_LE_32"},
    {76,0,"R_ARC_S25W_PCREL_PLT"},
    {77,0,"R_ARC_S21H_PCREL_PLT"},
    {78,2,"R_ARC_NPS_CMEM16"},
};

const char *helf_arc64_relocation_type_name_get(uint32_t type)
{
    return helf_arch_relocation_type_name_get(helf_arc64_relocation_type_names,sizeof(helf_arc64_relocation_type_names)/sizeof(helf_arc64_relocation_type_names[0]),type);
}

uint8_t helf_arc64_relocation_size_get(uint32_t type)
{
    return helf_arch_relocation_type_size_get(helf_arc64_relocation_type_names,sizeof(helf_arc64_relocation_type_names)/sizeof(helf_arc64_relocation_type_names[0]),type);
}

static const helf_arch_section_type_name_t helf_arc64_section_type_names[]=
{
    {1879048193,"SHT_ARC_ATTRIBUTES"},
};

const char *helf_arc64_section_type_name_get(uint32_t type)
{
    return helf_arch_section_type_name_get(helf_arc64_section_type_names,sizeof(helf_arc64_section_type_names)/sizeof(helf_arc64_section_type_names[0]),type);
}

bool helf_arc64_relocation_address_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_relocation_address_info_t *info)
{
    if(!helf_arc64_is_arc64(input_file) || relocation==NULL)
    {
        return false;
    }
    uint8_t size=helf_arc64_relocation_size_get(relocation->type);
    if(size==0)
    {
        return false;
    }
    return helf_arch_relocation_address_info_get(input_file,relocation,size,info);
}

size_t helf_arc64_relocation_data_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,void *buffer,size_t buffer_length)
{
    if(!helf_arc64_is_arc64(input_file) || relocation==NULL)
    {
        return 0;
    }
    uint8_t size=helf_arc64_relocation_size_get(relocation->type);
    if(size==0)
    {
        return 0;
    }
    return helf_arch_relocation_data_get(input_file,relocation,size,buffer,buffer_length);
}

bool helf_arc64_is_shared_object(helf_file_input_t *input_file)
{
    return helf_arch_is_shared_object(input_file,helf_arc64_machines,sizeof(helf_arc64_machines)/sizeof(helf_arc64_machines[0]),HELF_ELFCLASS_64);
}

bool helf_arc64_is_relocatable(helf_file_input_t *input_file)
{
    return helf_arch_is_relocatable(input_file,helf_arc64_machines,sizeof(helf_arc64_machines)/sizeof(helf_arc64_machines[0]),HELF_ELFCLASS_64);
}

size_t helf_arc64_dynamic_count_get(helf_file_input_t *input_file)
{
    if(!helf_arc64_is_arc64(input_file))
    {
        return 0;
    }
    return helf_arch_dynamic_count_get(input_file);
}

bool helf_arc64_dynamic_info_get(helf_file_input_t *input_file,size_t index,helf_arch_dynamic_info_t *info)
{
    if(!helf_arc64_is_arc64(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_info_get(input_file,index,info);
}

bool helf_arc64_dynamic_string_get(helf_file_input_t *input_file,uint64_t offset,char *buffer,size_t buffer_length)
{
    if(!helf_arc64_is_arc64(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_string_get(input_file,offset,buffer,buffer_length);
}

/* helf_relocation_howto_begin */
static const helf_arch_relocation_howto_t helf_arc64_reloc_howto[]=
{
    {0x00000000u,0,0,0,0,0x0040,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000001u,1,8,0,0,0x0000,0x0000000000000000ULL,0x00000000000000FFULL},
    {0x00000002u,2,16,0,0,0x0000,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000003u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000004u,4,32,0,0,0x0000,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000008u,1,8,0,0,0x0000,0x0000000000000000ULL,0x00000000000000FFULL},
    {0x00000009u,2,16,0,0,0x0000,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000000Au,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000000Bu,4,32,0,0,0x0000,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x0000000Cu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000000Du,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000000Eu,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000000Fu,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000010u,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000011u,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000012u,4,32,0,0,0x0000,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000013u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000014u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000015u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000016u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000017u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000018u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000019u,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Au,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Bu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Cu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Du,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Eu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Fu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000023u,1,8,0,0,0x0000,0x0000000000000000ULL,0x00000000000000FFULL},
    {0x00000024u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000025u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000026u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000027u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000028u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000029u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000002Au,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000002Bu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000002Cu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000002Du,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000030u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000031u,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000032u,4,32,0,0,0x0003,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000033u,4,32,0,0,0x0603,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000034u,4,32,0,0,0x0000,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000035u,0,0,0,0,0x0180,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000036u,0,0,0,0,0x0100,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000037u,0,0,0,0,0x0140,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000038u,0,0,0,0,0x0080,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000039u,0,0,0,0,0x0200,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000003Au,0,0,0,0,0x0200,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000003Bu,4,32,0,0,0x0200,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x0000003Cu,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000003Du,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000003Fu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000042u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000043u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000044u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000045u,0,0,0,0,0x03C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000046u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000047u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000048u,0,0,0,0,0x03C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000049u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000004Au,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000004Bu,4,32,0,0,0x01C0,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x0000004Cu,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000004Du,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000004Eu,2,16,0,0,0x0000,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000069u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
};

bool helf_arc64_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto)
{
    if(howto==NULL)
    {
        return false;
    }
    size_t count=sizeof(helf_arc64_reloc_howto)/sizeof(helf_arc64_reloc_howto[0]);
    if(count==0)
    {
        return false;
    }
    for(size_t i=0;i<count;i++)
    {
        if(helf_arc64_reloc_howto[i].type==type)
        {
            *howto=helf_arc64_reloc_howto[i];
            return true;
        }
    }
    return false;
}
/* helf_relocation_howto_end */

