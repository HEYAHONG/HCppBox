/***************************************************************
 * Name:      helf_arch_rl78.c
 * Purpose:   实现helf_arch_rl78接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "helf_arch_rl78.h"

static const uint16_t helf_rl78_machines[]=
{
    HELF_EM_RL78,
};

bool helf_rl78_is_rl78(helf_file_input_t *input_file)
{
    return helf_arch_is(input_file,helf_rl78_machines,sizeof(helf_rl78_machines)/sizeof(helf_rl78_machines[0]),HELF_ELFCLASS_32);
}

bool helf_rl78_is_big_endian(helf_file_input_t *input_file)
{
    if(!helf_rl78_is_rl78(input_file))
    {
        return false;
    }
    return helf_file_input_is_big_endian(input_file);
}

bool helf_rl78_is_executable(helf_file_input_t *input_file)
{
    return helf_arch_is_executable(input_file,helf_rl78_machines,sizeof(helf_rl78_machines)/sizeof(helf_rl78_machines[0]),HELF_ELFCLASS_32);
}

bool helf_rl78_entry_get(helf_file_input_t *input_file,uint32_t *entry)
{
    if(entry==NULL)
    {
        return false;
    }
    if(!helf_rl78_is_rl78(input_file))
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

size_t helf_rl78_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_rl78_is_rl78(input_file))
    {
        return 0;
    }
    return helf_arch_section_count_get(input_file);
}

bool helf_rl78_section_info_get(helf_file_input_t *input_file,size_t index,helf_rl78_section_info_t *info)
{
    if(!helf_rl78_is_rl78(input_file))
    {
        return false;
    }
    return helf_arch_section_info_get(input_file,index,info);
}

bool helf_rl78_is_rx_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_rl78_is_rl78(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_section(index,input_file);
}

intptr_t helf_rl78_rx_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_rl78_is_rx_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rx_section_offset_get(index,input_file);
}

size_t helf_rl78_rx_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_rl78_is_rl78(input_file))
    {
        return 0;
    }
    return helf_arch_rx_section_size_get(input_file);
}

bool helf_rl78_is_ro_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_rl78_is_rl78(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_section(index,input_file);
}

intptr_t helf_rl78_ro_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_rl78_is_ro_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_ro_section_offset_get(index,input_file);
}

size_t helf_rl78_ro_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_rl78_is_rl78(input_file))
    {
        return 0;
    }
    return helf_arch_ro_section_size_get(input_file);
}

bool helf_rl78_is_rw_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_rl78_is_rl78(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_section(index,input_file);
}

intptr_t helf_rl78_rw_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_rl78_is_rw_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rw_section_offset_get(index,input_file);
}

size_t helf_rl78_rw_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_rl78_is_rl78(input_file))
    {
        return 0;
    }
    return helf_arch_rw_section_size_get(input_file);
}

bool helf_rl78_is_bss_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_rl78_is_rl78(input_file))
    {
        return false;
    }
    return helf_arch_is_bss_section(index,input_file);
}

size_t helf_rl78_program_header_count_get(helf_file_input_t *input_file)
{
    if(!helf_rl78_is_rl78(input_file))
    {
        return 0;
    }
    return helf_arch_program_header_count_get(input_file);
}

bool helf_rl78_program_header_get(helf_file_input_t *input_file,size_t index,helf_elf32_program_header_t *programheader)
{
    if(programheader==NULL)
    {
        return false;
    }
    if(!helf_rl78_is_rl78(input_file))
    {
        return false;
    }
    if(index>=helf_arch_program_header_count_get(input_file))
    {
        return false;
    }
    return helf_file_input_32_bits_program_header_get(input_file,index,programheader);
}

size_t helf_rl78_segment_count_get(helf_file_input_t *input_file)
{
    return helf_rl78_program_header_count_get(input_file);
}

bool helf_rl78_segment_info_get(helf_file_input_t *input_file,size_t index,helf_rl78_segment_info_t *info)
{
    if(!helf_rl78_is_rl78(input_file))
    {
        return false;
    }
    return helf_arch_segment_info_get(input_file,index,info);
}

bool helf_rl78_is_load_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_rl78_is_rl78(input_file))
    {
        return false;
    }
    return helf_arch_is_load_segment(index,input_file);
}

bool helf_rl78_is_rx_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_rl78_is_rl78(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_segment(index,input_file);
}

bool helf_rl78_is_ro_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_rl78_is_rl78(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_segment(index,input_file);
}

bool helf_rl78_is_rw_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_rl78_is_rl78(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_segment(index,input_file);
}

size_t helf_rl78_load_segment_count_get(helf_file_input_t *input_file)
{
    if(!helf_rl78_is_rl78(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_count_get(input_file);
}

bool helf_rl78_load_segment_info_get(helf_file_input_t *input_file,size_t index,helf_rl78_segment_info_t *info)
{
    if(!helf_rl78_is_rl78(input_file))
    {
        return false;
    }
    return helf_arch_load_segment_info_get(input_file,index,info);
}

size_t helf_rl78_load_segment_total_filesz_get(helf_file_input_t *input_file)
{
    if(!helf_rl78_is_rl78(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_filesz_get(input_file);
}

size_t helf_rl78_load_segment_total_memsz_get(helf_file_input_t *input_file)
{
    if(!helf_rl78_is_rl78(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_memsz_get(input_file);
}

size_t helf_rl78_relocation_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_rl78_is_rl78(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_section_count_get(input_file);
}

bool helf_rl78_relocation_section_info_get(helf_file_input_t *input_file,size_t index,helf_arch_relocation_section_info_t *info)
{
    if(!helf_rl78_is_rl78(input_file))
    {
        return false;
    }
    return helf_arch_relocation_section_info_get(input_file,index,info);
}

size_t helf_rl78_relocation_count_get(helf_file_input_t *input_file,size_t section_index)
{
    if(!helf_rl78_is_rl78(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_count_get(input_file,section_index);
}

bool helf_rl78_relocation_get(helf_file_input_t *input_file,size_t section_index,size_t index,helf_arch_relocation_info_t *info)
{
    if(!helf_rl78_is_rl78(input_file))
    {
        return false;
    }
    if(!helf_arch_relocation_get(input_file,section_index,index,info))
    {
        return false;
    }
    return true;
}

bool helf_rl78_relocation_symbol_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_symbol_info_t *info)
{
    if(!helf_rl78_is_rl78(input_file))
    {
        return false;
    }
    return helf_arch_relocation_symbol_info_get(input_file,relocation,info);
}

static const helf_arch_relocation_type_name_t helf_rl78_relocation_type_names[]=
{
    {0,0,"R_RL78_NONE"},
    {1,4,"R_RL78_DIR32"},
    {2,0,"R_RL78_DIR24S"},
    {3,2,"R_RL78_DIR16"},
    {4,0,"R_RL78_DIR16U"},
    {5,0,"R_RL78_DIR16S"},
    {6,1,"R_RL78_DIR8"},
    {7,0,"R_RL78_DIR8U"},
    {8,0,"R_RL78_DIR8S"},
    {9,0,"R_RL78_DIR24S_PCREL"},
    {10,0,"R_RL78_DIR16S_PCREL"},
    {11,0,"R_RL78_DIR8S_PCREL"},
    {12,0,"R_RL78_DIR16UL"},
    {13,0,"R_RL78_DIR16UW"},
    {14,0,"R_RL78_DIR8UL"},
    {15,0,"R_RL78_DIR8UW"},
    {16,0,"R_RL78_DIR32_REV"},
    {17,0,"R_RL78_DIR16_REV"},
    {18,0,"R_RL78_DIR3U_PCREL"},
    {45,0,"R_RL78_RH_RELAX"},
    {46,0,"R_RL78_RH_SFR"},
    {47,0,"R_RL78_RH_SADDR"},
    {65,4,"R_RL78_ABS32"},
    {66,0,"R_RL78_ABS24S"},
    {67,2,"R_RL78_ABS16"},
    {68,0,"R_RL78_ABS16U"},
    {69,0,"R_RL78_ABS16S"},
    {70,1,"R_RL78_ABS8"},
    {71,0,"R_RL78_ABS8U"},
    {72,0,"R_RL78_ABS8S"},
    {73,0,"R_RL78_ABS24S_PCREL"},
    {74,0,"R_RL78_ABS16S_PCREL"},
    {75,0,"R_RL78_ABS8S_PCREL"},
    {76,0,"R_RL78_ABS16UL"},
    {77,0,"R_RL78_ABS16UW"},
    {78,0,"R_RL78_ABS8UL"},
    {79,0,"R_RL78_ABS8UW"},
    {80,0,"R_RL78_ABS32_REV"},
    {81,0,"R_RL78_ABS16_REV"},
    {128,0,"R_RL78_SYM"},
    {129,0,"R_RL78_OPneg"},
    {130,0,"R_RL78_OPadd"},
    {131,0,"R_RL78_OPsub"},
    {132,0,"R_RL78_OPmul"},
    {133,0,"R_RL78_OPdiv"},
    {134,0,"R_RL78_OPshla"},
    {135,0,"R_RL78_OPshra"},
    {136,0,"R_RL78_OPsctsize"},
    {141,0,"R_RL78_OPscttop"},
    {144,0,"R_RL78_OPand"},
    {145,0,"R_RL78_OPor"},
    {146,0,"R_RL78_OPxor"},
    {147,0,"R_RL78_OPnot"},
    {148,0,"R_RL78_OPmod"},
    {149,0,"R_RL78_OPromtop"},
    {150,0,"R_RL78_OPramtop"},
};

const char *helf_rl78_relocation_type_name_get(uint32_t type)
{
    return helf_arch_relocation_type_name_get(helf_rl78_relocation_type_names,sizeof(helf_rl78_relocation_type_names)/sizeof(helf_rl78_relocation_type_names[0]),type);
}

uint8_t helf_rl78_relocation_size_get(uint32_t type)
{
    return helf_arch_relocation_type_size_get(helf_rl78_relocation_type_names,sizeof(helf_rl78_relocation_type_names)/sizeof(helf_rl78_relocation_type_names[0]),type);
}

const char *helf_rl78_section_type_name_get(uint32_t type)
{
    (void)type;
    return NULL;
}

bool helf_rl78_relocation_address_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_relocation_address_info_t *info)
{
    if(!helf_rl78_is_rl78(input_file) || relocation==NULL)
    {
        return false;
    }
    uint8_t size=helf_rl78_relocation_size_get(relocation->type);
    if(size==0)
    {
        return false;
    }
    return helf_arch_relocation_address_info_get(input_file,relocation,size,info);
}

size_t helf_rl78_relocation_data_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,void *buffer,size_t buffer_length)
{
    if(!helf_rl78_is_rl78(input_file) || relocation==NULL)
    {
        return 0;
    }
    uint8_t size=helf_rl78_relocation_size_get(relocation->type);
    if(size==0)
    {
        return 0;
    }
    return helf_arch_relocation_data_get(input_file,relocation,size,buffer,buffer_length);
}

bool helf_rl78_is_shared_object(helf_file_input_t *input_file)
{
    return helf_arch_is_shared_object(input_file,helf_rl78_machines,sizeof(helf_rl78_machines)/sizeof(helf_rl78_machines[0]),HELF_ELFCLASS_32);
}

bool helf_rl78_is_relocatable(helf_file_input_t *input_file)
{
    return helf_arch_is_relocatable(input_file,helf_rl78_machines,sizeof(helf_rl78_machines)/sizeof(helf_rl78_machines[0]),HELF_ELFCLASS_32);
}

size_t helf_rl78_dynamic_count_get(helf_file_input_t *input_file)
{
    if(!helf_rl78_is_rl78(input_file))
    {
        return 0;
    }
    return helf_arch_dynamic_count_get(input_file);
}

bool helf_rl78_dynamic_info_get(helf_file_input_t *input_file,size_t index,helf_arch_dynamic_info_t *info)
{
    if(!helf_rl78_is_rl78(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_info_get(input_file,index,info);
}

bool helf_rl78_dynamic_string_get(helf_file_input_t *input_file,uint64_t offset,char *buffer,size_t buffer_length)
{
    if(!helf_rl78_is_rl78(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_string_get(input_file,offset,buffer,buffer_length);
}
