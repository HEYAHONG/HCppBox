/***************************************************************
 * Name:      helf_arch_nds32.c
 * Purpose:   实现helf_arch_nds32接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "helf_arch_nds32.h"

static const uint16_t helf_nds32_machines[]=
{
    HELF_EM_NDS32,
};

bool helf_nds32_is_nds32(helf_file_input_t *input_file)
{
    return helf_arch_is(input_file,helf_nds32_machines,sizeof(helf_nds32_machines)/sizeof(helf_nds32_machines[0]),HELF_ELFCLASS_32);
}

bool helf_nds32_is_big_endian(helf_file_input_t *input_file)
{
    if(!helf_nds32_is_nds32(input_file))
    {
        return false;
    }
    return helf_file_input_is_big_endian(input_file);
}

bool helf_nds32_is_executable(helf_file_input_t *input_file)
{
    return helf_arch_is_executable(input_file,helf_nds32_machines,sizeof(helf_nds32_machines)/sizeof(helf_nds32_machines[0]),HELF_ELFCLASS_32);
}

bool helf_nds32_entry_get(helf_file_input_t *input_file,uint32_t *entry)
{
    if(entry==NULL)
    {
        return false;
    }
    if(!helf_nds32_is_nds32(input_file))
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

size_t helf_nds32_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_nds32_is_nds32(input_file))
    {
        return 0;
    }
    return helf_arch_section_count_get(input_file);
}

bool helf_nds32_section_info_get(helf_file_input_t *input_file,size_t index,helf_nds32_section_info_t *info)
{
    if(!helf_nds32_is_nds32(input_file))
    {
        return false;
    }
    return helf_arch_section_info_get(input_file,index,info);
}

bool helf_nds32_is_rx_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_nds32_is_nds32(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_section(index,input_file);
}

intptr_t helf_nds32_rx_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_nds32_is_rx_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rx_section_offset_get(index,input_file);
}

size_t helf_nds32_rx_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_nds32_is_nds32(input_file))
    {
        return 0;
    }
    return helf_arch_rx_section_size_get(input_file);
}

bool helf_nds32_is_ro_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_nds32_is_nds32(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_section(index,input_file);
}

intptr_t helf_nds32_ro_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_nds32_is_ro_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_ro_section_offset_get(index,input_file);
}

size_t helf_nds32_ro_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_nds32_is_nds32(input_file))
    {
        return 0;
    }
    return helf_arch_ro_section_size_get(input_file);
}

bool helf_nds32_is_rw_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_nds32_is_nds32(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_section(index,input_file);
}

intptr_t helf_nds32_rw_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_nds32_is_rw_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rw_section_offset_get(index,input_file);
}

size_t helf_nds32_rw_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_nds32_is_nds32(input_file))
    {
        return 0;
    }
    return helf_arch_rw_section_size_get(input_file);
}

bool helf_nds32_is_bss_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_nds32_is_nds32(input_file))
    {
        return false;
    }
    return helf_arch_is_bss_section(index,input_file);
}

size_t helf_nds32_program_header_count_get(helf_file_input_t *input_file)
{
    if(!helf_nds32_is_nds32(input_file))
    {
        return 0;
    }
    return helf_arch_program_header_count_get(input_file);
}

bool helf_nds32_program_header_get(helf_file_input_t *input_file,size_t index,helf_elf32_program_header_t *programheader)
{
    if(programheader==NULL)
    {
        return false;
    }
    if(!helf_nds32_is_nds32(input_file))
    {
        return false;
    }
    if(index>=helf_arch_program_header_count_get(input_file))
    {
        return false;
    }
    return helf_file_input_32_bits_program_header_get(input_file,index,programheader);
}

size_t helf_nds32_segment_count_get(helf_file_input_t *input_file)
{
    return helf_nds32_program_header_count_get(input_file);
}

bool helf_nds32_segment_info_get(helf_file_input_t *input_file,size_t index,helf_nds32_segment_info_t *info)
{
    if(!helf_nds32_is_nds32(input_file))
    {
        return false;
    }
    return helf_arch_segment_info_get(input_file,index,info);
}

bool helf_nds32_is_load_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_nds32_is_nds32(input_file))
    {
        return false;
    }
    return helf_arch_is_load_segment(index,input_file);
}

bool helf_nds32_is_rx_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_nds32_is_nds32(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_segment(index,input_file);
}

bool helf_nds32_is_ro_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_nds32_is_nds32(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_segment(index,input_file);
}

bool helf_nds32_is_rw_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_nds32_is_nds32(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_segment(index,input_file);
}

size_t helf_nds32_load_segment_count_get(helf_file_input_t *input_file)
{
    if(!helf_nds32_is_nds32(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_count_get(input_file);
}

bool helf_nds32_load_segment_info_get(helf_file_input_t *input_file,size_t index,helf_nds32_segment_info_t *info)
{
    if(!helf_nds32_is_nds32(input_file))
    {
        return false;
    }
    return helf_arch_load_segment_info_get(input_file,index,info);
}

size_t helf_nds32_load_segment_total_filesz_get(helf_file_input_t *input_file)
{
    if(!helf_nds32_is_nds32(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_filesz_get(input_file);
}

size_t helf_nds32_load_segment_total_memsz_get(helf_file_input_t *input_file)
{
    if(!helf_nds32_is_nds32(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_memsz_get(input_file);
}

size_t helf_nds32_relocation_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_nds32_is_nds32(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_section_count_get(input_file);
}

bool helf_nds32_relocation_section_info_get(helf_file_input_t *input_file,size_t index,helf_arch_relocation_section_info_t *info)
{
    if(!helf_nds32_is_nds32(input_file))
    {
        return false;
    }
    return helf_arch_relocation_section_info_get(input_file,index,info);
}

size_t helf_nds32_relocation_count_get(helf_file_input_t *input_file,size_t section_index)
{
    if(!helf_nds32_is_nds32(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_count_get(input_file,section_index);
}

bool helf_nds32_relocation_get(helf_file_input_t *input_file,size_t section_index,size_t index,helf_arch_relocation_info_t *info)
{
    if(!helf_nds32_is_nds32(input_file))
    {
        return false;
    }
    if(!helf_arch_relocation_get(input_file,section_index,index,info))
    {
        return false;
    }
    return true;
}

bool helf_nds32_relocation_symbol_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_symbol_info_t *info)
{
    if(!helf_nds32_is_nds32(input_file))
    {
        return false;
    }
    return helf_arch_relocation_symbol_info_get(input_file,relocation,info);
}

static const helf_arch_relocation_type_name_t helf_nds32_relocation_type_names[]=
{
    {0,0,"R_NDS32_NONE"},
    {1,2,"R_NDS32_16"},
    {2,4,"R_NDS32_32"},
    {3,0,"R_NDS32_20"},
    {4,0,"R_NDS32_9_PCREL"},
    {5,0,"R_NDS32_15_PCREL"},
    {6,0,"R_NDS32_17_PCREL"},
    {7,0,"R_NDS32_25_PCREL"},
    {8,0,"R_NDS32_HI20"},
    {9,0,"R_NDS32_LO12S3"},
    {10,0,"R_NDS32_LO12S2"},
    {11,0,"R_NDS32_LO12S1"},
    {12,0,"R_NDS32_LO12S0"},
    {13,0,"R_NDS32_SDA15S3"},
    {14,0,"R_NDS32_SDA15S2"},
    {15,0,"R_NDS32_SDA15S1"},
    {16,0,"R_NDS32_SDA15S0"},
    {17,0,"R_NDS32_GNU_VTINHERIT"},
    {18,0,"R_NDS32_GNU_VTENTRY"},
    {19,0,"R_NDS32_16_RELA"},
    {20,0,"R_NDS32_32_RELA"},
    {21,0,"R_NDS32_20_RELA"},
    {22,0,"R_NDS32_9_PCREL_RELA"},
    {23,0,"R_NDS32_15_PCREL_RELA"},
    {24,0,"R_NDS32_17_PCREL_RELA"},
    {25,0,"R_NDS32_25_PCREL_RELA"},
    {26,0,"R_NDS32_HI20_RELA"},
    {27,0,"R_NDS32_LO12S3_RELA"},
    {28,0,"R_NDS32_LO12S2_RELA"},
    {29,0,"R_NDS32_LO12S1_RELA"},
    {30,0,"R_NDS32_LO12S0_RELA"},
    {31,0,"R_NDS32_SDA15S3_RELA"},
    {32,0,"R_NDS32_SDA15S2_RELA"},
    {33,0,"R_NDS32_SDA15S1_RELA"},
    {34,0,"R_NDS32_SDA15S0_RELA"},
    {35,0,"R_NDS32_RELA_GNU_VTINHERIT"},
    {36,0,"R_NDS32_RELA_GNU_VTENTRY"},
    {37,0,"R_NDS32_GOT20"},
    {38,0,"R_NDS32_25_PLTREL"},
    {39,0,"R_NDS32_COPY"},
    {40,0,"R_NDS32_GLOB_DAT"},
    {41,0,"R_NDS32_JMP_SLOT"},
    {42,0,"R_NDS32_RELATIVE"},
    {43,0,"R_NDS32_GOTOFF"},
    {44,0,"R_NDS32_GOTPC20"},
    {45,0,"R_NDS32_GOT_HI20"},
    {46,0,"R_NDS32_GOT_LO12"},
    {47,0,"R_NDS32_GOTPC_HI20"},
    {48,0,"R_NDS32_GOTPC_LO12"},
    {49,0,"R_NDS32_GOTOFF_HI20"},
    {50,0,"R_NDS32_GOTOFF_LO12"},
    {51,2,"R_NDS32_INSN16"},
    {52,0,"R_NDS32_LABEL"},
    {53,0,"R_NDS32_LONGCALL1"},
    {54,0,"R_NDS32_LONGCALL2"},
    {55,0,"R_NDS32_LONGCALL3"},
    {56,0,"R_NDS32_LONGJUMP1"},
    {57,0,"R_NDS32_LONGJUMP2"},
    {58,0,"R_NDS32_LONGJUMP3"},
    {59,0,"R_NDS32_LOADSTORE"},
    {60,0,"R_NDS32_9_FIXED_RELA"},
    {61,0,"R_NDS32_15_FIXED_RELA"},
    {62,0,"R_NDS32_17_FIXED_RELA"},
    {63,0,"R_NDS32_25_FIXED_RELA"},
    {64,0,"R_NDS32_PLTREL_HI20"},
    {65,0,"R_NDS32_PLTREL_LO12"},
    {66,0,"R_NDS32_PLT_GOTREL_HI20"},
    {67,0,"R_NDS32_PLT_GOTREL_LO12"},
    {68,0,"R_NDS32_SDA12S2_DP_RELA"},
    {69,0,"R_NDS32_SDA12S2_SP_RELA"},
    {70,0,"R_NDS32_LO12S2_DP_RELA"},
    {71,0,"R_NDS32_LO12S2_SP_RELA"},
    {72,0,"R_NDS32_LO12S0_ORI_RELA"},
    {73,0,"R_NDS32_SDA16S3_RELA"},
    {74,0,"R_NDS32_SDA17S2_RELA"},
    {75,0,"R_NDS32_SDA18S1_RELA"},
    {76,0,"R_NDS32_SDA19S0_RELA"},
    {77,0,"R_NDS32_DWARF2_OP1_RELA"},
    {78,0,"R_NDS32_DWARF2_OP2_RELA"},
    {79,0,"R_NDS32_DWARF2_LEB_RELA"},
    {80,0,"R_NDS32_UPDATE_TA_RELA"},
    {81,0,"R_NDS32_9_PLTREL"},
    {82,0,"R_NDS32_PLT_GOTREL_LO20"},
    {83,0,"R_NDS32_PLT_GOTREL_LO15"},
    {84,0,"R_NDS32_PLT_GOTREL_LO19"},
    {85,0,"R_NDS32_GOT_LO15"},
    {86,0,"R_NDS32_GOT_LO19"},
    {87,0,"R_NDS32_GOTOFF_LO15"},
    {88,0,"R_NDS32_GOTOFF_LO19"},
    {89,0,"R_NDS32_GOT15S2_RELA"},
    {90,0,"R_NDS32_GOT17S2_RELA"},
    {91,0,"R_NDS32_5_RELA"},
    {92,0,"R_NDS32_10_UPCREL_RELA"},
    {93,0,"R_NDS32_SDA_FP7U2_RELA"},
    {94,0,"R_NDS32_WORD_9_PCREL_RELA"},
    {95,0,"R_NDS32_25_ABS_RELA"},
    {96,0,"R_NDS32_17IFC_PCREL_RELA"},
    {97,0,"R_NDS32_10IFCU_PCREL_RELA"},
    {98,0,"R_NDS32_TLS_LE_HI20"},
    {99,0,"R_NDS32_TLS_LE_LO12"},
    {100,0,"R_NDS32_TLS_IE_HI20"},
    {101,0,"R_NDS32_TLS_IE_LO12S2"},
    {102,0,"R_NDS32_TLS_TPOFF"},
    {103,0,"R_NDS32_TLS_LE_20"},
    {104,0,"R_NDS32_TLS_LE_15S0"},
    {105,0,"R_NDS32_TLS_LE_15S1"},
    {106,0,"R_NDS32_TLS_LE_15S2"},
    {107,0,"R_NDS32_LONGCALL4"},
    {108,0,"R_NDS32_LONGCALL5"},
    {109,0,"R_NDS32_LONGCALL6"},
    {110,0,"R_NDS32_LONGJUMP4"},
    {111,0,"R_NDS32_LONGJUMP5"},
    {112,0,"R_NDS32_LONGJUMP6"},
    {113,0,"R_NDS32_LONGJUMP7"},
    {115,0,"R_NDS32_TLS_IE_LO12"},
    {116,0,"R_NDS32_TLS_IEGP_HI20"},
    {117,0,"R_NDS32_TLS_IEGP_LO12"},
    {118,0,"R_NDS32_TLS_IEGP_LO12S2"},
    {119,0,"R_NDS32_TLS_DESC"},
    {120,0,"R_NDS32_TLS_DESC_HI20"},
    {121,0,"R_NDS32_TLS_DESC_LO12"},
    {122,0,"R_NDS32_TLS_DESC_20"},
    {123,0,"R_NDS32_TLS_DESC_SDA17S2"},
    {192,0,"R_NDS32_RELAX_ENTRY"},
    {193,0,"R_NDS32_GOT_SUFF"},
    {194,0,"R_NDS32_GOTOFF_SUFF"},
    {195,0,"R_NDS32_PLT_GOT_SUFF"},
    {196,0,"R_NDS32_MULCALL_SUFF"},
    {197,0,"R_NDS32_PTR"},
    {198,0,"R_NDS32_PTR_COUNT"},
    {199,0,"R_NDS32_PTR_RESOLVED"},
    {200,0,"R_NDS32_PLTBLOCK"},
    {201,0,"R_NDS32_RELAX_REGION_BEGIN"},
    {202,0,"R_NDS32_RELAX_REGION_END"},
    {203,0,"R_NDS32_MINUEND"},
    {204,0,"R_NDS32_SUBTRAHEND"},
    {205,1,"R_NDS32_DIFF8"},
    {206,2,"R_NDS32_DIFF16"},
    {207,4,"R_NDS32_DIFF32"},
    {208,1,"R_NDS32_DIFF_ULEB128"},
    {209,0,"R_NDS32_DATA"},
    {210,0,"R_NDS32_TRAN"},
    {211,0,"R_NDS32_TLS_LE_ADD"},
    {212,0,"R_NDS32_TLS_LE_LS"},
    {213,0,"R_NDS32_EMPTY"},
    {214,0,"R_NDS32_TLS_DESC_ADD"},
    {215,0,"R_NDS32_TLS_DESC_FUNC"},
    {216,0,"R_NDS32_TLS_DESC_CALL"},
    {217,0,"R_NDS32_TLS_DESC_MEM"},
    {218,0,"R_NDS32_RELAX_REMOVE"},
    {219,0,"R_NDS32_RELAX_GROUP"},
    {220,0,"R_NDS32_TLS_IEGP_LW"},
    {221,0,"R_NDS32_LSI"},
};

const char *helf_nds32_relocation_type_name_get(uint32_t type)
{
    return helf_arch_relocation_type_name_get(helf_nds32_relocation_type_names,sizeof(helf_nds32_relocation_type_names)/sizeof(helf_nds32_relocation_type_names[0]),type);
}

uint8_t helf_nds32_relocation_size_get(uint32_t type)
{
    return helf_arch_relocation_type_size_get(helf_nds32_relocation_type_names,sizeof(helf_nds32_relocation_type_names)/sizeof(helf_nds32_relocation_type_names[0]),type);
}

const char *helf_nds32_section_type_name_get(uint32_t type)
{
    (void)type;
    return NULL;
}

bool helf_nds32_relocation_address_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_relocation_address_info_t *info)
{
    if(!helf_nds32_is_nds32(input_file) || relocation==NULL)
    {
        return false;
    }
    uint8_t size=helf_nds32_relocation_size_get(relocation->type);
    if(size==0)
    {
        return false;
    }
    return helf_arch_relocation_address_info_get(input_file,relocation,size,info);
}

size_t helf_nds32_relocation_data_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,void *buffer,size_t buffer_length)
{
    if(!helf_nds32_is_nds32(input_file) || relocation==NULL)
    {
        return 0;
    }
    uint8_t size=helf_nds32_relocation_size_get(relocation->type);
    if(size==0)
    {
        return 0;
    }
    return helf_arch_relocation_data_get(input_file,relocation,size,buffer,buffer_length);
}

bool helf_nds32_is_shared_object(helf_file_input_t *input_file)
{
    return helf_arch_is_shared_object(input_file,helf_nds32_machines,sizeof(helf_nds32_machines)/sizeof(helf_nds32_machines[0]),HELF_ELFCLASS_32);
}

bool helf_nds32_is_relocatable(helf_file_input_t *input_file)
{
    return helf_arch_is_relocatable(input_file,helf_nds32_machines,sizeof(helf_nds32_machines)/sizeof(helf_nds32_machines[0]),HELF_ELFCLASS_32);
}

size_t helf_nds32_dynamic_count_get(helf_file_input_t *input_file)
{
    if(!helf_nds32_is_nds32(input_file))
    {
        return 0;
    }
    return helf_arch_dynamic_count_get(input_file);
}

bool helf_nds32_dynamic_info_get(helf_file_input_t *input_file,size_t index,helf_arch_dynamic_info_t *info)
{
    if(!helf_nds32_is_nds32(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_info_get(input_file,index,info);
}

bool helf_nds32_dynamic_string_get(helf_file_input_t *input_file,uint64_t offset,char *buffer,size_t buffer_length)
{
    if(!helf_nds32_is_nds32(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_string_get(input_file,offset,buffer,buffer_length);
}

/* helf_relocation_howto_begin */
static const helf_arch_relocation_howto_t helf_nds32_reloc_howto[]=
{
    {0x00000000u,0,0,0,0,0x0040,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000001u,2,16,0,0,0x0000,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000002u,4,32,0,0,0x0000,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000003u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000004u,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000005u,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000006u,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000007u,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000008u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000009u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000000Au,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000000Bu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000000Cu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000000Du,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000000Eu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000000Fu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000010u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000011u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000012u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000013u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000014u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000015u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000016u,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000017u,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000018u,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000019u,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Au,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Bu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Cu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Du,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Eu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Fu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000020u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000021u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000022u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000023u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000024u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000025u,0,0,0,0,0x0200,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000026u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000027u,0,0,0,0,0x0180,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000028u,0,0,0,0,0x0100,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000029u,0,0,0,0,0x0140,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000002Au,0,0,0,0,0x0080,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000002Bu,0,0,0,0,0x0200,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000002Cu,0,0,0,0,0x0200,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000002Du,0,0,0,0,0x0200,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000002Eu,0,0,0,0,0x0200,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000002Fu,0,0,0,0,0x0200,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000030u,0,0,0,0,0x0200,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000031u,0,0,0,0,0x0200,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000032u,0,0,0,0,0x0200,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000033u,2,16,0,0,0x0000,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000034u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000035u,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000036u,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000037u,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000038u,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000039u,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000003Au,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000003Bu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000003Cu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000003Du,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000003Eu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000003Fu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000040u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000041u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000042u,0,0,0,0,0x0200,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000043u,0,0,0,0,0x0200,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000044u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000045u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
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
    {0x00000050u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000051u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000052u,0,0,0,0,0x0200,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000053u,0,0,0,0,0x0200,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000054u,0,0,0,0,0x0200,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000055u,0,0,0,0,0x0200,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000056u,0,0,0,0,0x0200,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000057u,0,0,0,0,0x0200,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000058u,0,0,0,0,0x0200,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000059u,0,0,0,0,0x0200,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000005Au,0,0,0,0,0x0200,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000005Bu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000005Cu,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000005Du,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000005Eu,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000005Fu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000060u,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000061u,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000062u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000063u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000064u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000065u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000066u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000067u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000068u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000069u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000006Au,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000006Bu,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000006Cu,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000006Du,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000006Eu,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000006Fu,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000070u,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000071u,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000073u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000074u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000075u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000076u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000077u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000078u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000079u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000007Au,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000007Bu,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000C0u,0,0,0,0,0x0040,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000C1u,0,0,0,0,0x0200,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000C2u,0,0,0,0,0x0200,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000C3u,0,0,0,0,0x0200,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000C4u,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000C5u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000C6u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000C7u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000C8u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000C9u,0,0,0,0,0x0040,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000CAu,0,0,0,0,0x0040,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000CBu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000CCu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000CDu,1,8,0,0,0x0000,0x0000000000000000ULL,0x00000000000000FFULL},
    {0x000000CEu,2,16,0,0,0x0000,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x000000CFu,4,32,0,0,0x0000,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x000000D0u,1,8,0,0,0x0000,0x0000000000000000ULL,0x00000000000000FFULL},
    {0x000000D1u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000D2u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000D3u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000D4u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000D5u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000D6u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000D7u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000D8u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000D9u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000DAu,0,0,0,0,0x0040,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000DBu,0,0,0,0,0x0040,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000DCu,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000DDu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
};

bool helf_nds32_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto)
{
    if(howto==NULL)
    {
        return false;
    }
    size_t count=sizeof(helf_nds32_reloc_howto)/sizeof(helf_nds32_reloc_howto[0]);
    if(count==0)
    {
        return false;
    }
    for(size_t i=0;i<count;i++)
    {
        if(helf_nds32_reloc_howto[i].type==type)
        {
            *howto=helf_nds32_reloc_howto[i];
            return true;
        }
    }
    return false;
}
/* helf_relocation_howto_end */


