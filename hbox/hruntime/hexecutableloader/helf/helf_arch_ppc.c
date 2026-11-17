/***************************************************************
 * Name:      helf_arch_ppc.c
 * Purpose:   实现helf_arch_ppc接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "helf_arch_ppc.h"

static const uint16_t helf_ppc_machines[]=
{
    HELF_EM_PPC,
    HELF_EM_PPC_OLD,
    HELF_EM_CYGNUS_POWERPC,
};

bool helf_ppc_is_ppc(helf_file_input_t *input_file)
{
    return helf_arch_is(input_file,helf_ppc_machines,sizeof(helf_ppc_machines)/sizeof(helf_ppc_machines[0]),HELF_ELFCLASS_32);
}

bool helf_ppc_is_big_endian(helf_file_input_t *input_file)
{
    if(!helf_ppc_is_ppc(input_file))
    {
        return false;
    }
    return helf_file_input_is_big_endian(input_file);
}

bool helf_ppc_is_executable(helf_file_input_t *input_file)
{
    return helf_arch_is_executable(input_file,helf_ppc_machines,sizeof(helf_ppc_machines)/sizeof(helf_ppc_machines[0]),HELF_ELFCLASS_32);
}

bool helf_ppc_entry_get(helf_file_input_t *input_file,uint32_t *entry)
{
    if(entry==NULL)
    {
        return false;
    }
    if(!helf_ppc_is_ppc(input_file))
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

size_t helf_ppc_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_ppc_is_ppc(input_file))
    {
        return 0;
    }
    return helf_arch_section_count_get(input_file);
}

bool helf_ppc_section_info_get(helf_file_input_t *input_file,size_t index,helf_ppc_section_info_t *info)
{
    if(!helf_ppc_is_ppc(input_file))
    {
        return false;
    }
    return helf_arch_section_info_get(input_file,index,info);
}

bool helf_ppc_is_rx_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ppc_is_ppc(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_section(index,input_file);
}

intptr_t helf_ppc_rx_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ppc_is_rx_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rx_section_offset_get(index,input_file);
}

size_t helf_ppc_rx_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_ppc_is_ppc(input_file))
    {
        return 0;
    }
    return helf_arch_rx_section_size_get(input_file);
}

bool helf_ppc_is_ro_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ppc_is_ppc(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_section(index,input_file);
}

intptr_t helf_ppc_ro_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ppc_is_ro_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_ro_section_offset_get(index,input_file);
}

size_t helf_ppc_ro_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_ppc_is_ppc(input_file))
    {
        return 0;
    }
    return helf_arch_ro_section_size_get(input_file);
}

bool helf_ppc_is_rw_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ppc_is_ppc(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_section(index,input_file);
}

intptr_t helf_ppc_rw_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ppc_is_rw_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rw_section_offset_get(index,input_file);
}

size_t helf_ppc_rw_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_ppc_is_ppc(input_file))
    {
        return 0;
    }
    return helf_arch_rw_section_size_get(input_file);
}

bool helf_ppc_is_bss_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ppc_is_ppc(input_file))
    {
        return false;
    }
    return helf_arch_is_bss_section(index,input_file);
}

size_t helf_ppc_program_header_count_get(helf_file_input_t *input_file)
{
    if(!helf_ppc_is_ppc(input_file))
    {
        return 0;
    }
    return helf_arch_program_header_count_get(input_file);
}

bool helf_ppc_program_header_get(helf_file_input_t *input_file,size_t index,helf_elf32_program_header_t *programheader)
{
    if(programheader==NULL)
    {
        return false;
    }
    if(!helf_ppc_is_ppc(input_file))
    {
        return false;
    }
    if(index>=helf_arch_program_header_count_get(input_file))
    {
        return false;
    }
    return helf_file_input_32_bits_program_header_get(input_file,index,programheader);
}

size_t helf_ppc_segment_count_get(helf_file_input_t *input_file)
{
    return helf_ppc_program_header_count_get(input_file);
}

bool helf_ppc_segment_info_get(helf_file_input_t *input_file,size_t index,helf_ppc_segment_info_t *info)
{
    if(!helf_ppc_is_ppc(input_file))
    {
        return false;
    }
    return helf_arch_segment_info_get(input_file,index,info);
}

bool helf_ppc_is_load_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ppc_is_ppc(input_file))
    {
        return false;
    }
    return helf_arch_is_load_segment(index,input_file);
}

bool helf_ppc_is_rx_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ppc_is_ppc(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_segment(index,input_file);
}

bool helf_ppc_is_ro_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ppc_is_ppc(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_segment(index,input_file);
}

bool helf_ppc_is_rw_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ppc_is_ppc(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_segment(index,input_file);
}

size_t helf_ppc_load_segment_count_get(helf_file_input_t *input_file)
{
    if(!helf_ppc_is_ppc(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_count_get(input_file);
}

bool helf_ppc_load_segment_info_get(helf_file_input_t *input_file,size_t index,helf_ppc_segment_info_t *info)
{
    if(!helf_ppc_is_ppc(input_file))
    {
        return false;
    }
    return helf_arch_load_segment_info_get(input_file,index,info);
}

size_t helf_ppc_load_segment_total_filesz_get(helf_file_input_t *input_file)
{
    if(!helf_ppc_is_ppc(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_filesz_get(input_file);
}

size_t helf_ppc_load_segment_total_memsz_get(helf_file_input_t *input_file)
{
    if(!helf_ppc_is_ppc(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_memsz_get(input_file);
}

size_t helf_ppc_relocation_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_ppc_is_ppc(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_section_count_get(input_file);
}

bool helf_ppc_relocation_section_info_get(helf_file_input_t *input_file,size_t index,helf_arch_relocation_section_info_t *info)
{
    if(!helf_ppc_is_ppc(input_file))
    {
        return false;
    }
    return helf_arch_relocation_section_info_get(input_file,index,info);
}

size_t helf_ppc_relocation_count_get(helf_file_input_t *input_file,size_t section_index)
{
    if(!helf_ppc_is_ppc(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_count_get(input_file,section_index);
}

bool helf_ppc_relocation_get(helf_file_input_t *input_file,size_t section_index,size_t index,helf_arch_relocation_info_t *info)
{
    if(!helf_ppc_is_ppc(input_file))
    {
        return false;
    }
    if(!helf_arch_relocation_get(input_file,section_index,index,info))
    {
        return false;
    }
    return true;
}

bool helf_ppc_relocation_symbol_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_symbol_info_t *info)
{
    if(!helf_ppc_is_ppc(input_file))
    {
        return false;
    }
    return helf_arch_relocation_symbol_info_get(input_file,relocation,info);
}

static const helf_arch_relocation_type_name_t helf_ppc_relocation_type_names[]=
{
    {0,0,"R_PPC_NONE"},
    {1,4,"R_PPC_ADDR32"},
    {2,4,"R_PPC_ADDR24"},
    {3,2,"R_PPC_ADDR16"},
    {4,2,"R_PPC_ADDR16_LO"},
    {5,2,"R_PPC_ADDR16_HI"},
    {6,2,"R_PPC_ADDR16_HA"},
    {7,4,"R_PPC_ADDR14"},
    {8,4,"R_PPC_ADDR14_BRTAKEN"},
    {9,4,"R_PPC_ADDR14_BRNTAKEN"},
    {10,4,"R_PPC_REL24"},
    {11,4,"R_PPC_REL14"},
    {12,4,"R_PPC_REL14_BRTAKEN"},
    {13,4,"R_PPC_REL14_BRNTAKEN"},
    {14,2,"R_PPC_GOT16"},
    {15,2,"R_PPC_GOT16_LO"},
    {16,2,"R_PPC_GOT16_HI"},
    {17,2,"R_PPC_GOT16_HA"},
    {18,4,"R_PPC_PLTREL24"},
    {19,4,"R_PPC_COPY"},
    {20,4,"R_PPC_GLOB_DAT"},
    {21,4,"R_PPC_JMP_SLOT"},
    {22,4,"R_PPC_RELATIVE"},
    {23,4,"R_PPC_LOCAL24PC"},
    {24,4,"R_PPC_UADDR32"},
    {25,2,"R_PPC_UADDR16"},
    {26,4,"R_PPC_REL32"},
    {27,4,"R_PPC_PLT32"},
    {28,4,"R_PPC_PLTREL32"},
    {29,2,"R_PPC_PLT16_LO"},
    {30,2,"R_PPC_PLT16_HI"},
    {31,2,"R_PPC_PLT16_HA"},
    {32,2,"R_PPC_SDAREL16"},
    {33,2,"R_PPC_SECTOFF"},
    {34,2,"R_PPC_SECTOFF_LO"},
    {35,2,"R_PPC_SECTOFF_HI"},
    {36,2,"R_PPC_SECTOFF_HA"},
    {37,0,"R_PPC_ADDR30"},
    {48,0,"R_PPC_RELAX"},
    {49,0,"R_PPC_RELAX_PLT"},
    {50,0,"R_PPC_RELAX_PLTREL24"},
    {51,4,"R_PPC_16DX_HA"},
    {67,4,"R_PPC_TLS"},
    {68,4,"R_PPC_DTPMOD32"},
    {69,2,"R_PPC_TPREL16"},
    {70,2,"R_PPC_TPREL16_LO"},
    {71,2,"R_PPC_TPREL16_HI"},
    {72,2,"R_PPC_TPREL16_HA"},
    {73,4,"R_PPC_TPREL32"},
    {74,2,"R_PPC_DTPREL16"},
    {75,2,"R_PPC_DTPREL16_LO"},
    {76,2,"R_PPC_DTPREL16_HI"},
    {77,2,"R_PPC_DTPREL16_HA"},
    {78,4,"R_PPC_DTPREL32"},
    {79,2,"R_PPC_GOT_TLSGD16"},
    {80,2,"R_PPC_GOT_TLSGD16_LO"},
    {81,2,"R_PPC_GOT_TLSGD16_HI"},
    {82,2,"R_PPC_GOT_TLSGD16_HA"},
    {83,2,"R_PPC_GOT_TLSLD16"},
    {84,2,"R_PPC_GOT_TLSLD16_LO"},
    {85,2,"R_PPC_GOT_TLSLD16_HI"},
    {86,2,"R_PPC_GOT_TLSLD16_HA"},
    {87,2,"R_PPC_GOT_TPREL16"},
    {88,2,"R_PPC_GOT_TPREL16_LO"},
    {89,2,"R_PPC_GOT_TPREL16_HI"},
    {90,2,"R_PPC_GOT_TPREL16_HA"},
    {91,2,"R_PPC_GOT_DTPREL16"},
    {92,2,"R_PPC_GOT_DTPREL16_LO"},
    {93,2,"R_PPC_GOT_DTPREL16_HI"},
    {94,2,"R_PPC_GOT_DTPREL16_HA"},
    {95,4,"R_PPC_TLSGD"},
    {96,4,"R_PPC_TLSLD"},
    {101,4,"R_PPC_EMB_NADDR32"},
    {102,2,"R_PPC_EMB_NADDR16"},
    {103,2,"R_PPC_EMB_NADDR16_LO"},
    {104,2,"R_PPC_EMB_NADDR16_HI"},
    {105,2,"R_PPC_EMB_NADDR16_HA"},
    {106,2,"R_PPC_EMB_SDAI16"},
    {107,2,"R_PPC_EMB_SDA2I16"},
    {108,2,"R_PPC_EMB_SDA2REL"},
    {109,4,"R_PPC_EMB_SDA21"},
    {110,0,"R_PPC_EMB_MRKREF"},
    {111,2,"R_PPC_EMB_RELSEC16"},
    {112,0,"R_PPC_EMB_RELST_LO"},
    {113,0,"R_PPC_EMB_RELST_HI"},
    {114,0,"R_PPC_EMB_RELST_HA"},
    {115,0,"R_PPC_EMB_BIT_FLD"},
    {116,2,"R_PPC_EMB_RELSDA"},
    {119,4,"R_PPC_PLTSEQ"},
    {120,4,"R_PPC_PLTCALL"},
    {216,2,"R_PPC_VLE_REL8"},
    {217,4,"R_PPC_VLE_REL15"},
    {218,4,"R_PPC_VLE_REL24"},
    {219,4,"R_PPC_VLE_LO16A"},
    {220,4,"R_PPC_VLE_LO16D"},
    {221,4,"R_PPC_VLE_HI16A"},
    {222,4,"R_PPC_VLE_HI16D"},
    {223,4,"R_PPC_VLE_HA16A"},
    {224,4,"R_PPC_VLE_HA16D"},
    {225,4,"R_PPC_VLE_SDA21"},
    {226,4,"R_PPC_VLE_SDA21_LO"},
    {227,4,"R_PPC_VLE_SDAREL_LO16A"},
    {228,4,"R_PPC_VLE_SDAREL_LO16D"},
    {229,4,"R_PPC_VLE_SDAREL_HI16A"},
    {230,4,"R_PPC_VLE_SDAREL_HI16D"},
    {231,4,"R_PPC_VLE_SDAREL_HA16A"},
    {232,4,"R_PPC_VLE_SDAREL_HA16D"},
    {233,4,"R_PPC_VLE_ADDR20"},
    {246,4,"R_PPC_REL16DX_HA"},
    {248,4,"R_PPC_IRELATIVE"},
    {249,2,"R_PPC_REL16"},
    {250,2,"R_PPC_REL16_LO"},
    {251,2,"R_PPC_REL16_HI"},
    {252,2,"R_PPC_REL16_HA"},
    {253,0,"R_PPC_GNU_VTINHERIT"},
    {254,0,"R_PPC_GNU_VTENTRY"},
    {255,2,"R_PPC_TOC16"},
};

const char *helf_ppc_relocation_type_name_get(uint32_t type)
{
    return helf_arch_relocation_type_name_get(helf_ppc_relocation_type_names,sizeof(helf_ppc_relocation_type_names)/sizeof(helf_ppc_relocation_type_names[0]),type);
}

uint8_t helf_ppc_relocation_size_get(uint32_t type)
{
    return helf_arch_relocation_type_size_get(helf_ppc_relocation_type_names,sizeof(helf_ppc_relocation_type_names)/sizeof(helf_ppc_relocation_type_names[0]),type);
}

static const helf_arch_section_type_name_t helf_ppc_section_type_names[]=
{
    {2147483647,"SHT_ORDERED"},
};

const char *helf_ppc_section_type_name_get(uint32_t type)
{
    return helf_arch_section_type_name_get(helf_ppc_section_type_names,sizeof(helf_ppc_section_type_names)/sizeof(helf_ppc_section_type_names[0]),type);
}

bool helf_ppc_relocation_address_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_relocation_address_info_t *info)
{
    if(!helf_ppc_is_ppc(input_file) || relocation==NULL)
    {
        return false;
    }
    uint8_t size=helf_ppc_relocation_size_get(relocation->type);
    if(size==0)
    {
        return false;
    }
    return helf_arch_relocation_address_info_get(input_file,relocation,size,info);
}

size_t helf_ppc_relocation_data_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,void *buffer,size_t buffer_length)
{
    if(!helf_ppc_is_ppc(input_file) || relocation==NULL)
    {
        return 0;
    }
    uint8_t size=helf_ppc_relocation_size_get(relocation->type);
    if(size==0)
    {
        return 0;
    }
    return helf_arch_relocation_data_get(input_file,relocation,size,buffer,buffer_length);
}

bool helf_ppc_is_shared_object(helf_file_input_t *input_file)
{
    return helf_arch_is_shared_object(input_file,helf_ppc_machines,sizeof(helf_ppc_machines)/sizeof(helf_ppc_machines[0]),HELF_ELFCLASS_32);
}

bool helf_ppc_is_relocatable(helf_file_input_t *input_file)
{
    return helf_arch_is_relocatable(input_file,helf_ppc_machines,sizeof(helf_ppc_machines)/sizeof(helf_ppc_machines[0]),HELF_ELFCLASS_32);
}

size_t helf_ppc_dynamic_count_get(helf_file_input_t *input_file)
{
    if(!helf_ppc_is_ppc(input_file))
    {
        return 0;
    }
    return helf_arch_dynamic_count_get(input_file);
}

bool helf_ppc_dynamic_info_get(helf_file_input_t *input_file,size_t index,helf_arch_dynamic_info_t *info)
{
    if(!helf_ppc_is_ppc(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_info_get(input_file,index,info);
}

bool helf_ppc_dynamic_string_get(helf_file_input_t *input_file,uint64_t offset,char *buffer,size_t buffer_length)
{
    if(!helf_ppc_is_ppc(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_string_get(input_file,offset,buffer,buffer_length);
}
