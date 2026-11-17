/***************************************************************
 * Name:      helf_arch_amd64.c
 * Purpose:   实现helf_arch_amd64接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "helf_arch_amd64.h"

static const uint16_t helf_amd64_machines[]=
{
    HELF_EM_X86_64,
    HELF_EM_AMD64,
    HELF_EM_K1OM,
    HELF_EM_L1OM,
};

bool helf_amd64_is_amd64(helf_file_input_t *input_file)
{
    return helf_arch_is(input_file,helf_amd64_machines,sizeof(helf_amd64_machines)/sizeof(helf_amd64_machines[0]),HELF_ELFCLASS_64);
}

bool helf_amd64_is_big_endian(helf_file_input_t *input_file)
{
    if(!helf_amd64_is_amd64(input_file))
    {
        return false;
    }
    return helf_file_input_is_big_endian(input_file);
}

bool helf_amd64_is_executable(helf_file_input_t *input_file)
{
    return helf_arch_is_executable(input_file,helf_amd64_machines,sizeof(helf_amd64_machines)/sizeof(helf_amd64_machines[0]),HELF_ELFCLASS_64);
}

bool helf_amd64_entry_get(helf_file_input_t *input_file,uint64_t *entry)
{
    if(entry==NULL)
    {
        return false;
    }
    if(!helf_amd64_is_amd64(input_file))
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

size_t helf_amd64_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_amd64_is_amd64(input_file))
    {
        return 0;
    }
    return helf_arch_section_count_get(input_file);
}

bool helf_amd64_section_info_get(helf_file_input_t *input_file,size_t index,helf_amd64_section_info_t *info)
{
    if(!helf_amd64_is_amd64(input_file))
    {
        return false;
    }
    return helf_arch_section_info_get(input_file,index,info);
}

bool helf_amd64_is_rx_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_amd64_is_amd64(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_section(index,input_file);
}

intptr_t helf_amd64_rx_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_amd64_is_rx_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rx_section_offset_get(index,input_file);
}

size_t helf_amd64_rx_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_amd64_is_amd64(input_file))
    {
        return 0;
    }
    return helf_arch_rx_section_size_get(input_file);
}

bool helf_amd64_is_ro_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_amd64_is_amd64(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_section(index,input_file);
}

intptr_t helf_amd64_ro_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_amd64_is_ro_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_ro_section_offset_get(index,input_file);
}

size_t helf_amd64_ro_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_amd64_is_amd64(input_file))
    {
        return 0;
    }
    return helf_arch_ro_section_size_get(input_file);
}

bool helf_amd64_is_rw_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_amd64_is_amd64(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_section(index,input_file);
}

intptr_t helf_amd64_rw_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_amd64_is_rw_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rw_section_offset_get(index,input_file);
}

size_t helf_amd64_rw_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_amd64_is_amd64(input_file))
    {
        return 0;
    }
    return helf_arch_rw_section_size_get(input_file);
}

bool helf_amd64_is_bss_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_amd64_is_amd64(input_file))
    {
        return false;
    }
    return helf_arch_is_bss_section(index,input_file);
}

size_t helf_amd64_program_header_count_get(helf_file_input_t *input_file)
{
    if(!helf_amd64_is_amd64(input_file))
    {
        return 0;
    }
    return helf_arch_program_header_count_get(input_file);
}

bool helf_amd64_program_header_get(helf_file_input_t *input_file,size_t index,helf_elf64_program_header_t *programheader)
{
    if(programheader==NULL)
    {
        return false;
    }
    if(!helf_amd64_is_amd64(input_file))
    {
        return false;
    }
    if(index>=helf_arch_program_header_count_get(input_file))
    {
        return false;
    }
    return helf_file_input_64_bits_program_header_get(input_file,index,programheader);
}

size_t helf_amd64_segment_count_get(helf_file_input_t *input_file)
{
    return helf_amd64_program_header_count_get(input_file);
}

bool helf_amd64_segment_info_get(helf_file_input_t *input_file,size_t index,helf_amd64_segment_info_t *info)
{
    if(!helf_amd64_is_amd64(input_file))
    {
        return false;
    }
    return helf_arch_segment_info_get(input_file,index,info);
}

bool helf_amd64_is_load_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_amd64_is_amd64(input_file))
    {
        return false;
    }
    return helf_arch_is_load_segment(index,input_file);
}

bool helf_amd64_is_rx_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_amd64_is_amd64(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_segment(index,input_file);
}

bool helf_amd64_is_ro_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_amd64_is_amd64(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_segment(index,input_file);
}

bool helf_amd64_is_rw_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_amd64_is_amd64(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_segment(index,input_file);
}

size_t helf_amd64_load_segment_count_get(helf_file_input_t *input_file)
{
    if(!helf_amd64_is_amd64(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_count_get(input_file);
}

bool helf_amd64_load_segment_info_get(helf_file_input_t *input_file,size_t index,helf_amd64_segment_info_t *info)
{
    if(!helf_amd64_is_amd64(input_file))
    {
        return false;
    }
    return helf_arch_load_segment_info_get(input_file,index,info);
}

size_t helf_amd64_load_segment_total_filesz_get(helf_file_input_t *input_file)
{
    if(!helf_amd64_is_amd64(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_filesz_get(input_file);
}

size_t helf_amd64_load_segment_total_memsz_get(helf_file_input_t *input_file)
{
    if(!helf_amd64_is_amd64(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_memsz_get(input_file);
}

size_t helf_amd64_relocation_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_amd64_is_amd64(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_section_count_get(input_file);
}

bool helf_amd64_relocation_section_info_get(helf_file_input_t *input_file,size_t index,helf_arch_relocation_section_info_t *info)
{
    if(!helf_amd64_is_amd64(input_file))
    {
        return false;
    }
    return helf_arch_relocation_section_info_get(input_file,index,info);
}

size_t helf_amd64_relocation_count_get(helf_file_input_t *input_file,size_t section_index)
{
    if(!helf_amd64_is_amd64(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_count_get(input_file,section_index);
}

bool helf_amd64_relocation_get(helf_file_input_t *input_file,size_t section_index,size_t index,helf_arch_relocation_info_t *info)
{
    if(!helf_amd64_is_amd64(input_file))
    {
        return false;
    }
    if(!helf_arch_relocation_get(input_file,section_index,index,info))
    {
        return false;
    }
    return true;
}

bool helf_amd64_relocation_symbol_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_symbol_info_t *info)
{
    if(!helf_amd64_is_amd64(input_file))
    {
        return false;
    }
    return helf_arch_relocation_symbol_info_get(input_file,relocation,info);
}

static const helf_arch_relocation_type_name_t helf_amd64_relocation_type_names[]=
{
    {0,0,"R_X86_64_NONE"},
    {1,8,"R_X86_64_64"},
    {2,4,"R_X86_64_PC32"},
    {3,4,"R_X86_64_GOT32"},
    {4,4,"R_X86_64_PLT32"},
    {5,4,"R_X86_64_COPY"},
    {6,8,"R_X86_64_GLOB_DAT"},
    {7,8,"R_X86_64_JUMP_SLOT"},
    {8,8,"R_X86_64_RELATIVE"},
    {9,4,"R_X86_64_GOTPCREL"},
    {10,4,"R_X86_64_32"},
    {11,4,"R_X86_64_32S"},
    {12,2,"R_X86_64_16"},
    {13,2,"R_X86_64_PC16"},
    {14,1,"R_X86_64_8"},
    {15,1,"R_X86_64_PC8"},
    {16,8,"R_X86_64_DTPMOD64"},
    {17,8,"R_X86_64_DTPOFF64"},
    {18,8,"R_X86_64_TPOFF64"},
    {19,4,"R_X86_64_TLSGD"},
    {20,4,"R_X86_64_TLSLD"},
    {21,4,"R_X86_64_DTPOFF32"},
    {22,4,"R_X86_64_GOTTPOFF"},
    {23,4,"R_X86_64_TPOFF32"},
    {24,8,"R_X86_64_PC64"},
    {25,8,"R_X86_64_GOTOFF64"},
    {26,4,"R_X86_64_GOTPC32"},
    {27,8,"R_X86_64_GOT64"},
    {28,8,"R_X86_64_GOTPCREL64"},
    {29,8,"R_X86_64_GOTPC64"},
    {30,8,"R_X86_64_GOTPLT64"},
    {31,8,"R_X86_64_PLTOFF64"},
    {32,4,"R_X86_64_SIZE32"},
    {33,8,"R_X86_64_SIZE64"},
    {34,4,"R_X86_64_GOTPC32_TLSDESC"},
    {35,0,"R_X86_64_TLSDESC_CALL"},
    {36,8,"R_X86_64_TLSDESC"},
    {37,8,"R_X86_64_IRELATIVE"},
    {38,8,"R_X86_64_RELATIVE64"},
    {39,4,"R_X86_64_PC32_BND"},
    {40,4,"R_X86_64_PLT32_BND"},
    {41,4,"R_X86_64_GOTPCRELX"},
    {42,4,"R_X86_64_REX_GOTPCRELX"},
    {43,4,"R_X86_64_CODE_4_GOTPCRELX"},
    {44,4,"R_X86_64_CODE_4_GOTTPOFF"},
    {45,4,"R_X86_64_CODE_4_GOTPC32_TLSDESC"},
    {46,4,"R_X86_64_CODE_5_GOTPCRELX"},
    {47,4,"R_X86_64_CODE_5_GOTTPOFF"},
    {48,4,"R_X86_64_CODE_5_GOTPC32_TLSDESC"},
    {49,4,"R_X86_64_CODE_6_GOTPCRELX"},
    {50,4,"R_X86_64_CODE_6_GOTTPOFF"},
    {51,4,"R_X86_64_CODE_6_GOTPC32_TLSDESC"},
    {250,8,"R_X86_64_GNU_VTINHERIT"},
    {251,8,"R_X86_64_GNU_VTENTRY"},
};

const char *helf_amd64_relocation_type_name_get(uint32_t type)
{
    return helf_arch_relocation_type_name_get(helf_amd64_relocation_type_names,sizeof(helf_amd64_relocation_type_names)/sizeof(helf_amd64_relocation_type_names[0]),type);
}

uint8_t helf_amd64_relocation_size_get(uint32_t type)
{
    return helf_arch_relocation_type_size_get(helf_amd64_relocation_type_names,sizeof(helf_amd64_relocation_type_names)/sizeof(helf_amd64_relocation_type_names[0]),type);
}

static const helf_arch_section_type_name_t helf_amd64_section_type_names[]=
{
    {1879048193,"SHT_X86_64_UNWIND"},
};

const char *helf_amd64_section_type_name_get(uint32_t type)
{
    return helf_arch_section_type_name_get(helf_amd64_section_type_names,sizeof(helf_amd64_section_type_names)/sizeof(helf_amd64_section_type_names[0]),type);
}

bool helf_amd64_relocation_address_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_relocation_address_info_t *info)
{
    if(!helf_amd64_is_amd64(input_file) || relocation==NULL)
    {
        return false;
    }
    uint8_t size=helf_amd64_relocation_size_get(relocation->type);
    if(size==0)
    {
        return false;
    }
    return helf_arch_relocation_address_info_get(input_file,relocation,size,info);
}

size_t helf_amd64_relocation_data_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,void *buffer,size_t buffer_length)
{
    if(!helf_amd64_is_amd64(input_file) || relocation==NULL)
    {
        return 0;
    }
    uint8_t size=helf_amd64_relocation_size_get(relocation->type);
    if(size==0)
    {
        return 0;
    }
    return helf_arch_relocation_data_get(input_file,relocation,size,buffer,buffer_length);
}

bool helf_amd64_is_shared_object(helf_file_input_t *input_file)
{
    return helf_arch_is_shared_object(input_file,helf_amd64_machines,sizeof(helf_amd64_machines)/sizeof(helf_amd64_machines[0]),HELF_ELFCLASS_64);
}

bool helf_amd64_is_relocatable(helf_file_input_t *input_file)
{
    return helf_arch_is_relocatable(input_file,helf_amd64_machines,sizeof(helf_amd64_machines)/sizeof(helf_amd64_machines[0]),HELF_ELFCLASS_64);
}

size_t helf_amd64_dynamic_count_get(helf_file_input_t *input_file)
{
    if(!helf_amd64_is_amd64(input_file))
    {
        return 0;
    }
    return helf_arch_dynamic_count_get(input_file);
}

bool helf_amd64_dynamic_info_get(helf_file_input_t *input_file,size_t index,helf_arch_dynamic_info_t *info)
{
    if(!helf_amd64_is_amd64(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_info_get(input_file,index,info);
}

bool helf_amd64_dynamic_string_get(helf_file_input_t *input_file,uint64_t offset,char *buffer,size_t buffer_length)
{
    if(!helf_amd64_is_amd64(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_string_get(input_file,offset,buffer,buffer_length);
}
