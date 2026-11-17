/***************************************************************
 * Name:      helf_arch_alpha.c
 * Purpose:   实现helf_arch_alpha接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "helf_arch_alpha.h"

static const uint16_t helf_alpha_machines[]=
{
    HELF_EM_ALPHA,
    HELF_EM_OLD_ALPHA,
    HELF_EM_ALPHA_STD,
};

bool helf_alpha_is_alpha(helf_file_input_t *input_file)
{
    return helf_arch_is(input_file,helf_alpha_machines,sizeof(helf_alpha_machines)/sizeof(helf_alpha_machines[0]),HELF_ELFCLASS_64);
}

bool helf_alpha_is_big_endian(helf_file_input_t *input_file)
{
    if(!helf_alpha_is_alpha(input_file))
    {
        return false;
    }
    return helf_file_input_is_big_endian(input_file);
}

bool helf_alpha_is_executable(helf_file_input_t *input_file)
{
    return helf_arch_is_executable(input_file,helf_alpha_machines,sizeof(helf_alpha_machines)/sizeof(helf_alpha_machines[0]),HELF_ELFCLASS_64);
}

bool helf_alpha_entry_get(helf_file_input_t *input_file,uint64_t *entry)
{
    if(entry==NULL)
    {
        return false;
    }
    if(!helf_alpha_is_alpha(input_file))
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

size_t helf_alpha_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_alpha_is_alpha(input_file))
    {
        return 0;
    }
    return helf_arch_section_count_get(input_file);
}

bool helf_alpha_section_info_get(helf_file_input_t *input_file,size_t index,helf_alpha_section_info_t *info)
{
    if(!helf_alpha_is_alpha(input_file))
    {
        return false;
    }
    return helf_arch_section_info_get(input_file,index,info);
}

bool helf_alpha_is_rx_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_alpha_is_alpha(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_section(index,input_file);
}

intptr_t helf_alpha_rx_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_alpha_is_rx_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rx_section_offset_get(index,input_file);
}

size_t helf_alpha_rx_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_alpha_is_alpha(input_file))
    {
        return 0;
    }
    return helf_arch_rx_section_size_get(input_file);
}

bool helf_alpha_is_ro_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_alpha_is_alpha(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_section(index,input_file);
}

intptr_t helf_alpha_ro_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_alpha_is_ro_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_ro_section_offset_get(index,input_file);
}

size_t helf_alpha_ro_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_alpha_is_alpha(input_file))
    {
        return 0;
    }
    return helf_arch_ro_section_size_get(input_file);
}

bool helf_alpha_is_rw_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_alpha_is_alpha(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_section(index,input_file);
}

intptr_t helf_alpha_rw_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_alpha_is_rw_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rw_section_offset_get(index,input_file);
}

size_t helf_alpha_rw_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_alpha_is_alpha(input_file))
    {
        return 0;
    }
    return helf_arch_rw_section_size_get(input_file);
}

bool helf_alpha_is_bss_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_alpha_is_alpha(input_file))
    {
        return false;
    }
    return helf_arch_is_bss_section(index,input_file);
}

size_t helf_alpha_program_header_count_get(helf_file_input_t *input_file)
{
    if(!helf_alpha_is_alpha(input_file))
    {
        return 0;
    }
    return helf_arch_program_header_count_get(input_file);
}

bool helf_alpha_program_header_get(helf_file_input_t *input_file,size_t index,helf_elf64_program_header_t *programheader)
{
    if(programheader==NULL)
    {
        return false;
    }
    if(!helf_alpha_is_alpha(input_file))
    {
        return false;
    }
    if(index>=helf_arch_program_header_count_get(input_file))
    {
        return false;
    }
    return helf_file_input_64_bits_program_header_get(input_file,index,programheader);
}

size_t helf_alpha_segment_count_get(helf_file_input_t *input_file)
{
    return helf_alpha_program_header_count_get(input_file);
}

bool helf_alpha_segment_info_get(helf_file_input_t *input_file,size_t index,helf_alpha_segment_info_t *info)
{
    if(!helf_alpha_is_alpha(input_file))
    {
        return false;
    }
    return helf_arch_segment_info_get(input_file,index,info);
}

bool helf_alpha_is_load_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_alpha_is_alpha(input_file))
    {
        return false;
    }
    return helf_arch_is_load_segment(index,input_file);
}

bool helf_alpha_is_rx_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_alpha_is_alpha(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_segment(index,input_file);
}

bool helf_alpha_is_ro_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_alpha_is_alpha(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_segment(index,input_file);
}

bool helf_alpha_is_rw_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_alpha_is_alpha(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_segment(index,input_file);
}

size_t helf_alpha_load_segment_count_get(helf_file_input_t *input_file)
{
    if(!helf_alpha_is_alpha(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_count_get(input_file);
}

bool helf_alpha_load_segment_info_get(helf_file_input_t *input_file,size_t index,helf_alpha_segment_info_t *info)
{
    if(!helf_alpha_is_alpha(input_file))
    {
        return false;
    }
    return helf_arch_load_segment_info_get(input_file,index,info);
}

size_t helf_alpha_load_segment_total_filesz_get(helf_file_input_t *input_file)
{
    if(!helf_alpha_is_alpha(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_filesz_get(input_file);
}

size_t helf_alpha_load_segment_total_memsz_get(helf_file_input_t *input_file)
{
    if(!helf_alpha_is_alpha(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_memsz_get(input_file);
}

size_t helf_alpha_relocation_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_alpha_is_alpha(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_section_count_get(input_file);
}

bool helf_alpha_relocation_section_info_get(helf_file_input_t *input_file,size_t index,helf_arch_relocation_section_info_t *info)
{
    if(!helf_alpha_is_alpha(input_file))
    {
        return false;
    }
    return helf_arch_relocation_section_info_get(input_file,index,info);
}

size_t helf_alpha_relocation_count_get(helf_file_input_t *input_file,size_t section_index)
{
    if(!helf_alpha_is_alpha(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_count_get(input_file,section_index);
}

bool helf_alpha_relocation_get(helf_file_input_t *input_file,size_t section_index,size_t index,helf_arch_relocation_info_t *info)
{
    if(!helf_alpha_is_alpha(input_file))
    {
        return false;
    }
    if(!helf_arch_relocation_get(input_file,section_index,index,info))
    {
        return false;
    }
    return true;
}

bool helf_alpha_relocation_symbol_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_symbol_info_t *info)
{
    if(!helf_alpha_is_alpha(input_file))
    {
        return false;
    }
    return helf_arch_relocation_symbol_info_get(input_file,relocation,info);
}

static const helf_arch_relocation_type_name_t helf_alpha_relocation_type_names[]=
{
    {0,0,"R_ALPHA_NONE"},
    {1,4,"R_ALPHA_REFLONG"},
    {2,8,"R_ALPHA_REFQUAD"},
    {3,4,"R_ALPHA_GPREL32"},
    {4,2,"R_ALPHA_LITERAL"},
    {5,2,"R_ALPHA_LITUSE"},
    {6,4,"R_ALPHA_GPDISP"},
    {7,4,"R_ALPHA_BRADDR"},
    {8,2,"R_ALPHA_HINT"},
    {9,2,"R_ALPHA_SREL16"},
    {10,4,"R_ALPHA_SREL32"},
    {11,8,"R_ALPHA_SREL64"},
    {17,2,"R_ALPHA_GPRELHIGH"},
    {18,2,"R_ALPHA_GPRELLOW"},
    {19,2,"R_ALPHA_GPREL16"},
    {24,0,"R_ALPHA_COPY"},
    {25,0,"R_ALPHA_GLOB_DAT"},
    {26,0,"R_ALPHA_JMP_SLOT"},
    {27,0,"R_ALPHA_RELATIVE"},
    {28,4,"R_ALPHA_BRSGP"},
    {29,2,"R_ALPHA_TLSGD"},
    {30,2,"R_ALPHA_TLSLDM"},
    {31,8,"R_ALPHA_DTPMOD64"},
    {32,2,"R_ALPHA_GOTDTPREL"},
    {33,8,"R_ALPHA_DTPREL64"},
    {34,2,"R_ALPHA_DTPRELHI"},
    {35,2,"R_ALPHA_DTPRELLO"},
    {36,2,"R_ALPHA_DTPREL16"},
    {37,2,"R_ALPHA_GOTTPREL"},
    {38,8,"R_ALPHA_TPREL64"},
    {39,2,"R_ALPHA_TPRELHI"},
    {40,2,"R_ALPHA_TPRELLO"},
    {41,2,"R_ALPHA_TPREL16"},
    {42,0,"R_ALPHA_IRELATIVE"},
};

const char *helf_alpha_relocation_type_name_get(uint32_t type)
{
    return helf_arch_relocation_type_name_get(helf_alpha_relocation_type_names,sizeof(helf_alpha_relocation_type_names)/sizeof(helf_alpha_relocation_type_names[0]),type);
}

uint8_t helf_alpha_relocation_size_get(uint32_t type)
{
    return helf_arch_relocation_type_size_get(helf_alpha_relocation_type_names,sizeof(helf_alpha_relocation_type_names)/sizeof(helf_alpha_relocation_type_names[0]),type);
}

static const helf_arch_section_type_name_t helf_alpha_section_type_names[]=
{
    {1879048193,"SHT_ALPHA_DEBUG"},
    {1879048194,"SHT_ALPHA_REGINFO"},
};

const char *helf_alpha_section_type_name_get(uint32_t type)
{
    return helf_arch_section_type_name_get(helf_alpha_section_type_names,sizeof(helf_alpha_section_type_names)/sizeof(helf_alpha_section_type_names[0]),type);
}

bool helf_alpha_relocation_address_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_relocation_address_info_t *info)
{
    if(!helf_alpha_is_alpha(input_file) || relocation==NULL)
    {
        return false;
    }
    uint8_t size=helf_alpha_relocation_size_get(relocation->type);
    if(size==0)
    {
        return false;
    }
    return helf_arch_relocation_address_info_get(input_file,relocation,size,info);
}

size_t helf_alpha_relocation_data_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,void *buffer,size_t buffer_length)
{
    if(!helf_alpha_is_alpha(input_file) || relocation==NULL)
    {
        return 0;
    }
    uint8_t size=helf_alpha_relocation_size_get(relocation->type);
    if(size==0)
    {
        return 0;
    }
    return helf_arch_relocation_data_get(input_file,relocation,size,buffer,buffer_length);
}

bool helf_alpha_is_shared_object(helf_file_input_t *input_file)
{
    return helf_arch_is_shared_object(input_file,helf_alpha_machines,sizeof(helf_alpha_machines)/sizeof(helf_alpha_machines[0]),HELF_ELFCLASS_64);
}

bool helf_alpha_is_relocatable(helf_file_input_t *input_file)
{
    return helf_arch_is_relocatable(input_file,helf_alpha_machines,sizeof(helf_alpha_machines)/sizeof(helf_alpha_machines[0]),HELF_ELFCLASS_64);
}

size_t helf_alpha_dynamic_count_get(helf_file_input_t *input_file)
{
    if(!helf_alpha_is_alpha(input_file))
    {
        return 0;
    }
    return helf_arch_dynamic_count_get(input_file);
}

bool helf_alpha_dynamic_info_get(helf_file_input_t *input_file,size_t index,helf_arch_dynamic_info_t *info)
{
    if(!helf_alpha_is_alpha(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_info_get(input_file,index,info);
}

bool helf_alpha_dynamic_string_get(helf_file_input_t *input_file,uint64_t offset,char *buffer,size_t buffer_length)
{
    if(!helf_alpha_is_alpha(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_string_get(input_file,offset,buffer,buffer_length);
}
