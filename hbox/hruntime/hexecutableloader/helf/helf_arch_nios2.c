/***************************************************************
 * Name:      helf_arch_nios2.c
 * Purpose:   实现helf_arch_nios2接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "helf_arch_nios2.h"

static const uint16_t helf_nios2_machines[]=
{
    HELF_EM_ALTERA_NIOS2,
    HELF_EM_NIOS32,
};

bool helf_nios2_is_nios2(helf_file_input_t *input_file)
{
    return helf_arch_is(input_file,helf_nios2_machines,sizeof(helf_nios2_machines)/sizeof(helf_nios2_machines[0]),HELF_ELFCLASS_32);
}

bool helf_nios2_is_big_endian(helf_file_input_t *input_file)
{
    if(!helf_nios2_is_nios2(input_file))
    {
        return false;
    }
    return helf_file_input_is_big_endian(input_file);
}

bool helf_nios2_is_executable(helf_file_input_t *input_file)
{
    return helf_arch_is_executable(input_file,helf_nios2_machines,sizeof(helf_nios2_machines)/sizeof(helf_nios2_machines[0]),HELF_ELFCLASS_32);
}

bool helf_nios2_entry_get(helf_file_input_t *input_file,uint32_t *entry)
{
    if(entry==NULL)
    {
        return false;
    }
    if(!helf_nios2_is_nios2(input_file))
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

size_t helf_nios2_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_nios2_is_nios2(input_file))
    {
        return 0;
    }
    return helf_arch_section_count_get(input_file);
}

bool helf_nios2_section_info_get(helf_file_input_t *input_file,size_t index,helf_nios2_section_info_t *info)
{
    if(!helf_nios2_is_nios2(input_file))
    {
        return false;
    }
    return helf_arch_section_info_get(input_file,index,info);
}

bool helf_nios2_is_rx_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_nios2_is_nios2(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_section(index,input_file);
}

intptr_t helf_nios2_rx_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_nios2_is_rx_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rx_section_offset_get(index,input_file);
}

size_t helf_nios2_rx_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_nios2_is_nios2(input_file))
    {
        return 0;
    }
    return helf_arch_rx_section_size_get(input_file);
}

bool helf_nios2_is_ro_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_nios2_is_nios2(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_section(index,input_file);
}

intptr_t helf_nios2_ro_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_nios2_is_ro_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_ro_section_offset_get(index,input_file);
}

size_t helf_nios2_ro_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_nios2_is_nios2(input_file))
    {
        return 0;
    }
    return helf_arch_ro_section_size_get(input_file);
}

bool helf_nios2_is_rw_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_nios2_is_nios2(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_section(index,input_file);
}

intptr_t helf_nios2_rw_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_nios2_is_rw_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rw_section_offset_get(index,input_file);
}

size_t helf_nios2_rw_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_nios2_is_nios2(input_file))
    {
        return 0;
    }
    return helf_arch_rw_section_size_get(input_file);
}

bool helf_nios2_is_bss_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_nios2_is_nios2(input_file))
    {
        return false;
    }
    return helf_arch_is_bss_section(index,input_file);
}

size_t helf_nios2_program_header_count_get(helf_file_input_t *input_file)
{
    if(!helf_nios2_is_nios2(input_file))
    {
        return 0;
    }
    return helf_arch_program_header_count_get(input_file);
}

bool helf_nios2_program_header_get(helf_file_input_t *input_file,size_t index,helf_elf32_program_header_t *programheader)
{
    if(programheader==NULL)
    {
        return false;
    }
    if(!helf_nios2_is_nios2(input_file))
    {
        return false;
    }
    if(index>=helf_arch_program_header_count_get(input_file))
    {
        return false;
    }
    return helf_file_input_32_bits_program_header_get(input_file,index,programheader);
}

size_t helf_nios2_segment_count_get(helf_file_input_t *input_file)
{
    return helf_nios2_program_header_count_get(input_file);
}

bool helf_nios2_segment_info_get(helf_file_input_t *input_file,size_t index,helf_nios2_segment_info_t *info)
{
    if(!helf_nios2_is_nios2(input_file))
    {
        return false;
    }
    return helf_arch_segment_info_get(input_file,index,info);
}

bool helf_nios2_is_load_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_nios2_is_nios2(input_file))
    {
        return false;
    }
    return helf_arch_is_load_segment(index,input_file);
}

bool helf_nios2_is_rx_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_nios2_is_nios2(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_segment(index,input_file);
}

bool helf_nios2_is_ro_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_nios2_is_nios2(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_segment(index,input_file);
}

bool helf_nios2_is_rw_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_nios2_is_nios2(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_segment(index,input_file);
}

size_t helf_nios2_load_segment_count_get(helf_file_input_t *input_file)
{
    if(!helf_nios2_is_nios2(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_count_get(input_file);
}

bool helf_nios2_load_segment_info_get(helf_file_input_t *input_file,size_t index,helf_nios2_segment_info_t *info)
{
    if(!helf_nios2_is_nios2(input_file))
    {
        return false;
    }
    return helf_arch_load_segment_info_get(input_file,index,info);
}

size_t helf_nios2_load_segment_total_filesz_get(helf_file_input_t *input_file)
{
    if(!helf_nios2_is_nios2(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_filesz_get(input_file);
}

size_t helf_nios2_load_segment_total_memsz_get(helf_file_input_t *input_file)
{
    if(!helf_nios2_is_nios2(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_memsz_get(input_file);
}

size_t helf_nios2_relocation_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_nios2_is_nios2(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_section_count_get(input_file);
}

bool helf_nios2_relocation_section_info_get(helf_file_input_t *input_file,size_t index,helf_arch_relocation_section_info_t *info)
{
    if(!helf_nios2_is_nios2(input_file))
    {
        return false;
    }
    return helf_arch_relocation_section_info_get(input_file,index,info);
}

size_t helf_nios2_relocation_count_get(helf_file_input_t *input_file,size_t section_index)
{
    if(!helf_nios2_is_nios2(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_count_get(input_file,section_index);
}

bool helf_nios2_relocation_get(helf_file_input_t *input_file,size_t section_index,size_t index,helf_arch_relocation_info_t *info)
{
    if(!helf_nios2_is_nios2(input_file))
    {
        return false;
    }
    if(!helf_arch_relocation_get(input_file,section_index,index,info))
    {
        return false;
    }
    return true;
}

bool helf_nios2_relocation_symbol_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_symbol_info_t *info)
{
    if(!helf_nios2_is_nios2(input_file))
    {
        return false;
    }
    return helf_arch_relocation_symbol_info_get(input_file,relocation,info);
}

static const helf_arch_relocation_type_name_t helf_nios2_relocation_type_names[]=
{
    {0,0,"R_NIOS2_NONE"},
    {1,2,"R_NIOS2_S16"},
    {2,2,"R_NIOS2_U16"},
    {3,2,"R_NIOS2_PCREL16"},
    {4,0,"R_NIOS2_CALL26"},
    {5,0,"R_NIOS2_IMM5"},
    {6,0,"R_NIOS2_CACHE_OPX"},
    {7,0,"R_NIOS2_IMM6"},
    {8,1,"R_NIOS2_IMM8"},
    {9,2,"R_NIOS2_HI16"},
    {10,2,"R_NIOS2_LO16"},
    {11,2,"R_NIOS2_HIADJ16"},
    {12,4,"R_NIOS2_BFD_RELOC_32"},
    {13,2,"R_NIOS2_BFD_RELOC_16"},
    {14,1,"R_NIOS2_BFD_RELOC_8"},
    {15,0,"R_NIOS2_GPREL"},
    {16,0,"R_NIOS2_GNU_VTINHERIT"},
    {17,0,"R_NIOS2_GNU_VTENTRY"},
    {18,0,"R_NIOS2_UJMP"},
    {19,0,"R_NIOS2_CJMP"},
    {20,0,"R_NIOS2_CALLR"},
    {21,0,"R_NIOS2_ALIGN"},
    {22,2,"R_NIOS2_GOT16"},
    {23,2,"R_NIOS2_CALL16"},
    {24,0,"R_NIOS2_GOTOFF_LO"},
    {25,0,"R_NIOS2_GOTOFF_HA"},
    {26,0,"R_NIOS2_PCREL_LO"},
    {27,0,"R_NIOS2_PCREL_HA"},
    {28,2,"R_NIOS2_TLS_GD16"},
    {29,2,"R_NIOS2_TLS_LDM16"},
    {30,2,"R_NIOS2_TLS_LDO16"},
    {31,2,"R_NIOS2_TLS_IE16"},
    {32,2,"R_NIOS2_TLS_LE16"},
    {33,0,"R_NIOS2_TLS_DTPMOD"},
    {34,0,"R_NIOS2_TLS_DTPREL"},
    {35,0,"R_NIOS2_TLS_TPREL"},
    {36,0,"R_NIOS2_COPY"},
    {37,0,"R_NIOS2_GLOB_DAT"},
    {38,0,"R_NIOS2_JUMP_SLOT"},
    {39,0,"R_NIOS2_RELATIVE"},
    {40,0,"R_NIOS2_GOTOFF"},
    {41,0,"R_NIOS2_CALL26_NOAT"},
    {42,0,"R_NIOS2_GOT_LO"},
    {43,0,"R_NIOS2_GOT_HA"},
    {44,0,"R_NIOS2_CALL_LO"},
    {45,0,"R_NIOS2_CALL_HA"},
    {64,0,"R_NIOS2_R2_S12"},
    {65,0,"R_NIOS2_R2_I10_1_PCREL"},
    {66,0,"R_NIOS2_R2_T1I7_1_PCREL"},
    {67,0,"R_NIOS2_R2_T1I7_2"},
    {68,0,"R_NIOS2_R2_T2I4"},
    {69,0,"R_NIOS2_R2_T2I4_1"},
    {70,0,"R_NIOS2_R2_T2I4_2"},
    {71,0,"R_NIOS2_R2_X1I7_2"},
    {72,0,"R_NIOS2_R2_X2L5"},
    {73,0,"R_NIOS2_R2_F1I5_2"},
    {74,0,"R_NIOS2_R2_L5I4X1"},
    {75,0,"R_NIOS2_R2_T1X1I6"},
    {76,0,"R_NIOS2_R2_T1X1I6_2"},
    {77,0,"R_NIOS2_ILLEGAL"},
};

const char *helf_nios2_relocation_type_name_get(uint32_t type)
{
    return helf_arch_relocation_type_name_get(helf_nios2_relocation_type_names,sizeof(helf_nios2_relocation_type_names)/sizeof(helf_nios2_relocation_type_names[0]),type);
}

uint8_t helf_nios2_relocation_size_get(uint32_t type)
{
    return helf_arch_relocation_type_size_get(helf_nios2_relocation_type_names,sizeof(helf_nios2_relocation_type_names)/sizeof(helf_nios2_relocation_type_names[0]),type);
}

const char *helf_nios2_section_type_name_get(uint32_t type)
{
    (void)type;
    return NULL;
}

bool helf_nios2_relocation_address_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_relocation_address_info_t *info)
{
    if(!helf_nios2_is_nios2(input_file) || relocation==NULL)
    {
        return false;
    }
    uint8_t size=helf_nios2_relocation_size_get(relocation->type);
    if(size==0)
    {
        return false;
    }
    return helf_arch_relocation_address_info_get(input_file,relocation,size,info);
}

size_t helf_nios2_relocation_data_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,void *buffer,size_t buffer_length)
{
    if(!helf_nios2_is_nios2(input_file) || relocation==NULL)
    {
        return 0;
    }
    uint8_t size=helf_nios2_relocation_size_get(relocation->type);
    if(size==0)
    {
        return 0;
    }
    return helf_arch_relocation_data_get(input_file,relocation,size,buffer,buffer_length);
}

bool helf_nios2_is_shared_object(helf_file_input_t *input_file)
{
    return helf_arch_is_shared_object(input_file,helf_nios2_machines,sizeof(helf_nios2_machines)/sizeof(helf_nios2_machines[0]),HELF_ELFCLASS_32);
}

bool helf_nios2_is_relocatable(helf_file_input_t *input_file)
{
    return helf_arch_is_relocatable(input_file,helf_nios2_machines,sizeof(helf_nios2_machines)/sizeof(helf_nios2_machines[0]),HELF_ELFCLASS_32);
}

size_t helf_nios2_dynamic_count_get(helf_file_input_t *input_file)
{
    if(!helf_nios2_is_nios2(input_file))
    {
        return 0;
    }
    return helf_arch_dynamic_count_get(input_file);
}

bool helf_nios2_dynamic_info_get(helf_file_input_t *input_file,size_t index,helf_arch_dynamic_info_t *info)
{
    if(!helf_nios2_is_nios2(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_info_get(input_file,index,info);
}

bool helf_nios2_dynamic_string_get(helf_file_input_t *input_file,uint64_t offset,char *buffer,size_t buffer_length)
{
    if(!helf_nios2_is_nios2(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_string_get(input_file,offset,buffer,buffer_length);
}
