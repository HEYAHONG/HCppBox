/***************************************************************
 * Name:      helf_arch_loongarch32.c
 * Purpose:   实现helf_arch_loongarch32接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "helf_arch_loongarch32.h"

static const uint16_t helf_loongarch32_machines[]=
{
    HELF_EM_LOONGARCH,
};

bool helf_loongarch32_is_loongarch32(helf_file_input_t *input_file)
{
    return helf_arch_is(input_file,helf_loongarch32_machines,sizeof(helf_loongarch32_machines)/sizeof(helf_loongarch32_machines[0]),HELF_ELFCLASS_32);
}

bool helf_loongarch32_is_big_endian(helf_file_input_t *input_file)
{
    if(!helf_loongarch32_is_loongarch32(input_file))
    {
        return false;
    }
    return helf_file_input_is_big_endian(input_file);
}

bool helf_loongarch32_is_executable(helf_file_input_t *input_file)
{
    return helf_arch_is_executable(input_file,helf_loongarch32_machines,sizeof(helf_loongarch32_machines)/sizeof(helf_loongarch32_machines[0]),HELF_ELFCLASS_32);
}

bool helf_loongarch32_entry_get(helf_file_input_t *input_file,uint32_t *entry)
{
    if(entry==NULL)
    {
        return false;
    }
    if(!helf_loongarch32_is_loongarch32(input_file))
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

size_t helf_loongarch32_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_loongarch32_is_loongarch32(input_file))
    {
        return 0;
    }
    return helf_arch_section_count_get(input_file);
}

bool helf_loongarch32_section_info_get(helf_file_input_t *input_file,size_t index,helf_loongarch32_section_info_t *info)
{
    if(!helf_loongarch32_is_loongarch32(input_file))
    {
        return false;
    }
    return helf_arch_section_info_get(input_file,index,info);
}

bool helf_loongarch32_is_rx_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_loongarch32_is_loongarch32(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_section(index,input_file);
}

intptr_t helf_loongarch32_rx_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_loongarch32_is_rx_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rx_section_offset_get(index,input_file);
}

size_t helf_loongarch32_rx_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_loongarch32_is_loongarch32(input_file))
    {
        return 0;
    }
    return helf_arch_rx_section_size_get(input_file);
}

bool helf_loongarch32_is_ro_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_loongarch32_is_loongarch32(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_section(index,input_file);
}

intptr_t helf_loongarch32_ro_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_loongarch32_is_ro_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_ro_section_offset_get(index,input_file);
}

size_t helf_loongarch32_ro_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_loongarch32_is_loongarch32(input_file))
    {
        return 0;
    }
    return helf_arch_ro_section_size_get(input_file);
}

bool helf_loongarch32_is_rw_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_loongarch32_is_loongarch32(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_section(index,input_file);
}

intptr_t helf_loongarch32_rw_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_loongarch32_is_rw_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rw_section_offset_get(index,input_file);
}

size_t helf_loongarch32_rw_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_loongarch32_is_loongarch32(input_file))
    {
        return 0;
    }
    return helf_arch_rw_section_size_get(input_file);
}

bool helf_loongarch32_is_bss_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_loongarch32_is_loongarch32(input_file))
    {
        return false;
    }
    return helf_arch_is_bss_section(index,input_file);
}

size_t helf_loongarch32_program_header_count_get(helf_file_input_t *input_file)
{
    if(!helf_loongarch32_is_loongarch32(input_file))
    {
        return 0;
    }
    return helf_arch_program_header_count_get(input_file);
}

bool helf_loongarch32_program_header_get(helf_file_input_t *input_file,size_t index,helf_elf32_program_header_t *programheader)
{
    if(programheader==NULL)
    {
        return false;
    }
    if(!helf_loongarch32_is_loongarch32(input_file))
    {
        return false;
    }
    if(index>=helf_arch_program_header_count_get(input_file))
    {
        return false;
    }
    return helf_file_input_32_bits_program_header_get(input_file,index,programheader);
}

size_t helf_loongarch32_segment_count_get(helf_file_input_t *input_file)
{
    return helf_loongarch32_program_header_count_get(input_file);
}

bool helf_loongarch32_segment_info_get(helf_file_input_t *input_file,size_t index,helf_loongarch32_segment_info_t *info)
{
    if(!helf_loongarch32_is_loongarch32(input_file))
    {
        return false;
    }
    return helf_arch_segment_info_get(input_file,index,info);
}

bool helf_loongarch32_is_load_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_loongarch32_is_loongarch32(input_file))
    {
        return false;
    }
    return helf_arch_is_load_segment(index,input_file);
}

bool helf_loongarch32_is_rx_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_loongarch32_is_loongarch32(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_segment(index,input_file);
}

bool helf_loongarch32_is_ro_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_loongarch32_is_loongarch32(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_segment(index,input_file);
}

bool helf_loongarch32_is_rw_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_loongarch32_is_loongarch32(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_segment(index,input_file);
}

size_t helf_loongarch32_load_segment_count_get(helf_file_input_t *input_file)
{
    if(!helf_loongarch32_is_loongarch32(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_count_get(input_file);
}

bool helf_loongarch32_load_segment_info_get(helf_file_input_t *input_file,size_t index,helf_loongarch32_segment_info_t *info)
{
    if(!helf_loongarch32_is_loongarch32(input_file))
    {
        return false;
    }
    return helf_arch_load_segment_info_get(input_file,index,info);
}

size_t helf_loongarch32_load_segment_total_filesz_get(helf_file_input_t *input_file)
{
    if(!helf_loongarch32_is_loongarch32(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_filesz_get(input_file);
}

size_t helf_loongarch32_load_segment_total_memsz_get(helf_file_input_t *input_file)
{
    if(!helf_loongarch32_is_loongarch32(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_memsz_get(input_file);
}

size_t helf_loongarch32_relocation_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_loongarch32_is_loongarch32(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_section_count_get(input_file);
}

bool helf_loongarch32_relocation_section_info_get(helf_file_input_t *input_file,size_t index,helf_arch_relocation_section_info_t *info)
{
    if(!helf_loongarch32_is_loongarch32(input_file))
    {
        return false;
    }
    return helf_arch_relocation_section_info_get(input_file,index,info);
}

size_t helf_loongarch32_relocation_count_get(helf_file_input_t *input_file,size_t section_index)
{
    if(!helf_loongarch32_is_loongarch32(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_count_get(input_file,section_index);
}

bool helf_loongarch32_relocation_get(helf_file_input_t *input_file,size_t section_index,size_t index,helf_arch_relocation_info_t *info)
{
    if(!helf_loongarch32_is_loongarch32(input_file))
    {
        return false;
    }
    if(!helf_arch_relocation_get(input_file,section_index,index,info))
    {
        return false;
    }
    return true;
}

bool helf_loongarch32_relocation_symbol_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_symbol_info_t *info)
{
    if(!helf_loongarch32_is_loongarch32(input_file))
    {
        return false;
    }
    return helf_arch_relocation_symbol_info_get(input_file,relocation,info);
}

static const helf_arch_relocation_type_name_t helf_loongarch32_relocation_type_names[]=
{
    {0,0,"R_LARCH_NONE"},
    {1,4,"R_LARCH_32"},
    {2,8,"R_LARCH_64"},
    {3,4,"R_LARCH_RELATIVE"},
    {4,0,"R_LARCH_COPY"},
    {5,8,"R_LARCH_JUMP_SLOT"},
    {6,4,"R_LARCH_TLS_DTPMOD32"},
    {7,8,"R_LARCH_TLS_DTPMOD64"},
    {8,4,"R_LARCH_TLS_DTPREL32"},
    {9,8,"R_LARCH_TLS_DTPREL64"},
    {10,4,"R_LARCH_TLS_TPREL32"},
    {11,8,"R_LARCH_TLS_TPREL64"},
    {12,8,"R_LARCH_IRELATIVE"},
    {13,4,"R_LARCH_TLS_DESC32"},
    {14,8,"R_LARCH_TLS_DESC64"},
    {20,0,"R_LARCH_MARK_LA"},
    {21,0,"R_LARCH_MARK_PCREL"},
    {22,4,"R_LARCH_SOP_PUSH_PCREL"},
    {23,0,"R_LARCH_SOP_PUSH_ABSOLUTE"},
    {24,0,"R_LARCH_SOP_PUSH_DUP"},
    {25,0,"R_LARCH_SOP_PUSH_GPREL"},
    {26,0,"R_LARCH_SOP_PUSH_TLS_TPREL"},
    {27,0,"R_LARCH_SOP_PUSH_TLS_GOT"},
    {28,0,"R_LARCH_SOP_PUSH_TLS_GD"},
    {29,0,"R_LARCH_SOP_PUSH_PLT_PCREL"},
    {30,0,"R_LARCH_SOP_ASSERT"},
    {31,0,"R_LARCH_SOP_NOT"},
    {32,0,"R_LARCH_SOP_SUB"},
    {33,0,"R_LARCH_SOP_SL"},
    {34,0,"R_LARCH_SOP_SR"},
    {35,0,"R_LARCH_SOP_ADD"},
    {36,0,"R_LARCH_SOP_AND"},
    {37,0,"R_LARCH_SOP_IF_ELSE"},
    {38,4,"R_LARCH_SOP_POP_32_S_10_5"},
    {39,4,"R_LARCH_SOP_POP_32_U_10_12"},
    {40,4,"R_LARCH_SOP_POP_32_S_10_12"},
    {41,4,"R_LARCH_SOP_POP_32_S_10_16"},
    {42,4,"R_LARCH_SOP_POP_32_S_10_16_S2"},
    {43,4,"R_LARCH_SOP_POP_32_S_5_20"},
    {44,4,"R_LARCH_SOP_POP_32_S_0_5_10_16_S2"},
    {45,4,"R_LARCH_SOP_POP_32_S_0_10_10_16_S2"},
    {46,4,"R_LARCH_SOP_POP_32_U"},
    {47,1,"R_LARCH_ADD8"},
    {48,2,"R_LARCH_ADD16"},
    {49,3,"R_LARCH_ADD24"},
    {50,4,"R_LARCH_ADD32"},
    {51,8,"R_LARCH_ADD64"},
    {52,1,"R_LARCH_SUB8"},
    {53,2,"R_LARCH_SUB16"},
    {54,3,"R_LARCH_SUB24"},
    {55,4,"R_LARCH_SUB32"},
    {56,8,"R_LARCH_SUB64"},
    {57,0,"R_LARCH_GNU_VTINHERIT"},
    {58,0,"R_LARCH_GNU_VTENTRY"},
    {64,4,"R_LARCH_B16"},
    {65,4,"R_LARCH_B21"},
    {66,4,"R_LARCH_B26"},
    {67,4,"R_LARCH_ABS_HI20"},
    {68,4,"R_LARCH_ABS_LO12"},
    {69,4,"R_LARCH_ABS64_LO20"},
    {70,4,"R_LARCH_ABS64_HI12"},
    {71,4,"R_LARCH_PCALA_HI20"},
    {72,4,"R_LARCH_PCALA_LO12"},
    {73,4,"R_LARCH_PCALA64_LO20"},
    {74,4,"R_LARCH_PCALA64_HI12"},
    {75,4,"R_LARCH_GOT_PC_HI20"},
    {76,4,"R_LARCH_GOT_PC_LO12"},
    {77,4,"R_LARCH_GOT64_PC_LO20"},
    {78,4,"R_LARCH_GOT64_PC_HI12"},
    {79,4,"R_LARCH_GOT_HI20"},
    {80,4,"R_LARCH_GOT_LO12"},
    {81,4,"R_LARCH_GOT64_LO20"},
    {82,4,"R_LARCH_GOT64_HI12"},
    {83,4,"R_LARCH_TLS_LE_HI20"},
    {84,4,"R_LARCH_TLS_LE_LO12"},
    {85,4,"R_LARCH_TLS_LE64_LO20"},
    {86,4,"R_LARCH_TLS_LE64_HI12"},
    {87,4,"R_LARCH_TLS_IE_PC_HI20"},
    {88,4,"R_LARCH_TLS_IE_PC_LO12"},
    {89,4,"R_LARCH_TLS_IE64_PC_LO20"},
    {90,4,"R_LARCH_TLS_IE64_PC_HI12"},
    {91,4,"R_LARCH_TLS_IE_HI20"},
    {92,4,"R_LARCH_TLS_IE_LO12"},
    {93,4,"R_LARCH_TLS_IE64_LO20"},
    {94,4,"R_LARCH_TLS_IE64_HI12"},
    {95,4,"R_LARCH_TLS_LD_PC_HI20"},
    {96,4,"R_LARCH_TLS_LD_HI20"},
    {97,4,"R_LARCH_TLS_GD_PC_HI20"},
    {98,4,"R_LARCH_TLS_GD_HI20"},
    {99,4,"R_LARCH_32_PCREL"},
    {100,1,"R_LARCH_RELAX"},
    {101,0,"R_LARCH_DELETE"},
    {102,0,"R_LARCH_ALIGN"},
    {103,4,"R_LARCH_PCREL20_S2"},
    {104,0,"R_LARCH_CFA"},
    {105,1,"R_LARCH_ADD6"},
    {106,1,"R_LARCH_SUB6"},
    {107,1,"R_LARCH_ADD_ULEB128"},
    {108,1,"R_LARCH_SUB_ULEB128"},
    {109,8,"R_LARCH_64_PCREL"},
    {110,8,"R_LARCH_CALL36"},
    {111,4,"R_LARCH_TLS_DESC_PC_HI20"},
    {112,4,"R_LARCH_TLS_DESC_PC_LO12"},
    {113,4,"R_LARCH_TLS_DESC64_PC_LO20"},
    {114,4,"R_LARCH_TLS_DESC64_PC_HI12"},
    {115,4,"R_LARCH_TLS_DESC_HI20"},
    {116,4,"R_LARCH_TLS_DESC_LO12"},
    {117,4,"R_LARCH_TLS_DESC64_LO20"},
    {118,4,"R_LARCH_TLS_DESC64_HI12"},
    {119,4,"R_LARCH_TLS_DESC_LD"},
    {120,4,"R_LARCH_TLS_DESC_CALL"},
    {121,4,"R_LARCH_TLS_LE_HI20_R"},
    {122,0,"R_LARCH_TLS_LE_ADD_R"},
    {123,4,"R_LARCH_TLS_LE_LO12_R"},
    {124,4,"R_LARCH_TLS_LD_PCREL20_S2"},
    {125,4,"R_LARCH_TLS_GD_PCREL20_S2"},
    {126,4,"R_LARCH_TLS_DESC_PCREL20_S2"},
    {127,8,"R_LARCH_CALL30"},
    {128,4,"R_LARCH_PCADD_HI20"},
    {129,4,"R_LARCH_PCADD_LO12"},
    {130,4,"R_LARCH_GOT_PCADD_HI20"},
    {131,4,"R_LARCH_GOT_PCADD_LO12"},
    {132,4,"R_LARCH_TLS_IE_PCADD_HI20"},
    {133,4,"R_LARCH_TLS_IE_PCADD_LO12"},
    {134,4,"R_LARCH_TLS_LD_PCADD_HI20"},
    {135,4,"R_LARCH_TLS_LD_PCADD_LO12"},
    {136,4,"R_LARCH_TLS_GD_PCADD_HI20"},
    {137,4,"R_LARCH_TLS_GD_PCADD_LO12"},
    {138,4,"R_LARCH_TLS_DESC_PCADD_HI20"},
    {139,4,"R_LARCH_TLS_DESC_PCADD_LO12"},
};

const char *helf_loongarch32_relocation_type_name_get(uint32_t type)
{
    return helf_arch_relocation_type_name_get(helf_loongarch32_relocation_type_names,sizeof(helf_loongarch32_relocation_type_names)/sizeof(helf_loongarch32_relocation_type_names[0]),type);
}

uint8_t helf_loongarch32_relocation_size_get(uint32_t type)
{
    return helf_arch_relocation_type_size_get(helf_loongarch32_relocation_type_names,sizeof(helf_loongarch32_relocation_type_names)/sizeof(helf_loongarch32_relocation_type_names[0]),type);
}

const char *helf_loongarch32_section_type_name_get(uint32_t type)
{
    (void)type;
    return NULL;
}

bool helf_loongarch32_relocation_address_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_relocation_address_info_t *info)
{
    if(!helf_loongarch32_is_loongarch32(input_file) || relocation==NULL)
    {
        return false;
    }
    uint8_t size=helf_loongarch32_relocation_size_get(relocation->type);
    if(size==0)
    {
        return false;
    }
    return helf_arch_relocation_address_info_get(input_file,relocation,size,info);
}

size_t helf_loongarch32_relocation_data_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,void *buffer,size_t buffer_length)
{
    if(!helf_loongarch32_is_loongarch32(input_file) || relocation==NULL)
    {
        return 0;
    }
    uint8_t size=helf_loongarch32_relocation_size_get(relocation->type);
    if(size==0)
    {
        return 0;
    }
    return helf_arch_relocation_data_get(input_file,relocation,size,buffer,buffer_length);
}

bool helf_loongarch32_is_shared_object(helf_file_input_t *input_file)
{
    return helf_arch_is_shared_object(input_file,helf_loongarch32_machines,sizeof(helf_loongarch32_machines)/sizeof(helf_loongarch32_machines[0]),HELF_ELFCLASS_32);
}

bool helf_loongarch32_is_relocatable(helf_file_input_t *input_file)
{
    return helf_arch_is_relocatable(input_file,helf_loongarch32_machines,sizeof(helf_loongarch32_machines)/sizeof(helf_loongarch32_machines[0]),HELF_ELFCLASS_32);
}

size_t helf_loongarch32_dynamic_count_get(helf_file_input_t *input_file)
{
    if(!helf_loongarch32_is_loongarch32(input_file))
    {
        return 0;
    }
    return helf_arch_dynamic_count_get(input_file);
}

bool helf_loongarch32_dynamic_info_get(helf_file_input_t *input_file,size_t index,helf_arch_dynamic_info_t *info)
{
    if(!helf_loongarch32_is_loongarch32(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_info_get(input_file,index,info);
}

bool helf_loongarch32_dynamic_string_get(helf_file_input_t *input_file,uint64_t offset,char *buffer,size_t buffer_length)
{
    if(!helf_loongarch32_is_loongarch32(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_string_get(input_file,offset,buffer,buffer_length);
}
