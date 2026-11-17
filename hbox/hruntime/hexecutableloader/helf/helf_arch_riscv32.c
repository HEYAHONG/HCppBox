/***************************************************************
 * Name:      helf_arch_riscv32.c
 * Purpose:   实现helf_arch_riscv32接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "helf_arch_riscv32.h"

static const uint16_t helf_riscv32_machines[]=
{
    HELF_EM_RISCV,
};

bool helf_riscv32_is_riscv32(helf_file_input_t *input_file)
{
    return helf_arch_is(input_file,helf_riscv32_machines,sizeof(helf_riscv32_machines)/sizeof(helf_riscv32_machines[0]),HELF_ELFCLASS_32);
}

bool helf_riscv32_is_big_endian(helf_file_input_t *input_file)
{
    if(!helf_riscv32_is_riscv32(input_file))
    {
        return false;
    }
    return helf_file_input_is_big_endian(input_file);
}

bool helf_riscv32_is_executable(helf_file_input_t *input_file)
{
    return helf_arch_is_executable(input_file,helf_riscv32_machines,sizeof(helf_riscv32_machines)/sizeof(helf_riscv32_machines[0]),HELF_ELFCLASS_32);
}

bool helf_riscv32_entry_get(helf_file_input_t *input_file,uint32_t *entry)
{
    if(entry==NULL)
    {
        return false;
    }
    if(!helf_riscv32_is_riscv32(input_file))
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

size_t helf_riscv32_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_riscv32_is_riscv32(input_file))
    {
        return 0;
    }
    return helf_arch_section_count_get(input_file);
}

bool helf_riscv32_section_info_get(helf_file_input_t *input_file,size_t index,helf_riscv32_section_info_t *info)
{
    if(!helf_riscv32_is_riscv32(input_file))
    {
        return false;
    }
    return helf_arch_section_info_get(input_file,index,info);
}

bool helf_riscv32_is_rx_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_riscv32_is_riscv32(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_section(index,input_file);
}

intptr_t helf_riscv32_rx_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_riscv32_is_rx_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rx_section_offset_get(index,input_file);
}

size_t helf_riscv32_rx_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_riscv32_is_riscv32(input_file))
    {
        return 0;
    }
    return helf_arch_rx_section_size_get(input_file);
}

bool helf_riscv32_is_ro_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_riscv32_is_riscv32(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_section(index,input_file);
}

intptr_t helf_riscv32_ro_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_riscv32_is_ro_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_ro_section_offset_get(index,input_file);
}

size_t helf_riscv32_ro_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_riscv32_is_riscv32(input_file))
    {
        return 0;
    }
    return helf_arch_ro_section_size_get(input_file);
}

bool helf_riscv32_is_rw_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_riscv32_is_riscv32(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_section(index,input_file);
}

intptr_t helf_riscv32_rw_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_riscv32_is_rw_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rw_section_offset_get(index,input_file);
}

size_t helf_riscv32_rw_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_riscv32_is_riscv32(input_file))
    {
        return 0;
    }
    return helf_arch_rw_section_size_get(input_file);
}

bool helf_riscv32_is_bss_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_riscv32_is_riscv32(input_file))
    {
        return false;
    }
    return helf_arch_is_bss_section(index,input_file);
}

size_t helf_riscv32_program_header_count_get(helf_file_input_t *input_file)
{
    if(!helf_riscv32_is_riscv32(input_file))
    {
        return 0;
    }
    return helf_arch_program_header_count_get(input_file);
}

bool helf_riscv32_program_header_get(helf_file_input_t *input_file,size_t index,helf_elf32_program_header_t *programheader)
{
    if(programheader==NULL)
    {
        return false;
    }
    if(!helf_riscv32_is_riscv32(input_file))
    {
        return false;
    }
    if(index>=helf_arch_program_header_count_get(input_file))
    {
        return false;
    }
    return helf_file_input_32_bits_program_header_get(input_file,index,programheader);
}

size_t helf_riscv32_segment_count_get(helf_file_input_t *input_file)
{
    return helf_riscv32_program_header_count_get(input_file);
}

bool helf_riscv32_segment_info_get(helf_file_input_t *input_file,size_t index,helf_riscv32_segment_info_t *info)
{
    if(!helf_riscv32_is_riscv32(input_file))
    {
        return false;
    }
    return helf_arch_segment_info_get(input_file,index,info);
}

bool helf_riscv32_is_load_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_riscv32_is_riscv32(input_file))
    {
        return false;
    }
    return helf_arch_is_load_segment(index,input_file);
}

bool helf_riscv32_is_rx_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_riscv32_is_riscv32(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_segment(index,input_file);
}

bool helf_riscv32_is_ro_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_riscv32_is_riscv32(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_segment(index,input_file);
}

bool helf_riscv32_is_rw_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_riscv32_is_riscv32(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_segment(index,input_file);
}

size_t helf_riscv32_load_segment_count_get(helf_file_input_t *input_file)
{
    if(!helf_riscv32_is_riscv32(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_count_get(input_file);
}

bool helf_riscv32_load_segment_info_get(helf_file_input_t *input_file,size_t index,helf_riscv32_segment_info_t *info)
{
    if(!helf_riscv32_is_riscv32(input_file))
    {
        return false;
    }
    return helf_arch_load_segment_info_get(input_file,index,info);
}

size_t helf_riscv32_load_segment_total_filesz_get(helf_file_input_t *input_file)
{
    if(!helf_riscv32_is_riscv32(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_filesz_get(input_file);
}

size_t helf_riscv32_load_segment_total_memsz_get(helf_file_input_t *input_file)
{
    if(!helf_riscv32_is_riscv32(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_memsz_get(input_file);
}

size_t helf_riscv32_relocation_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_riscv32_is_riscv32(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_section_count_get(input_file);
}

bool helf_riscv32_relocation_section_info_get(helf_file_input_t *input_file,size_t index,helf_arch_relocation_section_info_t *info)
{
    if(!helf_riscv32_is_riscv32(input_file))
    {
        return false;
    }
    return helf_arch_relocation_section_info_get(input_file,index,info);
}

size_t helf_riscv32_relocation_count_get(helf_file_input_t *input_file,size_t section_index)
{
    if(!helf_riscv32_is_riscv32(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_count_get(input_file,section_index);
}

bool helf_riscv32_relocation_get(helf_file_input_t *input_file,size_t section_index,size_t index,helf_arch_relocation_info_t *info)
{
    if(!helf_riscv32_is_riscv32(input_file))
    {
        return false;
    }
    if(!helf_arch_relocation_get(input_file,section_index,index,info))
    {
        return false;
    }
    return true;
}

bool helf_riscv32_relocation_symbol_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_symbol_info_t *info)
{
    if(!helf_riscv32_is_riscv32(input_file))
    {
        return false;
    }
    return helf_arch_relocation_symbol_info_get(input_file,relocation,info);
}

static const helf_arch_relocation_type_name_t helf_riscv32_relocation_type_names[]=
{
    {0,0,"R_RISCV_NONE"},
    {1,4,"R_RISCV_32"},
    {2,8,"R_RISCV_64"},
    {3,4,"R_RISCV_RELATIVE"},
    {4,0,"R_RISCV_COPY"},
    {5,8,"R_RISCV_JUMP_SLOT"},
    {6,4,"R_RISCV_TLS_DTPMOD32"},
    {7,8,"R_RISCV_TLS_DTPMOD64"},
    {8,4,"R_RISCV_TLS_DTPREL32"},
    {9,8,"R_RISCV_TLS_DTPREL64"},
    {10,4,"R_RISCV_TLS_TPREL32"},
    {11,8,"R_RISCV_TLS_TPREL64"},
    {12,0,"R_RISCV_TLSDESC"},
    {16,4,"R_RISCV_BRANCH"},
    {17,4,"R_RISCV_JAL"},
    {18,8,"R_RISCV_CALL"},
    {19,8,"R_RISCV_CALL_PLT"},
    {20,4,"R_RISCV_GOT_HI20"},
    {21,4,"R_RISCV_TLS_GOT_HI20"},
    {22,4,"R_RISCV_TLS_GD_HI20"},
    {23,4,"R_RISCV_PCREL_HI20"},
    {24,4,"R_RISCV_PCREL_LO12_I"},
    {25,4,"R_RISCV_PCREL_LO12_S"},
    {26,4,"R_RISCV_HI20"},
    {27,4,"R_RISCV_LO12_I"},
    {28,4,"R_RISCV_LO12_S"},
    {29,4,"R_RISCV_TPREL_HI20"},
    {30,4,"R_RISCV_TPREL_LO12_I"},
    {31,4,"R_RISCV_TPREL_LO12_S"},
    {32,0,"R_RISCV_TPREL_ADD"},
    {33,1,"R_RISCV_ADD8"},
    {34,2,"R_RISCV_ADD16"},
    {35,4,"R_RISCV_ADD32"},
    {36,8,"R_RISCV_ADD64"},
    {37,1,"R_RISCV_SUB8"},
    {38,2,"R_RISCV_SUB16"},
    {39,4,"R_RISCV_SUB32"},
    {40,8,"R_RISCV_SUB64"},
    {43,0,"R_RISCV_ALIGN"},
    {44,2,"R_RISCV_RVC_BRANCH"},
    {45,2,"R_RISCV_RVC_JUMP"},
    {46,2,"R_RISCV_RVC_LUI"},
    {47,4,"R_RISCV_GPREL_I"},
    {48,4,"R_RISCV_GPREL_S"},
    {49,4,"R_RISCV_TPREL_I"},
    {50,4,"R_RISCV_TPREL_S"},
    {51,0,"R_RISCV_RELAX"},
    {52,1,"R_RISCV_SUB6"},
    {53,1,"R_RISCV_SET6"},
    {54,1,"R_RISCV_SET8"},
    {55,2,"R_RISCV_SET16"},
    {56,4,"R_RISCV_SET32"},
    {57,4,"R_RISCV_32_PCREL"},
    {58,4,"R_RISCV_IRELATIVE"},
    {60,0,"R_RISCV_SET_ULEB128"},
    {61,0,"R_RISCV_SUB_ULEB128"},
    {62,4,"R_RISCV_TLSDESC_HI20"},
    {63,4,"R_RISCV_TLSDESC_LOAD_LO12"},
    {64,4,"R_RISCV_TLSDESC_ADD_LO12"},
    {65,0,"R_RISCV_TLSDESC_CALL"},
};

const char *helf_riscv32_relocation_type_name_get(uint32_t type)
{
    return helf_arch_relocation_type_name_get(helf_riscv32_relocation_type_names,sizeof(helf_riscv32_relocation_type_names)/sizeof(helf_riscv32_relocation_type_names[0]),type);
}

uint8_t helf_riscv32_relocation_size_get(uint32_t type)
{
    return helf_arch_relocation_type_size_get(helf_riscv32_relocation_type_names,sizeof(helf_riscv32_relocation_type_names)/sizeof(helf_riscv32_relocation_type_names[0]),type);
}

static const helf_arch_section_type_name_t helf_riscv32_section_type_names[]=
{
    {1879048195,"SHT_RISCV_ATTRIBUTES"},
};

const char *helf_riscv32_section_type_name_get(uint32_t type)
{
    return helf_arch_section_type_name_get(helf_riscv32_section_type_names,sizeof(helf_riscv32_section_type_names)/sizeof(helf_riscv32_section_type_names[0]),type);
}

bool helf_riscv32_relocation_address_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_relocation_address_info_t *info)
{
    if(!helf_riscv32_is_riscv32(input_file) || relocation==NULL)
    {
        return false;
    }
    uint8_t size=helf_riscv32_relocation_size_get(relocation->type);
    if(size==0)
    {
        return false;
    }
    return helf_arch_relocation_address_info_get(input_file,relocation,size,info);
}

size_t helf_riscv32_relocation_data_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,void *buffer,size_t buffer_length)
{
    if(!helf_riscv32_is_riscv32(input_file) || relocation==NULL)
    {
        return 0;
    }
    uint8_t size=helf_riscv32_relocation_size_get(relocation->type);
    if(size==0)
    {
        return 0;
    }
    return helf_arch_relocation_data_get(input_file,relocation,size,buffer,buffer_length);
}

bool helf_riscv32_is_shared_object(helf_file_input_t *input_file)
{
    return helf_arch_is_shared_object(input_file,helf_riscv32_machines,sizeof(helf_riscv32_machines)/sizeof(helf_riscv32_machines[0]),HELF_ELFCLASS_32);
}

bool helf_riscv32_is_relocatable(helf_file_input_t *input_file)
{
    return helf_arch_is_relocatable(input_file,helf_riscv32_machines,sizeof(helf_riscv32_machines)/sizeof(helf_riscv32_machines[0]),HELF_ELFCLASS_32);
}

size_t helf_riscv32_dynamic_count_get(helf_file_input_t *input_file)
{
    if(!helf_riscv32_is_riscv32(input_file))
    {
        return 0;
    }
    return helf_arch_dynamic_count_get(input_file);
}

bool helf_riscv32_dynamic_info_get(helf_file_input_t *input_file,size_t index,helf_arch_dynamic_info_t *info)
{
    if(!helf_riscv32_is_riscv32(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_info_get(input_file,index,info);
}

bool helf_riscv32_dynamic_string_get(helf_file_input_t *input_file,uint64_t offset,char *buffer,size_t buffer_length)
{
    if(!helf_riscv32_is_riscv32(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_string_get(input_file,offset,buffer,buffer_length);
}
