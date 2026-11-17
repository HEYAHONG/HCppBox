/***************************************************************
 * Name:      helf_arch_m32r.c
 * Purpose:   实现helf_arch_m32r接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "helf_arch_m32r.h"

static const uint16_t helf_m32r_machines[]=
{
    HELF_EM_M32R,
    HELF_EM_CYGNUS_M32R,
};

bool helf_m32r_is_m32r(helf_file_input_t *input_file)
{
    return helf_arch_is(input_file,helf_m32r_machines,sizeof(helf_m32r_machines)/sizeof(helf_m32r_machines[0]),HELF_ELFCLASS_32);
}

bool helf_m32r_is_big_endian(helf_file_input_t *input_file)
{
    if(!helf_m32r_is_m32r(input_file))
    {
        return false;
    }
    return helf_file_input_is_big_endian(input_file);
}

bool helf_m32r_is_executable(helf_file_input_t *input_file)
{
    return helf_arch_is_executable(input_file,helf_m32r_machines,sizeof(helf_m32r_machines)/sizeof(helf_m32r_machines[0]),HELF_ELFCLASS_32);
}

bool helf_m32r_entry_get(helf_file_input_t *input_file,uint32_t *entry)
{
    if(entry==NULL)
    {
        return false;
    }
    if(!helf_m32r_is_m32r(input_file))
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

size_t helf_m32r_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_m32r_is_m32r(input_file))
    {
        return 0;
    }
    return helf_arch_section_count_get(input_file);
}

bool helf_m32r_section_info_get(helf_file_input_t *input_file,size_t index,helf_m32r_section_info_t *info)
{
    if(!helf_m32r_is_m32r(input_file))
    {
        return false;
    }
    return helf_arch_section_info_get(input_file,index,info);
}

bool helf_m32r_is_rx_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_m32r_is_m32r(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_section(index,input_file);
}

intptr_t helf_m32r_rx_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_m32r_is_rx_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rx_section_offset_get(index,input_file);
}

size_t helf_m32r_rx_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_m32r_is_m32r(input_file))
    {
        return 0;
    }
    return helf_arch_rx_section_size_get(input_file);
}

bool helf_m32r_is_ro_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_m32r_is_m32r(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_section(index,input_file);
}

intptr_t helf_m32r_ro_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_m32r_is_ro_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_ro_section_offset_get(index,input_file);
}

size_t helf_m32r_ro_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_m32r_is_m32r(input_file))
    {
        return 0;
    }
    return helf_arch_ro_section_size_get(input_file);
}

bool helf_m32r_is_rw_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_m32r_is_m32r(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_section(index,input_file);
}

intptr_t helf_m32r_rw_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_m32r_is_rw_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rw_section_offset_get(index,input_file);
}

size_t helf_m32r_rw_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_m32r_is_m32r(input_file))
    {
        return 0;
    }
    return helf_arch_rw_section_size_get(input_file);
}

bool helf_m32r_is_bss_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_m32r_is_m32r(input_file))
    {
        return false;
    }
    return helf_arch_is_bss_section(index,input_file);
}

size_t helf_m32r_program_header_count_get(helf_file_input_t *input_file)
{
    if(!helf_m32r_is_m32r(input_file))
    {
        return 0;
    }
    return helf_arch_program_header_count_get(input_file);
}

bool helf_m32r_program_header_get(helf_file_input_t *input_file,size_t index,helf_elf32_program_header_t *programheader)
{
    if(programheader==NULL)
    {
        return false;
    }
    if(!helf_m32r_is_m32r(input_file))
    {
        return false;
    }
    if(index>=helf_arch_program_header_count_get(input_file))
    {
        return false;
    }
    return helf_file_input_32_bits_program_header_get(input_file,index,programheader);
}

size_t helf_m32r_segment_count_get(helf_file_input_t *input_file)
{
    return helf_m32r_program_header_count_get(input_file);
}

bool helf_m32r_segment_info_get(helf_file_input_t *input_file,size_t index,helf_m32r_segment_info_t *info)
{
    if(!helf_m32r_is_m32r(input_file))
    {
        return false;
    }
    return helf_arch_segment_info_get(input_file,index,info);
}

bool helf_m32r_is_load_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_m32r_is_m32r(input_file))
    {
        return false;
    }
    return helf_arch_is_load_segment(index,input_file);
}

bool helf_m32r_is_rx_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_m32r_is_m32r(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_segment(index,input_file);
}

bool helf_m32r_is_ro_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_m32r_is_m32r(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_segment(index,input_file);
}

bool helf_m32r_is_rw_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_m32r_is_m32r(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_segment(index,input_file);
}

size_t helf_m32r_load_segment_count_get(helf_file_input_t *input_file)
{
    if(!helf_m32r_is_m32r(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_count_get(input_file);
}

bool helf_m32r_load_segment_info_get(helf_file_input_t *input_file,size_t index,helf_m32r_segment_info_t *info)
{
    if(!helf_m32r_is_m32r(input_file))
    {
        return false;
    }
    return helf_arch_load_segment_info_get(input_file,index,info);
}

size_t helf_m32r_load_segment_total_filesz_get(helf_file_input_t *input_file)
{
    if(!helf_m32r_is_m32r(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_filesz_get(input_file);
}

size_t helf_m32r_load_segment_total_memsz_get(helf_file_input_t *input_file)
{
    if(!helf_m32r_is_m32r(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_memsz_get(input_file);
}

size_t helf_m32r_relocation_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_m32r_is_m32r(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_section_count_get(input_file);
}

bool helf_m32r_relocation_section_info_get(helf_file_input_t *input_file,size_t index,helf_arch_relocation_section_info_t *info)
{
    if(!helf_m32r_is_m32r(input_file))
    {
        return false;
    }
    return helf_arch_relocation_section_info_get(input_file,index,info);
}

size_t helf_m32r_relocation_count_get(helf_file_input_t *input_file,size_t section_index)
{
    if(!helf_m32r_is_m32r(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_count_get(input_file,section_index);
}

bool helf_m32r_relocation_get(helf_file_input_t *input_file,size_t section_index,size_t index,helf_arch_relocation_info_t *info)
{
    if(!helf_m32r_is_m32r(input_file))
    {
        return false;
    }
    if(!helf_arch_relocation_get(input_file,section_index,index,info))
    {
        return false;
    }
    return true;
}

bool helf_m32r_relocation_symbol_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_symbol_info_t *info)
{
    if(!helf_m32r_is_m32r(input_file))
    {
        return false;
    }
    return helf_arch_relocation_symbol_info_get(input_file,relocation,info);
}

static const helf_arch_relocation_type_name_t helf_m32r_relocation_type_names[]=
{
    {0,0,"R_M32R_NONE"},
    {1,2,"R_M32R_16"},
    {2,4,"R_M32R_32"},
    {3,4,"R_M32R_24"},
    {4,2,"R_M32R_10_PCREL"},
    {5,4,"R_M32R_18_PCREL"},
    {6,4,"R_M32R_26_PCREL"},
    {7,4,"R_M32R_HI16_ULO"},
    {8,4,"R_M32R_HI16_SLO"},
    {9,4,"R_M32R_LO16"},
    {10,4,"R_M32R_SDA16"},
    {11,4,"R_M32R_GNU_VTINHERIT"},
    {12,4,"R_M32R_GNU_VTENTRY"},
    {33,2,"R_M32R_16_RELA"},
    {34,4,"R_M32R_32_RELA"},
    {35,4,"R_M32R_24_RELA"},
    {36,2,"R_M32R_10_PCREL_RELA"},
    {37,4,"R_M32R_18_PCREL_RELA"},
    {38,4,"R_M32R_26_PCREL_RELA"},
    {39,4,"R_M32R_HI16_ULO_RELA"},
    {40,4,"R_M32R_HI16_SLO_RELA"},
    {41,4,"R_M32R_LO16_RELA"},
    {42,4,"R_M32R_SDA16_RELA"},
    {43,4,"R_M32R_RELA_GNU_VTINHERIT"},
    {44,4,"R_M32R_RELA_GNU_VTENTRY"},
    {45,4,"R_M32R_REL32"},
    {48,4,"R_M32R_GOT24"},
    {49,4,"R_M32R_26_PLTREL"},
    {50,4,"R_M32R_COPY"},
    {51,4,"R_M32R_GLOB_DAT"},
    {52,4,"R_M32R_JMP_SLOT"},
    {53,4,"R_M32R_RELATIVE"},
    {54,4,"R_M32R_GOTOFF"},
    {55,4,"R_M32R_GOTPC24"},
    {56,4,"R_M32R_GOT16_HI_ULO"},
    {57,4,"R_M32R_GOT16_HI_SLO"},
    {58,4,"R_M32R_GOT16_LO"},
    {59,4,"R_M32R_GOTPC_HI_ULO"},
    {60,4,"R_M32R_GOTPC_HI_SLO"},
    {61,4,"R_M32R_GOTPC_LO"},
    {62,4,"R_M32R_GOTOFF_HI_ULO"},
    {63,4,"R_M32R_GOTOFF_HI_SLO"},
    {64,4,"R_M32R_GOTOFF_LO"},
};

const char *helf_m32r_relocation_type_name_get(uint32_t type)
{
    return helf_arch_relocation_type_name_get(helf_m32r_relocation_type_names,sizeof(helf_m32r_relocation_type_names)/sizeof(helf_m32r_relocation_type_names[0]),type);
}

uint8_t helf_m32r_relocation_size_get(uint32_t type)
{
    return helf_arch_relocation_type_size_get(helf_m32r_relocation_type_names,sizeof(helf_m32r_relocation_type_names)/sizeof(helf_m32r_relocation_type_names[0]),type);
}

const char *helf_m32r_section_type_name_get(uint32_t type)
{
    (void)type;
    return NULL;
}

bool helf_m32r_relocation_address_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_relocation_address_info_t *info)
{
    if(!helf_m32r_is_m32r(input_file) || relocation==NULL)
    {
        return false;
    }
    uint8_t size=helf_m32r_relocation_size_get(relocation->type);
    if(size==0)
    {
        return false;
    }
    return helf_arch_relocation_address_info_get(input_file,relocation,size,info);
}

size_t helf_m32r_relocation_data_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,void *buffer,size_t buffer_length)
{
    if(!helf_m32r_is_m32r(input_file) || relocation==NULL)
    {
        return 0;
    }
    uint8_t size=helf_m32r_relocation_size_get(relocation->type);
    if(size==0)
    {
        return 0;
    }
    return helf_arch_relocation_data_get(input_file,relocation,size,buffer,buffer_length);
}

bool helf_m32r_is_shared_object(helf_file_input_t *input_file)
{
    return helf_arch_is_shared_object(input_file,helf_m32r_machines,sizeof(helf_m32r_machines)/sizeof(helf_m32r_machines[0]),HELF_ELFCLASS_32);
}

bool helf_m32r_is_relocatable(helf_file_input_t *input_file)
{
    return helf_arch_is_relocatable(input_file,helf_m32r_machines,sizeof(helf_m32r_machines)/sizeof(helf_m32r_machines[0]),HELF_ELFCLASS_32);
}

size_t helf_m32r_dynamic_count_get(helf_file_input_t *input_file)
{
    if(!helf_m32r_is_m32r(input_file))
    {
        return 0;
    }
    return helf_arch_dynamic_count_get(input_file);
}

bool helf_m32r_dynamic_info_get(helf_file_input_t *input_file,size_t index,helf_arch_dynamic_info_t *info)
{
    if(!helf_m32r_is_m32r(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_info_get(input_file,index,info);
}

bool helf_m32r_dynamic_string_get(helf_file_input_t *input_file,uint64_t offset,char *buffer,size_t buffer_length)
{
    if(!helf_m32r_is_m32r(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_string_get(input_file,offset,buffer,buffer_length);
}
