/***************************************************************
 * Name:      helf_arch_mips64.c
 * Purpose:   实现helf_arch_mips64接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "helf_arch_mips64.h"

static const uint16_t helf_mips64_machines[]=
{
    HELF_EM_MIPS,
};

bool helf_mips64_is_mips64(helf_file_input_t *input_file)
{
    return helf_arch_is(input_file,helf_mips64_machines,sizeof(helf_mips64_machines)/sizeof(helf_mips64_machines[0]),HELF_ELFCLASS_64);
}

bool helf_mips64_is_big_endian(helf_file_input_t *input_file)
{
    if(!helf_mips64_is_mips64(input_file))
    {
        return false;
    }
    return helf_file_input_is_big_endian(input_file);
}

bool helf_mips64_is_executable(helf_file_input_t *input_file)
{
    return helf_arch_is_executable(input_file,helf_mips64_machines,sizeof(helf_mips64_machines)/sizeof(helf_mips64_machines[0]),HELF_ELFCLASS_64);
}

bool helf_mips64_entry_get(helf_file_input_t *input_file,uint64_t *entry)
{
    if(entry==NULL)
    {
        return false;
    }
    if(!helf_mips64_is_mips64(input_file))
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

size_t helf_mips64_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_mips64_is_mips64(input_file))
    {
        return 0;
    }
    return helf_arch_section_count_get(input_file);
}

bool helf_mips64_section_info_get(helf_file_input_t *input_file,size_t index,helf_mips64_section_info_t *info)
{
    if(!helf_mips64_is_mips64(input_file))
    {
        return false;
    }
    return helf_arch_section_info_get(input_file,index,info);
}

bool helf_mips64_is_rx_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_mips64_is_mips64(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_section(index,input_file);
}

intptr_t helf_mips64_rx_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_mips64_is_rx_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rx_section_offset_get(index,input_file);
}

size_t helf_mips64_rx_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_mips64_is_mips64(input_file))
    {
        return 0;
    }
    return helf_arch_rx_section_size_get(input_file);
}

bool helf_mips64_is_ro_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_mips64_is_mips64(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_section(index,input_file);
}

intptr_t helf_mips64_ro_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_mips64_is_ro_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_ro_section_offset_get(index,input_file);
}

size_t helf_mips64_ro_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_mips64_is_mips64(input_file))
    {
        return 0;
    }
    return helf_arch_ro_section_size_get(input_file);
}

bool helf_mips64_is_rw_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_mips64_is_mips64(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_section(index,input_file);
}

intptr_t helf_mips64_rw_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_mips64_is_rw_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rw_section_offset_get(index,input_file);
}

size_t helf_mips64_rw_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_mips64_is_mips64(input_file))
    {
        return 0;
    }
    return helf_arch_rw_section_size_get(input_file);
}

bool helf_mips64_is_bss_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_mips64_is_mips64(input_file))
    {
        return false;
    }
    return helf_arch_is_bss_section(index,input_file);
}

size_t helf_mips64_program_header_count_get(helf_file_input_t *input_file)
{
    if(!helf_mips64_is_mips64(input_file))
    {
        return 0;
    }
    return helf_arch_program_header_count_get(input_file);
}

bool helf_mips64_program_header_get(helf_file_input_t *input_file,size_t index,helf_elf64_program_header_t *programheader)
{
    if(programheader==NULL)
    {
        return false;
    }
    if(!helf_mips64_is_mips64(input_file))
    {
        return false;
    }
    if(index>=helf_arch_program_header_count_get(input_file))
    {
        return false;
    }
    return helf_file_input_64_bits_program_header_get(input_file,index,programheader);
}

size_t helf_mips64_segment_count_get(helf_file_input_t *input_file)
{
    return helf_mips64_program_header_count_get(input_file);
}

bool helf_mips64_segment_info_get(helf_file_input_t *input_file,size_t index,helf_mips64_segment_info_t *info)
{
    if(!helf_mips64_is_mips64(input_file))
    {
        return false;
    }
    return helf_arch_segment_info_get(input_file,index,info);
}

bool helf_mips64_is_load_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_mips64_is_mips64(input_file))
    {
        return false;
    }
    return helf_arch_is_load_segment(index,input_file);
}

bool helf_mips64_is_rx_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_mips64_is_mips64(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_segment(index,input_file);
}

bool helf_mips64_is_ro_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_mips64_is_mips64(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_segment(index,input_file);
}

bool helf_mips64_is_rw_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_mips64_is_mips64(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_segment(index,input_file);
}

size_t helf_mips64_load_segment_count_get(helf_file_input_t *input_file)
{
    if(!helf_mips64_is_mips64(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_count_get(input_file);
}

bool helf_mips64_load_segment_info_get(helf_file_input_t *input_file,size_t index,helf_mips64_segment_info_t *info)
{
    if(!helf_mips64_is_mips64(input_file))
    {
        return false;
    }
    return helf_arch_load_segment_info_get(input_file,index,info);
}

size_t helf_mips64_load_segment_total_filesz_get(helf_file_input_t *input_file)
{
    if(!helf_mips64_is_mips64(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_filesz_get(input_file);
}

size_t helf_mips64_load_segment_total_memsz_get(helf_file_input_t *input_file)
{
    if(!helf_mips64_is_mips64(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_memsz_get(input_file);
}

size_t helf_mips64_relocation_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_mips64_is_mips64(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_section_count_get(input_file);
}

bool helf_mips64_relocation_section_info_get(helf_file_input_t *input_file,size_t index,helf_arch_relocation_section_info_t *info)
{
    if(!helf_mips64_is_mips64(input_file))
    {
        return false;
    }
    return helf_arch_relocation_section_info_get(input_file,index,info);
}

size_t helf_mips64_relocation_count_get(helf_file_input_t *input_file,size_t section_index)
{
    if(!helf_mips64_is_mips64(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_count_get(input_file,section_index);
}

bool helf_mips64_relocation_get(helf_file_input_t *input_file,size_t section_index,size_t index,helf_arch_relocation_info_t *info)
{
    if(!helf_mips64_is_mips64(input_file))
    {
        return false;
    }
    if(!helf_arch_relocation_get(input_file,section_index,index,info))
    {
        return false;
    }
    if(helf_file_input_is_64_bits(input_file))
    {
        info->special_symbol=(uint32_t)((info->info >> 24) & 0xFF);
        info->type3=(uint32_t)((info->info >> 16) & 0xFF);
        info->type2=(uint32_t)((info->info >> 8) & 0xFF);
        info->type=(uint32_t)(info->info & 0xFF);
    }
    return true;
}

bool helf_mips64_relocation_symbol_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_symbol_info_t *info)
{
    if(!helf_mips64_is_mips64(input_file))
    {
        return false;
    }
    return helf_arch_relocation_symbol_info_get(input_file,relocation,info);
}

static const helf_arch_relocation_type_name_t helf_mips64_relocation_type_names[]=
{
    {0,0,"R_MIPS_NONE"},
    {1,4,"R_MIPS_16"},
    {2,4,"R_MIPS_32"},
    {3,4,"R_MIPS_REL32"},
    {4,4,"R_MIPS_26"},
    {5,4,"R_MIPS_HI16"},
    {6,4,"R_MIPS_LO16"},
    {7,4,"R_MIPS_GPREL16"},
    {8,4,"R_MIPS_LITERAL"},
    {9,4,"R_MIPS_GOT16"},
    {10,4,"R_MIPS_PC16"},
    {11,4,"R_MIPS_CALL16"},
    {12,4,"R_MIPS_GPREL32"},
    {13,0,"R_MIPS_UNUSED1"},
    {14,0,"R_MIPS_UNUSED2"},
    {15,0,"R_MIPS_UNUSED3"},
    {16,4,"R_MIPS_SHIFT5"},
    {17,4,"R_MIPS_SHIFT6"},
    {18,8,"R_MIPS_64"},
    {19,4,"R_MIPS_GOT_DISP"},
    {20,4,"R_MIPS_GOT_PAGE"},
    {21,4,"R_MIPS_GOT_OFST"},
    {22,4,"R_MIPS_GOT_HI16"},
    {23,4,"R_MIPS_GOT_LO16"},
    {24,8,"R_MIPS_SUB"},
    {25,4,"R_MIPS_INSERT_A"},
    {26,4,"R_MIPS_INSERT_B"},
    {27,4,"R_MIPS_DELETE"},
    {28,4,"R_MIPS_HIGHER"},
    {29,4,"R_MIPS_HIGHEST"},
    {30,4,"R_MIPS_CALL_HI16"},
    {31,4,"R_MIPS_CALL_LO16"},
    {32,4,"R_MIPS_SCN_DISP"},
    {33,2,"R_MIPS_REL16"},
    {34,0,"R_MIPS_ADD_IMMEDIATE"},
    {35,0,"R_MIPS_PJUMP"},
    {36,4,"R_MIPS_RELGOT"},
    {37,4,"R_MIPS_JALR"},
    {38,4,"R_MIPS_TLS_DTPMOD32"},
    {39,4,"R_MIPS_TLS_DTPREL32"},
    {40,8,"R_MIPS_TLS_DTPMOD64"},
    {41,8,"R_MIPS_TLS_DTPREL64"},
    {42,4,"R_MIPS_TLS_GD"},
    {43,4,"R_MIPS_TLS_LDM"},
    {44,4,"R_MIPS_TLS_DTPREL_HI16"},
    {45,4,"R_MIPS_TLS_DTPREL_LO16"},
    {46,4,"R_MIPS_TLS_GOTTPREL"},
    {47,4,"R_MIPS_TLS_TPREL32"},
    {48,8,"R_MIPS_TLS_TPREL64"},
    {49,4,"R_MIPS_TLS_TPREL_HI16"},
    {50,4,"R_MIPS_TLS_TPREL_LO16"},
    {51,4,"R_MIPS_GLOB_DAT"},
    {60,4,"R_MIPS_PC21_S2"},
    {61,4,"R_MIPS_PC26_S2"},
    {62,4,"R_MIPS_PC18_S3"},
    {63,4,"R_MIPS_PC19_S2"},
    {64,4,"R_MIPS_PCHI16"},
    {65,4,"R_MIPS_PCLO16"},
    {100,4,"R_MIPS16_26"},
    {101,4,"R_MIPS16_GPREL"},
    {102,4,"R_MIPS16_GOT16"},
    {103,4,"R_MIPS16_CALL16"},
    {104,4,"R_MIPS16_HI16"},
    {105,4,"R_MIPS16_LO16"},
    {106,4,"R_MIPS16_TLS_GD"},
    {107,4,"R_MIPS16_TLS_LDM"},
    {108,4,"R_MIPS16_TLS_DTPREL_HI16"},
    {109,4,"R_MIPS16_TLS_DTPREL_LO16"},
    {110,4,"R_MIPS16_TLS_GOTTPREL"},
    {111,4,"R_MIPS16_TLS_TPREL_HI16"},
    {112,4,"R_MIPS16_TLS_TPREL_LO16"},
    {113,4,"R_MIPS16_PC16_S1"},
    {126,0,"R_MIPS_COPY"},
    {127,8,"R_MIPS_JUMP_SLOT"},
    {133,4,"R_MICROMIPS_26_S1"},
    {134,4,"R_MICROMIPS_HI16"},
    {135,4,"R_MICROMIPS_LO16"},
    {136,4,"R_MICROMIPS_GPREL16"},
    {137,4,"R_MICROMIPS_LITERAL"},
    {138,4,"R_MICROMIPS_GOT16"},
    {139,2,"R_MICROMIPS_PC7_S1"},
    {140,2,"R_MICROMIPS_PC10_S1"},
    {141,4,"R_MICROMIPS_PC16_S1"},
    {142,4,"R_MICROMIPS_CALL16"},
    {145,4,"R_MICROMIPS_GOT_DISP"},
    {146,4,"R_MICROMIPS_GOT_PAGE"},
    {147,4,"R_MICROMIPS_GOT_OFST"},
    {148,4,"R_MICROMIPS_GOT_HI16"},
    {149,4,"R_MICROMIPS_GOT_LO16"},
    {150,8,"R_MICROMIPS_SUB"},
    {151,4,"R_MICROMIPS_HIGHER"},
    {152,4,"R_MICROMIPS_HIGHEST"},
    {153,4,"R_MICROMIPS_CALL_HI16"},
    {154,4,"R_MICROMIPS_CALL_LO16"},
    {155,4,"R_MICROMIPS_SCN_DISP"},
    {156,4,"R_MICROMIPS_JALR"},
    {157,4,"R_MICROMIPS_HI0_LO16"},
    {162,4,"R_MICROMIPS_TLS_GD"},
    {163,4,"R_MICROMIPS_TLS_LDM"},
    {164,4,"R_MICROMIPS_TLS_DTPREL_HI16"},
    {165,4,"R_MICROMIPS_TLS_DTPREL_LO16"},
    {166,4,"R_MICROMIPS_TLS_GOTTPREL"},
    {169,4,"R_MICROMIPS_TLS_TPREL_HI16"},
    {170,4,"R_MICROMIPS_TLS_TPREL_LO16"},
    {172,2,"R_MICROMIPS_GPREL7_S2"},
    {173,4,"R_MICROMIPS_PC23_S2"},
    {248,4,"R_MIPS_PC32"},
    {249,4,"R_MIPS_EH"},
    {250,4,"R_MIPS_GNU_REL16_S2"},
    {253,4,"R_MIPS_GNU_VTINHERIT"},
    {254,4,"R_MIPS_GNU_VTENTRY"},
};

const char *helf_mips64_relocation_type_name_get(uint32_t type)
{
    return helf_arch_relocation_type_name_get(helf_mips64_relocation_type_names,sizeof(helf_mips64_relocation_type_names)/sizeof(helf_mips64_relocation_type_names[0]),type);
}

uint8_t helf_mips64_relocation_size_get(uint32_t type)
{
    return helf_arch_relocation_type_size_get(helf_mips64_relocation_type_names,sizeof(helf_mips64_relocation_type_names)/sizeof(helf_mips64_relocation_type_names[0]),type);
}

static const helf_arch_section_type_name_t helf_mips64_section_type_names[]=
{
    {1879048192,"SHT_MIPS_LIBLIST"},
    {1879048193,"SHT_MIPS_MSYM"},
    {1879048194,"SHT_MIPS_CONFLICT"},
    {1879048195,"SHT_MIPS_GPTAB"},
    {1879048196,"SHT_MIPS_UCODE"},
    {1879048197,"SHT_MIPS_DEBUG"},
    {1879048198,"SHT_MIPS_REGINFO"},
    {1879048199,"SHT_MIPS_PACKAGE"},
    {1879048200,"SHT_MIPS_PACKSYM"},
    {1879048201,"SHT_MIPS_RELD"},
    {1879048203,"SHT_MIPS_IFACE"},
    {1879048204,"SHT_MIPS_CONTENT"},
    {1879048205,"SHT_MIPS_OPTIONS"},
    {1879048208,"SHT_MIPS_SHDR"},
    {1879048209,"SHT_MIPS_FDESC"},
    {1879048210,"SHT_MIPS_EXTSYM"},
    {1879048211,"SHT_MIPS_DENSE"},
    {1879048212,"SHT_MIPS_PDESC"},
    {1879048213,"SHT_MIPS_LOCSYM"},
    {1879048214,"SHT_MIPS_AUXSYM"},
    {1879048215,"SHT_MIPS_OPTSYM"},
    {1879048216,"SHT_MIPS_LOCSTR"},
    {1879048217,"SHT_MIPS_LINE"},
    {1879048218,"SHT_MIPS_RFDESC"},
    {1879048219,"SHT_MIPS_DELTASYM"},
    {1879048220,"SHT_MIPS_DELTAINST"},
    {1879048221,"SHT_MIPS_DELTACLASS"},
    {1879048222,"SHT_MIPS_DWARF"},
    {1879048223,"SHT_MIPS_DELTADECL"},
    {1879048224,"SHT_MIPS_SYMBOL_LIB"},
    {1879048225,"SHT_MIPS_EVENTS"},
    {1879048226,"SHT_MIPS_TRANSLATE"},
    {1879048227,"SHT_MIPS_PIXIE"},
    {1879048228,"SHT_MIPS_XLATE"},
    {1879048229,"SHT_MIPS_XLATE_DEBUG"},
    {1879048230,"SHT_MIPS_WHIRL"},
    {1879048231,"SHT_MIPS_EH_REGION"},
    {1879048232,"SHT_MIPS_XLATE_OLD"},
    {1879048233,"SHT_MIPS_PDR_EXCEPTION"},
    {1879048234,"SHT_MIPS_ABIFLAGS"},
    {1879048235,"SHT_MIPS_XHASH"},
};

const char *helf_mips64_section_type_name_get(uint32_t type)
{
    return helf_arch_section_type_name_get(helf_mips64_section_type_names,sizeof(helf_mips64_section_type_names)/sizeof(helf_mips64_section_type_names[0]),type);
}

bool helf_mips64_relocation_address_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_relocation_address_info_t *info)
{
    if(!helf_mips64_is_mips64(input_file) || relocation==NULL)
    {
        return false;
    }
    uint8_t size=helf_mips64_relocation_size_get(relocation->type);
    if(size==0)
    {
        return false;
    }
    return helf_arch_relocation_address_info_get(input_file,relocation,size,info);
}

size_t helf_mips64_relocation_data_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,void *buffer,size_t buffer_length)
{
    if(!helf_mips64_is_mips64(input_file) || relocation==NULL)
    {
        return 0;
    }
    uint8_t size=helf_mips64_relocation_size_get(relocation->type);
    if(size==0)
    {
        return 0;
    }
    return helf_arch_relocation_data_get(input_file,relocation,size,buffer,buffer_length);
}

bool helf_mips64_is_shared_object(helf_file_input_t *input_file)
{
    return helf_arch_is_shared_object(input_file,helf_mips64_machines,sizeof(helf_mips64_machines)/sizeof(helf_mips64_machines[0]),HELF_ELFCLASS_64);
}

bool helf_mips64_is_relocatable(helf_file_input_t *input_file)
{
    return helf_arch_is_relocatable(input_file,helf_mips64_machines,sizeof(helf_mips64_machines)/sizeof(helf_mips64_machines[0]),HELF_ELFCLASS_64);
}

size_t helf_mips64_dynamic_count_get(helf_file_input_t *input_file)
{
    if(!helf_mips64_is_mips64(input_file))
    {
        return 0;
    }
    return helf_arch_dynamic_count_get(input_file);
}

bool helf_mips64_dynamic_info_get(helf_file_input_t *input_file,size_t index,helf_arch_dynamic_info_t *info)
{
    if(!helf_mips64_is_mips64(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_info_get(input_file,index,info);
}

bool helf_mips64_dynamic_string_get(helf_file_input_t *input_file,uint64_t offset,char *buffer,size_t buffer_length)
{
    if(!helf_mips64_is_mips64(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_string_get(input_file,offset,buffer,buffer_length);
}

/* helf_relocation_howto_begin */
static const helf_arch_relocation_howto_t helf_mips64_reloc_howto[]=
{
    {0x00000000u,0,0,0,0,0x0050,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000001u,4,16,0,0,0x0018,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000002u,4,32,0,0,0x0018,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000003u,4,32,0,0,0x0018,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000004u,4,26,2,0,0x0018,0x0000000003FFFFFFULL,0x0000000003FFFFFFULL},
    {0x00000005u,4,16,16,0,0x0018,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000006u,4,16,0,0,0x0018,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000007u,4,16,0,0,0x0018,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000008u,4,16,0,0,0x0018,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000009u,4,16,0,0,0x0218,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x0000000Au,4,16,2,0,0x001B,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x0000000Bu,4,16,0,0,0x0018,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x0000000Cu,4,32,0,0,0x0018,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x0000000Du,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000000Eu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000000Fu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000010u,4,5,0,6,0x0018,0x00000000000007C0ULL,0x00000000000007C0ULL},
    {0x00000011u,4,6,0,6,0x0018,0x00000000000007C4ULL,0x00000000000007C4ULL},
    {0x00000012u,8,64,0,0,0x0018,0xFFFFFFFFFFFFFFFFULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000013u,4,16,0,0,0x0218,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000014u,4,16,0,0,0x0218,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000015u,4,16,0,0,0x0218,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000016u,4,16,16,0,0x0218,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000017u,4,16,0,0,0x0218,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000018u,8,64,0,0,0x0018,0xFFFFFFFFFFFFFFFFULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000019u,4,32,0,0,0x0018,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x0000001Au,4,32,0,0,0x0018,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x0000001Bu,4,32,0,0,0x0018,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x0000001Cu,4,16,32,0,0x0010,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000001Du,4,16,48,0,0x0010,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000001Eu,4,16,16,0,0x0018,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x0000001Fu,4,16,0,0,0x0018,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000020u,4,32,0,0,0x0018,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000021u,2,16,0,0,0x0018,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000022u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000023u,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000024u,4,32,0,0,0x0218,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000025u,4,32,0,0,0x0010,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000026u,4,32,0,0,0x01D8,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000027u,4,32,0,0,0x01D8,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000028u,8,64,0,0,0x01D8,0xFFFFFFFFFFFFFFFFULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000029u,8,64,0,0,0x01D8,0xFFFFFFFFFFFFFFFFULL,0xFFFFFFFFFFFFFFFFULL},
    {0x0000002Au,4,16,0,0,0x01D8,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x0000002Bu,4,16,0,0,0x01D8,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x0000002Cu,4,16,16,0,0x01D8,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x0000002Du,4,16,0,0,0x01D8,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x0000002Eu,4,16,0,0,0x03D8,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x0000002Fu,4,32,0,0,0x01D8,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x00000030u,8,64,0,0,0x01D8,0xFFFFFFFFFFFFFFFFULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000031u,4,16,16,0,0x01D8,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000032u,4,16,0,0,0x01D8,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000033u,4,32,0,0,0x0110,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x0000003Cu,4,21,2,0,0x001B,0x00000000001FFFFFULL,0x00000000001FFFFFULL},
    {0x0000003Du,4,26,2,0,0x001B,0x0000000003FFFFFFULL,0x0000000003FFFFFFULL},
    {0x0000003Eu,4,18,3,0,0x001B,0x000000000003FFFFULL,0x000000000003FFFFULL},
    {0x0000003Fu,4,19,2,0,0x001B,0x000000000007FFFFULL,0x000000000007FFFFULL},
    {0x00000040u,4,16,16,0,0x001B,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000041u,4,16,0,0,0x001B,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000064u,4,26,2,0,0x0018,0x0000000003FFFFFFULL,0x0000000003FFFFFFULL},
    {0x00000065u,4,16,0,0,0x0018,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000066u,4,16,0,0,0x0218,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000067u,4,16,0,0,0x0018,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000068u,4,16,16,0,0x0018,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000069u,4,16,0,0,0x0018,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x0000006Au,4,16,0,0,0x01D8,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x0000006Bu,4,16,0,0,0x01D8,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x0000006Cu,4,16,16,0,0x01D8,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x0000006Du,4,16,0,0,0x01D8,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x0000006Eu,4,16,0,0,0x03D8,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x0000006Fu,4,16,16,0,0x01D8,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000070u,4,16,0,0,0x01D8,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000071u,4,16,1,0,0x001B,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x0000007Eu,0,0,0,0,0x0190,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000007Fu,8,64,0,0,0x0150,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000085u,4,26,1,0,0x0018,0x0000000003FFFFFFULL,0x0000000003FFFFFFULL},
    {0x00000086u,4,16,16,0,0x0018,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000087u,4,16,0,0,0x0018,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000088u,4,16,0,0,0x0018,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000089u,4,16,0,0,0x0018,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x0000008Au,4,16,0,0,0x0218,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x0000008Bu,2,7,1,0,0x001B,0x000000000000007FULL,0x000000000000007FULL},
    {0x0000008Cu,2,10,1,0,0x001B,0x00000000000003FFULL,0x00000000000003FFULL},
    {0x0000008Du,4,16,1,0,0x001B,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x0000008Eu,4,16,0,0,0x0018,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000091u,4,16,0,0,0x0218,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000092u,4,16,0,0,0x0218,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000093u,4,16,0,0,0x0218,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000094u,4,16,16,0,0x0218,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000095u,4,16,0,0,0x0218,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x00000096u,8,64,0,0,0x0018,0xFFFFFFFFFFFFFFFFULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000097u,4,16,32,0,0x0010,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000098u,4,16,48,0,0x0010,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000099u,4,16,16,0,0x0018,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x0000009Au,4,16,0,0,0x0018,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x0000009Bu,4,32,0,0,0x0018,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x0000009Cu,4,32,0,0,0x0010,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000009Du,4,16,0,0,0x0018,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x000000A2u,4,16,0,0,0x01D8,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x000000A3u,4,16,0,0,0x01D8,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x000000A4u,4,16,16,0,0x01D8,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x000000A5u,4,16,0,0,0x01D8,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x000000A6u,4,16,0,0,0x03D8,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x000000A9u,4,16,16,0,0x01D8,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x000000AAu,4,16,0,0,0x01D8,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x000000ACu,2,7,2,0,0x0018,0x000000000000007FULL,0x000000000000007FULL},
    {0x000000ADu,4,23,2,0,0x001B,0x00000000007FFFFFULL,0x00000000007FFFFFULL},
    {0x000000F8u,4,32,0,0,0x001B,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x000000F9u,4,32,0,0,0x0018,0x00000000FFFFFFFFULL,0x00000000FFFFFFFFULL},
    {0x000000FAu,4,16,2,0,0x001B,0x000000000000FFFFULL,0x000000000000FFFFULL},
    {0x000000FDu,4,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000FEu,4,0,0,0,0x0010,0x0000000000000000ULL,0x0000000000000000ULL},
};

