/***************************************************************
 * Name:      helf_arch_frv.c
 * Purpose:   实现helf_arch_frv接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "helf_arch_frv.h"

static const uint16_t helf_frv_machines[]=
{
    HELF_EM_CYGNUS_FRV,
};

bool helf_frv_is_frv(helf_file_input_t *input_file)
{
    return helf_arch_is(input_file,helf_frv_machines,sizeof(helf_frv_machines)/sizeof(helf_frv_machines[0]),HELF_ELFCLASS_32);
}

bool helf_frv_is_big_endian(helf_file_input_t *input_file)
{
    if(!helf_frv_is_frv(input_file))
    {
        return false;
    }
    return helf_file_input_is_big_endian(input_file);
}

bool helf_frv_is_executable(helf_file_input_t *input_file)
{
    return helf_arch_is_executable(input_file,helf_frv_machines,sizeof(helf_frv_machines)/sizeof(helf_frv_machines[0]),HELF_ELFCLASS_32);
}

bool helf_frv_entry_get(helf_file_input_t *input_file,uint32_t *entry)
{
    if(entry==NULL)
    {
        return false;
    }
    if(!helf_frv_is_frv(input_file))
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

size_t helf_frv_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_frv_is_frv(input_file))
    {
        return 0;
    }
    return helf_arch_section_count_get(input_file);
}

bool helf_frv_section_info_get(helf_file_input_t *input_file,size_t index,helf_frv_section_info_t *info)
{
    if(!helf_frv_is_frv(input_file))
    {
        return false;
    }
    return helf_arch_section_info_get(input_file,index,info);
}

bool helf_frv_is_rx_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_frv_is_frv(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_section(index,input_file);
}

intptr_t helf_frv_rx_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_frv_is_rx_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rx_section_offset_get(index,input_file);
}

size_t helf_frv_rx_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_frv_is_frv(input_file))
    {
        return 0;
    }
    return helf_arch_rx_section_size_get(input_file);
}

bool helf_frv_is_ro_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_frv_is_frv(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_section(index,input_file);
}

intptr_t helf_frv_ro_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_frv_is_ro_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_ro_section_offset_get(index,input_file);
}

size_t helf_frv_ro_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_frv_is_frv(input_file))
    {
        return 0;
    }
    return helf_arch_ro_section_size_get(input_file);
}

bool helf_frv_is_rw_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_frv_is_frv(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_section(index,input_file);
}

intptr_t helf_frv_rw_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_frv_is_rw_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rw_section_offset_get(index,input_file);
}

size_t helf_frv_rw_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_frv_is_frv(input_file))
    {
        return 0;
    }
    return helf_arch_rw_section_size_get(input_file);
}

bool helf_frv_is_bss_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_frv_is_frv(input_file))
    {
        return false;
    }
    return helf_arch_is_bss_section(index,input_file);
}

size_t helf_frv_program_header_count_get(helf_file_input_t *input_file)
{
    if(!helf_frv_is_frv(input_file))
    {
        return 0;
    }
    return helf_arch_program_header_count_get(input_file);
}

bool helf_frv_program_header_get(helf_file_input_t *input_file,size_t index,helf_elf32_program_header_t *programheader)
{
    if(programheader==NULL)
    {
        return false;
    }
    if(!helf_frv_is_frv(input_file))
    {
        return false;
    }
    if(index>=helf_arch_program_header_count_get(input_file))
    {
        return false;
    }
    return helf_file_input_32_bits_program_header_get(input_file,index,programheader);
}

size_t helf_frv_segment_count_get(helf_file_input_t *input_file)
{
    return helf_frv_program_header_count_get(input_file);
}

bool helf_frv_segment_info_get(helf_file_input_t *input_file,size_t index,helf_frv_segment_info_t *info)
{
    if(!helf_frv_is_frv(input_file))
    {
        return false;
    }
    return helf_arch_segment_info_get(input_file,index,info);
}

bool helf_frv_is_load_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_frv_is_frv(input_file))
    {
        return false;
    }
    return helf_arch_is_load_segment(index,input_file);
}

bool helf_frv_is_rx_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_frv_is_frv(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_segment(index,input_file);
}

bool helf_frv_is_ro_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_frv_is_frv(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_segment(index,input_file);
}

bool helf_frv_is_rw_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_frv_is_frv(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_segment(index,input_file);
}

size_t helf_frv_load_segment_count_get(helf_file_input_t *input_file)
{
    if(!helf_frv_is_frv(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_count_get(input_file);
}

bool helf_frv_load_segment_info_get(helf_file_input_t *input_file,size_t index,helf_frv_segment_info_t *info)
{
    if(!helf_frv_is_frv(input_file))
    {
        return false;
    }
    return helf_arch_load_segment_info_get(input_file,index,info);
}

size_t helf_frv_load_segment_total_filesz_get(helf_file_input_t *input_file)
{
    if(!helf_frv_is_frv(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_filesz_get(input_file);
}

size_t helf_frv_load_segment_total_memsz_get(helf_file_input_t *input_file)
{
    if(!helf_frv_is_frv(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_memsz_get(input_file);
}

size_t helf_frv_relocation_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_frv_is_frv(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_section_count_get(input_file);
}

bool helf_frv_relocation_section_info_get(helf_file_input_t *input_file,size_t index,helf_arch_relocation_section_info_t *info)
{
    if(!helf_frv_is_frv(input_file))
    {
        return false;
    }
    return helf_arch_relocation_section_info_get(input_file,index,info);
}

size_t helf_frv_relocation_count_get(helf_file_input_t *input_file,size_t section_index)
{
    if(!helf_frv_is_frv(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_count_get(input_file,section_index);
}

bool helf_frv_relocation_get(helf_file_input_t *input_file,size_t section_index,size_t index,helf_arch_relocation_info_t *info)
{
    if(!helf_frv_is_frv(input_file))
    {
        return false;
    }
    if(!helf_arch_relocation_get(input_file,section_index,index,info))
    {
        return false;
    }
    return true;
}

bool helf_frv_relocation_symbol_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_symbol_info_t *info)
{
    if(!helf_frv_is_frv(input_file))
    {
        return false;
    }
    return helf_arch_relocation_symbol_info_get(input_file,relocation,info);
}

static const helf_arch_relocation_type_name_t helf_frv_relocation_type_names[]=
{
    {0,0,"R_FRV_NONE"},
    {1,4,"R_FRV_32"},
    {2,4,"R_FRV_LABEL16"},
    {3,4,"R_FRV_LABEL24"},
    {4,4,"R_FRV_LO16"},
    {5,4,"R_FRV_HI16"},
    {6,4,"R_FRV_GPREL12"},
    {7,4,"R_FRV_GPRELU12"},
    {8,4,"R_FRV_GPREL32"},
    {9,4,"R_FRV_GPRELHI"},
    {10,4,"R_FRV_GPRELLO"},
    {11,4,"R_FRV_GOT12"},
    {12,4,"R_FRV_GOTHI"},
    {13,4,"R_FRV_GOTLO"},
    {14,4,"R_FRV_FUNCDESC"},
    {15,4,"R_FRV_FUNCDESC_GOT12"},
    {16,4,"R_FRV_FUNCDESC_GOTHI"},
    {17,4,"R_FRV_FUNCDESC_GOTLO"},
    {18,4,"R_FRV_FUNCDESC_VALUE"},
    {19,4,"R_FRV_FUNCDESC_GOTOFF12"},
    {20,4,"R_FRV_FUNCDESC_GOTOFFHI"},
    {21,4,"R_FRV_FUNCDESC_GOTOFFLO"},
    {22,4,"R_FRV_GOTOFF12"},
    {23,4,"R_FRV_GOTOFFHI"},
    {24,4,"R_FRV_GOTOFFLO"},
    {25,4,"R_FRV_GETTLSOFF"},
    {26,4,"R_FRV_TLSDESC_VALUE"},
    {27,4,"R_FRV_GOTTLSDESC12"},
    {28,4,"R_FRV_GOTTLSDESCHI"},
    {29,4,"R_FRV_GOTTLSDESCLO"},
    {30,4,"R_FRV_TLSMOFF12"},
    {31,4,"R_FRV_TLSMOFFHI"},
    {32,4,"R_FRV_TLSMOFFLO"},
    {33,4,"R_FRV_GOTTLSOFF12"},
    {34,4,"R_FRV_GOTTLSOFFHI"},
    {35,4,"R_FRV_GOTTLSOFFLO"},
    {36,4,"R_FRV_TLSOFF"},
    {37,4,"R_FRV_TLSDESC_RELAX"},
    {38,4,"R_FRV_GETTLSOFF_RELAX"},
    {39,4,"R_FRV_TLSOFF_RELAX"},
    {40,4,"R_FRV_TLSMOFF"},
    {200,4,"R_FRV_GNU_VTINHERIT"},
    {201,4,"R_FRV_GNU_VTENTRY"},
};

const char *helf_frv_relocation_type_name_get(uint32_t type)
{
    return helf_arch_relocation_type_name_get(helf_frv_relocation_type_names,sizeof(helf_frv_relocation_type_names)/sizeof(helf_frv_relocation_type_names[0]),type);
}

uint8_t helf_frv_relocation_size_get(uint32_t type)
{
    return helf_arch_relocation_type_size_get(helf_frv_relocation_type_names,sizeof(helf_frv_relocation_type_names)/sizeof(helf_frv_relocation_type_names[0]),type);
}

const char *helf_frv_section_type_name_get(uint32_t type)
{
    (void)type;
    return NULL;
}

bool helf_frv_relocation_address_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_relocation_address_info_t *info)
{
    if(!helf_frv_is_frv(input_file) || relocation==NULL)
    {
        return false;
    }
    uint8_t size=helf_frv_relocation_size_get(relocation->type);
    if(size==0)
    {
        return false;
    }
    return helf_arch_relocation_address_info_get(input_file,relocation,size,info);
}

size_t helf_frv_relocation_data_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,void *buffer,size_t buffer_length)
{
    if(!helf_frv_is_frv(input_file) || relocation==NULL)
    {
        return 0;
    }
    uint8_t size=helf_frv_relocation_size_get(relocation->type);
    if(size==0)
    {
        return 0;
    }
    return helf_arch_relocation_data_get(input_file,relocation,size,buffer,buffer_length);
}

bool helf_frv_is_shared_object(helf_file_input_t *input_file)
{
    return helf_arch_is_shared_object(input_file,helf_frv_machines,sizeof(helf_frv_machines)/sizeof(helf_frv_machines[0]),HELF_ELFCLASS_32);
}

bool helf_frv_is_relocatable(helf_file_input_t *input_file)
{
    return helf_arch_is_relocatable(input_file,helf_frv_machines,sizeof(helf_frv_machines)/sizeof(helf_frv_machines[0]),HELF_ELFCLASS_32);
}

size_t helf_frv_dynamic_count_get(helf_file_input_t *input_file)
{
    if(!helf_frv_is_frv(input_file))
    {
        return 0;
    }
    return helf_arch_dynamic_count_get(input_file);
}

bool helf_frv_dynamic_info_get(helf_file_input_t *input_file,size_t index,helf_arch_dynamic_info_t *info)
{
    if(!helf_frv_is_frv(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_info_get(input_file,index,info);
}

bool helf_frv_dynamic_string_get(helf_file_input_t *input_file,uint64_t offset,char *buffer,size_t buffer_length)
{
    if(!helf_frv_is_frv(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_string_get(input_file,offset,buffer,buffer_length);
}

/* helf_relocation_howto_begin */
static const helf_arch_relocation_howto_t helf_frv_reloc_howto[]=
{
    {0x00000000u,0,0,0,0,0x0040,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000001u,4,32,0,0,0x0000,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000002u,4,16,2,0,0x0003,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000003u,4,26,2,0,0x0003,0x000000007E03FFFFULL,0x000000007E03FFFFULL},
    {0x00000004u,4,16,0,0,0x0000,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000005u,4,16,0,0,0x0000,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000006u,4,12,0,0,0x0000,0x0000000000000FFFULL,0x0000000000000FFFULL},
    {0x00000007u,4,12,0,0,0x0000,0x0000000000000FFFULL,0x000000000003F03FULL},
    {0x00000008u,4,32,0,0,0x0000,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000009u,4,16,0,0,0x0000,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x0000000Au,4,16,0,0,0x0000,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x0000000Bu,4,12,0,0,0x0200,0x0000000000000FFFULL,0x0000000000000FFFULL},
    {0x0000000Cu,4,16,0,0,0x0200,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x0000000Du,4,16,0,0,0x0200,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x0000000Eu,4,32,0,0,0x0000,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x0000000Fu,4,12,0,0,0x0200,0x0000000000000FFFULL,0x0000000000000FFFULL},
    {0x00000010u,4,16,0,0,0x0200,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000011u,4,16,0,0,0x0200,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000012u,4,64,0,0,0x0000,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000013u,4,12,0,0,0x0200,0x0000000000000FFFULL,0x0000000000000FFFULL},
    {0x00000014u,4,16,0,0,0x0200,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000015u,4,16,0,0,0x0200,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000016u,4,12,0,0,0x0200,0x0000000000000FFFULL,0x0000000000000FFFULL},
    {0x00000017u,4,16,0,0,0x0200,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000018u,4,16,0,0,0x0200,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000019u,4,26,2,0,0x0003,0x000000007E03FFFFULL,0x000000007E03FFFFULL},
    {0x0000001Au,4,64,0,0,0x01C0,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x0000001Bu,4,12,0,0,0x03C0,0x0000000000000FFFULL,0x0000000000000FFFULL},
    {0x0000001Cu,4,16,0,0,0x03C0,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x0000001Du,4,16,0,0,0x03C0,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x0000001Eu,4,12,0,0,0x01C0,0x0000000000000FFFULL,0x0000000000000FFFULL},
    {0x0000001Fu,4,16,0,0,0x01C0,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000020u,4,16,0,0,0x01C0,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000021u,4,12,0,0,0x0200,0x0000000000000FFFULL,0x0000000000000FFFULL},
    {0x00000022u,4,16,0,0,0x0200,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000023u,4,16,0,0,0x0200,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000024u,4,32,0,0,0x01C0,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000025u,4,0,0,0,0x0040,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000026u,4,0,0,0,0x0040,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000027u,4,0,0,0,0x0040,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000028u,4,32,0,0,0x01C0,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x000000C8u,4,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000C9u,4,0,0,0,0x0010,0x0000000000000000ULL,0x0000000000000000ULL},
};

bool helf_frv_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto)
{
    if(howto==NULL)
    {
        return false;
    }
    size_t count=sizeof(helf_frv_reloc_howto)/sizeof(helf_frv_reloc_howto[0]);
    if(count==0)
    {
        return false;
    }
    for(size_t i=0;i<count;i++)
    {
        if(helf_frv_reloc_howto[i].type==type)
        {
            *howto=helf_frv_reloc_howto[i];
            return true;
        }
    }
    return false;
}
/* helf_relocation_howto_end */

