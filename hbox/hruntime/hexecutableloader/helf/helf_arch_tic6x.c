/***************************************************************
 * Name:      helf_arch_tic6x.c
 * Purpose:   实现helf_arch_tic6x接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "helf_arch_tic6x.h"

static const uint16_t helf_tic6x_machines[]=
{
    HELF_EM_TI_C6000,
    HELF_EM_TI_C2000,
    HELF_EM_TI_C5500,
};

bool helf_tic6x_is_tic6x(helf_file_input_t *input_file)
{
    return helf_arch_is(input_file,helf_tic6x_machines,sizeof(helf_tic6x_machines)/sizeof(helf_tic6x_machines[0]),HELF_ELFCLASS_32);
}

bool helf_tic6x_is_big_endian(helf_file_input_t *input_file)
{
    if(!helf_tic6x_is_tic6x(input_file))
    {
        return false;
    }
    return helf_file_input_is_big_endian(input_file);
}

bool helf_tic6x_is_executable(helf_file_input_t *input_file)
{
    return helf_arch_is_executable(input_file,helf_tic6x_machines,sizeof(helf_tic6x_machines)/sizeof(helf_tic6x_machines[0]),HELF_ELFCLASS_32);
}

bool helf_tic6x_entry_get(helf_file_input_t *input_file,uint32_t *entry)
{
    if(entry==NULL)
    {
        return false;
    }
    if(!helf_tic6x_is_tic6x(input_file))
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

size_t helf_tic6x_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_tic6x_is_tic6x(input_file))
    {
        return 0;
    }
    return helf_arch_section_count_get(input_file);
}

bool helf_tic6x_section_info_get(helf_file_input_t *input_file,size_t index,helf_tic6x_section_info_t *info)
{
    if(!helf_tic6x_is_tic6x(input_file))
    {
        return false;
    }
    return helf_arch_section_info_get(input_file,index,info);
}

bool helf_tic6x_is_rx_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tic6x_is_tic6x(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_section(index,input_file);
}

intptr_t helf_tic6x_rx_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tic6x_is_rx_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rx_section_offset_get(index,input_file);
}

size_t helf_tic6x_rx_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_tic6x_is_tic6x(input_file))
    {
        return 0;
    }
    return helf_arch_rx_section_size_get(input_file);
}

bool helf_tic6x_is_ro_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tic6x_is_tic6x(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_section(index,input_file);
}

intptr_t helf_tic6x_ro_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tic6x_is_ro_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_ro_section_offset_get(index,input_file);
}

size_t helf_tic6x_ro_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_tic6x_is_tic6x(input_file))
    {
        return 0;
    }
    return helf_arch_ro_section_size_get(input_file);
}

bool helf_tic6x_is_rw_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tic6x_is_tic6x(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_section(index,input_file);
}

intptr_t helf_tic6x_rw_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tic6x_is_rw_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rw_section_offset_get(index,input_file);
}

size_t helf_tic6x_rw_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_tic6x_is_tic6x(input_file))
    {
        return 0;
    }
    return helf_arch_rw_section_size_get(input_file);
}

bool helf_tic6x_is_bss_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tic6x_is_tic6x(input_file))
    {
        return false;
    }
    return helf_arch_is_bss_section(index,input_file);
}

size_t helf_tic6x_program_header_count_get(helf_file_input_t *input_file)
{
    if(!helf_tic6x_is_tic6x(input_file))
    {
        return 0;
    }
    return helf_arch_program_header_count_get(input_file);
}

bool helf_tic6x_program_header_get(helf_file_input_t *input_file,size_t index,helf_elf32_program_header_t *programheader)
{
    if(programheader==NULL)
    {
        return false;
    }
    if(!helf_tic6x_is_tic6x(input_file))
    {
        return false;
    }
    if(index>=helf_arch_program_header_count_get(input_file))
    {
        return false;
    }
    return helf_file_input_32_bits_program_header_get(input_file,index,programheader);
}

size_t helf_tic6x_segment_count_get(helf_file_input_t *input_file)
{
    return helf_tic6x_program_header_count_get(input_file);
}

bool helf_tic6x_segment_info_get(helf_file_input_t *input_file,size_t index,helf_tic6x_segment_info_t *info)
{
    if(!helf_tic6x_is_tic6x(input_file))
    {
        return false;
    }
    return helf_arch_segment_info_get(input_file,index,info);
}

bool helf_tic6x_is_load_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tic6x_is_tic6x(input_file))
    {
        return false;
    }
    return helf_arch_is_load_segment(index,input_file);
}

bool helf_tic6x_is_rx_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tic6x_is_tic6x(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_segment(index,input_file);
}

bool helf_tic6x_is_ro_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tic6x_is_tic6x(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_segment(index,input_file);
}

bool helf_tic6x_is_rw_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tic6x_is_tic6x(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_segment(index,input_file);
}

size_t helf_tic6x_load_segment_count_get(helf_file_input_t *input_file)
{
    if(!helf_tic6x_is_tic6x(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_count_get(input_file);
}

bool helf_tic6x_load_segment_info_get(helf_file_input_t *input_file,size_t index,helf_tic6x_segment_info_t *info)
{
    if(!helf_tic6x_is_tic6x(input_file))
    {
        return false;
    }
    return helf_arch_load_segment_info_get(input_file,index,info);
}

size_t helf_tic6x_load_segment_total_filesz_get(helf_file_input_t *input_file)
{
    if(!helf_tic6x_is_tic6x(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_filesz_get(input_file);
}

size_t helf_tic6x_load_segment_total_memsz_get(helf_file_input_t *input_file)
{
    if(!helf_tic6x_is_tic6x(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_memsz_get(input_file);
}

size_t helf_tic6x_relocation_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_tic6x_is_tic6x(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_section_count_get(input_file);
}

bool helf_tic6x_relocation_section_info_get(helf_file_input_t *input_file,size_t index,helf_arch_relocation_section_info_t *info)
{
    if(!helf_tic6x_is_tic6x(input_file))
    {
        return false;
    }
    return helf_arch_relocation_section_info_get(input_file,index,info);
}

size_t helf_tic6x_relocation_count_get(helf_file_input_t *input_file,size_t section_index)
{
    if(!helf_tic6x_is_tic6x(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_count_get(input_file,section_index);
}

bool helf_tic6x_relocation_get(helf_file_input_t *input_file,size_t section_index,size_t index,helf_arch_relocation_info_t *info)
{
    if(!helf_tic6x_is_tic6x(input_file))
    {
        return false;
    }
    if(!helf_arch_relocation_get(input_file,section_index,index,info))
    {
        return false;
    }
    return true;
}

bool helf_tic6x_relocation_symbol_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_symbol_info_t *info)
{
    if(!helf_tic6x_is_tic6x(input_file))
    {
        return false;
    }
    return helf_arch_relocation_symbol_info_get(input_file,relocation,info);
}

static const helf_arch_relocation_type_name_t helf_tic6x_relocation_type_names[]=
{
    {0,0,"R_C6000_NONE"},
    {1,4,"R_C6000_ABS32"},
    {2,2,"R_C6000_ABS16"},
    {3,1,"R_C6000_ABS8"},
    {4,4,"R_C6000_PCR_S21"},
    {5,4,"R_C6000_PCR_S12"},
    {6,4,"R_C6000_PCR_S10"},
    {7,4,"R_C6000_PCR_S7"},
    {8,4,"R_C6000_ABS_S16"},
    {9,4,"R_C6000_ABS_L16"},
    {10,4,"R_C6000_ABS_H16"},
    {11,4,"R_C6000_SBR_U15_B"},
    {12,4,"R_C6000_SBR_U15_H"},
    {13,4,"R_C6000_SBR_U15_W"},
    {14,4,"R_C6000_SBR_S16"},
    {15,4,"R_C6000_SBR_L16_B"},
    {16,4,"R_C6000_SBR_L16_H"},
    {17,4,"R_C6000_SBR_L16_W"},
    {18,4,"R_C6000_SBR_H16_B"},
    {19,4,"R_C6000_SBR_H16_H"},
    {20,4,"R_C6000_SBR_H16_W"},
    {21,4,"R_C6000_SBR_GOT_U15_W"},
    {22,4,"R_C6000_SBR_GOT_L16_W"},
    {23,4,"R_C6000_SBR_GOT_H16_W"},
    {24,4,"R_C6000_DSBT_INDEX"},
    {25,4,"R_C6000_PREL31"},
    {26,4,"R_C6000_COPY"},
    {27,4,"R_C6000_JUMP_SLOT"},
    {28,4,"R_C6000_EHTYPE"},
    {29,4,"R_C6000_PCR_H16"},
    {30,4,"R_C6000_PCR_L16"},
    {253,0,"R_C6000_ALIGN"},
    {254,0,"R_C6000_FPHEAD"},
    {255,0,"R_C6000_NOCMP"},
};

const char *helf_tic6x_relocation_type_name_get(uint32_t type)
{
    return helf_arch_relocation_type_name_get(helf_tic6x_relocation_type_names,sizeof(helf_tic6x_relocation_type_names)/sizeof(helf_tic6x_relocation_type_names[0]),type);
}

uint8_t helf_tic6x_relocation_size_get(uint32_t type)
{
    return helf_arch_relocation_type_size_get(helf_tic6x_relocation_type_names,sizeof(helf_tic6x_relocation_type_names)/sizeof(helf_tic6x_relocation_type_names[0]),type);
}

static const helf_arch_section_type_name_t helf_tic6x_section_type_names[]=
{
    {1879048193,"SHT_C6000_UNWIND"},
    {1879048194,"SHT_C6000_PREEMPTMAP"},
    {1879048195,"SHT_C6000_ATTRIBUTES"},
    {2130706432,"SHT_TI_ICODE"},
    {2130706433,"SHT_TI_XREF"},
    {2130706434,"SHT_TI_HANDLER"},
    {2130706435,"SHT_TI_INITINFO"},
    {2130706436,"SHT_TI_PHATTRS"},
};

const char *helf_tic6x_section_type_name_get(uint32_t type)
{
    return helf_arch_section_type_name_get(helf_tic6x_section_type_names,sizeof(helf_tic6x_section_type_names)/sizeof(helf_tic6x_section_type_names[0]),type);
}

bool helf_tic6x_relocation_address_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_relocation_address_info_t *info)
{
    if(!helf_tic6x_is_tic6x(input_file) || relocation==NULL)
    {
        return false;
    }
    uint8_t size=helf_tic6x_relocation_size_get(relocation->type);
    if(size==0)
    {
        return false;
    }
    return helf_arch_relocation_address_info_get(input_file,relocation,size,info);
}

size_t helf_tic6x_relocation_data_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,void *buffer,size_t buffer_length)
{
    if(!helf_tic6x_is_tic6x(input_file) || relocation==NULL)
    {
        return 0;
    }
    uint8_t size=helf_tic6x_relocation_size_get(relocation->type);
    if(size==0)
    {
        return 0;
    }
    return helf_arch_relocation_data_get(input_file,relocation,size,buffer,buffer_length);
}

bool helf_tic6x_is_shared_object(helf_file_input_t *input_file)
{
    return helf_arch_is_shared_object(input_file,helf_tic6x_machines,sizeof(helf_tic6x_machines)/sizeof(helf_tic6x_machines[0]),HELF_ELFCLASS_32);
}

bool helf_tic6x_is_relocatable(helf_file_input_t *input_file)
{
    return helf_arch_is_relocatable(input_file,helf_tic6x_machines,sizeof(helf_tic6x_machines)/sizeof(helf_tic6x_machines[0]),HELF_ELFCLASS_32);
}

size_t helf_tic6x_dynamic_count_get(helf_file_input_t *input_file)
{
    if(!helf_tic6x_is_tic6x(input_file))
    {
        return 0;
    }
    return helf_arch_dynamic_count_get(input_file);
}

bool helf_tic6x_dynamic_info_get(helf_file_input_t *input_file,size_t index,helf_arch_dynamic_info_t *info)
{
    if(!helf_tic6x_is_tic6x(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_info_get(input_file,index,info);
}

bool helf_tic6x_dynamic_string_get(helf_file_input_t *input_file,uint64_t offset,char *buffer,size_t buffer_length)
{
    if(!helf_tic6x_is_tic6x(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_string_get(input_file,offset,buffer,buffer_length);
}

/* helf_relocation_howto_begin */
static const helf_arch_relocation_howto_t helf_tic6x_reloc_howto[]=
{
    {0x00000000u,0,0,0,0,0x0040,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000001u,4,32,0,0,0x0000,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000002u,2,16,0,0,0x0000,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000003u,1,8,0,0,0x0000,0x0000000000000000ULL,0x00000000000000FFULL},
    {0x00000004u,4,21,2,7,0x0003,0x0000000000000000ULL,0x000000000FFFFF80ULL},
    {0x00000005u,4,12,2,16,0x0003,0x0000000000000000ULL,0x000000000FFF0000ULL},
    {0x00000006u,4,10,2,13,0x0003,0x0000000000000000ULL,0x00000000007FE000ULL},
    {0x00000007u,4,7,2,16,0x0003,0x0000000000000000ULL,0x00000000007F0000ULL},
    {0x00000008u,4,16,0,7,0x0000,0x0000000000000000ULL,0x00000000007FFF80ULL},
    {0x00000009u,4,16,0,7,0x0000,0x0000000000000000ULL,0x00000000007FFF80ULL},
    {0x0000000Au,4,16,16,7,0x0000,0x0000000000000000ULL,0x00000000007FFF80ULL},
    {0x0000000Bu,4,15,0,8,0x0000,0x0000000000000000ULL,0x00000000007FFF00ULL},
    {0x0000000Cu,4,15,1,8,0x0000,0x0000000000000000ULL,0x00000000007FFF00ULL},
    {0x0000000Du,4,15,2,8,0x0000,0x0000000000000000ULL,0x00000000007FFF00ULL},
    {0x0000000Eu,4,16,0,7,0x0000,0x0000000000000000ULL,0x00000000007FFF80ULL},
    {0x0000000Fu,4,16,0,7,0x0000,0x0000000000000000ULL,0x00000000007FFF80ULL},
    {0x00000010u,4,16,1,7,0x0000,0x0000000000000000ULL,0x00000000007FFF80ULL},
    {0x00000011u,4,16,2,7,0x0000,0x0000000000000000ULL,0x00000000007FFF80ULL},
    {0x00000012u,4,16,16,7,0x0000,0x0000000000000000ULL,0x00000000007FFF80ULL},
    {0x00000013u,4,16,17,7,0x0000,0x0000000000000000ULL,0x00000000007FFF80ULL},
    {0x00000014u,4,16,18,7,0x0000,0x0000000000000000ULL,0x00000000007FFF80ULL},
    {0x00000015u,4,15,2,8,0x0200,0x0000000000000000ULL,0x00000000007FFF00ULL},
    {0x00000016u,4,16,2,7,0x0200,0x0000000000000000ULL,0x00000000007FFF80ULL},
    {0x00000017u,4,16,18,7,0x0200,0x0000000000000000ULL,0x00000000007FFF80ULL},
    {0x00000018u,4,15,0,8,0x0000,0x0000000000000000ULL,0x00000000007FFF00ULL},
    {0x00000019u,4,31,1,0,0x0003,0x0000000000000000ULL,0x000000007FFFFFFFULL},
    {0x0000001Au,4,32,0,0,0x0180,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x0000001Bu,4,32,0,0,0x0140,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x0000001Cu,4,32,0,0,0x0000,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x0000001Du,4,16,16,7,0x0003,0x0000000000000000ULL,0x00000000007FFF80ULL},
    {0x0000001Eu,4,16,0,7,0x0003,0x0000000000000000ULL,0x00000000007FFF80ULL},
    {0x000000FDu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000FEu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000FFu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
};

bool helf_tic6x_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto)
{
    if(howto==NULL)
    {
        return false;
    }
    size_t count=sizeof(helf_tic6x_reloc_howto)/sizeof(helf_tic6x_reloc_howto[0]);
    if(count==0)
    {
        return false;
    }
    for(size_t i=0;i<count;i++)
    {
        if(helf_tic6x_reloc_howto[i].type==type)
        {
            *howto=helf_tic6x_reloc_howto[i];
            return true;
        }
    }
    return false;
}
/* helf_relocation_howto_end */


