/***************************************************************
 * Name:      helf_arch_score.c
 * Purpose:   实现helf_arch_score接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "helf_arch_score.h"

static const uint16_t helf_score_machines[]=
{
    HELF_EM_SCORE,
    HELF_EM_SCORE_OLD,
};

bool helf_score_is_score(helf_file_input_t *input_file)
{
    return helf_arch_is(input_file,helf_score_machines,sizeof(helf_score_machines)/sizeof(helf_score_machines[0]),HELF_ELFCLASS_32);
}

bool helf_score_is_big_endian(helf_file_input_t *input_file)
{
    if(!helf_score_is_score(input_file))
    {
        return false;
    }
    return helf_file_input_is_big_endian(input_file);
}

bool helf_score_is_executable(helf_file_input_t *input_file)
{
    return helf_arch_is_executable(input_file,helf_score_machines,sizeof(helf_score_machines)/sizeof(helf_score_machines[0]),HELF_ELFCLASS_32);
}

bool helf_score_entry_get(helf_file_input_t *input_file,uint32_t *entry)
{
    if(entry==NULL)
    {
        return false;
    }
    if(!helf_score_is_score(input_file))
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

size_t helf_score_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_score_is_score(input_file))
    {
        return 0;
    }
    return helf_arch_section_count_get(input_file);
}

bool helf_score_section_info_get(helf_file_input_t *input_file,size_t index,helf_score_section_info_t *info)
{
    if(!helf_score_is_score(input_file))
    {
        return false;
    }
    return helf_arch_section_info_get(input_file,index,info);
}

bool helf_score_is_rx_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_score_is_score(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_section(index,input_file);
}

intptr_t helf_score_rx_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_score_is_rx_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rx_section_offset_get(index,input_file);
}

size_t helf_score_rx_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_score_is_score(input_file))
    {
        return 0;
    }
    return helf_arch_rx_section_size_get(input_file);
}

bool helf_score_is_ro_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_score_is_score(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_section(index,input_file);
}

intptr_t helf_score_ro_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_score_is_ro_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_ro_section_offset_get(index,input_file);
}

size_t helf_score_ro_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_score_is_score(input_file))
    {
        return 0;
    }
    return helf_arch_ro_section_size_get(input_file);
}

bool helf_score_is_rw_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_score_is_score(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_section(index,input_file);
}

intptr_t helf_score_rw_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_score_is_rw_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rw_section_offset_get(index,input_file);
}

size_t helf_score_rw_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_score_is_score(input_file))
    {
        return 0;
    }
    return helf_arch_rw_section_size_get(input_file);
}

bool helf_score_is_bss_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_score_is_score(input_file))
    {
        return false;
    }
    return helf_arch_is_bss_section(index,input_file);
}

size_t helf_score_program_header_count_get(helf_file_input_t *input_file)
{
    if(!helf_score_is_score(input_file))
    {
        return 0;
    }
    return helf_arch_program_header_count_get(input_file);
}

bool helf_score_program_header_get(helf_file_input_t *input_file,size_t index,helf_elf32_program_header_t *programheader)
{
    if(programheader==NULL)
    {
        return false;
    }
    if(!helf_score_is_score(input_file))
    {
        return false;
    }
    if(index>=helf_arch_program_header_count_get(input_file))
    {
        return false;
    }
    return helf_file_input_32_bits_program_header_get(input_file,index,programheader);
}

size_t helf_score_segment_count_get(helf_file_input_t *input_file)
{
    return helf_score_program_header_count_get(input_file);
}

bool helf_score_segment_info_get(helf_file_input_t *input_file,size_t index,helf_score_segment_info_t *info)
{
    if(!helf_score_is_score(input_file))
    {
        return false;
    }
    return helf_arch_segment_info_get(input_file,index,info);
}

bool helf_score_is_load_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_score_is_score(input_file))
    {
        return false;
    }
    return helf_arch_is_load_segment(index,input_file);
}

bool helf_score_is_rx_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_score_is_score(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_segment(index,input_file);
}

bool helf_score_is_ro_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_score_is_score(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_segment(index,input_file);
}

bool helf_score_is_rw_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_score_is_score(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_segment(index,input_file);
}

size_t helf_score_load_segment_count_get(helf_file_input_t *input_file)
{
    if(!helf_score_is_score(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_count_get(input_file);
}

bool helf_score_load_segment_info_get(helf_file_input_t *input_file,size_t index,helf_score_segment_info_t *info)
{
    if(!helf_score_is_score(input_file))
    {
        return false;
    }
    return helf_arch_load_segment_info_get(input_file,index,info);
}

size_t helf_score_load_segment_total_filesz_get(helf_file_input_t *input_file)
{
    if(!helf_score_is_score(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_filesz_get(input_file);
}

size_t helf_score_load_segment_total_memsz_get(helf_file_input_t *input_file)
{
    if(!helf_score_is_score(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_memsz_get(input_file);
}

size_t helf_score_relocation_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_score_is_score(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_section_count_get(input_file);
}

bool helf_score_relocation_section_info_get(helf_file_input_t *input_file,size_t index,helf_arch_relocation_section_info_t *info)
{
    if(!helf_score_is_score(input_file))
    {
        return false;
    }
    return helf_arch_relocation_section_info_get(input_file,index,info);
}

size_t helf_score_relocation_count_get(helf_file_input_t *input_file,size_t section_index)
{
    if(!helf_score_is_score(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_count_get(input_file,section_index);
}

bool helf_score_relocation_get(helf_file_input_t *input_file,size_t section_index,size_t index,helf_arch_relocation_info_t *info)
{
    if(!helf_score_is_score(input_file))
    {
        return false;
    }
    if(!helf_arch_relocation_get(input_file,section_index,index,info))
    {
        return false;
    }
    return true;
}

bool helf_score_relocation_symbol_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_symbol_info_t *info)
{
    if(!helf_score_is_score(input_file))
    {
        return false;
    }
    return helf_arch_relocation_symbol_info_get(input_file,relocation,info);
}

static const helf_arch_relocation_type_name_t helf_score_relocation_type_names[]=
{
    {0,0,"R_SCORE_NONE"},
    {1,4,"R_SCORE_HI16"},
    {2,4,"R_SCORE_LO16"},
    {3,4,"R_SCORE_BCMP"},
    {4,4,"R_SCORE_24"},
    {5,4,"R_SCORE_PC19"},
    {6,2,"R_SCORE16_11"},
    {7,2,"R_SCORE16_PC8"},
    {8,4,"R_SCORE_ABS32"},
    {9,2,"R_SCORE_ABS16"},
    {10,4,"R_SCORE_DUMMY2"},
    {11,4,"R_SCORE_GP15"},
    {12,4,"R_SCORE_GNU_VTINHERIT"},
    {13,4,"R_SCORE_GNU_VTENTRY"},
    {14,4,"R_SCORE_GOT15"},
    {15,4,"R_SCORE_GOT_LO16"},
    {16,4,"R_SCORE_CALL15"},
    {17,4,"R_SCORE_GPREL32"},
    {18,4,"R_SCORE_REL32"},
    {19,4,"R_SCORE_DUMMY_HI16"},
    {20,4,"R_SCORE_IMM30"},
    {21,4,"R_SCORE_IMM32"},
};

const char *helf_score_relocation_type_name_get(uint32_t type)
{
    return helf_arch_relocation_type_name_get(helf_score_relocation_type_names,sizeof(helf_score_relocation_type_names)/sizeof(helf_score_relocation_type_names[0]),type);
}

uint8_t helf_score_relocation_size_get(uint32_t type)
{
    return helf_arch_relocation_type_size_get(helf_score_relocation_type_names,sizeof(helf_score_relocation_type_names)/sizeof(helf_score_relocation_type_names[0]),type);
}

const char *helf_score_section_type_name_get(uint32_t type)
{
    (void)type;
    return NULL;
}

bool helf_score_relocation_address_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_relocation_address_info_t *info)
{
    if(!helf_score_is_score(input_file) || relocation==NULL)
    {
        return false;
    }
    uint8_t size=helf_score_relocation_size_get(relocation->type);
    if(size==0)
    {
        return false;
    }
    return helf_arch_relocation_address_info_get(input_file,relocation,size,info);
}

size_t helf_score_relocation_data_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,void *buffer,size_t buffer_length)
{
    if(!helf_score_is_score(input_file) || relocation==NULL)
    {
        return 0;
    }
    uint8_t size=helf_score_relocation_size_get(relocation->type);
    if(size==0)
    {
        return 0;
    }
    return helf_arch_relocation_data_get(input_file,relocation,size,buffer,buffer_length);
}

bool helf_score_is_shared_object(helf_file_input_t *input_file)
{
    return helf_arch_is_shared_object(input_file,helf_score_machines,sizeof(helf_score_machines)/sizeof(helf_score_machines[0]),HELF_ELFCLASS_32);
}

bool helf_score_is_relocatable(helf_file_input_t *input_file)
{
    return helf_arch_is_relocatable(input_file,helf_score_machines,sizeof(helf_score_machines)/sizeof(helf_score_machines[0]),HELF_ELFCLASS_32);
}

size_t helf_score_dynamic_count_get(helf_file_input_t *input_file)
{
    if(!helf_score_is_score(input_file))
    {
        return 0;
    }
    return helf_arch_dynamic_count_get(input_file);
}

bool helf_score_dynamic_info_get(helf_file_input_t *input_file,size_t index,helf_arch_dynamic_info_t *info)
{
    if(!helf_score_is_score(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_info_get(input_file,index,info);
}

bool helf_score_dynamic_string_get(helf_file_input_t *input_file,uint64_t offset,char *buffer,size_t buffer_length)
{
    if(!helf_score_is_score(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_string_get(input_file,offset,buffer,buffer_length);
}

/* helf_relocation_howto_begin */
static const helf_arch_relocation_howto_t helf_score_reloc_howto[]=
{
    {0x00000000u,0,0,0,0,0x0040,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000001u,4,16,0,1,0x0018,0x0000000000037FFFULL,0x0000000000037FFFULL},
    {0x00000002u,4,16,0,1,0x0018,0x0000000000037FFFULL,0x0000000000037FFFULL},
    {0x00000003u,4,16,1,1,0x0001,0x0000000003E00381ULL,0x0000000003E00381ULL},
    {0x00000004u,4,24,1,1,0x0000,0x0000000003FF7FFFULL,0x0000000003FF7FFFULL},
    {0x00000005u,4,19,1,1,0x0001,0x0000000003FF03FEULL,0x0000000003FF03FEULL},
    {0x00000006u,2,11,1,1,0x0000,0x0000000000000FFEULL,0x0000000000000FFEULL},
    {0x00000007u,2,9,1,0,0x0001,0x00000000000001FFULL,0x00000000000001FFULL},
    {0x00000008u,4,32,0,0,0x0000,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000009u,2,16,0,0,0x0000,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x0000000Au,4,16,0,0,0x0008,0x0000000000007FFFULL,0x0000000000007FFFULL},
    {0x0000000Bu,4,16,0,0,0x0018,0x0000000000007FFFULL,0x0000000000007FFFULL},
    {0x0000000Cu,4,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000000Du,4,0,0,0,0x0010,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000000Eu,4,16,0,0,0x0218,0x0000000000007FFFULL,0x0000000000007FFFULL},
    {0x0000000Fu,4,16,0,1,0x0218,0x0000000000037FFEULL,0x0000000000037FFEULL},
    {0x00000010u,4,16,0,0,0x0008,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000011u,4,32,0,0,0x0018,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000012u,4,32,0,0,0x0008,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000013u,4,16,0,1,0x0018,0x0000000000037FFFULL,0x0000000000037FFFULL},
    {0x00000014u,4,30,2,7,0x0000,0x0000007F7FFF7F80ULL,0x0000007F7FFF7F80ULL},
    {0x00000015u,4,32,0,5,0x0000,0x0000007F7FFF7FE0ULL,0x0000007F7FFF7FE0ULL},
};

bool helf_score_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto)
{
    if(howto==NULL)
    {
        return false;
    }
    size_t count=sizeof(helf_score_reloc_howto)/sizeof(helf_score_reloc_howto[0]);
    if(count==0)
    {
        return false;
    }
    for(size_t i=0;i<count;i++)
    {
        if(helf_score_reloc_howto[i].type==type)
        {
            *howto=helf_score_reloc_howto[i];
            return true;
        }
    }
    return false;
}
/* helf_relocation_howto_end */

