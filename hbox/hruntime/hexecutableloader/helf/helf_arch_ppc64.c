/***************************************************************
 * Name:      helf_arch_ppc64.c
 * Purpose:   实现helf_arch_ppc64接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "helf_arch_ppc64.h"

static const uint16_t helf_ppc64_machines[]=
{
    HELF_EM_PPC64,
};

bool helf_ppc64_is_ppc64(helf_file_input_t *input_file)
{
    return helf_arch_is(input_file,helf_ppc64_machines,sizeof(helf_ppc64_machines)/sizeof(helf_ppc64_machines[0]),HELF_ELFCLASS_64);
}

bool helf_ppc64_is_big_endian(helf_file_input_t *input_file)
{
    if(!helf_ppc64_is_ppc64(input_file))
    {
        return false;
    }
    return helf_file_input_is_big_endian(input_file);
}

bool helf_ppc64_is_executable(helf_file_input_t *input_file)
{
    return helf_arch_is_executable(input_file,helf_ppc64_machines,sizeof(helf_ppc64_machines)/sizeof(helf_ppc64_machines[0]),HELF_ELFCLASS_64);
}

bool helf_ppc64_entry_get(helf_file_input_t *input_file,uint64_t *entry)
{
    if(entry==NULL)
    {
        return false;
    }
    if(!helf_ppc64_is_ppc64(input_file))
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

size_t helf_ppc64_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_ppc64_is_ppc64(input_file))
    {
        return 0;
    }
    return helf_arch_section_count_get(input_file);
}

bool helf_ppc64_section_info_get(helf_file_input_t *input_file,size_t index,helf_ppc64_section_info_t *info)
{
    if(!helf_ppc64_is_ppc64(input_file))
    {
        return false;
    }
    return helf_arch_section_info_get(input_file,index,info);
}

bool helf_ppc64_is_rx_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ppc64_is_ppc64(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_section(index,input_file);
}

intptr_t helf_ppc64_rx_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ppc64_is_rx_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rx_section_offset_get(index,input_file);
}

size_t helf_ppc64_rx_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_ppc64_is_ppc64(input_file))
    {
        return 0;
    }
    return helf_arch_rx_section_size_get(input_file);
}

bool helf_ppc64_is_ro_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ppc64_is_ppc64(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_section(index,input_file);
}

intptr_t helf_ppc64_ro_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ppc64_is_ro_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_ro_section_offset_get(index,input_file);
}

size_t helf_ppc64_ro_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_ppc64_is_ppc64(input_file))
    {
        return 0;
    }
    return helf_arch_ro_section_size_get(input_file);
}

bool helf_ppc64_is_rw_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ppc64_is_ppc64(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_section(index,input_file);
}

intptr_t helf_ppc64_rw_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ppc64_is_rw_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rw_section_offset_get(index,input_file);
}

size_t helf_ppc64_rw_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_ppc64_is_ppc64(input_file))
    {
        return 0;
    }
    return helf_arch_rw_section_size_get(input_file);
}

bool helf_ppc64_is_bss_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ppc64_is_ppc64(input_file))
    {
        return false;
    }
    return helf_arch_is_bss_section(index,input_file);
}

size_t helf_ppc64_program_header_count_get(helf_file_input_t *input_file)
{
    if(!helf_ppc64_is_ppc64(input_file))
    {
        return 0;
    }
    return helf_arch_program_header_count_get(input_file);
}

bool helf_ppc64_program_header_get(helf_file_input_t *input_file,size_t index,helf_elf64_program_header_t *programheader)
{
    if(programheader==NULL)
    {
        return false;
    }
    if(!helf_ppc64_is_ppc64(input_file))
    {
        return false;
    }
    if(index>=helf_arch_program_header_count_get(input_file))
    {
        return false;
    }
    return helf_file_input_64_bits_program_header_get(input_file,index,programheader);
}

size_t helf_ppc64_segment_count_get(helf_file_input_t *input_file)
{
    return helf_ppc64_program_header_count_get(input_file);
}

bool helf_ppc64_segment_info_get(helf_file_input_t *input_file,size_t index,helf_ppc64_segment_info_t *info)
{
    if(!helf_ppc64_is_ppc64(input_file))
    {
        return false;
    }
    return helf_arch_segment_info_get(input_file,index,info);
}

bool helf_ppc64_is_load_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ppc64_is_ppc64(input_file))
    {
        return false;
    }
    return helf_arch_is_load_segment(index,input_file);
}

bool helf_ppc64_is_rx_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ppc64_is_ppc64(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_segment(index,input_file);
}

bool helf_ppc64_is_ro_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ppc64_is_ppc64(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_segment(index,input_file);
}

bool helf_ppc64_is_rw_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ppc64_is_ppc64(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_segment(index,input_file);
}

size_t helf_ppc64_load_segment_count_get(helf_file_input_t *input_file)
{
    if(!helf_ppc64_is_ppc64(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_count_get(input_file);
}

bool helf_ppc64_load_segment_info_get(helf_file_input_t *input_file,size_t index,helf_ppc64_segment_info_t *info)
{
    if(!helf_ppc64_is_ppc64(input_file))
    {
        return false;
    }
    return helf_arch_load_segment_info_get(input_file,index,info);
}

size_t helf_ppc64_load_segment_total_filesz_get(helf_file_input_t *input_file)
{
    if(!helf_ppc64_is_ppc64(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_filesz_get(input_file);
}

size_t helf_ppc64_load_segment_total_memsz_get(helf_file_input_t *input_file)
{
    if(!helf_ppc64_is_ppc64(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_memsz_get(input_file);
}

size_t helf_ppc64_relocation_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_ppc64_is_ppc64(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_section_count_get(input_file);
}

bool helf_ppc64_relocation_section_info_get(helf_file_input_t *input_file,size_t index,helf_arch_relocation_section_info_t *info)
{
    if(!helf_ppc64_is_ppc64(input_file))
    {
        return false;
    }
    return helf_arch_relocation_section_info_get(input_file,index,info);
}

size_t helf_ppc64_relocation_count_get(helf_file_input_t *input_file,size_t section_index)
{
    if(!helf_ppc64_is_ppc64(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_count_get(input_file,section_index);
}

bool helf_ppc64_relocation_get(helf_file_input_t *input_file,size_t section_index,size_t index,helf_arch_relocation_info_t *info)
{
    if(!helf_ppc64_is_ppc64(input_file))
    {
        return false;
    }
    if(!helf_arch_relocation_get(input_file,section_index,index,info))
    {
        return false;
    }
    return true;
}

bool helf_ppc64_relocation_symbol_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_symbol_info_t *info)
{
    if(!helf_ppc64_is_ppc64(input_file))
    {
        return false;
    }
    return helf_arch_relocation_symbol_info_get(input_file,relocation,info);
}

static const helf_arch_relocation_type_name_t helf_ppc64_relocation_type_names[]=
{
    {0,0,"R_PPC64_NONE"},
    {1,4,"R_PPC64_ADDR32"},
    {2,4,"R_PPC64_ADDR24"},
    {3,2,"R_PPC64_ADDR16"},
    {4,2,"R_PPC64_ADDR16_LO"},
    {5,2,"R_PPC64_ADDR16_HI"},
    {6,2,"R_PPC64_ADDR16_HA"},
    {7,4,"R_PPC64_ADDR14"},
    {8,4,"R_PPC64_ADDR14_BRTAKEN"},
    {9,4,"R_PPC64_ADDR14_BRNTAKEN"},
    {10,4,"R_PPC64_REL24"},
    {11,4,"R_PPC64_REL14"},
    {12,4,"R_PPC64_REL14_BRTAKEN"},
    {13,4,"R_PPC64_REL14_BRNTAKEN"},
    {14,2,"R_PPC64_GOT16"},
    {15,2,"R_PPC64_GOT16_LO"},
    {16,2,"R_PPC64_GOT16_HI"},
    {17,2,"R_PPC64_GOT16_HA"},
    {19,0,"R_PPC64_COPY"},
    {20,8,"R_PPC64_GLOB_DAT"},
    {21,0,"R_PPC64_JMP_SLOT"},
    {22,8,"R_PPC64_RELATIVE"},
    {24,4,"R_PPC64_UADDR32"},
    {25,2,"R_PPC64_UADDR16"},
    {26,4,"R_PPC64_REL32"},
    {27,4,"R_PPC64_PLT32"},
    {28,4,"R_PPC64_PLTREL32"},
    {29,2,"R_PPC64_PLT16_LO"},
    {30,2,"R_PPC64_PLT16_HI"},
    {31,2,"R_PPC64_PLT16_HA"},
    {33,2,"R_PPC64_SECTOFF"},
    {34,2,"R_PPC64_SECTOFF_LO"},
    {35,2,"R_PPC64_SECTOFF_HI"},
    {36,2,"R_PPC64_SECTOFF_HA"},
    {37,4,"R_PPC64_REL30"},
    {38,8,"R_PPC64_ADDR64"},
    {39,2,"R_PPC64_ADDR16_HIGHER"},
    {40,2,"R_PPC64_ADDR16_HIGHERA"},
    {41,2,"R_PPC64_ADDR16_HIGHEST"},
    {42,2,"R_PPC64_ADDR16_HIGHESTA"},
    {43,8,"R_PPC64_UADDR64"},
    {44,8,"R_PPC64_REL64"},
    {45,8,"R_PPC64_PLT64"},
    {46,8,"R_PPC64_PLTREL64"},
    {47,2,"R_PPC64_TOC16"},
    {48,2,"R_PPC64_TOC16_LO"},
    {49,2,"R_PPC64_TOC16_HI"},
    {50,2,"R_PPC64_TOC16_HA"},
    {51,8,"R_PPC64_TOC"},
    {52,2,"R_PPC64_PLTGOT16"},
    {53,2,"R_PPC64_PLTGOT16_LO"},
    {54,2,"R_PPC64_PLTGOT16_HI"},
    {55,2,"R_PPC64_PLTGOT16_HA"},
    {56,2,"R_PPC64_ADDR16_DS"},
    {57,2,"R_PPC64_ADDR16_LO_DS"},
    {58,2,"R_PPC64_GOT16_DS"},
    {59,2,"R_PPC64_GOT16_LO_DS"},
    {60,2,"R_PPC64_PLT16_LO_DS"},
    {61,2,"R_PPC64_SECTOFF_DS"},
    {62,2,"R_PPC64_SECTOFF_LO_DS"},
    {63,2,"R_PPC64_TOC16_DS"},
    {64,2,"R_PPC64_TOC16_LO_DS"},
    {65,2,"R_PPC64_PLTGOT16_DS"},
    {66,2,"R_PPC64_PLTGOT16_LO_DS"},
    {67,4,"R_PPC64_TLS"},
    {68,8,"R_PPC64_DTPMOD64"},
    {69,2,"R_PPC64_TPREL16"},
    {70,2,"R_PPC64_TPREL16_LO"},
    {71,2,"R_PPC64_TPREL16_HI"},
    {72,2,"R_PPC64_TPREL16_HA"},
    {73,8,"R_PPC64_TPREL64"},
    {74,2,"R_PPC64_DTPREL16"},
    {75,2,"R_PPC64_DTPREL16_LO"},
    {76,2,"R_PPC64_DTPREL16_HI"},
    {77,2,"R_PPC64_DTPREL16_HA"},
    {78,8,"R_PPC64_DTPREL64"},
    {79,2,"R_PPC64_GOT_TLSGD16"},
    {80,2,"R_PPC64_GOT_TLSGD16_LO"},
    {81,2,"R_PPC64_GOT_TLSGD16_HI"},
    {82,2,"R_PPC64_GOT_TLSGD16_HA"},
    {83,2,"R_PPC64_GOT_TLSLD16"},
    {84,2,"R_PPC64_GOT_TLSLD16_LO"},
    {85,2,"R_PPC64_GOT_TLSLD16_HI"},
    {86,2,"R_PPC64_GOT_TLSLD16_HA"},
    {87,2,"R_PPC64_GOT_TPREL16_DS"},
    {88,2,"R_PPC64_GOT_TPREL16_LO_DS"},
    {89,2,"R_PPC64_GOT_TPREL16_HI"},
    {90,2,"R_PPC64_GOT_TPREL16_HA"},
    {91,2,"R_PPC64_GOT_DTPREL16_DS"},
    {92,2,"R_PPC64_GOT_DTPREL16_LO_DS"},
    {93,2,"R_PPC64_GOT_DTPREL16_HI"},
    {94,2,"R_PPC64_GOT_DTPREL16_HA"},
    {95,2,"R_PPC64_TPREL16_DS"},
    {96,2,"R_PPC64_TPREL16_LO_DS"},
    {97,2,"R_PPC64_TPREL16_HIGHER"},
    {98,2,"R_PPC64_TPREL16_HIGHERA"},
    {99,2,"R_PPC64_TPREL16_HIGHEST"},
    {100,2,"R_PPC64_TPREL16_HIGHESTA"},
    {101,2,"R_PPC64_DTPREL16_DS"},
    {102,2,"R_PPC64_DTPREL16_LO_DS"},
    {103,2,"R_PPC64_DTPREL16_HIGHER"},
    {104,2,"R_PPC64_DTPREL16_HIGHERA"},
    {105,2,"R_PPC64_DTPREL16_HIGHEST"},
    {106,2,"R_PPC64_DTPREL16_HIGHESTA"},
    {107,4,"R_PPC64_TLSGD"},
    {108,4,"R_PPC64_TLSLD"},
    {109,4,"R_PPC64_TOCSAVE"},
    {110,2,"R_PPC64_ADDR16_HIGH"},
    {111,2,"R_PPC64_ADDR16_HIGHA"},
    {112,2,"R_PPC64_TPREL16_HIGH"},
    {113,2,"R_PPC64_TPREL16_HIGHA"},
    {114,2,"R_PPC64_DTPREL16_HIGH"},
    {115,2,"R_PPC64_DTPREL16_HIGHA"},
    {116,4,"R_PPC64_REL24_NOTOC"},
    {117,8,"R_PPC64_ADDR64_LOCAL"},
    {118,4,"R_PPC64_ENTRY"},
    {119,4,"R_PPC64_PLTSEQ"},
    {120,4,"R_PPC64_PLTCALL"},
    {121,4,"R_PPC64_PLTSEQ_NOTOC"},
    {122,4,"R_PPC64_PLTCALL_NOTOC"},
    {123,4,"R_PPC64_PCREL_OPT"},
    {124,4,"R_PPC64_REL24_P9NOTOC"},
    {128,8,"R_PPC64_D34"},
    {129,8,"R_PPC64_D34_LO"},
    {130,8,"R_PPC64_D34_HI30"},
    {131,8,"R_PPC64_D34_HA30"},
    {132,8,"R_PPC64_PCREL34"},
    {133,8,"R_PPC64_GOT_PCREL34"},
    {134,8,"R_PPC64_PLT_PCREL34"},
    {135,8,"R_PPC64_PLT_PCREL34_NOTOC"},
    {136,2,"R_PPC64_ADDR16_HIGHER34"},
    {137,2,"R_PPC64_ADDR16_HIGHERA34"},
    {138,2,"R_PPC64_ADDR16_HIGHEST34"},
    {139,2,"R_PPC64_ADDR16_HIGHESTA34"},
    {140,2,"R_PPC64_REL16_HIGHER34"},
    {141,2,"R_PPC64_REL16_HIGHERA34"},
    {142,2,"R_PPC64_REL16_HIGHEST34"},
    {143,2,"R_PPC64_REL16_HIGHESTA34"},
    {144,8,"R_PPC64_D28"},
    {145,8,"R_PPC64_PCREL28"},
    {146,8,"R_PPC64_TPREL34"},
    {147,8,"R_PPC64_DTPREL34"},
    {148,8,"R_PPC64_GOT_TLSGD_PCREL34"},
    {149,8,"R_PPC64_GOT_TLSLD_PCREL34"},
    {150,8,"R_PPC64_GOT_TPREL_PCREL34"},
    {151,8,"R_PPC64_GOT_DTPREL_PCREL34"},
    {200,0,"R_PPC64_LO_DS_OPT"},
    {201,4,"R_PPC64_16DX_HA"},
    {240,2,"R_PPC64_REL16_HIGH"},
    {241,2,"R_PPC64_REL16_HIGHA"},
    {242,2,"R_PPC64_REL16_HIGHER"},
    {243,2,"R_PPC64_REL16_HIGHERA"},
    {244,2,"R_PPC64_REL16_HIGHEST"},
    {245,2,"R_PPC64_REL16_HIGHESTA"},
    {246,4,"R_PPC64_REL16DX_HA"},
    {247,0,"R_PPC64_JMP_IREL"},
    {248,8,"R_PPC64_IRELATIVE"},
    {249,2,"R_PPC64_REL16"},
    {250,2,"R_PPC64_REL16_LO"},
    {251,2,"R_PPC64_REL16_HI"},
    {252,2,"R_PPC64_REL16_HA"},
    {253,0,"R_PPC64_GNU_VTINHERIT"},
    {254,0,"R_PPC64_GNU_VTENTRY"},
};

const char *helf_ppc64_relocation_type_name_get(uint32_t type)
{
    return helf_arch_relocation_type_name_get(helf_ppc64_relocation_type_names,sizeof(helf_ppc64_relocation_type_names)/sizeof(helf_ppc64_relocation_type_names[0]),type);
}

uint8_t helf_ppc64_relocation_size_get(uint32_t type)
{
    return helf_arch_relocation_type_size_get(helf_ppc64_relocation_type_names,sizeof(helf_ppc64_relocation_type_names)/sizeof(helf_ppc64_relocation_type_names[0]),type);
}

const char *helf_ppc64_section_type_name_get(uint32_t type)
{
    (void)type;
    return NULL;
}

bool helf_ppc64_relocation_address_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_relocation_address_info_t *info)
{
    if(!helf_ppc64_is_ppc64(input_file) || relocation==NULL)
    {
        return false;
    }
    uint8_t size=helf_ppc64_relocation_size_get(relocation->type);
    if(size==0)
    {
        return false;
    }
    return helf_arch_relocation_address_info_get(input_file,relocation,size,info);
}

size_t helf_ppc64_relocation_data_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,void *buffer,size_t buffer_length)
{
    if(!helf_ppc64_is_ppc64(input_file) || relocation==NULL)
    {
        return 0;
    }
    uint8_t size=helf_ppc64_relocation_size_get(relocation->type);
    if(size==0)
    {
        return 0;
    }
    return helf_arch_relocation_data_get(input_file,relocation,size,buffer,buffer_length);
}

bool helf_ppc64_is_shared_object(helf_file_input_t *input_file)
{
    return helf_arch_is_shared_object(input_file,helf_ppc64_machines,sizeof(helf_ppc64_machines)/sizeof(helf_ppc64_machines[0]),HELF_ELFCLASS_64);
}

bool helf_ppc64_is_relocatable(helf_file_input_t *input_file)
{
    return helf_arch_is_relocatable(input_file,helf_ppc64_machines,sizeof(helf_ppc64_machines)/sizeof(helf_ppc64_machines[0]),HELF_ELFCLASS_64);
}

size_t helf_ppc64_dynamic_count_get(helf_file_input_t *input_file)
{
    if(!helf_ppc64_is_ppc64(input_file))
    {
        return 0;
    }
    return helf_arch_dynamic_count_get(input_file);
}

bool helf_ppc64_dynamic_info_get(helf_file_input_t *input_file,size_t index,helf_arch_dynamic_info_t *info)
{
    if(!helf_ppc64_is_ppc64(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_info_get(input_file,index,info);
}

bool helf_ppc64_dynamic_string_get(helf_file_input_t *input_file,uint64_t offset,char *buffer,size_t buffer_length)
{
    if(!helf_ppc64_is_ppc64(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_string_get(input_file,offset,buffer,buffer_length);
}
