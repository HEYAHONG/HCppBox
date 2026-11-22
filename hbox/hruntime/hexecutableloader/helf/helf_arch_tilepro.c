/***************************************************************
 * Name:      helf_arch_tilepro.c
 * Purpose:   实现helf_arch_tilepro接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "helf_arch_tilepro.h"

static const uint16_t helf_tilepro_machines[]=
{
    HELF_EM_TILEPRO,
};

bool helf_tilepro_is_tilepro(helf_file_input_t *input_file)
{
    return helf_arch_is(input_file,helf_tilepro_machines,sizeof(helf_tilepro_machines)/sizeof(helf_tilepro_machines[0]),HELF_ELFCLASS_32);
}

bool helf_tilepro_is_big_endian(helf_file_input_t *input_file)
{
    if(!helf_tilepro_is_tilepro(input_file))
    {
        return false;
    }
    return helf_file_input_is_big_endian(input_file);
}

bool helf_tilepro_is_executable(helf_file_input_t *input_file)
{
    return helf_arch_is_executable(input_file,helf_tilepro_machines,sizeof(helf_tilepro_machines)/sizeof(helf_tilepro_machines[0]),HELF_ELFCLASS_32);
}

bool helf_tilepro_entry_get(helf_file_input_t *input_file,uint32_t *entry)
{
    if(entry==NULL)
    {
        return false;
    }
    if(!helf_tilepro_is_tilepro(input_file))
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

size_t helf_tilepro_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_tilepro_is_tilepro(input_file))
    {
        return 0;
    }
    return helf_arch_section_count_get(input_file);
}

bool helf_tilepro_section_info_get(helf_file_input_t *input_file,size_t index,helf_tilepro_section_info_t *info)
{
    if(!helf_tilepro_is_tilepro(input_file))
    {
        return false;
    }
    return helf_arch_section_info_get(input_file,index,info);
}

bool helf_tilepro_is_rx_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tilepro_is_tilepro(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_section(index,input_file);
}

intptr_t helf_tilepro_rx_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tilepro_is_rx_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rx_section_offset_get(index,input_file);
}

size_t helf_tilepro_rx_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_tilepro_is_tilepro(input_file))
    {
        return 0;
    }
    return helf_arch_rx_section_size_get(input_file);
}

bool helf_tilepro_is_ro_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tilepro_is_tilepro(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_section(index,input_file);
}

intptr_t helf_tilepro_ro_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tilepro_is_ro_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_ro_section_offset_get(index,input_file);
}

size_t helf_tilepro_ro_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_tilepro_is_tilepro(input_file))
    {
        return 0;
    }
    return helf_arch_ro_section_size_get(input_file);
}

bool helf_tilepro_is_rw_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tilepro_is_tilepro(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_section(index,input_file);
}

intptr_t helf_tilepro_rw_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tilepro_is_rw_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rw_section_offset_get(index,input_file);
}

size_t helf_tilepro_rw_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_tilepro_is_tilepro(input_file))
    {
        return 0;
    }
    return helf_arch_rw_section_size_get(input_file);
}

bool helf_tilepro_is_bss_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tilepro_is_tilepro(input_file))
    {
        return false;
    }
    return helf_arch_is_bss_section(index,input_file);
}

size_t helf_tilepro_program_header_count_get(helf_file_input_t *input_file)
{
    if(!helf_tilepro_is_tilepro(input_file))
    {
        return 0;
    }
    return helf_arch_program_header_count_get(input_file);
}

bool helf_tilepro_program_header_get(helf_file_input_t *input_file,size_t index,helf_elf32_program_header_t *programheader)
{
    if(programheader==NULL)
    {
        return false;
    }
    if(!helf_tilepro_is_tilepro(input_file))
    {
        return false;
    }
    if(index>=helf_arch_program_header_count_get(input_file))
    {
        return false;
    }
    return helf_file_input_32_bits_program_header_get(input_file,index,programheader);
}

size_t helf_tilepro_segment_count_get(helf_file_input_t *input_file)
{
    return helf_tilepro_program_header_count_get(input_file);
}

bool helf_tilepro_segment_info_get(helf_file_input_t *input_file,size_t index,helf_tilepro_segment_info_t *info)
{
    if(!helf_tilepro_is_tilepro(input_file))
    {
        return false;
    }
    return helf_arch_segment_info_get(input_file,index,info);
}

bool helf_tilepro_is_load_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tilepro_is_tilepro(input_file))
    {
        return false;
    }
    return helf_arch_is_load_segment(index,input_file);
}

bool helf_tilepro_is_rx_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tilepro_is_tilepro(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_segment(index,input_file);
}

bool helf_tilepro_is_ro_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tilepro_is_tilepro(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_segment(index,input_file);
}

bool helf_tilepro_is_rw_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tilepro_is_tilepro(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_segment(index,input_file);
}

size_t helf_tilepro_load_segment_count_get(helf_file_input_t *input_file)
{
    if(!helf_tilepro_is_tilepro(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_count_get(input_file);
}

bool helf_tilepro_load_segment_info_get(helf_file_input_t *input_file,size_t index,helf_tilepro_segment_info_t *info)
{
    if(!helf_tilepro_is_tilepro(input_file))
    {
        return false;
    }
    return helf_arch_load_segment_info_get(input_file,index,info);
}

size_t helf_tilepro_load_segment_total_filesz_get(helf_file_input_t *input_file)
{
    if(!helf_tilepro_is_tilepro(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_filesz_get(input_file);
}

size_t helf_tilepro_load_segment_total_memsz_get(helf_file_input_t *input_file)
{
    if(!helf_tilepro_is_tilepro(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_memsz_get(input_file);
}

size_t helf_tilepro_relocation_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_tilepro_is_tilepro(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_section_count_get(input_file);
}

bool helf_tilepro_relocation_section_info_get(helf_file_input_t *input_file,size_t index,helf_arch_relocation_section_info_t *info)
{
    if(!helf_tilepro_is_tilepro(input_file))
    {
        return false;
    }
    return helf_arch_relocation_section_info_get(input_file,index,info);
}

size_t helf_tilepro_relocation_count_get(helf_file_input_t *input_file,size_t section_index)
{
    if(!helf_tilepro_is_tilepro(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_count_get(input_file,section_index);
}

bool helf_tilepro_relocation_get(helf_file_input_t *input_file,size_t section_index,size_t index,helf_arch_relocation_info_t *info)
{
    if(!helf_tilepro_is_tilepro(input_file))
    {
        return false;
    }
    if(!helf_arch_relocation_get(input_file,section_index,index,info))
    {
        return false;
    }
    return true;
}

bool helf_tilepro_relocation_symbol_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_symbol_info_t *info)
{
    if(!helf_tilepro_is_tilepro(input_file))
    {
        return false;
    }
    return helf_arch_relocation_symbol_info_get(input_file,relocation,info);
}

static const helf_arch_relocation_type_name_t helf_tilepro_relocation_type_names[]=
{
    {0,0,"R_TILEPRO_NONE"},
    {1,4,"R_TILEPRO_32"},
    {2,2,"R_TILEPRO_16"},
    {3,1,"R_TILEPRO_8"},
    {4,4,"R_TILEPRO_32_PCREL"},
    {5,2,"R_TILEPRO_16_PCREL"},
    {6,1,"R_TILEPRO_8_PCREL"},
    {7,2,"R_TILEPRO_LO16"},
    {8,2,"R_TILEPRO_HI16"},
    {9,2,"R_TILEPRO_HA16"},
    {10,0,"R_TILEPRO_COPY"},
    {11,4,"R_TILEPRO_GLOB_DAT"},
    {12,4,"R_TILEPRO_JMP_SLOT"},
    {13,4,"R_TILEPRO_RELATIVE"},
    {14,4,"R_TILEPRO_BROFF_X1"},
    {15,4,"R_TILEPRO_JOFFLONG_X1"},
    {16,4,"R_TILEPRO_JOFFLONG_X1_PLT"},
    {17,0,"R_TILEPRO_IMM8_X0"},
    {18,0,"R_TILEPRO_IMM8_Y0"},
    {19,0,"R_TILEPRO_IMM8_X1"},
    {20,0,"R_TILEPRO_IMM8_Y1"},
    {21,0,"R_TILEPRO_MT_IMM15_X1"},
    {22,0,"R_TILEPRO_MF_IMM15_X1"},
    {23,0,"R_TILEPRO_IMM16_X0"},
    {24,0,"R_TILEPRO_IMM16_X1"},
    {25,0,"R_TILEPRO_IMM16_X0_LO"},
    {26,0,"R_TILEPRO_IMM16_X1_LO"},
    {27,0,"R_TILEPRO_IMM16_X0_HI"},
    {28,0,"R_TILEPRO_IMM16_X1_HI"},
    {29,0,"R_TILEPRO_IMM16_X0_HA"},
    {30,0,"R_TILEPRO_IMM16_X1_HA"},
    {31,2,"R_TILEPRO_IMM16_X0_PCREL"},
    {32,2,"R_TILEPRO_IMM16_X1_PCREL"},
    {33,0,"R_TILEPRO_IMM16_X0_LO_PCREL"},
    {34,0,"R_TILEPRO_IMM16_X1_LO_PCREL"},
    {35,0,"R_TILEPRO_IMM16_X0_HI_PCREL"},
    {36,0,"R_TILEPRO_IMM16_X1_HI_PCREL"},
    {37,0,"R_TILEPRO_IMM16_X0_HA_PCREL"},
    {38,0,"R_TILEPRO_IMM16_X1_HA_PCREL"},
    {39,0,"R_TILEPRO_IMM16_X0_GOT"},
    {40,0,"R_TILEPRO_IMM16_X1_GOT"},
    {41,0,"R_TILEPRO_IMM16_X0_GOT_LO"},
    {42,0,"R_TILEPRO_IMM16_X1_GOT_LO"},
    {43,0,"R_TILEPRO_IMM16_X0_GOT_HI"},
    {44,0,"R_TILEPRO_IMM16_X1_GOT_HI"},
    {45,0,"R_TILEPRO_IMM16_X0_GOT_HA"},
    {46,0,"R_TILEPRO_IMM16_X1_GOT_HA"},
    {47,0,"R_TILEPRO_MMSTART_X0"},
    {48,0,"R_TILEPRO_MMEND_X0"},
    {49,0,"R_TILEPRO_MMSTART_X1"},
    {50,0,"R_TILEPRO_MMEND_X1"},
    {51,0,"R_TILEPRO_SHAMT_X0"},
    {52,0,"R_TILEPRO_SHAMT_X1"},
    {53,0,"R_TILEPRO_SHAMT_Y0"},
    {54,0,"R_TILEPRO_SHAMT_Y1"},
    {55,0,"R_TILEPRO_DEST_IMM8_X1"},
    {60,4,"R_TILEPRO_TLS_GD_CALL"},
    {61,0,"R_TILEPRO_IMM8_X0_TLS_GD_ADD"},
    {62,0,"R_TILEPRO_IMM8_X1_TLS_GD_ADD"},
    {63,0,"R_TILEPRO_IMM8_Y0_TLS_GD_ADD"},
    {64,0,"R_TILEPRO_IMM8_Y1_TLS_GD_ADD"},
    {65,0,"R_TILEPRO_TLS_IE_LOAD"},
    {66,2,"R_TILEPRO_IMM16_X0_TLS_GD"},
    {67,2,"R_TILEPRO_IMM16_X1_TLS_GD"},
    {68,0,"R_TILEPRO_IMM16_X0_TLS_GD_LO"},
    {69,0,"R_TILEPRO_IMM16_X1_TLS_GD_LO"},
    {70,0,"R_TILEPRO_IMM16_X0_TLS_GD_HI"},
    {71,0,"R_TILEPRO_IMM16_X1_TLS_GD_HI"},
    {72,0,"R_TILEPRO_IMM16_X0_TLS_GD_HA"},
    {73,0,"R_TILEPRO_IMM16_X1_TLS_GD_HA"},
    {74,2,"R_TILEPRO_IMM16_X0_TLS_IE"},
    {75,2,"R_TILEPRO_IMM16_X1_TLS_IE"},
    {76,0,"R_TILEPRO_IMM16_X0_TLS_IE_LO"},
    {77,0,"R_TILEPRO_IMM16_X1_TLS_IE_LO"},
    {78,0,"R_TILEPRO_IMM16_X0_TLS_IE_HI"},
    {79,0,"R_TILEPRO_IMM16_X1_TLS_IE_HI"},
    {80,0,"R_TILEPRO_IMM16_X0_TLS_IE_HA"},
    {81,0,"R_TILEPRO_IMM16_X1_TLS_IE_HA"},
    {82,0,"R_TILEPRO_TLS_DTPMOD32"},
    {83,4,"R_TILEPRO_TLS_DTPOFF32"},
    {84,0,"R_TILEPRO_TLS_TPOFF32"},
    {85,2,"R_TILEPRO_IMM16_X0_TLS_LE"},
    {86,2,"R_TILEPRO_IMM16_X1_TLS_LE"},
    {87,0,"R_TILEPRO_IMM16_X0_TLS_LE_LO"},
    {88,0,"R_TILEPRO_IMM16_X1_TLS_LE_LO"},
    {89,0,"R_TILEPRO_IMM16_X0_TLS_LE_HI"},
    {90,0,"R_TILEPRO_IMM16_X1_TLS_LE_HI"},
    {91,0,"R_TILEPRO_IMM16_X0_TLS_LE_HA"},
    {92,0,"R_TILEPRO_IMM16_X1_TLS_LE_HA"},
    {128,4,"R_TILEPRO_GNU_VTINHERIT"},
    {129,4,"R_TILEPRO_GNU_VTENTRY"},
};

const char *helf_tilepro_relocation_type_name_get(uint32_t type)
{
    return helf_arch_relocation_type_name_get(helf_tilepro_relocation_type_names,sizeof(helf_tilepro_relocation_type_names)/sizeof(helf_tilepro_relocation_type_names[0]),type);
}

uint8_t helf_tilepro_relocation_size_get(uint32_t type)
{
    return helf_arch_relocation_type_size_get(helf_tilepro_relocation_type_names,sizeof(helf_tilepro_relocation_type_names)/sizeof(helf_tilepro_relocation_type_names[0]),type);
}

const char *helf_tilepro_section_type_name_get(uint32_t type)
{
    (void)type;
    return NULL;
}

bool helf_tilepro_relocation_address_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_relocation_address_info_t *info)
{
    if(!helf_tilepro_is_tilepro(input_file) || relocation==NULL)
    {
        return false;
    }
    uint8_t size=helf_tilepro_relocation_size_get(relocation->type);
    if(size==0)
    {
        return false;
    }
    return helf_arch_relocation_address_info_get(input_file,relocation,size,info);
}

size_t helf_tilepro_relocation_data_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,void *buffer,size_t buffer_length)
{
    if(!helf_tilepro_is_tilepro(input_file) || relocation==NULL)
    {
        return 0;
    }
    uint8_t size=helf_tilepro_relocation_size_get(relocation->type);
    if(size==0)
    {
        return 0;
    }
    return helf_arch_relocation_data_get(input_file,relocation,size,buffer,buffer_length);
}

bool helf_tilepro_is_shared_object(helf_file_input_t *input_file)
{
    return helf_arch_is_shared_object(input_file,helf_tilepro_machines,sizeof(helf_tilepro_machines)/sizeof(helf_tilepro_machines[0]),HELF_ELFCLASS_32);
}

bool helf_tilepro_is_relocatable(helf_file_input_t *input_file)
{
    return helf_arch_is_relocatable(input_file,helf_tilepro_machines,sizeof(helf_tilepro_machines)/sizeof(helf_tilepro_machines[0]),HELF_ELFCLASS_32);
}

size_t helf_tilepro_dynamic_count_get(helf_file_input_t *input_file)
{
    if(!helf_tilepro_is_tilepro(input_file))
    {
        return 0;
    }
    return helf_arch_dynamic_count_get(input_file);
}

bool helf_tilepro_dynamic_info_get(helf_file_input_t *input_file,size_t index,helf_arch_dynamic_info_t *info)
{
    if(!helf_tilepro_is_tilepro(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_info_get(input_file,index,info);
}

bool helf_tilepro_dynamic_string_get(helf_file_input_t *input_file,uint64_t offset,char *buffer,size_t buffer_length)
{
    if(!helf_tilepro_is_tilepro(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_string_get(input_file,offset,buffer,buffer_length);
}

/* helf_relocation_howto_begin */
static const helf_arch_relocation_howto_t helf_tilepro_reloc_howto[]=
{
    {0x00000000u,0,0,0,0,0x0040,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000001u,4,32,0,0,0x0000,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000002u,2,16,0,0,0x0000,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000003u,1,8,0,0,0x0000,0x0000000000000000ULL,0x00000000000000FFULL},
    {0x00000004u,4,32,0,0,0x0003,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000005u,2,16,0,0,0x0003,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000006u,1,8,0,0,0x0003,0x0000000000000000ULL,0x00000000000000FFULL},
    {0x00000007u,2,16,0,0,0x0000,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000008u,2,16,16,0,0x0000,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000009u,2,16,16,0,0x0000,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000000Au,0,0,0,0,0x0182,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000000Bu,4,0,0,0,0x0102,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x0000000Cu,4,0,0,0,0x0142,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x0000000Du,4,0,0,0,0x0082,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x0000000Eu,4,17,0,0,0x0003,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x0000000Fu,4,29,0,0,0x0003,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000010u,4,29,0,0,0x0003,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000011u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000012u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000013u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000014u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000015u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000016u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000017u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000018u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000019u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Au,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Bu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Cu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Du,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Eu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Fu,2,16,0,0,0x0003,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000020u,2,16,0,0,0x0003,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000021u,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000022u,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000023u,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000024u,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000025u,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000026u,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000027u,0,0,0,0,0x0200,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000028u,0,0,0,0,0x0200,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000029u,0,0,0,0,0x0200,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000002Au,0,0,0,0,0x0200,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000002Bu,0,0,0,0,0x0200,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000002Cu,0,0,0,0,0x0200,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000002Du,0,0,0,0,0x0200,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000002Eu,0,0,0,0,0x0200,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000002Fu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000030u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000031u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000032u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000033u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000034u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000035u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000036u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000037u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000003Cu,4,29,0,0,0x01C3,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x0000003Du,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000003Eu,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000003Fu,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000040u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000041u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000042u,2,16,0,0,0x01C0,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000043u,2,16,0,0,0x01C0,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000044u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000045u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000046u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000047u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000048u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000049u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000004Au,2,16,0,0,0x01C2,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x0000004Bu,2,16,0,0,0x01C2,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x0000004Cu,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000004Du,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000004Eu,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000004Fu,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000050u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000051u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000052u,0,0,0,0,0x01C2,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000053u,4,32,0,0,0x01C2,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000054u,0,0,0,0,0x01C2,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000055u,2,16,0,0,0x01C2,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000056u,2,16,0,0,0x01C2,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000057u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000058u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000059u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000005Au,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000005Bu,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000005Cu,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000080u,4,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000081u,4,0,0,0,0x0010,0x0000000000000000ULL,0x0000000000000000ULL},
};

bool helf_tilepro_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto)
{
    if(howto==NULL)
    {
        return false;
    }
    size_t count=sizeof(helf_tilepro_reloc_howto)/sizeof(helf_tilepro_reloc_howto[0]);
    if(count==0)
    {
        return false;
    }
    for(size_t i=0;i<count;i++)
    {
        if(helf_tilepro_reloc_howto[i].type==type)
        {
            *howto=helf_tilepro_reloc_howto[i];
            return true;
        }
    }
    return false;
}
/* helf_relocation_howto_end */

