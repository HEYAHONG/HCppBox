/***************************************************************
 * Name:      helf_arch_ppc.c
 * Purpose:   实现helf_arch_ppc接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "helf_arch_ppc.h"

static const uint16_t helf_ppc_machines[]=
{
    HELF_EM_PPC,
    HELF_EM_PPC_OLD,
    HELF_EM_CYGNUS_POWERPC,
};

bool helf_ppc_is_ppc(helf_file_input_t *input_file)
{
    return helf_arch_is(input_file,helf_ppc_machines,sizeof(helf_ppc_machines)/sizeof(helf_ppc_machines[0]),HELF_ELFCLASS_32);
}

bool helf_ppc_is_big_endian(helf_file_input_t *input_file)
{
    if(!helf_ppc_is_ppc(input_file))
    {
        return false;
    }
    return helf_file_input_is_big_endian(input_file);
}

bool helf_ppc_is_executable(helf_file_input_t *input_file)
{
    return helf_arch_is_executable(input_file,helf_ppc_machines,sizeof(helf_ppc_machines)/sizeof(helf_ppc_machines[0]),HELF_ELFCLASS_32);
}

bool helf_ppc_entry_get(helf_file_input_t *input_file,uint32_t *entry)
{
    if(entry==NULL)
    {
        return false;
    }
    if(!helf_ppc_is_ppc(input_file))
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

size_t helf_ppc_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_ppc_is_ppc(input_file))
    {
        return 0;
    }
    return helf_arch_section_count_get(input_file);
}

bool helf_ppc_section_info_get(helf_file_input_t *input_file,size_t index,helf_ppc_section_info_t *info)
{
    if(!helf_ppc_is_ppc(input_file))
    {
        return false;
    }
    return helf_arch_section_info_get(input_file,index,info);
}

bool helf_ppc_is_rx_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ppc_is_ppc(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_section(index,input_file);
}

intptr_t helf_ppc_rx_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ppc_is_rx_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rx_section_offset_get(index,input_file);
}

size_t helf_ppc_rx_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_ppc_is_ppc(input_file))
    {
        return 0;
    }
    return helf_arch_rx_section_size_get(input_file);
}

bool helf_ppc_is_ro_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ppc_is_ppc(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_section(index,input_file);
}

intptr_t helf_ppc_ro_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ppc_is_ro_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_ro_section_offset_get(index,input_file);
}

size_t helf_ppc_ro_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_ppc_is_ppc(input_file))
    {
        return 0;
    }
    return helf_arch_ro_section_size_get(input_file);
}

bool helf_ppc_is_rw_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ppc_is_ppc(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_section(index,input_file);
}

intptr_t helf_ppc_rw_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ppc_is_rw_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rw_section_offset_get(index,input_file);
}

size_t helf_ppc_rw_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_ppc_is_ppc(input_file))
    {
        return 0;
    }
    return helf_arch_rw_section_size_get(input_file);
}

bool helf_ppc_is_bss_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ppc_is_ppc(input_file))
    {
        return false;
    }
    return helf_arch_is_bss_section(index,input_file);
}

size_t helf_ppc_program_header_count_get(helf_file_input_t *input_file)
{
    if(!helf_ppc_is_ppc(input_file))
    {
        return 0;
    }
    return helf_arch_program_header_count_get(input_file);
}

bool helf_ppc_program_header_get(helf_file_input_t *input_file,size_t index,helf_elf32_program_header_t *programheader)
{
    if(programheader==NULL)
    {
        return false;
    }
    if(!helf_ppc_is_ppc(input_file))
    {
        return false;
    }
    if(index>=helf_arch_program_header_count_get(input_file))
    {
        return false;
    }
    return helf_file_input_32_bits_program_header_get(input_file,index,programheader);
}

size_t helf_ppc_segment_count_get(helf_file_input_t *input_file)
{
    return helf_ppc_program_header_count_get(input_file);
}

bool helf_ppc_segment_info_get(helf_file_input_t *input_file,size_t index,helf_ppc_segment_info_t *info)
{
    if(!helf_ppc_is_ppc(input_file))
    {
        return false;
    }
    return helf_arch_segment_info_get(input_file,index,info);
}

bool helf_ppc_is_load_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ppc_is_ppc(input_file))
    {
        return false;
    }
    return helf_arch_is_load_segment(index,input_file);
}

bool helf_ppc_is_rx_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ppc_is_ppc(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_segment(index,input_file);
}

bool helf_ppc_is_ro_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ppc_is_ppc(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_segment(index,input_file);
}

bool helf_ppc_is_rw_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ppc_is_ppc(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_segment(index,input_file);
}

size_t helf_ppc_load_segment_count_get(helf_file_input_t *input_file)
{
    if(!helf_ppc_is_ppc(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_count_get(input_file);
}

bool helf_ppc_load_segment_info_get(helf_file_input_t *input_file,size_t index,helf_ppc_segment_info_t *info)
{
    if(!helf_ppc_is_ppc(input_file))
    {
        return false;
    }
    return helf_arch_load_segment_info_get(input_file,index,info);
}

size_t helf_ppc_load_segment_total_filesz_get(helf_file_input_t *input_file)
{
    if(!helf_ppc_is_ppc(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_filesz_get(input_file);
}

size_t helf_ppc_load_segment_total_memsz_get(helf_file_input_t *input_file)
{
    if(!helf_ppc_is_ppc(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_memsz_get(input_file);
}

size_t helf_ppc_relocation_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_ppc_is_ppc(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_section_count_get(input_file);
}

bool helf_ppc_relocation_section_info_get(helf_file_input_t *input_file,size_t index,helf_arch_relocation_section_info_t *info)
{
    if(!helf_ppc_is_ppc(input_file))
    {
        return false;
    }
    return helf_arch_relocation_section_info_get(input_file,index,info);
}

size_t helf_ppc_relocation_count_get(helf_file_input_t *input_file,size_t section_index)
{
    if(!helf_ppc_is_ppc(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_count_get(input_file,section_index);
}

bool helf_ppc_relocation_get(helf_file_input_t *input_file,size_t section_index,size_t index,helf_arch_relocation_info_t *info)
{
    if(!helf_ppc_is_ppc(input_file))
    {
        return false;
    }
    if(!helf_arch_relocation_get(input_file,section_index,index,info))
    {
        return false;
    }
    return true;
}

bool helf_ppc_relocation_symbol_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_symbol_info_t *info)
{
    if(!helf_ppc_is_ppc(input_file))
    {
        return false;
    }
    return helf_arch_relocation_symbol_info_get(input_file,relocation,info);
}

static const helf_arch_relocation_type_name_t helf_ppc_relocation_type_names[]=
{
    {0,0,"R_PPC_NONE"},
    {1,4,"R_PPC_ADDR32"},
    {2,4,"R_PPC_ADDR24"},
    {3,2,"R_PPC_ADDR16"},
    {4,2,"R_PPC_ADDR16_LO"},
    {5,2,"R_PPC_ADDR16_HI"},
    {6,2,"R_PPC_ADDR16_HA"},
    {7,4,"R_PPC_ADDR14"},
    {8,4,"R_PPC_ADDR14_BRTAKEN"},
    {9,4,"R_PPC_ADDR14_BRNTAKEN"},
    {10,4,"R_PPC_REL24"},
    {11,4,"R_PPC_REL14"},
    {12,4,"R_PPC_REL14_BRTAKEN"},
    {13,4,"R_PPC_REL14_BRNTAKEN"},
    {14,2,"R_PPC_GOT16"},
    {15,2,"R_PPC_GOT16_LO"},
    {16,2,"R_PPC_GOT16_HI"},
    {17,2,"R_PPC_GOT16_HA"},
    {18,4,"R_PPC_PLTREL24"},
    {19,4,"R_PPC_COPY"},
    {20,4,"R_PPC_GLOB_DAT"},
    {21,4,"R_PPC_JMP_SLOT"},
    {22,4,"R_PPC_RELATIVE"},
    {23,4,"R_PPC_LOCAL24PC"},
    {24,4,"R_PPC_UADDR32"},
    {25,2,"R_PPC_UADDR16"},
    {26,4,"R_PPC_REL32"},
    {27,4,"R_PPC_PLT32"},
    {28,4,"R_PPC_PLTREL32"},
    {29,2,"R_PPC_PLT16_LO"},
    {30,2,"R_PPC_PLT16_HI"},
    {31,2,"R_PPC_PLT16_HA"},
    {32,2,"R_PPC_SDAREL16"},
    {33,2,"R_PPC_SECTOFF"},
    {34,2,"R_PPC_SECTOFF_LO"},
    {35,2,"R_PPC_SECTOFF_HI"},
    {36,2,"R_PPC_SECTOFF_HA"},
    {37,0,"R_PPC_ADDR30"},
    {48,0,"R_PPC_RELAX"},
    {49,0,"R_PPC_RELAX_PLT"},
    {50,0,"R_PPC_RELAX_PLTREL24"},
    {51,4,"R_PPC_16DX_HA"},
    {67,4,"R_PPC_TLS"},
    {68,4,"R_PPC_DTPMOD32"},
    {69,2,"R_PPC_TPREL16"},
    {70,2,"R_PPC_TPREL16_LO"},
    {71,2,"R_PPC_TPREL16_HI"},
    {72,2,"R_PPC_TPREL16_HA"},
    {73,4,"R_PPC_TPREL32"},
    {74,2,"R_PPC_DTPREL16"},
    {75,2,"R_PPC_DTPREL16_LO"},
    {76,2,"R_PPC_DTPREL16_HI"},
    {77,2,"R_PPC_DTPREL16_HA"},
    {78,4,"R_PPC_DTPREL32"},
    {79,2,"R_PPC_GOT_TLSGD16"},
    {80,2,"R_PPC_GOT_TLSGD16_LO"},
    {81,2,"R_PPC_GOT_TLSGD16_HI"},
    {82,2,"R_PPC_GOT_TLSGD16_HA"},
    {83,2,"R_PPC_GOT_TLSLD16"},
    {84,2,"R_PPC_GOT_TLSLD16_LO"},
    {85,2,"R_PPC_GOT_TLSLD16_HI"},
    {86,2,"R_PPC_GOT_TLSLD16_HA"},
    {87,2,"R_PPC_GOT_TPREL16"},
    {88,2,"R_PPC_GOT_TPREL16_LO"},
    {89,2,"R_PPC_GOT_TPREL16_HI"},
    {90,2,"R_PPC_GOT_TPREL16_HA"},
    {91,2,"R_PPC_GOT_DTPREL16"},
    {92,2,"R_PPC_GOT_DTPREL16_LO"},
    {93,2,"R_PPC_GOT_DTPREL16_HI"},
    {94,2,"R_PPC_GOT_DTPREL16_HA"},
    {95,4,"R_PPC_TLSGD"},
    {96,4,"R_PPC_TLSLD"},
    {101,4,"R_PPC_EMB_NADDR32"},
    {102,2,"R_PPC_EMB_NADDR16"},
    {103,2,"R_PPC_EMB_NADDR16_LO"},
    {104,2,"R_PPC_EMB_NADDR16_HI"},
    {105,2,"R_PPC_EMB_NADDR16_HA"},
    {106,2,"R_PPC_EMB_SDAI16"},
    {107,2,"R_PPC_EMB_SDA2I16"},
    {108,2,"R_PPC_EMB_SDA2REL"},
    {109,4,"R_PPC_EMB_SDA21"},
    {110,0,"R_PPC_EMB_MRKREF"},
    {111,2,"R_PPC_EMB_RELSEC16"},
    {112,0,"R_PPC_EMB_RELST_LO"},
    {113,0,"R_PPC_EMB_RELST_HI"},
    {114,0,"R_PPC_EMB_RELST_HA"},
    {115,0,"R_PPC_EMB_BIT_FLD"},
    {116,2,"R_PPC_EMB_RELSDA"},
    {119,4,"R_PPC_PLTSEQ"},
    {120,4,"R_PPC_PLTCALL"},
    {216,2,"R_PPC_VLE_REL8"},
    {217,4,"R_PPC_VLE_REL15"},
    {218,4,"R_PPC_VLE_REL24"},
    {219,4,"R_PPC_VLE_LO16A"},
    {220,4,"R_PPC_VLE_LO16D"},
    {221,4,"R_PPC_VLE_HI16A"},
    {222,4,"R_PPC_VLE_HI16D"},
    {223,4,"R_PPC_VLE_HA16A"},
    {224,4,"R_PPC_VLE_HA16D"},
    {225,4,"R_PPC_VLE_SDA21"},
    {226,4,"R_PPC_VLE_SDA21_LO"},
    {227,4,"R_PPC_VLE_SDAREL_LO16A"},
    {228,4,"R_PPC_VLE_SDAREL_LO16D"},
    {229,4,"R_PPC_VLE_SDAREL_HI16A"},
    {230,4,"R_PPC_VLE_SDAREL_HI16D"},
    {231,4,"R_PPC_VLE_SDAREL_HA16A"},
    {232,4,"R_PPC_VLE_SDAREL_HA16D"},
    {233,4,"R_PPC_VLE_ADDR20"},
    {246,4,"R_PPC_REL16DX_HA"},
    {248,4,"R_PPC_IRELATIVE"},
    {249,2,"R_PPC_REL16"},
    {250,2,"R_PPC_REL16_LO"},
    {251,2,"R_PPC_REL16_HI"},
    {252,2,"R_PPC_REL16_HA"},
    {253,0,"R_PPC_GNU_VTINHERIT"},
    {254,0,"R_PPC_GNU_VTENTRY"},
    {255,2,"R_PPC_TOC16"},
};

const char *helf_ppc_relocation_type_name_get(uint32_t type)
{
    return helf_arch_relocation_type_name_get(helf_ppc_relocation_type_names,sizeof(helf_ppc_relocation_type_names)/sizeof(helf_ppc_relocation_type_names[0]),type);
}

uint8_t helf_ppc_relocation_size_get(uint32_t type)
{
    return helf_arch_relocation_type_size_get(helf_ppc_relocation_type_names,sizeof(helf_ppc_relocation_type_names)/sizeof(helf_ppc_relocation_type_names[0]),type);
}

static const helf_arch_section_type_name_t helf_ppc_section_type_names[]=
{
    {2147483647,"SHT_ORDERED"},
};

const char *helf_ppc_section_type_name_get(uint32_t type)
{
    return helf_arch_section_type_name_get(helf_ppc_section_type_names,sizeof(helf_ppc_section_type_names)/sizeof(helf_ppc_section_type_names[0]),type);
}

bool helf_ppc_relocation_address_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_relocation_address_info_t *info)
{
    if(!helf_ppc_is_ppc(input_file) || relocation==NULL)
    {
        return false;
    }
    uint8_t size=helf_ppc_relocation_size_get(relocation->type);
    if(size==0)
    {
        return false;
    }
    return helf_arch_relocation_address_info_get(input_file,relocation,size,info);
}

size_t helf_ppc_relocation_data_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,void *buffer,size_t buffer_length)
{
    if(!helf_ppc_is_ppc(input_file) || relocation==NULL)
    {
        return 0;
    }
    uint8_t size=helf_ppc_relocation_size_get(relocation->type);
    if(size==0)
    {
        return 0;
    }
    return helf_arch_relocation_data_get(input_file,relocation,size,buffer,buffer_length);
}

bool helf_ppc_is_shared_object(helf_file_input_t *input_file)
{
    return helf_arch_is_shared_object(input_file,helf_ppc_machines,sizeof(helf_ppc_machines)/sizeof(helf_ppc_machines[0]),HELF_ELFCLASS_32);
}

bool helf_ppc_is_relocatable(helf_file_input_t *input_file)
{
    return helf_arch_is_relocatable(input_file,helf_ppc_machines,sizeof(helf_ppc_machines)/sizeof(helf_ppc_machines[0]),HELF_ELFCLASS_32);
}

size_t helf_ppc_dynamic_count_get(helf_file_input_t *input_file)
{
    if(!helf_ppc_is_ppc(input_file))
    {
        return 0;
    }
    return helf_arch_dynamic_count_get(input_file);
}

bool helf_ppc_dynamic_info_get(helf_file_input_t *input_file,size_t index,helf_arch_dynamic_info_t *info)
{
    if(!helf_ppc_is_ppc(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_info_get(input_file,index,info);
}

bool helf_ppc_dynamic_string_get(helf_file_input_t *input_file,uint64_t offset,char *buffer,size_t buffer_length)
{
    if(!helf_ppc_is_ppc(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_string_get(input_file,offset,buffer,buffer_length);
}

/* helf_relocation_howto_begin */
static const helf_arch_relocation_howto_t helf_ppc_reloc_howto[]=
{
    {0x00000000u,0,0,0,0,0x0040,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000001u,4,32,0,0,0x0000,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000002u,4,26,0,0,0x0000,0x0000000000000000ULL,0x0000000003FFFFFCULL},
    {0x00000003u,2,16,0,0,0x0000,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000004u,2,16,0,0,0x0000,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000005u,2,16,16,0,0x0000,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000006u,2,16,16,0,0x0010,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000007u,4,16,0,0,0x0000,0x0000000000000000ULL,0x000000000000FFFCULL},
    {0x00000008u,4,16,0,0,0x0000,0x0000000000000000ULL,0x000000000000FFFCULL},
    {0x00000009u,4,16,0,0,0x0000,0x0000000000000000ULL,0x000000000000FFFCULL},
    {0x0000000Au,4,26,0,0,0x0003,0x0000000000000000ULL,0x0000000003FFFFFCULL},
    {0x0000000Bu,4,16,0,0,0x0003,0x0000000000000000ULL,0x000000000000FFFCULL},
    {0x0000000Cu,4,16,0,0,0x0003,0x0000000000000000ULL,0x000000000000FFFCULL},
    {0x0000000Du,4,16,0,0,0x0003,0x0000000000000000ULL,0x000000000000FFFCULL},
    {0x0000000Eu,2,16,0,0,0x0210,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000000Fu,2,16,0,0,0x0210,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000010u,2,16,16,0,0x0210,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000011u,2,16,16,0,0x0210,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000012u,4,26,0,0,0x0013,0x0000000000000000ULL,0x0000000003FFFFFCULL},
    {0x00000013u,4,32,0,0,0x0190,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000014u,4,32,0,0,0x0110,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000015u,4,32,0,0,0x0150,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000016u,4,32,0,0,0x0080,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000017u,4,26,0,0,0x0003,0x0000000000000000ULL,0x0000000003FFFFFCULL},
    {0x00000018u,4,32,0,0,0x0000,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000019u,2,16,0,0,0x0000,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000001Au,4,32,0,0,0x0003,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x0000001Bu,4,32,0,0,0x0010,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Cu,4,32,0,0,0x0013,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Du,2,16,0,0,0x0010,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000001Eu,2,16,16,0,0x0010,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000001Fu,2,16,16,0,0x0010,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000020u,2,16,0,0,0x0010,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000021u,2,16,0,0,0x0010,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000022u,2,16,0,0,0x0010,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000023u,2,16,16,0,0x0010,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000024u,2,16,16,0,0x0010,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000025u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000030u,0,0,0,0,0x0040,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000031u,0,0,0,0,0x0040,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000032u,0,0,0,0,0x0040,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000033u,4,16,16,0,0x0010,0x0000000000000000ULL,0x00000000001FFFC1ULL},
    {0x00000043u,4,32,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000044u,4,32,0,0,0x01D0,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000045u,2,16,0,0,0x01D0,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000046u,2,16,0,0,0x01D0,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000047u,2,16,16,0,0x01D0,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000048u,2,16,16,0,0x01D0,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000049u,4,32,0,0,0x01D0,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x0000004Au,2,16,0,0,0x01D0,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000004Bu,2,16,0,0,0x01D0,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000004Cu,2,16,16,0,0x01D0,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000004Du,2,16,16,0,0x01D0,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000004Eu,4,32,0,0,0x01D0,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x0000004Fu,2,16,0,0,0x03D0,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000050u,2,16,0,0,0x03D0,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000051u,2,16,16,0,0x03D0,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000052u,2,16,16,0,0x03D0,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000053u,2,16,0,0,0x03D0,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000054u,2,16,0,0,0x03D0,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000055u,2,16,16,0,0x03D0,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000056u,2,16,16,0,0x03D0,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000057u,2,16,0,0,0x03D0,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000058u,2,16,0,0,0x03D0,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000059u,2,16,16,0,0x03D0,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000005Au,2,16,16,0,0x03D0,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000005Bu,2,16,0,0,0x03D0,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000005Cu,2,16,0,0,0x03D0,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000005Du,2,16,16,0,0x03D0,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000005Eu,2,16,16,0,0x03D0,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000005Fu,4,32,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000060u,4,32,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000065u,4,32,0,0,0x0010,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000066u,2,16,0,0,0x0010,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000067u,2,16,0,0,0x0010,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000068u,2,16,16,0,0x0010,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000069u,2,16,16,0,0x0010,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000006Au,2,16,0,0,0x0010,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000006Bu,2,16,0,0,0x0010,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000006Cu,2,16,0,0,0x0010,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000006Du,4,16,0,0,0x0010,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000006Eu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000006Fu,2,16,0,0,0x0000,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000070u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000071u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000072u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000073u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000074u,2,16,0,0,0x0010,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000077u,4,32,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000078u,4,32,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000D8u,2,8,1,0,0x0003,0x0000000000000000ULL,0x00000000000000FFULL},
    {0x000000D9u,4,16,0,0,0x0003,0x0000000000000000ULL,0x000000000000FFFEULL},
    {0x000000DAu,4,25,0,0,0x0003,0x0000000000000000ULL,0x0000000001FFFFFEULL},
    {0x000000DBu,4,16,0,0,0x0010,0x0000000000000000ULL,0x00000000001F07FFULL},
    {0x000000DCu,4,16,0,0,0x0010,0x0000000000000000ULL,0x0000000003E007FFULL},
    {0x000000DDu,4,16,16,0,0x0010,0x0000000000000000ULL,0x00000000001F07FFULL},
    {0x000000DEu,4,16,16,0,0x0010,0x0000000000000000ULL,0x0000000003E007FFULL},
    {0x000000DFu,4,16,16,0,0x0010,0x0000000000000000ULL,0x00000000001F07FFULL},
    {0x000000E0u,4,16,16,0,0x0010,0x0000000000000000ULL,0x0000000003E007FFULL},
    {0x000000E1u,4,16,0,0,0x0010,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x000000E2u,4,16,0,0,0x0010,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x000000E3u,4,16,0,0,0x0010,0x0000000000000000ULL,0x00000000001F07FFULL},
    {0x000000E4u,4,16,0,0,0x0010,0x0000000000000000ULL,0x0000000003E007FFULL},
    {0x000000E5u,4,16,16,0,0x0010,0x0000000000000000ULL,0x00000000001F07FFULL},
    {0x000000E6u,4,16,16,0,0x0010,0x0000000000000000ULL,0x0000000003E007FFULL},
    {0x000000E7u,4,16,16,0,0x0010,0x0000000000000000ULL,0x00000000001F07FFULL},
    {0x000000E8u,4,16,16,0,0x0010,0x0000000000000000ULL,0x0000000003E007FFULL},
    {0x000000E9u,4,20,0,0,0x0010,0x0000000000000000ULL,0x00000000001F7FFFULL},
    {0x000000F6u,4,16,16,0,0x0013,0x0000000000000000ULL,0x00000000001FFFC1ULL},
    {0x000000F8u,4,32,0,0,0x0090,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x000000F9u,2,16,0,0,0x0003,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x000000FAu,2,16,0,0,0x0003,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x000000FBu,2,16,16,0,0x0003,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x000000FCu,2,16,16,0,0x0013,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x000000FDu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000FEu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000FFu,2,16,0,0,0x0010,0x0000000000000000ULL,0x000000000000FFFFULL},
};

bool helf_ppc_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto)
{
    if(howto==NULL)
    {
        return false;
    }
    size_t count=sizeof(helf_ppc_reloc_howto)/sizeof(helf_ppc_reloc_howto[0]);
    if(count==0)
    {
        return false;
    }
    for(size_t i=0;i<count;i++)
    {
        if(helf_ppc_reloc_howto[i].type==type)
        {
            *howto=helf_ppc_reloc_howto[i];
            return true;
        }
    }
    return false;
}
/* helf_relocation_howto_end */


