/***************************************************************
 * Name:      helf_arch_xtensa.c
 * Purpose:   实现helf_arch_xtensa接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "helf_arch_xtensa.h"

static const uint16_t helf_xtensa_machines[]=
{
    HELF_EM_XTENSA,
    HELF_EM_XTENSA_OLD,
};

bool helf_xtensa_is_xtensa(helf_file_input_t *input_file)
{
    return helf_arch_is(input_file,helf_xtensa_machines,sizeof(helf_xtensa_machines)/sizeof(helf_xtensa_machines[0]),HELF_ELFCLASS_32);
}

bool helf_xtensa_is_big_endian(helf_file_input_t *input_file)
{
    if(!helf_xtensa_is_xtensa(input_file))
    {
        return false;
    }
    return helf_file_input_is_big_endian(input_file);
}

bool helf_xtensa_is_executable(helf_file_input_t *input_file)
{
    return helf_arch_is_executable(input_file,helf_xtensa_machines,sizeof(helf_xtensa_machines)/sizeof(helf_xtensa_machines[0]),HELF_ELFCLASS_32);
}

bool helf_xtensa_entry_get(helf_file_input_t *input_file,uint32_t *entry)
{
    if(entry==NULL)
    {
        return false;
    }
    if(!helf_xtensa_is_xtensa(input_file))
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

size_t helf_xtensa_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_xtensa_is_xtensa(input_file))
    {
        return 0;
    }
    return helf_arch_section_count_get(input_file);
}

bool helf_xtensa_section_info_get(helf_file_input_t *input_file,size_t index,helf_xtensa_section_info_t *info)
{
    if(!helf_xtensa_is_xtensa(input_file))
    {
        return false;
    }
    return helf_arch_section_info_get(input_file,index,info);
}

bool helf_xtensa_is_rx_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_xtensa_is_xtensa(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_section(index,input_file);
}

intptr_t helf_xtensa_rx_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_xtensa_is_rx_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rx_section_offset_get(index,input_file);
}

size_t helf_xtensa_rx_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_xtensa_is_xtensa(input_file))
    {
        return 0;
    }
    return helf_arch_rx_section_size_get(input_file);
}

bool helf_xtensa_is_ro_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_xtensa_is_xtensa(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_section(index,input_file);
}

intptr_t helf_xtensa_ro_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_xtensa_is_ro_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_ro_section_offset_get(index,input_file);
}

size_t helf_xtensa_ro_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_xtensa_is_xtensa(input_file))
    {
        return 0;
    }
    return helf_arch_ro_section_size_get(input_file);
}

bool helf_xtensa_is_rw_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_xtensa_is_xtensa(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_section(index,input_file);
}

intptr_t helf_xtensa_rw_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_xtensa_is_rw_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rw_section_offset_get(index,input_file);
}

size_t helf_xtensa_rw_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_xtensa_is_xtensa(input_file))
    {
        return 0;
    }
    return helf_arch_rw_section_size_get(input_file);
}

bool helf_xtensa_is_bss_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_xtensa_is_xtensa(input_file))
    {
        return false;
    }
    return helf_arch_is_bss_section(index,input_file);
}

size_t helf_xtensa_program_header_count_get(helf_file_input_t *input_file)
{
    if(!helf_xtensa_is_xtensa(input_file))
    {
        return 0;
    }
    return helf_arch_program_header_count_get(input_file);
}

bool helf_xtensa_program_header_get(helf_file_input_t *input_file,size_t index,helf_elf32_program_header_t *programheader)
{
    if(programheader==NULL)
    {
        return false;
    }
    if(!helf_xtensa_is_xtensa(input_file))
    {
        return false;
    }
    if(index>=helf_arch_program_header_count_get(input_file))
    {
        return false;
    }
    return helf_file_input_32_bits_program_header_get(input_file,index,programheader);
}

size_t helf_xtensa_segment_count_get(helf_file_input_t *input_file)
{
    return helf_xtensa_program_header_count_get(input_file);
}

bool helf_xtensa_segment_info_get(helf_file_input_t *input_file,size_t index,helf_xtensa_segment_info_t *info)
{
    if(!helf_xtensa_is_xtensa(input_file))
    {
        return false;
    }
    return helf_arch_segment_info_get(input_file,index,info);
}

bool helf_xtensa_is_load_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_xtensa_is_xtensa(input_file))
    {
        return false;
    }
    return helf_arch_is_load_segment(index,input_file);
}

bool helf_xtensa_is_rx_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_xtensa_is_xtensa(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_segment(index,input_file);
}

bool helf_xtensa_is_ro_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_xtensa_is_xtensa(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_segment(index,input_file);
}

bool helf_xtensa_is_rw_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_xtensa_is_xtensa(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_segment(index,input_file);
}

size_t helf_xtensa_load_segment_count_get(helf_file_input_t *input_file)
{
    if(!helf_xtensa_is_xtensa(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_count_get(input_file);
}

bool helf_xtensa_load_segment_info_get(helf_file_input_t *input_file,size_t index,helf_xtensa_segment_info_t *info)
{
    if(!helf_xtensa_is_xtensa(input_file))
    {
        return false;
    }
    return helf_arch_load_segment_info_get(input_file,index,info);
}

size_t helf_xtensa_load_segment_total_filesz_get(helf_file_input_t *input_file)
{
    if(!helf_xtensa_is_xtensa(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_filesz_get(input_file);
}

size_t helf_xtensa_load_segment_total_memsz_get(helf_file_input_t *input_file)
{
    if(!helf_xtensa_is_xtensa(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_memsz_get(input_file);
}

size_t helf_xtensa_relocation_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_xtensa_is_xtensa(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_section_count_get(input_file);
}

bool helf_xtensa_relocation_section_info_get(helf_file_input_t *input_file,size_t index,helf_arch_relocation_section_info_t *info)
{
    if(!helf_xtensa_is_xtensa(input_file))
    {
        return false;
    }
    return helf_arch_relocation_section_info_get(input_file,index,info);
}

size_t helf_xtensa_relocation_count_get(helf_file_input_t *input_file,size_t section_index)
{
    if(!helf_xtensa_is_xtensa(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_count_get(input_file,section_index);
}

bool helf_xtensa_relocation_get(helf_file_input_t *input_file,size_t section_index,size_t index,helf_arch_relocation_info_t *info)
{
    if(!helf_xtensa_is_xtensa(input_file))
    {
        return false;
    }
    if(!helf_arch_relocation_get(input_file,section_index,index,info))
    {
        return false;
    }
    return true;
}

bool helf_xtensa_relocation_symbol_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_symbol_info_t *info)
{
    if(!helf_xtensa_is_xtensa(input_file))
    {
        return false;
    }
    return helf_arch_relocation_symbol_info_get(input_file,relocation,info);
}

static const helf_arch_relocation_type_name_t helf_xtensa_relocation_type_names[]=
{
    {0,0,"R_XTENSA_NONE"},
    {1,4,"R_XTENSA_32"},
    {2,4,"R_XTENSA_RTLD"},
    {3,4,"R_XTENSA_GLOB_DAT"},
    {4,4,"R_XTENSA_JMP_SLOT"},
    {5,4,"R_XTENSA_RELATIVE"},
    {6,4,"R_XTENSA_PLT"},
    {8,0,"R_XTENSA_OP0"},
    {9,0,"R_XTENSA_OP1"},
    {10,0,"R_XTENSA_OP2"},
    {11,0,"R_XTENSA_ASM_EXPAND"},
    {12,0,"R_XTENSA_ASM_SIMPLIFY"},
    {14,4,"R_XTENSA_32_PCREL"},
    {15,4,"R_XTENSA_GNU_VTINHERIT"},
    {16,4,"R_XTENSA_GNU_VTENTRY"},
    {17,1,"R_XTENSA_DIFF8"},
    {18,2,"R_XTENSA_DIFF16"},
    {19,4,"R_XTENSA_DIFF32"},
    {20,0,"R_XTENSA_SLOT0_OP"},
    {21,0,"R_XTENSA_SLOT1_OP"},
    {22,0,"R_XTENSA_SLOT2_OP"},
    {23,0,"R_XTENSA_SLOT3_OP"},
    {24,0,"R_XTENSA_SLOT4_OP"},
    {25,0,"R_XTENSA_SLOT5_OP"},
    {26,0,"R_XTENSA_SLOT6_OP"},
    {27,0,"R_XTENSA_SLOT7_OP"},
    {28,0,"R_XTENSA_SLOT8_OP"},
    {29,0,"R_XTENSA_SLOT9_OP"},
    {30,0,"R_XTENSA_SLOT10_OP"},
    {31,0,"R_XTENSA_SLOT11_OP"},
    {32,0,"R_XTENSA_SLOT12_OP"},
    {33,0,"R_XTENSA_SLOT13_OP"},
    {34,0,"R_XTENSA_SLOT14_OP"},
    {35,0,"R_XTENSA_SLOT0_ALT"},
    {36,0,"R_XTENSA_SLOT1_ALT"},
    {37,0,"R_XTENSA_SLOT2_ALT"},
    {38,0,"R_XTENSA_SLOT3_ALT"},
    {39,0,"R_XTENSA_SLOT4_ALT"},
    {40,0,"R_XTENSA_SLOT5_ALT"},
    {41,0,"R_XTENSA_SLOT6_ALT"},
    {42,0,"R_XTENSA_SLOT7_ALT"},
    {43,0,"R_XTENSA_SLOT8_ALT"},
    {44,0,"R_XTENSA_SLOT9_ALT"},
    {45,0,"R_XTENSA_SLOT10_ALT"},
    {46,0,"R_XTENSA_SLOT11_ALT"},
    {47,0,"R_XTENSA_SLOT12_ALT"},
    {48,0,"R_XTENSA_SLOT13_ALT"},
    {49,0,"R_XTENSA_SLOT14_ALT"},
    {50,4,"R_XTENSA_TLSDESC_FN"},
    {51,4,"R_XTENSA_TLSDESC_ARG"},
    {52,4,"R_XTENSA_TLS_DTPOFF"},
    {53,4,"R_XTENSA_TLS_TPOFF"},
    {54,0,"R_XTENSA_TLS_FUNC"},
    {55,0,"R_XTENSA_TLS_ARG"},
    {56,0,"R_XTENSA_TLS_CALL"},
    {57,1,"R_XTENSA_PDIFF8"},
    {58,2,"R_XTENSA_PDIFF16"},
    {59,4,"R_XTENSA_PDIFF32"},
    {60,1,"R_XTENSA_NDIFF8"},
    {61,2,"R_XTENSA_NDIFF16"},
    {62,4,"R_XTENSA_NDIFF32"},
    {63,0,"R_XTENSA_PDIFF_ULEB128"},
    {64,4,"R_XTENSA_32_ABS"},
};

const char *helf_xtensa_relocation_type_name_get(uint32_t type)
{
    return helf_arch_relocation_type_name_get(helf_xtensa_relocation_type_names,sizeof(helf_xtensa_relocation_type_names)/sizeof(helf_xtensa_relocation_type_names[0]),type);
}

uint8_t helf_xtensa_relocation_size_get(uint32_t type)
{
    return helf_arch_relocation_type_size_get(helf_xtensa_relocation_type_names,sizeof(helf_xtensa_relocation_type_names)/sizeof(helf_xtensa_relocation_type_names[0]),type);
}

const char *helf_xtensa_section_type_name_get(uint32_t type)
{
    (void)type;
    return NULL;
}

bool helf_xtensa_relocation_address_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_relocation_address_info_t *info)
{
    if(!helf_xtensa_is_xtensa(input_file) || relocation==NULL)
    {
        return false;
    }
    uint8_t size=helf_xtensa_relocation_size_get(relocation->type);
    if(size==0)
    {
        return false;
    }
    return helf_arch_relocation_address_info_get(input_file,relocation,size,info);
}

size_t helf_xtensa_relocation_data_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,void *buffer,size_t buffer_length)
{
    if(!helf_xtensa_is_xtensa(input_file) || relocation==NULL)
    {
        return 0;
    }
    uint8_t size=helf_xtensa_relocation_size_get(relocation->type);
    if(size==0)
    {
        return 0;
    }
    return helf_arch_relocation_data_get(input_file,relocation,size,buffer,buffer_length);
}

bool helf_xtensa_is_shared_object(helf_file_input_t *input_file)
{
    return helf_arch_is_shared_object(input_file,helf_xtensa_machines,sizeof(helf_xtensa_machines)/sizeof(helf_xtensa_machines[0]),HELF_ELFCLASS_32);
}

bool helf_xtensa_is_relocatable(helf_file_input_t *input_file)
{
    return helf_arch_is_relocatable(input_file,helf_xtensa_machines,sizeof(helf_xtensa_machines)/sizeof(helf_xtensa_machines[0]),HELF_ELFCLASS_32);
}

size_t helf_xtensa_dynamic_count_get(helf_file_input_t *input_file)
{
    if(!helf_xtensa_is_xtensa(input_file))
    {
        return 0;
    }
    return helf_arch_dynamic_count_get(input_file);
}

bool helf_xtensa_dynamic_info_get(helf_file_input_t *input_file,size_t index,helf_arch_dynamic_info_t *info)
{
    if(!helf_xtensa_is_xtensa(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_info_get(input_file,index,info);
}

bool helf_xtensa_dynamic_string_get(helf_file_input_t *input_file,uint64_t offset,char *buffer,size_t buffer_length)
{
    if(!helf_xtensa_is_xtensa(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_string_get(input_file,offset,buffer,buffer_length);
}

/* helf_relocation_howto_begin */
static const helf_arch_relocation_howto_t helf_xtensa_reloc_howto[]=
{
    {0x00000000u,0,0,0,0,0x0050,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000001u,4,32,0,0,0x0018,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000002u,4,32,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000003u,4,32,0,0,0x0100,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000004u,4,32,0,0,0x0140,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000005u,4,32,0,0,0x0080,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000006u,4,32,0,0,0x0010,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000008u,0,0,0,0,0x0013,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000009u,0,0,0,0,0x0013,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000000Au,0,0,0,0,0x0013,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000000Bu,0,0,0,0,0x0013,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000000Cu,0,0,0,0,0x0013,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000000Eu,4,32,0,0,0x0013,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x0000000Fu,4,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000010u,4,0,0,0,0x0010,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000011u,1,8,0,0,0x0010,0x0000000000000000ULL,0x00000000000000FFULL},
    {0x00000012u,2,16,0,0,0x0010,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000013u,4,32,0,0,0x0010,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000014u,0,0,0,0,0x0013,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000015u,0,0,0,0,0x0013,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000016u,0,0,0,0,0x0013,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000017u,0,0,0,0,0x0013,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000018u,0,0,0,0,0x0013,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000019u,0,0,0,0,0x0013,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Au,0,0,0,0,0x0013,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Bu,0,0,0,0,0x0013,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Cu,0,0,0,0,0x0013,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Du,0,0,0,0,0x0013,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Eu,0,0,0,0,0x0013,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Fu,0,0,0,0,0x0013,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000020u,0,0,0,0,0x0013,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000021u,0,0,0,0,0x0013,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000022u,0,0,0,0,0x0013,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000023u,0,0,0,0,0x0013,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000024u,0,0,0,0,0x0013,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000025u,0,0,0,0,0x0013,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000026u,0,0,0,0,0x0013,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000027u,0,0,0,0,0x0013,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000028u,0,0,0,0,0x0013,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000029u,0,0,0,0,0x0013,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000002Au,0,0,0,0,0x0013,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000002Bu,0,0,0,0,0x0013,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000002Cu,0,0,0,0,0x0013,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000002Du,0,0,0,0,0x0013,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000002Eu,0,0,0,0,0x0013,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000002Fu,0,0,0,0,0x0013,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000030u,0,0,0,0,0x0013,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000031u,0,0,0,0,0x0013,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000032u,4,32,0,0,0x01D0,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000033u,4,32,0,0,0x01D0,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000034u,4,32,0,0,0x01D0,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000035u,4,32,0,0,0x01D0,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000036u,0,0,0,0,0x01D0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000037u,0,0,0,0,0x01D0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000038u,0,0,0,0,0x01D0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000039u,1,8,0,0,0x0010,0x0000000000000000ULL,0x00000000000000FFULL},
    {0x0000003Au,2,16,0,0,0x0010,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000003Bu,4,32,0,0,0x0010,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x0000003Cu,1,8,0,0,0x0010,0x0000000000000000ULL,0x00000000000000FFULL},
    {0x0000003Du,2,16,0,0,0x0010,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000003Eu,4,32,0,0,0x0010,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x0000003Fu,0,0,0,0,0x0010,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000040u,4,32,0,0,0x0010,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
};

bool helf_xtensa_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto)
{
    if(howto==NULL)
    {
        return false;
    }
    size_t count=sizeof(helf_xtensa_reloc_howto)/sizeof(helf_xtensa_reloc_howto[0]);
    if(count==0)
    {
        return false;
    }
    for(size_t i=0;i<count;i++)
    {
        if(helf_xtensa_reloc_howto[i].type==type)
        {
            *howto=helf_xtensa_reloc_howto[i];
            return true;
        }
    }
    return false;
}
/* helf_relocation_howto_end */


