/***************************************************************
 * Name:      helf_arch_m68k.c
 * Purpose:   实现helf_arch_m68k接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "helf_arch_m68k.h"

static const uint16_t helf_m68k_machines[]=
{
    HELF_EM_68K,
};

bool helf_m68k_is_m68k(helf_file_input_t *input_file)
{
    return helf_arch_is(input_file,helf_m68k_machines,sizeof(helf_m68k_machines)/sizeof(helf_m68k_machines[0]),HELF_ELFCLASS_32);
}

bool helf_m68k_is_big_endian(helf_file_input_t *input_file)
{
    if(!helf_m68k_is_m68k(input_file))
    {
        return false;
    }
    return helf_file_input_is_big_endian(input_file);
}

bool helf_m68k_is_executable(helf_file_input_t *input_file)
{
    return helf_arch_is_executable(input_file,helf_m68k_machines,sizeof(helf_m68k_machines)/sizeof(helf_m68k_machines[0]),HELF_ELFCLASS_32);
}

bool helf_m68k_entry_get(helf_file_input_t *input_file,uint32_t *entry)
{
    if(entry==NULL)
    {
        return false;
    }
    if(!helf_m68k_is_m68k(input_file))
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

size_t helf_m68k_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_m68k_is_m68k(input_file))
    {
        return 0;
    }
    return helf_arch_section_count_get(input_file);
}

bool helf_m68k_section_info_get(helf_file_input_t *input_file,size_t index,helf_m68k_section_info_t *info)
{
    if(!helf_m68k_is_m68k(input_file))
    {
        return false;
    }
    return helf_arch_section_info_get(input_file,index,info);
}

bool helf_m68k_is_rx_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_m68k_is_m68k(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_section(index,input_file);
}

intptr_t helf_m68k_rx_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_m68k_is_rx_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rx_section_offset_get(index,input_file);
}

size_t helf_m68k_rx_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_m68k_is_m68k(input_file))
    {
        return 0;
    }
    return helf_arch_rx_section_size_get(input_file);
}

bool helf_m68k_is_ro_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_m68k_is_m68k(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_section(index,input_file);
}

intptr_t helf_m68k_ro_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_m68k_is_ro_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_ro_section_offset_get(index,input_file);
}

size_t helf_m68k_ro_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_m68k_is_m68k(input_file))
    {
        return 0;
    }
    return helf_arch_ro_section_size_get(input_file);
}

bool helf_m68k_is_rw_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_m68k_is_m68k(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_section(index,input_file);
}

intptr_t helf_m68k_rw_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_m68k_is_rw_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rw_section_offset_get(index,input_file);
}

size_t helf_m68k_rw_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_m68k_is_m68k(input_file))
    {
        return 0;
    }
    return helf_arch_rw_section_size_get(input_file);
}

bool helf_m68k_is_bss_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_m68k_is_m68k(input_file))
    {
        return false;
    }
    return helf_arch_is_bss_section(index,input_file);
}

size_t helf_m68k_program_header_count_get(helf_file_input_t *input_file)
{
    if(!helf_m68k_is_m68k(input_file))
    {
        return 0;
    }
    return helf_arch_program_header_count_get(input_file);
}

bool helf_m68k_program_header_get(helf_file_input_t *input_file,size_t index,helf_elf32_program_header_t *programheader)
{
    if(programheader==NULL)
    {
        return false;
    }
    if(!helf_m68k_is_m68k(input_file))
    {
        return false;
    }
    if(index>=helf_arch_program_header_count_get(input_file))
    {
        return false;
    }
    return helf_file_input_32_bits_program_header_get(input_file,index,programheader);
}

size_t helf_m68k_segment_count_get(helf_file_input_t *input_file)
{
    return helf_m68k_program_header_count_get(input_file);
}

bool helf_m68k_segment_info_get(helf_file_input_t *input_file,size_t index,helf_m68k_segment_info_t *info)
{
    if(!helf_m68k_is_m68k(input_file))
    {
        return false;
    }
    return helf_arch_segment_info_get(input_file,index,info);
}

bool helf_m68k_is_load_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_m68k_is_m68k(input_file))
    {
        return false;
    }
    return helf_arch_is_load_segment(index,input_file);
}

bool helf_m68k_is_rx_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_m68k_is_m68k(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_segment(index,input_file);
}

bool helf_m68k_is_ro_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_m68k_is_m68k(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_segment(index,input_file);
}

bool helf_m68k_is_rw_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_m68k_is_m68k(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_segment(index,input_file);
}

size_t helf_m68k_load_segment_count_get(helf_file_input_t *input_file)
{
    if(!helf_m68k_is_m68k(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_count_get(input_file);
}

bool helf_m68k_load_segment_info_get(helf_file_input_t *input_file,size_t index,helf_m68k_segment_info_t *info)
{
    if(!helf_m68k_is_m68k(input_file))
    {
        return false;
    }
    return helf_arch_load_segment_info_get(input_file,index,info);
}

size_t helf_m68k_load_segment_total_filesz_get(helf_file_input_t *input_file)
{
    if(!helf_m68k_is_m68k(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_filesz_get(input_file);
}

size_t helf_m68k_load_segment_total_memsz_get(helf_file_input_t *input_file)
{
    if(!helf_m68k_is_m68k(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_memsz_get(input_file);
}

size_t helf_m68k_relocation_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_m68k_is_m68k(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_section_count_get(input_file);
}

bool helf_m68k_relocation_section_info_get(helf_file_input_t *input_file,size_t index,helf_arch_relocation_section_info_t *info)
{
    if(!helf_m68k_is_m68k(input_file))
    {
        return false;
    }
    return helf_arch_relocation_section_info_get(input_file,index,info);
}

size_t helf_m68k_relocation_count_get(helf_file_input_t *input_file,size_t section_index)
{
    if(!helf_m68k_is_m68k(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_count_get(input_file,section_index);
}

bool helf_m68k_relocation_get(helf_file_input_t *input_file,size_t section_index,size_t index,helf_arch_relocation_info_t *info)
{
    if(!helf_m68k_is_m68k(input_file))
    {
        return false;
    }
    if(!helf_arch_relocation_get(input_file,section_index,index,info))
    {
        return false;
    }
    return true;
}

bool helf_m68k_relocation_symbol_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_symbol_info_t *info)
{
    if(!helf_m68k_is_m68k(input_file))
    {
        return false;
    }
    return helf_arch_relocation_symbol_info_get(input_file,relocation,info);
}

static const helf_arch_relocation_type_name_t helf_m68k_relocation_type_names[]=
{
    {0,0,"R_68K_NONE"},
    {1,4,"R_68K_32"},
    {2,2,"R_68K_16"},
    {3,1,"R_68K_8"},
    {4,4,"R_68K_PC32"},
    {5,2,"R_68K_PC16"},
    {6,1,"R_68K_PC8"},
    {7,4,"R_68K_GOT32"},
    {8,2,"R_68K_GOT16"},
    {9,1,"R_68K_GOT8"},
    {10,4,"R_68K_GOT32O"},
    {11,2,"R_68K_GOT16O"},
    {12,1,"R_68K_GOT8O"},
    {13,4,"R_68K_PLT32"},
    {14,2,"R_68K_PLT16"},
    {15,1,"R_68K_PLT8"},
    {16,4,"R_68K_PLT32O"},
    {17,2,"R_68K_PLT16O"},
    {18,1,"R_68K_PLT8O"},
    {19,0,"R_68K_COPY"},
    {20,4,"R_68K_GLOB_DAT"},
    {21,4,"R_68K_JMP_SLOT"},
    {22,4,"R_68K_RELATIVE"},
    {23,4,"R_68K_GNU_VTINHERIT"},
    {24,4,"R_68K_GNU_VTENTRY"},
    {25,4,"R_68K_TLS_GD32"},
    {26,2,"R_68K_TLS_GD16"},
    {27,1,"R_68K_TLS_GD8"},
    {28,4,"R_68K_TLS_LDM32"},
    {29,2,"R_68K_TLS_LDM16"},
    {30,1,"R_68K_TLS_LDM8"},
    {31,4,"R_68K_TLS_LDO32"},
    {32,2,"R_68K_TLS_LDO16"},
    {33,1,"R_68K_TLS_LDO8"},
    {34,4,"R_68K_TLS_IE32"},
    {35,2,"R_68K_TLS_IE16"},
    {36,1,"R_68K_TLS_IE8"},
    {37,4,"R_68K_TLS_LE32"},
    {38,2,"R_68K_TLS_LE16"},
    {39,1,"R_68K_TLS_LE8"},
    {40,4,"R_68K_TLS_DTPMOD32"},
    {41,4,"R_68K_TLS_DTPREL32"},
    {42,4,"R_68K_TLS_TPREL32"},
};

const char *helf_m68k_relocation_type_name_get(uint32_t type)
{
    return helf_arch_relocation_type_name_get(helf_m68k_relocation_type_names,sizeof(helf_m68k_relocation_type_names)/sizeof(helf_m68k_relocation_type_names[0]),type);
}

uint8_t helf_m68k_relocation_size_get(uint32_t type)
{
    return helf_arch_relocation_type_size_get(helf_m68k_relocation_type_names,sizeof(helf_m68k_relocation_type_names)/sizeof(helf_m68k_relocation_type_names[0]),type);
}

const char *helf_m68k_section_type_name_get(uint32_t type)
{
    (void)type;
    return NULL;
}

bool helf_m68k_relocation_address_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_relocation_address_info_t *info)
{
    if(!helf_m68k_is_m68k(input_file) || relocation==NULL)
    {
        return false;
    }
    uint8_t size=helf_m68k_relocation_size_get(relocation->type);
    if(size==0)
    {
        return false;
    }
    return helf_arch_relocation_address_info_get(input_file,relocation,size,info);
}

size_t helf_m68k_relocation_data_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,void *buffer,size_t buffer_length)
{
    if(!helf_m68k_is_m68k(input_file) || relocation==NULL)
    {
        return 0;
    }
    uint8_t size=helf_m68k_relocation_size_get(relocation->type);
    if(size==0)
    {
        return 0;
    }
    return helf_arch_relocation_data_get(input_file,relocation,size,buffer,buffer_length);
}

bool helf_m68k_is_shared_object(helf_file_input_t *input_file)
{
    return helf_arch_is_shared_object(input_file,helf_m68k_machines,sizeof(helf_m68k_machines)/sizeof(helf_m68k_machines[0]),HELF_ELFCLASS_32);
}

bool helf_m68k_is_relocatable(helf_file_input_t *input_file)
{
    return helf_arch_is_relocatable(input_file,helf_m68k_machines,sizeof(helf_m68k_machines)/sizeof(helf_m68k_machines[0]),HELF_ELFCLASS_32);
}

size_t helf_m68k_dynamic_count_get(helf_file_input_t *input_file)
{
    if(!helf_m68k_is_m68k(input_file))
    {
        return 0;
    }
    return helf_arch_dynamic_count_get(input_file);
}

bool helf_m68k_dynamic_info_get(helf_file_input_t *input_file,size_t index,helf_arch_dynamic_info_t *info)
{
    if(!helf_m68k_is_m68k(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_info_get(input_file,index,info);
}

bool helf_m68k_dynamic_string_get(helf_file_input_t *input_file,uint64_t offset,char *buffer,size_t buffer_length)
{
    if(!helf_m68k_is_m68k(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_string_get(input_file,offset,buffer,buffer_length);
}
