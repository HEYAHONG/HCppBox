/***************************************************************
 * Name:      helf_arch_metag.c
 * Purpose:   实现helf_arch_metag接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "helf_arch_metag.h"

static const uint16_t helf_metag_machines[]=
{
    HELF_EM_METAG,
};

bool helf_metag_is_metag(helf_file_input_t *input_file)
{
    return helf_arch_is(input_file,helf_metag_machines,sizeof(helf_metag_machines)/sizeof(helf_metag_machines[0]),HELF_ELFCLASS_32);
}

bool helf_metag_is_big_endian(helf_file_input_t *input_file)
{
    if(!helf_metag_is_metag(input_file))
    {
        return false;
    }
    return helf_file_input_is_big_endian(input_file);
}

bool helf_metag_is_executable(helf_file_input_t *input_file)
{
    return helf_arch_is_executable(input_file,helf_metag_machines,sizeof(helf_metag_machines)/sizeof(helf_metag_machines[0]),HELF_ELFCLASS_32);
}

bool helf_metag_entry_get(helf_file_input_t *input_file,uint32_t *entry)
{
    if(entry==NULL)
    {
        return false;
    }
    if(!helf_metag_is_metag(input_file))
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

size_t helf_metag_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_metag_is_metag(input_file))
    {
        return 0;
    }
    return helf_arch_section_count_get(input_file);
}

bool helf_metag_section_info_get(helf_file_input_t *input_file,size_t index,helf_metag_section_info_t *info)
{
    if(!helf_metag_is_metag(input_file))
    {
        return false;
    }
    return helf_arch_section_info_get(input_file,index,info);
}

bool helf_metag_is_rx_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_metag_is_metag(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_section(index,input_file);
}

intptr_t helf_metag_rx_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_metag_is_rx_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rx_section_offset_get(index,input_file);
}

size_t helf_metag_rx_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_metag_is_metag(input_file))
    {
        return 0;
    }
    return helf_arch_rx_section_size_get(input_file);
}

bool helf_metag_is_ro_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_metag_is_metag(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_section(index,input_file);
}

intptr_t helf_metag_ro_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_metag_is_ro_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_ro_section_offset_get(index,input_file);
}

size_t helf_metag_ro_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_metag_is_metag(input_file))
    {
        return 0;
    }
    return helf_arch_ro_section_size_get(input_file);
}

bool helf_metag_is_rw_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_metag_is_metag(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_section(index,input_file);
}

intptr_t helf_metag_rw_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_metag_is_rw_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rw_section_offset_get(index,input_file);
}

size_t helf_metag_rw_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_metag_is_metag(input_file))
    {
        return 0;
    }
    return helf_arch_rw_section_size_get(input_file);
}

bool helf_metag_is_bss_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_metag_is_metag(input_file))
    {
        return false;
    }
    return helf_arch_is_bss_section(index,input_file);
}

size_t helf_metag_program_header_count_get(helf_file_input_t *input_file)
{
    if(!helf_metag_is_metag(input_file))
    {
        return 0;
    }
    return helf_arch_program_header_count_get(input_file);
}

bool helf_metag_program_header_get(helf_file_input_t *input_file,size_t index,helf_elf32_program_header_t *programheader)
{
    if(programheader==NULL)
    {
        return false;
    }
    if(!helf_metag_is_metag(input_file))
    {
        return false;
    }
    if(index>=helf_arch_program_header_count_get(input_file))
    {
        return false;
    }
    return helf_file_input_32_bits_program_header_get(input_file,index,programheader);
}

size_t helf_metag_segment_count_get(helf_file_input_t *input_file)
{
    return helf_metag_program_header_count_get(input_file);
}

bool helf_metag_segment_info_get(helf_file_input_t *input_file,size_t index,helf_metag_segment_info_t *info)
{
    if(!helf_metag_is_metag(input_file))
    {
        return false;
    }
    return helf_arch_segment_info_get(input_file,index,info);
}

bool helf_metag_is_load_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_metag_is_metag(input_file))
    {
        return false;
    }
    return helf_arch_is_load_segment(index,input_file);
}

bool helf_metag_is_rx_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_metag_is_metag(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_segment(index,input_file);
}

bool helf_metag_is_ro_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_metag_is_metag(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_segment(index,input_file);
}

bool helf_metag_is_rw_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_metag_is_metag(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_segment(index,input_file);
}

size_t helf_metag_load_segment_count_get(helf_file_input_t *input_file)
{
    if(!helf_metag_is_metag(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_count_get(input_file);
}

bool helf_metag_load_segment_info_get(helf_file_input_t *input_file,size_t index,helf_metag_segment_info_t *info)
{
    if(!helf_metag_is_metag(input_file))
    {
        return false;
    }
    return helf_arch_load_segment_info_get(input_file,index,info);
}

size_t helf_metag_load_segment_total_filesz_get(helf_file_input_t *input_file)
{
    if(!helf_metag_is_metag(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_filesz_get(input_file);
}

size_t helf_metag_load_segment_total_memsz_get(helf_file_input_t *input_file)
{
    if(!helf_metag_is_metag(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_memsz_get(input_file);
}

size_t helf_metag_relocation_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_metag_is_metag(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_section_count_get(input_file);
}

bool helf_metag_relocation_section_info_get(helf_file_input_t *input_file,size_t index,helf_arch_relocation_section_info_t *info)
{
    if(!helf_metag_is_metag(input_file))
    {
        return false;
    }
    return helf_arch_relocation_section_info_get(input_file,index,info);
}

size_t helf_metag_relocation_count_get(helf_file_input_t *input_file,size_t section_index)
{
    if(!helf_metag_is_metag(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_count_get(input_file,section_index);
}

bool helf_metag_relocation_get(helf_file_input_t *input_file,size_t section_index,size_t index,helf_arch_relocation_info_t *info)
{
    if(!helf_metag_is_metag(input_file))
    {
        return false;
    }
    if(!helf_arch_relocation_get(input_file,section_index,index,info))
    {
        return false;
    }
    return true;
}

bool helf_metag_relocation_symbol_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_symbol_info_t *info)
{
    if(!helf_metag_is_metag(input_file))
    {
        return false;
    }
    return helf_arch_relocation_symbol_info_get(input_file,relocation,info);
}

static const helf_arch_relocation_type_name_t helf_metag_relocation_type_names[]=
{
    {0,4,"R_METAG_HIADDR16"},
    {1,4,"R_METAG_LOADDR16"},
    {2,4,"R_METAG_ADDR32"},
    {3,0,"R_METAG_NONE"},
    {4,4,"R_METAG_RELBRANCH"},
    {5,2,"R_METAG_GETSETOFF"},
    {6,0,"R_METAG_REG32OP1"},
    {7,0,"R_METAG_REG32OP2"},
    {8,0,"R_METAG_REG32OP3"},
    {9,0,"R_METAG_REG16OP1"},
    {10,0,"R_METAG_REG16OP2"},
    {11,0,"R_METAG_REG16OP3"},
    {12,0,"R_METAG_REG32OP4"},
    {13,0,"R_METAG_HIOG"},
    {14,0,"R_METAG_LOOG"},
    {15,1,"R_METAG_REL8"},
    {16,2,"R_METAG_REL16"},
    {30,4,"R_METAG_GNU_VTINHERIT"},
    {31,4,"R_METAG_GNU_VTENTRY"},
    {32,4,"R_METAG_HI16_GOTOFF"},
    {33,4,"R_METAG_LO16_GOTOFF"},
    {34,2,"R_METAG_GETSET_GOTOFF"},
    {35,2,"R_METAG_GETSET_GOT"},
    {36,4,"R_METAG_HI16_GOTPC"},
    {37,4,"R_METAG_LO16_GOTPC"},
    {38,4,"R_METAG_HI16_PLT"},
    {39,4,"R_METAG_LO16_PLT"},
    {40,4,"R_METAG_RELBRANCH_PLT"},
    {41,4,"R_METAG_GOTOFF"},
    {42,4,"R_METAG_PLT"},
    {43,4,"R_METAG_COPY"},
    {44,4,"R_METAG_JMP_SLOT"},
    {45,4,"R_METAG_RELATIVE"},
    {46,4,"R_METAG_GLOB_DAT"},
    {47,4,"R_METAG_TLS_GD"},
    {48,4,"R_METAG_TLS_LDM"},
    {49,4,"R_METAG_TLS_LDO_HI16"},
    {50,4,"R_METAG_TLS_LDO_LO16"},
    {51,4,"R_METAG_TLS_LDO"},
    {52,4,"R_METAG_TLS_IE"},
    {53,4,"R_METAG_TLS_IENONPIC"},
    {54,4,"R_METAG_TLS_IENONPIC_HI16"},
    {55,4,"R_METAG_TLS_IENONPIC_LO16"},
    {56,4,"R_METAG_TLS_TPOFF"},
    {57,4,"R_METAG_TLS_DTPMOD"},
    {58,4,"R_METAG_TLS_DTPOFF"},
    {59,4,"R_METAG_TLS_LE"},
    {60,4,"R_METAG_TLS_LE_HI16"},
    {61,4,"R_METAG_TLS_LE_LO16"},
};

const char *helf_metag_relocation_type_name_get(uint32_t type)
{
    return helf_arch_relocation_type_name_get(helf_metag_relocation_type_names,sizeof(helf_metag_relocation_type_names)/sizeof(helf_metag_relocation_type_names[0]),type);
}

uint8_t helf_metag_relocation_size_get(uint32_t type)
{
    return helf_arch_relocation_type_size_get(helf_metag_relocation_type_names,sizeof(helf_metag_relocation_type_names)/sizeof(helf_metag_relocation_type_names[0]),type);
}

const char *helf_metag_section_type_name_get(uint32_t type)
{
    (void)type;
    return NULL;
}

bool helf_metag_relocation_address_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_relocation_address_info_t *info)
{
    if(!helf_metag_is_metag(input_file) || relocation==NULL)
    {
        return false;
    }
    uint8_t size=helf_metag_relocation_size_get(relocation->type);
    if(size==0)
    {
        return false;
    }
    return helf_arch_relocation_address_info_get(input_file,relocation,size,info);
}

size_t helf_metag_relocation_data_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,void *buffer,size_t buffer_length)
{
    if(!helf_metag_is_metag(input_file) || relocation==NULL)
    {
        return 0;
    }
    uint8_t size=helf_metag_relocation_size_get(relocation->type);
    if(size==0)
    {
        return 0;
    }
    return helf_arch_relocation_data_get(input_file,relocation,size,buffer,buffer_length);
}

bool helf_metag_is_shared_object(helf_file_input_t *input_file)
{
    return helf_arch_is_shared_object(input_file,helf_metag_machines,sizeof(helf_metag_machines)/sizeof(helf_metag_machines[0]),HELF_ELFCLASS_32);
}

bool helf_metag_is_relocatable(helf_file_input_t *input_file)
{
    return helf_arch_is_relocatable(input_file,helf_metag_machines,sizeof(helf_metag_machines)/sizeof(helf_metag_machines[0]),HELF_ELFCLASS_32);
}

size_t helf_metag_dynamic_count_get(helf_file_input_t *input_file)
{
    if(!helf_metag_is_metag(input_file))
    {
        return 0;
    }
    return helf_arch_dynamic_count_get(input_file);
}

bool helf_metag_dynamic_info_get(helf_file_input_t *input_file,size_t index,helf_arch_dynamic_info_t *info)
{
    if(!helf_metag_is_metag(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_info_get(input_file,index,info);
}

bool helf_metag_dynamic_string_get(helf_file_input_t *input_file,uint64_t offset,char *buffer,size_t buffer_length)
{
    if(!helf_metag_is_metag(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_string_get(input_file,offset,buffer,buffer_length);
}
