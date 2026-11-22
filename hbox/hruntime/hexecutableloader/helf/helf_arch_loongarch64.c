/***************************************************************
 * Name:      helf_arch_loongarch64.c
 * Purpose:   实现helf_arch_loongarch64接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "helf_arch_loongarch64.h"

static const uint16_t helf_loongarch64_machines[]=
{
    HELF_EM_LOONGARCH,
};

bool helf_loongarch64_is_loongarch64(helf_file_input_t *input_file)
{
    return helf_arch_is(input_file,helf_loongarch64_machines,sizeof(helf_loongarch64_machines)/sizeof(helf_loongarch64_machines[0]),HELF_ELFCLASS_64);
}

bool helf_loongarch64_is_big_endian(helf_file_input_t *input_file)
{
    if(!helf_loongarch64_is_loongarch64(input_file))
    {
        return false;
    }
    return helf_file_input_is_big_endian(input_file);
}

bool helf_loongarch64_is_executable(helf_file_input_t *input_file)
{
    return helf_arch_is_executable(input_file,helf_loongarch64_machines,sizeof(helf_loongarch64_machines)/sizeof(helf_loongarch64_machines[0]),HELF_ELFCLASS_64);
}

bool helf_loongarch64_entry_get(helf_file_input_t *input_file,uint64_t *entry)
{
    if(entry==NULL)
    {
        return false;
    }
    if(!helf_loongarch64_is_loongarch64(input_file))
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

size_t helf_loongarch64_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_loongarch64_is_loongarch64(input_file))
    {
        return 0;
    }
    return helf_arch_section_count_get(input_file);
}

bool helf_loongarch64_section_info_get(helf_file_input_t *input_file,size_t index,helf_loongarch64_section_info_t *info)
{
    if(!helf_loongarch64_is_loongarch64(input_file))
    {
        return false;
    }
    return helf_arch_section_info_get(input_file,index,info);
}

bool helf_loongarch64_is_rx_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_loongarch64_is_loongarch64(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_section(index,input_file);
}

intptr_t helf_loongarch64_rx_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_loongarch64_is_rx_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rx_section_offset_get(index,input_file);
}

size_t helf_loongarch64_rx_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_loongarch64_is_loongarch64(input_file))
    {
        return 0;
    }
    return helf_arch_rx_section_size_get(input_file);
}

bool helf_loongarch64_is_ro_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_loongarch64_is_loongarch64(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_section(index,input_file);
}

intptr_t helf_loongarch64_ro_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_loongarch64_is_ro_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_ro_section_offset_get(index,input_file);
}

size_t helf_loongarch64_ro_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_loongarch64_is_loongarch64(input_file))
    {
        return 0;
    }
    return helf_arch_ro_section_size_get(input_file);
}

bool helf_loongarch64_is_rw_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_loongarch64_is_loongarch64(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_section(index,input_file);
}

intptr_t helf_loongarch64_rw_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_loongarch64_is_rw_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rw_section_offset_get(index,input_file);
}

size_t helf_loongarch64_rw_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_loongarch64_is_loongarch64(input_file))
    {
        return 0;
    }
    return helf_arch_rw_section_size_get(input_file);
}

bool helf_loongarch64_is_bss_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_loongarch64_is_loongarch64(input_file))
    {
        return false;
    }
    return helf_arch_is_bss_section(index,input_file);
}

size_t helf_loongarch64_program_header_count_get(helf_file_input_t *input_file)
{
    if(!helf_loongarch64_is_loongarch64(input_file))
    {
        return 0;
    }
    return helf_arch_program_header_count_get(input_file);
}

bool helf_loongarch64_program_header_get(helf_file_input_t *input_file,size_t index,helf_elf64_program_header_t *programheader)
{
    if(programheader==NULL)
    {
        return false;
    }
    if(!helf_loongarch64_is_loongarch64(input_file))
    {
        return false;
    }
    if(index>=helf_arch_program_header_count_get(input_file))
    {
        return false;
    }
    return helf_file_input_64_bits_program_header_get(input_file,index,programheader);
}

size_t helf_loongarch64_segment_count_get(helf_file_input_t *input_file)
{
    return helf_loongarch64_program_header_count_get(input_file);
}

bool helf_loongarch64_segment_info_get(helf_file_input_t *input_file,size_t index,helf_loongarch64_segment_info_t *info)
{
    if(!helf_loongarch64_is_loongarch64(input_file))
    {
        return false;
    }
    return helf_arch_segment_info_get(input_file,index,info);
}

bool helf_loongarch64_is_load_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_loongarch64_is_loongarch64(input_file))
    {
        return false;
    }
    return helf_arch_is_load_segment(index,input_file);
}

bool helf_loongarch64_is_rx_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_loongarch64_is_loongarch64(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_segment(index,input_file);
}

bool helf_loongarch64_is_ro_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_loongarch64_is_loongarch64(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_segment(index,input_file);
}

bool helf_loongarch64_is_rw_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_loongarch64_is_loongarch64(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_segment(index,input_file);
}

size_t helf_loongarch64_load_segment_count_get(helf_file_input_t *input_file)
{
    if(!helf_loongarch64_is_loongarch64(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_count_get(input_file);
}

bool helf_loongarch64_load_segment_info_get(helf_file_input_t *input_file,size_t index,helf_loongarch64_segment_info_t *info)
{
    if(!helf_loongarch64_is_loongarch64(input_file))
    {
        return false;
    }
    return helf_arch_load_segment_info_get(input_file,index,info);
}

size_t helf_loongarch64_load_segment_total_filesz_get(helf_file_input_t *input_file)
{
    if(!helf_loongarch64_is_loongarch64(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_filesz_get(input_file);
}

size_t helf_loongarch64_load_segment_total_memsz_get(helf_file_input_t *input_file)
{
    if(!helf_loongarch64_is_loongarch64(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_memsz_get(input_file);
}

size_t helf_loongarch64_relocation_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_loongarch64_is_loongarch64(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_section_count_get(input_file);
}

bool helf_loongarch64_relocation_section_info_get(helf_file_input_t *input_file,size_t index,helf_arch_relocation_section_info_t *info)
{
    if(!helf_loongarch64_is_loongarch64(input_file))
    {
        return false;
    }
    return helf_arch_relocation_section_info_get(input_file,index,info);
}

size_t helf_loongarch64_relocation_count_get(helf_file_input_t *input_file,size_t section_index)
{
    if(!helf_loongarch64_is_loongarch64(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_count_get(input_file,section_index);
}

bool helf_loongarch64_relocation_get(helf_file_input_t *input_file,size_t section_index,size_t index,helf_arch_relocation_info_t *info)
{
    if(!helf_loongarch64_is_loongarch64(input_file))
    {
        return false;
    }
    if(!helf_arch_relocation_get(input_file,section_index,index,info))
    {
        return false;
    }
    return true;
}

bool helf_loongarch64_relocation_symbol_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_symbol_info_t *info)
{
    if(!helf_loongarch64_is_loongarch64(input_file))
    {
        return false;
    }
    return helf_arch_relocation_symbol_info_get(input_file,relocation,info);
}

static const helf_arch_relocation_type_name_t helf_loongarch64_relocation_type_names[]=
{
    {0,0,"R_LARCH_NONE"},
    {1,4,"R_LARCH_32"},
    {2,8,"R_LARCH_64"},
    {3,8,"R_LARCH_RELATIVE"},
    {4,0,"R_LARCH_COPY"},
    {5,8,"R_LARCH_JUMP_SLOT"},
    {6,4,"R_LARCH_TLS_DTPMOD32"},
    {7,8,"R_LARCH_TLS_DTPMOD64"},
    {8,4,"R_LARCH_TLS_DTPREL32"},
    {9,8,"R_LARCH_TLS_DTPREL64"},
    {10,4,"R_LARCH_TLS_TPREL32"},
    {11,8,"R_LARCH_TLS_TPREL64"},
    {12,8,"R_LARCH_IRELATIVE"},
    {13,4,"R_LARCH_TLS_DESC32"},
    {14,8,"R_LARCH_TLS_DESC64"},
    {20,0,"R_LARCH_MARK_LA"},
    {21,0,"R_LARCH_MARK_PCREL"},
    {22,4,"R_LARCH_SOP_PUSH_PCREL"},
    {23,0,"R_LARCH_SOP_PUSH_ABSOLUTE"},
    {24,0,"R_LARCH_SOP_PUSH_DUP"},
    {25,0,"R_LARCH_SOP_PUSH_GPREL"},
    {26,0,"R_LARCH_SOP_PUSH_TLS_TPREL"},
    {27,0,"R_LARCH_SOP_PUSH_TLS_GOT"},
    {28,0,"R_LARCH_SOP_PUSH_TLS_GD"},
    {29,0,"R_LARCH_SOP_PUSH_PLT_PCREL"},
    {30,0,"R_LARCH_SOP_ASSERT"},
    {31,0,"R_LARCH_SOP_NOT"},
    {32,0,"R_LARCH_SOP_SUB"},
    {33,0,"R_LARCH_SOP_SL"},
    {34,0,"R_LARCH_SOP_SR"},
    {35,0,"R_LARCH_SOP_ADD"},
    {36,0,"R_LARCH_SOP_AND"},
    {37,0,"R_LARCH_SOP_IF_ELSE"},
    {38,4,"R_LARCH_SOP_POP_32_S_10_5"},
    {39,4,"R_LARCH_SOP_POP_32_U_10_12"},
    {40,4,"R_LARCH_SOP_POP_32_S_10_12"},
    {41,4,"R_LARCH_SOP_POP_32_S_10_16"},
    {42,4,"R_LARCH_SOP_POP_32_S_10_16_S2"},
    {43,4,"R_LARCH_SOP_POP_32_S_5_20"},
    {44,4,"R_LARCH_SOP_POP_32_S_0_5_10_16_S2"},
    {45,4,"R_LARCH_SOP_POP_32_S_0_10_10_16_S2"},
    {46,4,"R_LARCH_SOP_POP_32_U"},
    {47,1,"R_LARCH_ADD8"},
    {48,2,"R_LARCH_ADD16"},
    {49,3,"R_LARCH_ADD24"},
    {50,4,"R_LARCH_ADD32"},
    {51,8,"R_LARCH_ADD64"},
    {52,1,"R_LARCH_SUB8"},
    {53,2,"R_LARCH_SUB16"},
    {54,3,"R_LARCH_SUB24"},
    {55,4,"R_LARCH_SUB32"},
    {56,8,"R_LARCH_SUB64"},
    {57,0,"R_LARCH_GNU_VTINHERIT"},
    {58,0,"R_LARCH_GNU_VTENTRY"},
    {64,4,"R_LARCH_B16"},
    {65,4,"R_LARCH_B21"},
    {66,4,"R_LARCH_B26"},
    {67,4,"R_LARCH_ABS_HI20"},
    {68,4,"R_LARCH_ABS_LO12"},
    {69,4,"R_LARCH_ABS64_LO20"},
    {70,4,"R_LARCH_ABS64_HI12"},
    {71,4,"R_LARCH_PCALA_HI20"},
    {72,4,"R_LARCH_PCALA_LO12"},
    {73,4,"R_LARCH_PCALA64_LO20"},
    {74,4,"R_LARCH_PCALA64_HI12"},
    {75,4,"R_LARCH_GOT_PC_HI20"},
    {76,4,"R_LARCH_GOT_PC_LO12"},
    {77,4,"R_LARCH_GOT64_PC_LO20"},
    {78,4,"R_LARCH_GOT64_PC_HI12"},
    {79,4,"R_LARCH_GOT_HI20"},
    {80,4,"R_LARCH_GOT_LO12"},
    {81,4,"R_LARCH_GOT64_LO20"},
    {82,4,"R_LARCH_GOT64_HI12"},
    {83,4,"R_LARCH_TLS_LE_HI20"},
    {84,4,"R_LARCH_TLS_LE_LO12"},
    {85,4,"R_LARCH_TLS_LE64_LO20"},
    {86,4,"R_LARCH_TLS_LE64_HI12"},
    {87,4,"R_LARCH_TLS_IE_PC_HI20"},
    {88,4,"R_LARCH_TLS_IE_PC_LO12"},
    {89,4,"R_LARCH_TLS_IE64_PC_LO20"},
    {90,4,"R_LARCH_TLS_IE64_PC_HI12"},
    {91,4,"R_LARCH_TLS_IE_HI20"},
    {92,4,"R_LARCH_TLS_IE_LO12"},
    {93,4,"R_LARCH_TLS_IE64_LO20"},
    {94,4,"R_LARCH_TLS_IE64_HI12"},
    {95,4,"R_LARCH_TLS_LD_PC_HI20"},
    {96,4,"R_LARCH_TLS_LD_HI20"},
    {97,4,"R_LARCH_TLS_GD_PC_HI20"},
    {98,4,"R_LARCH_TLS_GD_HI20"},
    {99,4,"R_LARCH_32_PCREL"},
    {100,1,"R_LARCH_RELAX"},
    {101,0,"R_LARCH_DELETE"},
    {102,0,"R_LARCH_ALIGN"},
    {103,4,"R_LARCH_PCREL20_S2"},
    {104,0,"R_LARCH_CFA"},
    {105,1,"R_LARCH_ADD6"},
    {106,1,"R_LARCH_SUB6"},
    {107,1,"R_LARCH_ADD_ULEB128"},
    {108,1,"R_LARCH_SUB_ULEB128"},
    {109,8,"R_LARCH_64_PCREL"},
    {110,8,"R_LARCH_CALL36"},
    {111,4,"R_LARCH_TLS_DESC_PC_HI20"},
    {112,4,"R_LARCH_TLS_DESC_PC_LO12"},
    {113,4,"R_LARCH_TLS_DESC64_PC_LO20"},
    {114,4,"R_LARCH_TLS_DESC64_PC_HI12"},
    {115,4,"R_LARCH_TLS_DESC_HI20"},
    {116,4,"R_LARCH_TLS_DESC_LO12"},
    {117,4,"R_LARCH_TLS_DESC64_LO20"},
    {118,4,"R_LARCH_TLS_DESC64_HI12"},
    {119,4,"R_LARCH_TLS_DESC_LD"},
    {120,4,"R_LARCH_TLS_DESC_CALL"},
    {121,4,"R_LARCH_TLS_LE_HI20_R"},
    {122,0,"R_LARCH_TLS_LE_ADD_R"},
    {123,4,"R_LARCH_TLS_LE_LO12_R"},
    {124,4,"R_LARCH_TLS_LD_PCREL20_S2"},
    {125,4,"R_LARCH_TLS_GD_PCREL20_S2"},
    {126,4,"R_LARCH_TLS_DESC_PCREL20_S2"},
    {127,8,"R_LARCH_CALL30"},
    {128,4,"R_LARCH_PCADD_HI20"},
    {129,4,"R_LARCH_PCADD_LO12"},
    {130,4,"R_LARCH_GOT_PCADD_HI20"},
    {131,4,"R_LARCH_GOT_PCADD_LO12"},
    {132,4,"R_LARCH_TLS_IE_PCADD_HI20"},
    {133,4,"R_LARCH_TLS_IE_PCADD_LO12"},
    {134,4,"R_LARCH_TLS_LD_PCADD_HI20"},
    {135,4,"R_LARCH_TLS_LD_PCADD_LO12"},
    {136,4,"R_LARCH_TLS_GD_PCADD_HI20"},
    {137,4,"R_LARCH_TLS_GD_PCADD_LO12"},
    {138,4,"R_LARCH_TLS_DESC_PCADD_HI20"},
    {139,4,"R_LARCH_TLS_DESC_PCADD_LO12"},
};

const char *helf_loongarch64_relocation_type_name_get(uint32_t type)
{
    return helf_arch_relocation_type_name_get(helf_loongarch64_relocation_type_names,sizeof(helf_loongarch64_relocation_type_names)/sizeof(helf_loongarch64_relocation_type_names[0]),type);
}

uint8_t helf_loongarch64_relocation_size_get(uint32_t type)
{
    return helf_arch_relocation_type_size_get(helf_loongarch64_relocation_type_names,sizeof(helf_loongarch64_relocation_type_names)/sizeof(helf_loongarch64_relocation_type_names[0]),type);
}

const char *helf_loongarch64_section_type_name_get(uint32_t type)
{
    (void)type;
    return NULL;
}

bool helf_loongarch64_relocation_address_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_relocation_address_info_t *info)
{
    if(!helf_loongarch64_is_loongarch64(input_file) || relocation==NULL)
    {
        return false;
    }
    uint8_t size=helf_loongarch64_relocation_size_get(relocation->type);
    if(size==0)
    {
        return false;
    }
    return helf_arch_relocation_address_info_get(input_file,relocation,size,info);
}

size_t helf_loongarch64_relocation_data_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,void *buffer,size_t buffer_length)
{
    if(!helf_loongarch64_is_loongarch64(input_file) || relocation==NULL)
    {
        return 0;
    }
    uint8_t size=helf_loongarch64_relocation_size_get(relocation->type);
    if(size==0)
    {
        return 0;
    }
    return helf_arch_relocation_data_get(input_file,relocation,size,buffer,buffer_length);
}

bool helf_loongarch64_is_shared_object(helf_file_input_t *input_file)
{
    return helf_arch_is_shared_object(input_file,helf_loongarch64_machines,sizeof(helf_loongarch64_machines)/sizeof(helf_loongarch64_machines[0]),HELF_ELFCLASS_64);
}

bool helf_loongarch64_is_relocatable(helf_file_input_t *input_file)
{
    return helf_arch_is_relocatable(input_file,helf_loongarch64_machines,sizeof(helf_loongarch64_machines)/sizeof(helf_loongarch64_machines[0]),HELF_ELFCLASS_64);
}

size_t helf_loongarch64_dynamic_count_get(helf_file_input_t *input_file)
{
    if(!helf_loongarch64_is_loongarch64(input_file))
    {
        return 0;
    }
    return helf_arch_dynamic_count_get(input_file);
}

bool helf_loongarch64_dynamic_info_get(helf_file_input_t *input_file,size_t index,helf_arch_dynamic_info_t *info)
{
    if(!helf_loongarch64_is_loongarch64(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_info_get(input_file,index,info);
}

bool helf_loongarch64_dynamic_string_get(helf_file_input_t *input_file,uint64_t offset,char *buffer,size_t buffer_length)
{
    if(!helf_loongarch64_is_loongarch64(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_string_get(input_file,offset,buffer,buffer_length);
}

/* helf_relocation_howto_begin */
static const helf_arch_relocation_howto_t helf_loongarch64_reloc_howto[]=
{
    {0x00000000u,0,0,0,0,0x0040,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000001u,4,32,0,0,0x0000,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000002u,8,64,0,0,0x0000,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000003u,8,32,0,0,0x0080,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000004u,0,0,0,0,0x0180,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000005u,8,64,0,0,0x0140,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000006u,4,32,0,0,0x01C0,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000007u,8,64,0,0,0x01C0,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000008u,4,32,0,0,0x01C0,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000009u,8,64,0,0,0x01C0,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x0000000Au,4,32,0,0,0x01C0,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x0000000Bu,8,64,0,0,0x01C0,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x0000000Cu,8,64,0,0,0x0080,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x0000000Du,4,32,0,0,0x01C0,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x0000000Eu,8,64,0,0,0x01C0,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000014u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000015u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000016u,4,32,2,0,0x0001,0x0000000003FFFFFFULL,0x0000000003FFFFFFULL},
    {0x00000017u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000018u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000019u,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Au,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Bu,0,0,0,0,0x03C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Cu,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Du,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Eu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Fu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000020u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000021u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000022u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000023u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000024u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000025u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000026u,4,5,0,10,0x0000,0x0000000000000000ULL,0x0000000000007C00ULL},
    {0x00000027u,4,12,0,10,0x0000,0x0000000000000000ULL,0x00000000003FFC00ULL},
    {0x00000028u,4,12,0,10,0x0000,0x0000000000000000ULL,0x00000000003FFC00ULL},
    {0x00000029u,4,16,0,10,0x0000,0x0000000000000000ULL,0x0000000003FFFC00ULL},
    {0x0000002Au,4,16,2,10,0x0000,0x0000000000000000ULL,0x0000000003FFFC00ULL},
    {0x0000002Bu,4,20,0,5,0x0000,0x0000000000000000ULL,0x0000000001FFFFE0ULL},
    {0x0000002Cu,4,21,2,0,0x0000,0x0000000000000000ULL,0x0000000003FFFC1FULL},
    {0x0000002Du,4,26,2,0,0x0000,0x0000000000000000ULL,0x0000000003FFFFFFULL},
    {0x0000002Eu,4,32,0,0,0x0000,0xFFFFFFFF00000000ULL,0x00000000FFFFFFFFULL},
    {0x0000002Fu,1,8,0,0,0x0010,0x0000000000000000ULL,0x00000000000000FFULL},
    {0x00000030u,2,16,0,0,0x0010,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000031u,3,24,0,0,0x0010,0x0000000000000000ULL,0x0000000000FFFFFFULL},
    {0x00000032u,4,32,0,0,0x0010,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000033u,8,64,0,0,0x0010,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000034u,1,8,0,0,0x0010,0x0000000000000000ULL,0x00000000000000FFULL},
    {0x00000035u,2,16,0,0,0x0010,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000036u,3,24,0,0,0x0010,0x0000000000000000ULL,0x0000000000FFFFFFULL},
    {0x00000037u,4,32,0,0,0x0010,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000038u,8,64,0,0,0x0010,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000039u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000003Au,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000040u,4,16,2,10,0x0000,0x0000000000000000ULL,0x0000000003FFFC00ULL},
    {0x00000041u,4,21,2,0,0x0000,0x0000000000000000ULL,0x0000000003FFFC1FULL},
    {0x00000042u,4,26,2,0,0x0000,0x0000000000000000ULL,0x0000000003FFFFFFULL},
    {0x00000043u,4,20,12,5,0x0000,0x0000000000000000ULL,0x0000000001FFFFE0ULL},
    {0x00000044u,4,12,0,10,0x0000,0x0000000000000000ULL,0x00000000003FFC00ULL},
    {0x00000045u,4,20,32,5,0x0000,0x0000000000000000ULL,0x0000000001FFFFE0ULL},
    {0x00000046u,4,12,52,10,0x0000,0x0000000000000000ULL,0x00000000003FFC00ULL},
    {0x00000047u,4,20,12,5,0x0000,0x0000000000000000ULL,0x0000000001FFFFE0ULL},
    {0x00000048u,4,12,0,10,0x0000,0x0000000000000000ULL,0x00000000003FFC00ULL},
    {0x00000049u,4,20,32,5,0x0000,0x0000000000000000ULL,0x0000000001FFFFE0ULL},
    {0x0000004Au,4,12,52,10,0x0000,0x0000000000000000ULL,0x00000000003FFC00ULL},
    {0x0000004Bu,4,20,12,5,0x0200,0x0000000000000000ULL,0x0000000001FFFFE0ULL},
    {0x0000004Cu,4,12,0,10,0x0200,0x0000000000000000ULL,0x00000000003FFC00ULL},
    {0x0000004Du,4,20,32,5,0x0200,0x0000000000000000ULL,0x0000000001FFFFE0ULL},
    {0x0000004Eu,4,12,52,10,0x0200,0x0000000000000000ULL,0x00000000003FFC00ULL},
    {0x0000004Fu,4,20,12,5,0x0200,0x0000000000000000ULL,0x0000000001FFFFE0ULL},
    {0x00000050u,4,12,0,10,0x0200,0x0000000000000000ULL,0x00000000003FFC00ULL},
    {0x00000051u,4,20,32,5,0x0200,0x0000000000000000ULL,0x0000000001FFFFE0ULL},
    {0x00000052u,4,12,52,10,0x0200,0x0000000000000000ULL,0x00000000003FFC00ULL},
    {0x00000053u,4,20,12,5,0x01C0,0x0000000000000000ULL,0x0000000001FFFFE0ULL},
    {0x00000054u,4,12,0,10,0x01C0,0x0000000000000000ULL,0x00000000003FFC00ULL},
    {0x00000055u,4,20,32,5,0x01C0,0x0000000000000000ULL,0x0000000001FFFFE0ULL},
    {0x00000056u,4,12,52,10,0x01C0,0x0000000000000000ULL,0x00000000003FFC00ULL},
    {0x00000057u,4,20,12,5,0x01C0,0x0000000000000000ULL,0x0000000001FFFFE0ULL},
    {0x00000058u,4,12,0,10,0x01C0,0x0000000000000000ULL,0x00000000003FFC00ULL},
    {0x00000059u,4,20,32,5,0x01C0,0x0000000000000000ULL,0x0000000001FFFFE0ULL},
    {0x0000005Au,4,12,52,10,0x01C0,0x0000000000000000ULL,0x00000000003FFC00ULL},
    {0x0000005Bu,4,20,12,5,0x01C0,0x0000000000000000ULL,0x0000000001FFFFE0ULL},
    {0x0000005Cu,4,12,0,10,0x01C0,0x0000000000000000ULL,0x00000000003FFC00ULL},
    {0x0000005Du,4,20,32,5,0x01C0,0x0000000000000000ULL,0x0000000001FFFFE0ULL},
    {0x0000005Eu,4,12,52,10,0x01C0,0x0000000000000000ULL,0x00000000003FFC00ULL},
    {0x0000005Fu,4,20,12,5,0x01C0,0x0000000000000000ULL,0x0000000001FFFFE0ULL},
    {0x00000060u,4,20,12,5,0x01C0,0x0000000000000000ULL,0x0000000001FFFFE0ULL},
    {0x00000061u,4,20,12,5,0x01C0,0x0000000000000000ULL,0x0000000001FFFFE0ULL},
    {0x00000062u,4,20,12,5,0x01C0,0x0000000000000000ULL,0x0000000001FFFFE0ULL},
    {0x00000063u,4,32,0,0,0x0003,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000064u,1,0,0,0,0x0040,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000065u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000066u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000067u,4,20,2,5,0x0000,0x0000000000000000ULL,0x0000000001FFFFE0ULL},
    {0x00000068u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000069u,1,8,0,0,0x0010,0x0000000000000000ULL,0x000000000000003FULL},
    {0x0000006Au,1,8,0,0,0x0010,0x0000000000000000ULL,0x000000000000003FULL},
    {0x0000006Bu,1,0,0,0,0x0010,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000006Cu,1,0,0,0,0x0010,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000006Du,8,64,0,0,0x0003,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x0000006Eu,8,36,2,0,0x0001,0x0000000000000000ULL,0x03FFFC0001FFFFE0ULL},
    {0x0000006Fu,4,20,12,5,0x01C1,0x0000000000000000ULL,0x0000000001FFFFE0ULL},
    {0x00000070u,4,12,0,10,0x01C1,0x0000000000000000ULL,0x00000000003FFC00ULL},
    {0x00000071u,4,20,32,5,0x01C1,0x0000000000000000ULL,0x0000000001FFFFE0ULL},
    {0x00000072u,4,12,52,10,0x01C1,0x0000000000000000ULL,0x00000000003FFC00ULL},
    {0x00000073u,4,20,12,5,0x01C0,0x0000000000000000ULL,0x0000000001FFFFE0ULL},
    {0x00000074u,4,12,0,10,0x01C0,0x0000000000000000ULL,0x00000000003FFC00ULL},
    {0x00000075u,4,20,32,5,0x01C0,0x0000000000000000ULL,0x0000000001FFFFE0ULL},
    {0x00000076u,4,12,52,10,0x01C0,0x0000000000000000ULL,0x00000000003FFC00ULL},
    {0x00000077u,4,0,0,0,0x01C1,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000078u,4,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000079u,4,20,12,5,0x01C0,0x0000000000000000ULL,0x0000000001FFFFE0ULL},
    {0x0000007Au,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000007Bu,4,12,0,10,0x01C0,0x0000000000000000ULL,0x00000000003FFC00ULL},
    {0x0000007Cu,4,20,2,5,0x01C2,0x0000000000000000ULL,0x0000000001FFFFE0ULL},
    {0x0000007Du,4,20,2,5,0x01C2,0x0000000000000000ULL,0x0000000001FFFFE0ULL},
    {0x0000007Eu,4,20,2,5,0x01C2,0x0000000000000000ULL,0x0000000001FFFFE0ULL},
    {0x0000007Fu,8,30,2,0,0x0001,0x0000000000000000ULL,0x000FFC0001FFFFE0ULL},
    {0x00000080u,4,20,12,5,0x0003,0x0000000000000000ULL,0x0000000001FFFFE0ULL},
    {0x00000081u,4,12,0,10,0x0000,0x0000000000000000ULL,0x00000000003FFC00ULL},
    {0x00000082u,4,20,12,5,0x0203,0x0000000000000000ULL,0x0000000001FFFFE0ULL},
    {0x00000083u,4,12,0,10,0x0200,0x0000000000000000ULL,0x00000000003FFC00ULL},
    {0x00000084u,4,20,12,5,0x01C3,0x0000000000000000ULL,0x0000000001FFFFE0ULL},
    {0x00000085u,4,12,0,10,0x01C0,0x0000000000000000ULL,0x00000000003FFC00ULL},
    {0x00000086u,4,20,12,5,0x01C3,0x0000000000000000ULL,0x0000000001FFFFE0ULL},
    {0x00000087u,4,12,0,10,0x01C0,0x0000000000000000ULL,0x00000000003FFC00ULL},
    {0x00000088u,4,20,12,5,0x01C3,0x0000000000000000ULL,0x0000000001FFFFE0ULL},
    {0x00000089u,4,12,0,10,0x01C0,0x0000000000000000ULL,0x00000000003FFC00ULL},
    {0x0000008Au,4,20,12,5,0x01C3,0x0000000000000000ULL,0x0000000001FFFFE0ULL},
    {0x0000008Bu,4,12,0,10,0x01C0,0x0000000000000000ULL,0x00000000003FFC00ULL},
};

