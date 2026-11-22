/***************************************************************
 * Name:      helf_arch_sparc.c
 * Purpose:   实现helf_arch_sparc接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "helf_arch_sparc.h"

static const uint16_t helf_sparc_machines[]=
{
    HELF_EM_SPARC,
    HELF_EM_SPARC32PLUS,
};

bool helf_sparc_is_sparc(helf_file_input_t *input_file)
{
    return helf_arch_is(input_file,helf_sparc_machines,sizeof(helf_sparc_machines)/sizeof(helf_sparc_machines[0]),HELF_ELFCLASS_32);
}

bool helf_sparc_is_big_endian(helf_file_input_t *input_file)
{
    if(!helf_sparc_is_sparc(input_file))
    {
        return false;
    }
    return helf_file_input_is_big_endian(input_file);
}

bool helf_sparc_is_executable(helf_file_input_t *input_file)
{
    return helf_arch_is_executable(input_file,helf_sparc_machines,sizeof(helf_sparc_machines)/sizeof(helf_sparc_machines[0]),HELF_ELFCLASS_32);
}

bool helf_sparc_entry_get(helf_file_input_t *input_file,uint32_t *entry)
{
    if(entry==NULL)
    {
        return false;
    }
    if(!helf_sparc_is_sparc(input_file))
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

size_t helf_sparc_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_sparc_is_sparc(input_file))
    {
        return 0;
    }
    return helf_arch_section_count_get(input_file);
}

bool helf_sparc_section_info_get(helf_file_input_t *input_file,size_t index,helf_sparc_section_info_t *info)
{
    if(!helf_sparc_is_sparc(input_file))
    {
        return false;
    }
    return helf_arch_section_info_get(input_file,index,info);
}

bool helf_sparc_is_rx_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_sparc_is_sparc(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_section(index,input_file);
}

intptr_t helf_sparc_rx_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_sparc_is_rx_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rx_section_offset_get(index,input_file);
}

size_t helf_sparc_rx_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_sparc_is_sparc(input_file))
    {
        return 0;
    }
    return helf_arch_rx_section_size_get(input_file);
}

bool helf_sparc_is_ro_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_sparc_is_sparc(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_section(index,input_file);
}

intptr_t helf_sparc_ro_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_sparc_is_ro_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_ro_section_offset_get(index,input_file);
}

size_t helf_sparc_ro_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_sparc_is_sparc(input_file))
    {
        return 0;
    }
    return helf_arch_ro_section_size_get(input_file);
}

bool helf_sparc_is_rw_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_sparc_is_sparc(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_section(index,input_file);
}

intptr_t helf_sparc_rw_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_sparc_is_rw_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rw_section_offset_get(index,input_file);
}

size_t helf_sparc_rw_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_sparc_is_sparc(input_file))
    {
        return 0;
    }
    return helf_arch_rw_section_size_get(input_file);
}

bool helf_sparc_is_bss_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_sparc_is_sparc(input_file))
    {
        return false;
    }
    return helf_arch_is_bss_section(index,input_file);
}

size_t helf_sparc_program_header_count_get(helf_file_input_t *input_file)
{
    if(!helf_sparc_is_sparc(input_file))
    {
        return 0;
    }
    return helf_arch_program_header_count_get(input_file);
}

bool helf_sparc_program_header_get(helf_file_input_t *input_file,size_t index,helf_elf32_program_header_t *programheader)
{
    if(programheader==NULL)
    {
        return false;
    }
    if(!helf_sparc_is_sparc(input_file))
    {
        return false;
    }
    if(index>=helf_arch_program_header_count_get(input_file))
    {
        return false;
    }
    return helf_file_input_32_bits_program_header_get(input_file,index,programheader);
}

size_t helf_sparc_segment_count_get(helf_file_input_t *input_file)
{
    return helf_sparc_program_header_count_get(input_file);
}

bool helf_sparc_segment_info_get(helf_file_input_t *input_file,size_t index,helf_sparc_segment_info_t *info)
{
    if(!helf_sparc_is_sparc(input_file))
    {
        return false;
    }
    return helf_arch_segment_info_get(input_file,index,info);
}

bool helf_sparc_is_load_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_sparc_is_sparc(input_file))
    {
        return false;
    }
    return helf_arch_is_load_segment(index,input_file);
}

bool helf_sparc_is_rx_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_sparc_is_sparc(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_segment(index,input_file);
}

bool helf_sparc_is_ro_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_sparc_is_sparc(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_segment(index,input_file);
}

bool helf_sparc_is_rw_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_sparc_is_sparc(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_segment(index,input_file);
}

size_t helf_sparc_load_segment_count_get(helf_file_input_t *input_file)
{
    if(!helf_sparc_is_sparc(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_count_get(input_file);
}

bool helf_sparc_load_segment_info_get(helf_file_input_t *input_file,size_t index,helf_sparc_segment_info_t *info)
{
    if(!helf_sparc_is_sparc(input_file))
    {
        return false;
    }
    return helf_arch_load_segment_info_get(input_file,index,info);
}

size_t helf_sparc_load_segment_total_filesz_get(helf_file_input_t *input_file)
{
    if(!helf_sparc_is_sparc(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_filesz_get(input_file);
}

size_t helf_sparc_load_segment_total_memsz_get(helf_file_input_t *input_file)
{
    if(!helf_sparc_is_sparc(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_memsz_get(input_file);
}

size_t helf_sparc_relocation_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_sparc_is_sparc(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_section_count_get(input_file);
}

bool helf_sparc_relocation_section_info_get(helf_file_input_t *input_file,size_t index,helf_arch_relocation_section_info_t *info)
{
    if(!helf_sparc_is_sparc(input_file))
    {
        return false;
    }
    return helf_arch_relocation_section_info_get(input_file,index,info);
}

size_t helf_sparc_relocation_count_get(helf_file_input_t *input_file,size_t section_index)
{
    if(!helf_sparc_is_sparc(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_count_get(input_file,section_index);
}

bool helf_sparc_relocation_get(helf_file_input_t *input_file,size_t section_index,size_t index,helf_arch_relocation_info_t *info)
{
    if(!helf_sparc_is_sparc(input_file))
    {
        return false;
    }
    if(!helf_arch_relocation_get(input_file,section_index,index,info))
    {
        return false;
    }
    return true;
}

bool helf_sparc_relocation_symbol_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_symbol_info_t *info)
{
    if(!helf_sparc_is_sparc(input_file))
    {
        return false;
    }
    return helf_arch_relocation_symbol_info_get(input_file,relocation,info);
}

static const helf_arch_relocation_type_name_t helf_sparc_relocation_type_names[]=
{
    {0,0,"R_SPARC_NONE"},
    {1,1,"R_SPARC_8"},
    {2,2,"R_SPARC_16"},
    {3,4,"R_SPARC_32"},
    {4,1,"R_SPARC_DISP8"},
    {5,2,"R_SPARC_DISP16"},
    {6,4,"R_SPARC_DISP32"},
    {7,4,"R_SPARC_WDISP30"},
    {8,4,"R_SPARC_WDISP22"},
    {9,4,"R_SPARC_HI22"},
    {10,4,"R_SPARC_22"},
    {11,4,"R_SPARC_13"},
    {12,4,"R_SPARC_LO10"},
    {13,4,"R_SPARC_GOT10"},
    {14,4,"R_SPARC_GOT13"},
    {15,4,"R_SPARC_GOT22"},
    {16,4,"R_SPARC_PC10"},
    {17,4,"R_SPARC_PC22"},
    {18,4,"R_SPARC_WPLT30"},
    {19,0,"R_SPARC_COPY"},
    {20,4,"R_SPARC_GLOB_DAT"},
    {21,4,"R_SPARC_JMP_SLOT"},
    {22,4,"R_SPARC_RELATIVE"},
    {23,4,"R_SPARC_UA32"},
    {24,4,"R_SPARC_PLT32"},
    {25,0,"R_SPARC_HIPLT22"},
    {26,0,"R_SPARC_LOPLT10"},
    {27,0,"R_SPARC_PCPLT32"},
    {28,0,"R_SPARC_PCPLT22"},
    {29,0,"R_SPARC_PCPLT10"},
    {30,4,"R_SPARC_10"},
    {31,4,"R_SPARC_11"},
    {32,8,"R_SPARC_64"},
    {33,4,"R_SPARC_OLO10"},
    {34,4,"R_SPARC_HH22"},
    {35,4,"R_SPARC_HM10"},
    {36,4,"R_SPARC_LM22"},
    {37,4,"R_SPARC_PC_HH22"},
    {38,4,"R_SPARC_PC_HM10"},
    {39,4,"R_SPARC_PC_LM22"},
    {40,4,"R_SPARC_WDISP16"},
    {41,4,"R_SPARC_WDISP19"},
    {42,0,"R_SPARC_UNUSED_42"},
    {43,4,"R_SPARC_7"},
    {44,4,"R_SPARC_5"},
    {45,4,"R_SPARC_6"},
    {46,8,"R_SPARC_DISP64"},
    {47,8,"R_SPARC_PLT64"},
    {48,8,"R_SPARC_HIX22"},
    {49,8,"R_SPARC_LOX10"},
    {50,4,"R_SPARC_H44"},
    {51,4,"R_SPARC_M44"},
    {52,4,"R_SPARC_L44"},
    {53,8,"R_SPARC_REGISTER"},
    {54,8,"R_SPARC_UA64"},
    {55,2,"R_SPARC_UA16"},
    {56,4,"R_SPARC_TLS_GD_HI22"},
    {57,4,"R_SPARC_TLS_GD_LO10"},
    {58,0,"R_SPARC_TLS_GD_ADD"},
    {59,4,"R_SPARC_TLS_GD_CALL"},
    {60,4,"R_SPARC_TLS_LDM_HI22"},
    {61,4,"R_SPARC_TLS_LDM_LO10"},
    {62,0,"R_SPARC_TLS_LDM_ADD"},
    {63,4,"R_SPARC_TLS_LDM_CALL"},
    {64,4,"R_SPARC_TLS_LDO_HIX22"},
    {65,4,"R_SPARC_TLS_LDO_LOX10"},
    {66,0,"R_SPARC_TLS_LDO_ADD"},
    {67,4,"R_SPARC_TLS_IE_HI22"},
    {68,4,"R_SPARC_TLS_IE_LO10"},
    {69,0,"R_SPARC_TLS_IE_LD"},
    {70,0,"R_SPARC_TLS_IE_LDX"},
    {71,0,"R_SPARC_TLS_IE_ADD"},
    {72,4,"R_SPARC_TLS_LE_HIX22"},
    {73,4,"R_SPARC_TLS_LE_LOX10"},
    {74,0,"R_SPARC_TLS_DTPMOD32"},
    {75,0,"R_SPARC_TLS_DTPMOD64"},
    {76,4,"R_SPARC_TLS_DTPOFF32"},
    {77,8,"R_SPARC_TLS_DTPOFF64"},
    {78,0,"R_SPARC_TLS_TPOFF32"},
    {79,0,"R_SPARC_TLS_TPOFF64"},
    {80,4,"R_SPARC_GOTDATA_HIX22"},
    {81,4,"R_SPARC_GOTDATA_LOX10"},
    {82,4,"R_SPARC_GOTDATA_OP_HIX22"},
    {83,4,"R_SPARC_GOTDATA_OP_LOX10"},
    {84,0,"R_SPARC_GOTDATA_OP"},
    {85,4,"R_SPARC_H34"},
    {86,4,"R_SPARC_SIZE32"},
    {87,8,"R_SPARC_SIZE64"},
    {88,4,"R_SPARC_WDISP10"},
    {248,0,"R_SPARC_JMP_IREL"},
    {249,4,"R_SPARC_IRELATIVE"},
    {250,4,"R_SPARC_GNU_VTINHERIT"},
    {251,4,"R_SPARC_GNU_VTENTRY"},
    {252,4,"R_SPARC_REV32"},
};

const char *helf_sparc_relocation_type_name_get(uint32_t type)
{
    return helf_arch_relocation_type_name_get(helf_sparc_relocation_type_names,sizeof(helf_sparc_relocation_type_names)/sizeof(helf_sparc_relocation_type_names[0]),type);
}

uint8_t helf_sparc_relocation_size_get(uint32_t type)
{
    return helf_arch_relocation_type_size_get(helf_sparc_relocation_type_names,sizeof(helf_sparc_relocation_type_names)/sizeof(helf_sparc_relocation_type_names[0]),type);
}

const char *helf_sparc_section_type_name_get(uint32_t type)
{
    (void)type;
    return NULL;
}

bool helf_sparc_relocation_address_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_relocation_address_info_t *info)
{
    if(!helf_sparc_is_sparc(input_file) || relocation==NULL)
    {
        return false;
    }
    uint8_t size=helf_sparc_relocation_size_get(relocation->type);
    if(size==0)
    {
        return false;
    }
    return helf_arch_relocation_address_info_get(input_file,relocation,size,info);
}

size_t helf_sparc_relocation_data_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,void *buffer,size_t buffer_length)
{
    if(!helf_sparc_is_sparc(input_file) || relocation==NULL)
    {
        return 0;
    }
    uint8_t size=helf_sparc_relocation_size_get(relocation->type);
    if(size==0)
    {
        return 0;
    }
    return helf_arch_relocation_data_get(input_file,relocation,size,buffer,buffer_length);
}

bool helf_sparc_is_shared_object(helf_file_input_t *input_file)
{
    return helf_arch_is_shared_object(input_file,helf_sparc_machines,sizeof(helf_sparc_machines)/sizeof(helf_sparc_machines[0]),HELF_ELFCLASS_32);
}

bool helf_sparc_is_relocatable(helf_file_input_t *input_file)
{
    return helf_arch_is_relocatable(input_file,helf_sparc_machines,sizeof(helf_sparc_machines)/sizeof(helf_sparc_machines[0]),HELF_ELFCLASS_32);
}

size_t helf_sparc_dynamic_count_get(helf_file_input_t *input_file)
{
    if(!helf_sparc_is_sparc(input_file))
    {
        return 0;
    }
    return helf_arch_dynamic_count_get(input_file);
}

bool helf_sparc_dynamic_info_get(helf_file_input_t *input_file,size_t index,helf_arch_dynamic_info_t *info)
{
    if(!helf_sparc_is_sparc(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_info_get(input_file,index,info);
}

bool helf_sparc_dynamic_string_get(helf_file_input_t *input_file,uint64_t offset,char *buffer,size_t buffer_length)
{
    if(!helf_sparc_is_sparc(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_string_get(input_file,offset,buffer,buffer_length);
}

/* helf_relocation_howto_begin */
static const helf_arch_relocation_howto_t helf_sparc_reloc_howto[]=
{
    {0x00000000u,0,0,0,0,0x0042,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000001u,1,8,0,0,0x0002,0x0000000000000000ULL,0x00000000000000FFULL},
    {0x00000002u,2,16,0,0,0x0002,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000003u,4,32,0,0,0x0002,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000004u,1,8,0,0,0x0003,0x0000000000000000ULL,0x00000000000000FFULL},
    {0x00000005u,2,16,0,0,0x0003,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000006u,4,32,0,0,0x0003,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000007u,4,30,2,0,0x0003,0x0000000000000000ULL,0x000000003FFFFFFFULL},
    {0x00000008u,4,22,2,0,0x0003,0x0000000000000000ULL,0x00000000003FFFFFULL},
    {0x00000009u,4,22,10,0,0x0002,0x0000000000000000ULL,0x00000000003FFFFFULL},
    {0x0000000Au,4,22,0,0,0x0002,0x0000000000000000ULL,0x00000000003FFFFFULL},
    {0x0000000Bu,4,13,0,0,0x0002,0x0000000000000000ULL,0x0000000000001FFFULL},
    {0x0000000Cu,4,10,0,0,0x0002,0x0000000000000000ULL,0x00000000000003FFULL},
    {0x0000000Du,4,10,0,0,0x0202,0x0000000000000000ULL,0x00000000000003FFULL},
    {0x0000000Eu,4,13,0,0,0x0202,0x0000000000000000ULL,0x0000000000001FFFULL},
    {0x0000000Fu,4,22,10,0,0x0202,0x0000000000000000ULL,0x00000000003FFFFFULL},
    {0x00000010u,4,10,0,0,0x0003,0x0000000000000000ULL,0x00000000000003FFULL},
    {0x00000011u,4,22,10,0,0x0003,0x0000000000000000ULL,0x00000000003FFFFFULL},
    {0x00000012u,4,30,2,0,0x0003,0x0000000000000000ULL,0x000000003FFFFFFFULL},
    {0x00000013u,0,0,0,0,0x0182,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000014u,4,0,0,0,0x0102,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000015u,4,0,0,0,0x0142,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000016u,4,0,0,0,0x0082,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000017u,4,32,0,0,0x0002,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000018u,4,32,0,0,0x0002,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000019u,0,0,0,0,0x0012,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Au,0,0,0,0,0x0012,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Bu,0,0,0,0,0x0012,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Cu,0,0,0,0,0x0012,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Du,0,0,0,0,0x0012,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Eu,4,10,0,0,0x0002,0x0000000000000000ULL,0x00000000000003FFULL},
    {0x0000001Fu,4,11,0,0,0x0002,0x0000000000000000ULL,0x00000000000007FFULL},
    {0x00000020u,8,64,0,0,0x0002,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000021u,4,13,0,0,0x0012,0x0000000000000000ULL,0x0000000000001FFFULL},
    {0x00000022u,4,22,42,0,0x0002,0x0000000000000000ULL,0x00000000003FFFFFULL},
    {0x00000023u,4,10,32,0,0x0002,0x0000000000000000ULL,0x00000000000003FFULL},
    {0x00000024u,4,22,10,0,0x0002,0x0000000000000000ULL,0x00000000003FFFFFULL},
    {0x00000025u,4,22,42,0,0x0003,0x0000000000000000ULL,0x00000000003FFFFFULL},
    {0x00000026u,4,10,32,0,0x0003,0x0000000000000000ULL,0x00000000000003FFULL},
    {0x00000027u,4,22,10,0,0x0003,0x0000000000000000ULL,0x00000000003FFFFFULL},
    {0x00000028u,4,16,2,0,0x0013,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000029u,4,19,2,0,0x0003,0x0000000000000000ULL,0x000000000007FFFFULL},
    {0x0000002Au,0,0,0,0,0x0002,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000002Bu,4,7,0,0,0x0002,0x0000000000000000ULL,0x000000000000007FULL},
    {0x0000002Cu,4,5,0,0,0x0002,0x0000000000000000ULL,0x000000000000001FULL},
    {0x0000002Du,4,6,0,0,0x0002,0x0000000000000000ULL,0x000000000000003FULL},
    {0x0000002Eu,8,64,0,0,0x0003,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x0000002Fu,8,64,0,0,0x0002,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000030u,8,0,0,0,0x0010,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000031u,8,0,0,0,0x0010,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000032u,4,22,22,0,0x0000,0x0000000000000000ULL,0x00000000003FFFFFULL},
    {0x00000033u,4,10,12,0,0x0000,0x0000000000000000ULL,0x00000000000003FFULL},
    {0x00000034u,4,13,0,0,0x0000,0x0000000000000000ULL,0x0000000000000FFFULL},
    {0x00000035u,8,0,0,0,0x0010,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000036u,8,64,0,0,0x0002,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000037u,2,16,0,0,0x0002,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000038u,4,22,10,0,0x01C2,0x0000000000000000ULL,0x00000000003FFFFFULL},
    {0x00000039u,4,10,0,0,0x01C2,0x0000000000000000ULL,0x00000000000003FFULL},
    {0x0000003Au,0,0,0,0,0x01C2,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000003Bu,4,30,2,0,0x01C3,0x0000000000000000ULL,0x000000003FFFFFFFULL},
    {0x0000003Cu,4,22,10,0,0x01C2,0x0000000000000000ULL,0x00000000003FFFFFULL},
    {0x0000003Du,4,10,0,0,0x01C2,0x0000000000000000ULL,0x00000000000003FFULL},
    {0x0000003Eu,0,0,0,0,0x01C2,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000003Fu,4,30,2,0,0x01C3,0x0000000000000000ULL,0x000000003FFFFFFFULL},
    {0x00000040u,4,0,0,0,0x01D0,0x0000000000000000ULL,0x00000000003FFFFFULL},
    {0x00000041u,4,0,0,0,0x01D0,0x0000000000000000ULL,0x00000000000003FFULL},
    {0x00000042u,0,0,0,0,0x01C2,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000043u,4,22,10,0,0x01C2,0x0000000000000000ULL,0x00000000003FFFFFULL},
    {0x00000044u,4,10,0,0,0x01C2,0x0000000000000000ULL,0x00000000000003FFULL},
    {0x00000045u,0,0,0,0,0x01C2,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000046u,0,0,0,0,0x01C2,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000047u,0,0,0,0,0x01C2,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000048u,4,0,0,0,0x01D0,0x0000000000000000ULL,0x00000000003FFFFFULL},
    {0x00000049u,4,0,0,0,0x01D0,0x0000000000000000ULL,0x00000000000003FFULL},
    {0x0000004Au,0,0,0,0,0x01C2,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000004Bu,0,0,0,0,0x01C2,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000004Cu,4,32,0,0,0x01C2,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x0000004Du,8,64,0,0,0x01C2,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x0000004Eu,0,0,0,0,0x01C2,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000004Fu,0,0,0,0,0x01C2,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000050u,4,0,0,0,0x0210,0x0000000000000000ULL,0x00000000003FFFFFULL},
    {0x00000051u,4,0,0,0,0x0210,0x0000000000000000ULL,0x00000000000003FFULL},
    {0x00000052u,4,0,0,0,0x0210,0x0000000000000000ULL,0x00000000003FFFFFULL},
    {0x00000053u,4,0,0,0,0x0210,0x0000000000000000ULL,0x00000000000003FFULL},
    {0x00000054u,0,0,0,0,0x0202,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000055u,4,22,12,0,0x0000,0x0000000000000000ULL,0x00000000003FFFFFULL},
    {0x00000056u,4,32,0,0,0x0022,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000057u,8,64,0,0,0x0022,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000058u,4,10,2,0,0x0013,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000F8u,0,0,0,0,0x0002,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000F9u,4,0,0,0,0x0082,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x000000FAu,4,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000FBu,4,0,0,0,0x0010,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000FCu,4,32,0,0,0x0002,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
};

bool helf_sparc_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto)
{
    if(howto==NULL)
    {
        return false;
    }
    size_t count=sizeof(helf_sparc_reloc_howto)/sizeof(helf_sparc_reloc_howto[0]);
    if(count==0)
    {
        return false;
    }
    for(size_t i=0;i<count;i++)
    {
        if(helf_sparc_reloc_howto[i].type==type)
        {
            *howto=helf_sparc_reloc_howto[i];
            return true;
        }
    }
    return false;
}
/* helf_relocation_howto_end */

