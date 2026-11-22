/***************************************************************
 * Name:      helf_arch_riscv32.c
 * Purpose:   实现helf_arch_riscv32接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "helf_arch_riscv32.h"

static const uint16_t helf_riscv32_machines[]=
{
    HELF_EM_RISCV,
};

bool helf_riscv32_is_riscv32(helf_file_input_t *input_file)
{
    return helf_arch_is(input_file,helf_riscv32_machines,sizeof(helf_riscv32_machines)/sizeof(helf_riscv32_machines[0]),HELF_ELFCLASS_32);
}

bool helf_riscv32_is_big_endian(helf_file_input_t *input_file)
{
    if(!helf_riscv32_is_riscv32(input_file))
    {
        return false;
    }
    return helf_file_input_is_big_endian(input_file);
}

bool helf_riscv32_is_executable(helf_file_input_t *input_file)
{
    return helf_arch_is_executable(input_file,helf_riscv32_machines,sizeof(helf_riscv32_machines)/sizeof(helf_riscv32_machines[0]),HELF_ELFCLASS_32);
}

bool helf_riscv32_entry_get(helf_file_input_t *input_file,uint32_t *entry)
{
    if(entry==NULL)
    {
        return false;
    }
    if(!helf_riscv32_is_riscv32(input_file))
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

size_t helf_riscv32_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_riscv32_is_riscv32(input_file))
    {
        return 0;
    }
    return helf_arch_section_count_get(input_file);
}

bool helf_riscv32_section_info_get(helf_file_input_t *input_file,size_t index,helf_riscv32_section_info_t *info)
{
    if(!helf_riscv32_is_riscv32(input_file))
    {
        return false;
    }
    return helf_arch_section_info_get(input_file,index,info);
}

bool helf_riscv32_is_rx_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_riscv32_is_riscv32(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_section(index,input_file);
}

intptr_t helf_riscv32_rx_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_riscv32_is_rx_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rx_section_offset_get(index,input_file);
}

size_t helf_riscv32_rx_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_riscv32_is_riscv32(input_file))
    {
        return 0;
    }
    return helf_arch_rx_section_size_get(input_file);
}

bool helf_riscv32_is_ro_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_riscv32_is_riscv32(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_section(index,input_file);
}

intptr_t helf_riscv32_ro_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_riscv32_is_ro_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_ro_section_offset_get(index,input_file);
}

size_t helf_riscv32_ro_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_riscv32_is_riscv32(input_file))
    {
        return 0;
    }
    return helf_arch_ro_section_size_get(input_file);
}

bool helf_riscv32_is_rw_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_riscv32_is_riscv32(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_section(index,input_file);
}

intptr_t helf_riscv32_rw_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_riscv32_is_rw_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rw_section_offset_get(index,input_file);
}

size_t helf_riscv32_rw_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_riscv32_is_riscv32(input_file))
    {
        return 0;
    }
    return helf_arch_rw_section_size_get(input_file);
}

bool helf_riscv32_is_bss_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_riscv32_is_riscv32(input_file))
    {
        return false;
    }
    return helf_arch_is_bss_section(index,input_file);
}

size_t helf_riscv32_program_header_count_get(helf_file_input_t *input_file)
{
    if(!helf_riscv32_is_riscv32(input_file))
    {
        return 0;
    }
    return helf_arch_program_header_count_get(input_file);
}

bool helf_riscv32_program_header_get(helf_file_input_t *input_file,size_t index,helf_elf32_program_header_t *programheader)
{
    if(programheader==NULL)
    {
        return false;
    }
    if(!helf_riscv32_is_riscv32(input_file))
    {
        return false;
    }
    if(index>=helf_arch_program_header_count_get(input_file))
    {
        return false;
    }
    return helf_file_input_32_bits_program_header_get(input_file,index,programheader);
}

size_t helf_riscv32_segment_count_get(helf_file_input_t *input_file)
{
    return helf_riscv32_program_header_count_get(input_file);
}

bool helf_riscv32_segment_info_get(helf_file_input_t *input_file,size_t index,helf_riscv32_segment_info_t *info)
{
    if(!helf_riscv32_is_riscv32(input_file))
    {
        return false;
    }
    return helf_arch_segment_info_get(input_file,index,info);
}

bool helf_riscv32_is_load_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_riscv32_is_riscv32(input_file))
    {
        return false;
    }
    return helf_arch_is_load_segment(index,input_file);
}

bool helf_riscv32_is_rx_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_riscv32_is_riscv32(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_segment(index,input_file);
}

bool helf_riscv32_is_ro_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_riscv32_is_riscv32(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_segment(index,input_file);
}

bool helf_riscv32_is_rw_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_riscv32_is_riscv32(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_segment(index,input_file);
}

size_t helf_riscv32_load_segment_count_get(helf_file_input_t *input_file)
{
    if(!helf_riscv32_is_riscv32(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_count_get(input_file);
}

bool helf_riscv32_load_segment_info_get(helf_file_input_t *input_file,size_t index,helf_riscv32_segment_info_t *info)
{
    if(!helf_riscv32_is_riscv32(input_file))
    {
        return false;
    }
    return helf_arch_load_segment_info_get(input_file,index,info);
}

size_t helf_riscv32_load_segment_total_filesz_get(helf_file_input_t *input_file)
{
    if(!helf_riscv32_is_riscv32(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_filesz_get(input_file);
}

size_t helf_riscv32_load_segment_total_memsz_get(helf_file_input_t *input_file)
{
    if(!helf_riscv32_is_riscv32(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_memsz_get(input_file);
}

size_t helf_riscv32_relocation_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_riscv32_is_riscv32(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_section_count_get(input_file);
}

bool helf_riscv32_relocation_section_info_get(helf_file_input_t *input_file,size_t index,helf_arch_relocation_section_info_t *info)
{
    if(!helf_riscv32_is_riscv32(input_file))
    {
        return false;
    }
    return helf_arch_relocation_section_info_get(input_file,index,info);
}

size_t helf_riscv32_relocation_count_get(helf_file_input_t *input_file,size_t section_index)
{
    if(!helf_riscv32_is_riscv32(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_count_get(input_file,section_index);
}

bool helf_riscv32_relocation_get(helf_file_input_t *input_file,size_t section_index,size_t index,helf_arch_relocation_info_t *info)
{
    if(!helf_riscv32_is_riscv32(input_file))
    {
        return false;
    }
    if(!helf_arch_relocation_get(input_file,section_index,index,info))
    {
        return false;
    }
    return true;
}

bool helf_riscv32_relocation_symbol_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_symbol_info_t *info)
{
    if(!helf_riscv32_is_riscv32(input_file))
    {
        return false;
    }
    return helf_arch_relocation_symbol_info_get(input_file,relocation,info);
}

static const helf_arch_relocation_type_name_t helf_riscv32_relocation_type_names[]=
{
    {0,0,"R_RISCV_NONE"},
    {1,4,"R_RISCV_32"},
    {2,8,"R_RISCV_64"},
    {3,4,"R_RISCV_RELATIVE"},
    {4,0,"R_RISCV_COPY"},
    {5,8,"R_RISCV_JUMP_SLOT"},
    {6,4,"R_RISCV_TLS_DTPMOD32"},
    {7,8,"R_RISCV_TLS_DTPMOD64"},
    {8,4,"R_RISCV_TLS_DTPREL32"},
    {9,8,"R_RISCV_TLS_DTPREL64"},
    {10,4,"R_RISCV_TLS_TPREL32"},
    {11,8,"R_RISCV_TLS_TPREL64"},
    {12,0,"R_RISCV_TLSDESC"},
    {16,4,"R_RISCV_BRANCH"},
    {17,4,"R_RISCV_JAL"},
    {18,8,"R_RISCV_CALL"},
    {19,8,"R_RISCV_CALL_PLT"},
    {20,4,"R_RISCV_GOT_HI20"},
    {21,4,"R_RISCV_TLS_GOT_HI20"},
    {22,4,"R_RISCV_TLS_GD_HI20"},
    {23,4,"R_RISCV_PCREL_HI20"},
    {24,4,"R_RISCV_PCREL_LO12_I"},
    {25,4,"R_RISCV_PCREL_LO12_S"},
    {26,4,"R_RISCV_HI20"},
    {27,4,"R_RISCV_LO12_I"},
    {28,4,"R_RISCV_LO12_S"},
    {29,4,"R_RISCV_TPREL_HI20"},
    {30,4,"R_RISCV_TPREL_LO12_I"},
    {31,4,"R_RISCV_TPREL_LO12_S"},
    {32,0,"R_RISCV_TPREL_ADD"},
    {33,1,"R_RISCV_ADD8"},
    {34,2,"R_RISCV_ADD16"},
    {35,4,"R_RISCV_ADD32"},
    {36,8,"R_RISCV_ADD64"},
    {37,1,"R_RISCV_SUB8"},
    {38,2,"R_RISCV_SUB16"},
    {39,4,"R_RISCV_SUB32"},
    {40,8,"R_RISCV_SUB64"},
    {43,0,"R_RISCV_ALIGN"},
    {44,2,"R_RISCV_RVC_BRANCH"},
    {45,2,"R_RISCV_RVC_JUMP"},
    {46,2,"R_RISCV_RVC_LUI"},
    {47,4,"R_RISCV_GPREL_I"},
    {48,4,"R_RISCV_GPREL_S"},
    {49,4,"R_RISCV_TPREL_I"},
    {50,4,"R_RISCV_TPREL_S"},
    {51,0,"R_RISCV_RELAX"},
    {52,1,"R_RISCV_SUB6"},
    {53,1,"R_RISCV_SET6"},
    {54,1,"R_RISCV_SET8"},
    {55,2,"R_RISCV_SET16"},
    {56,4,"R_RISCV_SET32"},
    {57,4,"R_RISCV_32_PCREL"},
    {58,4,"R_RISCV_IRELATIVE"},
    {60,0,"R_RISCV_SET_ULEB128"},
    {61,0,"R_RISCV_SUB_ULEB128"},
    {62,4,"R_RISCV_TLSDESC_HI20"},
    {63,4,"R_RISCV_TLSDESC_LOAD_LO12"},
    {64,4,"R_RISCV_TLSDESC_ADD_LO12"},
    {65,0,"R_RISCV_TLSDESC_CALL"},
};

const char *helf_riscv32_relocation_type_name_get(uint32_t type)
{
    return helf_arch_relocation_type_name_get(helf_riscv32_relocation_type_names,sizeof(helf_riscv32_relocation_type_names)/sizeof(helf_riscv32_relocation_type_names[0]),type);
}

uint8_t helf_riscv32_relocation_size_get(uint32_t type)
{
    return helf_arch_relocation_type_size_get(helf_riscv32_relocation_type_names,sizeof(helf_riscv32_relocation_type_names)/sizeof(helf_riscv32_relocation_type_names[0]),type);
}

static const helf_arch_section_type_name_t helf_riscv32_section_type_names[]=
{
    {1879048195,"SHT_RISCV_ATTRIBUTES"},
};

const char *helf_riscv32_section_type_name_get(uint32_t type)
{
    return helf_arch_section_type_name_get(helf_riscv32_section_type_names,sizeof(helf_riscv32_section_type_names)/sizeof(helf_riscv32_section_type_names[0]),type);
}

bool helf_riscv32_relocation_address_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_relocation_address_info_t *info)
{
    if(!helf_riscv32_is_riscv32(input_file) || relocation==NULL)
    {
        return false;
    }
    uint8_t size=helf_riscv32_relocation_size_get(relocation->type);
    if(size==0)
    {
        return false;
    }
    return helf_arch_relocation_address_info_get(input_file,relocation,size,info);
}

size_t helf_riscv32_relocation_data_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,void *buffer,size_t buffer_length)
{
    if(!helf_riscv32_is_riscv32(input_file) || relocation==NULL)
    {
        return 0;
    }
    uint8_t size=helf_riscv32_relocation_size_get(relocation->type);
    if(size==0)
    {
        return 0;
    }
    return helf_arch_relocation_data_get(input_file,relocation,size,buffer,buffer_length);
}

bool helf_riscv32_is_shared_object(helf_file_input_t *input_file)
{
    return helf_arch_is_shared_object(input_file,helf_riscv32_machines,sizeof(helf_riscv32_machines)/sizeof(helf_riscv32_machines[0]),HELF_ELFCLASS_32);
}

bool helf_riscv32_is_relocatable(helf_file_input_t *input_file)
{
    return helf_arch_is_relocatable(input_file,helf_riscv32_machines,sizeof(helf_riscv32_machines)/sizeof(helf_riscv32_machines[0]),HELF_ELFCLASS_32);
}

size_t helf_riscv32_dynamic_count_get(helf_file_input_t *input_file)
{
    if(!helf_riscv32_is_riscv32(input_file))
    {
        return 0;
    }
    return helf_arch_dynamic_count_get(input_file);
}

bool helf_riscv32_dynamic_info_get(helf_file_input_t *input_file,size_t index,helf_arch_dynamic_info_t *info)
{
    if(!helf_riscv32_is_riscv32(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_info_get(input_file,index,info);
}

bool helf_riscv32_dynamic_string_get(helf_file_input_t *input_file,uint64_t offset,char *buffer,size_t buffer_length)
{
    if(!helf_riscv32_is_riscv32(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_string_get(input_file,offset,buffer,buffer_length);
}

/* helf_relocation_howto_begin */
static const helf_arch_relocation_howto_t helf_riscv32_reloc_howto[]=
{
    {0x00000000u,0,0,0,0,0x0040,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000001u,4,32,0,0,0x0000,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000002u,8,64,0,0,0x0000,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000003u,4,32,0,0,0x0080,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000004u,0,0,0,0,0x0180,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000005u,8,64,0,0,0x0140,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000006u,4,32,0,0,0x01C0,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000007u,8,64,0,0,0x01C0,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000008u,4,32,0,0,0x01C8,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000009u,8,64,0,0,0x01C8,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x0000000Au,4,32,0,0,0x01C0,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x0000000Bu,8,64,0,0,0x01C0,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x0000000Cu,0,0,0,0,0x01C0,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000010u,4,32,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000011u,4,32,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000012u,8,64,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000013u,8,64,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000014u,4,32,0,0,0x0201,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000015u,4,32,0,0,0x03C1,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000016u,4,32,0,0,0x01C1,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000017u,4,32,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000018u,4,32,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000019u,4,32,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Au,4,32,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Bu,4,32,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Cu,4,32,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Du,4,32,0,0,0x01C8,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Eu,4,32,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Fu,4,32,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000020u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000021u,1,8,0,0,0x0010,0x0000000000000000ULL,0x00000000000000FFULL},
    {0x00000022u,2,16,0,0,0x0010,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000023u,4,32,0,0,0x0010,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000024u,8,64,0,0,0x0010,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000025u,1,8,0,0,0x0010,0x0000000000000000ULL,0x00000000000000FFULL},
    {0x00000026u,2,16,0,0,0x0010,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000027u,4,32,0,0,0x0010,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000028u,8,64,0,0,0x0010,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x0000002Bu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000002Cu,2,16,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000002Du,2,16,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000002Eu,2,16,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000002Fu,4,32,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000030u,4,32,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000031u,4,32,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000032u,4,32,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000033u,0,0,0,0,0x0040,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000034u,1,8,0,0,0x0010,0x0000000000000000ULL,0x000000000000003FULL},
    {0x00000035u,1,8,0,0,0x0000,0x0000000000000000ULL,0x000000000000003FULL},
    {0x00000036u,1,8,0,0,0x0000,0x0000000000000000ULL,0x00000000000000FFULL},
    {0x00000037u,2,16,0,0,0x0000,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000038u,4,32,0,0,0x0000,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000039u,4,32,0,0,0x0001,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x0000003Au,4,32,0,0,0x0080,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x0000003Cu,0,0,0,0,0x0010,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000003Du,0,0,0,0,0x0010,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000003Eu,4,32,0,0,0x01C9,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000003Fu,4,32,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000040u,4,32,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000041u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
};

bool helf_riscv32_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto)
{
    if(howto==NULL)
    {
        return false;
    }
    size_t count=sizeof(helf_riscv32_reloc_howto)/sizeof(helf_riscv32_reloc_howto[0]);
    if(count==0)
    {
        return false;
    }
    for(size_t i=0;i<count;i++)
    {
        if(helf_riscv32_reloc_howto[i].type==type)
        {
            *howto=helf_riscv32_reloc_howto[i];
            return true;
        }
    }
    return false;
}
/* helf_relocation_howto_end */



