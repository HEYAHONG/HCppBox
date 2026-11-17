/***************************************************************
 * Name:      helf_arch_mmix.c
 * Purpose:   实现helf_arch_mmix接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "helf_arch_mmix.h"

static const uint16_t helf_mmix_machines[]=
{
    HELF_EM_MMIX,
};

bool helf_mmix_is_mmix(helf_file_input_t *input_file)
{
    return helf_arch_is(input_file,helf_mmix_machines,sizeof(helf_mmix_machines)/sizeof(helf_mmix_machines[0]),HELF_ELFCLASS_64);
}

bool helf_mmix_is_big_endian(helf_file_input_t *input_file)
{
    if(!helf_mmix_is_mmix(input_file))
    {
        return false;
    }
    return helf_file_input_is_big_endian(input_file);
}

bool helf_mmix_is_executable(helf_file_input_t *input_file)
{
    return helf_arch_is_executable(input_file,helf_mmix_machines,sizeof(helf_mmix_machines)/sizeof(helf_mmix_machines[0]),HELF_ELFCLASS_64);
}

bool helf_mmix_entry_get(helf_file_input_t *input_file,uint64_t *entry)
{
    if(entry==NULL)
    {
        return false;
    }
    if(!helf_mmix_is_mmix(input_file))
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

size_t helf_mmix_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_mmix_is_mmix(input_file))
    {
        return 0;
    }
    return helf_arch_section_count_get(input_file);
}

bool helf_mmix_section_info_get(helf_file_input_t *input_file,size_t index,helf_mmix_section_info_t *info)
{
    if(!helf_mmix_is_mmix(input_file))
    {
        return false;
    }
    return helf_arch_section_info_get(input_file,index,info);
}

bool helf_mmix_is_rx_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_mmix_is_mmix(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_section(index,input_file);
}

intptr_t helf_mmix_rx_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_mmix_is_rx_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rx_section_offset_get(index,input_file);
}

size_t helf_mmix_rx_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_mmix_is_mmix(input_file))
    {
        return 0;
    }
    return helf_arch_rx_section_size_get(input_file);
}

bool helf_mmix_is_ro_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_mmix_is_mmix(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_section(index,input_file);
}

intptr_t helf_mmix_ro_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_mmix_is_ro_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_ro_section_offset_get(index,input_file);
}

size_t helf_mmix_ro_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_mmix_is_mmix(input_file))
    {
        return 0;
    }
    return helf_arch_ro_section_size_get(input_file);
}

bool helf_mmix_is_rw_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_mmix_is_mmix(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_section(index,input_file);
}

intptr_t helf_mmix_rw_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_mmix_is_rw_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rw_section_offset_get(index,input_file);
}

size_t helf_mmix_rw_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_mmix_is_mmix(input_file))
    {
        return 0;
    }
    return helf_arch_rw_section_size_get(input_file);
}

bool helf_mmix_is_bss_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_mmix_is_mmix(input_file))
    {
        return false;
    }
    return helf_arch_is_bss_section(index,input_file);
}

size_t helf_mmix_program_header_count_get(helf_file_input_t *input_file)
{
    if(!helf_mmix_is_mmix(input_file))
    {
        return 0;
    }
    return helf_arch_program_header_count_get(input_file);
}

bool helf_mmix_program_header_get(helf_file_input_t *input_file,size_t index,helf_elf64_program_header_t *programheader)
{
    if(programheader==NULL)
    {
        return false;
    }
    if(!helf_mmix_is_mmix(input_file))
    {
        return false;
    }
    if(index>=helf_arch_program_header_count_get(input_file))
    {
        return false;
    }
    return helf_file_input_64_bits_program_header_get(input_file,index,programheader);
}

size_t helf_mmix_segment_count_get(helf_file_input_t *input_file)
{
    return helf_mmix_program_header_count_get(input_file);
}

bool helf_mmix_segment_info_get(helf_file_input_t *input_file,size_t index,helf_mmix_segment_info_t *info)
{
    if(!helf_mmix_is_mmix(input_file))
    {
        return false;
    }
    return helf_arch_segment_info_get(input_file,index,info);
}

bool helf_mmix_is_load_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_mmix_is_mmix(input_file))
    {
        return false;
    }
    return helf_arch_is_load_segment(index,input_file);
}

bool helf_mmix_is_rx_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_mmix_is_mmix(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_segment(index,input_file);
}

bool helf_mmix_is_ro_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_mmix_is_mmix(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_segment(index,input_file);
}

bool helf_mmix_is_rw_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_mmix_is_mmix(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_segment(index,input_file);
}

size_t helf_mmix_load_segment_count_get(helf_file_input_t *input_file)
{
    if(!helf_mmix_is_mmix(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_count_get(input_file);
}

bool helf_mmix_load_segment_info_get(helf_file_input_t *input_file,size_t index,helf_mmix_segment_info_t *info)
{
    if(!helf_mmix_is_mmix(input_file))
    {
        return false;
    }
    return helf_arch_load_segment_info_get(input_file,index,info);
}

size_t helf_mmix_load_segment_total_filesz_get(helf_file_input_t *input_file)
{
    if(!helf_mmix_is_mmix(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_filesz_get(input_file);
}

size_t helf_mmix_load_segment_total_memsz_get(helf_file_input_t *input_file)
{
    if(!helf_mmix_is_mmix(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_memsz_get(input_file);
}

size_t helf_mmix_relocation_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_mmix_is_mmix(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_section_count_get(input_file);
}

bool helf_mmix_relocation_section_info_get(helf_file_input_t *input_file,size_t index,helf_arch_relocation_section_info_t *info)
{
    if(!helf_mmix_is_mmix(input_file))
    {
        return false;
    }
    return helf_arch_relocation_section_info_get(input_file,index,info);
}

size_t helf_mmix_relocation_count_get(helf_file_input_t *input_file,size_t section_index)
{
    if(!helf_mmix_is_mmix(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_count_get(input_file,section_index);
}

bool helf_mmix_relocation_get(helf_file_input_t *input_file,size_t section_index,size_t index,helf_arch_relocation_info_t *info)
{
    if(!helf_mmix_is_mmix(input_file))
    {
        return false;
    }
    if(!helf_arch_relocation_get(input_file,section_index,index,info))
    {
        return false;
    }
    return true;
}

bool helf_mmix_relocation_symbol_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_symbol_info_t *info)
{
    if(!helf_mmix_is_mmix(input_file))
    {
        return false;
    }
    return helf_arch_relocation_symbol_info_get(input_file,relocation,info);
}

static const helf_arch_relocation_type_name_t helf_mmix_relocation_type_names[]=
{
    {0,0,"R_MMIX_NONE"},
    {1,1,"R_MMIX_8"},
    {2,2,"R_MMIX_16"},
    {3,4,"R_MMIX_24"},
    {4,4,"R_MMIX_32"},
    {5,8,"R_MMIX_64"},
    {6,1,"R_MMIX_PC_8"},
    {7,2,"R_MMIX_PC_16"},
    {8,4,"R_MMIX_PC_24"},
    {9,4,"R_MMIX_PC_32"},
    {10,8,"R_MMIX_PC_64"},
    {11,0,"R_MMIX_GNU_VTINHERIT"},
    {12,0,"R_MMIX_GNU_VTENTRY"},
    {13,4,"R_MMIX_GETA"},
    {14,4,"R_MMIX_GETA_1"},
    {15,4,"R_MMIX_GETA_2"},
    {16,4,"R_MMIX_GETA_3"},
    {17,4,"R_MMIX_CBRANCH"},
    {18,4,"R_MMIX_CBRANCH_J"},
    {19,4,"R_MMIX_CBRANCH_1"},
    {20,4,"R_MMIX_CBRANCH_2"},
    {21,4,"R_MMIX_CBRANCH_3"},
    {22,4,"R_MMIX_PUSHJ"},
    {23,4,"R_MMIX_PUSHJ_1"},
    {24,4,"R_MMIX_PUSHJ_2"},
    {25,4,"R_MMIX_PUSHJ_3"},
    {26,4,"R_MMIX_JMP"},
    {27,4,"R_MMIX_JMP_1"},
    {28,4,"R_MMIX_JMP_2"},
    {29,4,"R_MMIX_JMP_3"},
    {30,4,"R_MMIX_ADDR19"},
    {31,4,"R_MMIX_ADDR27"},
    {32,2,"R_MMIX_REG_OR_BYTE"},
    {33,2,"R_MMIX_REG"},
    {34,2,"R_MMIX_BASE_PLUS_OFFSET"},
    {35,0,"R_MMIX_LOCAL"},
    {36,4,"R_MMIX_PUSHJ_STUBBABLE"},
};

const char *helf_mmix_relocation_type_name_get(uint32_t type)
{
    return helf_arch_relocation_type_name_get(helf_mmix_relocation_type_names,sizeof(helf_mmix_relocation_type_names)/sizeof(helf_mmix_relocation_type_names[0]),type);
}

uint8_t helf_mmix_relocation_size_get(uint32_t type)
{
    return helf_arch_relocation_type_size_get(helf_mmix_relocation_type_names,sizeof(helf_mmix_relocation_type_names)/sizeof(helf_mmix_relocation_type_names[0]),type);
}

const char *helf_mmix_section_type_name_get(uint32_t type)
{
    (void)type;
    return NULL;
}

bool helf_mmix_relocation_address_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_relocation_address_info_t *info)
{
    if(!helf_mmix_is_mmix(input_file) || relocation==NULL)
    {
        return false;
    }
    uint8_t size=helf_mmix_relocation_size_get(relocation->type);
    if(size==0)
    {
        return false;
    }
    return helf_arch_relocation_address_info_get(input_file,relocation,size,info);
}

size_t helf_mmix_relocation_data_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,void *buffer,size_t buffer_length)
{
    if(!helf_mmix_is_mmix(input_file) || relocation==NULL)
    {
        return 0;
    }
    uint8_t size=helf_mmix_relocation_size_get(relocation->type);
    if(size==0)
    {
        return 0;
    }
    return helf_arch_relocation_data_get(input_file,relocation,size,buffer,buffer_length);
}

bool helf_mmix_is_shared_object(helf_file_input_t *input_file)
{
    return helf_arch_is_shared_object(input_file,helf_mmix_machines,sizeof(helf_mmix_machines)/sizeof(helf_mmix_machines[0]),HELF_ELFCLASS_64);
}

bool helf_mmix_is_relocatable(helf_file_input_t *input_file)
{
    return helf_arch_is_relocatable(input_file,helf_mmix_machines,sizeof(helf_mmix_machines)/sizeof(helf_mmix_machines[0]),HELF_ELFCLASS_64);
}

size_t helf_mmix_dynamic_count_get(helf_file_input_t *input_file)
{
    if(!helf_mmix_is_mmix(input_file))
    {
        return 0;
    }
    return helf_arch_dynamic_count_get(input_file);
}

bool helf_mmix_dynamic_info_get(helf_file_input_t *input_file,size_t index,helf_arch_dynamic_info_t *info)
{
    if(!helf_mmix_is_mmix(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_info_get(input_file,index,info);
}

bool helf_mmix_dynamic_string_get(helf_file_input_t *input_file,uint64_t offset,char *buffer,size_t buffer_length)
{
    if(!helf_mmix_is_mmix(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_string_get(input_file,offset,buffer,buffer_length);
}
