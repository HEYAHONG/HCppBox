/***************************************************************
 * Name:      helf_arch_kvx.c
 * Purpose:   实现helf_arch_kvx接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "helf_arch_kvx.h"

static const uint16_t helf_kvx_machines[]=
{
    HELF_EM_KVX,
};

bool helf_kvx_is_kvx(helf_file_input_t *input_file)
{
    return helf_arch_is(input_file,helf_kvx_machines,sizeof(helf_kvx_machines)/sizeof(helf_kvx_machines[0]),HELF_ELFCLASS_64);
}

bool helf_kvx_is_big_endian(helf_file_input_t *input_file)
{
    if(!helf_kvx_is_kvx(input_file))
    {
        return false;
    }
    return helf_file_input_is_big_endian(input_file);
}

bool helf_kvx_is_executable(helf_file_input_t *input_file)
{
    return helf_arch_is_executable(input_file,helf_kvx_machines,sizeof(helf_kvx_machines)/sizeof(helf_kvx_machines[0]),HELF_ELFCLASS_64);
}

bool helf_kvx_entry_get(helf_file_input_t *input_file,uint64_t *entry)
{
    if(entry==NULL)
    {
        return false;
    }
    if(!helf_kvx_is_kvx(input_file))
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

size_t helf_kvx_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_kvx_is_kvx(input_file))
    {
        return 0;
    }
    return helf_arch_section_count_get(input_file);
}

bool helf_kvx_section_info_get(helf_file_input_t *input_file,size_t index,helf_kvx_section_info_t *info)
{
    if(!helf_kvx_is_kvx(input_file))
    {
        return false;
    }
    return helf_arch_section_info_get(input_file,index,info);
}

bool helf_kvx_is_rx_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_kvx_is_kvx(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_section(index,input_file);
}

intptr_t helf_kvx_rx_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_kvx_is_rx_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rx_section_offset_get(index,input_file);
}

size_t helf_kvx_rx_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_kvx_is_kvx(input_file))
    {
        return 0;
    }
    return helf_arch_rx_section_size_get(input_file);
}

bool helf_kvx_is_ro_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_kvx_is_kvx(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_section(index,input_file);
}

intptr_t helf_kvx_ro_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_kvx_is_ro_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_ro_section_offset_get(index,input_file);
}

size_t helf_kvx_ro_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_kvx_is_kvx(input_file))
    {
        return 0;
    }
    return helf_arch_ro_section_size_get(input_file);
}

bool helf_kvx_is_rw_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_kvx_is_kvx(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_section(index,input_file);
}

intptr_t helf_kvx_rw_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_kvx_is_rw_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rw_section_offset_get(index,input_file);
}

size_t helf_kvx_rw_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_kvx_is_kvx(input_file))
    {
        return 0;
    }
    return helf_arch_rw_section_size_get(input_file);
}

bool helf_kvx_is_bss_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_kvx_is_kvx(input_file))
    {
        return false;
    }
    return helf_arch_is_bss_section(index,input_file);
}

size_t helf_kvx_program_header_count_get(helf_file_input_t *input_file)
{
    if(!helf_kvx_is_kvx(input_file))
    {
        return 0;
    }
    return helf_arch_program_header_count_get(input_file);
}

bool helf_kvx_program_header_get(helf_file_input_t *input_file,size_t index,helf_elf64_program_header_t *programheader)
{
    if(programheader==NULL)
    {
        return false;
    }
    if(!helf_kvx_is_kvx(input_file))
    {
        return false;
    }
    if(index>=helf_arch_program_header_count_get(input_file))
    {
        return false;
    }
    return helf_file_input_64_bits_program_header_get(input_file,index,programheader);
}

size_t helf_kvx_segment_count_get(helf_file_input_t *input_file)
{
    return helf_kvx_program_header_count_get(input_file);
}

bool helf_kvx_segment_info_get(helf_file_input_t *input_file,size_t index,helf_kvx_segment_info_t *info)
{
    if(!helf_kvx_is_kvx(input_file))
    {
        return false;
    }
    return helf_arch_segment_info_get(input_file,index,info);
}

bool helf_kvx_is_load_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_kvx_is_kvx(input_file))
    {
        return false;
    }
    return helf_arch_is_load_segment(index,input_file);
}

bool helf_kvx_is_rx_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_kvx_is_kvx(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_segment(index,input_file);
}

bool helf_kvx_is_ro_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_kvx_is_kvx(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_segment(index,input_file);
}

bool helf_kvx_is_rw_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_kvx_is_kvx(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_segment(index,input_file);
}

size_t helf_kvx_load_segment_count_get(helf_file_input_t *input_file)
{
    if(!helf_kvx_is_kvx(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_count_get(input_file);
}

bool helf_kvx_load_segment_info_get(helf_file_input_t *input_file,size_t index,helf_kvx_segment_info_t *info)
{
    if(!helf_kvx_is_kvx(input_file))
    {
        return false;
    }
    return helf_arch_load_segment_info_get(input_file,index,info);
}

size_t helf_kvx_load_segment_total_filesz_get(helf_file_input_t *input_file)
{
    if(!helf_kvx_is_kvx(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_filesz_get(input_file);
}

size_t helf_kvx_load_segment_total_memsz_get(helf_file_input_t *input_file)
{
    if(!helf_kvx_is_kvx(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_memsz_get(input_file);
}

size_t helf_kvx_relocation_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_kvx_is_kvx(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_section_count_get(input_file);
}

bool helf_kvx_relocation_section_info_get(helf_file_input_t *input_file,size_t index,helf_arch_relocation_section_info_t *info)
{
    if(!helf_kvx_is_kvx(input_file))
    {
        return false;
    }
    return helf_arch_relocation_section_info_get(input_file,index,info);
}

size_t helf_kvx_relocation_count_get(helf_file_input_t *input_file,size_t section_index)
{
    if(!helf_kvx_is_kvx(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_count_get(input_file,section_index);
}

bool helf_kvx_relocation_get(helf_file_input_t *input_file,size_t section_index,size_t index,helf_arch_relocation_info_t *info)
{
    if(!helf_kvx_is_kvx(input_file))
    {
        return false;
    }
    if(!helf_arch_relocation_get(input_file,section_index,index,info))
    {
        return false;
    }
    return true;
}

bool helf_kvx_relocation_symbol_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_symbol_info_t *info)
{
    if(!helf_kvx_is_kvx(input_file))
    {
        return false;
    }
    return helf_arch_relocation_symbol_info_get(input_file,relocation,info);
}

static const helf_arch_relocation_type_name_t helf_kvx_relocation_type_names[]=
{
    {0,0,"R_KVX_NONE"},
    {1,2,"R_KVX_16"},
    {2,4,"R_KVX_32"},
    {3,8,"R_KVX_64"},
    {4,0,"R_KVX_S16_PCREL"},
    {5,0,"R_KVX_PCREL17"},
    {6,0,"R_KVX_PCREL27"},
    {7,0,"R_KVX_32_PCREL"},
    {8,0,"R_KVX_S37_PCREL_LO10"},
    {9,0,"R_KVX_S37_PCREL_UP27"},
    {10,0,"R_KVX_S43_PCREL_LO10"},
    {11,0,"R_KVX_S43_PCREL_UP27"},
    {12,0,"R_KVX_S43_PCREL_EX6"},
    {13,0,"R_KVX_S64_PCREL_LO10"},
    {14,0,"R_KVX_S64_PCREL_UP27"},
    {15,0,"R_KVX_S64_PCREL_EX27"},
    {16,0,"R_KVX_64_PCREL"},
    {17,2,"R_KVX_S16"},
    {18,0,"R_KVX_S32_LO5"},
    {19,0,"R_KVX_S32_UP27"},
    {20,0,"R_KVX_S37_LO10"},
    {21,0,"R_KVX_S37_UP27"},
    {22,0,"R_KVX_S37_GOTOFF_LO10"},
    {23,0,"R_KVX_S37_GOTOFF_UP27"},
    {24,0,"R_KVX_S43_GOTOFF_LO10"},
    {25,0,"R_KVX_S43_GOTOFF_UP27"},
    {26,0,"R_KVX_S43_GOTOFF_EX6"},
    {27,0,"R_KVX_32_GOTOFF"},
    {28,0,"R_KVX_64_GOTOFF"},
    {29,0,"R_KVX_32_GOT"},
    {30,0,"R_KVX_S37_GOT_LO10"},
    {31,0,"R_KVX_S37_GOT_UP27"},
    {32,0,"R_KVX_S43_GOT_LO10"},
    {33,0,"R_KVX_S43_GOT_UP27"},
    {34,0,"R_KVX_S43_GOT_EX6"},
    {35,0,"R_KVX_64_GOT"},
    {36,0,"R_KVX_GLOB_DAT"},
    {37,0,"R_KVX_COPY"},
    {38,0,"R_KVX_JMP_SLOT"},
    {39,0,"R_KVX_RELATIVE"},
    {40,0,"R_KVX_S43_LO10"},
    {41,0,"R_KVX_S43_UP27"},
    {42,0,"R_KVX_S43_EX6"},
    {43,0,"R_KVX_S64_LO10"},
    {44,0,"R_KVX_S64_UP27"},
    {45,0,"R_KVX_S64_EX27"},
    {46,0,"R_KVX_S37_GOTADDR_LO10"},
    {47,0,"R_KVX_S37_GOTADDR_UP27"},
    {48,0,"R_KVX_S43_GOTADDR_LO10"},
    {49,0,"R_KVX_S43_GOTADDR_UP27"},
    {50,0,"R_KVX_S43_GOTADDR_EX6"},
    {51,0,"R_KVX_S64_GOTADDR_LO10"},
    {52,0,"R_KVX_S64_GOTADDR_UP27"},
    {53,0,"R_KVX_S64_GOTADDR_EX27"},
    {54,0,"R_KVX_64_DTPMOD"},
    {55,0,"R_KVX_64_DTPOFF"},
    {56,0,"R_KVX_S37_TLS_DTPOFF_LO10"},
    {57,0,"R_KVX_S37_TLS_DTPOFF_UP27"},
    {58,0,"R_KVX_S43_TLS_DTPOFF_LO10"},
    {59,0,"R_KVX_S43_TLS_DTPOFF_UP27"},
    {60,0,"R_KVX_S43_TLS_DTPOFF_EX6"},
    {61,0,"R_KVX_S37_TLS_GD_LO10"},
    {62,0,"R_KVX_S37_TLS_GD_UP27"},
    {63,0,"R_KVX_S43_TLS_GD_LO10"},
    {64,0,"R_KVX_S43_TLS_GD_UP27"},
    {65,0,"R_KVX_S43_TLS_GD_EX6"},
    {66,0,"R_KVX_S37_TLS_LD_LO10"},
    {67,0,"R_KVX_S37_TLS_LD_UP27"},
    {68,0,"R_KVX_S43_TLS_LD_LO10"},
    {69,0,"R_KVX_S43_TLS_LD_UP27"},
    {70,0,"R_KVX_S43_TLS_LD_EX6"},
    {71,0,"R_KVX_64_TPOFF"},
    {72,0,"R_KVX_S37_TLS_IE_LO10"},
    {73,0,"R_KVX_S37_TLS_IE_UP27"},
    {74,0,"R_KVX_S43_TLS_IE_LO10"},
    {75,0,"R_KVX_S43_TLS_IE_UP27"},
    {76,0,"R_KVX_S43_TLS_IE_EX6"},
    {77,0,"R_KVX_S37_TLS_LE_LO10"},
    {78,0,"R_KVX_S37_TLS_LE_UP27"},
    {79,0,"R_KVX_S43_TLS_LE_LO10"},
    {80,0,"R_KVX_S43_TLS_LE_UP27"},
    {81,0,"R_KVX_S43_TLS_LE_EX6"},
    {82,1,"R_KVX_8"},
};

const char *helf_kvx_relocation_type_name_get(uint32_t type)
{
    return helf_arch_relocation_type_name_get(helf_kvx_relocation_type_names,sizeof(helf_kvx_relocation_type_names)/sizeof(helf_kvx_relocation_type_names[0]),type);
}

uint8_t helf_kvx_relocation_size_get(uint32_t type)
{
    return helf_arch_relocation_type_size_get(helf_kvx_relocation_type_names,sizeof(helf_kvx_relocation_type_names)/sizeof(helf_kvx_relocation_type_names[0]),type);
}

const char *helf_kvx_section_type_name_get(uint32_t type)
{
    (void)type;
    return NULL;
}

bool helf_kvx_relocation_address_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_relocation_address_info_t *info)
{
    if(!helf_kvx_is_kvx(input_file) || relocation==NULL)
    {
        return false;
    }
    uint8_t size=helf_kvx_relocation_size_get(relocation->type);
    if(size==0)
    {
        return false;
    }
    return helf_arch_relocation_address_info_get(input_file,relocation,size,info);
}

size_t helf_kvx_relocation_data_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,void *buffer,size_t buffer_length)
{
    if(!helf_kvx_is_kvx(input_file) || relocation==NULL)
    {
        return 0;
    }
    uint8_t size=helf_kvx_relocation_size_get(relocation->type);
    if(size==0)
    {
        return 0;
    }
    return helf_arch_relocation_data_get(input_file,relocation,size,buffer,buffer_length);
}

bool helf_kvx_is_shared_object(helf_file_input_t *input_file)
{
    return helf_arch_is_shared_object(input_file,helf_kvx_machines,sizeof(helf_kvx_machines)/sizeof(helf_kvx_machines[0]),HELF_ELFCLASS_64);
}

bool helf_kvx_is_relocatable(helf_file_input_t *input_file)
{
    return helf_arch_is_relocatable(input_file,helf_kvx_machines,sizeof(helf_kvx_machines)/sizeof(helf_kvx_machines[0]),HELF_ELFCLASS_64);
}

size_t helf_kvx_dynamic_count_get(helf_file_input_t *input_file)
{
    if(!helf_kvx_is_kvx(input_file))
    {
        return 0;
    }
    return helf_arch_dynamic_count_get(input_file);
}

bool helf_kvx_dynamic_info_get(helf_file_input_t *input_file,size_t index,helf_arch_dynamic_info_t *info)
{
    if(!helf_kvx_is_kvx(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_info_get(input_file,index,info);
}

bool helf_kvx_dynamic_string_get(helf_file_input_t *input_file,uint64_t offset,char *buffer,size_t buffer_length)
{
    if(!helf_kvx_is_kvx(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_string_get(input_file,offset,buffer,buffer_length);
}

/* helf_relocation_howto_begin */
static const helf_arch_relocation_howto_t helf_kvx_reloc_howto[]=
{
    {0x00000000u,0,32,0,0,0x0040,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000001u,2,16,0,0,0x0000,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000002u,4,32,0,0,0x0000,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000003u,8,64,0,0,0x0000,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000004u,2,16,0,0,0x0003,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000005u,3,17,2,6,0x0003,0x0000000000000000ULL,0x00000000007FFFC0ULL},
    {0x00000006u,4,27,2,0,0x0003,0x0000000000000000ULL,0x0000000007FFFFFFULL},
    {0x00000007u,4,32,0,0,0x0003,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000008u,4,10,0,6,0x0003,0x0000000000000000ULL,0x000000000000FFC0ULL},
    {0x00000009u,4,27,10,0,0x0003,0x0000000000000000ULL,0x0000000007FFFFFFULL},
    {0x0000000Au,4,10,0,6,0x0003,0x0000000000000000ULL,0x000000000000FFC0ULL},
    {0x0000000Bu,4,27,10,0,0x0003,0x0000000000000000ULL,0x0000000007FFFFFFULL},
    {0x0000000Cu,4,6,37,0,0x0003,0x0000000000000000ULL,0x000000000000003FULL},
    {0x0000000Du,4,10,0,6,0x0003,0x0000000000000000ULL,0x000000000000FFC0ULL},
    {0x0000000Eu,4,27,10,0,0x0003,0x0000000000000000ULL,0x0000000007FFFFFFULL},
    {0x0000000Fu,4,27,37,0,0x0003,0x0000000000000000ULL,0x0000000007FFFFFFULL},
    {0x00000010u,8,64,0,0,0x0003,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000011u,2,16,0,0,0x0000,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000012u,4,5,0,6,0x0000,0x0000000000000000ULL,0x00000000000007C0ULL},
    {0x00000013u,4,27,5,0,0x0000,0x0000000000000000ULL,0x0000000007FFFFFFULL},
    {0x00000014u,4,10,0,6,0x0000,0x0000000000000000ULL,0x000000000000FFC0ULL},
    {0x00000015u,4,27,10,0,0x0000,0x0000000000000000ULL,0x0000000007FFFFFFULL},
    {0x00000016u,4,10,0,6,0x0200,0x0000000000000000ULL,0x000000000000FFC0ULL},
    {0x00000017u,4,27,10,0,0x0200,0x0000000000000000ULL,0x0000000007FFFFFFULL},
    {0x00000018u,4,10,0,6,0x0200,0x0000000000000000ULL,0x000000000000FFC0ULL},
    {0x00000019u,4,27,10,0,0x0200,0x0000000000000000ULL,0x0000000007FFFFFFULL},
    {0x0000001Au,4,6,37,0,0x0200,0x0000000000000000ULL,0x000000000000003FULL},
    {0x0000001Bu,4,32,0,0,0x0200,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x0000001Cu,8,64,0,0,0x0200,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x0000001Du,4,32,0,0,0x0200,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x0000001Eu,4,10,0,6,0x0200,0x0000000000000000ULL,0x000000000000FFC0ULL},
    {0x0000001Fu,4,27,10,0,0x0200,0x0000000000000000ULL,0x0000000007FFFFFFULL},
    {0x00000020u,4,10,0,6,0x0200,0x0000000000000000ULL,0x000000000000FFC0ULL},
    {0x00000021u,4,27,10,0,0x0200,0x0000000000000000ULL,0x0000000007FFFFFFULL},
    {0x00000022u,4,6,37,0,0x0200,0x0000000000000000ULL,0x000000000000003FULL},
    {0x00000023u,8,64,0,0,0x0200,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000024u,4,32,0,0,0x0100,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000025u,4,32,0,0,0x0180,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000026u,4,32,0,0,0x0140,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000027u,4,32,0,0,0x0080,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000028u,4,10,0,6,0x0000,0x0000000000000000ULL,0x000000000000FFC0ULL},
    {0x00000029u,4,27,10,0,0x0000,0x0000000000000000ULL,0x0000000007FFFFFFULL},
    {0x0000002Au,4,6,37,0,0x0000,0x0000000000000000ULL,0x000000000000003FULL},
    {0x0000002Bu,4,10,0,6,0x0000,0x0000000000000000ULL,0x000000000000FFC0ULL},
    {0x0000002Cu,4,27,10,0,0x0000,0x0000000000000000ULL,0x0000000007FFFFFFULL},
    {0x0000002Du,4,27,37,0,0x0000,0x0000000000000000ULL,0x0000000007FFFFFFULL},
    {0x0000002Eu,4,10,0,6,0x0203,0x0000000000000000ULL,0x000000000000FFC0ULL},
    {0x0000002Fu,4,27,10,0,0x0203,0x0000000000000000ULL,0x0000000007FFFFFFULL},
    {0x00000030u,4,10,0,6,0x0203,0x0000000000000000ULL,0x000000000000FFC0ULL},
    {0x00000031u,4,27,10,0,0x0203,0x0000000000000000ULL,0x0000000007FFFFFFULL},
    {0x00000032u,4,6,37,0,0x0203,0x0000000000000000ULL,0x000000000000003FULL},
    {0x00000033u,4,10,0,6,0x0203,0x0000000000000000ULL,0x000000000000FFC0ULL},
    {0x00000034u,4,27,10,0,0x0203,0x0000000000000000ULL,0x0000000007FFFFFFULL},
    {0x00000035u,4,27,37,0,0x0203,0x0000000000000000ULL,0x0000000007FFFFFFULL},
    {0x00000036u,8,64,0,0,0x01C0,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000037u,8,64,0,0,0x01C0,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000038u,4,10,0,6,0x01C0,0x0000000000000000ULL,0x000000000000FFC0ULL},
    {0x00000039u,4,27,10,0,0x01C0,0x0000000000000000ULL,0x0000000007FFFFFFULL},
    {0x0000003Au,4,10,0,6,0x01C0,0x0000000000000000ULL,0x000000000000FFC0ULL},
    {0x0000003Bu,4,27,10,0,0x01C0,0x0000000000000000ULL,0x0000000007FFFFFFULL},
    {0x0000003Cu,4,6,37,0,0x01C0,0x0000000000000000ULL,0x000000000000003FULL},
    {0x0000003Du,4,10,0,6,0x01C0,0x0000000000000000ULL,0x000000000000FFC0ULL},
    {0x0000003Eu,4,27,10,0,0x01C0,0x0000000000000000ULL,0x0000000007FFFFFFULL},
    {0x0000003Fu,4,10,0,6,0x01C0,0x0000000000000000ULL,0x000000000000FFC0ULL},
    {0x00000040u,4,27,10,0,0x01C0,0x0000000000000000ULL,0x0000000007FFFFFFULL},
    {0x00000041u,4,6,37,0,0x01C0,0x0000000000000000ULL,0x000000000000003FULL},
    {0x00000042u,4,10,0,6,0x01C0,0x0000000000000000ULL,0x000000000000FFC0ULL},
    {0x00000043u,4,27,10,0,0x01C0,0x0000000000000000ULL,0x0000000007FFFFFFULL},
    {0x00000044u,4,10,0,6,0x01C0,0x0000000000000000ULL,0x000000000000FFC0ULL},
    {0x00000045u,4,27,10,0,0x01C0,0x0000000000000000ULL,0x0000000007FFFFFFULL},
    {0x00000046u,4,6,37,0,0x01C0,0x0000000000000000ULL,0x000000000000003FULL},
    {0x00000047u,8,64,0,0,0x01C0,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000048u,4,10,0,6,0x01C0,0x0000000000000000ULL,0x000000000000FFC0ULL},
    {0x00000049u,4,27,10,0,0x01C0,0x0000000000000000ULL,0x0000000007FFFFFFULL},
    {0x0000004Au,4,10,0,6,0x01C0,0x0000000000000000ULL,0x000000000000FFC0ULL},
    {0x0000004Bu,4,27,10,0,0x01C0,0x0000000000000000ULL,0x0000000007FFFFFFULL},
    {0x0000004Cu,4,6,37,0,0x01C0,0x0000000000000000ULL,0x000000000000003FULL},
    {0x0000004Du,4,10,0,6,0x01C0,0x0000000000000000ULL,0x000000000000FFC0ULL},
    {0x0000004Eu,4,27,10,0,0x01C0,0x0000000000000000ULL,0x0000000007FFFFFFULL},
    {0x0000004Fu,4,10,0,6,0x01C0,0x0000000000000000ULL,0x000000000000FFC0ULL},
    {0x00000050u,4,27,10,0,0x01C0,0x0000000000000000ULL,0x0000000007FFFFFFULL},
    {0x00000051u,4,6,37,0,0x01C0,0x0000000000000000ULL,0x000000000000003FULL},
    {0x00000052u,1,8,0,0,0x0000,0x0000000000000000ULL,0x00000000000000FFULL},
};

bool helf_kvx_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto)
{
    if(howto==NULL)
    {
        return false;
    }
    size_t count=sizeof(helf_kvx_reloc_howto)/sizeof(helf_kvx_reloc_howto[0]);
    if(count==0)
    {
        return false;
    }
    for(size_t i=0;i<count;i++)
    {
        if(helf_kvx_reloc_howto[i].type==type)
        {
            *howto=helf_kvx_reloc_howto[i];
            return true;
        }
    }
    return false;
}
/* helf_relocation_howto_end */

