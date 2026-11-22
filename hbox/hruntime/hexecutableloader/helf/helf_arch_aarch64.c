/***************************************************************
 * Name:      helf_arch_aarch64.c
 * Purpose:   实现helf_arch_aarch64接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "helf_arch_aarch64.h"

static const uint16_t helf_aarch64_machines[]=
{
    HELF_EM_AARCH64,
};

bool helf_aarch64_is_aarch64(helf_file_input_t *input_file)
{
    return helf_arch_is(input_file,helf_aarch64_machines,sizeof(helf_aarch64_machines)/sizeof(helf_aarch64_machines[0]),HELF_ELFCLASS_64);
}

bool helf_aarch64_is_big_endian(helf_file_input_t *input_file)
{
    if(!helf_aarch64_is_aarch64(input_file))
    {
        return false;
    }
    return helf_file_input_is_big_endian(input_file);
}

bool helf_aarch64_is_executable(helf_file_input_t *input_file)
{
    return helf_arch_is_executable(input_file,helf_aarch64_machines,sizeof(helf_aarch64_machines)/sizeof(helf_aarch64_machines[0]),HELF_ELFCLASS_64);
}

bool helf_aarch64_entry_get(helf_file_input_t *input_file,uint64_t *entry)
{
    if(entry==NULL)
    {
        return false;
    }
    if(!helf_aarch64_is_aarch64(input_file))
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

size_t helf_aarch64_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_aarch64_is_aarch64(input_file))
    {
        return 0;
    }
    return helf_arch_section_count_get(input_file);
}

bool helf_aarch64_section_info_get(helf_file_input_t *input_file,size_t index,helf_aarch64_section_info_t *info)
{
    if(!helf_aarch64_is_aarch64(input_file))
    {
        return false;
    }
    return helf_arch_section_info_get(input_file,index,info);
}

bool helf_aarch64_is_rx_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_aarch64_is_aarch64(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_section(index,input_file);
}

intptr_t helf_aarch64_rx_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_aarch64_is_rx_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rx_section_offset_get(index,input_file);
}

size_t helf_aarch64_rx_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_aarch64_is_aarch64(input_file))
    {
        return 0;
    }
    return helf_arch_rx_section_size_get(input_file);
}

bool helf_aarch64_is_ro_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_aarch64_is_aarch64(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_section(index,input_file);
}

intptr_t helf_aarch64_ro_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_aarch64_is_ro_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_ro_section_offset_get(index,input_file);
}

size_t helf_aarch64_ro_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_aarch64_is_aarch64(input_file))
    {
        return 0;
    }
    return helf_arch_ro_section_size_get(input_file);
}

bool helf_aarch64_is_rw_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_aarch64_is_aarch64(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_section(index,input_file);
}

intptr_t helf_aarch64_rw_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_aarch64_is_rw_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rw_section_offset_get(index,input_file);
}

size_t helf_aarch64_rw_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_aarch64_is_aarch64(input_file))
    {
        return 0;
    }
    return helf_arch_rw_section_size_get(input_file);
}

bool helf_aarch64_is_bss_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_aarch64_is_aarch64(input_file))
    {
        return false;
    }
    return helf_arch_is_bss_section(index,input_file);
}

size_t helf_aarch64_program_header_count_get(helf_file_input_t *input_file)
{
    if(!helf_aarch64_is_aarch64(input_file))
    {
        return 0;
    }
    return helf_arch_program_header_count_get(input_file);
}

bool helf_aarch64_program_header_get(helf_file_input_t *input_file,size_t index,helf_elf64_program_header_t *programheader)
{
    if(programheader==NULL)
    {
        return false;
    }
    if(!helf_aarch64_is_aarch64(input_file))
    {
        return false;
    }
    if(index>=helf_arch_program_header_count_get(input_file))
    {
        return false;
    }
    return helf_file_input_64_bits_program_header_get(input_file,index,programheader);
}

size_t helf_aarch64_segment_count_get(helf_file_input_t *input_file)
{
    return helf_aarch64_program_header_count_get(input_file);
}

bool helf_aarch64_segment_info_get(helf_file_input_t *input_file,size_t index,helf_aarch64_segment_info_t *info)
{
    if(!helf_aarch64_is_aarch64(input_file))
    {
        return false;
    }
    return helf_arch_segment_info_get(input_file,index,info);
}

bool helf_aarch64_is_load_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_aarch64_is_aarch64(input_file))
    {
        return false;
    }
    return helf_arch_is_load_segment(index,input_file);
}

bool helf_aarch64_is_rx_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_aarch64_is_aarch64(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_segment(index,input_file);
}

bool helf_aarch64_is_ro_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_aarch64_is_aarch64(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_segment(index,input_file);
}

bool helf_aarch64_is_rw_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_aarch64_is_aarch64(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_segment(index,input_file);
}

size_t helf_aarch64_load_segment_count_get(helf_file_input_t *input_file)
{
    if(!helf_aarch64_is_aarch64(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_count_get(input_file);
}

bool helf_aarch64_load_segment_info_get(helf_file_input_t *input_file,size_t index,helf_aarch64_segment_info_t *info)
{
    if(!helf_aarch64_is_aarch64(input_file))
    {
        return false;
    }
    return helf_arch_load_segment_info_get(input_file,index,info);
}

size_t helf_aarch64_load_segment_total_filesz_get(helf_file_input_t *input_file)
{
    if(!helf_aarch64_is_aarch64(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_filesz_get(input_file);
}

size_t helf_aarch64_load_segment_total_memsz_get(helf_file_input_t *input_file)
{
    if(!helf_aarch64_is_aarch64(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_memsz_get(input_file);
}

size_t helf_aarch64_relocation_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_aarch64_is_aarch64(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_section_count_get(input_file);
}

bool helf_aarch64_relocation_section_info_get(helf_file_input_t *input_file,size_t index,helf_arch_relocation_section_info_t *info)
{
    if(!helf_aarch64_is_aarch64(input_file))
    {
        return false;
    }
    return helf_arch_relocation_section_info_get(input_file,index,info);
}

size_t helf_aarch64_relocation_count_get(helf_file_input_t *input_file,size_t section_index)
{
    if(!helf_aarch64_is_aarch64(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_count_get(input_file,section_index);
}

bool helf_aarch64_relocation_get(helf_file_input_t *input_file,size_t section_index,size_t index,helf_arch_relocation_info_t *info)
{
    if(!helf_aarch64_is_aarch64(input_file))
    {
        return false;
    }
    if(!helf_arch_relocation_get(input_file,section_index,index,info))
    {
        return false;
    }
    return true;
}

bool helf_aarch64_relocation_symbol_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_symbol_info_t *info)
{
    if(!helf_aarch64_is_aarch64(input_file))
    {
        return false;
    }
    return helf_arch_relocation_symbol_info_get(input_file,relocation,info);
}

static const helf_arch_relocation_type_name_t helf_aarch64_relocation_type_names[]=
{
    {0,0,"R_AARCH64_NONE"},
    {1,4,"R_AARCH64_P32_ABS32"},
    {2,2,"R_AARCH64_P32_ABS16"},
    {3,4,"R_AARCH64_P32_PREL32"},
    {4,2,"R_AARCH64_P32_PREL16"},
    {5,0,"R_AARCH64_P32_MOVW_UABS_G0"},
    {6,0,"R_AARCH64_P32_MOVW_UABS_G0_NC"},
    {7,0,"R_AARCH64_P32_MOVW_UABS_G1"},
    {8,0,"R_AARCH64_P32_MOVW_SABS_G0"},
    {9,0,"R_AARCH64_P32_LD_PREL_LO19"},
    {10,0,"R_AARCH64_P32_ADR_PREL_LO21"},
    {11,0,"R_AARCH64_P32_ADR_PREL_PG_HI21"},
    {12,0,"R_AARCH64_P32_ADD_ABS_LO12_NC"},
    {13,0,"R_AARCH64_P32_LDST8_ABS_LO12_NC"},
    {14,0,"R_AARCH64_P32_LDST16_ABS_LO12_NC"},
    {15,0,"R_AARCH64_P32_LDST32_ABS_LO12_NC"},
    {16,0,"R_AARCH64_P32_LDST64_ABS_LO12_NC"},
    {17,0,"R_AARCH64_P32_LDST128_ABS_LO12_NC"},
    {18,0,"R_AARCH64_P32_TSTBR14"},
    {19,0,"R_AARCH64_P32_CONDBR19"},
    {20,0,"R_AARCH64_P32_JUMP26"},
    {21,0,"R_AARCH64_P32_CALL26"},
    {22,0,"R_AARCH64_P32_MOVW_PREL_G0"},
    {23,0,"R_AARCH64_P32_MOVW_PREL_G0_NC"},
    {24,0,"R_AARCH64_P32_MOVW_PREL_G1"},
    {25,0,"R_AARCH64_P32_GOT_LD_PREL19"},
    {26,0,"R_AARCH64_P32_ADR_GOT_PAGE"},
    {27,0,"R_AARCH64_P32_LD32_GOT_LO12_NC"},
    {28,0,"R_AARCH64_P32_LD32_GOTPAGE_LO14"},
    {80,0,"R_AARCH64_P32_TLSGD_ADR_PREL21"},
    {81,0,"R_AARCH64_P32_TLSGD_ADR_PAGE21"},
    {82,0,"R_AARCH64_P32_TLSGD_ADD_LO12_NC"},
    {83,0,"R_AARCH64_P32_TLSLD_ADR_PREL21"},
    {84,0,"R_AARCH64_P32_TLSLD_ADR_PAGE21"},
    {85,0,"R_AARCH64_P32_TLSLD_ADD_LO12_NC"},
    {87,0,"R_AARCH64_P32_TLSLD_MOVW_DTPREL_G1"},
    {88,0,"R_AARCH64_P32_TLSLD_MOVW_DTPREL_G0"},
    {89,0,"R_AARCH64_P32_TLSLD_MOVW_DTPREL_G0_NC"},
    {90,0,"R_AARCH64_P32_TLSLD_ADD_DTPREL_HI12"},
    {91,0,"R_AARCH64_P32_TLSLD_ADD_DTPREL_LO12"},
    {92,0,"R_AARCH64_P32_TLSLD_ADD_DTPREL_LO12_NC"},
    {103,0,"R_AARCH64_P32_TLSIE_ADR_GOTTPREL_PAGE21"},
    {104,0,"R_AARCH64_P32_TLSIE_LD32_GOTTPREL_LO12_NC"},
    {105,0,"R_AARCH64_P32_TLSIE_LD_GOTTPREL_PREL19"},
    {106,0,"R_AARCH64_P32_TLSLE_MOVW_TPREL_G1"},
    {107,0,"R_AARCH64_P32_TLSLE_MOVW_TPREL_G0"},
    {108,0,"R_AARCH64_P32_TLSLE_MOVW_TPREL_G0_NC"},
    {109,0,"R_AARCH64_P32_TLSLE_ADD_TPREL_HI12"},
    {110,0,"R_AARCH64_P32_TLSLE_ADD_TPREL_LO12"},
    {111,0,"R_AARCH64_P32_TLSLE_ADD_TPREL_LO12_NC"},
    {112,0,"R_AARCH64_P32_TLSLE_LDST8_TPREL_LO12"},
    {113,0,"R_AARCH64_P32_TLSLE_LDST8_TPREL_LO12_NC"},
    {114,0,"R_AARCH64_P32_TLSLE_LDST16_TPREL_LO12"},
    {115,0,"R_AARCH64_P32_TLSLE_LDST16_TPREL_LO12_NC"},
    {116,0,"R_AARCH64_P32_TLSLE_LDST32_TPREL_LO12"},
    {117,0,"R_AARCH64_P32_TLSLE_LDST32_TPREL_LO12_NC"},
    {118,0,"R_AARCH64_P32_TLSLE_LDST64_TPREL_LO12"},
    {119,0,"R_AARCH64_P32_TLSLE_LDST64_TPREL_LO12_NC"},
    {122,0,"R_AARCH64_P32_TLSDESC_LD_PREL19"},
    {123,0,"R_AARCH64_P32_TLSDESC_ADR_PREL21"},
    {124,0,"R_AARCH64_P32_TLSDESC_ADR_PAGE21"},
    {125,0,"R_AARCH64_P32_TLSDESC_LD32_LO12_NC"},
    {126,0,"R_AARCH64_P32_TLSDESC_ADD_LO12_NC"},
    {127,0,"R_AARCH64_P32_TLSDESC_CALL"},
    {180,0,"R_AARCH64_P32_COPY"},
    {181,0,"R_AARCH64_P32_GLOB_DAT"},
    {182,0,"R_AARCH64_P32_JUMP_SLOT"},
    {183,0,"R_AARCH64_P32_RELATIVE"},
    {184,0,"R_AARCH64_P32_TLS_DTPMOD"},
    {185,0,"R_AARCH64_P32_TLS_DTPREL"},
    {186,0,"R_AARCH64_P32_TLS_TPREL"},
    {187,0,"R_AARCH64_P32_TLSDESC"},
    {188,0,"R_AARCH64_P32_IRELATIVE"},
    {256,0,"R_AARCH64_NULL"},
    {257,8,"R_AARCH64_ABS64"},
    {258,4,"R_AARCH64_ABS32"},
    {259,2,"R_AARCH64_ABS16"},
    {260,8,"R_AARCH64_PREL64"},
    {261,4,"R_AARCH64_PREL32"},
    {262,2,"R_AARCH64_PREL16"},
    {263,4,"R_AARCH64_MOVW_UABS_G0"},
    {264,4,"R_AARCH64_MOVW_UABS_G0_NC"},
    {265,4,"R_AARCH64_MOVW_UABS_G1"},
    {266,4,"R_AARCH64_MOVW_UABS_G1_NC"},
    {267,4,"R_AARCH64_MOVW_UABS_G2"},
    {268,4,"R_AARCH64_MOVW_UABS_G2_NC"},
    {269,4,"R_AARCH64_MOVW_UABS_G3"},
    {270,4,"R_AARCH64_MOVW_SABS_G0"},
    {271,4,"R_AARCH64_MOVW_SABS_G1"},
    {272,4,"R_AARCH64_MOVW_SABS_G2"},
    {273,4,"R_AARCH64_LD_PREL_LO19"},
    {274,4,"R_AARCH64_ADR_PREL_LO21"},
    {275,4,"R_AARCH64_ADR_PREL_PG_HI21"},
    {276,4,"R_AARCH64_ADR_PREL_PG_HI21_NC"},
    {277,4,"R_AARCH64_ADD_ABS_LO12_NC"},
    {278,4,"R_AARCH64_LDST8_ABS_LO12_NC"},
    {279,4,"R_AARCH64_TSTBR14"},
    {280,4,"R_AARCH64_CONDBR19"},
    {282,4,"R_AARCH64_JUMP26"},
    {283,4,"R_AARCH64_CALL26"},
    {284,4,"R_AARCH64_LDST16_ABS_LO12_NC"},
    {285,4,"R_AARCH64_LDST32_ABS_LO12_NC"},
    {286,4,"R_AARCH64_LDST64_ABS_LO12_NC"},
    {287,4,"R_AARCH64_MOVW_PREL_G0"},
    {288,4,"R_AARCH64_MOVW_PREL_G0_NC"},
    {289,4,"R_AARCH64_MOVW_PREL_G1"},
    {290,4,"R_AARCH64_MOVW_PREL_G1_NC"},
    {291,4,"R_AARCH64_MOVW_PREL_G2"},
    {292,4,"R_AARCH64_MOVW_PREL_G2_NC"},
    {293,4,"R_AARCH64_MOVW_PREL_G3"},
    {299,4,"R_AARCH64_LDST128_ABS_LO12_NC"},
    {300,0,"R_AARCH64_MOVW_GOTOFF_G0"},
    {301,4,"R_AARCH64_MOVW_GOTOFF_G0_NC"},
    {302,4,"R_AARCH64_MOVW_GOTOFF_G1"},
    {303,0,"R_AARCH64_MOVW_GOTOFF_G1_NC"},
    {304,0,"R_AARCH64_MOVW_GOTOFF_G2"},
    {305,0,"R_AARCH64_MOVW_GOTOFF_G2_NC"},
    {306,0,"R_AARCH64_MOVW_GOTOFF_G3"},
    {307,8,"R_AARCH64_GOTREL64"},
    {308,4,"R_AARCH64_GOTREL32"},
    {309,4,"R_AARCH64_GOT_LD_PREL19"},
    {310,4,"R_AARCH64_LD64_GOTOFF_LO15"},
    {311,4,"R_AARCH64_ADR_GOT_PAGE"},
    {312,4,"R_AARCH64_LD64_GOT_LO12_NC"},
    {313,4,"R_AARCH64_LD64_GOTPAGE_LO15"},
    {512,4,"R_AARCH64_TLSGD_ADR_PREL21"},
    {513,4,"R_AARCH64_TLSGD_ADR_PAGE21"},
    {514,4,"R_AARCH64_TLSGD_ADD_LO12_NC"},
    {515,4,"R_AARCH64_TLSGD_MOVW_G1"},
    {516,4,"R_AARCH64_TLSGD_MOVW_G0_NC"},
    {517,4,"R_AARCH64_TLSLD_ADR_PREL21"},
    {518,4,"R_AARCH64_TLSLD_ADR_PAGE21"},
    {519,4,"R_AARCH64_TLSLD_ADD_LO12_NC"},
    {520,0,"R_AARCH64_TLSLD_MOVW_G1"},
    {521,0,"R_AARCH64_TLSLD_MOVW_G0_NC"},
    {522,0,"R_AARCH64_TLSLD_LD_PREL19"},
    {523,4,"R_AARCH64_TLSLD_MOVW_DTPREL_G2"},
    {524,4,"R_AARCH64_TLSLD_MOVW_DTPREL_G1"},
    {525,4,"R_AARCH64_TLSLD_MOVW_DTPREL_G1_NC"},
    {526,4,"R_AARCH64_TLSLD_MOVW_DTPREL_G0"},
    {527,4,"R_AARCH64_TLSLD_MOVW_DTPREL_G0_NC"},
    {528,4,"R_AARCH64_TLSLD_ADD_DTPREL_HI12"},
    {529,4,"R_AARCH64_TLSLD_ADD_DTPREL_LO12"},
    {530,4,"R_AARCH64_TLSLD_ADD_DTPREL_LO12_NC"},
    {531,4,"R_AARCH64_TLSLD_LDST8_DTPREL_LO12"},
    {532,4,"R_AARCH64_TLSLD_LDST8_DTPREL_LO12_NC"},
    {533,4,"R_AARCH64_TLSLD_LDST16_DTPREL_LO12"},
    {534,4,"R_AARCH64_TLSLD_LDST16_DTPREL_LO12_NC"},
    {535,4,"R_AARCH64_TLSLD_LDST32_DTPREL_LO12"},
    {536,4,"R_AARCH64_TLSLD_LDST32_DTPREL_LO12_NC"},
    {537,4,"R_AARCH64_TLSLD_LDST64_DTPREL_LO12"},
    {538,4,"R_AARCH64_TLSLD_LDST64_DTPREL_LO12_NC"},
    {539,4,"R_AARCH64_TLSIE_MOVW_GOTTPREL_G1"},
    {540,4,"R_AARCH64_TLSIE_MOVW_GOTTPREL_G0_NC"},
    {541,4,"R_AARCH64_TLSIE_ADR_GOTTPREL_PAGE21"},
    {542,4,"R_AARCH64_TLSIE_LD64_GOTTPREL_LO12_NC"},
    {543,4,"R_AARCH64_TLSIE_LD_GOTTPREL_PREL19"},
    {544,4,"R_AARCH64_TLSLE_MOVW_TPREL_G2"},
    {545,4,"R_AARCH64_TLSLE_MOVW_TPREL_G1"},
    {546,4,"R_AARCH64_TLSLE_MOVW_TPREL_G1_NC"},
    {547,4,"R_AARCH64_TLSLE_MOVW_TPREL_G0"},
    {548,4,"R_AARCH64_TLSLE_MOVW_TPREL_G0_NC"},
    {549,4,"R_AARCH64_TLSLE_ADD_TPREL_HI12"},
    {550,4,"R_AARCH64_TLSLE_ADD_TPREL_LO12"},
    {551,4,"R_AARCH64_TLSLE_ADD_TPREL_LO12_NC"},
    {552,4,"R_AARCH64_TLSLE_LDST8_TPREL_LO12"},
    {553,4,"R_AARCH64_TLSLE_LDST8_TPREL_LO12_NC"},
    {554,4,"R_AARCH64_TLSLE_LDST16_TPREL_LO12"},
    {555,4,"R_AARCH64_TLSLE_LDST16_TPREL_LO12_NC"},
    {556,4,"R_AARCH64_TLSLE_LDST32_TPREL_LO12"},
    {557,4,"R_AARCH64_TLSLE_LDST32_TPREL_LO12_NC"},
    {558,4,"R_AARCH64_TLSLE_LDST64_TPREL_LO12"},
    {559,4,"R_AARCH64_TLSLE_LDST64_TPREL_LO12_NC"},
    {560,4,"R_AARCH64_TLSDESC_LD_PREL19"},
    {561,4,"R_AARCH64_TLSDESC_ADR_PREL21"},
    {562,4,"R_AARCH64_TLSDESC_ADR_PAGE21"},
    {563,4,"R_AARCH64_TLSDESC_LD64_LO12"},
    {564,4,"R_AARCH64_TLSDESC_ADD_LO12"},
    {565,4,"R_AARCH64_TLSDESC_OFF_G1"},
    {566,4,"R_AARCH64_TLSDESC_OFF_G0_NC"},
    {567,4,"R_AARCH64_TLSDESC_LDR"},
    {568,4,"R_AARCH64_TLSDESC_ADD"},
    {569,4,"R_AARCH64_TLSDESC_CALL"},
    {570,0,"R_AARCH64_TLSLE_LDST128_TPREL_LO12"},
    {571,0,"R_AARCH64_TLSLE_LDST128_TPREL_LO12_NC"},
    {572,0,"R_AARCH64_TLSLD_LDST128_DTPREL_LO12"},
    {573,0,"R_AARCH64_TLSLD_LDST128_DTPREL_LO12_NC"},
    {1024,8,"R_AARCH64_COPY"},
    {1025,8,"R_AARCH64_GLOB_DAT"},
    {1026,8,"R_AARCH64_JUMP_SLOT"},
    {1027,8,"R_AARCH64_RELATIVE"},
    {1028,8,"R_AARCH64_TLS_DTPMOD64"},
    {1029,8,"R_AARCH64_TLS_DTPREL64"},
    {1030,8,"R_AARCH64_TLS_TPREL64"},
    {1031,8,"R_AARCH64_TLSDESC"},
    {1032,8,"R_AARCH64_IRELATIVE"},
};

const char *helf_aarch64_relocation_type_name_get(uint32_t type)
{
    return helf_arch_relocation_type_name_get(helf_aarch64_relocation_type_names,sizeof(helf_aarch64_relocation_type_names)/sizeof(helf_aarch64_relocation_type_names[0]),type);
}

uint8_t helf_aarch64_relocation_size_get(uint32_t type)
{
    return helf_arch_relocation_type_size_get(helf_aarch64_relocation_type_names,sizeof(helf_aarch64_relocation_type_names)/sizeof(helf_aarch64_relocation_type_names[0]),type);
}

static const helf_arch_section_type_name_t helf_aarch64_section_type_names[]=
{
    {1879048195,"SHT_AARCH64_ATTRIBUTES"},
    {1879048196,"SHT_AARCH64_AUTH_RELR"},
    {1879048199,"SHT_AARCH64_MEMTAG_GLOBALS_STATIC"},
    {1879048200,"SHT_AARCH64_MEMTAG_GLOBALS_DYNAMIC"},
};

const char *helf_aarch64_section_type_name_get(uint32_t type)
{
    return helf_arch_section_type_name_get(helf_aarch64_section_type_names,sizeof(helf_aarch64_section_type_names)/sizeof(helf_aarch64_section_type_names[0]),type);
}

bool helf_aarch64_relocation_address_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_relocation_address_info_t *info)
{
    if(!helf_aarch64_is_aarch64(input_file) || relocation==NULL)
    {
        return false;
    }
    uint8_t size=helf_aarch64_relocation_size_get(relocation->type);
    if(size==0)
    {
        return false;
    }
    return helf_arch_relocation_address_info_get(input_file,relocation,size,info);
}

size_t helf_aarch64_relocation_data_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,void *buffer,size_t buffer_length)
{
    if(!helf_aarch64_is_aarch64(input_file) || relocation==NULL)
    {
        return 0;
    }
    uint8_t size=helf_aarch64_relocation_size_get(relocation->type);
    if(size==0)
    {
        return 0;
    }
    return helf_arch_relocation_data_get(input_file,relocation,size,buffer,buffer_length);
}

bool helf_aarch64_is_shared_object(helf_file_input_t *input_file)
{
    return helf_arch_is_shared_object(input_file,helf_aarch64_machines,sizeof(helf_aarch64_machines)/sizeof(helf_aarch64_machines[0]),HELF_ELFCLASS_64);
}

bool helf_aarch64_is_relocatable(helf_file_input_t *input_file)
{
    return helf_arch_is_relocatable(input_file,helf_aarch64_machines,sizeof(helf_aarch64_machines)/sizeof(helf_aarch64_machines[0]),HELF_ELFCLASS_64);
}

size_t helf_aarch64_dynamic_count_get(helf_file_input_t *input_file)
{
    if(!helf_aarch64_is_aarch64(input_file))
    {
        return 0;
    }
    return helf_arch_dynamic_count_get(input_file);
}

bool helf_aarch64_dynamic_info_get(helf_file_input_t *input_file,size_t index,helf_arch_dynamic_info_t *info)
{
    if(!helf_aarch64_is_aarch64(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_info_get(input_file,index,info);
}

bool helf_aarch64_dynamic_string_get(helf_file_input_t *input_file,uint64_t offset,char *buffer,size_t buffer_length)
{
    if(!helf_aarch64_is_aarch64(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_string_get(input_file,offset,buffer,buffer_length);
}

/* helf_relocation_howto_begin */
static const helf_arch_relocation_howto_t helf_aarch64_reloc_howto[]=
{
    {0x00000000u,0,0,0,0,0x0040,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000001u,4,32,0,0,0x0000,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000002u,2,16,0,0,0x0000,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000003u,4,32,0,0,0x0003,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000004u,2,16,0,0,0x0003,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000005u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000006u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000007u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000008u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000009u,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000000Au,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000000Bu,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000000Cu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000000Du,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000000Eu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000000Fu,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000010u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000011u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000012u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000013u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000014u,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000015u,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000016u,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000017u,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000018u,0,0,0,0,0x0003,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000019u,0,0,0,0,0x0203,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Au,0,0,0,0,0x0200,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Bu,0,0,0,0,0x0200,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000001Cu,0,0,0,0,0x0200,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000050u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000051u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000052u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000053u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000054u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000055u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000057u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000058u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000059u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000005Au,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000005Bu,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000005Cu,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000067u,0,0,0,0,0x03C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000068u,0,0,0,0,0x03C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000069u,0,0,0,0,0x03C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000006Au,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000006Bu,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000006Cu,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000006Du,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000006Eu,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000006Fu,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000070u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000071u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000072u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000073u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000074u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000075u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000076u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000077u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000007Au,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000007Bu,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000007Cu,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000007Du,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000007Eu,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000007Fu,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000B4u,0,0,0,0,0x0180,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000B5u,0,0,0,0,0x0100,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000B6u,0,0,0,0,0x0140,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000B7u,0,0,0,0,0x0080,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000B8u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000B9u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000BAu,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000BBu,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x000000BCu,0,0,0,0,0x0080,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000100u,0,0,0,0,0x0000,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000101u,8,64,0,0,0x0000,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000102u,4,32,0,0,0x0000,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000103u,2,16,0,0,0x0000,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000104u,8,64,0,0,0x0003,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000105u,4,32,0,0,0x0003,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000106u,2,16,0,0,0x0003,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000107u,4,16,0,0,0x0000,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000108u,4,16,0,0,0x0000,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000109u,4,16,16,0,0x0000,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000010Au,4,16,16,0,0x0000,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000010Bu,4,16,32,0,0x0000,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000010Cu,4,16,32,0,0x0000,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000010Du,4,16,48,0,0x0000,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000010Eu,4,17,0,0,0x0000,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000010Fu,4,17,16,0,0x0000,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000110u,4,17,32,0,0x0000,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000111u,4,19,2,0,0x0003,0x0000000000000000ULL,0x000000000007FFFFULL},
    {0x00000112u,4,21,0,0,0x0003,0x0000000000000000ULL,0x00000000001FFFFFULL},
    {0x00000113u,4,21,12,0,0x0003,0x0000000000000000ULL,0x00000000001FFFFFULL},
    {0x00000114u,4,21,12,0,0x0003,0x0000000000000000ULL,0x00000000001FFFFFULL},
    {0x00000115u,4,12,0,10,0x0000,0x0000000000000000ULL,0x00000000003FFC00ULL},
    {0x00000116u,4,12,0,0,0x0000,0x0000000000000000ULL,0x0000000000000FFFULL},
    {0x00000117u,4,14,2,0,0x0003,0x0000000000000000ULL,0x0000000000003FFFULL},
    {0x00000118u,4,19,2,0,0x0003,0x0000000000000000ULL,0x000000000007FFFFULL},
    {0x0000011Au,4,26,2,0,0x0003,0x0000000000000000ULL,0x0000000003FFFFFFULL},
    {0x0000011Bu,4,26,2,0,0x0003,0x0000000000000000ULL,0x0000000003FFFFFFULL},
    {0x0000011Cu,4,12,1,0,0x0000,0x0000000000000000ULL,0x0000000000000FFEULL},
    {0x0000011Du,4,12,2,0,0x0000,0x0000000000000000ULL,0x0000000000000FFCULL},
    {0x0000011Eu,4,12,3,0,0x0000,0x0000000000000000ULL,0x0000000000000FF8ULL},
    {0x0000011Fu,4,17,0,0,0x0003,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000120u,4,16,0,0,0x0003,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000121u,4,17,16,0,0x0003,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000122u,4,16,16,0,0x0003,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000123u,4,17,32,0,0x0003,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000124u,4,16,32,0,0x0003,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000125u,4,16,48,0,0x0003,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000012Bu,4,12,4,0,0x0000,0x0000000000000000ULL,0x0000000000000FF0ULL},
    {0x0000012Cu,0,0,0,0,0x0200,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000012Du,4,16,0,0,0x0200,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000012Eu,4,16,16,0,0x0200,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000012Fu,0,0,0,0,0x0200,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000130u,0,0,0,0,0x0200,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000131u,0,0,0,0,0x0200,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000132u,0,0,0,0,0x0200,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000133u,8,64,0,0,0x0203,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000134u,4,32,0,0,0x0203,0x0000000000000000ULL,0x00000000FFFFFFFFULL},
    {0x00000135u,4,19,2,0,0x0203,0x0000000000000000ULL,0x0000000000FFFFE0ULL},
    {0x00000136u,4,12,3,0,0x0200,0x0000000000000000ULL,0x0000000000007FF8ULL},
    {0x00000137u,4,21,12,0,0x0203,0x0000000000000000ULL,0x00000000001FFFFFULL},
    {0x00000138u,4,12,3,0,0x0200,0x0000000000000000ULL,0x0000000000000FF8ULL},
    {0x00000139u,4,12,3,0,0x0200,0x0000000000000000ULL,0x0000000000007FF8ULL},
    {0x00000200u,4,21,0,0,0x01C3,0x0000000000000000ULL,0x00000000001FFFFFULL},
    {0x00000201u,4,21,12,0,0x01C3,0x0000000000000000ULL,0x00000000001FFFFFULL},
    {0x00000202u,4,12,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000FFFULL},
    {0x00000203u,4,16,16,0,0x01C0,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000204u,4,16,0,0,0x01C0,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000205u,4,21,0,0,0x01C3,0x0000000000000000ULL,0x00000000001FFFFFULL},
    {0x00000206u,4,21,12,0,0x01C3,0x0000000000000000ULL,0x00000000001FFFFFULL},
    {0x00000207u,4,12,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000FFFULL},
    {0x00000208u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000209u,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000020Au,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000020Bu,4,16,32,0,0x01C0,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000020Cu,4,16,16,0,0x01C0,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000020Du,4,16,16,0,0x01C0,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000020Eu,4,16,0,0,0x01C0,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000020Fu,4,16,0,0,0x01C0,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000210u,4,12,12,0,0x01C0,0x0000000000000000ULL,0x0000000000000FFFULL},
    {0x00000211u,4,12,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000FFFULL},
    {0x00000212u,4,12,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000FFFULL},
    {0x00000213u,4,12,0,10,0x01C0,0x0000000000000000ULL,0x00000000003FFC00ULL},
    {0x00000214u,4,12,0,10,0x01C0,0x0000000000000000ULL,0x00000000003FFC00ULL},
    {0x00000215u,4,11,1,10,0x01C0,0x0000000000000000ULL,0x00000000001FFC00ULL},
    {0x00000216u,4,11,1,10,0x01C0,0x0000000000000000ULL,0x00000000001FFC00ULL},
    {0x00000217u,4,10,2,10,0x01C0,0x0000000000000000ULL,0x00000000003FFC00ULL},
    {0x00000218u,4,10,2,10,0x01C0,0x0000000000000000ULL,0x00000000000FFC00ULL},
    {0x00000219u,4,9,3,10,0x01C0,0x0000000000000000ULL,0x00000000003FFC00ULL},
    {0x0000021Au,4,9,3,10,0x01C0,0x0000000000000000ULL,0x000000000007FC00ULL},
    {0x0000021Bu,4,16,16,0,0x03C0,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000021Cu,4,16,0,0,0x03C0,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x0000021Du,4,21,12,0,0x03C0,0x0000000000000000ULL,0x00000000001FFFFFULL},
    {0x0000021Eu,4,12,3,0,0x03C0,0x0000000000000000ULL,0x0000000000000FF8ULL},
    {0x0000021Fu,4,19,2,0,0x03C0,0x0000000000000000ULL,0x00000000001FFFFCULL},
    {0x00000220u,4,16,32,0,0x01C0,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000221u,4,16,16,0,0x01C0,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000222u,4,16,16,0,0x01C0,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000223u,4,16,0,0,0x01C0,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000224u,4,16,0,0,0x01C0,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000225u,4,12,12,0,0x01C0,0x0000000000000000ULL,0x0000000000000FFFULL},
    {0x00000226u,4,12,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000FFFULL},
    {0x00000227u,4,12,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000FFFULL},
    {0x00000228u,4,12,0,10,0x01C0,0x0000000000000000ULL,0x00000000003FFC00ULL},
    {0x00000229u,4,12,0,10,0x01C0,0x0000000000000000ULL,0x00000000003FFC00ULL},
    {0x0000022Au,4,11,1,10,0x01C0,0x0000000000000000ULL,0x00000000001FFC00ULL},
    {0x0000022Bu,4,11,1,10,0x01C0,0x0000000000000000ULL,0x00000000001FFC00ULL},
    {0x0000022Cu,4,10,2,10,0x01C0,0x0000000000000000ULL,0x00000000000FFC00ULL},
    {0x0000022Du,4,10,2,10,0x01C0,0x0000000000000000ULL,0x00000000000FFC00ULL},
    {0x0000022Eu,4,9,3,10,0x01C0,0x0000000000000000ULL,0x000000000007FC00ULL},
    {0x0000022Fu,4,9,3,10,0x01C0,0x0000000000000000ULL,0x000000000007FC00ULL},
    {0x00000230u,4,19,2,0,0x01C3,0x0000000000000000ULL,0x0000000000FFFFE0ULL},
    {0x00000231u,4,21,0,0,0x01C3,0x0000000000000000ULL,0x00000000001FFFFFULL},
    {0x00000232u,4,21,12,0,0x01C3,0x0000000000000000ULL,0x00000000001FFFFFULL},
    {0x00000233u,4,12,3,0,0x01C0,0x0000000000000000ULL,0x0000000000000FF8ULL},
    {0x00000234u,4,12,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000FFFULL},
    {0x00000235u,4,12,16,0,0x01C0,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000236u,4,12,0,0,0x01C0,0x0000000000000000ULL,0x000000000000FFFFULL},
    {0x00000237u,4,12,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000238u,4,12,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000239u,4,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000023Au,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000023Bu,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000023Cu,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x0000023Du,0,0,0,0,0x01C0,0x0000000000000000ULL,0x0000000000000000ULL},
    {0x00000400u,8,64,0,0,0x0188,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000401u,8,64,0,0,0x0108,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000402u,8,64,0,0,0x0148,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000403u,8,64,0,0,0x0088,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000404u,8,64,0,0,0x01C0,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000405u,8,64,0,0,0x01C0,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000406u,8,64,0,0,0x01C0,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000407u,4,64,0,0,0x01C0,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
    {0x00000408u,8,64,0,0,0x0080,0x0000000000000000ULL,0xFFFFFFFFFFFFFFFFULL},
};

bool helf_aarch64_relocation_howto_get(uint32_t type,helf_arch_relocation_howto_t *howto)
{
    if(howto==NULL)
    {
        return false;
    }
    size_t count=sizeof(helf_aarch64_reloc_howto)/sizeof(helf_aarch64_reloc_howto[0]);
    if(count==0)
    {
        return false;
    }
    for(size_t i=0;i<count;i++)
    {
        if(helf_aarch64_reloc_howto[i].type==type)
        {
            *howto=helf_aarch64_reloc_howto[i];
            return true;
        }
    }
    return false;
}
/* helf_relocation_howto_end */


static void helf_aarch64_relocation_write_imm21(uint8_t *target,uint64_t value,bool big_endian)
{
    uint32_t insn=helf_arch_read32(target,big_endian);
    uint32_t imm=(uint32_t)value&0x1FFFFF;
    insn=(insn&~((0x7FFFFU<<5)|(3U<<29)))|(((imm>>2)&0x7FFFFU)<<5)|((imm&3U)<<29);
    helf_arch_write32(target,insn,big_endian);
}

bool helf_aarch64_relocation_apply(uint32_t type,uint8_t *target,uintptr_t place,uintptr_t symbol_address,int64_t addend,const helf_arch_relocation_context_t *context)
{
    bool big_endian=(context!=NULL)?context->big_endian:false;
    uint64_t value=(uint64_t)(symbol_address+(uintptr_t)addend);
    switch(type)
    {
    case 275:
    case 276:
        helf_aarch64_relocation_write_imm21(target,((value>>12)-(uint64_t)(place>>12))&0x1FFFFF,big_endian);
        return true;
    case 274:
        helf_aarch64_relocation_write_imm21(target,(uint64_t)((int64_t)value-(int64_t)place),big_endian);
        return true;
    case 277:
        {
            uint32_t insn=helf_arch_read32(target,big_endian);
            insn=(insn&~0x3FFC00U)|(((uint32_t)value&0xFFFU)<<10);
            helf_arch_write32(target,insn,big_endian);
        }
        return true;
    case 278:
    case 284:
    case 285:
    case 286:
    case 299:
        {
            static const uint8_t shift_table[]= {0,1,2,3,4};
            size_t index=0;
            if(type==284) index=1;
            else if(type==285) index=2;
            else if(type==286) index=3;
            else if(type==299) index=4;
            uint32_t insn=helf_arch_read32(target,big_endian);
            insn=(insn&~0x3FFC00U)|((((uint32_t)value&0xFFFU)>>shift_table[index])<<10);
            helf_arch_write32(target,insn,big_endian);
        }
        return true;
    case 263:
    case 264:
    case 265:
    case 266:
    case 267:
    case 268:
    case 269:
    case 270:
    case 271:
    case 272:
    case 273:
        {
            uint32_t shift=0;
            if(type==263 || type==264 || type==270) shift=0;
            else if(type==265 || type==266 || type==271) shift=16;
            else if(type==267 || type==268 || type==272) shift=32;
            else shift=48;
            uint32_t insn=helf_arch_read32(target,big_endian);
            insn=(insn&~0x001F0000U)|((((uint32_t)value>>shift)&0xFFFFU)<<5);
            helf_arch_write32(target,insn,big_endian);
        }
        return true;
    default:
        return false;
    }
}
