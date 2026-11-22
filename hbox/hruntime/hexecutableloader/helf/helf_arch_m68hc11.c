/***************************************************************
 * Name:      helf_arch_m68hc11.c
 * Purpose:   实现helf_arch_m68hc11接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "helf_arch_m68hc11.h"

static const uint16_t helf_m68hc11_machines[]=
{
    HELF_EM_68HC11,
};

bool helf_m68hc11_is_m68hc11(helf_file_input_t *input_file)
{
    return helf_arch_is(input_file,helf_m68hc11_machines,sizeof(helf_m68hc11_machines)/sizeof(helf_m68hc11_machines[0]),HELF_ELFCLASS_32);
}

bool helf_m68hc11_is_big_endian(helf_file_input_t *input_file)
{
    if(!helf_m68hc11_is_m68hc11(input_file))
    {
        return false;
    }
    return helf_file_input_is_big_endian(input_file);
}

bool helf_m68hc11_is_executable(helf_file_input_t *input_file)
{
    return helf_arch_is_executable(input_file,helf_m68hc11_machines,sizeof(helf_m68hc11_machines)/sizeof(helf_m68hc11_machines[0]),HELF_ELFCLASS_32);
}

bool helf_m68hc11_entry_get(helf_file_input_t *input_file,uint32_t *entry)
{
    if(entry==NULL)
    {
        return false;
    }
    if(!helf_m68hc11_is_m68hc11(input_file))
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

size_t helf_m68hc11_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_m68hc11_is_m68hc11(input_file))
    {
        return 0;
    }
    return helf_arch_section_count_get(input_file);
}

bool helf_m68hc11_section_info_get(helf_file_input_t *input_file,size_t index,helf_m68hc11_section_info_t *info)
{
    if(!helf_m68hc11_is_m68hc11(input_file))
    {
        return false;
    }
    return helf_arch_section_info_get(input_file,index,info);
}

bool helf_m68hc11_is_rx_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_m68hc11_is_m68hc11(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_section(index,input_file);
}

intptr_t helf_m68hc11_rx_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_m68hc11_is_rx_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rx_section_offset_get(index,input_file);
}

size_t helf_m68hc11_rx_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_m68hc11_is_m68hc11(input_file))
    {
        return 0;
    }
    return helf_arch_rx_section_size_get(input_file);
}

bool helf_m68hc11_is_ro_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_m68hc11_is_m68hc11(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_section(index,input_file);
}

intptr_t helf_m68hc11_ro_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_m68hc11_is_ro_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_ro_section_offset_get(index,input_file);
}

size_t helf_m68hc11_ro_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_m68hc11_is_m68hc11(input_file))
    {
        return 0;
    }
    return helf_arch_ro_section_size_get(input_file);
}

bool helf_m68hc11_is_rw_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_m68hc11_is_m68hc11(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_section(index,input_file);
}

intptr_t helf_m68hc11_rw_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_m68hc11_is_rw_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rw_section_offset_get(index,input_file);
}

size_t helf_m68hc11_rw_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_m68hc11_is_m68hc11(input_file))
    {
        return 0;
    }
    return helf_arch_rw_section_size_get(input_file);
}

bool helf_m68hc11_is_bss_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_m68hc11_is_m68hc11(input_file))
    {
        return false;
    }
    return helf_arch_is_bss_section(index,input_file);
}

size_t helf_m68hc11_program_header_count_get(helf_file_input_t *input_file)
{
    if(!helf_m68hc11_is_m68hc11(input_file))
    {
        return 0;
    }
    return helf_arch_program_header_count_get(input_file);
}

bool helf_m68hc11_program_header_get(helf_file_input_t *input_file,size_t index,helf_elf32_program_header_t *programheader)
{
    if(programheader==NULL)
    {
        return false;
    }
    if(!helf_m68hc11_is_m68hc11(input_file))
    {
        return false;
    }
    if(index>=helf_arch_program_header_count_get(input_file))
    {
        return false;
    }
    return helf_file_input_32_bits_program_header_get(input_file,index,programheader);
}

size_t helf_m68hc11_segment_count_get(helf_file_input_t *input_file)
{
    return helf_m68hc11_program_header_count_get(input_file);
}

bool helf_m68hc11_segment_info_get(helf_file_input_t *input_file,size_t index,helf_m68hc11_segment_info_t *info)
{
    if(!helf_m68hc11_is_m68hc11(input_file))
    {
        return false;
    }
    return helf_arch_segment_info_get(input_file,index,info);
}

bool helf_m68hc11_is_load_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_m68hc11_is_m68hc11(input_file))
    {
        return false;
    }
    return helf_arch_is_load_segment(index,input_file);
}

bool helf_m68hc11_is_rx_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_m68hc11_is_m68hc11(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_segment(index,input_file);
}

bool helf_m68hc11_is_ro_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_m68hc11_is_m68hc11(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_segment(index,input_file);
}

bool helf_m68hc11_is_rw_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_m68hc11_is_m68hc11(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_segment(index,input_file);
}

size_t helf_m68hc11_load_segment_count_get(helf_file_input_t *input_file)
{
    if(!helf_m68hc11_is_m68hc11(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_count_get(input_file);
}

bool helf_m68hc11_load_segment_info_get(helf_file_input_t *input_file,size_t index,helf_m68hc11_segment_info_t *info)
{
    if(!helf_m68hc11_is_m68hc11(input_file))
    {
        return false;
    }
    return helf_arch_load_segment_info_get(input_file,index,info);
}

size_t helf_m68hc11_load_segment_total_filesz_get(helf_file_input_t *input_file)
{
    if(!helf_m68hc11_is_m68hc11(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_filesz_get(input_file);
}

size_t helf_m68hc11_load_segment_total_memsz_get(helf_file_input_t *input_file)
{
    if(!helf_m68hc11_is_m68hc11(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_memsz_get(input_file);
}

size_t helf_m68hc11_relocation_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_m68hc11_is_m68hc11(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_section_count_get(input_file);
}

bool helf_m68hc11_relocation_section_info_get(helf_file_input_t *input_file,size_t index,helf_arch_relocation_section_info_t *info)
{
    if(!helf_m68hc11_is_m68hc11(input_file))
    {
        return false;
    }
    return helf_arch_relocation_section_info_get(input_file,index,info);
}

size_t helf_m68hc11_relocation_count_get(helf_file_input_t *input_file,size_t section_index)
{
    if(!helf_m68hc11_is_m68hc11(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_count_get(input_file,section_index);
}

bool helf_m68hc11_relocation_get(helf_file_input_t *input_file,size_t section_index,size_t index,helf_arch_relocation_info_t *info)
{
    if(!helf_m68hc11_is_m68hc11(input_file))
    {
        return false;
    }
    if(!helf_arch_relocation_get(input_file,section_index,index,info))
    {
        return false;
    }
    return true;
}

bool helf_m68hc11_relocation_symbol_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_symbol_info_t *info)
{
    if(!helf_m68hc11_is_m68hc11(input_file))
    {
        return false;
    }
    return helf_arch_relocation_symbol_info_get(input_file,relocation,info);
}

static const helf_arch_relocation_type_name_t helf_m68hc11_relocation_type_names[]=
{
    {0,0,"R_M68HC11_NONE"},
    {1,1,"R_M68HC11_8"},
    {2,1,"R_M68HC11_HI8"},
    {3,1,"R_M68HC11_LO8"},
    {4,1,"R_M68HC11_PCREL_8"},
    {5,2,"R_M68HC11_16"},
    {6,4,"R_M68HC11_32"},
    {7,1,"R_M68HC11_3B"},
    {8,2,"R_M68HC11_PCREL_16"},
    {9,2,"R_M68HC11_GNU_VTINHERIT"},
    {10,2,"R_M68HC11_GNU_VTENTRY"},
    {11,4,"R_M68HC11_24"},
    {12,2,"R_M68HC11_LO16"},
    {13,1,"R_M68HC11_PAGE"},
    {15,2,"R_M68HC12_16B"},
    {16,2,"R_M68HC12_PCREL_9"},
    {17,2,"R_M68HC12_PCREL_10"},
    {18,1,"R_M68HC12_HI8XG"},
    {19,1,"R_M68HC12_LO8XG"},
    {20,2,"R_M68HC11_RL_JUMP"},
    {21,2,"R_M68HC11_RL_GROUP"},
};

const char *helf_m68hc11_relocation_type_name_get(uint32_t type)
{
    return helf_arch_relocation_type_name_get(helf_m68hc11_relocation_type_names,sizeof(helf_m68hc11_relocation_type_names)/sizeof(helf_m68hc11_relocation_type_names[0]),type);
}

uint8_t helf_m68hc11_relocation_size_get(uint32_t type)
{
    return helf_arch_relocation_type_size_get(helf_m68hc11_relocation_type_names,sizeof(helf_m68hc11_relocation_type_names)/sizeof(helf_m68hc11_relocation_type_names[0]),type);
}

const char *helf_m68hc11_section_type_name_get(uint32_t type)
{
    (void)type;
    return NULL;
}

bool helf_m68hc11_relocation_address_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_relocation_address_info_t *info)
{
    if(!helf_m68hc11_is_m68hc11(input_file) || relocation==NULL)
    {
        return false;
    }
    uint8_t size=helf_m68hc11_relocation_size_get(relocation->type);
    if(size==0)
    {
        return false;
    }
    return helf_arch_relocation_address_info_get(input_file,relocation,size,info);
}

size_t helf_m68hc11_relocation_data_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,void *buffer,size_t buffer_length)
{
    if(!helf_m68hc11_is_m68hc11(input_file) || relocation==NULL)
    {
        return 0;
    }
    uint8_t size=helf_m68hc11_relocation_size_get(relocation->type);
    if(size==0)
    {
        return 0;
    }
    return helf_arch_relocation_data_get(input_file,relocation,size,buffer,buffer_length);
}

bool helf_m68hc11_is_shared_object(helf_file_input_t *input_file)
{
    return helf_arch_is_shared_object(input_file,helf_m68hc11_machines,sizeof(helf_m68hc11_machines)/sizeof(helf_m68hc11_machines[0]),HELF_ELFCLASS_32);
}

bool helf_m68hc11_is_relocatable(helf_file_input_t *input_file)
{
    return helf_arch_is_relocatable(input_file,helf_m68hc11_machines,sizeof(helf_m68hc11_machines)/sizeof(helf_m68hc11_machines[0]),HELF_ELFCLASS_32);
}

size_t helf_m68hc11_dynamic_count_get(helf_file_input_t *input_file)
{
    if(!helf_m68hc11_is_m68hc11(input_file))
    {
        return 0;
    }
    return helf_arch_dynamic_count_get(input_file);
}

bool helf_m68hc11_dynamic_info_get(helf_file_input_t *input_file,size_t index,helf_arch_dynamic_info_t *info)
{
    if(!helf_m68hc11_is_m68hc11(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_info_get(input_file,index,info);
}

bool helf_m68hc11_dynamic_string_get(helf_file_input_t *input_file,uint64_t offset,char *buffer,size_t buffer_length)
{
    if(!helf_m68hc11_is_m68hc11(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_string_get(input_file,offset,buffer,buffer_length);
}

/* helf_relocation_howto_begin */
static const helf_arch_relocation_howto_t helf_m68hc11_reloc_howto[]=
{
    {0x00000000u,0,0,0,0,0x0040,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000001u,1,8,0,0,0x0000,0x00000000000000FFULL,0x00000000000000FFULL},
    {0x00000002u,1,8,8,0,0x0000,0x00000000000000FFULL,0x00000000000000FFULL},
    {0x00000003u,1,8,0,0,0x0000,0x00000000000000FFULL,0x00000000000000FFULL},
    {0x00000004u,1,8,0,0,0x0003,0x00000000000000FFULL,0x00000000000000FFULL},
    {0x00000005u,2,16,0,0,0x0000,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000006u,4,32,0,0,0x0000,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000007u,1,3,0,0,0x0000,0x0000000000000003ULL,0x0000000000000003ULL},
    {0x00000008u,2,16,0,0,0x0003,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000009u,2,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000000Au,2,0,0,0,0x0010,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000000Bu,4,24,0,0,0x0010,0x0000000000FFFFFFULL,0x0000000000FFFFFFULL},
    {0x0000000Cu,2,16,0,0,0x0000,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x0000000Du,1,8,0,0,0x0000,0x00000000000000FFULL,0x00000000000000FFULL},
    {0x0000000Fu,2,16,0,0,0x0000,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000010u,2,10,1,0,0x000B,0x000000000000FE00ULL,0x00000000000001FFULL},
    {0x00000011u,2,11,1,0,0x000B,0x000000000000FC00ULL,0x00000000000003FFULL},
    {0x00000012u,1,8,8,0,0x0000,0x00000000000000FFULL,0x00000000000000FFULL},
    {0x00000013u,1,8,8,0,0x0000,0x00000000000000FFULL,0x00000000000000FFULL},
    {0x00000014u,2,0,0,0,0x001A,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000015u,2,0,0,0,0x001A,0x0000000000000000ULL,0x0000000000000000ULL},
};

bool helf_m68hc11_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto)
{
    if(howto==NULL)
    {
        return false;
    }
    size_t count=sizeof(helf_m68hc11_reloc_howto)/sizeof(helf_m68hc11_reloc_howto[0]);
    if(count==0)
    {
        return false;
    }
    for(size_t i=0;i<count;i++)
    {
        if(helf_m68hc11_reloc_howto[i].type==type)
        {
            *howto=helf_m68hc11_reloc_howto[i];
            return true;
        }
    }
    return false;
}
/* helf_relocation_howto_end */


