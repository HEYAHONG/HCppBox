/***************************************************************
 * Name:      helf_arch_kvx.c
 * Purpose:   实现helf_arch_kvx接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "helf_arch_kvx.h"

static const uint16_t helf_kvx_machines[]=
{
    HELF_EM_KVX,
};

bool helf_kvx_is_kvx(helf_file_input_t *input_file)
{
    return helf_arch_is(input_file,helf_kvx_machines,sizeof(helf_kvx_machines)/sizeof(helf_kvx_machines[0]),HELF_ELFCLASS_64);
}

bool helf_kvx_is_big_endian(helf_file_input_t *input_file)
{
    if(!helf_kvx_is_kvx(input_file))
    {
        return false;
    }
    return helf_file_input_is_big_endian(input_file);
}

bool helf_kvx_is_executable(helf_file_input_t *input_file)
{
    return helf_arch_is_executable(input_file,helf_kvx_machines,sizeof(helf_kvx_machines)/sizeof(helf_kvx_machines[0]),HELF_ELFCLASS_64);
}

bool helf_kvx_entry_get(helf_file_input_t *input_file,uint64_t *entry)
{
    if(entry==NULL)
    {
        return false;
    }
    if(!helf_kvx_is_kvx(input_file))
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

size_t helf_kvx_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_kvx_is_kvx(input_file))
    {
        return 0;
    }
    return helf_arch_section_count_get(input_file);
}

bool helf_kvx_section_info_get(helf_file_input_t *input_file,size_t index,helf_kvx_section_info_t *info)
{
    if(!helf_kvx_is_kvx(input_file))
    {
        return false;
    }
    return helf_arch_section_info_get(input_file,index,info);
}

bool helf_kvx_is_rx_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_kvx_is_kvx(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_section(index,input_file);
}

intptr_t helf_kvx_rx_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_kvx_is_rx_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rx_section_offset_get(index,input_file);
}

size_t helf_kvx_rx_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_kvx_is_kvx(input_file))
    {
        return 0;
    }
    return helf_arch_rx_section_size_get(input_file);
}

bool helf_kvx_is_ro_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_kvx_is_kvx(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_section(index,input_file);
}

intptr_t helf_kvx_ro_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_kvx_is_ro_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_ro_section_offset_get(index,input_file);
}

size_t helf_kvx_ro_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_kvx_is_kvx(input_file))
    {
        return 0;
    }
    return helf_arch_ro_section_size_get(input_file);
}

bool helf_kvx_is_rw_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_kvx_is_kvx(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_section(index,input_file);
}

intptr_t helf_kvx_rw_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_kvx_is_rw_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rw_section_offset_get(index,input_file);
}

size_t helf_kvx_rw_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_kvx_is_kvx(input_file))
    {
        return 0;
    }
    return helf_arch_rw_section_size_get(input_file);
}

bool helf_kvx_is_bss_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_kvx_is_kvx(input_file))
    {
        return false;
    }
    return helf_arch_is_bss_section(index,input_file);
}

size_t helf_kvx_program_header_count_get(helf_file_input_t *input_file)
{
    if(!helf_kvx_is_kvx(input_file))
    {
        return 0;
    }
    return helf_arch_program_header_count_get(input_file);
}

bool helf_kvx_program_header_get(helf_file_input_t *input_file,size_t index,helf_elf64_program_header_t *programheader)
{
    if(programheader==NULL)
    {
        return false;
    }
    if(!helf_kvx_is_kvx(input_file))
    {
        return false;
    }
    if(index>=helf_arch_program_header_count_get(input_file))
    {
        return false;
    }
    return helf_file_input_64_bits_program_header_get(input_file,index,programheader);
}

size_t helf_kvx_segment_count_get(helf_file_input_t *input_file)
{
    return helf_kvx_program_header_count_get(input_file);
}

bool helf_kvx_segment_info_get(helf_file_input_t *input_file,size_t index,helf_kvx_segment_info_t *info)
{
    if(!helf_kvx_is_kvx(input_file))
    {
        return false;
    }
    return helf_arch_segment_info_get(input_file,index,info);
}

bool helf_kvx_is_load_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_kvx_is_kvx(input_file))
    {
        return false;
    }
    return helf_arch_is_load_segment(index,input_file);
}

bool helf_kvx_is_rx_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_kvx_is_kvx(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_segment(index,input_file);
}

bool helf_kvx_is_ro_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_kvx_is_kvx(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_segment(index,input_file);
}

bool helf_kvx_is_rw_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_kvx_is_kvx(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_segment(index,input_file);
}

size_t helf_kvx_load_segment_count_get(helf_file_input_t *input_file)
{
    if(!helf_kvx_is_kvx(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_count_get(input_file);
}

bool helf_kvx_load_segment_info_get(helf_file_input_t *input_file,size_t index,helf_kvx_segment_info_t *info)
{
    if(!helf_kvx_is_kvx(input_file))
    {
        return false;
    }
    return helf_arch_load_segment_info_get(input_file,index,info);
}

size_t helf_kvx_load_segment_total_filesz_get(helf_file_input_t *input_file)
{
    if(!helf_kvx_is_kvx(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_filesz_get(input_file);
}

size_t helf_kvx_load_segment_total_memsz_get(helf_file_input_t *input_file)
{
    if(!helf_kvx_is_kvx(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_memsz_get(input_file);
}

size_t helf_kvx_relocation_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_kvx_is_kvx(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_section_count_get(input_file);
}

bool helf_kvx_relocation_section_info_get(helf_file_input_t *input_file,size_t index,helf_arch_relocation_section_info_t *info)
{
    if(!helf_kvx_is_kvx(input_file))
    {
        return false;
    }
    return helf_arch_relocation_section_info_get(input_file,index,info);
}

size_t helf_kvx_relocation_count_get(helf_file_input_t *input_file,size_t section_index)
{
    if(!helf_kvx_is_kvx(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_count_get(input_file,section_index);
}

bool helf_kvx_relocation_get(helf_file_input_t *input_file,size_t section_index,size_t index,helf_arch_relocation_info_t *info)
{
    if(!helf_kvx_is_kvx(input_file))
    {
        return false;
    }
    if(!helf_arch_relocation_get(input_file,section_index,index,info))
    {
        return false;
    }
    return true;
}

bool helf_kvx_relocation_symbol_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_symbol_info_t *info)
{
    if(!helf_kvx_is_kvx(input_file))
    {
        return false;
    }
    return helf_arch_relocation_symbol_info_get(input_file,relocation,info);
}

static const helf_arch_relocation_type_name_t helf_kvx_relocation_type_names[]=
{
    {0,0,"R_KVX_NONE"},
    {1,2,"R_KVX_16"},
    {2,4,"R_KVX_32"},
    {3,8,"R_KVX_64"},
    {4,0,"R_KVX_S16_PCREL"},
    {5,0,"R_KVX_PCREL17"},
    {6,0,"R_KVX_PCREL27"},
    {7,0,"R_KVX_32_PCREL"},
    {8,0,"R_KVX_S37_PCREL_LO10"},
    {9,0,"R_KVX_S37_PCREL_UP27"},
    {10,0,"R_KVX_S43_PCREL_LO10"},
    {11,0,"R_KVX_S43_PCREL_UP27"},
    {12,0,"R_KVX_S43_PCREL_EX6"},
    {13,0,"R_KVX_S64_PCREL_LO10"},
    {14,0,"R_KVX_S64_PCREL_UP27"},
    {15,0,"R_KVX_S64_PCREL_EX27"},
    {16,0,"R_KVX_64_PCREL"},
    {17,2,"R_KVX_S16"},
    {18,0,"R_KVX_S32_LO5"},
    {19,0,"R_KVX_S32_UP27"},
    {20,0,"R_KVX_S37_LO10"},
    {21,0,"R_KVX_S37_UP27"},
    {22,0,"R_KVX_S37_GOTOFF_LO10"},
    {23,0,"R_KVX_S37_GOTOFF_UP27"},
    {24,0,"R_KVX_S43_GOTOFF_LO10"},
    {25,0,"R_KVX_S43_GOTOFF_UP27"},
    {26,0,"R_KVX_S43_GOTOFF_EX6"},
    {27,0,"R_KVX_32_GOTOFF"},
    {28,0,"R_KVX_64_GOTOFF"},
    {29,0,"R_KVX_32_GOT"},
    {30,0,"R_KVX_S37_GOT_LO10"},
    {31,0,"R_KVX_S37_GOT_UP27"},
    {32,0,"R_KVX_S43_GOT_LO10"},
    {33,0,"R_KVX_S43_GOT_UP27"},
    {34,0,"R_KVX_S43_GOT_EX6"},
    {35,0,"R_KVX_64_GOT"},
    {36,0,"R_KVX_GLOB_DAT"},
    {37,0,"R_KVX_COPY"},
    {38,0,"R_KVX_JMP_SLOT"},
    {39,0,"R_KVX_RELATIVE"},
    {40,0,"R_KVX_S43_LO10"},
    {41,0,"R_KVX_S43_UP27"},
    {42,0,"R_KVX_S43_EX6"},
    {43,0,"R_KVX_S64_LO10"},
    {44,0,"R_KVX_S64_UP27"},
    {45,0,"R_KVX_S64_EX27"},
    {46,0,"R_KVX_S37_GOTADDR_LO10"},
    {47,0,"R_KVX_S37_GOTADDR_UP27"},
    {48,0,"R_KVX_S43_GOTADDR_LO10"},
    {49,0,"R_KVX_S43_GOTADDR_UP27"},
    {50,0,"R_KVX_S43_GOTADDR_EX6"},
    {51,0,"R_KVX_S64_GOTADDR_LO10"},
    {52,0,"R_KVX_S64_GOTADDR_UP27"},
    {53,0,"R_KVX_S64_GOTADDR_EX27"},
    {54,0,"R_KVX_64_DTPMOD"},
    {55,0,"R_KVX_64_DTPOFF"},
    {56,0,"R_KVX_S37_TLS_DTPOFF_LO10"},
    {57,0,"R_KVX_S37_TLS_DTPOFF_UP27"},
    {58,0,"R_KVX_S43_TLS_DTPOFF_LO10"},
    {59,0,"R_KVX_S43_TLS_DTPOFF_UP27"},
    {60,0,"R_KVX_S43_TLS_DTPOFF_EX6"},
    {61,0,"R_KVX_S37_TLS_GD_LO10"},
    {62,0,"R_KVX_S37_TLS_GD_UP27"},
    {63,0,"R_KVX_S43_TLS_GD_LO10"},
    {64,0,"R_KVX_S43_TLS_GD_UP27"},
    {65,0,"R_KVX_S43_TLS_GD_EX6"},
    {66,0,"R_KVX_S37_TLS_LD_LO10"},
    {67,0,"R_KVX_S37_TLS_LD_UP27"},
    {68,0,"R_KVX_S43_TLS_LD_LO10"},
    {69,0,"R_KVX_S43_TLS_LD_UP27"},
    {70,0,"R_KVX_S43_TLS_LD_EX6"},
    {71,0,"R_KVX_64_TPOFF"},
    {72,0,"R_KVX_S37_TLS_IE_LO10"},
    {73,0,"R_KVX_S37_TLS_IE_UP27"},
    {74,0,"R_KVX_S43_TLS_IE_LO10"},
    {75,0,"R_KVX_S43_TLS_IE_UP27"},
    {76,0,"R_KVX_S43_TLS_IE_EX6"},
    {77,0,"R_KVX_S37_TLS_LE_LO10"},
    {78,0,"R_KVX_S37_TLS_LE_UP27"},
    {79,0,"R_KVX_S43_TLS_LE_LO10"},
    {80,0,"R_KVX_S43_TLS_LE_UP27"},
    {81,0,"R_KVX_S43_TLS_LE_EX6"},
    {82,1,"R_KVX_8"},
};

const char *helf_kvx_relocation_type_name_get(uint32_t type)
{
    return helf_arch_relocation_type_name_get(helf_kvx_relocation_type_names,sizeof(helf_kvx_relocation_type_names)/sizeof(helf_kvx_relocation_type_names[0]),type);
}

uint8_t helf_kvx_relocation_size_get(uint32_t type)
{
    return helf_arch_relocation_type_size_get(helf_kvx_relocation_type_names,sizeof(helf_kvx_relocation_type_names)/sizeof(helf_kvx_relocation_type_names[0]),type);
}

const char *helf_kvx_section_type_name_get(uint32_t type)
{
    (void)type;
    return NULL;
}

bool helf_kvx_relocation_address_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_relocation_address_info_t *info)
{
    if(!helf_kvx_is_kvx(input_file) || relocation==NULL)
    {
        return false;
    }
    uint8_t size=helf_kvx_relocation_size_get(relocation->type);
    if(size==0)
    {
        return false;
    }
    return helf_arch_relocation_address_info_get(input_file,relocation,size,info);
}

size_t helf_kvx_relocation_data_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,void *buffer,size_t buffer_length)
{
    if(!helf_kvx_is_kvx(input_file) || relocation==NULL)
    {
        return 0;
    }
    uint8_t size=helf_kvx_relocation_size_get(relocation->type);
    if(size==0)
    {
        return 0;
    }
    return helf_arch_relocation_data_get(input_file,relocation,size,buffer,buffer_length);
}

bool helf_kvx_is_shared_object(helf_file_input_t *input_file)
{
    return helf_arch_is_shared_object(input_file,helf_kvx_machines,sizeof(helf_kvx_machines)/sizeof(helf_kvx_machines[0]),HELF_ELFCLASS_64);
}

bool helf_kvx_is_relocatable(helf_file_input_t *input_file)
{
    return helf_arch_is_relocatable(input_file,helf_kvx_machines,sizeof(helf_kvx_machines)/sizeof(helf_kvx_machines[0]),HELF_ELFCLASS_64);
}

size_t helf_kvx_dynamic_count_get(helf_file_input_t *input_file)
{
    if(!helf_kvx_is_kvx(input_file))
    {
        return 0;
    }
    return helf_arch_dynamic_count_get(input_file);
}

bool helf_kvx_dynamic_info_get(helf_file_input_t *input_file,size_t index,helf_arch_dynamic_info_t *info)
{
    if(!helf_kvx_is_kvx(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_info_get(input_file,index,info);
}

bool helf_kvx_dynamic_string_get(helf_file_input_t *input_file,uint64_t offset,char *buffer,size_t buffer_length)
{
    if(!helf_kvx_is_kvx(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_string_get(input_file,offset,buffer,buffer_length);
}
