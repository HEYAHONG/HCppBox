/***************************************************************
 * Name:      helf_arch_tilepro.c
 * Purpose:   实现helf_arch_tilepro接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "helf_arch_tilepro.h"

static const uint16_t helf_tilepro_machines[]=
{
    HELF_EM_TILEPRO,
};

bool helf_tilepro_is_tilepro(helf_file_input_t *input_file)
{
    return helf_arch_is(input_file,helf_tilepro_machines,sizeof(helf_tilepro_machines)/sizeof(helf_tilepro_machines[0]),HELF_ELFCLASS_32);
}

bool helf_tilepro_is_big_endian(helf_file_input_t *input_file)
{
    if(!helf_tilepro_is_tilepro(input_file))
    {
        return false;
    }
    return helf_file_input_is_big_endian(input_file);
}

bool helf_tilepro_is_executable(helf_file_input_t *input_file)
{
    return helf_arch_is_executable(input_file,helf_tilepro_machines,sizeof(helf_tilepro_machines)/sizeof(helf_tilepro_machines[0]),HELF_ELFCLASS_32);
}

bool helf_tilepro_entry_get(helf_file_input_t *input_file,uint32_t *entry)
{
    if(entry==NULL)
    {
        return false;
    }
    if(!helf_tilepro_is_tilepro(input_file))
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

size_t helf_tilepro_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_tilepro_is_tilepro(input_file))
    {
        return 0;
    }
    return helf_arch_section_count_get(input_file);
}

bool helf_tilepro_section_info_get(helf_file_input_t *input_file,size_t index,helf_tilepro_section_info_t *info)
{
    if(!helf_tilepro_is_tilepro(input_file))
    {
        return false;
    }
    return helf_arch_section_info_get(input_file,index,info);
}

bool helf_tilepro_is_rx_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tilepro_is_tilepro(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_section(index,input_file);
}

intptr_t helf_tilepro_rx_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tilepro_is_rx_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rx_section_offset_get(index,input_file);
}

size_t helf_tilepro_rx_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_tilepro_is_tilepro(input_file))
    {
        return 0;
    }
    return helf_arch_rx_section_size_get(input_file);
}

bool helf_tilepro_is_ro_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tilepro_is_tilepro(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_section(index,input_file);
}

intptr_t helf_tilepro_ro_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tilepro_is_ro_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_ro_section_offset_get(index,input_file);
}

size_t helf_tilepro_ro_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_tilepro_is_tilepro(input_file))
    {
        return 0;
    }
    return helf_arch_ro_section_size_get(input_file);
}

bool helf_tilepro_is_rw_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tilepro_is_tilepro(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_section(index,input_file);
}

intptr_t helf_tilepro_rw_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tilepro_is_rw_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rw_section_offset_get(index,input_file);
}

size_t helf_tilepro_rw_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_tilepro_is_tilepro(input_file))
    {
        return 0;
    }
    return helf_arch_rw_section_size_get(input_file);
}

bool helf_tilepro_is_bss_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tilepro_is_tilepro(input_file))
    {
        return false;
    }
    return helf_arch_is_bss_section(index,input_file);
}

size_t helf_tilepro_program_header_count_get(helf_file_input_t *input_file)
{
    if(!helf_tilepro_is_tilepro(input_file))
    {
        return 0;
    }
    return helf_arch_program_header_count_get(input_file);
}

bool helf_tilepro_program_header_get(helf_file_input_t *input_file,size_t index,helf_elf32_program_header_t *programheader)
{
    if(programheader==NULL)
    {
        return false;
    }
    if(!helf_tilepro_is_tilepro(input_file))
    {
        return false;
    }
    if(index>=helf_arch_program_header_count_get(input_file))
    {
        return false;
    }
    return helf_file_input_32_bits_program_header_get(input_file,index,programheader);
}

size_t helf_tilepro_segment_count_get(helf_file_input_t *input_file)
{
    return helf_tilepro_program_header_count_get(input_file);
}

bool helf_tilepro_segment_info_get(helf_file_input_t *input_file,size_t index,helf_tilepro_segment_info_t *info)
{
    if(!helf_tilepro_is_tilepro(input_file))
    {
        return false;
    }
    return helf_arch_segment_info_get(input_file,index,info);
}

bool helf_tilepro_is_load_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tilepro_is_tilepro(input_file))
    {
        return false;
    }
    return helf_arch_is_load_segment(index,input_file);
}

bool helf_tilepro_is_rx_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tilepro_is_tilepro(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_segment(index,input_file);
}

bool helf_tilepro_is_ro_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tilepro_is_tilepro(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_segment(index,input_file);
}

bool helf_tilepro_is_rw_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_tilepro_is_tilepro(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_segment(index,input_file);
}

size_t helf_tilepro_load_segment_count_get(helf_file_input_t *input_file)
{
    if(!helf_tilepro_is_tilepro(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_count_get(input_file);
}

bool helf_tilepro_load_segment_info_get(helf_file_input_t *input_file,size_t index,helf_tilepro_segment_info_t *info)
{
    if(!helf_tilepro_is_tilepro(input_file))
    {
        return false;
    }
    return helf_arch_load_segment_info_get(input_file,index,info);
}

size_t helf_tilepro_load_segment_total_filesz_get(helf_file_input_t *input_file)
{
    if(!helf_tilepro_is_tilepro(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_filesz_get(input_file);
}

size_t helf_tilepro_load_segment_total_memsz_get(helf_file_input_t *input_file)
{
    if(!helf_tilepro_is_tilepro(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_memsz_get(input_file);
}

size_t helf_tilepro_relocation_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_tilepro_is_tilepro(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_section_count_get(input_file);
}

bool helf_tilepro_relocation_section_info_get(helf_file_input_t *input_file,size_t index,helf_arch_relocation_section_info_t *info)
{
    if(!helf_tilepro_is_tilepro(input_file))
    {
        return false;
    }
    return helf_arch_relocation_section_info_get(input_file,index,info);
}

size_t helf_tilepro_relocation_count_get(helf_file_input_t *input_file,size_t section_index)
{
    if(!helf_tilepro_is_tilepro(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_count_get(input_file,section_index);
}

bool helf_tilepro_relocation_get(helf_file_input_t *input_file,size_t section_index,size_t index,helf_arch_relocation_info_t *info)
{
    if(!helf_tilepro_is_tilepro(input_file))
    {
        return false;
    }
    if(!helf_arch_relocation_get(input_file,section_index,index,info))
    {
        return false;
    }
    return true;
}

bool helf_tilepro_relocation_symbol_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_symbol_info_t *info)
{
    if(!helf_tilepro_is_tilepro(input_file))
    {
        return false;
    }
    return helf_arch_relocation_symbol_info_get(input_file,relocation,info);
}

static const helf_arch_relocation_type_name_t helf_tilepro_relocation_type_names[]=
{
    {0,0,"R_TILEPRO_NONE"},
    {1,4,"R_TILEPRO_32"},
    {2,2,"R_TILEPRO_16"},
    {3,1,"R_TILEPRO_8"},
    {4,4,"R_TILEPRO_32_PCREL"},
    {5,2,"R_TILEPRO_16_PCREL"},
    {6,1,"R_TILEPRO_8_PCREL"},
    {7,2,"R_TILEPRO_LO16"},
    {8,2,"R_TILEPRO_HI16"},
    {9,2,"R_TILEPRO_HA16"},
    {10,0,"R_TILEPRO_COPY"},
    {11,4,"R_TILEPRO_GLOB_DAT"},
    {12,4,"R_TILEPRO_JMP_SLOT"},
    {13,4,"R_TILEPRO_RELATIVE"},
    {14,4,"R_TILEPRO_BROFF_X1"},
    {15,4,"R_TILEPRO_JOFFLONG_X1"},
    {16,4,"R_TILEPRO_JOFFLONG_X1_PLT"},
    {17,0,"R_TILEPRO_IMM8_X0"},
    {18,0,"R_TILEPRO_IMM8_Y0"},
    {19,0,"R_TILEPRO_IMM8_X1"},
    {20,0,"R_TILEPRO_IMM8_Y1"},
    {21,0,"R_TILEPRO_MT_IMM15_X1"},
    {22,0,"R_TILEPRO_MF_IMM15_X1"},
    {23,0,"R_TILEPRO_IMM16_X0"},
    {24,0,"R_TILEPRO_IMM16_X1"},
    {25,0,"R_TILEPRO_IMM16_X0_LO"},
    {26,0,"R_TILEPRO_IMM16_X1_LO"},
    {27,0,"R_TILEPRO_IMM16_X0_HI"},
    {28,0,"R_TILEPRO_IMM16_X1_HI"},
    {29,0,"R_TILEPRO_IMM16_X0_HA"},
    {30,0,"R_TILEPRO_IMM16_X1_HA"},
    {31,2,"R_TILEPRO_IMM16_X0_PCREL"},
    {32,2,"R_TILEPRO_IMM16_X1_PCREL"},
    {33,0,"R_TILEPRO_IMM16_X0_LO_PCREL"},
    {34,0,"R_TILEPRO_IMM16_X1_LO_PCREL"},
    {35,0,"R_TILEPRO_IMM16_X0_HI_PCREL"},
    {36,0,"R_TILEPRO_IMM16_X1_HI_PCREL"},
    {37,0,"R_TILEPRO_IMM16_X0_HA_PCREL"},
    {38,0,"R_TILEPRO_IMM16_X1_HA_PCREL"},
    {39,0,"R_TILEPRO_IMM16_X0_GOT"},
    {40,0,"R_TILEPRO_IMM16_X1_GOT"},
    {41,0,"R_TILEPRO_IMM16_X0_GOT_LO"},
    {42,0,"R_TILEPRO_IMM16_X1_GOT_LO"},
    {43,0,"R_TILEPRO_IMM16_X0_GOT_HI"},
    {44,0,"R_TILEPRO_IMM16_X1_GOT_HI"},
    {45,0,"R_TILEPRO_IMM16_X0_GOT_HA"},
    {46,0,"R_TILEPRO_IMM16_X1_GOT_HA"},
    {47,0,"R_TILEPRO_MMSTART_X0"},
    {48,0,"R_TILEPRO_MMEND_X0"},
    {49,0,"R_TILEPRO_MMSTART_X1"},
    {50,0,"R_TILEPRO_MMEND_X1"},
    {51,0,"R_TILEPRO_SHAMT_X0"},
    {52,0,"R_TILEPRO_SHAMT_X1"},
    {53,0,"R_TILEPRO_SHAMT_Y0"},
    {54,0,"R_TILEPRO_SHAMT_Y1"},
    {55,0,"R_TILEPRO_DEST_IMM8_X1"},
    {60,4,"R_TILEPRO_TLS_GD_CALL"},
    {61,0,"R_TILEPRO_IMM8_X0_TLS_GD_ADD"},
    {62,0,"R_TILEPRO_IMM8_X1_TLS_GD_ADD"},
    {63,0,"R_TILEPRO_IMM8_Y0_TLS_GD_ADD"},
    {64,0,"R_TILEPRO_IMM8_Y1_TLS_GD_ADD"},
    {65,0,"R_TILEPRO_TLS_IE_LOAD"},
    {66,2,"R_TILEPRO_IMM16_X0_TLS_GD"},
    {67,2,"R_TILEPRO_IMM16_X1_TLS_GD"},
    {68,0,"R_TILEPRO_IMM16_X0_TLS_GD_LO"},
    {69,0,"R_TILEPRO_IMM16_X1_TLS_GD_LO"},
    {70,0,"R_TILEPRO_IMM16_X0_TLS_GD_HI"},
    {71,0,"R_TILEPRO_IMM16_X1_TLS_GD_HI"},
    {72,0,"R_TILEPRO_IMM16_X0_TLS_GD_HA"},
    {73,0,"R_TILEPRO_IMM16_X1_TLS_GD_HA"},
    {74,2,"R_TILEPRO_IMM16_X0_TLS_IE"},
    {75,2,"R_TILEPRO_IMM16_X1_TLS_IE"},
    {76,0,"R_TILEPRO_IMM16_X0_TLS_IE_LO"},
    {77,0,"R_TILEPRO_IMM16_X1_TLS_IE_LO"},
    {78,0,"R_TILEPRO_IMM16_X0_TLS_IE_HI"},
    {79,0,"R_TILEPRO_IMM16_X1_TLS_IE_HI"},
    {80,0,"R_TILEPRO_IMM16_X0_TLS_IE_HA"},
    {81,0,"R_TILEPRO_IMM16_X1_TLS_IE_HA"},
    {82,0,"R_TILEPRO_TLS_DTPMOD32"},
    {83,4,"R_TILEPRO_TLS_DTPOFF32"},
    {84,0,"R_TILEPRO_TLS_TPOFF32"},
    {85,2,"R_TILEPRO_IMM16_X0_TLS_LE"},
    {86,2,"R_TILEPRO_IMM16_X1_TLS_LE"},
    {87,0,"R_TILEPRO_IMM16_X0_TLS_LE_LO"},
    {88,0,"R_TILEPRO_IMM16_X1_TLS_LE_LO"},
    {89,0,"R_TILEPRO_IMM16_X0_TLS_LE_HI"},
    {90,0,"R_TILEPRO_IMM16_X1_TLS_LE_HI"},
    {91,0,"R_TILEPRO_IMM16_X0_TLS_LE_HA"},
    {92,0,"R_TILEPRO_IMM16_X1_TLS_LE_HA"},
    {128,4,"R_TILEPRO_GNU_VTINHERIT"},
    {129,4,"R_TILEPRO_GNU_VTENTRY"},
};

const char *helf_tilepro_relocation_type_name_get(uint32_t type)
{
    return helf_arch_relocation_type_name_get(helf_tilepro_relocation_type_names,sizeof(helf_tilepro_relocation_type_names)/sizeof(helf_tilepro_relocation_type_names[0]),type);
}

uint8_t helf_tilepro_relocation_size_get(uint32_t type)
{
    return helf_arch_relocation_type_size_get(helf_tilepro_relocation_type_names,sizeof(helf_tilepro_relocation_type_names)/sizeof(helf_tilepro_relocation_type_names[0]),type);
}

const char *helf_tilepro_section_type_name_get(uint32_t type)
{
    (void)type;
    return NULL;
}

bool helf_tilepro_relocation_address_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_relocation_address_info_t *info)
{
    if(!helf_tilepro_is_tilepro(input_file) || relocation==NULL)
    {
        return false;
    }
    uint8_t size=helf_tilepro_relocation_size_get(relocation->type);
    if(size==0)
    {
        return false;
    }
    return helf_arch_relocation_address_info_get(input_file,relocation,size,info);
}

size_t helf_tilepro_relocation_data_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,void *buffer,size_t buffer_length)
{
    if(!helf_tilepro_is_tilepro(input_file) || relocation==NULL)
    {
        return 0;
    }
    uint8_t size=helf_tilepro_relocation_size_get(relocation->type);
    if(size==0)
    {
        return 0;
    }
    return helf_arch_relocation_data_get(input_file,relocation,size,buffer,buffer_length);
}

bool helf_tilepro_is_shared_object(helf_file_input_t *input_file)
{
    return helf_arch_is_shared_object(input_file,helf_tilepro_machines,sizeof(helf_tilepro_machines)/sizeof(helf_tilepro_machines[0]),HELF_ELFCLASS_32);
}

bool helf_tilepro_is_relocatable(helf_file_input_t *input_file)
{
    return helf_arch_is_relocatable(input_file,helf_tilepro_machines,sizeof(helf_tilepro_machines)/sizeof(helf_tilepro_machines[0]),HELF_ELFCLASS_32);
}

size_t helf_tilepro_dynamic_count_get(helf_file_input_t *input_file)
{
    if(!helf_tilepro_is_tilepro(input_file))
    {
        return 0;
    }
    return helf_arch_dynamic_count_get(input_file);
}

bool helf_tilepro_dynamic_info_get(helf_file_input_t *input_file,size_t index,helf_arch_dynamic_info_t *info)
{
    if(!helf_tilepro_is_tilepro(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_info_get(input_file,index,info);
}

bool helf_tilepro_dynamic_string_get(helf_file_input_t *input_file,uint64_t offset,char *buffer,size_t buffer_length)
{
    if(!helf_tilepro_is_tilepro(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_string_get(input_file,offset,buffer,buffer_length);
}
