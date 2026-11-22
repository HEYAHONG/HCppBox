/***************************************************************
 * Name:      helf_arch_avr.c
 * Purpose:   实现helf_arch_avr接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "helf_arch_avr.h"

static const uint16_t helf_avr_machines[]=
{
    HELF_EM_AVR,
    HELF_EM_AVR_OLD,
};

bool helf_avr_is_avr(helf_file_input_t *input_file)
{
    return helf_arch_is(input_file,helf_avr_machines,sizeof(helf_avr_machines)/sizeof(helf_avr_machines[0]),HELF_ELFCLASS_32);
}

bool helf_avr_is_big_endian(helf_file_input_t *input_file)
{
    if(!helf_avr_is_avr(input_file))
    {
        return false;
    }
    return helf_file_input_is_big_endian(input_file);
}

bool helf_avr_is_executable(helf_file_input_t *input_file)
{
    return helf_arch_is_executable(input_file,helf_avr_machines,sizeof(helf_avr_machines)/sizeof(helf_avr_machines[0]),HELF_ELFCLASS_32);
}

bool helf_avr_entry_get(helf_file_input_t *input_file,uint32_t *entry)
{
    if(entry==NULL)
    {
        return false;
    }
    if(!helf_avr_is_avr(input_file))
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

size_t helf_avr_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_avr_is_avr(input_file))
    {
        return 0;
    }
    return helf_arch_section_count_get(input_file);
}

bool helf_avr_section_info_get(helf_file_input_t *input_file,size_t index,helf_avr_section_info_t *info)
{
    if(!helf_avr_is_avr(input_file))
    {
        return false;
    }
    return helf_arch_section_info_get(input_file,index,info);
}

bool helf_avr_is_rx_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_avr_is_avr(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_section(index,input_file);
}

intptr_t helf_avr_rx_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_avr_is_rx_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rx_section_offset_get(index,input_file);
}

size_t helf_avr_rx_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_avr_is_avr(input_file))
    {
        return 0;
    }
    return helf_arch_rx_section_size_get(input_file);
}

bool helf_avr_is_ro_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_avr_is_avr(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_section(index,input_file);
}

intptr_t helf_avr_ro_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_avr_is_ro_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_ro_section_offset_get(index,input_file);
}

size_t helf_avr_ro_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_avr_is_avr(input_file))
    {
        return 0;
    }
    return helf_arch_ro_section_size_get(input_file);
}

bool helf_avr_is_rw_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_avr_is_avr(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_section(index,input_file);
}

intptr_t helf_avr_rw_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_avr_is_rw_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rw_section_offset_get(index,input_file);
}

size_t helf_avr_rw_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_avr_is_avr(input_file))
    {
        return 0;
    }
    return helf_arch_rw_section_size_get(input_file);
}

bool helf_avr_is_bss_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_avr_is_avr(input_file))
    {
        return false;
    }
    return helf_arch_is_bss_section(index,input_file);
}

size_t helf_avr_program_header_count_get(helf_file_input_t *input_file)
{
    if(!helf_avr_is_avr(input_file))
    {
        return 0;
    }
    return helf_arch_program_header_count_get(input_file);
}

bool helf_avr_program_header_get(helf_file_input_t *input_file,size_t index,helf_elf32_program_header_t *programheader)
{
    if(programheader==NULL)
    {
        return false;
    }
    if(!helf_avr_is_avr(input_file))
    {
        return false;
    }
    if(index>=helf_arch_program_header_count_get(input_file))
    {
        return false;
    }
    return helf_file_input_32_bits_program_header_get(input_file,index,programheader);
}

size_t helf_avr_segment_count_get(helf_file_input_t *input_file)
{
    return helf_avr_program_header_count_get(input_file);
}

bool helf_avr_segment_info_get(helf_file_input_t *input_file,size_t index,helf_avr_segment_info_t *info)
{
    if(!helf_avr_is_avr(input_file))
    {
        return false;
    }
    return helf_arch_segment_info_get(input_file,index,info);
}

bool helf_avr_is_load_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_avr_is_avr(input_file))
    {
        return false;
    }
    return helf_arch_is_load_segment(index,input_file);
}

bool helf_avr_is_rx_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_avr_is_avr(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_segment(index,input_file);
}

bool helf_avr_is_ro_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_avr_is_avr(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_segment(index,input_file);
}

bool helf_avr_is_rw_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_avr_is_avr(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_segment(index,input_file);
}

size_t helf_avr_load_segment_count_get(helf_file_input_t *input_file)
{
    if(!helf_avr_is_avr(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_count_get(input_file);
}

bool helf_avr_load_segment_info_get(helf_file_input_t *input_file,size_t index,helf_avr_segment_info_t *info)
{
    if(!helf_avr_is_avr(input_file))
    {
        return false;
    }
    return helf_arch_load_segment_info_get(input_file,index,info);
}

size_t helf_avr_load_segment_total_filesz_get(helf_file_input_t *input_file)
{
    if(!helf_avr_is_avr(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_filesz_get(input_file);
}

size_t helf_avr_load_segment_total_memsz_get(helf_file_input_t *input_file)
{
    if(!helf_avr_is_avr(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_memsz_get(input_file);
}

size_t helf_avr_relocation_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_avr_is_avr(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_section_count_get(input_file);
}

bool helf_avr_relocation_section_info_get(helf_file_input_t *input_file,size_t index,helf_arch_relocation_section_info_t *info)
{
    if(!helf_avr_is_avr(input_file))
    {
        return false;
    }
    return helf_arch_relocation_section_info_get(input_file,index,info);
}

size_t helf_avr_relocation_count_get(helf_file_input_t *input_file,size_t section_index)
{
    if(!helf_avr_is_avr(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_count_get(input_file,section_index);
}

bool helf_avr_relocation_get(helf_file_input_t *input_file,size_t section_index,size_t index,helf_arch_relocation_info_t *info)
{
    if(!helf_avr_is_avr(input_file))
    {
        return false;
    }
    if(!helf_arch_relocation_get(input_file,section_index,index,info))
    {
        return false;
    }
    return true;
}

bool helf_avr_relocation_symbol_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_symbol_info_t *info)
{
    if(!helf_avr_is_avr(input_file))
    {
        return false;
    }
    return helf_arch_relocation_symbol_info_get(input_file,relocation,info);
}

static const helf_arch_relocation_type_name_t helf_avr_relocation_type_names[]=
{
    {0,0,"R_AVR_NONE"},
    {1,4,"R_AVR_32"},
    {2,2,"R_AVR_7_PCREL"},
    {3,2,"R_AVR_13_PCREL"},
    {4,2,"R_AVR_16"},
    {5,2,"R_AVR_16_PM"},
    {6,2,"R_AVR_LO8_LDI"},
    {7,2,"R_AVR_HI8_LDI"},
    {8,2,"R_AVR_HH8_LDI"},
    {9,2,"R_AVR_LO8_LDI_NEG"},
    {10,2,"R_AVR_HI8_LDI_NEG"},
    {11,2,"R_AVR_HH8_LDI_NEG"},
    {12,2,"R_AVR_LO8_LDI_PM"},
    {13,2,"R_AVR_HI8_LDI_PM"},
    {14,2,"R_AVR_HH8_LDI_PM"},
    {15,2,"R_AVR_LO8_LDI_PM_NEG"},
    {16,2,"R_AVR_HI8_LDI_PM_NEG"},
    {17,2,"R_AVR_HH8_LDI_PM_NEG"},
    {18,4,"R_AVR_CALL"},
    {19,2,"R_AVR_LDI"},
    {20,1,"R_AVR_6"},
    {21,1,"R_AVR_6_ADIW"},
    {22,2,"R_AVR_MS8_LDI"},
    {23,2,"R_AVR_MS8_LDI_NEG"},
    {24,2,"R_AVR_LO8_LDI_GS"},
    {25,2,"R_AVR_HI8_LDI_GS"},
    {26,1,"R_AVR_8"},
    {27,1,"R_AVR_8_LO8"},
    {28,1,"R_AVR_8_HI8"},
    {29,1,"R_AVR_8_HLO8"},
    {30,1,"R_AVR_DIFF8"},
    {31,2,"R_AVR_DIFF16"},
    {32,4,"R_AVR_DIFF32"},
    {33,2,"R_AVR_LDS_STS_16"},
    {34,1,"R_AVR_PORT6"},
    {35,1,"R_AVR_PORT5"},
    {36,4,"R_AVR_32_PCREL"},
};

const char *helf_avr_relocation_type_name_get(uint32_t type)
{
    return helf_arch_relocation_type_name_get(helf_avr_relocation_type_names,sizeof(helf_avr_relocation_type_names)/sizeof(helf_avr_relocation_type_names[0]),type);
}

uint8_t helf_avr_relocation_size_get(uint32_t type)
{
    return helf_arch_relocation_type_size_get(helf_avr_relocation_type_names,sizeof(helf_avr_relocation_type_names)/sizeof(helf_avr_relocation_type_names[0]),type);
}

const char *helf_avr_section_type_name_get(uint32_t type)
{
    (void)type;
    return NULL;
}

bool helf_avr_relocation_address_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_relocation_address_info_t *info)
{
    if(!helf_avr_is_avr(input_file) || relocation==NULL)
    {
        return false;
    }
    uint8_t size=helf_avr_relocation_size_get(relocation->type);
    if(size==0)
    {
        return false;
    }
    return helf_arch_relocation_address_info_get(input_file,relocation,size,info);
}

size_t helf_avr_relocation_data_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,void *buffer,size_t buffer_length)
{
    if(!helf_avr_is_avr(input_file) || relocation==NULL)
    {
        return 0;
    }
    uint8_t size=helf_avr_relocation_size_get(relocation->type);
    if(size==0)
    {
        return 0;
    }
    return helf_arch_relocation_data_get(input_file,relocation,size,buffer,buffer_length);
}

bool helf_avr_is_shared_object(helf_file_input_t *input_file)
{
    return helf_arch_is_shared_object(input_file,helf_avr_machines,sizeof(helf_avr_machines)/sizeof(helf_avr_machines[0]),HELF_ELFCLASS_32);
}

bool helf_avr_is_relocatable(helf_file_input_t *input_file)
{
    return helf_arch_is_relocatable(input_file,helf_avr_machines,sizeof(helf_avr_machines)/sizeof(helf_avr_machines[0]),HELF_ELFCLASS_32);
}

size_t helf_avr_dynamic_count_get(helf_file_input_t *input_file)
{
    if(!helf_avr_is_avr(input_file))
    {
        return 0;
    }
    return helf_arch_dynamic_count_get(input_file);
}

bool helf_avr_dynamic_info_get(helf_file_input_t *input_file,size_t index,helf_arch_dynamic_info_t *info)
{
    if(!helf_avr_is_avr(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_info_get(input_file,index,info);
}

bool helf_avr_dynamic_string_get(helf_file_input_t *input_file,uint64_t offset,char *buffer,size_t buffer_length)
{
    if(!helf_avr_is_avr(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_string_get(input_file,offset,buffer,buffer_length);
}

/* helf_relocation_howto_begin */
static const helf_arch_relocation_howto_t helf_avr_reloc_howto[]=
{
    {0x00000000u,0,0,0,0,0x0040,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000001u,4,32,0,0,0x0000,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000002u,2,7,1,3,0x0003,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000003u,2,13,1,0,0x0003,0x0000000000000FFFULL,0x0000000000000FFFULL},
    {0x00000004u,2,16,0,0,0x0000,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000005u,2,16,1,0,0x0000,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000006u,2,8,0,0,0x0000,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000007u,2,8,8,0,0x0000,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000008u,2,8,16,0,0x0000,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000009u,2,8,0,0,0x0000,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x0000000Au,2,8,8,0,0x0000,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x0000000Bu,2,8,16,0,0x0000,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x0000000Cu,2,8,1,0,0x0000,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x0000000Du,2,8,9,0,0x0000,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x0000000Eu,2,8,17,0,0x0000,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x0000000Fu,2,8,1,0,0x0000,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000010u,2,8,9,0,0x0000,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000011u,2,8,17,0,0x0000,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000012u,4,23,1,0,0x0000,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000013u,2,16,0,0,0x0000,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000014u,1,6,0,0,0x0000,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000015u,1,6,0,0,0x0000,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000016u,2,8,24,0,0x0000,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000017u,2,8,24,0,0x0000,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000018u,2,8,1,0,0x0000,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000019u,2,8,9,0,0x0000,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x0000001Au,1,8,0,0,0x0000,0x00000000000000FFULL,0x00000000000000FFULL},
    {0x0000001Bu,1,8,0,0,0x0000,0x0000000000FFFFFFULL,0x0000000000FFFFFFULL},
    {0x0000001Cu,1,8,8,0,0x0000,0x0000000000FFFFFFULL,0x0000000000FFFFFFULL},
    {0x0000001Du,1,8,16,0,0x0000,0x0000000000FFFFFFULL,0x0000000000FFFFFFULL},
    {0x0000001Eu,1,8,0,0,0x0010,0x0000000000000000ULL,0x00000000000000FFULL},
    {0x0000001Fu,2,16,0,0,0x0010,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000020u,4,32,0,0,0x0010,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000021u,2,7,0,0,0x0000,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000022u,1,6,0,0,0x0000,0x0000000000FFFFFFULL,0x0000000000FFFFFFULL},
    {0x00000023u,1,5,0,0,0x0000,0x0000000000FFFFFFULL,0x0000000000FFFFFFULL},
    {0x00000024u,4,32,0,0,0x0003,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
};

bool helf_avr_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto)
{
    if(howto==NULL)
    {
        return false;
    }
    size_t count=sizeof(helf_avr_reloc_howto)/sizeof(helf_avr_reloc_howto[0]);
    if(count==0)
    {
        return false;
    }
    for(size_t i=0;i<count;i++)
    {
        if(helf_avr_reloc_howto[i].type==type)
        {
            *howto=helf_avr_reloc_howto[i];
            return true;
        }
    }
    return false;
}
/* helf_relocation_howto_end */


