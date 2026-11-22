/***************************************************************
 * Name:      helf_arch_ia64.c
 * Purpose:   实现helf_arch_ia64接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "helf_arch_ia64.h"

static const uint16_t helf_ia64_machines[]=
{
    HELF_EM_IA_64,
};

bool helf_ia64_is_ia64(helf_file_input_t *input_file)
{
    return helf_arch_is(input_file,helf_ia64_machines,sizeof(helf_ia64_machines)/sizeof(helf_ia64_machines[0]),HELF_ELFCLASS_64);
}

bool helf_ia64_is_big_endian(helf_file_input_t *input_file)
{
    if(!helf_ia64_is_ia64(input_file))
    {
        return false;
    }
    return helf_file_input_is_big_endian(input_file);
}

bool helf_ia64_is_executable(helf_file_input_t *input_file)
{
    return helf_arch_is_executable(input_file,helf_ia64_machines,sizeof(helf_ia64_machines)/sizeof(helf_ia64_machines[0]),HELF_ELFCLASS_64);
}

bool helf_ia64_entry_get(helf_file_input_t *input_file,uint64_t *entry)
{
    if(entry==NULL)
    {
        return false;
    }
    if(!helf_ia64_is_ia64(input_file))
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

size_t helf_ia64_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_ia64_is_ia64(input_file))
    {
        return 0;
    }
    return helf_arch_section_count_get(input_file);
}

bool helf_ia64_section_info_get(helf_file_input_t *input_file,size_t index,helf_ia64_section_info_t *info)
{
    if(!helf_ia64_is_ia64(input_file))
    {
        return false;
    }
    return helf_arch_section_info_get(input_file,index,info);
}

bool helf_ia64_is_rx_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ia64_is_ia64(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_section(index,input_file);
}

intptr_t helf_ia64_rx_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ia64_is_rx_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rx_section_offset_get(index,input_file);
}

size_t helf_ia64_rx_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_ia64_is_ia64(input_file))
    {
        return 0;
    }
    return helf_arch_rx_section_size_get(input_file);
}

bool helf_ia64_is_ro_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ia64_is_ia64(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_section(index,input_file);
}

intptr_t helf_ia64_ro_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ia64_is_ro_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_ro_section_offset_get(index,input_file);
}

size_t helf_ia64_ro_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_ia64_is_ia64(input_file))
    {
        return 0;
    }
    return helf_arch_ro_section_size_get(input_file);
}

bool helf_ia64_is_rw_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ia64_is_ia64(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_section(index,input_file);
}

intptr_t helf_ia64_rw_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ia64_is_rw_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rw_section_offset_get(index,input_file);
}

size_t helf_ia64_rw_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_ia64_is_ia64(input_file))
    {
        return 0;
    }
    return helf_arch_rw_section_size_get(input_file);
}

bool helf_ia64_is_bss_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ia64_is_ia64(input_file))
    {
        return false;
    }
    return helf_arch_is_bss_section(index,input_file);
}

size_t helf_ia64_program_header_count_get(helf_file_input_t *input_file)
{
    if(!helf_ia64_is_ia64(input_file))
    {
        return 0;
    }
    return helf_arch_program_header_count_get(input_file);
}

bool helf_ia64_program_header_get(helf_file_input_t *input_file,size_t index,helf_elf64_program_header_t *programheader)
{
    if(programheader==NULL)
    {
        return false;
    }
    if(!helf_ia64_is_ia64(input_file))
    {
        return false;
    }
    if(index>=helf_arch_program_header_count_get(input_file))
    {
        return false;
    }
    return helf_file_input_64_bits_program_header_get(input_file,index,programheader);
}

size_t helf_ia64_segment_count_get(helf_file_input_t *input_file)
{
    return helf_ia64_program_header_count_get(input_file);
}

bool helf_ia64_segment_info_get(helf_file_input_t *input_file,size_t index,helf_ia64_segment_info_t *info)
{
    if(!helf_ia64_is_ia64(input_file))
    {
        return false;
    }
    return helf_arch_segment_info_get(input_file,index,info);
}

bool helf_ia64_is_load_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ia64_is_ia64(input_file))
    {
        return false;
    }
    return helf_arch_is_load_segment(index,input_file);
}

bool helf_ia64_is_rx_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ia64_is_ia64(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_segment(index,input_file);
}

bool helf_ia64_is_ro_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ia64_is_ia64(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_segment(index,input_file);
}

bool helf_ia64_is_rw_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_ia64_is_ia64(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_segment(index,input_file);
}

size_t helf_ia64_load_segment_count_get(helf_file_input_t *input_file)
{
    if(!helf_ia64_is_ia64(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_count_get(input_file);
}

bool helf_ia64_load_segment_info_get(helf_file_input_t *input_file,size_t index,helf_ia64_segment_info_t *info)
{
    if(!helf_ia64_is_ia64(input_file))
    {
        return false;
    }
    return helf_arch_load_segment_info_get(input_file,index,info);
}

size_t helf_ia64_load_segment_total_filesz_get(helf_file_input_t *input_file)
{
    if(!helf_ia64_is_ia64(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_filesz_get(input_file);
}

size_t helf_ia64_load_segment_total_memsz_get(helf_file_input_t *input_file)
{
    if(!helf_ia64_is_ia64(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_memsz_get(input_file);
}

size_t helf_ia64_relocation_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_ia64_is_ia64(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_section_count_get(input_file);
}

bool helf_ia64_relocation_section_info_get(helf_file_input_t *input_file,size_t index,helf_arch_relocation_section_info_t *info)
{
    if(!helf_ia64_is_ia64(input_file))
    {
        return false;
    }
    return helf_arch_relocation_section_info_get(input_file,index,info);
}

size_t helf_ia64_relocation_count_get(helf_file_input_t *input_file,size_t section_index)
{
    if(!helf_ia64_is_ia64(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_count_get(input_file,section_index);
}

bool helf_ia64_relocation_get(helf_file_input_t *input_file,size_t section_index,size_t index,helf_arch_relocation_info_t *info)
{
    if(!helf_ia64_is_ia64(input_file))
    {
        return false;
    }
    if(!helf_arch_relocation_get(input_file,section_index,index,info))
    {
        return false;
    }
    return true;
}

bool helf_ia64_relocation_symbol_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_symbol_info_t *info)
{
    if(!helf_ia64_is_ia64(input_file))
    {
        return false;
    }
    return helf_arch_relocation_symbol_info_get(input_file,relocation,info);
}

static const helf_arch_relocation_type_name_t helf_ia64_relocation_type_names[]=
{
    {0,0,"R_IA64_NONE"},
    {33,1,"R_IA64_IMM14"},
    {34,1,"R_IA64_IMM22"},
    {35,1,"R_IA64_IMM64"},
    {36,4,"R_IA64_DIR32MSB"},
    {37,4,"R_IA64_DIR32LSB"},
    {38,8,"R_IA64_DIR64MSB"},
    {39,8,"R_IA64_DIR64LSB"},
    {42,1,"R_IA64_GPREL22"},
    {43,1,"R_IA64_GPREL64I"},
    {44,4,"R_IA64_GPREL32MSB"},
    {45,4,"R_IA64_GPREL32LSB"},
    {46,8,"R_IA64_GPREL64MSB"},
    {47,8,"R_IA64_GPREL64LSB"},
    {50,1,"R_IA64_LTOFF22"},
    {51,1,"R_IA64_LTOFF64I"},
    {58,1,"R_IA64_PLTOFF22"},
    {59,1,"R_IA64_PLTOFF64I"},
    {62,8,"R_IA64_PLTOFF64MSB"},
    {63,8,"R_IA64_PLTOFF64LSB"},
    {67,1,"R_IA64_FPTR64I"},
    {68,4,"R_IA64_FPTR32MSB"},
    {69,4,"R_IA64_FPTR32LSB"},
    {70,8,"R_IA64_FPTR64MSB"},
    {71,8,"R_IA64_FPTR64LSB"},
    {72,1,"R_IA64_PCREL60B"},
    {73,1,"R_IA64_PCREL21B"},
    {74,1,"R_IA64_PCREL21M"},
    {75,1,"R_IA64_PCREL21F"},
    {76,4,"R_IA64_PCREL32MSB"},
    {77,4,"R_IA64_PCREL32LSB"},
    {78,8,"R_IA64_PCREL64MSB"},
    {79,8,"R_IA64_PCREL64LSB"},
    {82,1,"R_IA64_LTOFF_FPTR22"},
    {83,1,"R_IA64_LTOFF_FPTR64I"},
    {84,4,"R_IA64_LTOFF_FPTR32MSB"},
    {85,4,"R_IA64_LTOFF_FPTR32LSB"},
    {86,8,"R_IA64_LTOFF_FPTR64MSB"},
    {87,8,"R_IA64_LTOFF_FPTR64LSB"},
    {92,4,"R_IA64_SEGREL32MSB"},
    {93,4,"R_IA64_SEGREL32LSB"},
    {94,8,"R_IA64_SEGREL64MSB"},
    {95,8,"R_IA64_SEGREL64LSB"},
    {100,4,"R_IA64_SECREL32MSB"},
    {101,4,"R_IA64_SECREL32LSB"},
    {102,8,"R_IA64_SECREL64MSB"},
    {103,8,"R_IA64_SECREL64LSB"},
    {108,4,"R_IA64_REL32MSB"},
    {109,4,"R_IA64_REL32LSB"},
    {110,8,"R_IA64_REL64MSB"},
    {111,8,"R_IA64_REL64LSB"},
    {116,4,"R_IA64_LTV32MSB"},
    {117,4,"R_IA64_LTV32LSB"},
    {118,8,"R_IA64_LTV64MSB"},
    {119,8,"R_IA64_LTV64LSB"},
    {121,1,"R_IA64_PCREL21BI"},
    {122,1,"R_IA64_PCREL22"},
    {123,1,"R_IA64_PCREL64I"},
    {128,8,"R_IA64_IPLTMSB"},
    {129,8,"R_IA64_IPLTLSB"},
    {132,8,"R_IA64_COPY"},
    {134,1,"R_IA64_LTOFF22X"},
    {135,1,"R_IA64_LDXMOV"},
    {145,1,"R_IA64_TPREL14"},
    {146,1,"R_IA64_TPREL22"},
    {147,1,"R_IA64_TPREL64I"},
    {150,8,"R_IA64_TPREL64MSB"},
    {151,8,"R_IA64_TPREL64LSB"},
    {154,1,"R_IA64_LTOFF_TPREL22"},
    {166,8,"R_IA64_DTPMOD64MSB"},
    {167,8,"R_IA64_DTPMOD64LSB"},
    {170,1,"R_IA64_LTOFF_DTPMOD22"},
    {177,1,"R_IA64_DTPREL14"},
    {178,1,"R_IA64_DTPREL22"},
    {179,1,"R_IA64_DTPREL64I"},
    {180,4,"R_IA64_DTPREL32MSB"},
    {181,4,"R_IA64_DTPREL32LSB"},
    {182,8,"R_IA64_DTPREL64MSB"},
    {183,8,"R_IA64_DTPREL64LSB"},
    {186,1,"R_IA64_LTOFF_DTPREL22"},
    {1879048192,1,"R_IA64_VMS_DIR8"},
    {1879048193,0,"R_IA64_VMS_DIR16LSB"},
    {1879048194,0,"R_IA64_VMS_CALL_SIGNATURE"},
    {1879048195,0,"R_IA64_VMS_EXECLET_FUNC"},
    {1879048196,0,"R_IA64_VMS_EXECLET_DATA"},
    {1879048197,1,"R_IA64_VMS_FIX8"},
    {1879048198,2,"R_IA64_VMS_FIX16"},
    {1879048199,4,"R_IA64_VMS_FIX32"},
    {1879048200,8,"R_IA64_VMS_FIX64"},
    {1879048201,0,"R_IA64_VMS_FIXFD"},
    {1879048202,0,"R_IA64_VMS_ACC_LOAD"},
    {1879048203,0,"R_IA64_VMS_ACC_ADD"},
    {1879048204,0,"R_IA64_VMS_ACC_SUB"},
    {1879048205,0,"R_IA64_VMS_ACC_MUL"},
    {1879048206,0,"R_IA64_VMS_ACC_DIV"},
    {1879048207,0,"R_IA64_VMS_ACC_AND"},
    {1879048208,0,"R_IA64_VMS_ACC_IOR"},
    {1879048209,0,"R_IA64_VMS_ACC_EOR"},
    {1879048210,0,"R_IA64_VMS_ACC_ASH"},
    {1879048212,1,"R_IA64_VMS_ACC_STO8"},
    {1879048213,0,"R_IA64_VMS_ACC_STO16LSH"},
    {1879048214,0,"R_IA64_VMS_ACC_STO32LSH"},
    {1879048215,0,"R_IA64_VMS_ACC_STO64LSH"},
};

const char *helf_ia64_relocation_type_name_get(uint32_t type)
{
    return helf_arch_relocation_type_name_get(helf_ia64_relocation_type_names,sizeof(helf_ia64_relocation_type_names)/sizeof(helf_ia64_relocation_type_names[0]),type);
}

uint8_t helf_ia64_relocation_size_get(uint32_t type)
{
    return helf_arch_relocation_type_size_get(helf_ia64_relocation_type_names,sizeof(helf_ia64_relocation_type_names)/sizeof(helf_ia64_relocation_type_names[0]),type);
}

static const helf_arch_section_type_name_t helf_ia64_section_type_names[]=
{
    {1879048192,"SHT_IA_64_EXT"},
    {1879048193,"SHT_IA_64_UNWIND"},
    {2013265920,"SHT_IA_64_LOPSREG"},
    {2030043135,"SHT_IA_64_HIPSREG"},
    {2030043136,"SHT_IA_64_PRIORITY_INIT"},
    {1610612740,"SHT_IA_64_HP_OPT_ANOT"},
    {1610612736,"SHT_IA_64_VMS_TRACE"},
    {1610612737,"SHT_IA_64_VMS_TIE_SIGNATURES"},
    {1610612738,"SHT_IA_64_VMS_DEBUG"},
    {1610612739,"SHT_IA_64_VMS_DEBUG_STR"},
    {1610612741,"SHT_IA_64_VMS_SYMBOL_VECTOR"},
    {1610612742,"SHT_IA_64_VMS_FIXUP"},
    {1610612743,"SHT_IA_64_VMS_DISPLAY_NAME_INFO"},
};

const char *helf_ia64_section_type_name_get(uint32_t type)
{
    return helf_arch_section_type_name_get(helf_ia64_section_type_names,sizeof(helf_ia64_section_type_names)/sizeof(helf_ia64_section_type_names[0]),type);
}

bool helf_ia64_relocation_address_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_relocation_address_info_t *info)
{
    if(!helf_ia64_is_ia64(input_file) || relocation==NULL)
    {
        return false;
    }
    uint8_t size=helf_ia64_relocation_size_get(relocation->type);
    if(size==0)
    {
        return false;
    }
    return helf_arch_relocation_address_info_get(input_file,relocation,size,info);
}

size_t helf_ia64_relocation_data_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,void *buffer,size_t buffer_length)
{
    if(!helf_ia64_is_ia64(input_file) || relocation==NULL)
    {
        return 0;
    }
    uint8_t size=helf_ia64_relocation_size_get(relocation->type);
    if(size==0)
    {
        return 0;
    }
    return helf_arch_relocation_data_get(input_file,relocation,size,buffer,buffer_length);
}

bool helf_ia64_is_shared_object(helf_file_input_t *input_file)
{
    return helf_arch_is_shared_object(input_file,helf_ia64_machines,sizeof(helf_ia64_machines)/sizeof(helf_ia64_machines[0]),HELF_ELFCLASS_64);
}

bool helf_ia64_is_relocatable(helf_file_input_t *input_file)
{
    return helf_arch_is_relocatable(input_file,helf_ia64_machines,sizeof(helf_ia64_machines)/sizeof(helf_ia64_machines[0]),HELF_ELFCLASS_64);
}

size_t helf_ia64_dynamic_count_get(helf_file_input_t *input_file)
{
    if(!helf_ia64_is_ia64(input_file))
    {
        return 0;
    }
    return helf_arch_dynamic_count_get(input_file);
}

bool helf_ia64_dynamic_info_get(helf_file_input_t *input_file,size_t index,helf_arch_dynamic_info_t *info)
{
    if(!helf_ia64_is_ia64(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_info_get(input_file,index,info);
}

bool helf_ia64_dynamic_string_get(helf_file_input_t *input_file,uint64_t offset,char *buffer,size_t buffer_length)
{
    if(!helf_ia64_is_ia64(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_string_get(input_file,offset,buffer,buffer_length);
}

/* helf_relocation_howto_begin */
static const helf_arch_relocation_howto_t helf_ia64_reloc_howto[]=
{
    {0x00000000u,0,0,0,0,0x0050,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000021u,1,0,0,0,0x0010,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000022u,1,0,0,0,0x0010,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000023u,1,0,0,0,0x0010,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000024u,4,0,0,0,0x0010,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000025u,4,0,0,0,0x0010,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000026u,8,0,0,0,0x0010,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000027u,8,0,0,0,0x0010,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x0000002Au,1,0,0,0,0x0010,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x0000002Bu,1,0,0,0,0x0010,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x0000002Cu,4,0,0,0,0x0010,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x0000002Du,4,0,0,0,0x0010,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x0000002Eu,8,0,0,0,0x0010,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x0000002Fu,8,0,0,0,0x0010,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000032u,1,0,0,0,0x0010,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000033u,1,0,0,0,0x0010,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x0000003Au,1,0,0,0,0x0010,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x0000003Bu,1,0,0,0,0x0010,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x0000003Eu,8,0,0,0,0x0010,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x0000003Fu,8,0,0,0,0x0010,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000043u,1,0,0,0,0x0010,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000044u,4,0,0,0,0x0010,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000045u,4,0,0,0,0x0010,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000046u,8,0,0,0,0x0010,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000047u,8,0,0,0,0x0010,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000048u,1,0,0,0,0x0011,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000049u,1,0,0,0,0x0011,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x0000004Au,1,0,0,0,0x0011,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x0000004Bu,1,0,0,0,0x0011,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x0000004Cu,4,0,0,0,0x0011,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x0000004Du,4,0,0,0,0x0011,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x0000004Eu,8,0,0,0,0x0011,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x0000004Fu,8,0,0,0,0x0011,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000052u,1,0,0,0,0x0010,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000053u,1,0,0,0,0x0010,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000054u,4,0,0,0,0x0010,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000055u,4,0,0,0,0x0010,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000056u,8,0,0,0,0x0010,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000057u,8,0,0,0,0x0010,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x0000005Cu,4,0,0,0,0x0010,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x0000005Du,4,0,0,0,0x0010,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x0000005Eu,8,0,0,0,0x0010,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x0000005Fu,8,0,0,0,0x0010,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000064u,4,0,0,0,0x0010,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000065u,4,0,0,0,0x0010,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000066u,8,0,0,0,0x0010,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000067u,8,0,0,0,0x0010,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x0000006Cu,4,0,0,0,0x0010,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x0000006Du,4,0,0,0,0x0010,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x0000006Eu,8,0,0,0,0x0010,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x0000006Fu,8,0,0,0,0x0010,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000074u,4,0,0,0,0x0010,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000075u,4,0,0,0,0x0010,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000076u,8,0,0,0,0x0010,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000077u,8,0,0,0,0x0010,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000079u,1,0,0,0,0x0011,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x0000007Au,1,0,0,0,0x0011,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x0000007Bu,1,0,0,0,0x0011,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000080u,8,0,0,0,0x0010,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000081u,8,0,0,0,0x0010,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000084u,8,0,0,0,0x0190,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000086u,1,0,0,0,0x0010,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000087u,1,0,0,0,0x0010,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000091u,1,0,0,0,0x01D0,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000092u,1,0,0,0,0x01D0,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000093u,1,0,0,0,0x01D0,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000096u,8,0,0,0,0x01D0,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000097u,8,0,0,0,0x01D0,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x0000009Au,1,0,0,0,0x01D0,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x000000A6u,8,0,0,0,0x01D0,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x000000A7u,8,0,0,0,0x01D0,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x000000AAu,1,0,0,0,0x01D0,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x000000B1u,1,0,0,0,0x01D0,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x000000B2u,1,0,0,0,0x01D0,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x000000B3u,1,0,0,0,0x01D0,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x000000B4u,4,0,0,0,0x01D0,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x000000B5u,4,0,0,0,0x01D0,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x000000B6u,8,0,0,0,0x01D0,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x000000B7u,8,0,0,0,0x01D0,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x000000BAu,1,0,0,0,0x01D0,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x70000000u,1,8,0,0,0x0000,0x0000000000000000ULL,0x00000000000000FFULL},
    {0x70000001u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x70000002u,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x70000003u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x70000004u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x70000005u,1,8,0,0,0x0000,0x0000000000000000ULL,0x00000000000000FFULL},
    {0x70000006u,2,16,0,0,0x0000,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x70000007u,4,32,0,0,0x0000,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x70000008u,8,64,0,0,0x0000,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x70000009u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x7000000Au,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x7000000Bu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x7000000Cu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x7000000Du,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x7000000Eu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x7000000Fu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x70000010u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x70000011u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x70000012u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x70000014u,1,8,0,0,0x0000,0x0000000000000000ULL,0x00000000000000FFULL},
    {0x70000015u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x70000016u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x70000017u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
};

bool helf_ia64_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto)
{
    if(howto==NULL)
    {
        return false;
    }
    size_t count=sizeof(helf_ia64_reloc_howto)/sizeof(helf_ia64_reloc_howto[0]);
    if(count==0)
    {
        return false;
    }
    for(size_t i=0;i<count;i++)
    {
        if(helf_ia64_reloc_howto[i].type==type)
        {
            *howto=helf_ia64_reloc_howto[i];
            return true;
        }
    }
    return false;
}
/* helf_relocation_howto_end */


