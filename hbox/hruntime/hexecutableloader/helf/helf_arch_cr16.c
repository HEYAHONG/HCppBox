/***************************************************************
 * Name:      helf_arch_cr16.c
 * Purpose:   实现helf_arch_cr16接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "helf_arch_cr16.h"

static const uint16_t helf_cr16_machines[]=
{
    HELF_EM_CR16,
};

bool helf_cr16_is_cr16(helf_file_input_t *input_file)
{
    return helf_arch_is(input_file,helf_cr16_machines,sizeof(helf_cr16_machines)/sizeof(helf_cr16_machines[0]),HELF_ELFCLASS_32);
}

bool helf_cr16_is_big_endian(helf_file_input_t *input_file)
{
    if(!helf_cr16_is_cr16(input_file))
    {
        return false;
    }
    return helf_file_input_is_big_endian(input_file);
}

bool helf_cr16_is_executable(helf_file_input_t *input_file)
{
    return helf_arch_is_executable(input_file,helf_cr16_machines,sizeof(helf_cr16_machines)/sizeof(helf_cr16_machines[0]),HELF_ELFCLASS_32);
}

bool helf_cr16_entry_get(helf_file_input_t *input_file,uint32_t *entry)
{
    if(entry==NULL)
    {
        return false;
    }
    if(!helf_cr16_is_cr16(input_file))
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

size_t helf_cr16_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_cr16_is_cr16(input_file))
    {
        return 0;
    }
    return helf_arch_section_count_get(input_file);
}

bool helf_cr16_section_info_get(helf_file_input_t *input_file,size_t index,helf_cr16_section_info_t *info)
{
    if(!helf_cr16_is_cr16(input_file))
    {
        return false;
    }
    return helf_arch_section_info_get(input_file,index,info);
}

bool helf_cr16_is_rx_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_cr16_is_cr16(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_section(index,input_file);
}

intptr_t helf_cr16_rx_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_cr16_is_rx_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rx_section_offset_get(index,input_file);
}

size_t helf_cr16_rx_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_cr16_is_cr16(input_file))
    {
        return 0;
    }
    return helf_arch_rx_section_size_get(input_file);
}

bool helf_cr16_is_ro_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_cr16_is_cr16(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_section(index,input_file);
}

intptr_t helf_cr16_ro_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_cr16_is_ro_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_ro_section_offset_get(index,input_file);
}

size_t helf_cr16_ro_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_cr16_is_cr16(input_file))
    {
        return 0;
    }
    return helf_arch_ro_section_size_get(input_file);
}

bool helf_cr16_is_rw_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_cr16_is_cr16(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_section(index,input_file);
}

intptr_t helf_cr16_rw_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_cr16_is_rw_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rw_section_offset_get(index,input_file);
}

size_t helf_cr16_rw_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_cr16_is_cr16(input_file))
    {
        return 0;
    }
    return helf_arch_rw_section_size_get(input_file);
}

bool helf_cr16_is_bss_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_cr16_is_cr16(input_file))
    {
        return false;
    }
    return helf_arch_is_bss_section(index,input_file);
}

size_t helf_cr16_program_header_count_get(helf_file_input_t *input_file)
{
    if(!helf_cr16_is_cr16(input_file))
    {
        return 0;
    }
    return helf_arch_program_header_count_get(input_file);
}

bool helf_cr16_program_header_get(helf_file_input_t *input_file,size_t index,helf_elf32_program_header_t *programheader)
{
    if(programheader==NULL)
    {
        return false;
    }
    if(!helf_cr16_is_cr16(input_file))
    {
        return false;
    }
    if(index>=helf_arch_program_header_count_get(input_file))
    {
        return false;
    }
    return helf_file_input_32_bits_program_header_get(input_file,index,programheader);
}

size_t helf_cr16_segment_count_get(helf_file_input_t *input_file)
{
    return helf_cr16_program_header_count_get(input_file);
}

bool helf_cr16_segment_info_get(helf_file_input_t *input_file,size_t index,helf_cr16_segment_info_t *info)
{
    if(!helf_cr16_is_cr16(input_file))
    {
        return false;
    }
    return helf_arch_segment_info_get(input_file,index,info);
}

bool helf_cr16_is_load_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_cr16_is_cr16(input_file))
    {
        return false;
    }
    return helf_arch_is_load_segment(index,input_file);
}

bool helf_cr16_is_rx_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_cr16_is_cr16(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_segment(index,input_file);
}

bool helf_cr16_is_ro_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_cr16_is_cr16(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_segment(index,input_file);
}

bool helf_cr16_is_rw_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_cr16_is_cr16(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_segment(index,input_file);
}

size_t helf_cr16_load_segment_count_get(helf_file_input_t *input_file)
{
    if(!helf_cr16_is_cr16(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_count_get(input_file);
}

bool helf_cr16_load_segment_info_get(helf_file_input_t *input_file,size_t index,helf_cr16_segment_info_t *info)
{
    if(!helf_cr16_is_cr16(input_file))
    {
        return false;
    }
    return helf_arch_load_segment_info_get(input_file,index,info);
}

size_t helf_cr16_load_segment_total_filesz_get(helf_file_input_t *input_file)
{
    if(!helf_cr16_is_cr16(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_filesz_get(input_file);
}

size_t helf_cr16_load_segment_total_memsz_get(helf_file_input_t *input_file)
{
    if(!helf_cr16_is_cr16(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_memsz_get(input_file);
}

size_t helf_cr16_relocation_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_cr16_is_cr16(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_section_count_get(input_file);
}

bool helf_cr16_relocation_section_info_get(helf_file_input_t *input_file,size_t index,helf_arch_relocation_section_info_t *info)
{
    if(!helf_cr16_is_cr16(input_file))
    {
        return false;
    }
    return helf_arch_relocation_section_info_get(input_file,index,info);
}

size_t helf_cr16_relocation_count_get(helf_file_input_t *input_file,size_t section_index)
{
    if(!helf_cr16_is_cr16(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_count_get(input_file,section_index);
}

bool helf_cr16_relocation_get(helf_file_input_t *input_file,size_t section_index,size_t index,helf_arch_relocation_info_t *info)
{
    if(!helf_cr16_is_cr16(input_file))
    {
        return false;
    }
    if(!helf_arch_relocation_get(input_file,section_index,index,info))
    {
        return false;
    }
    return true;
}

bool helf_cr16_relocation_symbol_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_symbol_info_t *info)
{
    if(!helf_cr16_is_cr16(input_file))
    {
        return false;
    }
    return helf_arch_relocation_symbol_info_get(input_file,relocation,info);
}

static const helf_arch_relocation_type_name_t helf_cr16_relocation_type_names[]=
{
    {0,0,"R_CR16_NONE"},
    {1,1,"R_CR16_NUM8"},
    {2,2,"R_CR16_NUM16"},
    {3,4,"R_CR16_NUM32"},
    {4,4,"R_CR16_NUM32a"},
    {5,1,"R_CR16_REGREL4"},
    {6,1,"R_CR16_REGREL4a"},
    {7,2,"R_CR16_REGREL14"},
    {8,2,"R_CR16_REGREL14a"},
    {9,2,"R_CR16_REGREL16"},
    {10,4,"R_CR16_REGREL20"},
    {11,4,"R_CR16_REGREL20a"},
    {12,4,"R_CR16_ABS20"},
    {13,4,"R_CR16_ABS24"},
    {14,1,"R_CR16_IMM4"},
    {15,1,"R_CR16_IMM8"},
    {16,2,"R_CR16_IMM16"},
    {17,4,"R_CR16_IMM20"},
    {18,4,"R_CR16_IMM24"},
    {19,4,"R_CR16_IMM32"},
    {20,4,"R_CR16_IMM32a"},
    {21,1,"R_CR16_DISP4"},
    {22,1,"R_CR16_DISP8"},
    {23,2,"R_CR16_DISP16"},
    {24,4,"R_CR16_DISP24"},
    {25,4,"R_CR16_DISP24a"},
    {26,1,"R_CR16_SWITCH8"},
    {27,2,"R_CR16_SWITCH16"},
    {28,4,"R_CR16_SWITCH32"},
    {29,4,"R_CR16_GOT_REGREL20"},
    {30,4,"R_CR16_GOTC_REGREL20"},
    {31,4,"R_CR16_GLOB_DAT"},
};

const char *helf_cr16_relocation_type_name_get(uint32_t type)
{
    return helf_arch_relocation_type_name_get(helf_cr16_relocation_type_names,sizeof(helf_cr16_relocation_type_names)/sizeof(helf_cr16_relocation_type_names[0]),type);
}

uint8_t helf_cr16_relocation_size_get(uint32_t type)
{
    return helf_arch_relocation_type_size_get(helf_cr16_relocation_type_names,sizeof(helf_cr16_relocation_type_names)/sizeof(helf_cr16_relocation_type_names[0]),type);
}

const char *helf_cr16_section_type_name_get(uint32_t type)
{
    (void)type;
    return NULL;
}

bool helf_cr16_relocation_address_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_relocation_address_info_t *info)
{
    if(!helf_cr16_is_cr16(input_file) || relocation==NULL)
    {
        return false;
    }
    uint8_t size=helf_cr16_relocation_size_get(relocation->type);
    if(size==0)
    {
        return false;
    }
    return helf_arch_relocation_address_info_get(input_file,relocation,size,info);
}

size_t helf_cr16_relocation_data_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,void *buffer,size_t buffer_length)
{
    if(!helf_cr16_is_cr16(input_file) || relocation==NULL)
    {
        return 0;
    }
    uint8_t size=helf_cr16_relocation_size_get(relocation->type);
    if(size==0)
    {
        return 0;
    }
    return helf_arch_relocation_data_get(input_file,relocation,size,buffer,buffer_length);
}

bool helf_cr16_is_shared_object(helf_file_input_t *input_file)
{
    return helf_arch_is_shared_object(input_file,helf_cr16_machines,sizeof(helf_cr16_machines)/sizeof(helf_cr16_machines[0]),HELF_ELFCLASS_32);
}

bool helf_cr16_is_relocatable(helf_file_input_t *input_file)
{
    return helf_arch_is_relocatable(input_file,helf_cr16_machines,sizeof(helf_cr16_machines)/sizeof(helf_cr16_machines[0]),HELF_ELFCLASS_32);
}

size_t helf_cr16_dynamic_count_get(helf_file_input_t *input_file)
{
    if(!helf_cr16_is_cr16(input_file))
    {
        return 0;
    }
    return helf_arch_dynamic_count_get(input_file);
}

bool helf_cr16_dynamic_info_get(helf_file_input_t *input_file,size_t index,helf_arch_dynamic_info_t *info)
{
    if(!helf_cr16_is_cr16(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_info_get(input_file,index,info);
}

bool helf_cr16_dynamic_string_get(helf_file_input_t *input_file,uint64_t offset,char *buffer,size_t buffer_length)
{
    if(!helf_cr16_is_cr16(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_string_get(input_file,offset,buffer,buffer_length);
}
