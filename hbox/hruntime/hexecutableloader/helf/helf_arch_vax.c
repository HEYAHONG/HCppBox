/***************************************************************
 * Name:      helf_arch_vax.c
 * Purpose:   实现helf_arch_vax接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "helf_arch_vax.h"

static const uint16_t helf_vax_machines[]=
{
    HELF_EM_VAX,
};

bool helf_vax_is_vax(helf_file_input_t *input_file)
{
    return helf_arch_is(input_file,helf_vax_machines,sizeof(helf_vax_machines)/sizeof(helf_vax_machines[0]),HELF_ELFCLASS_32);
}

bool helf_vax_is_big_endian(helf_file_input_t *input_file)
{
    if(!helf_vax_is_vax(input_file))
    {
        return false;
    }
    return helf_file_input_is_big_endian(input_file);
}

bool helf_vax_is_executable(helf_file_input_t *input_file)
{
    return helf_arch_is_executable(input_file,helf_vax_machines,sizeof(helf_vax_machines)/sizeof(helf_vax_machines[0]),HELF_ELFCLASS_32);
}

bool helf_vax_entry_get(helf_file_input_t *input_file,uint32_t *entry)
{
    if(entry==NULL)
    {
        return false;
    }
    if(!helf_vax_is_vax(input_file))
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

size_t helf_vax_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_vax_is_vax(input_file))
    {
        return 0;
    }
    return helf_arch_section_count_get(input_file);
}

bool helf_vax_section_info_get(helf_file_input_t *input_file,size_t index,helf_vax_section_info_t *info)
{
    if(!helf_vax_is_vax(input_file))
    {
        return false;
    }
    return helf_arch_section_info_get(input_file,index,info);
}

bool helf_vax_is_rx_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_vax_is_vax(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_section(index,input_file);
}

intptr_t helf_vax_rx_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_vax_is_rx_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rx_section_offset_get(index,input_file);
}

size_t helf_vax_rx_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_vax_is_vax(input_file))
    {
        return 0;
    }
    return helf_arch_rx_section_size_get(input_file);
}

bool helf_vax_is_ro_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_vax_is_vax(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_section(index,input_file);
}

intptr_t helf_vax_ro_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_vax_is_ro_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_ro_section_offset_get(index,input_file);
}

size_t helf_vax_ro_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_vax_is_vax(input_file))
    {
        return 0;
    }
    return helf_arch_ro_section_size_get(input_file);
}

bool helf_vax_is_rw_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_vax_is_vax(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_section(index,input_file);
}

intptr_t helf_vax_rw_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_vax_is_rw_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rw_section_offset_get(index,input_file);
}

size_t helf_vax_rw_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_vax_is_vax(input_file))
    {
        return 0;
    }
    return helf_arch_rw_section_size_get(input_file);
}

bool helf_vax_is_bss_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_vax_is_vax(input_file))
    {
        return false;
    }
    return helf_arch_is_bss_section(index,input_file);
}

size_t helf_vax_program_header_count_get(helf_file_input_t *input_file)
{
    if(!helf_vax_is_vax(input_file))
    {
        return 0;
    }
    return helf_arch_program_header_count_get(input_file);
}

bool helf_vax_program_header_get(helf_file_input_t *input_file,size_t index,helf_elf32_program_header_t *programheader)
{
    if(programheader==NULL)
    {
        return false;
    }
    if(!helf_vax_is_vax(input_file))
    {
        return false;
    }
    if(index>=helf_arch_program_header_count_get(input_file))
    {
        return false;
    }
    return helf_file_input_32_bits_program_header_get(input_file,index,programheader);
}

size_t helf_vax_segment_count_get(helf_file_input_t *input_file)
{
    return helf_vax_program_header_count_get(input_file);
}

bool helf_vax_segment_info_get(helf_file_input_t *input_file,size_t index,helf_vax_segment_info_t *info)
{
    if(!helf_vax_is_vax(input_file))
    {
        return false;
    }
    return helf_arch_segment_info_get(input_file,index,info);
}

bool helf_vax_is_load_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_vax_is_vax(input_file))
    {
        return false;
    }
    return helf_arch_is_load_segment(index,input_file);
}

bool helf_vax_is_rx_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_vax_is_vax(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_segment(index,input_file);
}

bool helf_vax_is_ro_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_vax_is_vax(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_segment(index,input_file);
}

bool helf_vax_is_rw_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_vax_is_vax(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_segment(index,input_file);
}

size_t helf_vax_load_segment_count_get(helf_file_input_t *input_file)
{
    if(!helf_vax_is_vax(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_count_get(input_file);
}

bool helf_vax_load_segment_info_get(helf_file_input_t *input_file,size_t index,helf_vax_segment_info_t *info)
{
    if(!helf_vax_is_vax(input_file))
    {
        return false;
    }
    return helf_arch_load_segment_info_get(input_file,index,info);
}

size_t helf_vax_load_segment_total_filesz_get(helf_file_input_t *input_file)
{
    if(!helf_vax_is_vax(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_filesz_get(input_file);
}

size_t helf_vax_load_segment_total_memsz_get(helf_file_input_t *input_file)
{
    if(!helf_vax_is_vax(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_memsz_get(input_file);
}

size_t helf_vax_relocation_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_vax_is_vax(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_section_count_get(input_file);
}

bool helf_vax_relocation_section_info_get(helf_file_input_t *input_file,size_t index,helf_arch_relocation_section_info_t *info)
{
    if(!helf_vax_is_vax(input_file))
    {
        return false;
    }
    return helf_arch_relocation_section_info_get(input_file,index,info);
}

size_t helf_vax_relocation_count_get(helf_file_input_t *input_file,size_t section_index)
{
    if(!helf_vax_is_vax(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_count_get(input_file,section_index);
}

bool helf_vax_relocation_get(helf_file_input_t *input_file,size_t section_index,size_t index,helf_arch_relocation_info_t *info)
{
    if(!helf_vax_is_vax(input_file))
    {
        return false;
    }
    if(!helf_arch_relocation_get(input_file,section_index,index,info))
    {
        return false;
    }
    return true;
}

bool helf_vax_relocation_symbol_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_symbol_info_t *info)
{
    if(!helf_vax_is_vax(input_file))
    {
        return false;
    }
    return helf_arch_relocation_symbol_info_get(input_file,relocation,info);
}

static const helf_arch_relocation_type_name_t helf_vax_relocation_type_names[]=
{
    {0,0,"R_VAX_NONE"},
    {1,4,"R_VAX_32"},
    {2,2,"R_VAX_16"},
    {3,1,"R_VAX_8"},
    {4,4,"R_VAX_PC32"},
    {5,2,"R_VAX_PC16"},
    {6,1,"R_VAX_PC8"},
    {7,4,"R_VAX_GOT32"},
    {13,4,"R_VAX_PLT32"},
    {19,0,"R_VAX_COPY"},
    {20,4,"R_VAX_GLOB_DAT"},
    {21,4,"R_VAX_JMP_SLOT"},
    {22,4,"R_VAX_RELATIVE"},
    {23,4,"R_VAX_GNU_VTINHERIT"},
    {24,4,"R_VAX_GNU_VTENTRY"},
};

const char *helf_vax_relocation_type_name_get(uint32_t type)
{
    return helf_arch_relocation_type_name_get(helf_vax_relocation_type_names,sizeof(helf_vax_relocation_type_names)/sizeof(helf_vax_relocation_type_names[0]),type);
}

uint8_t helf_vax_relocation_size_get(uint32_t type)
{
    return helf_arch_relocation_type_size_get(helf_vax_relocation_type_names,sizeof(helf_vax_relocation_type_names)/sizeof(helf_vax_relocation_type_names[0]),type);
}

const char *helf_vax_section_type_name_get(uint32_t type)
{
    (void)type;
    return NULL;
}

bool helf_vax_relocation_address_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_relocation_address_info_t *info)
{
    if(!helf_vax_is_vax(input_file) || relocation==NULL)
    {
        return false;
    }
    uint8_t size=helf_vax_relocation_size_get(relocation->type);
    if(size==0)
    {
        return false;
    }
    return helf_arch_relocation_address_info_get(input_file,relocation,size,info);
}

size_t helf_vax_relocation_data_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,void *buffer,size_t buffer_length)
{
    if(!helf_vax_is_vax(input_file) || relocation==NULL)
    {
        return 0;
    }
    uint8_t size=helf_vax_relocation_size_get(relocation->type);
    if(size==0)
    {
        return 0;
    }
    return helf_arch_relocation_data_get(input_file,relocation,size,buffer,buffer_length);
}

bool helf_vax_is_shared_object(helf_file_input_t *input_file)
{
    return helf_arch_is_shared_object(input_file,helf_vax_machines,sizeof(helf_vax_machines)/sizeof(helf_vax_machines[0]),HELF_ELFCLASS_32);
}

bool helf_vax_is_relocatable(helf_file_input_t *input_file)
{
    return helf_arch_is_relocatable(input_file,helf_vax_machines,sizeof(helf_vax_machines)/sizeof(helf_vax_machines[0]),HELF_ELFCLASS_32);
}

size_t helf_vax_dynamic_count_get(helf_file_input_t *input_file)
{
    if(!helf_vax_is_vax(input_file))
    {
        return 0;
    }
    return helf_arch_dynamic_count_get(input_file);
}

bool helf_vax_dynamic_info_get(helf_file_input_t *input_file,size_t index,helf_arch_dynamic_info_t *info)
{
    if(!helf_vax_is_vax(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_info_get(input_file,index,info);
}

bool helf_vax_dynamic_string_get(helf_file_input_t *input_file,uint64_t offset,char *buffer,size_t buffer_length)
{
    if(!helf_vax_is_vax(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_string_get(input_file,offset,buffer,buffer_length);
}
