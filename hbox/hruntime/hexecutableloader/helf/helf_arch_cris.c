/***************************************************************
 * Name:      helf_arch_cris.c
 * Purpose:   实现helf_arch_cris接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "helf_arch_cris.h"

static const uint16_t helf_cris_machines[]=
{
    HELF_EM_CRIS,
};

bool helf_cris_is_cris(helf_file_input_t *input_file)
{
    return helf_arch_is(input_file,helf_cris_machines,sizeof(helf_cris_machines)/sizeof(helf_cris_machines[0]),HELF_ELFCLASS_32);
}

bool helf_cris_is_big_endian(helf_file_input_t *input_file)
{
    if(!helf_cris_is_cris(input_file))
    {
        return false;
    }
    return helf_file_input_is_big_endian(input_file);
}

bool helf_cris_is_executable(helf_file_input_t *input_file)
{
    return helf_arch_is_executable(input_file,helf_cris_machines,sizeof(helf_cris_machines)/sizeof(helf_cris_machines[0]),HELF_ELFCLASS_32);
}

bool helf_cris_entry_get(helf_file_input_t *input_file,uint32_t *entry)
{
    if(entry==NULL)
    {
        return false;
    }
    if(!helf_cris_is_cris(input_file))
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

size_t helf_cris_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_cris_is_cris(input_file))
    {
        return 0;
    }
    return helf_arch_section_count_get(input_file);
}

bool helf_cris_section_info_get(helf_file_input_t *input_file,size_t index,helf_cris_section_info_t *info)
{
    if(!helf_cris_is_cris(input_file))
    {
        return false;
    }
    return helf_arch_section_info_get(input_file,index,info);
}

bool helf_cris_is_rx_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_cris_is_cris(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_section(index,input_file);
}

intptr_t helf_cris_rx_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_cris_is_rx_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rx_section_offset_get(index,input_file);
}

size_t helf_cris_rx_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_cris_is_cris(input_file))
    {
        return 0;
    }
    return helf_arch_rx_section_size_get(input_file);
}

bool helf_cris_is_ro_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_cris_is_cris(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_section(index,input_file);
}

intptr_t helf_cris_ro_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_cris_is_ro_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_ro_section_offset_get(index,input_file);
}

size_t helf_cris_ro_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_cris_is_cris(input_file))
    {
        return 0;
    }
    return helf_arch_ro_section_size_get(input_file);
}

bool helf_cris_is_rw_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_cris_is_cris(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_section(index,input_file);
}

intptr_t helf_cris_rw_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_cris_is_rw_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rw_section_offset_get(index,input_file);
}

size_t helf_cris_rw_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_cris_is_cris(input_file))
    {
        return 0;
    }
    return helf_arch_rw_section_size_get(input_file);
}

bool helf_cris_is_bss_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_cris_is_cris(input_file))
    {
        return false;
    }
    return helf_arch_is_bss_section(index,input_file);
}

size_t helf_cris_program_header_count_get(helf_file_input_t *input_file)
{
    if(!helf_cris_is_cris(input_file))
    {
        return 0;
    }
    return helf_arch_program_header_count_get(input_file);
}

bool helf_cris_program_header_get(helf_file_input_t *input_file,size_t index,helf_elf32_program_header_t *programheader)
{
    if(programheader==NULL)
    {
        return false;
    }
    if(!helf_cris_is_cris(input_file))
    {
        return false;
    }
    if(index>=helf_arch_program_header_count_get(input_file))
    {
        return false;
    }
    return helf_file_input_32_bits_program_header_get(input_file,index,programheader);
}

size_t helf_cris_segment_count_get(helf_file_input_t *input_file)
{
    return helf_cris_program_header_count_get(input_file);
}

bool helf_cris_segment_info_get(helf_file_input_t *input_file,size_t index,helf_cris_segment_info_t *info)
{
    if(!helf_cris_is_cris(input_file))
    {
        return false;
    }
    return helf_arch_segment_info_get(input_file,index,info);
}

bool helf_cris_is_load_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_cris_is_cris(input_file))
    {
        return false;
    }
    return helf_arch_is_load_segment(index,input_file);
}

bool helf_cris_is_rx_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_cris_is_cris(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_segment(index,input_file);
}

bool helf_cris_is_ro_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_cris_is_cris(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_segment(index,input_file);
}

bool helf_cris_is_rw_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_cris_is_cris(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_segment(index,input_file);
}

size_t helf_cris_load_segment_count_get(helf_file_input_t *input_file)
{
    if(!helf_cris_is_cris(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_count_get(input_file);
}

bool helf_cris_load_segment_info_get(helf_file_input_t *input_file,size_t index,helf_cris_segment_info_t *info)
{
    if(!helf_cris_is_cris(input_file))
    {
        return false;
    }
    return helf_arch_load_segment_info_get(input_file,index,info);
}

size_t helf_cris_load_segment_total_filesz_get(helf_file_input_t *input_file)
{
    if(!helf_cris_is_cris(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_filesz_get(input_file);
}

size_t helf_cris_load_segment_total_memsz_get(helf_file_input_t *input_file)
{
    if(!helf_cris_is_cris(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_memsz_get(input_file);
}

size_t helf_cris_relocation_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_cris_is_cris(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_section_count_get(input_file);
}

bool helf_cris_relocation_section_info_get(helf_file_input_t *input_file,size_t index,helf_arch_relocation_section_info_t *info)
{
    if(!helf_cris_is_cris(input_file))
    {
        return false;
    }
    return helf_arch_relocation_section_info_get(input_file,index,info);
}

size_t helf_cris_relocation_count_get(helf_file_input_t *input_file,size_t section_index)
{
    if(!helf_cris_is_cris(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_count_get(input_file,section_index);
}

bool helf_cris_relocation_get(helf_file_input_t *input_file,size_t section_index,size_t index,helf_arch_relocation_info_t *info)
{
    if(!helf_cris_is_cris(input_file))
    {
        return false;
    }
    if(!helf_arch_relocation_get(input_file,section_index,index,info))
    {
        return false;
    }
    return true;
}

bool helf_cris_relocation_symbol_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_symbol_info_t *info)
{
    if(!helf_cris_is_cris(input_file))
    {
        return false;
    }
    return helf_arch_relocation_symbol_info_get(input_file,relocation,info);
}

static const helf_arch_relocation_type_name_t helf_cris_relocation_type_names[]=
{
    {0,0,"R_CRIS_NONE"},
    {1,1,"R_CRIS_8"},
    {2,2,"R_CRIS_16"},
    {3,4,"R_CRIS_32"},
    {4,1,"R_CRIS_8_PCREL"},
    {5,2,"R_CRIS_16_PCREL"},
    {6,4,"R_CRIS_32_PCREL"},
    {7,4,"R_CRIS_GNU_VTINHERIT"},
    {8,4,"R_CRIS_GNU_VTENTRY"},
    {9,4,"R_CRIS_COPY"},
    {10,4,"R_CRIS_GLOB_DAT"},
    {11,4,"R_CRIS_JUMP_SLOT"},
    {12,4,"R_CRIS_RELATIVE"},
    {13,2,"R_CRIS_16_GOT"},
    {14,4,"R_CRIS_32_GOT"},
    {15,2,"R_CRIS_16_GOTPLT"},
    {16,4,"R_CRIS_32_GOTPLT"},
    {17,4,"R_CRIS_32_GOTREL"},
    {18,4,"R_CRIS_32_PLT_GOTREL"},
    {19,4,"R_CRIS_32_PLT_PCREL"},
    {20,0,"R_CRIS_32_GOT_GD"},
    {21,0,"R_CRIS_16_GOT_GD"},
    {22,0,"R_CRIS_32_GD"},
    {23,0,"R_CRIS_DTP"},
    {24,0,"R_CRIS_32_DTPREL"},
    {25,0,"R_CRIS_16_DTPREL"},
    {26,0,"R_CRIS_32_GOT_TPREL"},
    {27,0,"R_CRIS_16_GOT_TPREL"},
    {28,0,"R_CRIS_32_TPREL"},
    {29,0,"R_CRIS_16_TPREL"},
    {30,0,"R_CRIS_DTPMOD"},
    {31,0,"R_CRIS_32_IE"},
};

const char *helf_cris_relocation_type_name_get(uint32_t type)
{
    return helf_arch_relocation_type_name_get(helf_cris_relocation_type_names,sizeof(helf_cris_relocation_type_names)/sizeof(helf_cris_relocation_type_names[0]),type);
}

uint8_t helf_cris_relocation_size_get(uint32_t type)
{
    return helf_arch_relocation_type_size_get(helf_cris_relocation_type_names,sizeof(helf_cris_relocation_type_names)/sizeof(helf_cris_relocation_type_names[0]),type);
}

const char *helf_cris_section_type_name_get(uint32_t type)
{
    (void)type;
    return NULL;
}

bool helf_cris_relocation_address_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_relocation_address_info_t *info)
{
    if(!helf_cris_is_cris(input_file) || relocation==NULL)
    {
        return false;
    }
    uint8_t size=helf_cris_relocation_size_get(relocation->type);
    if(size==0)
    {
        return false;
    }
    return helf_arch_relocation_address_info_get(input_file,relocation,size,info);
}

size_t helf_cris_relocation_data_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,void *buffer,size_t buffer_length)
{
    if(!helf_cris_is_cris(input_file) || relocation==NULL)
    {
        return 0;
    }
    uint8_t size=helf_cris_relocation_size_get(relocation->type);
    if(size==0)
    {
        return 0;
    }
    return helf_arch_relocation_data_get(input_file,relocation,size,buffer,buffer_length);
}

bool helf_cris_is_shared_object(helf_file_input_t *input_file)
{
    return helf_arch_is_shared_object(input_file,helf_cris_machines,sizeof(helf_cris_machines)/sizeof(helf_cris_machines[0]),HELF_ELFCLASS_32);
}

bool helf_cris_is_relocatable(helf_file_input_t *input_file)
{
    return helf_arch_is_relocatable(input_file,helf_cris_machines,sizeof(helf_cris_machines)/sizeof(helf_cris_machines[0]),HELF_ELFCLASS_32);
}

size_t helf_cris_dynamic_count_get(helf_file_input_t *input_file)
{
    if(!helf_cris_is_cris(input_file))
    {
        return 0;
    }
    return helf_arch_dynamic_count_get(input_file);
}

bool helf_cris_dynamic_info_get(helf_file_input_t *input_file,size_t index,helf_arch_dynamic_info_t *info)
{
    if(!helf_cris_is_cris(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_info_get(input_file,index,info);
}

bool helf_cris_dynamic_string_get(helf_file_input_t *input_file,uint64_t offset,char *buffer,size_t buffer_length)
{
    if(!helf_cris_is_cris(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_string_get(input_file,offset,buffer,buffer_length);
}

/* helf_relocation_howto_begin */
static const helf_arch_relocation_howto_t helf_cris_reloc_howto[]=
{
    {0x00000000u,0,0,0,0,0x0040,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000001u,1,8,0,0,0x0000,0x0000000000000000ULL,0x00000000000000FFULL},
    {0x00000002u,2,16,0,0,0x0000,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000003u,4,32,0,0,0x0000,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000004u,1,8,0,0,0x0013,0x0000000000000000ULL,0x00000000000000FFULL},
    {0x00000005u,2,16,0,0,0x0013,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000006u,4,32,0,0,0x0013,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000007u,4,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000008u,4,0,0,0,0x0010,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000009u,4,32,0,0,0x0180,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000000Au,4,32,0,0,0x0100,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x0000000Bu,4,32,0,0,0x0140,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000000Cu,4,32,0,0,0x0080,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x0000000Du,2,16,0,0,0x0200,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000000Eu,4,32,0,0,0x0200,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x0000000Fu,2,16,0,0,0x0200,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000010u,4,32,0,0,0x0200,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000011u,4,32,0,0,0x0200,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000012u,4,32,0,0,0x0200,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000013u,4,32,0,0,0x0013,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000014u,0,0,0,0,0x0200,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000015u,0,0,0,0,0x0200,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000016u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000017u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000018u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000019u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Au,0,0,0,0,0x03C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Bu,0,0,0,0,0x03C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Cu,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Du,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Eu,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Fu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
};

bool helf_cris_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto)
{
    if(howto==NULL)
    {
        return false;
    }
    size_t count=sizeof(helf_cris_reloc_howto)/sizeof(helf_cris_reloc_howto[0]);
    if(count==0)
    {
        return false;
    }
    for(size_t i=0;i<count;i++)
    {
        if(helf_cris_reloc_howto[i].type==type)
        {
            *howto=helf_cris_reloc_howto[i];
            return true;
        }
    }
    return false;
}
/* helf_relocation_howto_end */


