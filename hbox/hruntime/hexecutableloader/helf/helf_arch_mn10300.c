/***************************************************************
 * Name:      helf_arch_mn10300.c
 * Purpose:   实现helf_arch_mn10300接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "helf_arch_mn10300.h"

static const uint16_t helf_mn10300_machines[]=
{
    HELF_EM_MN10300,
    HELF_EM_CYGNUS_MN10300,
};

bool helf_mn10300_is_mn10300(helf_file_input_t *input_file)
{
    return helf_arch_is(input_file,helf_mn10300_machines,sizeof(helf_mn10300_machines)/sizeof(helf_mn10300_machines[0]),HELF_ELFCLASS_32);
}

bool helf_mn10300_is_big_endian(helf_file_input_t *input_file)
{
    if(!helf_mn10300_is_mn10300(input_file))
    {
        return false;
    }
    return helf_file_input_is_big_endian(input_file);
}

bool helf_mn10300_is_executable(helf_file_input_t *input_file)
{
    return helf_arch_is_executable(input_file,helf_mn10300_machines,sizeof(helf_mn10300_machines)/sizeof(helf_mn10300_machines[0]),HELF_ELFCLASS_32);
}

bool helf_mn10300_entry_get(helf_file_input_t *input_file,uint32_t *entry)
{
    if(entry==NULL)
    {
        return false;
    }
    if(!helf_mn10300_is_mn10300(input_file))
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

size_t helf_mn10300_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_mn10300_is_mn10300(input_file))
    {
        return 0;
    }
    return helf_arch_section_count_get(input_file);
}

bool helf_mn10300_section_info_get(helf_file_input_t *input_file,size_t index,helf_mn10300_section_info_t *info)
{
    if(!helf_mn10300_is_mn10300(input_file))
    {
        return false;
    }
    return helf_arch_section_info_get(input_file,index,info);
}

bool helf_mn10300_is_rx_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_mn10300_is_mn10300(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_section(index,input_file);
}

intptr_t helf_mn10300_rx_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_mn10300_is_rx_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rx_section_offset_get(index,input_file);
}

size_t helf_mn10300_rx_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_mn10300_is_mn10300(input_file))
    {
        return 0;
    }
    return helf_arch_rx_section_size_get(input_file);
}

bool helf_mn10300_is_ro_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_mn10300_is_mn10300(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_section(index,input_file);
}

intptr_t helf_mn10300_ro_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_mn10300_is_ro_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_ro_section_offset_get(index,input_file);
}

size_t helf_mn10300_ro_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_mn10300_is_mn10300(input_file))
    {
        return 0;
    }
    return helf_arch_ro_section_size_get(input_file);
}

bool helf_mn10300_is_rw_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_mn10300_is_mn10300(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_section(index,input_file);
}

intptr_t helf_mn10300_rw_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_mn10300_is_rw_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rw_section_offset_get(index,input_file);
}

size_t helf_mn10300_rw_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_mn10300_is_mn10300(input_file))
    {
        return 0;
    }
    return helf_arch_rw_section_size_get(input_file);
}

bool helf_mn10300_is_bss_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_mn10300_is_mn10300(input_file))
    {
        return false;
    }
    return helf_arch_is_bss_section(index,input_file);
}

size_t helf_mn10300_program_header_count_get(helf_file_input_t *input_file)
{
    if(!helf_mn10300_is_mn10300(input_file))
    {
        return 0;
    }
    return helf_arch_program_header_count_get(input_file);
}

bool helf_mn10300_program_header_get(helf_file_input_t *input_file,size_t index,helf_elf32_program_header_t *programheader)
{
    if(programheader==NULL)
    {
        return false;
    }
    if(!helf_mn10300_is_mn10300(input_file))
    {
        return false;
    }
    if(index>=helf_arch_program_header_count_get(input_file))
    {
        return false;
    }
    return helf_file_input_32_bits_program_header_get(input_file,index,programheader);
}

size_t helf_mn10300_segment_count_get(helf_file_input_t *input_file)
{
    return helf_mn10300_program_header_count_get(input_file);
}

bool helf_mn10300_segment_info_get(helf_file_input_t *input_file,size_t index,helf_mn10300_segment_info_t *info)
{
    if(!helf_mn10300_is_mn10300(input_file))
    {
        return false;
    }
    return helf_arch_segment_info_get(input_file,index,info);
}

bool helf_mn10300_is_load_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_mn10300_is_mn10300(input_file))
    {
        return false;
    }
    return helf_arch_is_load_segment(index,input_file);
}

bool helf_mn10300_is_rx_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_mn10300_is_mn10300(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_segment(index,input_file);
}

bool helf_mn10300_is_ro_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_mn10300_is_mn10300(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_segment(index,input_file);
}

bool helf_mn10300_is_rw_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_mn10300_is_mn10300(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_segment(index,input_file);
}

size_t helf_mn10300_load_segment_count_get(helf_file_input_t *input_file)
{
    if(!helf_mn10300_is_mn10300(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_count_get(input_file);
}

bool helf_mn10300_load_segment_info_get(helf_file_input_t *input_file,size_t index,helf_mn10300_segment_info_t *info)
{
    if(!helf_mn10300_is_mn10300(input_file))
    {
        return false;
    }
    return helf_arch_load_segment_info_get(input_file,index,info);
}

size_t helf_mn10300_load_segment_total_filesz_get(helf_file_input_t *input_file)
{
    if(!helf_mn10300_is_mn10300(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_filesz_get(input_file);
}

size_t helf_mn10300_load_segment_total_memsz_get(helf_file_input_t *input_file)
{
    if(!helf_mn10300_is_mn10300(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_memsz_get(input_file);
}

size_t helf_mn10300_relocation_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_mn10300_is_mn10300(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_section_count_get(input_file);
}

bool helf_mn10300_relocation_section_info_get(helf_file_input_t *input_file,size_t index,helf_arch_relocation_section_info_t *info)
{
    if(!helf_mn10300_is_mn10300(input_file))
    {
        return false;
    }
    return helf_arch_relocation_section_info_get(input_file,index,info);
}

size_t helf_mn10300_relocation_count_get(helf_file_input_t *input_file,size_t section_index)
{
    if(!helf_mn10300_is_mn10300(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_count_get(input_file,section_index);
}

bool helf_mn10300_relocation_get(helf_file_input_t *input_file,size_t section_index,size_t index,helf_arch_relocation_info_t *info)
{
    if(!helf_mn10300_is_mn10300(input_file))
    {
        return false;
    }
    if(!helf_arch_relocation_get(input_file,section_index,index,info))
    {
        return false;
    }
    return true;
}

bool helf_mn10300_relocation_symbol_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_symbol_info_t *info)
{
    if(!helf_mn10300_is_mn10300(input_file))
    {
        return false;
    }
    return helf_arch_relocation_symbol_info_get(input_file,relocation,info);
}

static const helf_arch_relocation_type_name_t helf_mn10300_relocation_type_names[]=
{
    {0,0,"R_MN10300_NONE"},
    {1,4,"R_MN10300_32"},
    {2,2,"R_MN10300_16"},
    {3,1,"R_MN10300_8"},
    {4,4,"R_MN10300_PCREL32"},
    {5,2,"R_MN10300_PCREL16"},
    {6,1,"R_MN10300_PCREL8"},
    {7,0,"R_MN10300_GNU_VTINHERIT"},
    {8,0,"R_MN10300_GNU_VTENTRY"},
    {9,4,"R_MN10300_24"},
    {10,4,"R_MN10300_GOTPC32"},
    {11,2,"R_MN10300_GOTPC16"},
    {12,4,"R_MN10300_GOTOFF32"},
    {13,4,"R_MN10300_GOTOFF24"},
    {14,2,"R_MN10300_GOTOFF16"},
    {15,4,"R_MN10300_PLT32"},
    {16,2,"R_MN10300_PLT16"},
    {17,4,"R_MN10300_GOT32"},
    {18,4,"R_MN10300_GOT24"},
    {19,2,"R_MN10300_GOT16"},
    {20,4,"R_MN10300_COPY"},
    {21,4,"R_MN10300_GLOB_DAT"},
    {22,4,"R_MN10300_JMP_SLOT"},
    {23,4,"R_MN10300_RELATIVE"},
    {24,4,"R_MN10300_TLS_GD"},
    {25,4,"R_MN10300_TLS_LD"},
    {26,4,"R_MN10300_TLS_LDO"},
    {27,4,"R_MN10300_TLS_GOTIE"},
    {28,4,"R_MN10300_TLS_IE"},
    {29,4,"R_MN10300_TLS_LE"},
    {30,4,"R_MN10300_TLS_DTPMOD"},
    {31,4,"R_MN10300_TLS_DTPOFF"},
    {32,4,"R_MN10300_TLS_TPOFF"},
    {33,4,"R_MN10300_SYM_DIFF"},
    {34,1,"R_MN10300_ALIGN"},
};

const char *helf_mn10300_relocation_type_name_get(uint32_t type)
{
    return helf_arch_relocation_type_name_get(helf_mn10300_relocation_type_names,sizeof(helf_mn10300_relocation_type_names)/sizeof(helf_mn10300_relocation_type_names[0]),type);
}

uint8_t helf_mn10300_relocation_size_get(uint32_t type)
{
    return helf_arch_relocation_type_size_get(helf_mn10300_relocation_type_names,sizeof(helf_mn10300_relocation_type_names)/sizeof(helf_mn10300_relocation_type_names[0]),type);
}

const char *helf_mn10300_section_type_name_get(uint32_t type)
{
    (void)type;
    return NULL;
}

bool helf_mn10300_relocation_address_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_relocation_address_info_t *info)
{
    if(!helf_mn10300_is_mn10300(input_file) || relocation==NULL)
    {
        return false;
    }
    uint8_t size=helf_mn10300_relocation_size_get(relocation->type);
    if(size==0)
    {
        return false;
    }
    return helf_arch_relocation_address_info_get(input_file,relocation,size,info);
}

size_t helf_mn10300_relocation_data_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,void *buffer,size_t buffer_length)
{
    if(!helf_mn10300_is_mn10300(input_file) || relocation==NULL)
    {
        return 0;
    }
    uint8_t size=helf_mn10300_relocation_size_get(relocation->type);
    if(size==0)
    {
        return 0;
    }
    return helf_arch_relocation_data_get(input_file,relocation,size,buffer,buffer_length);
}

bool helf_mn10300_is_shared_object(helf_file_input_t *input_file)
{
    return helf_arch_is_shared_object(input_file,helf_mn10300_machines,sizeof(helf_mn10300_machines)/sizeof(helf_mn10300_machines[0]),HELF_ELFCLASS_32);
}

bool helf_mn10300_is_relocatable(helf_file_input_t *input_file)
{
    return helf_arch_is_relocatable(input_file,helf_mn10300_machines,sizeof(helf_mn10300_machines)/sizeof(helf_mn10300_machines[0]),HELF_ELFCLASS_32);
}

size_t helf_mn10300_dynamic_count_get(helf_file_input_t *input_file)
{
    if(!helf_mn10300_is_mn10300(input_file))
    {
        return 0;
    }
    return helf_arch_dynamic_count_get(input_file);
}

bool helf_mn10300_dynamic_info_get(helf_file_input_t *input_file,size_t index,helf_arch_dynamic_info_t *info)
{
    if(!helf_mn10300_is_mn10300(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_info_get(input_file,index,info);
}

bool helf_mn10300_dynamic_string_get(helf_file_input_t *input_file,uint64_t offset,char *buffer,size_t buffer_length)
{
    if(!helf_mn10300_is_mn10300(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_string_get(input_file,offset,buffer,buffer_length);
}

/* helf_relocation_howto_begin */
static const helf_arch_relocation_howto_t helf_mn10300_reloc_howto[]=
{
    {0x00000000u,0,0,0,0,0x0040,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000001u,4,32,0,0,0x0000,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000002u,2,16,0,0,0x0000,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000003u,1,8,0,0,0x0000,0x00000000000000FFULL,0x00000000000000FFULL},
    {0x00000004u,4,32,0,0,0x0003,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000005u,2,16,0,0,0x0003,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000006u,1,8,0,0,0x0003,0x00000000000000FFULL,0x00000000000000FFULL},
    {0x00000007u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000008u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000009u,4,24,0,0,0x0000,0x0000000000FFFFFFULL,0x0000000000FFFFFFULL},
    {0x0000000Au,4,32,0,0,0x0603,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x0000000Bu,2,16,0,0,0x0203,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x0000000Cu,4,32,0,0,0x0200,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x0000000Du,4,24,0,0,0x0200,0x0000000000FFFFFFULL,0x0000000000FFFFFFULL},
    {0x0000000Eu,2,16,0,0,0x0200,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x0000000Fu,4,32,0,0,0x0003,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000010u,2,16,0,0,0x0003,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000011u,4,32,0,0,0x0200,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000012u,4,24,0,0,0x0200,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000013u,2,16,0,0,0x0200,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000014u,4,32,0,0,0x0180,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000015u,4,32,0,0,0x0100,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000016u,4,32,0,0,0x0140,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000017u,4,32,0,0,0x0080,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000018u,4,32,0,0,0x01C0,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000019u,4,32,0,0,0x01C0,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x0000001Au,4,32,0,0,0x01C0,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x0000001Bu,4,32,0,0,0x03C0,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x0000001Cu,4,32,0,0,0x01C0,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x0000001Du,4,32,0,0,0x01C0,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x0000001Eu,4,32,0,0,0x01C0,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x0000001Fu,4,32,0,0,0x01C0,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000020u,4,32,0,0,0x01C0,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000021u,4,32,0,0,0x0000,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000022u,1,32,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
};

bool helf_mn10300_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto)
{
    if(howto==NULL)
    {
        return false;
    }
    size_t count=sizeof(helf_mn10300_reloc_howto)/sizeof(helf_mn10300_reloc_howto[0]);
    if(count==0)
    {
        return false;
    }
    for(size_t i=0;i<count;i++)
    {
        if(helf_mn10300_reloc_howto[i].type==type)
        {
            *howto=helf_mn10300_reloc_howto[i];
            return true;
        }
    }
    return false;
}
/* helf_relocation_howto_end */


