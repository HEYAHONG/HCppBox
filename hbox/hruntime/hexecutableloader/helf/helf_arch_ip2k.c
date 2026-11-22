/***************************************************************
 * Name:      helf_arch_ip2k.c
 * Purpose:   实现helf_arch_ip2k接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "helf_arch_ip2k.h"

static const uint16_t helf_ip2k_machines[]=
{
    HELF_EM_IP2K,
    HELF_EM_IP2K_OLD,
};

bool helf_ip2k_is_ip2k(helf_file_input_t *input_file)
{
    return helf_arch_is(input_file,helf_ip2k_machines,sizeof(helf_ip2k_machines)/sizeof(helf_ip2k_machines[0]),HELF_ELFCLASS_32);
}

bool helf_ip2k_is_big_endian(helf_file_input_t *input_file)
{
    if(!helf_ip2k_is_ip2k(input_file))
    {
        return false;
    }
    return helf_file_input_is_big_endian(input_file);
}

bool helf_ip2k_is_executable(helf_file_input_t *input_file)
{
    return helf_arch_is_executable(input_file,helf_ip2k_machines,sizeof(helf_ip2k_machines)/sizeof(helf_ip2k_machines[0]),HELF_ELFCLASS_32);
}

bool helf_ip2k_entry_get(helf_file_input_t *input_file,uint32_t *entry)
{
    if(entry==NULL)
    {
        return false;
    }
    if(!helf_ip2k_is_ip2k(input_file))
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

size_t helf_ip2k_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_ip2k_is_ip2k(input_file))
    {
        return 0;
    }
    return helf_arch_section_count_get(input_file);
}

bool helf_ip2k_section_info_get(helf_file_input_t *input_file,size_t index,helf_ip2k_section_info_t *info)
{
    if(!helf_ip2k_is_ip2k(input_file))
    {
        return false;
    }
    return helf_arch_section_info_get(input_file,index,info);
}

bool helf_ip2k_is_rx_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ip2k_is_ip2k(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_section(index,input_file);
}

intptr_t helf_ip2k_rx_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ip2k_is_rx_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rx_section_offset_get(index,input_file);
}

size_t helf_ip2k_rx_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_ip2k_is_ip2k(input_file))
    {
        return 0;
    }
    return helf_arch_rx_section_size_get(input_file);
}

bool helf_ip2k_is_ro_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ip2k_is_ip2k(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_section(index,input_file);
}

intptr_t helf_ip2k_ro_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ip2k_is_ro_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_ro_section_offset_get(index,input_file);
}

size_t helf_ip2k_ro_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_ip2k_is_ip2k(input_file))
    {
        return 0;
    }
    return helf_arch_ro_section_size_get(input_file);
}

bool helf_ip2k_is_rw_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ip2k_is_ip2k(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_section(index,input_file);
}

intptr_t helf_ip2k_rw_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ip2k_is_rw_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rw_section_offset_get(index,input_file);
}

size_t helf_ip2k_rw_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_ip2k_is_ip2k(input_file))
    {
        return 0;
    }
    return helf_arch_rw_section_size_get(input_file);
}

bool helf_ip2k_is_bss_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ip2k_is_ip2k(input_file))
    {
        return false;
    }
    return helf_arch_is_bss_section(index,input_file);
}

size_t helf_ip2k_program_header_count_get(helf_file_input_t *input_file)
{
    if(!helf_ip2k_is_ip2k(input_file))
    {
        return 0;
    }
    return helf_arch_program_header_count_get(input_file);
}

bool helf_ip2k_program_header_get(helf_file_input_t *input_file,size_t index,helf_elf32_program_header_t *programheader)
{
    if(programheader==NULL)
    {
        return false;
    }
    if(!helf_ip2k_is_ip2k(input_file))
    {
        return false;
    }
    if(index>=helf_arch_program_header_count_get(input_file))
    {
        return false;
    }
    return helf_file_input_32_bits_program_header_get(input_file,index,programheader);
}

size_t helf_ip2k_segment_count_get(helf_file_input_t *input_file)
{
    return helf_ip2k_program_header_count_get(input_file);
}

bool helf_ip2k_segment_info_get(helf_file_input_t *input_file,size_t index,helf_ip2k_segment_info_t *info)
{
    if(!helf_ip2k_is_ip2k(input_file))
    {
        return false;
    }
    return helf_arch_segment_info_get(input_file,index,info);
}

bool helf_ip2k_is_load_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ip2k_is_ip2k(input_file))
    {
        return false;
    }
    return helf_arch_is_load_segment(index,input_file);
}

bool helf_ip2k_is_rx_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ip2k_is_ip2k(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_segment(index,input_file);
}

bool helf_ip2k_is_ro_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ip2k_is_ip2k(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_segment(index,input_file);
}

bool helf_ip2k_is_rw_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ip2k_is_ip2k(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_segment(index,input_file);
}

size_t helf_ip2k_load_segment_count_get(helf_file_input_t *input_file)
{
    if(!helf_ip2k_is_ip2k(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_count_get(input_file);
}

bool helf_ip2k_load_segment_info_get(helf_file_input_t *input_file,size_t index,helf_ip2k_segment_info_t *info)
{
    if(!helf_ip2k_is_ip2k(input_file))
    {
        return false;
    }
    return helf_arch_load_segment_info_get(input_file,index,info);
}

size_t helf_ip2k_load_segment_total_filesz_get(helf_file_input_t *input_file)
{
    if(!helf_ip2k_is_ip2k(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_filesz_get(input_file);
}

size_t helf_ip2k_load_segment_total_memsz_get(helf_file_input_t *input_file)
{
    if(!helf_ip2k_is_ip2k(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_memsz_get(input_file);
}

size_t helf_ip2k_relocation_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_ip2k_is_ip2k(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_section_count_get(input_file);
}

bool helf_ip2k_relocation_section_info_get(helf_file_input_t *input_file,size_t index,helf_arch_relocation_section_info_t *info)
{
    if(!helf_ip2k_is_ip2k(input_file))
    {
        return false;
    }
    return helf_arch_relocation_section_info_get(input_file,index,info);
}

size_t helf_ip2k_relocation_count_get(helf_file_input_t *input_file,size_t section_index)
{
    if(!helf_ip2k_is_ip2k(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_count_get(input_file,section_index);
}

bool helf_ip2k_relocation_get(helf_file_input_t *input_file,size_t section_index,size_t index,helf_arch_relocation_info_t *info)
{
    if(!helf_ip2k_is_ip2k(input_file))
    {
        return false;
    }
    if(!helf_arch_relocation_get(input_file,section_index,index,info))
    {
        return false;
    }
    return true;
}

bool helf_ip2k_relocation_symbol_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_symbol_info_t *info)
{
    if(!helf_ip2k_is_ip2k(input_file))
    {
        return false;
    }
    return helf_arch_relocation_symbol_info_get(input_file,relocation,info);
}

static const helf_arch_relocation_type_name_t helf_ip2k_relocation_type_names[]=
{
    {0,0,"R_IP2K_NONE"},
    {1,2,"R_IP2K_16"},
    {2,4,"R_IP2K_32"},
    {3,0,"R_IP2K_FR9"},
    {4,0,"R_IP2K_BANK"},
    {5,0,"R_IP2K_ADDR16CJP"},
    {6,0,"R_IP2K_PAGE3"},
    {7,0,"R_IP2K_LO8DATA"},
    {8,0,"R_IP2K_HI8DATA"},
    {9,0,"R_IP2K_LO8INSN"},
    {10,0,"R_IP2K_HI8INSN"},
    {11,0,"R_IP2K_PC_SKIP"},
    {12,0,"R_IP2K_TEXT"},
    {13,0,"R_IP2K_FR_OFFSET"},
    {14,0,"R_IP2K_EX8DATA"},
};

const char *helf_ip2k_relocation_type_name_get(uint32_t type)
{
    return helf_arch_relocation_type_name_get(helf_ip2k_relocation_type_names,sizeof(helf_ip2k_relocation_type_names)/sizeof(helf_ip2k_relocation_type_names[0]),type);
}

uint8_t helf_ip2k_relocation_size_get(uint32_t type)
{
    return helf_arch_relocation_type_size_get(helf_ip2k_relocation_type_names,sizeof(helf_ip2k_relocation_type_names)/sizeof(helf_ip2k_relocation_type_names[0]),type);
}

const char *helf_ip2k_section_type_name_get(uint32_t type)
{
    (void)type;
    return NULL;
}

bool helf_ip2k_relocation_address_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_relocation_address_info_t *info)
{
    if(!helf_ip2k_is_ip2k(input_file) || relocation==NULL)
    {
        return false;
    }
    uint8_t size=helf_ip2k_relocation_size_get(relocation->type);
    if(size==0)
    {
        return false;
    }
    return helf_arch_relocation_address_info_get(input_file,relocation,size,info);
}

size_t helf_ip2k_relocation_data_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,void *buffer,size_t buffer_length)
{
    if(!helf_ip2k_is_ip2k(input_file) || relocation==NULL)
    {
        return 0;
    }
    uint8_t size=helf_ip2k_relocation_size_get(relocation->type);
    if(size==0)
    {
        return 0;
    }
    return helf_arch_relocation_data_get(input_file,relocation,size,buffer,buffer_length);
}

bool helf_ip2k_is_shared_object(helf_file_input_t *input_file)
{
    return helf_arch_is_shared_object(input_file,helf_ip2k_machines,sizeof(helf_ip2k_machines)/sizeof(helf_ip2k_machines[0]),HELF_ELFCLASS_32);
}

bool helf_ip2k_is_relocatable(helf_file_input_t *input_file)
{
    return helf_arch_is_relocatable(input_file,helf_ip2k_machines,sizeof(helf_ip2k_machines)/sizeof(helf_ip2k_machines[0]),HELF_ELFCLASS_32);
}

size_t helf_ip2k_dynamic_count_get(helf_file_input_t *input_file)
{
    if(!helf_ip2k_is_ip2k(input_file))
    {
        return 0;
    }
    return helf_arch_dynamic_count_get(input_file);
}

bool helf_ip2k_dynamic_info_get(helf_file_input_t *input_file,size_t index,helf_arch_dynamic_info_t *info)
{
    if(!helf_ip2k_is_ip2k(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_info_get(input_file,index,info);
}

bool helf_ip2k_dynamic_string_get(helf_file_input_t *input_file,uint64_t offset,char *buffer,size_t buffer_length)
{
    if(!helf_ip2k_is_ip2k(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_string_get(input_file,offset,buffer,buffer_length);
}

/* helf_relocation_howto_begin */
static const helf_arch_relocation_howto_t helf_ip2k_reloc_howto[]=
{
    {0x00000000u,0,0,0,0,0x0040,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000001u,2,16,0,0,0x0000,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000002u,4,32,0,0,0x0000,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000003u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000004u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000005u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000006u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000007u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000008u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000009u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000000Au,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000000Bu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000000Cu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000000Du,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000000Eu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
};

bool helf_ip2k_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto)
{
    if(howto==NULL)
    {
        return false;
    }
    size_t count=sizeof(helf_ip2k_reloc_howto)/sizeof(helf_ip2k_reloc_howto[0]);
    if(count==0)
    {
        return false;
    }
    for(size_t i=0;i<count;i++)
    {
        if(helf_ip2k_reloc_howto[i].type==type)
        {
            *howto=helf_ip2k_reloc_howto[i];
            return true;
        }
    }
    return false;
}
/* helf_relocation_howto_end */


