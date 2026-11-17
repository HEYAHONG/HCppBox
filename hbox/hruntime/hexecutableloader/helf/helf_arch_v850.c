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
