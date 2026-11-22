/***************************************************************
 * Name:      helf_arch_tilegx.c
 * Purpose:   实现helf_arch_tilegx接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "helf_arch_tilegx.h"

static const uint16_t helf_tilegx_machines[]=
{
    HELF_EM_TILEGX,
};

bool helf_tilegx_is_tilegx(helf_file_input_t *input_file)
{
    return helf_arch_is(input_file,helf_tilegx_machines,sizeof(helf_tilegx_machines)/sizeof(helf_tilegx_machines[0]),HELF_ELFCLASS_64);
}

bool helf_tilegx_is_big_endian(helf_file_input_t *input_file)
{
    if(!helf_tilegx_is_tilegx(input_file))
    {
        return false;
    }
    return helf_file_input_is_big_endian(input_file);
}

bool helf_tilegx_is_executable(helf_file_input_t *input_file)
{
    return helf_arch_is_executable(input_file,helf_tilegx_machines,sizeof(helf_tilegx_machines)/sizeof(helf_tilegx_machines[0]),HELF_ELFCLASS_64);
}

bool helf_tilegx_entry_get(helf_file_input_t *input_file,uint64_t *entry)
{
    if(entry==NULL)
    {
        return false;
    }
    if(!helf_tilegx_is_tilegx(input_file))
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

size_t helf_tilegx_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_tilegx_is_tilegx(input_file))
    {
        return 0;
    }
    return helf_arch_section_count_get(input_file);
}

bool helf_tilegx_section_info_get(helf_file_input_t *input_file,size_t index,helf_tilegx_section_info_t *info)
{
    if(!helf_tilegx_is_tilegx(input_file))
    {
        return false;
    }
    return helf_arch_section_info_get(input_file,index,info);
}

bool helf_tilegx_is_rx_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tilegx_is_tilegx(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_section(index,input_file);
}

intptr_t helf_tilegx_rx_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tilegx_is_rx_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rx_section_offset_get(index,input_file);
}

size_t helf_tilegx_rx_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_tilegx_is_tilegx(input_file))
    {
        return 0;
    }
    return helf_arch_rx_section_size_get(input_file);
}

bool helf_tilegx_is_ro_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tilegx_is_tilegx(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_section(index,input_file);
}

intptr_t helf_tilegx_ro_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tilegx_is_ro_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_ro_section_offset_get(index,input_file);
}

size_t helf_tilegx_ro_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_tilegx_is_tilegx(input_file))
    {
        return 0;
    }
    return helf_arch_ro_section_size_get(input_file);
}

bool helf_tilegx_is_rw_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tilegx_is_tilegx(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_section(index,input_file);
}

intptr_t helf_tilegx_rw_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tilegx_is_rw_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rw_section_offset_get(index,input_file);
}

size_t helf_tilegx_rw_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_tilegx_is_tilegx(input_file))
    {
        return 0;
    }
    return helf_arch_rw_section_size_get(input_file);
}

bool helf_tilegx_is_bss_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tilegx_is_tilegx(input_file))
    {
        return false;
    }
    return helf_arch_is_bss_section(index,input_file);
}

size_t helf_tilegx_program_header_count_get(helf_file_input_t *input_file)
{
    if(!helf_tilegx_is_tilegx(input_file))
    {
        return 0;
    }
    return helf_arch_program_header_count_get(input_file);
}

bool helf_tilegx_program_header_get(helf_file_input_t *input_file,size_t index,helf_elf64_program_header_t *programheader)
{
    if(programheader==NULL)
    {
        return false;
    }
    if(!helf_tilegx_is_tilegx(input_file))
    {
        return false;
    }
    if(index>=helf_arch_program_header_count_get(input_file))
    {
        return false;
    }
    return helf_file_input_64_bits_program_header_get(input_file,index,programheader);
}

size_t helf_tilegx_segment_count_get(helf_file_input_t *input_file)
{
    return helf_tilegx_program_header_count_get(input_file);
}

bool helf_tilegx_segment_info_get(helf_file_input_t *input_file,size_t index,helf_tilegx_segment_info_t *info)
{
    if(!helf_tilegx_is_tilegx(input_file))
    {
        return false;
    }
    return helf_arch_segment_info_get(input_file,index,info);
}

bool helf_tilegx_is_load_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tilegx_is_tilegx(input_file))
    {
        return false;
    }
    return helf_arch_is_load_segment(index,input_file);
}

bool helf_tilegx_is_rx_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tilegx_is_tilegx(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_segment(index,input_file);
}

bool helf_tilegx_is_ro_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tilegx_is_tilegx(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_segment(index,input_file);
}

bool helf_tilegx_is_rw_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tilegx_is_tilegx(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_segment(index,input_file);
}

size_t helf_tilegx_load_segment_count_get(helf_file_input_t *input_file)
{
    if(!helf_tilegx_is_tilegx(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_count_get(input_file);
}

bool helf_tilegx_load_segment_info_get(helf_file_input_t *input_file,size_t index,helf_tilegx_segment_info_t *info)
{
    if(!helf_tilegx_is_tilegx(input_file))
    {
        return false;
    }
    return helf_arch_load_segment_info_get(input_file,index,info);
}

size_t helf_tilegx_load_segment_total_filesz_get(helf_file_input_t *input_file)
{
    if(!helf_tilegx_is_tilegx(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_filesz_get(input_file);
}

size_t helf_tilegx_load_segment_total_memsz_get(helf_file_input_t *input_file)
{
    if(!helf_tilegx_is_tilegx(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_memsz_get(input_file);
}

size_t helf_tilegx_relocation_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_tilegx_is_tilegx(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_section_count_get(input_file);
}

bool helf_tilegx_relocation_section_info_get(helf_file_input_t *input_file,size_t index,helf_arch_relocation_section_info_t *info)
{
    if(!helf_tilegx_is_tilegx(input_file))
    {
        return false;
    }
    return helf_arch_relocation_section_info_get(input_file,index,info);
}

size_t helf_tilegx_relocation_count_get(helf_file_input_t *input_file,size_t section_index)
{
    if(!helf_tilegx_is_tilegx(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_count_get(input_file,section_index);
}

bool helf_tilegx_relocation_get(helf_file_input_t *input_file,size_t section_index,size_t index,helf_arch_relocation_info_t *info)
{
    if(!helf_tilegx_is_tilegx(input_file))
    {
        return false;
    }
    if(!helf_arch_relocation_get(input_file,section_index,index,info))
    {
        return false;
    }
    return true;
}

bool helf_tilegx_relocation_symbol_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_symbol_info_t *info)
{
    if(!helf_tilegx_is_tilegx(input_file))
    {
        return false;
    }
    return helf_arch_relocation_symbol_info_get(input_file,relocation,info);
}

static const helf_arch_relocation_type_name_t helf_tilegx_relocation_type_names[]=
{
    {0,0,"R_TILEGX_NONE"},
    {1,8,"R_TILEGX_64"},
    {2,4,"R_TILEGX_32"},
    {3,2,"R_TILEGX_16"},
    {4,1,"R_TILEGX_8"},
    {5,8,"R_TILEGX_64_PCREL"},
    {6,4,"R_TILEGX_32_PCREL"},
    {7,2,"R_TILEGX_16_PCREL"},
    {8,1,"R_TILEGX_8_PCREL"},
    {9,2,"R_TILEGX_HW0"},
    {10,2,"R_TILEGX_HW1"},
    {11,2,"R_TILEGX_HW2"},
    {12,2,"R_TILEGX_HW3"},
    {13,2,"R_TILEGX_HW0_LAST"},
    {14,2,"R_TILEGX_HW1_LAST"},
    {15,2,"R_TILEGX_HW2_LAST"},
    {16,0,"R_TILEGX_COPY"},
    {17,8,"R_TILEGX_GLOB_DAT"},
    {18,8,"R_TILEGX_JMP_SLOT"},
    {19,8,"R_TILEGX_RELATIVE"},
    {20,4,"R_TILEGX_BROFF_X1"},
    {21,4,"R_TILEGX_JUMPOFF_X1"},
    {22,4,"R_TILEGX_JUMPOFF_X1_PLT"},
    {23,0,"R_TILEGX_IMM8_X0"},
    {24,0,"R_TILEGX_IMM8_Y0"},
    {25,0,"R_TILEGX_IMM8_X1"},
    {26,0,"R_TILEGX_IMM8_Y1"},
    {27,0,"R_TILEGX_DEST_IMM8_X1"},
    {28,0,"R_TILEGX_MT_IMM14_X1"},
    {29,0,"R_TILEGX_MF_IMM14_X1"},
    {30,0,"R_TILEGX_MMSTART_X0"},
    {31,0,"R_TILEGX_MMEND_X0"},
    {32,0,"R_TILEGX_SHAMT_X0"},
    {33,0,"R_TILEGX_SHAMT_X1"},
    {34,0,"R_TILEGX_SHAMT_Y0"},
    {35,0,"R_TILEGX_SHAMT_Y1"},
    {36,0,"R_TILEGX_IMM16_X0_HW0"},
    {37,0,"R_TILEGX_IMM16_X1_HW0"},
    {38,0,"R_TILEGX_IMM16_X0_HW1"},
    {39,0,"R_TILEGX_IMM16_X1_HW1"},
    {40,0,"R_TILEGX_IMM16_X0_HW2"},
    {41,0,"R_TILEGX_IMM16_X1_HW2"},
    {42,0,"R_TILEGX_IMM16_X0_HW3"},
    {43,0,"R_TILEGX_IMM16_X1_HW3"},
    {44,0,"R_TILEGX_IMM16_X0_HW0_LAST"},
    {45,0,"R_TILEGX_IMM16_X1_HW0_LAST"},
    {46,0,"R_TILEGX_IMM16_X0_HW1_LAST"},
    {47,0,"R_TILEGX_IMM16_X1_HW1_LAST"},
    {48,0,"R_TILEGX_IMM16_X0_HW2_LAST"},
    {49,0,"R_TILEGX_IMM16_X1_HW2_LAST"},
    {50,0,"R_TILEGX_IMM16_X0_HW0_PCREL"},
    {51,0,"R_TILEGX_IMM16_X1_HW0_PCREL"},
    {52,0,"R_TILEGX_IMM16_X0_HW1_PCREL"},
    {53,0,"R_TILEGX_IMM16_X1_HW1_PCREL"},
    {54,0,"R_TILEGX_IMM16_X0_HW2_PCREL"},
    {55,0,"R_TILEGX_IMM16_X1_HW2_PCREL"},
    {56,0,"R_TILEGX_IMM16_X0_HW3_PCREL"},
    {57,0,"R_TILEGX_IMM16_X1_HW3_PCREL"},
    {58,0,"R_TILEGX_IMM16_X0_HW0_LAST_PCREL"},
    {59,0,"R_TILEGX_IMM16_X1_HW0_LAST_PCREL"},
    {60,0,"R_TILEGX_IMM16_X0_HW1_LAST_PCREL"},
    {61,0,"R_TILEGX_IMM16_X1_HW1_LAST_PCREL"},
    {62,0,"R_TILEGX_IMM16_X0_HW2_LAST_PCREL"},
    {63,0,"R_TILEGX_IMM16_X1_HW2_LAST_PCREL"},
    {64,0,"R_TILEGX_IMM16_X0_HW0_GOT"},
    {65,0,"R_TILEGX_IMM16_X1_HW0_GOT"},
    {66,0,"R_TILEGX_IMM16_X0_HW0_PLT_PCREL"},
    {67,0,"R_TILEGX_IMM16_X1_HW0_PLT_PCREL"},
    {68,0,"R_TILEGX_IMM16_X0_HW1_PLT_PCREL"},
    {69,0,"R_TILEGX_IMM16_X1_HW1_PLT_PCREL"},
    {70,0,"R_TILEGX_IMM16_X0_HW2_PLT_PCREL"},
    {71,0,"R_TILEGX_IMM16_X1_HW2_PLT_PCREL"},
    {72,0,"R_TILEGX_IMM16_X0_HW0_LAST_GOT"},
    {73,0,"R_TILEGX_IMM16_X1_HW0_LAST_GOT"},
    {74,0,"R_TILEGX_IMM16_X0_HW1_LAST_GOT"},
    {75,0,"R_TILEGX_IMM16_X1_HW1_LAST_GOT"},
    {76,0,"R_TILEGX_IMM16_X0_HW3_PLT_PCREL"},
    {77,0,"R_TILEGX_IMM16_X1_HW3_PLT_PCREL"},
    {78,0,"R_TILEGX_IMM16_X0_HW0_TLS_GD"},
    {79,0,"R_TILEGX_IMM16_X1_HW0_TLS_GD"},
    {80,0,"R_TILEGX_IMM16_X0_HW0_TLS_LE"},
    {81,0,"R_TILEGX_IMM16_X1_HW0_TLS_LE"},
    {82,0,"R_TILEGX_IMM16_X0_HW0_LAST_TLS_LE"},
    {83,0,"R_TILEGX_IMM16_X1_HW0_LAST_TLS_LE"},
    {84,0,"R_TILEGX_IMM16_X0_HW1_LAST_TLS_LE"},
    {85,0,"R_TILEGX_IMM16_X1_HW1_LAST_TLS_LE"},
    {86,0,"R_TILEGX_IMM16_X0_HW0_LAST_TLS_GD"},
    {87,0,"R_TILEGX_IMM16_X1_HW0_LAST_TLS_GD"},
    {88,0,"R_TILEGX_IMM16_X0_HW1_LAST_TLS_GD"},
    {89,0,"R_TILEGX_IMM16_X1_HW1_LAST_TLS_GD"},
    {92,0,"R_TILEGX_IMM16_X0_HW0_TLS_IE"},
    {93,0,"R_TILEGX_IMM16_X1_HW0_TLS_IE"},
    {94,0,"R_TILEGX_IMM16_X0_HW0_LAST_PLT_PCREL"},
    {95,0,"R_TILEGX_IMM16_X1_HW0_LAST_PLT_PCREL"},
    {96,0,"R_TILEGX_IMM16_X0_HW1_LAST_PLT_PCREL"},
    {97,0,"R_TILEGX_IMM16_X1_HW1_LAST_PLT_PCREL"},
    {98,0,"R_TILEGX_IMM16_X0_HW2_LAST_PLT_PCREL"},
    {99,0,"R_TILEGX_IMM16_X1_HW2_LAST_PLT_PCREL"},
    {100,0,"R_TILEGX_IMM16_X0_HW0_LAST_TLS_IE"},
    {101,0,"R_TILEGX_IMM16_X1_HW0_LAST_TLS_IE"},
    {102,0,"R_TILEGX_IMM16_X0_HW1_LAST_TLS_IE"},
    {103,0,"R_TILEGX_IMM16_X1_HW1_LAST_TLS_IE"},
    {106,0,"R_TILEGX_TLS_DTPMOD64"},
    {107,8,"R_TILEGX_TLS_DTPOFF64"},
    {108,0,"R_TILEGX_TLS_TPOFF64"},
    {109,0,"R_TILEGX_TLS_DTPMOD32"},
    {110,8,"R_TILEGX_TLS_DTPOFF32"},
    {111,0,"R_TILEGX_TLS_TPOFF32"},
    {112,4,"R_TILEGX_TLS_GD_CALL"},
    {113,0,"R_TILEGX_IMM8_X0_TLS_GD_ADD"},
    {114,0,"R_TILEGX_IMM8_X1_TLS_GD_ADD"},
    {115,0,"R_TILEGX_IMM8_Y0_TLS_GD_ADD"},
    {116,0,"R_TILEGX_IMM8_Y1_TLS_GD_ADD"},
    {117,0,"R_TILEGX_TLS_IE_LOAD"},
    {118,0,"R_TILEGX_IMM8_X0_TLS_ADD"},
    {119,0,"R_TILEGX_IMM8_X1_TLS_ADD"},
    {120,0,"R_TILEGX_IMM8_Y0_TLS_ADD"},
    {121,0,"R_TILEGX_IMM8_Y1_TLS_ADD"},
    {128,8,"R_TILEGX_GNU_VTINHERIT"},
    {129,8,"R_TILEGX_GNU_VTENTRY"},
};

const char *helf_tilegx_relocation_type_name_get(uint32_t type)
{
    return helf_arch_relocation_type_name_get(helf_tilegx_relocation_type_names,sizeof(helf_tilegx_relocation_type_names)/sizeof(helf_tilegx_relocation_type_names[0]),type);
}

uint8_t helf_tilegx_relocation_size_get(uint32_t type)
{
    return helf_arch_relocation_type_size_get(helf_tilegx_relocation_type_names,sizeof(helf_tilegx_relocation_type_names)/sizeof(helf_tilegx_relocation_type_names[0]),type);
}

const char *helf_tilegx_section_type_name_get(uint32_t type)
{
    (void)type;
    return NULL;
}

bool helf_tilegx_relocation_address_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_relocation_address_info_t *info)
{
    if(!helf_tilegx_is_tilegx(input_file) || relocation==NULL)
    {
        return false;
    }
    uint8_t size=helf_tilegx_relocation_size_get(relocation->type);
    if(size==0)
    {
        return false;
    }
    return helf_arch_relocation_address_info_get(input_file,relocation,size,info);
}

size_t helf_tilegx_relocation_data_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,void *buffer,size_t buffer_length)
{
    if(!helf_tilegx_is_tilegx(input_file) || relocation==NULL)
    {
        return 0;
    }
    uint8_t size=helf_tilegx_relocation_size_get(relocation->type);
    if(size==0)
    {
        return 0;
    }
    return helf_arch_relocation_data_get(input_file,relocation,size,buffer,buffer_length);
}

bool helf_tilegx_is_shared_object(helf_file_input_t *input_file)
{
    return helf_arch_is_shared_object(input_file,helf_tilegx_machines,sizeof(helf_tilegx_machines)/sizeof(helf_tilegx_machines[0]),HELF_ELFCLASS_64);
}

bool helf_tilegx_is_relocatable(helf_file_input_t *input_file)
{
    return helf_arch_is_relocatable(input_file,helf_tilegx_machines,sizeof(helf_tilegx_machines)/sizeof(helf_tilegx_machines[0]),HELF_ELFCLASS_64);
}

size_t helf_tilegx_dynamic_count_get(helf_file_input_t *input_file)
{
    if(!helf_tilegx_is_tilegx(input_file))
    {
        return 0;
    }
    return helf_arch_dynamic_count_get(input_file);
}

bool helf_tilegx_dynamic_info_get(helf_file_input_t *input_file,size_t index,helf_arch_dynamic_info_t *info)
{
    if(!helf_tilegx_is_tilegx(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_info_get(input_file,index,info);
}

bool helf_tilegx_dynamic_string_get(helf_file_input_t *input_file,uint64_t offset,char *buffer,size_t buffer_length)
{
    if(!helf_tilegx_is_tilegx(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_string_get(input_file,offset,buffer,buffer_length);
}

/* helf_relocation_howto_begin */
static const helf_arch_relocation_howto_t helf_tilegx_reloc_howto[]=
{
    {0x00000000u,0,0,0,0,0x0040,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000001u,8,64,0,0,0x0000,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000002u,4,32,0,0,0x0000,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000003u,2,16,0,0,0x0000,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000004u,1,8,0,0,0x0000,0x0000000000000000ULL,0x00000000000000FFULL},
    {0x00000005u,8,64,0,0,0x0003,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000006u,4,32,0,0,0x0003,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000007u,2,16,0,0,0x0003,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000008u,1,8,0,0,0x0003,0x0000000000000000ULL,0x00000000000000FFULL},
    {0x00000009u,2,16,0,0,0x0000,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000000Au,2,16,16,0,0x0000,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000000Bu,2,16,32,0,0x0000,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000000Cu,2,16,48,0,0x0000,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000000Du,2,16,0,0,0x0000,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000000Eu,2,16,16,0,0x0000,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000000Fu,2,16,32,0,0x0000,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000010u,0,0,0,0,0x0182,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000011u,8,0,0,0,0x0102,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000012u,8,0,0,0,0x0142,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000013u,8,0,0,0,0x0082,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000014u,4,17,0,0,0x0003,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000015u,4,27,0,0,0x0003,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000016u,4,27,0,0,0x0003,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000017u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000018u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000019u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
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
    {0x00000025u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000026u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000027u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000028u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000029u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000002Au,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000002Bu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000002Cu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000002Du,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000002Eu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000002Fu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000030u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000031u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000032u,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000033u,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000034u,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000035u,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000036u,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000037u,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000038u,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000039u,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000003Au,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000003Bu,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000003Cu,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000003Du,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000003Eu,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000003Fu,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000040u,0,0,0,0,0x0200,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000041u,0,0,0,0,0x0200,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000042u,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000043u,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000044u,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000045u,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000046u,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000047u,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000048u,0,0,0,0,0x0200,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000049u,0,0,0,0,0x0200,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000004Au,0,0,0,0,0x0200,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000004Bu,0,0,0,0,0x0200,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000004Cu,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000004Du,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000004Eu,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000004Fu,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000050u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000051u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000052u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000053u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000054u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000055u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000056u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000057u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000058u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000059u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000005Cu,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000005Du,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000005Eu,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000005Fu,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000060u,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000061u,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000062u,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000063u,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000064u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000065u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000066u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000067u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000006Au,0,0,0,0,0x01C2,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000006Bu,8,64,0,0,0x01C2,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x0000006Cu,0,0,0,0,0x01C2,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000006Du,0,0,0,0,0x01C2,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000006Eu,8,32,0,0,0x01C2,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x0000006Fu,0,0,0,0,0x01C2,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000070u,4,27,0,0,0x01C3,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000071u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000072u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000073u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000074u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000075u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000076u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000077u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000078u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000079u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000080u,8,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000081u,8,0,0,0,0x0010,0x0000000000000000ULL,0x0000000000000000ULL},
};

bool helf_tilegx_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto)
{
    if(howto==NULL)
    {
        return false;
    }
    size_t count=sizeof(helf_tilegx_reloc_howto)/sizeof(helf_tilegx_reloc_howto[0]);
    if(count==0)
    {
        return false;
    }
    for(size_t i=0;i<count;i++)
    {
        if(helf_tilegx_reloc_howto[i].type==type)
        {
            *howto=helf_tilegx_reloc_howto[i];
            return true;
        }
    }
    return false;
}
/* helf_relocation_howto_end */


