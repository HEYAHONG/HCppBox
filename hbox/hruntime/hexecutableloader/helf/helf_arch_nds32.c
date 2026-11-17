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
