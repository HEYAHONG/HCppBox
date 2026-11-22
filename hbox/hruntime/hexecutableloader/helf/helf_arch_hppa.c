/***************************************************************
 * Name:      helf_arch_hppa.c
 * Purpose:   实现helf_arch_hppa接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "helf_arch_hppa.h"

static const uint16_t helf_hppa_machines[]=
{
    HELF_EM_PARISC,
};

bool helf_hppa_is_hppa(helf_file_input_t *input_file)
{
    return helf_arch_is(input_file,helf_hppa_machines,sizeof(helf_hppa_machines)/sizeof(helf_hppa_machines[0]),HELF_ELFCLASS_32);
}

bool helf_hppa_is_big_endian(helf_file_input_t *input_file)
{
    if(!helf_hppa_is_hppa(input_file))
    {
        return false;
    }
    return helf_file_input_is_big_endian(input_file);
}

bool helf_hppa_is_executable(helf_file_input_t *input_file)
{
    return helf_arch_is_executable(input_file,helf_hppa_machines,sizeof(helf_hppa_machines)/sizeof(helf_hppa_machines[0]),HELF_ELFCLASS_32);
}

bool helf_hppa_entry_get(helf_file_input_t *input_file,uint32_t *entry)
{
    if(entry==NULL)
    {
        return false;
    }
    if(!helf_hppa_is_hppa(input_file))
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

size_t helf_hppa_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_hppa_is_hppa(input_file))
    {
        return 0;
    }
    return helf_arch_section_count_get(input_file);
}

bool helf_hppa_section_info_get(helf_file_input_t *input_file,size_t index,helf_hppa_section_info_t *info)
{
    if(!helf_hppa_is_hppa(input_file))
    {
        return false;
    }
    return helf_arch_section_info_get(input_file,index,info);
}

bool helf_hppa_is_rx_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_hppa_is_hppa(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_section(index,input_file);
}

intptr_t helf_hppa_rx_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_hppa_is_rx_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rx_section_offset_get(index,input_file);
}

size_t helf_hppa_rx_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_hppa_is_hppa(input_file))
    {
        return 0;
    }
    return helf_arch_rx_section_size_get(input_file);
}

bool helf_hppa_is_ro_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_hppa_is_hppa(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_section(index,input_file);
}

intptr_t helf_hppa_ro_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_hppa_is_ro_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_ro_section_offset_get(index,input_file);
}

size_t helf_hppa_ro_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_hppa_is_hppa(input_file))
    {
        return 0;
    }
    return helf_arch_ro_section_size_get(input_file);
}

bool helf_hppa_is_rw_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_hppa_is_hppa(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_section(index,input_file);
}

intptr_t helf_hppa_rw_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_hppa_is_rw_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rw_section_offset_get(index,input_file);
}

size_t helf_hppa_rw_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_hppa_is_hppa(input_file))
    {
        return 0;
    }
    return helf_arch_rw_section_size_get(input_file);
}

bool helf_hppa_is_bss_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_hppa_is_hppa(input_file))
    {
        return false;
    }
    return helf_arch_is_bss_section(index,input_file);
}

size_t helf_hppa_program_header_count_get(helf_file_input_t *input_file)
{
    if(!helf_hppa_is_hppa(input_file))
    {
        return 0;
    }
    return helf_arch_program_header_count_get(input_file);
}

bool helf_hppa_program_header_get(helf_file_input_t *input_file,size_t index,helf_elf32_program_header_t *programheader)
{
    if(programheader==NULL)
    {
        return false;
    }
    if(!helf_hppa_is_hppa(input_file))
    {
        return false;
    }
    if(index>=helf_arch_program_header_count_get(input_file))
    {
        return false;
    }
    return helf_file_input_32_bits_program_header_get(input_file,index,programheader);
}

size_t helf_hppa_segment_count_get(helf_file_input_t *input_file)
{
    return helf_hppa_program_header_count_get(input_file);
}

bool helf_hppa_segment_info_get(helf_file_input_t *input_file,size_t index,helf_hppa_segment_info_t *info)
{
    if(!helf_hppa_is_hppa(input_file))
    {
        return false;
    }
    return helf_arch_segment_info_get(input_file,index,info);
}

bool helf_hppa_is_load_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_hppa_is_hppa(input_file))
    {
        return false;
    }
    return helf_arch_is_load_segment(index,input_file);
}

bool helf_hppa_is_rx_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_hppa_is_hppa(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_segment(index,input_file);
}

bool helf_hppa_is_ro_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_hppa_is_hppa(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_segment(index,input_file);
}

bool helf_hppa_is_rw_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_hppa_is_hppa(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_segment(index,input_file);
}

size_t helf_hppa_load_segment_count_get(helf_file_input_t *input_file)
{
    if(!helf_hppa_is_hppa(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_count_get(input_file);
}

bool helf_hppa_load_segment_info_get(helf_file_input_t *input_file,size_t index,helf_hppa_segment_info_t *info)
{
    if(!helf_hppa_is_hppa(input_file))
    {
        return false;
    }
    return helf_arch_load_segment_info_get(input_file,index,info);
}

size_t helf_hppa_load_segment_total_filesz_get(helf_file_input_t *input_file)
{
    if(!helf_hppa_is_hppa(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_filesz_get(input_file);
}

size_t helf_hppa_load_segment_total_memsz_get(helf_file_input_t *input_file)
{
    if(!helf_hppa_is_hppa(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_memsz_get(input_file);
}

size_t helf_hppa_relocation_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_hppa_is_hppa(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_section_count_get(input_file);
}

bool helf_hppa_relocation_section_info_get(helf_file_input_t *input_file,size_t index,helf_arch_relocation_section_info_t *info)
{
    if(!helf_hppa_is_hppa(input_file))
    {
        return false;
    }
    return helf_arch_relocation_section_info_get(input_file,index,info);
}

size_t helf_hppa_relocation_count_get(helf_file_input_t *input_file,size_t section_index)
{
    if(!helf_hppa_is_hppa(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_count_get(input_file,section_index);
}

bool helf_hppa_relocation_get(helf_file_input_t *input_file,size_t section_index,size_t index,helf_arch_relocation_info_t *info)
{
    if(!helf_hppa_is_hppa(input_file))
    {
        return false;
    }
    if(!helf_arch_relocation_get(input_file,section_index,index,info))
    {
        return false;
    }
    return true;
}

bool helf_hppa_relocation_symbol_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_symbol_info_t *info)
{
    if(!helf_hppa_is_hppa(input_file))
    {
        return false;
    }
    return helf_arch_relocation_symbol_info_get(input_file,relocation,info);
}

static const helf_arch_relocation_type_name_t helf_hppa_relocation_type_names[]=
{
    {0,0,"R_PARISC_NONE"},
    {1,4,"R_PARISC_DIR32"},
    {2,0,"R_PARISC_DIR21L"},
    {3,0,"R_PARISC_DIR17R"},
    {4,0,"R_PARISC_DIR17F"},
    {6,0,"R_PARISC_DIR14R"},
    {7,0,"R_PARISC_DIR14F"},
    {8,0,"R_PARISC_PCREL12F"},
    {9,4,"R_PARISC_PCREL32"},
    {10,0,"R_PARISC_PCREL21L"},
    {11,0,"R_PARISC_PCREL17R"},
    {12,0,"R_PARISC_PCREL17F"},
    {13,0,"R_PARISC_PCREL17C"},
    {14,0,"R_PARISC_PCREL14R"},
    {15,0,"R_PARISC_PCREL14F"},
    {18,0,"R_PARISC_DPREL21L"},
    {19,0,"R_PARISC_DPREL14WR"},
    {20,0,"R_PARISC_DPREL14DR"},
    {22,0,"R_PARISC_DPREL14R"},
    {23,0,"R_PARISC_DPREL14F"},
    {26,0,"R_PARISC_DLTREL21L"},
    {30,0,"R_PARISC_DLTREL14R"},
    {31,0,"R_PARISC_DLTREL14F"},
    {34,0,"R_PARISC_DLTIND21L"},
    {38,0,"R_PARISC_DLTIND14R"},
    {39,0,"R_PARISC_DLTIND14F"},
    {40,0,"R_PARISC_SETBASE"},
    {41,4,"R_PARISC_SECREL32"},
    {42,0,"R_PARISC_BASEREL21L"},
    {43,0,"R_PARISC_BASEREL17R"},
    {44,0,"R_PARISC_BASEREL17F"},
    {46,0,"R_PARISC_BASEREL14R"},
    {47,0,"R_PARISC_BASEREL14F"},
    {48,0,"R_PARISC_SEGBASE"},
    {49,4,"R_PARISC_SEGREL32"},
    {50,0,"R_PARISC_PLTOFF21L"},
    {54,0,"R_PARISC_PLTOFF14R"},
    {55,0,"R_PARISC_PLTOFF14F"},
    {57,4,"R_PARISC_LTOFF_FPTR32"},
    {58,0,"R_PARISC_LTOFF_FPTR21L"},
    {62,0,"R_PARISC_LTOFF_FPTR14R"},
    {64,8,"R_PARISC_FPTR64"},
    {65,4,"R_PARISC_PLABEL32"},
    {66,0,"R_PARISC_PLABEL21L"},
    {70,0,"R_PARISC_PLABEL14R"},
    {72,8,"R_PARISC_PCREL64"},
    {73,0,"R_PARISC_PCREL22C"},
    {74,0,"R_PARISC_PCREL22F"},
    {75,0,"R_PARISC_PCREL14WR"},
    {76,0,"R_PARISC_PCREL14DR"},
    {77,0,"R_PARISC_PCREL16F"},
    {78,0,"R_PARISC_PCREL16WF"},
    {79,0,"R_PARISC_PCREL16DF"},
    {80,8,"R_PARISC_DIR64"},
    {83,0,"R_PARISC_DIR14WR"},
    {84,0,"R_PARISC_DIR14DR"},
    {85,0,"R_PARISC_DIR16F"},
    {86,0,"R_PARISC_DIR16WF"},
    {87,0,"R_PARISC_DIR16DF"},
    {88,8,"R_PARISC_GPREL64"},
    {91,0,"R_PARISC_DLTREL14WR"},
    {92,0,"R_PARISC_DLTREL14DR"},
    {93,0,"R_PARISC_GPREL16F"},
    {94,0,"R_PARISC_GPREL16WF"},
    {95,0,"R_PARISC_GPREL16DF"},
    {96,8,"R_PARISC_LTOFF64"},
    {99,0,"R_PARISC_DLTIND14WR"},
    {100,0,"R_PARISC_DLTIND14DR"},
    {101,0,"R_PARISC_LTOFF16F"},
    {102,0,"R_PARISC_LTOFF16WF"},
    {103,0,"R_PARISC_LTOFF16DF"},
    {104,8,"R_PARISC_SECREL64"},
    {107,0,"R_PARISC_BASEREL14WR"},
    {108,0,"R_PARISC_BASEREL14DR"},
    {112,8,"R_PARISC_SEGREL64"},
    {115,0,"R_PARISC_PLTOFF14WR"},
    {116,0,"R_PARISC_PLTOFF14DR"},
    {117,0,"R_PARISC_PLTOFF16F"},
    {118,0,"R_PARISC_PLTOFF16WF"},
    {119,0,"R_PARISC_PLTOFF16DF"},
    {120,8,"R_PARISC_LTOFF_FPTR64"},
    {123,0,"R_PARISC_LTOFF_FPTR14WR"},
    {124,0,"R_PARISC_LTOFF_FPTR14DR"},
    {125,0,"R_PARISC_LTOFF_FPTR16F"},
    {126,0,"R_PARISC_LTOFF_FPTR16WF"},
    {127,0,"R_PARISC_LTOFF_FPTR16DF"},
    {128,0,"R_PARISC_COPY"},
    {129,0,"R_PARISC_IPLT"},
    {130,0,"R_PARISC_EPLT"},
    {153,4,"R_PARISC_TPREL32"},
    {154,0,"R_PARISC_TPREL21L"},
    {158,0,"R_PARISC_TPREL14R"},
    {162,0,"R_PARISC_LTOFF_TP21L"},
    {166,0,"R_PARISC_LTOFF_TP14R"},
    {167,0,"R_PARISC_LTOFF_TP14F"},
    {216,8,"R_PARISC_TPREL64"},
    {219,0,"R_PARISC_TPREL14WR"},
    {220,0,"R_PARISC_TPREL14DR"},
    {221,0,"R_PARISC_TPREL16F"},
    {222,0,"R_PARISC_TPREL16WF"},
    {223,0,"R_PARISC_TPREL16DF"},
    {224,8,"R_PARISC_LTOFF_TP64"},
    {227,0,"R_PARISC_LTOFF_TP14WR"},
    {228,0,"R_PARISC_LTOFF_TP14DR"},
    {229,0,"R_PARISC_LTOFF_TP16F"},
    {230,0,"R_PARISC_LTOFF_TP16WF"},
    {231,0,"R_PARISC_LTOFF_TP16DF"},
    {232,0,"R_PARISC_GNU_VTENTRY"},
    {233,0,"R_PARISC_GNU_VTINHERIT"},
    {234,0,"R_PARISC_TLS_GD21L"},
    {235,0,"R_PARISC_TLS_GD14R"},
    {236,0,"R_PARISC_TLS_GDCALL"},
    {237,0,"R_PARISC_TLS_LDM21L"},
    {238,0,"R_PARISC_TLS_LDM14R"},
    {239,0,"R_PARISC_TLS_LDMCALL"},
    {240,0,"R_PARISC_TLS_LDO21L"},
    {241,0,"R_PARISC_TLS_LDO14R"},
    {242,4,"R_PARISC_TLS_DTPMOD32"},
    {243,8,"R_PARISC_TLS_DTPMOD64"},
    {244,4,"R_PARISC_TLS_DTPOFF32"},
    {245,8,"R_PARISC_TLS_DTPOFF64"},
};

const char *helf_hppa_relocation_type_name_get(uint32_t type)
{
    return helf_arch_relocation_type_name_get(helf_hppa_relocation_type_names,sizeof(helf_hppa_relocation_type_names)/sizeof(helf_hppa_relocation_type_names[0]),type);
}

uint8_t helf_hppa_relocation_size_get(uint32_t type)
{
    return helf_arch_relocation_type_size_get(helf_hppa_relocation_type_names,sizeof(helf_hppa_relocation_type_names)/sizeof(helf_hppa_relocation_type_names[0]),type);
}

static const helf_arch_section_type_name_t helf_hppa_section_type_names[]=
{
    {1879048192,"SHT_PARISC_EXT"},
    {1879048193,"SHT_PARISC_UNWIND"},
    {1879048194,"SHT_PARISC_DOC"},
    {1879048195,"SHT_PARISC_ANNOT"},
    {1879048196,"SHT_PARISC_DLKM"},
    {1879048200,"SHT_PARISC_SYMEXTN"},
    {1879048201,"SHT_PARISC_STUBS"},
    {1610612736,"SHT_HP_OVLBITS"},
    {1610612737,"SHT_HP_DLKM"},
    {1610612738,"SHT_HP_COMDAT"},
    {1610612739,"SHT_HP_OBJDICT"},
    {1610612740,"SHT_HP_ANNOT"},
};

const char *helf_hppa_section_type_name_get(uint32_t type)
{
    return helf_arch_section_type_name_get(helf_hppa_section_type_names,sizeof(helf_hppa_section_type_names)/sizeof(helf_hppa_section_type_names[0]),type);
}

bool helf_hppa_relocation_address_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_relocation_address_info_t *info)
{
    if(!helf_hppa_is_hppa(input_file) || relocation==NULL)
    {
        return false;
    }
    uint8_t size=helf_hppa_relocation_size_get(relocation->type);
    if(size==0)
    {
        return false;
    }
    return helf_arch_relocation_address_info_get(input_file,relocation,size,info);
}

size_t helf_hppa_relocation_data_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,void *buffer,size_t buffer_length)
{
    if(!helf_hppa_is_hppa(input_file) || relocation==NULL)
    {
        return 0;
    }
    uint8_t size=helf_hppa_relocation_size_get(relocation->type);
    if(size==0)
    {
        return 0;
    }
    return helf_arch_relocation_data_get(input_file,relocation,size,buffer,buffer_length);
}

bool helf_hppa_is_shared_object(helf_file_input_t *input_file)
{
    return helf_arch_is_shared_object(input_file,helf_hppa_machines,sizeof(helf_hppa_machines)/sizeof(helf_hppa_machines[0]),HELF_ELFCLASS_32);
}

bool helf_hppa_is_relocatable(helf_file_input_t *input_file)
{
    return helf_arch_is_relocatable(input_file,helf_hppa_machines,sizeof(helf_hppa_machines)/sizeof(helf_hppa_machines[0]),HELF_ELFCLASS_32);
}

size_t helf_hppa_dynamic_count_get(helf_file_input_t *input_file)
{
    if(!helf_hppa_is_hppa(input_file))
    {
        return 0;
    }
    return helf_arch_dynamic_count_get(input_file);
}

bool helf_hppa_dynamic_info_get(helf_file_input_t *input_file,size_t index,helf_arch_dynamic_info_t *info)
{
    if(!helf_hppa_is_hppa(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_info_get(input_file,index,info);
}

bool helf_hppa_dynamic_string_get(helf_file_input_t *input_file,uint64_t offset,char *buffer,size_t buffer_length)
{
    if(!helf_hppa_is_hppa(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_string_get(input_file,offset,buffer,buffer_length);
}

/* helf_relocation_howto_begin */
static const helf_arch_relocation_howto_t helf_hppa_reloc_howto[]=
{
    {0x00000000u,0,0,0,0,0x0040,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000001u,4,32,0,0,0x0000,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000002u,4,21,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000003u,4,17,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000004u,4,17,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000006u,4,14,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000007u,4,14,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000008u,4,12,0,0,0x0001,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000009u,4,32,0,0,0x0001,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000000Au,4,21,0,0,0x0001,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000000Bu,4,17,0,0,0x0001,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000000Cu,4,17,0,0,0x0001,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000000Du,4,17,0,0,0x0001,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000000Eu,4,14,0,0,0x0001,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000000Fu,4,14,0,0,0x0001,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000012u,4,21,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000013u,4,14,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000014u,4,14,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000016u,4,14,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000017u,4,14,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Au,4,21,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Eu,4,14,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Fu,4,14,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000022u,4,21,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000026u,4,14,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000027u,4,14,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000028u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000029u,4,32,0,0,0x0000,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x0000002Au,4,21,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000002Bu,4,17,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000002Cu,4,17,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000002Eu,4,14,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000002Fu,4,14,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000030u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000031u,4,32,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000032u,4,21,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000036u,4,14,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000037u,4,14,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000039u,4,32,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000003Au,4,21,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000003Eu,4,14,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000040u,8,64,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000041u,4,32,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000042u,4,21,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000046u,4,14,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000048u,8,64,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000049u,4,22,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000004Au,4,22,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000004Bu,4,14,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000004Cu,4,14,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000004Du,4,16,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000004Eu,4,16,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000004Fu,4,16,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000050u,8,64,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000053u,4,14,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000054u,4,14,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000055u,4,16,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000056u,4,16,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000057u,4,16,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000058u,8,64,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000005Bu,4,14,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000005Cu,4,14,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000005Du,4,16,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000005Eu,4,16,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000005Fu,4,16,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000060u,8,64,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000063u,4,14,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000064u,4,14,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000065u,4,16,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000066u,4,16,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000067u,4,16,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000068u,8,64,0,0,0x0003,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x0000006Bu,4,14,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000006Cu,4,14,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000070u,8,64,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000073u,4,14,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000074u,4,14,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000075u,4,16,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000076u,4,16,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000077u,4,16,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000078u,8,64,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000007Bu,4,14,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000007Cu,4,14,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000007Du,4,16,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000007Eu,4,16,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000007Fu,4,16,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000080u,0,0,0,0,0x0180,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000081u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000082u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000099u,4,32,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000009Au,4,21,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000009Eu,4,14,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000A2u,4,21,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000A6u,4,14,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000A7u,4,14,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000D8u,8,64,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000DBu,4,14,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000DCu,4,14,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000DDu,4,16,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000DEu,4,16,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000DFu,4,16,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000E0u,8,64,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000E3u,4,14,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000E4u,4,14,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000E5u,4,16,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000E6u,4,16,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000E7u,4,16,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000E8u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000E9u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000EAu,4,21,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000EBu,4,14,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000ECu,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000EDu,4,21,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000EEu,4,14,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000EFu,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000F0u,4,21,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000F1u,4,14,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000F2u,4,32,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000F3u,8,64,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000F4u,4,32,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000F5u,8,64,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
};

bool helf_hppa_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto)
{
    if(howto==NULL)
    {
        return false;
    }
    size_t count=sizeof(helf_hppa_reloc_howto)/sizeof(helf_hppa_reloc_howto[0]);
    if(count==0)
    {
        return false;
    }
    for(size_t i=0;i<count;i++)
    {
        if(helf_hppa_reloc_howto[i].type==type)
        {
            *howto=helf_hppa_reloc_howto[i];
            return true;
        }
    }
    return false;
}
/* helf_relocation_howto_end */


