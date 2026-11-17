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
