/***************************************************************
 * Name:      helf_arch_h8300.c
 * Purpose:   实现helf_arch_h8300接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "helf_arch_h8300.h"

static const uint16_t helf_h8300_machines[]=
{
    HELF_EM_H8_300,
    HELF_EM_H8_300H,
    HELF_EM_H8S,
    HELF_EM_H8_500,
};

bool helf_h8300_is_h8300(helf_file_input_t *input_file)
{
    return helf_arch_is(input_file,helf_h8300_machines,sizeof(helf_h8300_machines)/sizeof(helf_h8300_machines[0]),HELF_ELFCLASS_32);
}

bool helf_h8300_is_big_endian(helf_file_input_t *input_file)
{
    if(!helf_h8300_is_h8300(input_file))
    {
        return false;
    }
    return helf_file_input_is_big_endian(input_file);
}

bool helf_h8300_is_executable(helf_file_input_t *input_file)
{
    return helf_arch_is_executable(input_file,helf_h8300_machines,sizeof(helf_h8300_machines)/sizeof(helf_h8300_machines[0]),HELF_ELFCLASS_32);
}

bool helf_h8300_entry_get(helf_file_input_t *input_file,uint32_t *entry)
{
    if(entry==NULL)
    {
        return false;
    }
    if(!helf_h8300_is_h8300(input_file))
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

size_t helf_h8300_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_h8300_is_h8300(input_file))
    {
        return 0;
    }
    return helf_arch_section_count_get(input_file);
}

bool helf_h8300_section_info_get(helf_file_input_t *input_file,size_t index,helf_h8300_section_info_t *info)
{
    if(!helf_h8300_is_h8300(input_file))
    {
        return false;
    }
    return helf_arch_section_info_get(input_file,index,info);
}

bool helf_h8300_is_rx_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_h8300_is_h8300(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_section(index,input_file);
}

intptr_t helf_h8300_rx_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_h8300_is_rx_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rx_section_offset_get(index,input_file);
}

size_t helf_h8300_rx_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_h8300_is_h8300(input_file))
    {
        return 0;
    }
    return helf_arch_rx_section_size_get(input_file);
}

bool helf_h8300_is_ro_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_h8300_is_h8300(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_section(index,input_file);
}

intptr_t helf_h8300_ro_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_h8300_is_ro_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_ro_section_offset_get(index,input_file);
}

size_t helf_h8300_ro_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_h8300_is_h8300(input_file))
    {
        return 0;
    }
    return helf_arch_ro_section_size_get(input_file);
}

bool helf_h8300_is_rw_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_h8300_is_h8300(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_section(index,input_file);
}

intptr_t helf_h8300_rw_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_h8300_is_rw_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rw_section_offset_get(index,input_file);
}

size_t helf_h8300_rw_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_h8300_is_h8300(input_file))
    {
        return 0;
    }
    return helf_arch_rw_section_size_get(input_file);
}

bool helf_h8300_is_bss_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_h8300_is_h8300(input_file))
    {
        return false;
    }
    return helf_arch_is_bss_section(index,input_file);
}

size_t helf_h8300_program_header_count_get(helf_file_input_t *input_file)
{
    if(!helf_h8300_is_h8300(input_file))
    {
        return 0;
    }
    return helf_arch_program_header_count_get(input_file);
}

bool helf_h8300_program_header_get(helf_file_input_t *input_file,size_t index,helf_elf32_program_header_t *programheader)
{
    if(programheader==NULL)
    {
        return false;
    }
    if(!helf_h8300_is_h8300(input_file))
    {
        return false;
    }
    if(index>=helf_arch_program_header_count_get(input_file))
    {
        return false;
    }
    return helf_file_input_32_bits_program_header_get(input_file,index,programheader);
}

size_t helf_h8300_segment_count_get(helf_file_input_t *input_file)
{
    return helf_h8300_program_header_count_get(input_file);
}

bool helf_h8300_segment_info_get(helf_file_input_t *input_file,size_t index,helf_h8300_segment_info_t *info)
{
    if(!helf_h8300_is_h8300(input_file))
    {
        return false;
    }
    return helf_arch_segment_info_get(input_file,index,info);
}

bool helf_h8300_is_load_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_h8300_is_h8300(input_file))
    {
        return false;
    }
    return helf_arch_is_load_segment(index,input_file);
}

bool helf_h8300_is_rx_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_h8300_is_h8300(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_segment(index,input_file);
}

bool helf_h8300_is_ro_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_h8300_is_h8300(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_segment(index,input_file);
}

bool helf_h8300_is_rw_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_h8300_is_h8300(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_segment(index,input_file);
}

size_t helf_h8300_load_segment_count_get(helf_file_input_t *input_file)
{
    if(!helf_h8300_is_h8300(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_count_get(input_file);
}

bool helf_h8300_load_segment_info_get(helf_file_input_t *input_file,size_t index,helf_h8300_segment_info_t *info)
{
    if(!helf_h8300_is_h8300(input_file))
    {
        return false;
    }
    return helf_arch_load_segment_info_get(input_file,index,info);
}

size_t helf_h8300_load_segment_total_filesz_get(helf_file_input_t *input_file)
{
    if(!helf_h8300_is_h8300(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_filesz_get(input_file);
}

size_t helf_h8300_load_segment_total_memsz_get(helf_file_input_t *input_file)
{
    if(!helf_h8300_is_h8300(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_memsz_get(input_file);
}

size_t helf_h8300_relocation_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_h8300_is_h8300(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_section_count_get(input_file);
}

bool helf_h8300_relocation_section_info_get(helf_file_input_t *input_file,size_t index,helf_arch_relocation_section_info_t *info)
{
    if(!helf_h8300_is_h8300(input_file))
    {
        return false;
    }
    return helf_arch_relocation_section_info_get(input_file,index,info);
}

size_t helf_h8300_relocation_count_get(helf_file_input_t *input_file,size_t section_index)
{
    if(!helf_h8300_is_h8300(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_count_get(input_file,section_index);
}

bool helf_h8300_relocation_get(helf_file_input_t *input_file,size_t section_index,size_t index,helf_arch_relocation_info_t *info)
{
    if(!helf_h8300_is_h8300(input_file))
    {
        return false;
    }
    if(!helf_arch_relocation_get(input_file,section_index,index,info))
    {
        return false;
    }
    return true;
}

bool helf_h8300_relocation_symbol_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_symbol_info_t *info)
{
    if(!helf_h8300_is_h8300(input_file))
    {
        return false;
    }
    return helf_arch_relocation_symbol_info_get(input_file,relocation,info);
}

static const helf_arch_relocation_type_name_t helf_h8300_relocation_type_names[]=
{
    {0,0,"R_H8_NONE"},
    {1,4,"R_H8_DIR32"},
    {2,1,"R_H8_DIR32_28"},
    {3,0,"R_H8_DIR32_24"},
    {4,2,"R_H8_DIR32_16"},
    {6,0,"R_H8_DIR32U"},
    {7,1,"R_H8_DIR32U_28"},
    {8,0,"R_H8_DIR32U_24"},
    {9,0,"R_H8_DIR32U_20"},
    {10,2,"R_H8_DIR32U_16"},
    {11,0,"R_H8_DIR24"},
    {12,0,"R_H8_DIR24_20"},
    {13,2,"R_H8_DIR24_16"},
    {14,0,"R_H8_DIR24U"},
    {15,0,"R_H8_DIR24U_20"},
    {16,2,"R_H8_DIR24U_16"},
    {17,2,"R_H8_DIR16"},
    {18,0,"R_H8_DIR16U"},
    {19,4,"R_H8_DIR16S_32"},
    {20,1,"R_H8_DIR16S_28"},
    {21,0,"R_H8_DIR16S_24"},
    {22,0,"R_H8_DIR16S_20"},
    {23,0,"R_H8_DIR16S"},
    {24,1,"R_H8_DIR8"},
    {25,0,"R_H8_DIR8U"},
    {26,4,"R_H8_DIR8Z_32"},
    {27,1,"R_H8_DIR8Z_28"},
    {28,0,"R_H8_DIR8Z_24"},
    {29,0,"R_H8_DIR8Z_20"},
    {30,2,"R_H8_DIR8Z_16"},
    {31,2,"R_H8_PCREL16"},
    {32,1,"R_H8_PCREL8"},
    {33,0,"R_H8_BPOS"},
    {59,2,"R_H8_DIR16A8"},
    {60,2,"R_H8_DIR16R8"},
    {61,4,"R_H8_DIR24A8"},
    {62,4,"R_H8_DIR24R8"},
    {63,4,"R_H8_DIR32A16"},
    {64,4,"R_H8_DISP32A16"},
    {65,4,"R_H8_ABS32"},
    {127,2,"R_H8_ABS32A16"},
    {128,0,"R_H8_SYM"},
    {129,0,"R_H8_OPneg"},
    {130,0,"R_H8_OPadd"},
    {131,0,"R_H8_OPsub"},
    {132,0,"R_H8_OPmul"},
    {133,0,"R_H8_OPdiv"},
    {134,0,"R_H8_OPshla"},
    {135,0,"R_H8_OPshra"},
    {136,0,"R_H8_OPsctsize"},
    {137,0,"R_H8_OPhword"},
    {138,0,"R_H8_OPlword"},
    {139,0,"R_H8_OPhigh"},
    {140,0,"R_H8_OPlow"},
    {141,0,"R_H8_OPscttop"},
};

const char *helf_h8300_relocation_type_name_get(uint32_t type)
{
    return helf_arch_relocation_type_name_get(helf_h8300_relocation_type_names,sizeof(helf_h8300_relocation_type_names)/sizeof(helf_h8300_relocation_type_names[0]),type);
}

uint8_t helf_h8300_relocation_size_get(uint32_t type)
{
    return helf_arch_relocation_type_size_get(helf_h8300_relocation_type_names,sizeof(helf_h8300_relocation_type_names)/sizeof(helf_h8300_relocation_type_names[0]),type);
}

const char *helf_h8300_section_type_name_get(uint32_t type)
{
    (void)type;
    return NULL;
}

bool helf_h8300_relocation_address_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_relocation_address_info_t *info)
{
    if(!helf_h8300_is_h8300(input_file) || relocation==NULL)
    {
        return false;
    }
    uint8_t size=helf_h8300_relocation_size_get(relocation->type);
    if(size==0)
    {
        return false;
    }
    return helf_arch_relocation_address_info_get(input_file,relocation,size,info);
}

size_t helf_h8300_relocation_data_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,void *buffer,size_t buffer_length)
{
    if(!helf_h8300_is_h8300(input_file) || relocation==NULL)
    {
        return 0;
    }
    uint8_t size=helf_h8300_relocation_size_get(relocation->type);
    if(size==0)
    {
        return 0;
    }
    return helf_arch_relocation_data_get(input_file,relocation,size,buffer,buffer_length);
}

bool helf_h8300_is_shared_object(helf_file_input_t *input_file)
{
    return helf_arch_is_shared_object(input_file,helf_h8300_machines,sizeof(helf_h8300_machines)/sizeof(helf_h8300_machines[0]),HELF_ELFCLASS_32);
}

bool helf_h8300_is_relocatable(helf_file_input_t *input_file)
{
    return helf_arch_is_relocatable(input_file,helf_h8300_machines,sizeof(helf_h8300_machines)/sizeof(helf_h8300_machines[0]),HELF_ELFCLASS_32);
}

size_t helf_h8300_dynamic_count_get(helf_file_input_t *input_file)
{
    if(!helf_h8300_is_h8300(input_file))
    {
        return 0;
    }
    return helf_arch_dynamic_count_get(input_file);
}

bool helf_h8300_dynamic_info_get(helf_file_input_t *input_file,size_t index,helf_arch_dynamic_info_t *info)
{
    if(!helf_h8300_is_h8300(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_info_get(input_file,index,info);
}

bool helf_h8300_dynamic_string_get(helf_file_input_t *input_file,uint64_t offset,char *buffer,size_t buffer_length)
{
    if(!helf_h8300_is_h8300(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_string_get(input_file,offset,buffer,buffer_length);
}
