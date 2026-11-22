/***************************************************************
 * Name:      helf_arch_nfp.c
 * Purpose:   实现helf_arch_nfp接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "helf_arch_nfp.h"

static const uint16_t helf_nfp_machines[]=
{
    HELF_EM_NFP,
};

bool helf_nfp_is_nfp(helf_file_input_t *input_file)
{
    return helf_arch_is(input_file,helf_nfp_machines,sizeof(helf_nfp_machines)/sizeof(helf_nfp_machines[0]),HELF_ELFCLASS_64);
}

bool helf_nfp_is_big_endian(helf_file_input_t *input_file)
{
    if(!helf_nfp_is_nfp(input_file))
    {
        return false;
    }
    return helf_file_input_is_big_endian(input_file);
}

bool helf_nfp_is_executable(helf_file_input_t *input_file)
{
    return helf_arch_is_executable(input_file,helf_nfp_machines,sizeof(helf_nfp_machines)/sizeof(helf_nfp_machines[0]),HELF_ELFCLASS_64);
}

bool helf_nfp_entry_get(helf_file_input_t *input_file,uint64_t *entry)
{
    if(entry==NULL)
    {
        return false;
    }
    if(!helf_nfp_is_nfp(input_file))
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

size_t helf_nfp_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_nfp_is_nfp(input_file))
    {
        return 0;
    }
    return helf_arch_section_count_get(input_file);
}

bool helf_nfp_section_info_get(helf_file_input_t *input_file,size_t index,helf_nfp_section_info_t *info)
{
    if(!helf_nfp_is_nfp(input_file))
    {
        return false;
    }
    return helf_arch_section_info_get(input_file,index,info);
}

bool helf_nfp_is_rx_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_nfp_is_nfp(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_section(index,input_file);
}

intptr_t helf_nfp_rx_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_nfp_is_rx_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rx_section_offset_get(index,input_file);
}

size_t helf_nfp_rx_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_nfp_is_nfp(input_file))
    {
        return 0;
    }
    return helf_arch_rx_section_size_get(input_file);
}

bool helf_nfp_is_ro_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_nfp_is_nfp(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_section(index,input_file);
}

intptr_t helf_nfp_ro_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_nfp_is_ro_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_ro_section_offset_get(index,input_file);
}

size_t helf_nfp_ro_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_nfp_is_nfp(input_file))
    {
        return 0;
    }
    return helf_arch_ro_section_size_get(input_file);
}

bool helf_nfp_is_rw_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_nfp_is_nfp(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_section(index,input_file);
}

intptr_t helf_nfp_rw_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_nfp_is_rw_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rw_section_offset_get(index,input_file);
}

size_t helf_nfp_rw_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_nfp_is_nfp(input_file))
    {
        return 0;
    }
    return helf_arch_rw_section_size_get(input_file);
}

bool helf_nfp_is_bss_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_nfp_is_nfp(input_file))
    {
        return false;
    }
    return helf_arch_is_bss_section(index,input_file);
}

size_t helf_nfp_program_header_count_get(helf_file_input_t *input_file)
{
    if(!helf_nfp_is_nfp(input_file))
    {
        return 0;
    }
    return helf_arch_program_header_count_get(input_file);
}

bool helf_nfp_program_header_get(helf_file_input_t *input_file,size_t index,helf_elf64_program_header_t *programheader)
{
    if(programheader==NULL)
    {
        return false;
    }
    if(!helf_nfp_is_nfp(input_file))
    {
        return false;
    }
    if(index>=helf_arch_program_header_count_get(input_file))
    {
        return false;
    }
    return helf_file_input_64_bits_program_header_get(input_file,index,programheader);
}

size_t helf_nfp_segment_count_get(helf_file_input_t *input_file)
{
    return helf_nfp_program_header_count_get(input_file);
}

bool helf_nfp_segment_info_get(helf_file_input_t *input_file,size_t index,helf_nfp_segment_info_t *info)
{
    if(!helf_nfp_is_nfp(input_file))
    {
        return false;
    }
    return helf_arch_segment_info_get(input_file,index,info);
}

bool helf_nfp_is_load_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_nfp_is_nfp(input_file))
    {
        return false;
    }
    return helf_arch_is_load_segment(index,input_file);
}

bool helf_nfp_is_rx_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_nfp_is_nfp(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_segment(index,input_file);
}

bool helf_nfp_is_ro_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_nfp_is_nfp(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_segment(index,input_file);
}

bool helf_nfp_is_rw_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_nfp_is_nfp(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_segment(index,input_file);
}

size_t helf_nfp_load_segment_count_get(helf_file_input_t *input_file)
{
    if(!helf_nfp_is_nfp(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_count_get(input_file);
}

bool helf_nfp_load_segment_info_get(helf_file_input_t *input_file,size_t index,helf_nfp_segment_info_t *info)
{
    if(!helf_nfp_is_nfp(input_file))
    {
        return false;
    }
    return helf_arch_load_segment_info_get(input_file,index,info);
}

size_t helf_nfp_load_segment_total_filesz_get(helf_file_input_t *input_file)
{
    if(!helf_nfp_is_nfp(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_filesz_get(input_file);
}

size_t helf_nfp_load_segment_total_memsz_get(helf_file_input_t *input_file)
{
    if(!helf_nfp_is_nfp(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_memsz_get(input_file);
}

size_t helf_nfp_relocation_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_nfp_is_nfp(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_section_count_get(input_file);
}

bool helf_nfp_relocation_section_info_get(helf_file_input_t *input_file,size_t index,helf_arch_relocation_section_info_t *info)
{
    if(!helf_nfp_is_nfp(input_file))
    {
        return false;
    }
    return helf_arch_relocation_section_info_get(input_file,index,info);
}

size_t helf_nfp_relocation_count_get(helf_file_input_t *input_file,size_t section_index)
{
    if(!helf_nfp_is_nfp(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_count_get(input_file,section_index);
}

bool helf_nfp_relocation_get(helf_file_input_t *input_file,size_t section_index,size_t index,helf_arch_relocation_info_t *info)
{
    if(!helf_nfp_is_nfp(input_file))
    {
        return false;
    }
    if(!helf_arch_relocation_get(input_file,section_index,index,info))
    {
        return false;
    }
    return true;
}

bool helf_nfp_relocation_symbol_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_symbol_info_t *info)
{
    if(!helf_nfp_is_nfp(input_file))
    {
        return false;
    }
    return helf_arch_relocation_symbol_info_get(input_file,relocation,info);
}

static const helf_arch_relocation_type_name_t helf_nfp_relocation_type_names[]=
{
    {0,0,"R_NFP3200_NOTYPE"},
    {1,0,"R_NFP3200_W32LE"},
    {2,0,"R_NFP3200_SRC8_A"},
    {3,0,"R_NFP3200_SRC8_B"},
    {4,0,"R_NFP3200_IMMED8_I"},
    {5,0,"R_NFP3200_SC"},
    {6,0,"R_NFP3200_IMMED_LO16_I_A"},
    {7,0,"R_NFP3200_IMMED_LO16_I_B"},
    {8,0,"R_NFP3200_SRC7_B"},
    {9,0,"R_NFP3200_SRC7_A"},
    {10,0,"R_NFP3200_SRC8_I_B"},
    {11,0,"R_NFP3200_SRC8_I_A"},
    {12,0,"R_NFP3200_IMMED_HI16_I_A"},
    {13,0,"R_NFP3200_IMMED_HI16_I_B"},
    {14,0,"R_NFP3200_RSVD_0"},
    {15,0,"R_NFP3200_RSVD_1"},
    {16,0,"R_NFP3200_RSVD_2"},
    {17,0,"R_NFP3200_RSVD_3"},
    {18,0,"R_NFP3200_RSVD_4"},
    {19,0,"R_NFP3200_RSVD_5"},
    {20,0,"R_NFP3200_RSVD_6"},
    {21,0,"R_NFP3200_W64LE"},
    {22,0,"R_NFP3200_W32BE"},
    {23,0,"R_NFP3200_W64BE"},
    {24,0,"R_NFP3200_W32LE_AND"},
    {25,0,"R_NFP3200_W32BE_AND"},
    {26,0,"R_NFP3200_W32LE_OR"},
    {27,0,"R_NFP3200_W32BE_OR"},
    {28,0,"R_NFP3200_W64LE_AND"},
    {29,0,"R_NFP3200_W64BE_AND"},
    {30,0,"R_NFP3200_W64LE_OR"},
    {31,0,"R_NFP3200_W64BE_OR"},
};

const char *helf_nfp_relocation_type_name_get(uint32_t type)
{
    return helf_arch_relocation_type_name_get(helf_nfp_relocation_type_names,sizeof(helf_nfp_relocation_type_names)/sizeof(helf_nfp_relocation_type_names[0]),type);
}

uint8_t helf_nfp_relocation_size_get(uint32_t type)
{
    return helf_arch_relocation_type_size_get(helf_nfp_relocation_type_names,sizeof(helf_nfp_relocation_type_names)/sizeof(helf_nfp_relocation_type_names[0]),type);
}

static const helf_arch_section_type_name_t helf_nfp_section_type_names[]=
{
    {1879048193,"SHT_NFP_MECONFIG"},
    {1879048194,"SHT_NFP_INITREG"},
    {2147483648,"SHT_NFP_UDEBUG"},
};

const char *helf_nfp_section_type_name_get(uint32_t type)
{
    return helf_arch_section_type_name_get(helf_nfp_section_type_names,sizeof(helf_nfp_section_type_names)/sizeof(helf_nfp_section_type_names[0]),type);
}

bool helf_nfp_relocation_address_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_relocation_address_info_t *info)
{
    if(!helf_nfp_is_nfp(input_file) || relocation==NULL)
    {
        return false;
    }
    uint8_t size=helf_nfp_relocation_size_get(relocation->type);
    if(size==0)
    {
        return false;
    }
    return helf_arch_relocation_address_info_get(input_file,relocation,size,info);
}

size_t helf_nfp_relocation_data_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,void *buffer,size_t buffer_length)
{
    if(!helf_nfp_is_nfp(input_file) || relocation==NULL)
    {
        return 0;
    }
    uint8_t size=helf_nfp_relocation_size_get(relocation->type);
    if(size==0)
    {
        return 0;
    }
    return helf_arch_relocation_data_get(input_file,relocation,size,buffer,buffer_length);
}

bool helf_nfp_is_shared_object(helf_file_input_t *input_file)
{
    return helf_arch_is_shared_object(input_file,helf_nfp_machines,sizeof(helf_nfp_machines)/sizeof(helf_nfp_machines[0]),HELF_ELFCLASS_64);
}

bool helf_nfp_is_relocatable(helf_file_input_t *input_file)
{
    return helf_arch_is_relocatable(input_file,helf_nfp_machines,sizeof(helf_nfp_machines)/sizeof(helf_nfp_machines[0]),HELF_ELFCLASS_64);
}

size_t helf_nfp_dynamic_count_get(helf_file_input_t *input_file)
{
    if(!helf_nfp_is_nfp(input_file))
    {
        return 0;
    }
    return helf_arch_dynamic_count_get(input_file);
}

bool helf_nfp_dynamic_info_get(helf_file_input_t *input_file,size_t index,helf_arch_dynamic_info_t *info)
{
    if(!helf_nfp_is_nfp(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_info_get(input_file,index,info);
}

bool helf_nfp_dynamic_string_get(helf_file_input_t *input_file,uint64_t offset,char *buffer,size_t buffer_length)
{
    if(!helf_nfp_is_nfp(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_string_get(input_file,offset,buffer,buffer_length);
}

/* helf_relocation_howto_begin */
static const helf_arch_relocation_howto_t helf_nfp_reloc_howto[]=
{
    {0x00000000u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000001u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000002u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000003u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000004u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000005u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000006u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000007u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000008u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000009u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000000Au,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000000Bu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000000Cu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000000Du,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000000Eu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000000Fu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000010u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000011u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000012u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000013u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000014u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000015u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000016u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000017u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000018u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000019u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Au,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Bu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Cu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Du,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Eu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Fu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
};

bool helf_nfp_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto)
{
    if(howto==NULL)
    {
        return false;
    }
    size_t count=sizeof(helf_nfp_reloc_howto)/sizeof(helf_nfp_reloc_howto[0]);
    if(count==0)
    {
        return false;
    }
    for(size_t i=0;i<count;i++)
    {
        if(helf_nfp_reloc_howto[i].type==type)
        {
            *howto=helf_nfp_reloc_howto[i];
            return true;
        }
    }
    return false;
}
/* helf_relocation_howto_end */


