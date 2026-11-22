/***************************************************************
 * Name:      helf_arch_tachyum.c
 * Purpose:   实现helf_arch_tachyum接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "helf_arch_tachyum.h"

static const uint16_t helf_tachyum_machines[]=
{
    HELF_EM_TACHYUM,
};

bool helf_tachyum_is_tachyum(helf_file_input_t *input_file)
{
    return helf_arch_is(input_file,helf_tachyum_machines,sizeof(helf_tachyum_machines)/sizeof(helf_tachyum_machines[0]),HELF_ELFCLASS_64);
}

bool helf_tachyum_is_big_endian(helf_file_input_t *input_file)
{
    if(!helf_tachyum_is_tachyum(input_file))
    {
        return false;
    }
    return helf_file_input_is_big_endian(input_file);
}

bool helf_tachyum_is_executable(helf_file_input_t *input_file)
{
    return helf_arch_is_executable(input_file,helf_tachyum_machines,sizeof(helf_tachyum_machines)/sizeof(helf_tachyum_machines[0]),HELF_ELFCLASS_64);
}

bool helf_tachyum_entry_get(helf_file_input_t *input_file,uint64_t *entry)
{
    if(entry==NULL)
    {
        return false;
    }
    if(!helf_tachyum_is_tachyum(input_file))
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

size_t helf_tachyum_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_tachyum_is_tachyum(input_file))
    {
        return 0;
    }
    return helf_arch_section_count_get(input_file);
}

bool helf_tachyum_section_info_get(helf_file_input_t *input_file,size_t index,helf_tachyum_section_info_t *info)
{
    if(!helf_tachyum_is_tachyum(input_file))
    {
        return false;
    }
    return helf_arch_section_info_get(input_file,index,info);
}

bool helf_tachyum_is_rx_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tachyum_is_tachyum(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_section(index,input_file);
}

intptr_t helf_tachyum_rx_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tachyum_is_rx_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rx_section_offset_get(index,input_file);
}

size_t helf_tachyum_rx_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_tachyum_is_tachyum(input_file))
    {
        return 0;
    }
    return helf_arch_rx_section_size_get(input_file);
}

bool helf_tachyum_is_ro_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tachyum_is_tachyum(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_section(index,input_file);
}

intptr_t helf_tachyum_ro_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tachyum_is_ro_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_ro_section_offset_get(index,input_file);
}

size_t helf_tachyum_ro_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_tachyum_is_tachyum(input_file))
    {
        return 0;
    }
    return helf_arch_ro_section_size_get(input_file);
}

bool helf_tachyum_is_rw_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tachyum_is_tachyum(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_section(index,input_file);
}

intptr_t helf_tachyum_rw_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tachyum_is_rw_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rw_section_offset_get(index,input_file);
}

size_t helf_tachyum_rw_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_tachyum_is_tachyum(input_file))
    {
        return 0;
    }
    return helf_arch_rw_section_size_get(input_file);
}

bool helf_tachyum_is_bss_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tachyum_is_tachyum(input_file))
    {
        return false;
    }
    return helf_arch_is_bss_section(index,input_file);
}

size_t helf_tachyum_program_header_count_get(helf_file_input_t *input_file)
{
    if(!helf_tachyum_is_tachyum(input_file))
    {
        return 0;
    }
    return helf_arch_program_header_count_get(input_file);
}

bool helf_tachyum_program_header_get(helf_file_input_t *input_file,size_t index,helf_elf64_program_header_t *programheader)
{
    if(programheader==NULL)
    {
        return false;
    }
    if(!helf_tachyum_is_tachyum(input_file))
    {
        return false;
    }
    if(index>=helf_arch_program_header_count_get(input_file))
    {
        return false;
    }
    return helf_file_input_64_bits_program_header_get(input_file,index,programheader);
}

size_t helf_tachyum_segment_count_get(helf_file_input_t *input_file)
{
    return helf_tachyum_program_header_count_get(input_file);
}

bool helf_tachyum_segment_info_get(helf_file_input_t *input_file,size_t index,helf_tachyum_segment_info_t *info)
{
    if(!helf_tachyum_is_tachyum(input_file))
    {
        return false;
    }
    return helf_arch_segment_info_get(input_file,index,info);
}

bool helf_tachyum_is_load_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tachyum_is_tachyum(input_file))
    {
        return false;
    }
    return helf_arch_is_load_segment(index,input_file);
}

bool helf_tachyum_is_rx_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tachyum_is_tachyum(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_segment(index,input_file);
}

bool helf_tachyum_is_ro_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tachyum_is_tachyum(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_segment(index,input_file);
}

bool helf_tachyum_is_rw_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tachyum_is_tachyum(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_segment(index,input_file);
}

size_t helf_tachyum_load_segment_count_get(helf_file_input_t *input_file)
{
    if(!helf_tachyum_is_tachyum(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_count_get(input_file);
}

bool helf_tachyum_load_segment_info_get(helf_file_input_t *input_file,size_t index,helf_tachyum_segment_info_t *info)
{
    if(!helf_tachyum_is_tachyum(input_file))
    {
        return false;
    }
    return helf_arch_load_segment_info_get(input_file,index,info);
}

size_t helf_tachyum_load_segment_total_filesz_get(helf_file_input_t *input_file)
{
    if(!helf_tachyum_is_tachyum(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_filesz_get(input_file);
}

size_t helf_tachyum_load_segment_total_memsz_get(helf_file_input_t *input_file)
{
    if(!helf_tachyum_is_tachyum(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_memsz_get(input_file);
}

size_t helf_tachyum_relocation_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_tachyum_is_tachyum(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_section_count_get(input_file);
}

bool helf_tachyum_relocation_section_info_get(helf_file_input_t *input_file,size_t index,helf_arch_relocation_section_info_t *info)
{
    if(!helf_tachyum_is_tachyum(input_file))
    {
        return false;
    }
    return helf_arch_relocation_section_info_get(input_file,index,info);
}

size_t helf_tachyum_relocation_count_get(helf_file_input_t *input_file,size_t section_index)
{
    if(!helf_tachyum_is_tachyum(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_count_get(input_file,section_index);
}

bool helf_tachyum_relocation_get(helf_file_input_t *input_file,size_t section_index,size_t index,helf_arch_relocation_info_t *info)
{
    if(!helf_tachyum_is_tachyum(input_file))
    {
        return false;
    }
    if(!helf_arch_relocation_get(input_file,section_index,index,info))
    {
        return false;
    }
    return true;
}

bool helf_tachyum_relocation_symbol_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_symbol_info_t *info)
{
    if(!helf_tachyum_is_tachyum(input_file))
    {
        return false;
    }
    return helf_arch_relocation_symbol_info_get(input_file,relocation,info);
}

const char *helf_tachyum_relocation_type_name_get(uint32_t type)
{
    (void)type;
    return NULL;
}

uint8_t helf_tachyum_relocation_size_get(uint32_t type)
{
    (void)type;
    return 0;
}

const char *helf_tachyum_section_type_name_get(uint32_t type)
{
    (void)type;
    return NULL;
}

bool helf_tachyum_relocation_address_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_relocation_address_info_t *info)
{
    if(!helf_tachyum_is_tachyum(input_file) || relocation==NULL)
    {
        return false;
    }
    uint8_t size=helf_tachyum_relocation_size_get(relocation->type);
    if(size==0)
    {
        return false;
    }
    return helf_arch_relocation_address_info_get(input_file,relocation,size,info);
}

size_t helf_tachyum_relocation_data_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,void *buffer,size_t buffer_length)
{
    if(!helf_tachyum_is_tachyum(input_file) || relocation==NULL)
    {
        return 0;
    }
    uint8_t size=helf_tachyum_relocation_size_get(relocation->type);
    if(size==0)
    {
        return 0;
    }
    return helf_arch_relocation_data_get(input_file,relocation,size,buffer,buffer_length);
}

bool helf_tachyum_is_shared_object(helf_file_input_t *input_file)
{
    return helf_arch_is_shared_object(input_file,helf_tachyum_machines,sizeof(helf_tachyum_machines)/sizeof(helf_tachyum_machines[0]),HELF_ELFCLASS_64);
}

bool helf_tachyum_is_relocatable(helf_file_input_t *input_file)
{
    return helf_arch_is_relocatable(input_file,helf_tachyum_machines,sizeof(helf_tachyum_machines)/sizeof(helf_tachyum_machines[0]),HELF_ELFCLASS_64);
}

size_t helf_tachyum_dynamic_count_get(helf_file_input_t *input_file)
{
    if(!helf_tachyum_is_tachyum(input_file))
    {
        return 0;
    }
    return helf_arch_dynamic_count_get(input_file);
}

bool helf_tachyum_dynamic_info_get(helf_file_input_t *input_file,size_t index,helf_arch_dynamic_info_t *info)
{
    if(!helf_tachyum_is_tachyum(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_info_get(input_file,index,info);
}

bool helf_tachyum_dynamic_string_get(helf_file_input_t *input_file,uint64_t offset,char *buffer,size_t buffer_length)
{
    if(!helf_tachyum_is_tachyum(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_string_get(input_file,offset,buffer,buffer_length);
}

/* helf_relocation_howto_begin */
bool helf_tachyum_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto)
{
    (void)type;
    (void)howto;
    return false;
}
/* helf_relocation_howto_end */


