/***************************************************************
 * Name:      helf_arch_s390.c
 * Purpose:   实现helf_arch_s390接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "helf_arch_s390.h"

static const uint16_t helf_s390_machines[]=
{
    HELF_EM_S390,
    HELF_EM_S390_OLD,
};

bool helf_s390_is_s390(helf_file_input_t *input_file)
{
    return helf_arch_is(input_file,helf_s390_machines,sizeof(helf_s390_machines)/sizeof(helf_s390_machines[0]),HELF_ELFCLASS_32);
}

bool helf_s390_is_big_endian(helf_file_input_t *input_file)
{
    if(!helf_s390_is_s390(input_file))
    {
        return false;
    }
    return helf_file_input_is_big_endian(input_file);
}

bool helf_s390_is_executable(helf_file_input_t *input_file)
{
    return helf_arch_is_executable(input_file,helf_s390_machines,sizeof(helf_s390_machines)/sizeof(helf_s390_machines[0]),HELF_ELFCLASS_32);
}

bool helf_s390_entry_get(helf_file_input_t *input_file,uint32_t *entry)
{
    if(entry==NULL)
    {
        return false;
    }
    if(!helf_s390_is_s390(input_file))
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

size_t helf_s390_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_s390_is_s390(input_file))
    {
        return 0;
    }
    return helf_arch_section_count_get(input_file);
}

bool helf_s390_section_info_get(helf_file_input_t *input_file,size_t index,helf_s390_section_info_t *info)
{
    if(!helf_s390_is_s390(input_file))
    {
        return false;
    }
    return helf_arch_section_info_get(input_file,index,info);
}

bool helf_s390_is_rx_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_s390_is_s390(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_section(index,input_file);
}

intptr_t helf_s390_rx_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_s390_is_rx_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rx_section_offset_get(index,input_file);
}

size_t helf_s390_rx_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_s390_is_s390(input_file))
    {
        return 0;
    }
    return helf_arch_rx_section_size_get(input_file);
}

bool helf_s390_is_ro_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_s390_is_s390(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_section(index,input_file);
}

intptr_t helf_s390_ro_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_s390_is_ro_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_ro_section_offset_get(index,input_file);
}

size_t helf_s390_ro_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_s390_is_s390(input_file))
    {
        return 0;
    }
    return helf_arch_ro_section_size_get(input_file);
}

bool helf_s390_is_rw_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_s390_is_s390(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_section(index,input_file);
}

intptr_t helf_s390_rw_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_s390_is_rw_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rw_section_offset_get(index,input_file);
}

size_t helf_s390_rw_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_s390_is_s390(input_file))
    {
        return 0;
    }
    return helf_arch_rw_section_size_get(input_file);
}

bool helf_s390_is_bss_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_s390_is_s390(input_file))
    {
        return false;
    }
    return helf_arch_is_bss_section(index,input_file);
}

size_t helf_s390_program_header_count_get(helf_file_input_t *input_file)
{
    if(!helf_s390_is_s390(input_file))
    {
        return 0;
    }
    return helf_arch_program_header_count_get(input_file);
}

bool helf_s390_program_header_get(helf_file_input_t *input_file,size_t index,helf_elf32_program_header_t *programheader)
{
    if(programheader==NULL)
    {
        return false;
    }
    if(!helf_s390_is_s390(input_file))
    {
        return false;
    }
    if(index>=helf_arch_program_header_count_get(input_file))
    {
        return false;
    }
    return helf_file_input_32_bits_program_header_get(input_file,index,programheader);
}

size_t helf_s390_segment_count_get(helf_file_input_t *input_file)
{
    return helf_s390_program_header_count_get(input_file);
}

bool helf_s390_segment_info_get(helf_file_input_t *input_file,size_t index,helf_s390_segment_info_t *info)
{
    if(!helf_s390_is_s390(input_file))
    {
        return false;
    }
    return helf_arch_segment_info_get(input_file,index,info);
}

bool helf_s390_is_load_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_s390_is_s390(input_file))
    {
        return false;
    }
    return helf_arch_is_load_segment(index,input_file);
}

bool helf_s390_is_rx_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_s390_is_s390(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_segment(index,input_file);
}

bool helf_s390_is_ro_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_s390_is_s390(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_segment(index,input_file);
}

bool helf_s390_is_rw_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_s390_is_s390(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_segment(index,input_file);
}

size_t helf_s390_load_segment_count_get(helf_file_input_t *input_file)
{
    if(!helf_s390_is_s390(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_count_get(input_file);
}

bool helf_s390_load_segment_info_get(helf_file_input_t *input_file,size_t index,helf_s390_segment_info_t *info)
{
    if(!helf_s390_is_s390(input_file))
    {
        return false;
    }
    return helf_arch_load_segment_info_get(input_file,index,info);
}

size_t helf_s390_load_segment_total_filesz_get(helf_file_input_t *input_file)
{
    if(!helf_s390_is_s390(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_filesz_get(input_file);
}

size_t helf_s390_load_segment_total_memsz_get(helf_file_input_t *input_file)
{
    if(!helf_s390_is_s390(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_memsz_get(input_file);
}

size_t helf_s390_relocation_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_s390_is_s390(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_section_count_get(input_file);
}

bool helf_s390_relocation_section_info_get(helf_file_input_t *input_file,size_t index,helf_arch_relocation_section_info_t *info)
{
    if(!helf_s390_is_s390(input_file))
    {
        return false;
    }
    return helf_arch_relocation_section_info_get(input_file,index,info);
}

size_t helf_s390_relocation_count_get(helf_file_input_t *input_file,size_t section_index)
{
    if(!helf_s390_is_s390(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_count_get(input_file,section_index);
}

bool helf_s390_relocation_get(helf_file_input_t *input_file,size_t section_index,size_t index,helf_arch_relocation_info_t *info)
{
    if(!helf_s390_is_s390(input_file))
    {
        return false;
    }
    if(!helf_arch_relocation_get(input_file,section_index,index,info))
    {
        return false;
    }
    return true;
}

bool helf_s390_relocation_symbol_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_symbol_info_t *info)
{
    if(!helf_s390_is_s390(input_file))
    {
        return false;
    }
    return helf_arch_relocation_symbol_info_get(input_file,relocation,info);
}

static const helf_arch_relocation_type_name_t helf_s390_relocation_type_names[]=
{
    {0,0,"R_390_NONE"},
    {1,1,"R_390_8"},
    {2,2,"R_390_12"},
    {3,2,"R_390_16"},
    {4,4,"R_390_32"},
    {5,4,"R_390_PC32"},
    {6,2,"R_390_GOT12"},
    {7,4,"R_390_GOT32"},
    {8,4,"R_390_PLT32"},
    {9,4,"R_390_COPY"},
    {10,4,"R_390_GLOB_DAT"},
    {11,4,"R_390_JMP_SLOT"},
    {12,4,"R_390_RELATIVE"},
    {13,4,"R_390_GOTOFF32"},
    {14,4,"R_390_GOTPC"},
    {15,2,"R_390_GOT16"},
    {16,2,"R_390_PC16"},
    {62,2,"R_390_PC12DBL"},
    {63,2,"R_390_PLT12DBL"},
    {17,2,"R_390_PC16DBL"},
    {18,2,"R_390_PLT16DBL"},
    {64,4,"R_390_PC24DBL"},
    {65,4,"R_390_PLT24DBL"},
    {19,4,"R_390_PC32DBL"},
    {20,4,"R_390_PLT32DBL"},
    {21,4,"R_390_GOTPCDBL"},
    {22,8,"R_390_64"},
    {23,8,"R_390_PC64"},
    {24,8,"R_390_GOT64"},
    {25,8,"R_390_PLT64"},
    {26,4,"R_390_GOTENT"},
    {27,2,"R_390_GOTOFF16"},
    {28,8,"R_390_GOTOFF64"},
    {29,2,"R_390_GOTPLT12"},
    {30,2,"R_390_GOTPLT16"},
    {31,4,"R_390_GOTPLT32"},
    {32,8,"R_390_GOTPLT64"},
    {33,4,"R_390_GOTPLTENT"},
    {34,2,"R_390_PLTOFF16"},
    {35,4,"R_390_PLTOFF32"},
    {36,8,"R_390_PLTOFF64"},
    {37,0,"R_390_TLS_LOAD"},
    {38,0,"R_390_TLS_GDCALL"},
    {39,0,"R_390_TLS_LDCALL"},
    {40,4,"R_390_TLS_GD32"},
    {41,8,"R_390_TLS_GD64"},
    {42,2,"R_390_TLS_GOTIE12"},
    {43,4,"R_390_TLS_GOTIE32"},
    {44,8,"R_390_TLS_GOTIE64"},
    {45,4,"R_390_TLS_LDM32"},
    {46,8,"R_390_TLS_LDM64"},
    {47,4,"R_390_TLS_IE32"},
    {48,8,"R_390_TLS_IE64"},
    {49,4,"R_390_TLS_IEENT"},
    {50,4,"R_390_TLS_LE32"},
    {51,8,"R_390_TLS_LE64"},
    {52,4,"R_390_TLS_LDO32"},
    {53,8,"R_390_TLS_LDO64"},
    {54,4,"R_390_TLS_DTPMOD"},
    {55,4,"R_390_TLS_DTPOFF"},
    {56,4,"R_390_TLS_TPOFF"},
    {57,4,"R_390_20"},
    {58,4,"R_390_GOT20"},
    {59,4,"R_390_GOTPLT20"},
    {60,4,"R_390_TLS_GOTIE20"},
    {61,4,"R_390_IRELATIVE"},
    {250,4,"R_390_GNU_VTINHERIT"},
    {251,4,"R_390_GNU_VTENTRY"},
};

const char *helf_s390_relocation_type_name_get(uint32_t type)
{
    return helf_arch_relocation_type_name_get(helf_s390_relocation_type_names,sizeof(helf_s390_relocation_type_names)/sizeof(helf_s390_relocation_type_names[0]),type);
}

uint8_t helf_s390_relocation_size_get(uint32_t type)
{
    return helf_arch_relocation_type_size_get(helf_s390_relocation_type_names,sizeof(helf_s390_relocation_type_names)/sizeof(helf_s390_relocation_type_names[0]),type);
}

const char *helf_s390_section_type_name_get(uint32_t type)
{
    (void)type;
    return NULL;
}

bool helf_s390_relocation_address_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_relocation_address_info_t *info)
{
    if(!helf_s390_is_s390(input_file) || relocation==NULL)
    {
        return false;
    }
    uint8_t size=helf_s390_relocation_size_get(relocation->type);
    if(size==0)
    {
        return false;
    }
    return helf_arch_relocation_address_info_get(input_file,relocation,size,info);
}

size_t helf_s390_relocation_data_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,void *buffer,size_t buffer_length)
{
    if(!helf_s390_is_s390(input_file) || relocation==NULL)
    {
        return 0;
    }
    uint8_t size=helf_s390_relocation_size_get(relocation->type);
    if(size==0)
    {
        return 0;
    }
    return helf_arch_relocation_data_get(input_file,relocation,size,buffer,buffer_length);
}

bool helf_s390_is_shared_object(helf_file_input_t *input_file)
{
    return helf_arch_is_shared_object(input_file,helf_s390_machines,sizeof(helf_s390_machines)/sizeof(helf_s390_machines[0]),HELF_ELFCLASS_32);
}

bool helf_s390_is_relocatable(helf_file_input_t *input_file)
{
    return helf_arch_is_relocatable(input_file,helf_s390_machines,sizeof(helf_s390_machines)/sizeof(helf_s390_machines[0]),HELF_ELFCLASS_32);
}

size_t helf_s390_dynamic_count_get(helf_file_input_t *input_file)
{
    if(!helf_s390_is_s390(input_file))
    {
        return 0;
    }
    return helf_arch_dynamic_count_get(input_file);
}

bool helf_s390_dynamic_info_get(helf_file_input_t *input_file,size_t index,helf_arch_dynamic_info_t *info)
{
    if(!helf_s390_is_s390(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_info_get(input_file,index,info);
}

bool helf_s390_dynamic_string_get(helf_file_input_t *input_file,uint64_t offset,char *buffer,size_t buffer_length)
{
    if(!helf_s390_is_s390(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_string_get(input_file,offset,buffer,buffer_length);
}

/* helf_relocation_howto_begin */
static const helf_arch_relocation_howto_t helf_s390_reloc_howto[]=
{
    {0x00000000u,0,0,0,0,0x0040,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000001u,1,8,0,0,0x0000,0x0000000000000000ULL,0x00000000000000FFULL},
    {0x00000002u,2,12,0,0,0x0000,0x0000000000000000ULL,0x0000000000000FFFULL},
    {0x00000003u,2,16,0,0,0x0000,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000004u,4,32,0,0,0x0000,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000005u,4,32,0,0,0x0003,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000006u,2,12,0,0,0x0200,0x0000000000000000ULL,0x0000000000000FFFULL},
    {0x00000007u,4,32,0,0,0x0200,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000008u,4,32,0,0,0x0003,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000009u,4,32,0,0,0x0180,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x0000000Au,4,32,0,0,0x0100,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x0000000Bu,4,32,0,0,0x0140,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x0000000Cu,4,32,0,0,0x0081,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x0000000Du,4,32,0,0,0x0200,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x0000000Eu,4,32,0,0,0x0203,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x0000000Fu,2,16,0,0,0x0200,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000010u,2,16,0,0,0x0003,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000011u,2,16,1,0,0x0003,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000012u,2,16,1,0,0x0003,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000013u,4,32,1,0,0x0003,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000014u,4,32,1,0,0x0003,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000015u,4,32,1,0,0x0203,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000016u,8,64,0,0,0x0000,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000017u,8,64,0,0,0x0003,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000018u,8,64,0,0,0x0200,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000019u,8,64,0,0,0x0003,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x0000001Au,4,32,1,0,0x0203,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x0000001Bu,2,16,0,0,0x0200,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000001Cu,8,64,0,0,0x0200,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x0000001Du,2,12,0,0,0x0200,0x0000000000000000ULL,0x0000000000000FFFULL},
    {0x0000001Eu,2,16,0,0,0x0200,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000001Fu,4,32,0,0,0x0200,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000020u,8,64,0,0,0x0200,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000021u,4,32,1,0,0x0203,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000022u,2,16,0,0,0x0000,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000023u,4,32,0,0,0x0000,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000024u,8,64,0,0,0x0000,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000025u,0,0,0,0,0x01D0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000026u,0,0,0,0,0x01D0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000027u,0,0,0,0,0x01D0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000028u,4,32,0,0,0x01C0,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000029u,8,64,0,0,0x01C0,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x0000002Au,2,12,0,0,0x03C0,0x0000000000000000ULL,0x0000000000000FFFULL},
    {0x0000002Bu,4,32,0,0,0x03C0,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x0000002Cu,8,64,0,0,0x03C0,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x0000002Du,4,32,0,0,0x01C0,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x0000002Eu,8,64,0,0,0x01C0,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x0000002Fu,4,32,0,0,0x01C0,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000030u,8,64,0,0,0x01C0,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000031u,4,32,1,0,0x01C3,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000032u,4,32,0,0,0x01C0,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000033u,8,64,0,0,0x01C0,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000034u,4,32,0,0,0x01C0,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000035u,8,64,0,0,0x01C0,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000036u,4,32,0,0,0x01C0,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000037u,4,32,0,0,0x01C0,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000038u,4,32,0,0,0x01C0,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000039u,4,20,0,8,0x0010,0x0000000000000000ULL,0x000000000FFFFF00ULL},
    {0x0000003Au,4,20,0,8,0x0210,0x0000000000000000ULL,0x000000000FFFFF00ULL},
    {0x0000003Bu,4,20,0,8,0x0210,0x0000000000000000ULL,0x000000000FFFFF00ULL},
    {0x0000003Cu,4,20,0,8,0x03D0,0x0000000000000000ULL,0x000000000FFFFF00ULL},
    {0x0000003Du,4,32,0,0,0x0081,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x0000003Eu,2,12,1,0,0x0003,0x0000000000000000ULL,0x0000000000000FFFULL},
    {0x0000003Fu,2,12,1,0,0x0003,0x0000000000000000ULL,0x0000000000000FFFULL},
    {0x00000040u,4,24,1,0,0x0003,0x0000000000000000ULL,0x0000000000FFFFFFULL},
    {0x00000041u,4,24,1,0,0x0003,0x0000000000000000ULL,0x0000000000FFFFFFULL},
    {0x000000FAu,4,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000FBu,4,0,0,0,0x0010,0x0000000000000000ULL,0x0000000000000000ULL},
};

bool helf_s390_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto)
{
    if(howto==NULL)
    {
        return false;
    }
    size_t count=sizeof(helf_s390_reloc_howto)/sizeof(helf_s390_reloc_howto[0]);
    if(count==0)
    {
        return false;
    }
    for(size_t i=0;i<count;i++)
    {
        if(helf_s390_reloc_howto[i].type==type)
        {
            *howto=helf_s390_reloc_howto[i];
            return true;
        }
    }
    return false;
}
/* helf_relocation_howto_end */


