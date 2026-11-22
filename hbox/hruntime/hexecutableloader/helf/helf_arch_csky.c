/***************************************************************
 * Name:      helf_arch_csky.c
 * Purpose:   实现helf_arch_csky接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "helf_arch_csky.h"

static const uint16_t helf_csky_machines[]=
{
    HELF_EM_CSKY,
    HELF_EM_CSKY_OLD,
};

bool helf_csky_is_csky(helf_file_input_t *input_file)
{
    return helf_arch_is(input_file,helf_csky_machines,sizeof(helf_csky_machines)/sizeof(helf_csky_machines[0]),HELF_ELFCLASS_32);
}

bool helf_csky_is_big_endian(helf_file_input_t *input_file)
{
    if(!helf_csky_is_csky(input_file))
    {
        return false;
    }
    return helf_file_input_is_big_endian(input_file);
}

bool helf_csky_is_executable(helf_file_input_t *input_file)
{
    return helf_arch_is_executable(input_file,helf_csky_machines,sizeof(helf_csky_machines)/sizeof(helf_csky_machines[0]),HELF_ELFCLASS_32);
}

bool helf_csky_entry_get(helf_file_input_t *input_file,uint32_t *entry)
{
    if(entry==NULL)
    {
        return false;
    }
    if(!helf_csky_is_csky(input_file))
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

size_t helf_csky_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_csky_is_csky(input_file))
    {
        return 0;
    }
    return helf_arch_section_count_get(input_file);
}

bool helf_csky_section_info_get(helf_file_input_t *input_file,size_t index,helf_csky_section_info_t *info)
{
    if(!helf_csky_is_csky(input_file))
    {
        return false;
    }
    return helf_arch_section_info_get(input_file,index,info);
}

bool helf_csky_is_rx_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_csky_is_csky(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_section(index,input_file);
}

intptr_t helf_csky_rx_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_csky_is_rx_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rx_section_offset_get(index,input_file);
}

size_t helf_csky_rx_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_csky_is_csky(input_file))
    {
        return 0;
    }
    return helf_arch_rx_section_size_get(input_file);
}

bool helf_csky_is_ro_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_csky_is_csky(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_section(index,input_file);
}

intptr_t helf_csky_ro_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_csky_is_ro_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_ro_section_offset_get(index,input_file);
}

size_t helf_csky_ro_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_csky_is_csky(input_file))
    {
        return 0;
    }
    return helf_arch_ro_section_size_get(input_file);
}

bool helf_csky_is_rw_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_csky_is_csky(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_section(index,input_file);
}

intptr_t helf_csky_rw_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_csky_is_rw_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rw_section_offset_get(index,input_file);
}

size_t helf_csky_rw_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_csky_is_csky(input_file))
    {
        return 0;
    }
    return helf_arch_rw_section_size_get(input_file);
}

bool helf_csky_is_bss_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_csky_is_csky(input_file))
    {
        return false;
    }
    return helf_arch_is_bss_section(index,input_file);
}

size_t helf_csky_program_header_count_get(helf_file_input_t *input_file)
{
    if(!helf_csky_is_csky(input_file))
    {
        return 0;
    }
    return helf_arch_program_header_count_get(input_file);
}

bool helf_csky_program_header_get(helf_file_input_t *input_file,size_t index,helf_elf32_program_header_t *programheader)
{
    if(programheader==NULL)
    {
        return false;
    }
    if(!helf_csky_is_csky(input_file))
    {
        return false;
    }
    if(index>=helf_arch_program_header_count_get(input_file))
    {
        return false;
    }
    return helf_file_input_32_bits_program_header_get(input_file,index,programheader);
}

size_t helf_csky_segment_count_get(helf_file_input_t *input_file)
{
    return helf_csky_program_header_count_get(input_file);
}

bool helf_csky_segment_info_get(helf_file_input_t *input_file,size_t index,helf_csky_segment_info_t *info)
{
    if(!helf_csky_is_csky(input_file))
    {
        return false;
    }
    return helf_arch_segment_info_get(input_file,index,info);
}

bool helf_csky_is_load_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_csky_is_csky(input_file))
    {
        return false;
    }
    return helf_arch_is_load_segment(index,input_file);
}

bool helf_csky_is_rx_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_csky_is_csky(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_segment(index,input_file);
}

bool helf_csky_is_ro_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_csky_is_csky(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_segment(index,input_file);
}

bool helf_csky_is_rw_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_csky_is_csky(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_segment(index,input_file);
}

size_t helf_csky_load_segment_count_get(helf_file_input_t *input_file)
{
    if(!helf_csky_is_csky(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_count_get(input_file);
}

bool helf_csky_load_segment_info_get(helf_file_input_t *input_file,size_t index,helf_csky_segment_info_t *info)
{
    if(!helf_csky_is_csky(input_file))
    {
        return false;
    }
    return helf_arch_load_segment_info_get(input_file,index,info);
}

size_t helf_csky_load_segment_total_filesz_get(helf_file_input_t *input_file)
{
    if(!helf_csky_is_csky(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_filesz_get(input_file);
}

size_t helf_csky_load_segment_total_memsz_get(helf_file_input_t *input_file)
{
    if(!helf_csky_is_csky(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_memsz_get(input_file);
}

size_t helf_csky_relocation_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_csky_is_csky(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_section_count_get(input_file);
}

bool helf_csky_relocation_section_info_get(helf_file_input_t *input_file,size_t index,helf_arch_relocation_section_info_t *info)
{
    if(!helf_csky_is_csky(input_file))
    {
        return false;
    }
    return helf_arch_relocation_section_info_get(input_file,index,info);
}

size_t helf_csky_relocation_count_get(helf_file_input_t *input_file,size_t section_index)
{
    if(!helf_csky_is_csky(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_count_get(input_file,section_index);
}

bool helf_csky_relocation_get(helf_file_input_t *input_file,size_t section_index,size_t index,helf_arch_relocation_info_t *info)
{
    if(!helf_csky_is_csky(input_file))
    {
        return false;
    }
    if(!helf_arch_relocation_get(input_file,section_index,index,info))
    {
        return false;
    }
    return true;
}

bool helf_csky_relocation_symbol_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_symbol_info_t *info)
{
    if(!helf_csky_is_csky(input_file))
    {
        return false;
    }
    return helf_arch_relocation_symbol_info_get(input_file,relocation,info);
}

static const helf_arch_relocation_type_name_t helf_csky_relocation_type_names[]=
{
    {0,0,"R_CKCORE_NONE"},
    {1,4,"R_CKCORE_ADDR32"},
    {2,2,"R_CKCORE_PCREL_IMM8BY4"},
    {3,2,"R_CKCORE_PCREL_IMM11BY2"},
    {4,0,"R_CKCORE_PCREL_IMM4BY2"},
    {5,4,"R_CKCORE_PCREL32"},
    {6,2,"R_CKCORE_PCREL_JSR_IMM11BY2"},
    {7,4,"R_CKCORE_GNU_VTINHERIT"},
    {8,4,"R_CKCORE_GNU_VTENTRY"},
    {9,4,"R_CKCORE_RELATIVE"},
    {10,4,"R_CKCORE_COPY"},
    {11,0,"R_CKCORE_GLOB_DAT"},
    {12,0,"R_CKCORE_JUMP_SLOT"},
    {13,4,"R_CKCORE_GOTOFF"},
    {14,4,"R_CKCORE_GOTPC"},
    {15,4,"R_CKCORE_GOT32"},
    {16,4,"R_CKCORE_PLT32"},
    {17,0,"R_CKCORE_ADDRGOT"},
    {18,0,"R_CKCORE_ADDRPLT"},
    {19,4,"R_CKCORE_PCREL_IMM26BY2"},
    {20,4,"R_CKCORE_PCREL_IMM16BY2"},
    {21,4,"R_CKCORE_PCREL_IMM16BY4"},
    {22,2,"R_CKCORE_PCREL_IMM10BY2"},
    {23,4,"R_CKCORE_PCREL_IMM10BY4"},
    {24,4,"R_CKCORE_ADDR_HI16"},
    {25,4,"R_CKCORE_ADDR_LO16"},
    {26,4,"R_CKCORE_GOTPC_HI16"},
    {27,4,"R_CKCORE_GOTPC_LO16"},
    {28,4,"R_CKCORE_GOTOFF_HI16"},
    {29,4,"R_CKCORE_GOTOFF_LO16"},
    {30,4,"R_CKCORE_GOT12"},
    {31,4,"R_CKCORE_GOT_HI16"},
    {32,4,"R_CKCORE_GOT_LO16"},
    {33,4,"R_CKCORE_PLT12"},
    {34,4,"R_CKCORE_PLT_HI16"},
    {35,4,"R_CKCORE_PLT_LO16"},
    {36,0,"R_CKCORE_ADDRGOT_HI16"},
    {37,0,"R_CKCORE_ADDRGOT_LO16"},
    {38,0,"R_CKCORE_ADDRPLT_HI16"},
    {39,0,"R_CKCORE_ADDRPLT_LO16"},
    {40,4,"R_CKCORE_PCREL_JSR_IMM26BY2"},
    {41,4,"R_CKCORE_TOFFSET_LO16"},
    {42,4,"R_CKCORE_DOFFSET_LO16"},
    {43,4,"R_CKCORE_PCREL_IMM18BY2"},
    {44,4,"R_CKCORE_DOFFSET_IMM18"},
    {45,4,"R_CKCORE_DOFFSET_IMM18BY2"},
    {46,4,"R_CKCORE_DOFFSET_IMM18BY4"},
    {47,4,"R_CKCORE_GOTOFF_IMM18"},
    {48,4,"R_CKCORE_GOT_IMM18BY4"},
    {49,4,"R_CKCORE_PLT_IMM18BY4"},
    {50,2,"R_CKCORE_PCREL_IMM7BY4"},
    {51,4,"R_CKCORE_TLS_LE32"},
    {52,4,"R_CKCORE_TLS_IE32"},
    {53,4,"R_CKCORE_TLS_GD32"},
    {54,4,"R_CKCORE_TLS_LDM32"},
    {55,4,"R_CKCORE_TLS_LDO32"},
    {56,0,"R_CKCORE_TLS_DTPMOD32"},
    {57,0,"R_CKCORE_TLS_DTPOFF32"},
    {58,0,"R_CKCORE_TLS_TPOFF32"},
    {59,4,"R_CKCORE_PCREL_FLRW_IMM8BY4"},
    {60,4,"R_CKCORE_NOJSRI"},
    {61,0,"R_CKCORE_CALLGRAPH"},
    {62,0,"R_CKCORE_IRELATIVE"},
    {63,4,"R_CKCORE_PCREL_BLOOP_IMM4BY4"},
    {64,4,"R_CKCORE_PCREL_BLOOP_IMM12BY4"},
};

const char *helf_csky_relocation_type_name_get(uint32_t type)
{
    return helf_arch_relocation_type_name_get(helf_csky_relocation_type_names,sizeof(helf_csky_relocation_type_names)/sizeof(helf_csky_relocation_type_names[0]),type);
}

uint8_t helf_csky_relocation_size_get(uint32_t type)
{
    return helf_arch_relocation_type_size_get(helf_csky_relocation_type_names,sizeof(helf_csky_relocation_type_names)/sizeof(helf_csky_relocation_type_names[0]),type);
}

static const helf_arch_section_type_name_t helf_csky_section_type_names[]=
{
    {1879048193,"SHT_CSKY_ATTRIBUTES"},
};

const char *helf_csky_section_type_name_get(uint32_t type)
{
    return helf_arch_section_type_name_get(helf_csky_section_type_names,sizeof(helf_csky_section_type_names)/sizeof(helf_csky_section_type_names[0]),type);
}

bool helf_csky_relocation_address_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_relocation_address_info_t *info)
{
    if(!helf_csky_is_csky(input_file) || relocation==NULL)
    {
        return false;
    }
    uint8_t size=helf_csky_relocation_size_get(relocation->type);
    if(size==0)
    {
        return false;
    }
    return helf_arch_relocation_address_info_get(input_file,relocation,size,info);
}

size_t helf_csky_relocation_data_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,void *buffer,size_t buffer_length)
{
    if(!helf_csky_is_csky(input_file) || relocation==NULL)
    {
        return 0;
    }
    uint8_t size=helf_csky_relocation_size_get(relocation->type);
    if(size==0)
    {
        return 0;
    }
    return helf_arch_relocation_data_get(input_file,relocation,size,buffer,buffer_length);
}

bool helf_csky_is_shared_object(helf_file_input_t *input_file)
{
    return helf_arch_is_shared_object(input_file,helf_csky_machines,sizeof(helf_csky_machines)/sizeof(helf_csky_machines[0]),HELF_ELFCLASS_32);
}

bool helf_csky_is_relocatable(helf_file_input_t *input_file)
{
    return helf_arch_is_relocatable(input_file,helf_csky_machines,sizeof(helf_csky_machines)/sizeof(helf_csky_machines[0]),HELF_ELFCLASS_32);
}

size_t helf_csky_dynamic_count_get(helf_file_input_t *input_file)
{
    if(!helf_csky_is_csky(input_file))
    {
        return 0;
    }
    return helf_arch_dynamic_count_get(input_file);
}

bool helf_csky_dynamic_info_get(helf_file_input_t *input_file,size_t index,helf_arch_dynamic_info_t *info)
{
    if(!helf_csky_is_csky(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_info_get(input_file,index,info);
}

bool helf_csky_dynamic_string_get(helf_file_input_t *input_file,uint64_t offset,char *buffer,size_t buffer_length)
{
    if(!helf_csky_is_csky(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_string_get(input_file,offset,buffer,buffer_length);
}

/* helf_relocation_howto_begin */
static const helf_arch_relocation_howto_t helf_csky_reloc_howto[]=
{
    {0x00000000u,0,0,0,0,0x0040,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000001u,4,32,0,0,0x0000,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000002u,2,8,2,0,0x0003,0x00000000000000FFULL,0x00000000000000FFULL},
    {0x00000003u,2,11,1,0,0x0003,0x00000000000007FFULL,0x00000000000007FFULL},
    {0x00000004u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000005u,4,32,0,0,0x0003,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000006u,2,11,1,0,0x0003,0x00000000000007FFULL,0x00000000000007FFULL},
    {0x00000007u,4,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000008u,4,0,0,0,0x0010,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000009u,4,32,0,0,0x0088,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x0000000Au,4,32,0,0,0x0188,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x0000000Bu,0,0,0,0,0x0100,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000000Cu,0,0,0,0,0x0140,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000000Du,4,32,0,0,0x0208,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x0000000Eu,4,32,0,0,0x0209,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x0000000Fu,4,32,0,0,0x020A,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000010u,4,32,0,0,0x000A,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000011u,0,0,0,0,0x0200,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000012u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000013u,4,26,1,0,0x0003,0x0000000000000000ULL,0x0000000003FFFFFFULL},
    {0x00000014u,4,16,1,0,0x0003,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000015u,4,16,2,0,0x0003,0x00000000FFFF0000ULL,0x000000000000FFFFULL},
    {0x00000016u,2,10,1,0,0x0003,0x0000000000000000ULL,0x00000000000003FFULL},
    {0x00000017u,4,10,2,0,0x0003,0x0000000000000000ULL,0x00000000000003FFULL},
    {0x00000018u,4,16,16,0,0x0000,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000019u,4,16,0,0,0x0000,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000001Au,4,16,16,0,0x0201,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000001Bu,4,16,0,0,0x0201,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000001Cu,4,16,16,0,0x0200,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000001Du,4,16,0,0,0x0200,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000001Eu,4,12,2,0,0x0208,0x0000000000000000ULL,0x0000000000000FFFULL},
    {0x0000001Fu,4,16,16,0,0x0208,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000020u,4,16,0,0,0x0208,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000021u,4,12,2,0,0x0008,0x0000000000000000ULL,0x0000000000000FFFULL},
    {0x00000022u,4,16,16,0,0x0008,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000023u,4,16,0,0,0x0008,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000024u,0,0,0,0,0x0200,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000025u,0,0,0,0,0x0200,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000026u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000027u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000028u,4,26,1,0,0x0003,0x0000000000000000ULL,0x0000000003FFFFFFULL},
    {0x00000029u,4,16,0,0,0x0000,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000002Au,4,16,0,0,0x0000,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000002Bu,4,18,1,0,0x0003,0x0000000000000000ULL,0x000000000003FFFFULL},
    {0x0000002Cu,4,18,0,0,0x0000,0x0000000000000000ULL,0x000000000003FFFFULL},
    {0x0000002Du,4,18,1,0,0x0000,0x0000000000000000ULL,0x000000000003FFFFULL},
    {0x0000002Eu,4,18,2,0,0x0000,0x0000000000000000ULL,0x000000000003FFFFULL},
    {0x0000002Fu,4,18,0,0,0x0208,0x000000000000FFFCULL,0x000000000003FFFFULL},
    {0x00000030u,4,18,2,0,0x0208,0x000000000000FFFCULL,0x000000000003FFFFULL},
    {0x00000031u,4,18,2,0,0x000A,0x000000000000FFFCULL,0x000000000003FFFFULL},
    {0x00000032u,2,7,2,0,0x0003,0x000000000000EC1FULL,0x000000000000031FULL},
    {0x00000033u,4,32,0,0,0x01C2,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000034u,4,32,0,0,0x01C2,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000035u,4,32,0,0,0x01C2,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000036u,4,32,0,0,0x01C2,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000037u,4,32,0,0,0x01C2,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000038u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000039u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000003Au,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000003Bu,4,8,2,0,0x0003,0x00000000FE1FFF0FULL,0x0000000001E000F0ULL},
    {0x0000003Cu,4,32,0,0,0x0000,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x0000003Du,0,0,0,0,0x0002,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000003Eu,0,0,0,0,0x0080,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000003Fu,4,4,1,0,0x0003,0x0000000000000000ULL,0x000000000000000FULL},
    {0x00000040u,4,12,1,0,0x0003,0x0000000000000000ULL,0x0000000000000FFFULL},
};

bool helf_csky_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto)
{
    if(howto==NULL)
    {
        return false;
    }
    size_t count=sizeof(helf_csky_reloc_howto)/sizeof(helf_csky_reloc_howto[0]);
    if(count==0)
    {
        return false;
    }
    for(size_t i=0;i<count;i++)
    {
        if(helf_csky_reloc_howto[i].type==type)
        {
            *howto=helf_csky_reloc_howto[i];
            return true;
        }
    }
    return false;
}
/* helf_relocation_howto_end */

