/***************************************************************
 * Name:      helf_arch_v850.c
 * Purpose:   实现helf_arch_v850接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "helf_arch_v850.h"

static const uint16_t helf_v850_machines[]=
{
    HELF_EM_V850,
    HELF_EM_CYGNUS_V850,
};

bool helf_v850_is_v850(helf_file_input_t *input_file)
{
    return helf_arch_is(input_file,helf_v850_machines,sizeof(helf_v850_machines)/sizeof(helf_v850_machines[0]),HELF_ELFCLASS_32);
}

bool helf_v850_is_big_endian(helf_file_input_t *input_file)
{
    if(!helf_v850_is_v850(input_file))
    {
        return false;
    }
    return helf_file_input_is_big_endian(input_file);
}

bool helf_v850_is_executable(helf_file_input_t *input_file)
{
    return helf_arch_is_executable(input_file,helf_v850_machines,sizeof(helf_v850_machines)/sizeof(helf_v850_machines[0]),HELF_ELFCLASS_32);
}

bool helf_v850_entry_get(helf_file_input_t *input_file,uint32_t *entry)
{
    if(entry==NULL)
    {
        return false;
    }
    if(!helf_v850_is_v850(input_file))
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

size_t helf_v850_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_v850_is_v850(input_file))
    {
        return 0;
    }
    return helf_arch_section_count_get(input_file);
}

bool helf_v850_section_info_get(helf_file_input_t *input_file,size_t index,helf_v850_section_info_t *info)
{
    if(!helf_v850_is_v850(input_file))
    {
        return false;
    }
    return helf_arch_section_info_get(input_file,index,info);
}

bool helf_v850_is_rx_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_v850_is_v850(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_section(index,input_file);
}

intptr_t helf_v850_rx_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_v850_is_rx_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rx_section_offset_get(index,input_file);
}

size_t helf_v850_rx_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_v850_is_v850(input_file))
    {
        return 0;
    }
    return helf_arch_rx_section_size_get(input_file);
}

bool helf_v850_is_ro_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_v850_is_v850(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_section(index,input_file);
}

intptr_t helf_v850_ro_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_v850_is_ro_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_ro_section_offset_get(index,input_file);
}

size_t helf_v850_ro_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_v850_is_v850(input_file))
    {
        return 0;
    }
    return helf_arch_ro_section_size_get(input_file);
}

bool helf_v850_is_rw_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_v850_is_v850(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_section(index,input_file);
}

intptr_t helf_v850_rw_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_v850_is_rw_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rw_section_offset_get(index,input_file);
}

size_t helf_v850_rw_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_v850_is_v850(input_file))
    {
        return 0;
    }
    return helf_arch_rw_section_size_get(input_file);
}

bool helf_v850_is_bss_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_v850_is_v850(input_file))
    {
        return false;
    }
    return helf_arch_is_bss_section(index,input_file);
}

size_t helf_v850_program_header_count_get(helf_file_input_t *input_file)
{
    if(!helf_v850_is_v850(input_file))
    {
        return 0;
    }
    return helf_arch_program_header_count_get(input_file);
}

bool helf_v850_program_header_get(helf_file_input_t *input_file,size_t index,helf_elf32_program_header_t *programheader)
{
    if(programheader==NULL)
    {
        return false;
    }
    if(!helf_v850_is_v850(input_file))
    {
        return false;
    }
    if(index>=helf_arch_program_header_count_get(input_file))
    {
        return false;
    }
    return helf_file_input_32_bits_program_header_get(input_file,index,programheader);
}

size_t helf_v850_segment_count_get(helf_file_input_t *input_file)
{
    return helf_v850_program_header_count_get(input_file);
}

bool helf_v850_segment_info_get(helf_file_input_t *input_file,size_t index,helf_v850_segment_info_t *info)
{
    if(!helf_v850_is_v850(input_file))
    {
        return false;
    }
    return helf_arch_segment_info_get(input_file,index,info);
}

bool helf_v850_is_load_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_v850_is_v850(input_file))
    {
        return false;
    }
    return helf_arch_is_load_segment(index,input_file);
}

bool helf_v850_is_rx_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_v850_is_v850(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_segment(index,input_file);
}

bool helf_v850_is_ro_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_v850_is_v850(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_segment(index,input_file);
}

bool helf_v850_is_rw_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_v850_is_v850(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_segment(index,input_file);
}

size_t helf_v850_load_segment_count_get(helf_file_input_t *input_file)
{
    if(!helf_v850_is_v850(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_count_get(input_file);
}

bool helf_v850_load_segment_info_get(helf_file_input_t *input_file,size_t index,helf_v850_segment_info_t *info)
{
    if(!helf_v850_is_v850(input_file))
    {
        return false;
    }
    return helf_arch_load_segment_info_get(input_file,index,info);
}

size_t helf_v850_load_segment_total_filesz_get(helf_file_input_t *input_file)
{
    if(!helf_v850_is_v850(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_filesz_get(input_file);
}

size_t helf_v850_load_segment_total_memsz_get(helf_file_input_t *input_file)
{
    if(!helf_v850_is_v850(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_memsz_get(input_file);
}

size_t helf_v850_relocation_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_v850_is_v850(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_section_count_get(input_file);
}

bool helf_v850_relocation_section_info_get(helf_file_input_t *input_file,size_t index,helf_arch_relocation_section_info_t *info)
{
    if(!helf_v850_is_v850(input_file))
    {
        return false;
    }
    return helf_arch_relocation_section_info_get(input_file,index,info);
}

size_t helf_v850_relocation_count_get(helf_file_input_t *input_file,size_t section_index)
{
    if(!helf_v850_is_v850(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_count_get(input_file,section_index);
}

bool helf_v850_relocation_get(helf_file_input_t *input_file,size_t section_index,size_t index,helf_arch_relocation_info_t *info)
{
    if(!helf_v850_is_v850(input_file))
    {
        return false;
    }
    if(!helf_arch_relocation_get(input_file,section_index,index,info))
    {
        return false;
    }
    return true;
}

bool helf_v850_relocation_symbol_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_symbol_info_t *info)
{
    if(!helf_v850_is_v850(input_file))
    {
        return false;
    }
    return helf_arch_relocation_symbol_info_get(input_file,relocation,info);
}

static const helf_arch_relocation_type_name_t helf_v850_relocation_type_names[]=
{
    {0,0,"R_V850_NONE"},
    {1,2,"R_V850_9_PCREL"},
    {2,4,"R_V850_22_PCREL"},
    {3,2,"R_V850_HI16_S"},
    {4,2,"R_V850_HI16"},
    {5,2,"R_V850_LO16"},
    {6,4,"R_V850_ABS32"},
    {7,2,"R_V850_16"},
    {8,1,"R_V850_8"},
    {9,2,"R_V850_SDA_16_16_OFFSET"},
    {10,2,"R_V850_SDA_15_16_OFFSET"},
    {11,2,"R_V850_ZDA_16_16_OFFSET"},
    {12,2,"R_V850_ZDA_15_16_OFFSET"},
    {13,2,"R_V850_TDA_6_8_OFFSET"},
    {14,2,"R_V850_TDA_7_8_OFFSET"},
    {15,2,"R_V850_TDA_7_7_OFFSET"},
    {16,2,"R_V850_TDA_16_16_OFFSET"},
    {17,2,"R_V850_TDA_4_5_OFFSET"},
    {18,2,"R_V850_TDA_4_4_OFFSET"},
    {19,4,"R_V850_SDA_16_16_SPLIT_OFFSET"},
    {20,4,"R_V850_ZDA_16_16_SPLIT_OFFSET"},
    {21,2,"R_V850_CALLT_6_7_OFFSET"},
    {22,2,"R_V850_CALLT_16_16_OFFSET"},
    {23,4,"R_V850_GNU_VTINHERIT"},
    {24,4,"R_V850_GNU_VTENTRY"},
    {25,4,"R_V850_LONGCALL"},
    {26,4,"R_V850_LONGJUMP"},
    {27,2,"R_V850_ALIGN"},
    {28,4,"R_V850_REL32"},
    {29,4,"R_V850_LO16_SPLIT_OFFSET"},
    {30,2,"R_V850_16_PCREL"},
    {31,4,"R_V850_17_PCREL"},
    {32,4,"R_V850_23"},
    {33,4,"R_V850_32_PCREL"},
    {34,4,"R_V850_32_ABS"},
    {35,0,"R_V850_16_SPLIT_OFFSET"},
    {36,2,"R_V850_16_S1"},
    {37,2,"R_V850_LO16_S1"},
    {38,2,"R_V850_CALLT_15_16_OFFSET"},
    {39,4,"R_V850_32_GOTPCREL"},
    {40,4,"R_V850_16_GOT"},
    {41,4,"R_V850_32_GOT"},
    {42,4,"R_V850_22_PLT"},
    {43,4,"R_V850_32_PLT"},
    {44,4,"R_V850_COPY"},
    {45,4,"R_V850_GLOB_DAT"},
    {46,4,"R_V850_JMP_SLOT"},
    {47,4,"R_V850_RELATIVE"},
    {48,4,"R_V850_16_GOTOFF"},
    {49,4,"R_V850_32_GOTOFF"},
    {50,2,"R_V850_CODE"},
    {51,2,"R_V850_DATA"},
    {52,0,"R_V810_WLO"},
    {53,0,"R_V810_WHI"},
    {54,0,"R_V810_WHI1"},
    {55,0,"R_V810_GPBYTE"},
    {56,0,"R_V810_GPHWORD"},
    {57,0,"R_V810_GPWORD"},
    {58,0,"R_V810_GPWLO"},
    {59,0,"R_V810_GPWHI"},
    {60,0,"R_V810_GPWHI1"},
    {61,0,"R_V850_HWLO"},
    {63,0,"R_V850_EP7BIT"},
    {64,0,"R_V850_EPHBYTE"},
    {65,0,"R_V850_EPWBYTE"},
    {66,0,"R_V850_REGHWLO"},
    {68,0,"R_V850_GPHWLO"},
    {70,0,"R_V850_PCR22"},
    {71,0,"R_V850_BLO"},
    {72,0,"R_V850_EP4BIT"},
    {73,0,"R_V850_EP5BIT"},
    {74,0,"R_V850_REGBLO"},
    {75,0,"R_V850_GPBLO"},
    {76,0,"R_V810_WLO_1"},
    {77,0,"R_V810_GPWLO_1"},
    {78,0,"R_V850_BLO_1"},
    {79,0,"R_V850_HWLO_1"},
    {81,0,"R_V850_GPBLO_1"},
    {82,0,"R_V850_GPHWLO_1"},
    {84,0,"R_V850_EPBLO"},
    {85,0,"R_V850_EPHWLO"},
    {87,0,"R_V850_EPWLO_N"},
    {88,4,"R_V850_PC32"},
    {89,0,"R_V850_W23BIT"},
    {90,0,"R_V850_GPW23BIT"},
    {91,0,"R_V850_EPW23BIT"},
    {92,0,"R_V850_B23BIT"},
    {93,0,"R_V850_GPB23BIT"},
    {94,0,"R_V850_EPB23BIT"},
    {95,0,"R_V850_PC16U"},
    {96,0,"R_V850_PC17"},
    {97,1,"R_V850_DW8"},
    {98,1,"R_V850_GPDW8"},
    {99,1,"R_V850_EPDW8"},
    {100,0,"R_V850_PC9"},
    {101,0,"R_V810_REGBYTE"},
    {102,0,"R_V810_REGHWORD"},
    {103,0,"R_V810_REGWORD"},
    {104,0,"R_V810_REGWLO"},
    {105,0,"R_V810_REGWHI"},
    {106,0,"R_V810_REGWHI1"},
    {107,0,"R_V850_REGW23BIT"},
    {108,0,"R_V850_REGB23BIT"},
    {109,1,"R_V850_REGDW8"},
    {110,0,"R_V810_EPBYTE"},
    {111,0,"R_V810_EPHWORD"},
    {112,0,"R_V810_EPWORD"},
    {113,0,"R_V850_WLO23"},
    {114,0,"R_V850_WORD_E"},
    {115,0,"R_V850_REGWORD_E"},
    {116,0,"R_V850_WORD"},
    {117,0,"R_V850_GPWORD"},
    {118,0,"R_V850_REGWORD"},
    {119,0,"R_V850_EPWORD"},
    {120,0,"R_V810_TPBYTE"},
    {121,0,"R_V810_TPHWORD"},
    {122,0,"R_V810_TPWORD"},
    {123,0,"R_V810_TPWLO"},
    {124,0,"R_V810_TPWHI"},
    {125,0,"R_V810_TPWHI1"},
    {126,0,"R_V850_TPHWLO"},
    {127,0,"R_V850_TPBLO"},
    {128,0,"R_V810_TPWLO_1"},
    {129,0,"R_V850_TPBLO_1"},
    {130,0,"R_V850_TPHWLO_1"},
    {131,0,"R_V850_TP23BIT"},
    {132,0,"R_V850_TPW23BIT"},
    {133,1,"R_V850_TPDW8"},
    {160,4,"R_V810_ABS32"},
    {224,0,"R_V850_SYM"},
    {225,0,"R_V850_OPadd"},
    {226,0,"R_V850_OPsub"},
    {227,0,"R_V850_OPsctsize"},
    {228,0,"R_V850_OPscttop"},
};

const char *helf_v850_relocation_type_name_get(uint32_t type)
{
    return helf_arch_relocation_type_name_get(helf_v850_relocation_type_names,sizeof(helf_v850_relocation_type_names)/sizeof(helf_v850_relocation_type_names[0]),type);
}

uint8_t helf_v850_relocation_size_get(uint32_t type)
{
    return helf_arch_relocation_type_size_get(helf_v850_relocation_type_names,sizeof(helf_v850_relocation_type_names)/sizeof(helf_v850_relocation_type_names[0]),type);
}

static const helf_arch_section_type_name_t helf_v850_section_type_names[]=
{
    {1879048192,"SHT_V850_SCOMMON"},
    {1879048193,"SHT_V850_TCOMMON"},
    {1879048194,"SHT_V850_ZCOMMON"},
    {2147483648,"SHT_RENESAS_IOP"},
    {2684354560,"SHT_RENESAS_INFO"},
};

const char *helf_v850_section_type_name_get(uint32_t type)
{
    return helf_arch_section_type_name_get(helf_v850_section_type_names,sizeof(helf_v850_section_type_names)/sizeof(helf_v850_section_type_names[0]),type);
}

bool helf_v850_relocation_address_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_relocation_address_info_t *info)
{
    if(!helf_v850_is_v850(input_file) || relocation==NULL)
    {
        return false;
    }
    uint8_t size=helf_v850_relocation_size_get(relocation->type);
    if(size==0)
    {
        return false;
    }
    return helf_arch_relocation_address_info_get(input_file,relocation,size,info);
}

size_t helf_v850_relocation_data_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,void *buffer,size_t buffer_length)
{
    if(!helf_v850_is_v850(input_file) || relocation==NULL)
    {
        return 0;
    }
    uint8_t size=helf_v850_relocation_size_get(relocation->type);
    if(size==0)
    {
        return 0;
    }
    return helf_arch_relocation_data_get(input_file,relocation,size,buffer,buffer_length);
}

bool helf_v850_is_shared_object(helf_file_input_t *input_file)
{
    return helf_arch_is_shared_object(input_file,helf_v850_machines,sizeof(helf_v850_machines)/sizeof(helf_v850_machines[0]),HELF_ELFCLASS_32);
}

bool helf_v850_is_relocatable(helf_file_input_t *input_file)
{
    return helf_arch_is_relocatable(input_file,helf_v850_machines,sizeof(helf_v850_machines)/sizeof(helf_v850_machines[0]),HELF_ELFCLASS_32);
}

size_t helf_v850_dynamic_count_get(helf_file_input_t *input_file)
{
    if(!helf_v850_is_v850(input_file))
    {
        return 0;
    }
    return helf_arch_dynamic_count_get(input_file);
}

bool helf_v850_dynamic_info_get(helf_file_input_t *input_file,size_t index,helf_arch_dynamic_info_t *info)
{
    if(!helf_v850_is_v850(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_info_get(input_file,index,info);
}

bool helf_v850_dynamic_string_get(helf_file_input_t *input_file,uint64_t offset,char *buffer,size_t buffer_length)
{
    if(!helf_v850_is_v850(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_string_get(input_file,offset,buffer,buffer_length);
}

/* helf_relocation_howto_begin */
static const helf_arch_relocation_howto_t helf_v850_reloc_howto[]=
{
    {0x00000000u,0,0,0,0,0x0040,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000001u,2,9,0,0,0x0013,0x0000000000FFFFFFULL,0x0000000000FFFFFFULL},
    {0x00000002u,4,22,0,0,0x0013,0x0000000007FFFF80ULL,0x0000000007FFFF80ULL},
    {0x00000003u,2,16,0,0,0x0010,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000004u,2,16,0,0,0x0010,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000005u,2,16,0,0,0x0010,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000006u,4,32,0,0,0x0010,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000007u,2,16,0,0,0x0000,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000008u,1,8,0,0,0x0000,0x00000000000000FFULL,0x00000000000000FFULL},
    {0x00000009u,2,16,0,0,0x0010,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x0000000Au,2,16,1,1,0x0010,0x000000000000FFFEULL,0x000000000000FFFEULL},
    {0x0000000Bu,2,16,0,0,0x0010,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x0000000Cu,2,16,1,1,0x0010,0x000000000000FFFEULL,0x000000000000FFFEULL},
    {0x0000000Du,2,8,2,1,0x0010,0x000000000000007EULL,0x000000000000007EULL},
    {0x0000000Eu,2,8,1,0,0x0010,0x000000000000007FULL,0x000000000000007FULL},
    {0x0000000Fu,2,7,0,0,0x0010,0x000000000000007FULL,0x000000000000007FULL},
    {0x00000010u,2,16,0,0,0x0010,0x000000000000FFFFULL,0x0000000000000FFFULL},
    {0x00000011u,2,5,1,0,0x0010,0x000000000000000FULL,0x000000000000000FULL},
    {0x00000012u,2,4,0,0,0x0010,0x000000000000000FULL,0x000000000000000FULL},
    {0x00000013u,4,16,0,0,0x0010,0x00000000FFFE0020ULL,0x00000000FFFE0020ULL},
    {0x00000014u,4,16,0,0,0x0010,0x00000000FFFE0020ULL,0x00000000FFFE0020ULL},
    {0x00000015u,2,7,0,0,0x0010,0x000000000000003FULL,0x000000000000003FULL},
    {0x00000016u,2,16,0,0,0x0010,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000017u,4,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000018u,4,0,0,0,0x0010,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000019u,4,32,0,0,0x0013,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Au,4,32,0,0,0x0013,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Bu,2,0,0,0,0x0012,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Cu,4,32,0,0,0x0011,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x0000001Du,4,16,0,0,0x0010,0x00000000FFFE0020ULL,0x00000000FFFE0020ULL},
    {0x0000001Eu,2,16,0,0,0x0013,0x000000000000FFFEULL,0x000000000000FFFEULL},
    {0x0000001Fu,4,17,0,0,0x0013,0x000000000010FFFEULL,0x000000000010FFFEULL},
    {0x00000020u,4,23,0,0,0x0010,0x00000000FFFF07F0ULL,0x00000000FFFF07F0ULL},
    {0x00000021u,4,32,1,1,0x0013,0x00000000FFFFFFFEULL,0x00000000FFFFFFFEULL},
    {0x00000022u,4,32,1,1,0x0011,0x00000000FFFFFFFEULL,0x00000000FFFFFFFEULL},
    {0x00000023u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000024u,2,16,1,1,0x0010,0x000000000000FFFEULL,0x000000000000FFFEULL},
    {0x00000025u,2,16,1,1,0x0010,0x000000000000FFFEULL,0x000000000000FFFEULL},
    {0x00000026u,2,16,1,1,0x0010,0x000000000000FFFEULL,0x000000000000FFFEULL},
    {0x00000027u,4,32,0,0,0x0613,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000028u,4,16,0,0,0x0200,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000029u,4,32,0,0,0x0200,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x0000002Au,4,22,1,7,0x0003,0x0000000007FFFF80ULL,0x0000000007FFFF80ULL},
    {0x0000002Bu,4,32,1,1,0x0003,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x0000002Cu,4,32,0,0,0x0180,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x0000002Du,4,32,0,0,0x0100,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x0000002Eu,4,32,0,0,0x0140,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x0000002Fu,4,32,0,0,0x0080,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000030u,4,16,0,0,0x0200,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000031u,4,32,0,0,0x0200,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000032u,2,0,0,0,0x0012,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000033u,2,0,0,0,0x0012,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000034u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000035u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000036u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000037u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000038u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000039u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000003Au,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000003Bu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000003Cu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000003Du,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000003Fu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000040u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000041u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000042u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000044u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000046u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000047u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000048u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000049u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000004Au,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000004Bu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000004Cu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000004Du,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000004Eu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000004Fu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000051u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000052u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000054u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000055u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000057u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000058u,4,32,0,0,0x0003,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000059u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000005Au,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000005Bu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000005Cu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000005Du,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000005Eu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000005Fu,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000060u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000061u,1,8,0,0,0x0000,0x0000000000000000ULL,0x00000000000000FFULL},
    {0x00000062u,1,8,0,0,0x0000,0x0000000000000000ULL,0x00000000000000FFULL},
    {0x00000063u,1,8,0,0,0x0000,0x0000000000000000ULL,0x00000000000000FFULL},
    {0x00000064u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000065u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000066u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000067u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000068u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000069u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000006Au,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000006Bu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000006Cu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000006Du,1,8,0,0,0x0000,0x0000000000000000ULL,0x00000000000000FFULL},
    {0x0000006Eu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000006Fu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000070u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000071u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000072u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000073u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000074u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000075u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000076u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000077u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000078u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000079u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000007Au,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000007Bu,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000007Cu,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000007Du,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000007Eu,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000007Fu,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000080u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000081u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000082u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000083u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000084u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000085u,1,8,0,0,0x01C0,0x0000000000000000ULL,0x00000000000000FFULL},
    {0x000000A0u,4,32,0,0,0x0000,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x000000E0u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000E1u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000E2u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000E3u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000E4u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
};

bool helf_v850_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto)
{
    if(howto==NULL)
    {
        return false;
    }
    size_t count=sizeof(helf_v850_reloc_howto)/sizeof(helf_v850_reloc_howto[0]);
    if(count==0)
    {
        return false;
    }
    for(size_t i=0;i<count;i++)
    {
        if(helf_v850_reloc_howto[i].type==type)
        {
            *howto=helf_v850_reloc_howto[i];
            return true;
        }
    }
    return false;
}
/* helf_relocation_howto_end */

