/***************************************************************
 * Name:      helf_arch_s390.c
 * Purpose:   实现helf_arch_s390接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "helf_arch_s390.h"

static const uint16_t helf_s390_machines[]=
{
    HELF_EM_S390,
    HELF_EM_S390_OLD,
};

bool helf_s390_is_s390(helf_file_input_t *input_file)
{
    return helf_arch_is(input_file,helf_s390_machines,sizeof(helf_s390_machines)/sizeof(helf_s390_machines[0]),HELF_ELFCLASS_32);
}

bool helf_s390_is_big_endian(helf_file_input_t *input_file)
{
    if(!helf_s390_is_s390(input_file))
    {
        return false;
    }
    return helf_file_input_is_big_endian(input_file);
}

bool helf_s390_is_executable(helf_file_input_t *input_file)
{
    return helf_arch_is_executable(input_file,helf_s390_machines,sizeof(helf_s390_machines)/sizeof(helf_s390_machines[0]),HELF_ELFCLASS_32);
}

bool helf_s390_entry_get(helf_file_input_t *input_file,uint32_t *entry)
{
    if(entry==NULL)
    {
        return false;
    }
    if(!helf_s390_is_s390(input_file))
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

size_t helf_s390_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_s390_is_s390(input_file))
    {
        return 0;
    }
    return helf_arch_section_count_get(input_file);
}

bool helf_s390_section_info_get(helf_file_input_t *input_file,size_t index,helf_s390_section_info_t *info)
{
    if(!helf_s390_is_s390(input_file))
    {
        return false;
    }
    return helf_arch_section_info_get(input_file,index,info);
}

bool helf_s390_is_rx_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_s390_is_s390(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_section(index,input_file);
}

intptr_t helf_s390_rx_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_s390_is_rx_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rx_section_offset_get(index,input_file);
}

size_t helf_s390_rx_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_s390_is_s390(input_file))
    {
        return 0;
    }
    return helf_arch_rx_section_size_get(input_file);
}

bool helf_s390_is_ro_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_s390_is_s390(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_section(index,input_file);
}

intptr_t helf_s390_ro_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_s390_is_ro_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_ro_section_offset_get(index,input_file);
}

size_t helf_s390_ro_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_s390_is_s390(input_file))
    {
        return 0;
    }
    return helf_arch_ro_section_size_get(input_file);
}

bool helf_s390_is_rw_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_s390_is_s390(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_section(index,input_file);
}

intptr_t helf_s390_rw_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_s390_is_rw_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rw_section_offset_get(index,input_file);
}

size_t helf_s390_rw_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_s390_is_s390(input_file))
    {
        return 0;
    }
    return helf_arch_rw_section_size_get(input_file);
}

bool helf_s390_is_bss_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_s390_is_s390(input_file))
    {
        return false;
    }
    return helf_arch_is_bss_section(index,input_file);
}

size_t helf_s390_program_header_count_get(helf_file_input_t *input_file)
{
    if(!helf_s390_is_s390(input_file))
    {
        return 0;
    }
    return helf_arch_program_header_count_get(input_file);
}

bool helf_s390_program_header_get(helf_file_input_t *input_file,size_t index,helf_elf32_program_header_t *programheader)
{
    if(programheader==NULL)
    {
        return false;
    }
    if(!helf_s390_is_s390(input_file))
    {
        return false;
    }
    if(index>=helf_arch_program_header_count_get(input_file))
    {
        return false;
    }
    return helf_file_input_32_bits_program_header_get(input_file,index,programheader);
}

size_t helf_s390_segment_count_get(helf_file_input_t *input_file)
{
    return helf_s390_program_header_count_get(input_file);
}

bool helf_s390_segment_info_get(helf_file_input_t *input_file,size_t index,helf_s390_segment_info_t *info)
{
    if(!helf_s390_is_s390(input_file))
    {
        return false;
    }
    return helf_arch_segment_info_get(input_file,index,info);
}

bool helf_s390_is_load_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_s390_is_s390(input_file))
    {
        return false;
    }
    return helf_arch_is_load_segment(index,input_file);
}

bool helf_s390_is_rx_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_s390_is_s390(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_segment(index,input_file);
}

bool helf_s390_is_ro_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_s390_is_s390(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_segment(index,input_file);
}

bool helf_s390_is_rw_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_s390_is_s390(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_segment(index,input_file);
}

size_t helf_s390_load_segment_count_get(helf_file_input_t *input_file)
{
    if(!helf_s390_is_s390(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_count_get(input_file);
}

bool helf_s390_load_segment_info_get(helf_file_input_t *input_file,size_t index,helf_s390_segment_info_t *info)
{
    if(!helf_s390_is_s390(input_file))
    {
        return false;
    }
    return helf_arch_load_segment_info_get(input_file,index,info);
}

size_t helf_s390_load_segment_total_filesz_get(helf_file_input_t *input_file)
{
    if(!helf_s390_is_s390(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_filesz_get(input_file);
}

size_t helf_s390_load_segment_total_memsz_get(helf_file_input_t *input_file)
{
    if(!helf_s390_is_s390(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_memsz_get(input_file);
}

size_t helf_s390_relocation_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_s390_is_s390(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_section_count_get(input_file);
}

bool helf_s390_relocation_section_info_get(helf_file_input_t *input_file,size_t index,helf_arch_relocation_section_info_t *info)
{
    if(!helf_s390_is_s390(input_file))
    {
        return false;
    }
    return helf_arch_relocation_section_info_get(input_file,index,info);
}

size_t helf_s390_relocation_count_get(helf_file_input_t *input_file,size_t section_index)
{
    if(!helf_s390_is_s390(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_count_get(input_file,section_index);
}

bool helf_s390_relocation_get(helf_file_input_t *input_file,size_t section_index,size_t index,helf_arch_relocation_info_t *info)
{
    if(!helf_s390_is_s390(input_file))
    {
        return false;
    }
    if(!helf_arch_relocation_get(input_file,section_index,index,info))
    {
        return false;
    }
    return true;
}

bool helf_s390_relocation_symbol_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_symbol_info_t *info)
{
    if(!helf_s390_is_s390(input_file))
    {
        return false;
    }
    return helf_arch_relocation_symbol_info_get(input_file,relocation,info);
}

static const helf_arch_relocation_type_name_t helf_s390_relocation_type_names[]=
{
    {0,0,"R_390_NONE"},
    {1,1,"R_390_8"},
    {2,2,"R_390_12"},
    {3,2,"R_390_16"},
    {4,4,"R_390_32"},
    {5,4,"R_390_PC32"},
    {6,2,"R_390_GOT12"},
    {7,4,"R_390_GOT32"},
    {8,4,"R_390_PLT32"},
    {9,4,"R_390_COPY"},
    {10,4,"R_390_GLOB_DAT"},
    {11,4,"R_390_JMP_SLOT"},
    {12,4,"R_390_RELATIVE"},
    {13,4,"R_390_GOTOFF32"},
    {14,4,"R_390_GOTPC"},
    {15,2,"R_390_GOT16"},
    {16,2,"R_390_PC16"},
    {62,2,"R_390_PC12DBL"},
    {63,2,"R_390_PLT12DBL"},
    {17,2,"R_390_PC16DBL"},
    {18,2,"R_390_PLT16DBL"},
    {64,4,"R_390_PC24DBL"},
    {65,4,"R_390_PLT24DBL"},
    {19,4,"R_390_PC32DBL"},
    {20,4,"R_390_PLT32DBL"},
    {21,4,"R_390_GOTPCDBL"},
    {22,8,"R_390_64"},
    {23,8,"R_390_PC64"},
    {24,8,"R_390_GOT64"},
    {25,8,"R_390_PLT64"},
    {26,4,"R_390_GOTENT"},
    {27,2,"R_390_GOTOFF16"},
    {28,8,"R_390_GOTOFF64"},
    {29,2,"R_390_GOTPLT12"},
    {30,2,"R_390_GOTPLT16"},
    {31,4,"R_390_GOTPLT32"},
    {32,8,"R_390_GOTPLT64"},
    {33,4,"R_390_GOTPLTENT"},
    {34,2,"R_390_PLTOFF16"},
    {35,4,"R_390_PLTOFF32"},
    {36,8,"R_390_PLTOFF64"},
    {37,0,"R_390_TLS_LOAD"},
    {38,0,"R_390_TLS_GDCALL"},
    {39,0,"R_390_TLS_LDCALL"},
    {40,4,"R_390_TLS_GD32"},
    {41,8,"R_390_TLS_GD64"},
    {42,2,"R_390_TLS_GOTIE12"},
    {43,4,"R_390_TLS_GOTIE32"},
    {44,8,"R_390_TLS_GOTIE64"},
    {45,4,"R_390_TLS_LDM32"},
    {46,8,"R_390_TLS_LDM64"},
    {47,4,"R_390_TLS_IE32"},
    {48,8,"R_390_TLS_IE64"},
    {49,4,"R_390_TLS_IEENT"},
    {50,4,"R_390_TLS_LE32"},
    {51,8,"R_390_TLS_LE64"},
    {52,4,"R_390_TLS_LDO32"},
    {53,8,"R_390_TLS_LDO64"},
    {54,4,"R_390_TLS_DTPMOD"},
    {55,4,"R_390_TLS_DTPOFF"},
    {56,4,"R_390_TLS_TPOFF"},
    {57,4,"R_390_20"},
    {58,4,"R_390_GOT20"},
    {59,4,"R_390_GOTPLT20"},
    {60,4,"R_390_TLS_GOTIE20"},
    {61,4,"R_390_IRELATIVE"},
    {250,4,"R_390_GNU_VTINHERIT"},
    {251,4,"R_390_GNU_VTENTRY"},
};

const char *helf_s390_relocation_type_name_get(uint32_t type)
{
    return helf_arch_relocation_type_name_get(helf_s390_relocation_type_names,sizeof(helf_s390_relocation_type_names)/sizeof(helf_s390_relocation_type_names[0]),type);
}

uint8_t helf_s390_relocation_size_get(uint32_t type)
{
    return helf_arch_relocation_type_size_get(helf_s390_relocation_type_names,sizeof(helf_s390_relocation_type_names)/sizeof(helf_s390_relocation_type_names[0]),type);
}

const char *helf_s390_section_type_name_get(uint32_t type)
{
    (void)type;
    return NULL;
}

bool helf_s390_relocation_address_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_relocation_address_info_t *info)
{
    if(!helf_s390_is_s390(input_file) || relocation==NULL)
    {
        return false;
    }
    uint8_t size=helf_s390_relocation_size_get(relocation->type);
    if(size==0)
    {
        return false;
    }
    return helf_arch_relocation_address_info_get(input_file,relocation,size,info);
}

size_t helf_s390_relocation_data_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,void *buffer,size_t buffer_length)
{
    if(!helf_s390_is_s390(input_file) || relocation==NULL)
    {
        return 0;
    }
    uint8_t size=helf_s390_relocation_size_get(relocation->type);
    if(size==0)
    {
        return 0;
    }
    return helf_arch_relocation_data_get(input_file,relocation,size,buffer,buffer_length);
}

bool helf_s390_is_shared_object(helf_file_input_t *input_file)
{
    return helf_arch_is_shared_object(input_file,helf_s390_machines,sizeof(helf_s390_machines)/sizeof(helf_s390_machines[0]),HELF_ELFCLASS_32);
}

bool helf_s390_is_relocatable(helf_file_input_t *input_file)
{
    return helf_arch_is_relocatable(input_file,helf_s390_machines,sizeof(helf_s390_machines)/sizeof(helf_s390_machines[0]),HELF_ELFCLASS_32);
}

size_t helf_s390_dynamic_count_get(helf_file_input_t *input_file)
{
    if(!helf_s390_is_s390(input_file))
    {
        return 0;
    }
    return helf_arch_dynamic_count_get(input_file);
}

bool helf_s390_dynamic_info_get(helf_file_input_t *input_file,size_t index,helf_arch_dynamic_info_t *info)
{
    if(!helf_s390_is_s390(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_info_get(input_file,index,info);
}

bool helf_s390_dynamic_string_get(helf_file_input_t *input_file,uint64_t offset,char *buffer,size_t buffer_length)
{
    if(!helf_s390_is_s390(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_string_get(input_file,offset,buffer,buffer_length);
}
