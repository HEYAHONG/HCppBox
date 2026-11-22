/***************************************************************
 * Name:      helf_arch_i386.c
 * Purpose:   实现helf_arch_i386接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "helf_arch_i386.h"

static const uint16_t helf_i386_machines[]=
{
    HELF_EM_386,
};

bool helf_i386_is_i386(helf_file_input_t *input_file)
{
    return helf_arch_is(input_file,helf_i386_machines,sizeof(helf_i386_machines)/sizeof(helf_i386_machines[0]),HELF_ELFCLASS_32);
}

bool helf_i386_is_big_endian(helf_file_input_t *input_file)
{
    if(!helf_i386_is_i386(input_file))
    {
        return false;
    }
    return helf_file_input_is_big_endian(input_file);
}

bool helf_i386_is_executable(helf_file_input_t *input_file)
{
    return helf_arch_is_executable(input_file,helf_i386_machines,sizeof(helf_i386_machines)/sizeof(helf_i386_machines[0]),HELF_ELFCLASS_32);
}

bool helf_i386_entry_get(helf_file_input_t *input_file,uint32_t *entry)
{
    if(entry==NULL)
    {
        return false;
    }
    if(!helf_i386_is_i386(input_file))
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

size_t helf_i386_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_i386_is_i386(input_file))
    {
        return 0;
    }
    return helf_arch_section_count_get(input_file);
}

bool helf_i386_section_info_get(helf_file_input_t *input_file,size_t index,helf_i386_section_info_t *info)
{
    if(!helf_i386_is_i386(input_file))
    {
        return false;
    }
    return helf_arch_section_info_get(input_file,index,info);
}

bool helf_i386_is_rx_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_i386_is_i386(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_section(index,input_file);
}

intptr_t helf_i386_rx_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_i386_is_rx_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rx_section_offset_get(index,input_file);
}

size_t helf_i386_rx_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_i386_is_i386(input_file))
    {
        return 0;
    }
    return helf_arch_rx_section_size_get(input_file);
}

bool helf_i386_is_ro_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_i386_is_i386(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_section(index,input_file);
}

intptr_t helf_i386_ro_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_i386_is_ro_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_ro_section_offset_get(index,input_file);
}

size_t helf_i386_ro_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_i386_is_i386(input_file))
    {
        return 0;
    }
    return helf_arch_ro_section_size_get(input_file);
}

bool helf_i386_is_rw_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_i386_is_i386(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_section(index,input_file);
}

intptr_t helf_i386_rw_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_i386_is_rw_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rw_section_offset_get(index,input_file);
}

size_t helf_i386_rw_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_i386_is_i386(input_file))
    {
        return 0;
    }
    return helf_arch_rw_section_size_get(input_file);
}

bool helf_i386_is_bss_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_i386_is_i386(input_file))
    {
        return false;
    }
    return helf_arch_is_bss_section(index,input_file);
}

size_t helf_i386_program_header_count_get(helf_file_input_t *input_file)
{
    if(!helf_i386_is_i386(input_file))
    {
        return 0;
    }
    return helf_arch_program_header_count_get(input_file);
}

bool helf_i386_program_header_get(helf_file_input_t *input_file,size_t index,helf_elf32_program_header_t *programheader)
{
    if(programheader==NULL)
    {
        return false;
    }
    if(!helf_i386_is_i386(input_file))
    {
        return false;
    }
    if(index>=helf_arch_program_header_count_get(input_file))
    {
        return false;
    }
    return helf_file_input_32_bits_program_header_get(input_file,index,programheader);
}

size_t helf_i386_segment_count_get(helf_file_input_t *input_file)
{
    return helf_i386_program_header_count_get(input_file);
}

bool helf_i386_segment_info_get(helf_file_input_t *input_file,size_t index,helf_i386_segment_info_t *info)
{
    if(!helf_i386_is_i386(input_file))
    {
        return false;
    }
    return helf_arch_segment_info_get(input_file,index,info);
}

bool helf_i386_is_load_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_i386_is_i386(input_file))
    {
        return false;
    }
    return helf_arch_is_load_segment(index,input_file);
}

bool helf_i386_is_rx_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_i386_is_i386(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_segment(index,input_file);
}

bool helf_i386_is_ro_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_i386_is_i386(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_segment(index,input_file);
}

bool helf_i386_is_rw_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_i386_is_i386(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_segment(index,input_file);
}

size_t helf_i386_load_segment_count_get(helf_file_input_t *input_file)
{
    if(!helf_i386_is_i386(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_count_get(input_file);
}

bool helf_i386_load_segment_info_get(helf_file_input_t *input_file,size_t index,helf_i386_segment_info_t *info)
{
    if(!helf_i386_is_i386(input_file))
    {
        return false;
    }
    return helf_arch_load_segment_info_get(input_file,index,info);
}

size_t helf_i386_load_segment_total_filesz_get(helf_file_input_t *input_file)
{
    if(!helf_i386_is_i386(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_filesz_get(input_file);
}

size_t helf_i386_load_segment_total_memsz_get(helf_file_input_t *input_file)
{
    if(!helf_i386_is_i386(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_memsz_get(input_file);
}

size_t helf_i386_relocation_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_i386_is_i386(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_section_count_get(input_file);
}

bool helf_i386_relocation_section_info_get(helf_file_input_t *input_file,size_t index,helf_arch_relocation_section_info_t *info)
{
    if(!helf_i386_is_i386(input_file))
    {
        return false;
    }
    return helf_arch_relocation_section_info_get(input_file,index,info);
}

size_t helf_i386_relocation_count_get(helf_file_input_t *input_file,size_t section_index)
{
    if(!helf_i386_is_i386(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_count_get(input_file,section_index);
}

bool helf_i386_relocation_get(helf_file_input_t *input_file,size_t section_index,size_t index,helf_arch_relocation_info_t *info)
{
    if(!helf_i386_is_i386(input_file))
    {
        return false;
    }
    if(!helf_arch_relocation_get(input_file,section_index,index,info))
    {
        return false;
    }
    return true;
}

bool helf_i386_relocation_symbol_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_symbol_info_t *info)
{
    if(!helf_i386_is_i386(input_file))
    {
        return false;
    }
    return helf_arch_relocation_symbol_info_get(input_file,relocation,info);
}

static const helf_arch_relocation_type_name_t helf_i386_relocation_type_names[]=
{
    {0,0,"R_386_NONE"},
    {1,4,"R_386_32"},
    {2,4,"R_386_PC32"},
    {3,4,"R_386_GOT32"},
    {4,4,"R_386_PLT32"},
    {5,4,"R_386_COPY"},
    {6,4,"R_386_GLOB_DAT"},
    {7,4,"R_386_JUMP_SLOT"},
    {8,4,"R_386_RELATIVE"},
    {9,4,"R_386_GOTOFF"},
    {10,4,"R_386_GOTPC"},
    {11,0,"R_386_32PLT"},
    {14,4,"R_386_TLS_TPOFF"},
    {15,4,"R_386_TLS_IE"},
    {16,4,"R_386_TLS_GOTIE"},
    {17,4,"R_386_TLS_LE"},
    {18,4,"R_386_TLS_GD"},
    {19,4,"R_386_TLS_LDM"},
    {20,2,"R_386_16"},
    {21,2,"R_386_PC16"},
    {22,1,"R_386_8"},
    {23,1,"R_386_PC8"},
    {24,4,"R_386_TLS_GD_32"},
    {25,0,"R_386_TLS_GD_PUSH"},
    {26,0,"R_386_TLS_GD_CALL"},
    {27,0,"R_386_TLS_GD_POP"},
    {28,4,"R_386_TLS_LDM_32"},
    {29,0,"R_386_TLS_LDM_PUSH"},
    {30,0,"R_386_TLS_LDM_CALL"},
    {31,0,"R_386_TLS_LDM_POP"},
    {32,4,"R_386_TLS_LDO_32"},
    {33,4,"R_386_TLS_IE_32"},
    {34,4,"R_386_TLS_LE_32"},
    {35,4,"R_386_TLS_DTPMOD32"},
    {36,4,"R_386_TLS_DTPOFF32"},
    {37,4,"R_386_TLS_TPOFF32"},
    {38,4,"R_386_SIZE32"},
    {39,4,"R_386_TLS_GOTDESC"},
    {40,0,"R_386_TLS_DESC_CALL"},
    {41,4,"R_386_TLS_DESC"},
    {42,4,"R_386_IRELATIVE"},
    {43,4,"R_386_GOT32X"},
    {200,0,"R_386_USED_BY_INTEL_200"},
    {250,4,"R_386_GNU_VTINHERIT"},
    {251,4,"R_386_GNU_VTENTRY"},
};

const char *helf_i386_relocation_type_name_get(uint32_t type)
{
    return helf_arch_relocation_type_name_get(helf_i386_relocation_type_names,sizeof(helf_i386_relocation_type_names)/sizeof(helf_i386_relocation_type_names[0]),type);
}

uint8_t helf_i386_relocation_size_get(uint32_t type)
{
    return helf_arch_relocation_type_size_get(helf_i386_relocation_type_names,sizeof(helf_i386_relocation_type_names)/sizeof(helf_i386_relocation_type_names[0]),type);
}

const char *helf_i386_section_type_name_get(uint32_t type)
{
    (void)type;
    return NULL;
}

bool helf_i386_relocation_address_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_relocation_address_info_t *info)
{
    if(!helf_i386_is_i386(input_file) || relocation==NULL)
    {
        return false;
    }
    uint8_t size=helf_i386_relocation_size_get(relocation->type);
    if(size==0)
    {
        return false;
    }
    return helf_arch_relocation_address_info_get(input_file,relocation,size,info);
}

size_t helf_i386_relocation_data_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,void *buffer,size_t buffer_length)
{
    if(!helf_i386_is_i386(input_file) || relocation==NULL)
    {
        return 0;
    }
    uint8_t size=helf_i386_relocation_size_get(relocation->type);
    if(size==0)
    {
        return 0;
    }
    return helf_arch_relocation_data_get(input_file,relocation,size,buffer,buffer_length);
}

bool helf_i386_is_shared_object(helf_file_input_t *input_file)
{
    return helf_arch_is_shared_object(input_file,helf_i386_machines,sizeof(helf_i386_machines)/sizeof(helf_i386_machines[0]),HELF_ELFCLASS_32);
}

bool helf_i386_is_relocatable(helf_file_input_t *input_file)
{
    return helf_arch_is_relocatable(input_file,helf_i386_machines,sizeof(helf_i386_machines)/sizeof(helf_i386_machines[0]),HELF_ELFCLASS_32);
}

size_t helf_i386_dynamic_count_get(helf_file_input_t *input_file)
{
    if(!helf_i386_is_i386(input_file))
    {
        return 0;
    }
    return helf_arch_dynamic_count_get(input_file);
}

bool helf_i386_dynamic_info_get(helf_file_input_t *input_file,size_t index,helf_arch_dynamic_info_t *info)
{
    if(!helf_i386_is_i386(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_info_get(input_file,index,info);
}

bool helf_i386_dynamic_string_get(helf_file_input_t *input_file,uint64_t offset,char *buffer,size_t buffer_length)
{
    if(!helf_i386_is_i386(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_string_get(input_file,offset,buffer,buffer_length);
}

/* helf_relocation_howto_begin */
static const helf_arch_relocation_howto_t helf_i386_reloc_howto[]=
{
    {0x00000000u,0,0,0,0,0x0048,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000001u,4,32,0,0,0x0008,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000002u,4,32,0,0,0x000B,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000003u,4,32,0,0,0x0208,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000004u,4,32,0,0,0x000B,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000005u,4,32,0,0,0x0188,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000006u,4,32,0,0,0x0108,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000007u,4,32,0,0,0x0148,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000008u,4,32,0,0,0x0088,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000009u,4,32,0,0,0x0208,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x0000000Au,4,32,0,0,0x020B,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x0000000Bu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000000Eu,4,32,0,0,0x01C8,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x0000000Fu,4,32,0,0,0x01C8,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000010u,4,32,0,0,0x03C8,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000011u,4,32,0,0,0x01C8,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000012u,4,32,0,0,0x01C8,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000013u,4,32,0,0,0x01C8,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000014u,2,16,0,0,0x0008,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000015u,2,16,0,0,0x000B,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000016u,1,8,0,0,0x0008,0x00000000000000FFULL,0x00000000000000FFULL},
    {0x00000017u,1,8,0,0,0x000B,0x00000000000000FFULL,0x00000000000000FFULL},
    {0x00000018u,4,32,0,0,0x01C0,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000019u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Au,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Bu,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Cu,4,32,0,0,0x01C0,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x0000001Du,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Eu,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Fu,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000020u,4,32,0,0,0x01C8,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000021u,4,32,0,0,0x01C8,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000022u,4,32,0,0,0x01C8,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000023u,4,32,0,0,0x01C8,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000024u,4,32,0,0,0x01C8,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000025u,4,32,0,0,0x01C8,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000026u,4,32,0,0,0x0028,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000027u,4,32,0,0,0x03C8,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000028u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000029u,4,32,0,0,0x01C8,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x0000002Au,4,32,0,0,0x0088,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x0000002Bu,4,32,0,0,0x0208,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x000000C8u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000FAu,4,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000FBu,4,0,0,0,0x0010,0x0000000000000000ULL,0x0000000000000000ULL},
};

bool helf_i386_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto)
{
    if(howto==NULL)
    {
        return false;
    }
    size_t count=sizeof(helf_i386_reloc_howto)/sizeof(helf_i386_reloc_howto[0]);
    if(count==0)
    {
        return false;
    }
    for(size_t i=0;i<count;i++)
    {
        if(helf_i386_reloc_howto[i].type==type)
        {
            *howto=helf_i386_reloc_howto[i];
            return true;
        }
    }
    return false;
}
/* helf_relocation_howto_end */