bool helf_mips64_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto)
{
    if(howto==NULL)
    {
        return false;
    }
    size_t count=sizeof(helf_mips64_reloc_howto)/sizeof(helf_mips64_reloc_howto[0]);
    if(count==0)
    {
        return false;
    }
    for(size_t i=0;i<count;i++)
    {
        if(helf_mips64_reloc_howto[i].type==type)
        {
            *howto=helf_mips64_reloc_howto[i];
            return true;
        }
    }
    return false;
}
/* helf_relocation_howto_end */



bool helf_mips64_relocation_apply(uint32_t type,uint8_t *target,uintptr_t place,uintptr_t symbol_address,int64_t addend,const helf_arch_relocation_context_t *context)
{
    (void)place;
    bool big_endian=(context!=NULL)?context->big_endian:false;
    uint64_t value=(uint64_t)(symbol_address+(uintptr_t)addend);
    switch(type)
    {
    case 29:
        helf_arch_write16(target+(big_endian?2:0),(uint16_t)((value+0x800080008000ULL)>>48),big_endian);
        return true;
    case 28:
        helf_arch_write16(target+(big_endian?2:0),(uint16_t)((value+0x80008000ULL)>>32),big_endian);
        return true;
    case 5:
        helf_arch_write16(target+(big_endian?2:0),(uint16_t)((value+0x8000)>>16),big_endian);
        return true;
    case 6:
        helf_arch_write16(target+(big_endian?2:0),(uint16_t)value,big_endian);
        return true;
    case 4:
        {
            uint32_t insn=helf_arch_read32(target,big_endian);
            insn=(insn&~0x03FFFFFFU)|(((uint32_t)(value>>2))&0x03FFFFFFU);
            helf_arch_write32(target,insn,big_endian);
        }
        return true;
    case 2:
        helf_arch_write32(target,(uint32_t)value,big_endian);
        return true;
    case 18:
        helf_arch_write64(target,value,big_endian);
        return true;
    default:
        return false;
    }
}

void helf_mips64_relocation_info_decode(uint64_t info,bool big_endian,helf_arch_relocation_info_t *out)
{
    if(out==NULL)
    {
        return;
    }
    if(big_endian)
    {
        out->symbol=(uint32_t)(info>>32);
        out->type=(uint32_t)((info>>0)&0xFF);
        out->type2=(uint32_t)((info>>8)&0xFF);
        out->type3=(uint32_t)((info>>16)&0xFF);
        out->special_symbol=(uint32_t)((info>>24)&0xFF);
    }
    else
    {
        out->symbol=(uint32_t)(info&0xFFFFFFFFULL);
        out->type=(uint32_t)((info>>56)&0xFF);
        out->type2=(uint32_t)((info>>48)&0xFF);
        out->type3=(uint32_t)((info>>40)&0xFF);
        out->special_symbol=(uint32_t)((info>>32)&0xFF);
    }
}