bool helf_loongarch64_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto)
{
    if(howto==NULL)
    {
        return false;
    }
    size_t count=sizeof(helf_loongarch64_reloc_howto)/sizeof(helf_loongarch64_reloc_howto[0]);
    if(count==0)
    {
        return false;
    }
    for(size_t i=0;i<count;i++)
    {
        if(helf_loongarch64_reloc_howto[i].type==type)
        {
            *howto=helf_loongarch64_reloc_howto[i];
            return true;
        }
    }
    return false;
}
/* helf_relocation_howto_end */


bool helf_loongarch64_relocation_apply(uint32_t type,uint8_t *target,uintptr_t place,uintptr_t symbol_address,int64_t addend,const helf_arch_relocation_context_t *context)
{
    bool big_endian=(context!=NULL)?context->big_endian:false;
    uint64_t value=(uint64_t)(symbol_address+(uintptr_t)addend);
    switch(type)
    {
    case 71:
        {
            uint64_t lo=value&0xFFF;
            int64_t relocation=(int64_t)(value&~0xFFFULL)-(int64_t)(place&~0xFFFULL);
            if(lo>0x7FF)
            {
                relocation+=0x1000;
            }
            relocation>>=12;
            uint32_t insn=helf_arch_read32(target,big_endian);
            insn=(insn&~0x1FFFFE0U)|(((uint32_t)relocation&0xFFFFF)<<5);
            helf_arch_write32(target,insn,big_endian);
        }
        return true;
    case 72:
        {
            uint32_t insn=helf_arch_read32(target,big_endian);
            insn=(insn&~0x3FFC00U)|(((uint32_t)value&0xFFF)<<10);
            helf_arch_write32(target,insn,big_endian);
        }
        return true;
    case 66:
        {
            int64_t relocation=(int64_t)value-(int64_t)place;
            uint32_t field=((uint32_t)(relocation>>2))&0x03FFFFFFU;
            uint32_t insn=helf_arch_read32(target,big_endian);
            insn=(insn&~0x03FFFFFFU)|((field&0xFFFFU)<<10)|((field>>16)&0x3FFU);
            helf_arch_write32(target,insn,big_endian);
        }
        return true;
    default:
        return false;
    }
}
