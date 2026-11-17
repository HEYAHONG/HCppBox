/***************************************************************
 * Name:      helf_arch_tile64.c
 * Purpose:   实现helf_arch_tile64接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "helf_arch_tile64.h"

static const uint16_t helf_tile64_machines[]=
{
    HELF_EM_TILE64,
};

bool helf_tile64_is_tile64(helf_file_input_t *input_file)
{
    return helf_arch_is(input_file,helf_tile64_machines,sizeof(helf_tile64_machines)/sizeof(helf_tile64_machines[0]),HELF_ELFCLASS_64);
}

bool helf_tile64_is_big_endian(helf_file_input_t *input_file)
{
    if(!helf_tile64_is_tile64(input_file))
    {
        return false;
    }
    return helf_file_input_is_big_endian(input_file);
}

bool helf_tile64_is_executable(helf_file_input_t *input_file)
{
    return helf_arch_is_executable(input_file,helf_tile64_machines,sizeof(helf_tile64_machines)/sizeof(helf_tile64_machines[0]),HELF_ELFCLASS_64);
}

bool helf_tile64_entry_get(helf_file_input_t *input_file,uint64_t *entry)
{
    if(entry==NULL)
    {
        return false;
    }
    if(!helf_tile64_is_tile64(input_file))
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

size_t helf_tile64_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_tile64_is_tile64(input_file))
    {
        return 0;
    }
    return helf_arch_section_count_get(input_file);
}

bool helf_tile64_section_info_get(helf_file_input_t *input_file,size_t index,helf_tile64_section_info_t *info)
{
    if(!helf_tile64_is_tile64(input_file))
    {
        return false;
    }
    return helf_arch_section_info_get(input_file,index,info);
}

bool helf_tile64_is_rx_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tile64_is_tile64(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_section(index,input_file);
}

intptr_t helf_tile64_rx_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tile64_is_rx_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rx_section_offset_get(index,input_file);
}

size_t helf_tile64_rx_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_tile64_is_tile64(input_file))
    {
        return 0;
    }
    return helf_arch_rx_section_size_get(input_file);
}

bool helf_tile64_is_ro_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tile64_is_tile64(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_section(index,input_file);
}

intptr_t helf_tile64_ro_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tile64_is_ro_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_ro_section_offset_get(index,input_file);
}

size_t helf_tile64_ro_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_tile64_is_tile64(input_file))
    {
        return 0;
    }
    return helf_arch_ro_section_size_get(input_file);
}

bool helf_tile64_is_rw_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tile64_is_tile64(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_section(index,input_file);
}

intptr_t helf_tile64_rw_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tile64_is_rw_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rw_section_offset_get(index,input_file);
}

size_t helf_tile64_rw_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_tile64_is_tile64(input_file))
    {
        return 0;
    }
    return helf_arch_rw_section_size_get(input_file);
}

bool helf_tile64_is_bss_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tile64_is_tile64(input_file))
    {
        return false;
    }
    return helf_arch_is_bss_section(index,input_file);
}

size_t helf_tile64_program_header_count_get(helf_file_input_t *input_file)
{
    if(!helf_tile64_is_tile64(input_file))
    {
        return 0;
    }
    return helf_arch_program_header_count_get(input_file);
}

bool helf_tile64_program_header_get(helf_file_input_t *input_file,size_t index,helf_elf64_program_header_t *programheader)
{
    if(programheader==NULL)
    {
        return false;
    }
    if(!helf_tile64_is_tile64(input_file))
    {
        return false;
    }
    if(index>=helf_arch_program_header_count_get(input_file))
    {
        return false;
    }
    return helf_file_input_64_bits_program_header_get(input_file,index,programheader);
}

size_t helf_tile64_segment_count_get(helf_file_input_t *input_file)
{
    return helf_tile64_program_header_count_get(input_file);
}

bool helf_tile64_segment_info_get(helf_file_input_t *input_file,size_t index,helf_tile64_segment_info_t *info)
{
    if(!helf_tile64_is_tile64(input_file))
    {
        return false;
    }
    return helf_arch_segment_info_get(input_file,index,info);
}

bool helf_tile64_is_load_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tile64_is_tile64(input_file))
    {
        return false;
    }
    return helf_arch_is_load_segment(index,input_file);
}

bool helf_tile64_is_rx_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tile64_is_tile64(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_segment(index,input_file);
}

bool helf_tile64_is_ro_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tile64_is_tile64(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_segment(index,input_file);
}

bool helf_tile64_is_rw_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tile64_is_tile64(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_segment(index,input_file);
}

size_t helf_tile64_load_segment_count_get(helf_file_input_t *input_file)
{
    if(!helf_tile64_is_tile64(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_count_get(input_file);
}

bool helf_tile64_load_segment_info_get(helf_file_input_t *input_file,size_t index,helf_tile64_segment_info_t *info)
{
    if(!helf_tile64_is_tile64(input_file))
    {
        return false;
    }
    return helf_arch_load_segment_info_get(input_file,index,info);
}

size_t helf_tile64_load_segment_total_filesz_get(helf_file_input_t *input_file)
{
    if(!helf_tile64_is_tile64(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_filesz_get(input_file);
}

size_t helf_tile64_load_segment_total_memsz_get(helf_file_input_t *input_file)
{
    if(!helf_tile64_is_tile64(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_memsz_get(input_file);
}

size_t helf_tile64_relocation_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_tile64_is_tile64(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_section_count_get(input_file);
}

bool helf_tile64_relocation_section_info_get(helf_file_input_t *input_file,size_t index,helf_arch_relocation_section_info_t *info)
{
    if(!helf_tile64_is_tile64(input_file))
    {
        return false;
    }
    return helf_arch_relocation_section_info_get(input_file,index,info);
}

size_t helf_tile64_relocation_count_get(helf_file_input_t *input_file,size_t section_index)
{
    if(!helf_tile64_is_tile64(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_count_get(input_file,section_index);
}

bool helf_tile64_relocation_get(helf_file_input_t *input_file,size_t section_index,size_t index,helf_arch_relocation_info_t *info)
{
    if(!helf_tile64_is_tile64(input_file))
    {
        return false;
    }
    if(!helf_arch_relocation_get(input_file,section_index,index,info))
    {
        return false;
    }
    return true;
}

bool helf_tile64_relocation_symbol_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_symbol_info_t *info)
{
    if(!helf_tile64_is_tile64(input_file))
    {
        return false;
    }
    return helf_arch_relocation_symbol_info_get(input_file,relocation,info);
}

static const helf_arch_relocation_type_name_t helf_tile64_relocation_type_names[]=
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
    {17,0,"R_TILEGX_GLOB_DAT"},
    {18,0,"R_TILEGX_JMP_SLOT"},
    {19,0,"R_TILEGX_RELATIVE"},
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

const char *helf_tile64_relocation_type_name_get(uint32_t type)
{
    return helf_arch_relocation_type_name_get(helf_tile64_relocation_type_names,sizeof(helf_tile64_relocation_type_names)/sizeof(helf_tile64_relocation_type_names[0]),type);
}

uint8_t helf_tile64_relocation_size_get(uint32_t type)
{
    return helf_arch_relocation_type_size_get(helf_tile64_relocation_type_names,sizeof(helf_tile64_relocation_type_names)/sizeof(helf_tile64_relocation_type_names[0]),type);
}

const char *helf_tile64_section_type_name_get(uint32_t type)
{
    (void)type;
    return NULL;
}

bool helf_tile64_relocation_address_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_relocation_address_info_t *info)
{
    if(!helf_tile64_is_tile64(input_file) || relocation==NULL)
    {
        return false;
    }
    uint8_t size=helf_tile64_relocation_size_get(relocation->type);
    if(size==0)
    {
        return false;
    }
    return helf_arch_relocation_address_info_get(input_file,relocation,size,info);
}

size_t helf_tile64_relocation_data_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,void *buffer,size_t buffer_length)
{
    if(!helf_tile64_is_tile64(input_file) || relocation==NULL)
    {
        return 0;
    }
    uint8_t size=helf_tile64_relocation_size_get(relocation->type);
    if(size==0)
    {
        return 0;
    }
    return helf_arch_relocation_data_get(input_file,relocation,size,buffer,buffer_length);
}

bool helf_tile64_is_shared_object(helf_file_input_t *input_file)
{
    return helf_arch_is_shared_object(input_file,helf_tile64_machines,sizeof(helf_tile64_machines)/sizeof(helf_tile64_machines[0]),HELF_ELFCLASS_64);
}

bool helf_tile64_is_relocatable(helf_file_input_t *input_file)
{
    return helf_arch_is_relocatable(input_file,helf_tile64_machines,sizeof(helf_tile64_machines)/sizeof(helf_tile64_machines[0]),HELF_ELFCLASS_64);
}

size_t helf_tile64_dynamic_count_get(helf_file_input_t *input_file)
{
    if(!helf_tile64_is_tile64(input_file))
    {
        return 0;
    }
    return helf_arch_dynamic_count_get(input_file);
}

bool helf_tile64_dynamic_info_get(helf_file_input_t *input_file,size_t index,helf_arch_dynamic_info_t *info)
{
    if(!helf_tile64_is_tile64(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_info_get(input_file,index,info);
}

bool helf_tile64_dynamic_string_get(helf_file_input_t *input_file,uint64_t offset,char *buffer,size_t buffer_length)
{
    if(!helf_tile64_is_tile64(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_string_get(input_file,offset,buffer,buffer_length);
}