bool helf_riscv32_relocation_apply(uint32_t type,uint8_t *target,uintptr_t place,uintptr_t symbol_address,int64_t addend,const helf_arch_relocation_context_t *context)
{
    bool big_endian=(context!=NULL)?context->big_endian:false;
    uint64_t value=(uint64_t)(symbol_address+(uintptr_t)addend);
    switch(type)
    {
    case 26:
        {
            uint32_t insn=helf_arch_read32(target,big_endian);
            uint32_t hi=(uint32_t)((value+0x800U)>>12)&0xFFFFF;
            helf_arch_write32(target,(insn&0xFFFU)|(hi<<12),big_endian);
        }
        return true;
    case 27:
        {
            uint32_t insn=helf_arch_read32(target,big_endian);
            uint32_t lo=(uint32_t)value&0xFFF;
            helf_arch_write32(target,(insn&0xFFFFFU)|(lo<<20),big_endian);
        }
        return true;
    case 28:
        {
            uint32_t insn=helf_arch_read32(target,big_endian);
            uint32_t lo=(uint32_t)value&0xFFF;
            insn=(insn&~0xFE000F80U)|(((lo>>5)&0x7F)<<25)|((lo&0x1F)<<7);
            helf_arch_write32(target,insn,big_endian);
        }
        return true;
    case 23:
        {
            uint64_t rel=(uint64_t)((int64_t)value-(int64_t)place);
            uint32_t insn=helf_arch_read32(target,big_endian);
            uint32_t hi=(uint32_t)((rel+0x800U)>>12)&0xFFFFF;
            helf_arch_write32(target,(insn&0xFFFU)|(hi<<12),big_endian);
        }
        return true;
    case 18:
    case 19:
        {
            int64_t rel=(int64_t)value-(int64_t)place;
            uint32_t hi=(uint32_t)((uint64_t)(rel+0x800)>>12)&0xFFFFF;
            uint32_t lo=(uint32_t)rel&0xFFF;
            uint32_t insn0=helf_arch_read32(target,big_endian);
            uint32_t insn1=helf_arch_read32(target+4,big_endian);
            helf_arch_write32(target,(insn0&0xFFFU)|(hi<<12),big_endian);
            helf_arch_write32(target+4,(insn1&0xFFFFFU)|(lo<<20),big_endian);
        }
        return true;
    case 17:
        {
            int64_t rel=(int64_t)value-(int64_t)place;
            uint64_t imm=(uint64_t)rel;
            uint32_t insn=helf_arch_read32(target,big_endian);
            insn&=0xFFFU;
            insn|=((imm>>20)&1U)<<31;
            insn|=((imm>>1)&0x3FFU)<<21;
            insn|=((imm>>11)&1U)<<20;
            insn|=((imm>>12)&0xFFU)<<12;
            helf_arch_write32(target,insn,big_endian);
        }
        return true;
    case 16:
        {
            int64_t rel=(int64_t)value-(int64_t)place;
            uint64_t imm=(uint64_t)rel;
            uint32_t insn=helf_arch_read32(target,big_endian);
            insn&=~0xFE000F80U;
            insn|=((imm>>12)&1U)<<31;
            insn|=((imm>>5)&0x3FU)<<25;
            insn|=((imm>>1)&0xFU)<<8;
            insn|=((imm>>11)&1U)<<7;
            helf_arch_write32(target,insn,big_endian);
        }
        return true;
    default:
        return false;
    }
}
