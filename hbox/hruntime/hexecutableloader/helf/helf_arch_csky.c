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
