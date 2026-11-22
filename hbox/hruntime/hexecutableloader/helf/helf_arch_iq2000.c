/***************************************************************
 * Name:      helf_arch_iq2000.c
 * Purpose:   实现helf_arch_iq2000接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "helf_arch_iq2000.h"

static const uint16_t helf_iq2000_machines[]=
{
    HELF_EM_IQ2000,
};

bool helf_iq2000_is_iq2000(helf_file_input_t *input_file)
{
    return helf_arch_is(input_file,helf_iq2000_machines,sizeof(helf_iq2000_machines)/sizeof(helf_iq2000_machines[0]),HELF_ELFCLASS_32);
}

bool helf_iq2000_is_big_endian(helf_file_input_t *input_file)
{
    if(!helf_iq2000_is_iq2000(input_file))
    {
        return false;
    }
    return helf_file_input_is_big_endian(input_file);
}

bool helf_iq2000_is_executable(helf_file_input_t *input_file)
{
    return helf_arch_is_executable(input_file,helf_iq2000_machines,sizeof(helf_iq2000_machines)/sizeof(helf_iq2000_machines[0]),HELF_ELFCLASS_32);
}

bool helf_iq2000_entry_get(helf_file_input_t *input_file,uint32_t *entry)
{
    if(entry==NULL)
    {
        return false;
    }
    if(!helf_iq2000_is_iq2000(input_file))
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

size_t helf_iq2000_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_iq2000_is_iq2000(input_file))
    {
        return 0;
    }
    return helf_arch_section_count_get(input_file);
}

bool helf_iq2000_section_info_get(helf_file_input_t *input_file,size_t index,helf_iq2000_section_info_t *info)
{
    if(!helf_iq2000_is_iq2000(input_file))
    {
        return false;
    }
    return helf_arch_section_info_get(input_file,index,info);
}

bool helf_iq2000_is_rx_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_iq2000_is_iq2000(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_section(index,input_file);
}

intptr_t helf_iq2000_rx_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_iq2000_is_rx_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rx_section_offset_get(index,input_file);
}

size_t helf_iq2000_rx_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_iq2000_is_iq2000(input_file))
    {
        return 0;
    }
    return helf_arch_rx_section_size_get(input_file);
}

bool helf_iq2000_is_ro_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_iq2000_is_iq2000(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_section(index,input_file);
}

intptr_t helf_iq2000_ro_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_iq2000_is_ro_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_ro_section_offset_get(index,input_file);
}

size_t helf_iq2000_ro_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_iq2000_is_iq2000(input_file))
    {
        return 0;
    }
    return helf_arch_ro_section_size_get(input_file);
}

bool helf_iq2000_is_rw_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_iq2000_is_iq2000(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_section(index,input_file);
}

intptr_t helf_iq2000_rw_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_iq2000_is_rw_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rw_section_offset_get(index,input_file);
}

size_t helf_iq2000_rw_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_iq2000_is_iq2000(input_file))
    {
        return 0;
    }
    return helf_arch_rw_section_size_get(input_file);
}

bool helf_iq2000_is_bss_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_iq2000_is_iq2000(input_file))
    {
        return false;
    }
    return helf_arch_is_bss_section(index,input_file);
}

size_t helf_iq2000_program_header_count_get(helf_file_input_t *input_file)
{
    if(!helf_iq2000_is_iq2000(input_file))
    {
        return 0;
    }
    return helf_arch_program_header_count_get(input_file);
}

bool helf_iq2000_program_header_get(helf_file_input_t *input_file,size_t index,helf_elf32_program_header_t *programheader)
{
    if(programheader==NULL)
    {
        return false;
    }
    if(!helf_iq2000_is_iq2000(input_file))
    {
        return false;
    }
    if(index>=helf_arch_program_header_count_get(input_file))
    {
        return false;
    }
    return helf_file_input_32_bits_program_header_get(input_file,index,programheader);
}

size_t helf_iq2000_segment_count_get(helf_file_input_t *input_file)
{
    return helf_iq2000_program_header_count_get(input_file);
}

bool helf_iq2000_segment_info_get(helf_file_input_t *input_file,size_t index,helf_iq2000_segment_info_t *info)
{
    if(!helf_iq2000_is_iq2000(input_file))
    {
        return false;
    }
    return helf_arch_segment_info_get(input_file,index,info);
}

bool helf_iq2000_is_load_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_iq2000_is_iq2000(input_file))
    {
        return false;
    }
    return helf_arch_is_load_segment(index,input_file);
}

bool helf_iq2000_is_rx_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_iq2000_is_iq2000(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_segment(index,input_file);
}

bool helf_iq2000_is_ro_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_iq2000_is_iq2000(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_segment(index,input_file);
}

bool helf_iq2000_is_rw_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_iq2000_is_iq2000(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_segment(index,input_file);
}

size_t helf_iq2000_load_segment_count_get(helf_file_input_t *input_file)
{
    if(!helf_iq2000_is_iq2000(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_count_get(input_file);
}

bool helf_iq2000_load_segment_info_get(helf_file_input_t *input_file,size_t index,helf_iq2000_segment_info_t *info)
{
    if(!helf_iq2000_is_iq2000(input_file))
    {
        return false;
    }
    return helf_arch_load_segment_info_get(input_file,index,info);
}

size_t helf_iq2000_load_segment_total_filesz_get(helf_file_input_t *input_file)
{
    if(!helf_iq2000_is_iq2000(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_filesz_get(input_file);
}

size_t helf_iq2000_load_segment_total_memsz_get(helf_file_input_t *input_file)
{
    if(!helf_iq2000_is_iq2000(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_memsz_get(input_file);
}

size_t helf_iq2000_relocation_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_iq2000_is_iq2000(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_section_count_get(input_file);
}

bool helf_iq2000_relocation_section_info_get(helf_file_input_t *input_file,size_t index,helf_arch_relocation_section_info_t *info)
{
    if(!helf_iq2000_is_iq2000(input_file))
    {
        return false;
    }
    return helf_arch_relocation_section_info_get(input_file,index,info);
}

size_t helf_iq2000_relocation_count_get(helf_file_input_t *input_file,size_t section_index)
{
    if(!helf_iq2000_is_iq2000(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_count_get(input_file,section_index);
}

bool helf_iq2000_relocation_get(helf_file_input_t *input_file,size_t section_index,size_t index,helf_arch_relocation_info_t *info)
{
    if(!helf_iq2000_is_iq2000(input_file))
    {
        return false;
    }
    if(!helf_arch_relocation_get(input_file,section_index,index,info))
    {
        return false;
    }
    return true;
}

bool helf_iq2000_relocation_symbol_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_symbol_info_t *info)
{
    if(!helf_iq2000_is_iq2000(input_file))
    {
        return false;
    }
    return helf_arch_relocation_symbol_info_get(input_file,relocation,info);
}

static const helf_arch_relocation_type_name_t helf_iq2000_relocation_type_names[]=
{
    {0,0,"R_IQ2000_NONE"},
    {1,2,"R_IQ2000_16"},
    {2,4,"R_IQ2000_32"},
    {3,4,"R_IQ2000_26"},
    {4,4,"R_IQ2000_PC16"},
    {5,4,"R_IQ2000_HI16"},
    {6,4,"R_IQ2000_LO16"},
    {7,4,"R_IQ2000_OFFSET_16"},
    {8,4,"R_IQ2000_OFFSET_21"},
    {9,2,"R_IQ2000_UHI16"},
    {10,4,"R_IQ2000_32_DEBUG"},
    {200,4,"R_IQ2000_GNU_VTINHERIT"},
    {201,4,"R_IQ2000_GNU_VTENTRY"},
};

const char *helf_iq2000_relocation_type_name_get(uint32_t type)
{
    return helf_arch_relocation_type_name_get(helf_iq2000_relocation_type_names,sizeof(helf_iq2000_relocation_type_names)/sizeof(helf_iq2000_relocation_type_names[0]),type);
}

uint8_t helf_iq2000_relocation_size_get(uint32_t type)
{
    return helf_arch_relocation_type_size_get(helf_iq2000_relocation_type_names,sizeof(helf_iq2000_relocation_type_names)/sizeof(helf_iq2000_relocation_type_names[0]),type);
}

const char *helf_iq2000_section_type_name_get(uint32_t type)
{
    (void)type;
    return NULL;
}

bool helf_iq2000_relocation_address_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_relocation_address_info_t *info)
{
    if(!helf_iq2000_is_iq2000(input_file) || relocation==NULL)
    {
        return false;
    }
    uint8_t size=helf_iq2000_relocation_size_get(relocation->type);
    if(size==0)
    {
        return false;
    }
    return helf_arch_relocation_address_info_get(input_file,relocation,size,info);
}

size_t helf_iq2000_relocation_data_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,void *buffer,size_t buffer_length)
{
    if(!helf_iq2000_is_iq2000(input_file) || relocation==NULL)
    {
        return 0;
    }
    uint8_t size=helf_iq2000_relocation_size_get(relocation->type);
    if(size==0)
    {
        return 0;
    }
    return helf_arch_relocation_data_get(input_file,relocation,size,buffer,buffer_length);
}

bool helf_iq2000_is_shared_object(helf_file_input_t *input_file)
{
    return helf_arch_is_shared_object(input_file,helf_iq2000_machines,sizeof(helf_iq2000_machines)/sizeof(helf_iq2000_machines[0]),HELF_ELFCLASS_32);
}

bool helf_iq2000_is_relocatable(helf_file_input_t *input_file)
{
    return helf_arch_is_relocatable(input_file,helf_iq2000_machines,sizeof(helf_iq2000_machines)/sizeof(helf_iq2000_machines[0]),HELF_ELFCLASS_32);
}

size_t helf_iq2000_dynamic_count_get(helf_file_input_t *input_file)
{
    if(!helf_iq2000_is_iq2000(input_file))
    {
        return 0;
    }
    return helf_arch_dynamic_count_get(input_file);
}

bool helf_iq2000_dynamic_info_get(helf_file_input_t *input_file,size_t index,helf_arch_dynamic_info_t *info)
{
    if(!helf_iq2000_is_iq2000(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_info_get(input_file,index,info);
}

bool helf_iq2000_dynamic_string_get(helf_file_input_t *input_file,uint64_t offset,char *buffer,size_t buffer_length)
{
    if(!helf_iq2000_is_iq2000(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_string_get(input_file,offset,buffer,buffer_length);
}

/* helf_relocation_howto_begin */
static const helf_arch_relocation_howto_t helf_iq2000_reloc_howto[]=
{
    {0x00000000u,0,0,0,0,0x0040,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000001u,2,16,0,0,0x0000,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000002u,4,31,0,0,0x0000,0x0000000000000000ULL,0x000000007FFFFFFFULL},
    {0x00000003u,4,26,2,0,0x0000,0x0000000000000000ULL,0x0000000003FFFFFFULL},
    {0x00000004u,4,16,2,0,0x0003,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000005u,4,15,16,0,0x0010,0x0000000000000000ULL,0x0000000000007FFFULL},
    {0x00000006u,4,16,0,0,0x0000,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000007u,4,16,2,0,0x0000,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000008u,4,21,2,0,0x0000,0x0000000000000000ULL,0x00000000001FFFFFULL},
    {0x00000009u,2,16,0,0,0x0000,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000000Au,4,32,0,0,0x0000,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x000000C8u,4,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000C9u,4,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
};

bool helf_iq2000_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto)
{
    if(howto==NULL)
    {
        return false;
    }
    size_t count=sizeof(helf_iq2000_reloc_howto)/sizeof(helf_iq2000_reloc_howto[0]);
    if(count==0)
    {
        return false;
    }
    for(size_t i=0;i<count;i++)
    {
        if(helf_iq2000_reloc_howto[i].type==type)
        {
            *howto=helf_iq2000_reloc_howto[i];
            return true;
        }
    }
    return false;
}
/* helf_relocation_howto_end */


