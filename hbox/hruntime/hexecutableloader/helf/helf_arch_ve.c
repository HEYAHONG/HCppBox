/***************************************************************
 * Name:      helf_arch_ve.c
 * Purpose:   实现helf_arch_ve接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "helf_arch_ve.h"

static const uint16_t helf_ve_machines[]=
{
    HELF_EM_VE,
};

bool helf_ve_is_ve(helf_file_input_t *input_file)
{
    return helf_arch_is(input_file,helf_ve_machines,sizeof(helf_ve_machines)/sizeof(helf_ve_machines[0]),HELF_ELFCLASS_64);
}

bool helf_ve_is_big_endian(helf_file_input_t *input_file)
{
    if(!helf_ve_is_ve(input_file))
    {
        return false;
    }
    return helf_file_input_is_big_endian(input_file);
}

bool helf_ve_is_executable(helf_file_input_t *input_file)
{
    return helf_arch_is_executable(input_file,helf_ve_machines,sizeof(helf_ve_machines)/sizeof(helf_ve_machines[0]),HELF_ELFCLASS_64);
}

bool helf_ve_entry_get(helf_file_input_t *input_file,uint64_t *entry)
{
    if(entry==NULL)
    {
        return false;
    }
    if(!helf_ve_is_ve(input_file))
    {
        return false;
    }
    uint64_t elf_entry=0;
    if(!helf_arch_entry_get(input_file,&elf_entry))
    {
        return false;
    }
    *entry=(uint64_t)elf_entry;
    return true;
}

size_t helf_ve_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_ve_is_ve(input_file))
    {
        return 0;
    }
    return helf_arch_section_count_get(input_file);
}

bool helf_ve_section_info_get(helf_file_input_t *input_file,size_t index,helf_ve_section_info_t *info)
{
    if(!helf_ve_is_ve(input_file))
    {
        return false;
    }
    return helf_arch_section_info_get(input_file,index,info);
}

bool helf_ve_is_rx_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ve_is_ve(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_section(index,input_file);
}

intptr_t helf_ve_rx_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ve_is_rx_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rx_section_offset_get(index,input_file);
}

size_t helf_ve_rx_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_ve_is_ve(input_file))
    {
        return 0;
    }
    return helf_arch_rx_section_size_get(input_file);
}

bool helf_ve_is_ro_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ve_is_ve(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_section(index,input_file);
}

intptr_t helf_ve_ro_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ve_is_ro_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_ro_section_offset_get(index,input_file);
}

size_t helf_ve_ro_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_ve_is_ve(input_file))
    {
        return 0;
    }
    return helf_arch_ro_section_size_get(input_file);
}

bool helf_ve_is_rw_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ve_is_ve(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_section(index,input_file);
}

intptr_t helf_ve_rw_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ve_is_rw_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rw_section_offset_get(index,input_file);
}

size_t helf_ve_rw_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_ve_is_ve(input_file))
    {
        return 0;
    }
    return helf_arch_rw_section_size_get(input_file);
}

bool helf_ve_is_bss_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ve_is_ve(input_file))
    {
        return false;
    }
    return helf_arch_is_bss_section(index,input_file);
}

size_t helf_ve_program_header_count_get(helf_file_input_t *input_file)
{
    if(!helf_ve_is_ve(input_file))
    {
        return 0;
    }
    return helf_arch_program_header_count_get(input_file);
}

bool helf_ve_program_header_get(helf_file_input_t *input_file,size_t index,helf_elf64_program_header_t *programheader)
{
    if(programheader==NULL)
    {
        return false;
    }
    if(!helf_ve_is_ve(input_file))
    {
        return false;
    }
    if(index>=helf_arch_program_header_count_get(input_file))
    {
        return false;
    }
    return helf_file_input_64_bits_program_header_get(input_file,index,programheader);
}

size_t helf_ve_segment_count_get(helf_file_input_t *input_file)
{
    return helf_ve_program_header_count_get(input_file);
}

bool helf_ve_segment_info_get(helf_file_input_t *input_file,size_t index,helf_ve_segment_info_t *info)
{
    if(!helf_ve_is_ve(input_file))
    {
        return false;
    }
    return helf_arch_segment_info_get(input_file,index,info);
}

bool helf_ve_is_load_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ve_is_ve(input_file))
    {
        return false;
    }
    return helf_arch_is_load_segment(index,input_file);
}

bool helf_ve_is_rx_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ve_is_ve(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_segment(index,input_file);
}

bool helf_ve_is_ro_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ve_is_ve(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_segment(index,input_file);
}

bool helf_ve_is_rw_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ve_is_ve(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_segment(index,input_file);
}

size_t helf_ve_load_segment_count_get(helf_file_input_t *input_file)
{
    if(!helf_ve_is_ve(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_count_get(input_file);
}

bool helf_ve_load_segment_info_get(helf_file_input_t *input_file,size_t index,helf_ve_segment_info_t *info)
{
    if(!helf_ve_is_ve(input_file))
    {
        return false;
    }
    return helf_arch_load_segment_info_get(input_file,index,info);
}

size_t helf_ve_load_segment_total_filesz_get(helf_file_input_t *input_file)
{
    if(!helf_ve_is_ve(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_filesz_get(input_file);
}

size_t helf_ve_load_segment_total_memsz_get(helf_file_input_t *input_file)
{
    if(!helf_ve_is_ve(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_memsz_get(input_file);
}

size_t helf_ve_relocation_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_ve_is_ve(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_section_count_get(input_file);
}

bool helf_ve_relocation_section_info_get(helf_file_input_t *input_file,size_t index,helf_arch_relocation_section_info_t *info)
{
    if(!helf_ve_is_ve(input_file))
    {
        return false;
    }
    return helf_arch_relocation_section_info_get(input_file,index,info);
}

size_t helf_ve_relocation_count_get(helf_file_input_t *input_file,size_t section_index)
{
    if(!helf_ve_is_ve(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_count_get(input_file,section_index);
}

bool helf_ve_relocation_get(helf_file_input_t *input_file,size_t section_index,size_t index,helf_arch_relocation_info_t *info)
{
    if(!helf_ve_is_ve(input_file))
    {
        return false;
    }
    if(!helf_arch_relocation_get(input_file,section_index,index,info))
    {
        return false;
    }
    return true;
}

bool helf_ve_relocation_symbol_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_symbol_info_t *info)
{
    if(!helf_ve_is_ve(input_file))
    {
        return false;
    }
    return helf_arch_relocation_symbol_info_get(input_file,relocation,info);
}

const char *helf_ve_relocation_type_name_get(uint32_t type)
{
    (void)type;
    return NULL;
}

uint8_t helf_ve_relocation_size_get(uint32_t type)
{
    (void)type;
    return 0;
}

const char *helf_ve_section_type_name_get(uint32_t type)
{
    (void)type;
    return NULL;
}

bool helf_ve_relocation_address_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_relocation_address_info_t *info)
{
    if(!helf_ve_is_ve(input_file) || relocation==NULL)
    {
        return false;
    }
    uint8_t size=helf_ve_relocation_size_get(relocation->type);
    if(size==0)
    {
        return false;
    }
    return helf_arch_relocation_address_info_get(input_file,relocation,size,info);
}

size_t helf_ve_relocation_data_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,void *buffer,size_t buffer_length)
{
    if(!helf_ve_is_ve(input_file) || relocation==NULL)
    {
        return 0;
    }
    uint8_t size=helf_ve_relocation_size_get(relocation->type);
    if(size==0)
    {
        return 0;
    }
    return helf_arch_relocation_data_get(input_file,relocation,size,buffer,buffer_length);
}

bool helf_ve_is_shared_object(helf_file_input_t *input_file)
{
    return helf_arch_is_shared_object(input_file,helf_ve_machines,sizeof(helf_ve_machines)/sizeof(helf_ve_machines[0]),HELF_ELFCLASS_64);
}

bool helf_ve_is_relocatable(helf_file_input_t *input_file)
{
    return helf_arch_is_relocatable(input_file,helf_ve_machines,sizeof(helf_ve_machines)/sizeof(helf_ve_machines[0]),HELF_ELFCLASS_64);
}

size_t helf_ve_dynamic_count_get(helf_file_input_t *input_file)
{
    if(!helf_ve_is_ve(input_file))
    {
        return 0;
    }
    return helf_arch_dynamic_count_get(input_file);
}

bool helf_ve_dynamic_info_get(helf_file_input_t *input_file,size_t index,helf_arch_dynamic_info_t *info)
{
    if(!helf_ve_is_ve(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_info_get(input_file,index,info);
}

bool helf_ve_dynamic_string_get(helf_file_input_t *input_file,uint64_t offset,char *buffer,size_t buffer_length)
{
    if(!helf_ve_is_ve(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_string_get(input_file,offset,buffer,buffer_length);
}
