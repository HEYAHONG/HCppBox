/***************************************************************
 * Name:      helf_arch_rx.c
 * Purpose:   实现helf_arch_rx接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "helf_arch_rx.h"

static const uint16_t helf_rx_machines[]=
{
    HELF_EM_RX,
};

bool helf_rx_is_rx(helf_file_input_t *input_file)
{
    return helf_arch_is(input_file,helf_rx_machines,sizeof(helf_rx_machines)/sizeof(helf_rx_machines[0]),HELF_ELFCLASS_32);
}

bool helf_rx_is_big_endian(helf_file_input_t *input_file)
{
    if(!helf_rx_is_rx(input_file))
    {
        return false;
    }
    return helf_file_input_is_big_endian(input_file);
}

bool helf_rx_is_executable(helf_file_input_t *input_file)
{
    return helf_arch_is_executable(input_file,helf_rx_machines,sizeof(helf_rx_machines)/sizeof(helf_rx_machines[0]),HELF_ELFCLASS_32);
}

bool helf_rx_entry_get(helf_file_input_t *input_file,uint32_t *entry)
{
    if(entry==NULL)
    {
        return false;
    }
    if(!helf_rx_is_rx(input_file))
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

size_t helf_rx_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_rx_is_rx(input_file))
    {
        return 0;
    }
    return helf_arch_section_count_get(input_file);
}

bool helf_rx_section_info_get(helf_file_input_t *input_file,size_t index,helf_rx_section_info_t *info)
{
    if(!helf_rx_is_rx(input_file))
    {
        return false;
    }
    return helf_arch_section_info_get(input_file,index,info);
}

bool helf_rx_is_rx_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_rx_is_rx(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_section(index,input_file);
}

intptr_t helf_rx_rx_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_rx_is_rx_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rx_section_offset_get(index,input_file);
}

size_t helf_rx_rx_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_rx_is_rx(input_file))
    {
        return 0;
    }
    return helf_arch_rx_section_size_get(input_file);
}

bool helf_rx_is_ro_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_rx_is_rx(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_section(index,input_file);
}

intptr_t helf_rx_ro_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_rx_is_ro_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_ro_section_offset_get(index,input_file);
}

size_t helf_rx_ro_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_rx_is_rx(input_file))
    {
        return 0;
    }
    return helf_arch_ro_section_size_get(input_file);
}

bool helf_rx_is_rw_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_rx_is_rx(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_section(index,input_file);
}

intptr_t helf_rx_rw_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_rx_is_rw_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rw_section_offset_get(index,input_file);
}

size_t helf_rx_rw_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_rx_is_rx(input_file))
    {
        return 0;
    }
    return helf_arch_rw_section_size_get(input_file);
}

bool helf_rx_is_bss_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_rx_is_rx(input_file))
    {
        return false;
    }
    return helf_arch_is_bss_section(index,input_file);
}

size_t helf_rx_program_header_count_get(helf_file_input_t *input_file)
{
    if(!helf_rx_is_rx(input_file))
    {
        return 0;
    }
    return helf_arch_program_header_count_get(input_file);
}

bool helf_rx_program_header_get(helf_file_input_t *input_file,size_t index,helf_elf32_program_header_t *programheader)
{
    if(programheader==NULL)
    {
        return false;
    }
    if(!helf_rx_is_rx(input_file))
    {
        return false;
    }
    if(index>=helf_arch_program_header_count_get(input_file))
    {
        return false;
    }
    return helf_file_input_32_bits_program_header_get(input_file,index,programheader);
}

size_t helf_rx_segment_count_get(helf_file_input_t *input_file)
{
    return helf_rx_program_header_count_get(input_file);
}

bool helf_rx_segment_info_get(helf_file_input_t *input_file,size_t index,helf_rx_segment_info_t *info)
{
    if(!helf_rx_is_rx(input_file))
    {
        return false;
    }
    return helf_arch_segment_info_get(input_file,index,info);
}

bool helf_rx_is_load_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_rx_is_rx(input_file))
    {
        return false;
    }
    return helf_arch_is_load_segment(index,input_file);
}

bool helf_rx_is_rx_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_rx_is_rx(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_segment(index,input_file);
}

bool helf_rx_is_ro_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_rx_is_rx(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_segment(index,input_file);
}

bool helf_rx_is_rw_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_rx_is_rx(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_segment(index,input_file);
}

size_t helf_rx_load_segment_count_get(helf_file_input_t *input_file)
{
    if(!helf_rx_is_rx(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_count_get(input_file);
}

bool helf_rx_load_segment_info_get(helf_file_input_t *input_file,size_t index,helf_rx_segment_info_t *info)
{
    if(!helf_rx_is_rx(input_file))
    {
        return false;
    }
    return helf_arch_load_segment_info_get(input_file,index,info);
}

size_t helf_rx_load_segment_total_filesz_get(helf_file_input_t *input_file)
{
    if(!helf_rx_is_rx(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_filesz_get(input_file);
}

size_t helf_rx_load_segment_total_memsz_get(helf_file_input_t *input_file)
{
    if(!helf_rx_is_rx(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_memsz_get(input_file);
}

size_t helf_rx_relocation_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_rx_is_rx(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_section_count_get(input_file);
}

bool helf_rx_relocation_section_info_get(helf_file_input_t *input_file,size_t index,helf_arch_relocation_section_info_t *info)
{
    if(!helf_rx_is_rx(input_file))
    {
        return false;
    }
    return helf_arch_relocation_section_info_get(input_file,index,info);
}

size_t helf_rx_relocation_count_get(helf_file_input_t *input_file,size_t section_index)
{
    if(!helf_rx_is_rx(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_count_get(input_file,section_index);
}

bool helf_rx_relocation_get(helf_file_input_t *input_file,size_t section_index,size_t index,helf_arch_relocation_info_t *info)
{
    if(!helf_rx_is_rx(input_file))
    {
        return false;
    }
    if(!helf_arch_relocation_get(input_file,section_index,index,info))
    {
        return false;
    }
    return true;
}

bool helf_rx_relocation_symbol_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_symbol_info_t *info)
{
    if(!helf_rx_is_rx(input_file))
    {
        return false;
    }
    return helf_arch_relocation_symbol_info_get(input_file,relocation,info);
}

static const helf_arch_relocation_type_name_t helf_rx_relocation_type_names[]=
{
    {0,0,"R_RX_NONE"},
    {1,4,"R_RX_DIR32"},
    {2,0,"R_RX_DIR24S"},
    {3,2,"R_RX_DIR16"},
    {4,0,"R_RX_DIR16U"},
    {5,0,"R_RX_DIR16S"},
    {6,1,"R_RX_DIR8"},
    {7,0,"R_RX_DIR8U"},
    {8,0,"R_RX_DIR8S"},
    {9,0,"R_RX_DIR24S_PCREL"},
    {10,0,"R_RX_DIR16S_PCREL"},
    {11,0,"R_RX_DIR8S_PCREL"},
    {12,0,"R_RX_DIR16UL"},
    {13,0,"R_RX_DIR16UW"},
    {14,0,"R_RX_DIR8UL"},
    {15,0,"R_RX_DIR8UW"},
    {16,0,"R_RX_DIR32_REV"},
    {17,0,"R_RX_DIR16_REV"},
    {18,0,"R_RX_DIR3U_PCREL"},
    {32,0,"R_RX_RH_3_PCREL"},
    {33,0,"R_RX_RH_16_OP"},
    {34,0,"R_RX_RH_24_OP"},
    {35,0,"R_RX_RH_32_OP"},
    {36,0,"R_RX_RH_24_UNS"},
    {37,0,"R_RX_RH_8_NEG"},
    {38,0,"R_RX_RH_16_NEG"},
    {39,0,"R_RX_RH_24_NEG"},
    {40,0,"R_RX_RH_32_NEG"},
    {41,0,"R_RX_RH_DIFF"},
    {42,0,"R_RX_RH_GPRELB"},
    {43,0,"R_RX_RH_GPRELW"},
    {44,0,"R_RX_RH_GPRELL"},
    {45,0,"R_RX_RH_RELAX"},
    {65,4,"R_RX_ABS32"},
    {66,0,"R_RX_ABS24S"},
    {67,2,"R_RX_ABS16"},
    {68,0,"R_RX_ABS16U"},
    {69,0,"R_RX_ABS16S"},
    {70,1,"R_RX_ABS8"},
    {71,0,"R_RX_ABS8U"},
    {72,0,"R_RX_ABS8S"},
    {73,0,"R_RX_ABS24S_PCREL"},
    {74,0,"R_RX_ABS16S_PCREL"},
    {75,0,"R_RX_ABS8S_PCREL"},
    {76,0,"R_RX_ABS16UL"},
    {77,0,"R_RX_ABS16UW"},
    {78,0,"R_RX_ABS8UL"},
    {79,0,"R_RX_ABS8UW"},
    {80,0,"R_RX_ABS32_REV"},
    {81,0,"R_RX_ABS16_REV"},
    {128,0,"R_RX_SYM"},
    {129,0,"R_RX_OPneg"},
    {130,0,"R_RX_OPadd"},
    {131,0,"R_RX_OPsub"},
    {132,0,"R_RX_OPmul"},
    {133,0,"R_RX_OPdiv"},
    {134,0,"R_RX_OPshla"},
    {135,0,"R_RX_OPshra"},
    {136,0,"R_RX_OPsctsize"},
    {141,0,"R_RX_OPscttop"},
    {144,0,"R_RX_OPand"},
    {145,0,"R_RX_OPor"},
    {146,0,"R_RX_OPxor"},
    {147,0,"R_RX_OPnot"},
    {148,0,"R_RX_OPmod"},
    {149,0,"R_RX_OPromtop"},
    {150,0,"R_RX_OPramtop"},
};

const char *helf_rx_relocation_type_name_get(uint32_t type)
{
    return helf_arch_relocation_type_name_get(helf_rx_relocation_type_names,sizeof(helf_rx_relocation_type_names)/sizeof(helf_rx_relocation_type_names[0]),type);
}

uint8_t helf_rx_relocation_size_get(uint32_t type)
{
    return helf_arch_relocation_type_size_get(helf_rx_relocation_type_names,sizeof(helf_rx_relocation_type_names)/sizeof(helf_rx_relocation_type_names[0]),type);
}

const char *helf_rx_section_type_name_get(uint32_t type)
{
    (void)type;
    return NULL;
}

bool helf_rx_relocation_address_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_relocation_address_info_t *info)
{
    if(!helf_rx_is_rx(input_file) || relocation==NULL)
    {
        return false;
    }
    uint8_t size=helf_rx_relocation_size_get(relocation->type);
    if(size==0)
    {
        return false;
    }
    return helf_arch_relocation_address_info_get(input_file,relocation,size,info);
}

size_t helf_rx_relocation_data_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,void *buffer,size_t buffer_length)
{
    if(!helf_rx_is_rx(input_file) || relocation==NULL)
    {
        return 0;
    }
    uint8_t size=helf_rx_relocation_size_get(relocation->type);
    if(size==0)
    {
        return 0;
    }
    return helf_arch_relocation_data_get(input_file,relocation,size,buffer,buffer_length);
}

bool helf_rx_is_shared_object(helf_file_input_t *input_file)
{
    return helf_arch_is_shared_object(input_file,helf_rx_machines,sizeof(helf_rx_machines)/sizeof(helf_rx_machines[0]),HELF_ELFCLASS_32);
}

bool helf_rx_is_relocatable(helf_file_input_t *input_file)
{
    return helf_arch_is_relocatable(input_file,helf_rx_machines,sizeof(helf_rx_machines)/sizeof(helf_rx_machines[0]),HELF_ELFCLASS_32);
}

size_t helf_rx_dynamic_count_get(helf_file_input_t *input_file)
{
    if(!helf_rx_is_rx(input_file))
    {
        return 0;
    }
    return helf_arch_dynamic_count_get(input_file);
}

bool helf_rx_dynamic_info_get(helf_file_input_t *input_file,size_t index,helf_arch_dynamic_info_t *info)
{
    if(!helf_rx_is_rx(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_info_get(input_file,index,info);
}

bool helf_rx_dynamic_string_get(helf_file_input_t *input_file,uint64_t offset,char *buffer,size_t buffer_length)
{
    if(!helf_rx_is_rx(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_string_get(input_file,offset,buffer,buffer_length);
}

/* helf_relocation_howto_begin */
static const helf_arch_relocation_howto_t helf_rx_reloc_howto[]=
{
    {0x00000000u,0,0,0,0,0x0040,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000001u,4,32,0,0,0x0000,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000002u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000003u,2,16,0,0,0x0000,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000004u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000005u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000006u,1,8,0,0,0x0000,0x0000000000000000ULL,0x00000000000000FFULL},
    {0x00000007u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000008u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000009u,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000000Au,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000000Bu,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000000Cu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000000Du,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000000Eu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000000Fu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000010u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000011u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000012u,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000020u,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000021u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000022u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000023u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000024u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000025u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000026u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000027u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000028u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000029u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000002Au,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000002Bu,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000002Cu,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000002Du,0,0,0,0,0x0040,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000041u,4,32,0,0,0x0000,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000042u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000043u,2,16,0,0,0x0000,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000044u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000045u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000046u,1,8,0,0,0x0000,0x0000000000000000ULL,0x00000000000000FFULL},
    {0x00000047u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000048u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000049u,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000004Au,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000004Bu,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000004Cu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000004Du,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000004Eu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000004Fu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000050u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000051u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000080u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000081u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000082u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000083u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000084u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000085u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000086u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000087u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000088u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000008Du,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000090u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000091u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000092u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000093u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000094u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000095u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000096u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
};

bool helf_rx_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto)
{
    if(howto==NULL)
    {
        return false;
    }
    size_t count=sizeof(helf_rx_reloc_howto)/sizeof(helf_rx_reloc_howto[0]);
    if(count==0)
    {
        return false;
    }
    for(size_t i=0;i<count;i++)
    {
        if(helf_rx_reloc_howto[i].type==type)
        {
            *howto=helf_rx_reloc_howto[i];
            return true;
        }
    }
    return false;
}
/* helf_relocation_howto_end */


