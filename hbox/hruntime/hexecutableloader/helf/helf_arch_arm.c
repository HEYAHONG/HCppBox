/***************************************************************
 * Name:      helf_arch_arm.c
 * Purpose:   实现helf_arch_arm接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "helf_arch_arm.h"

static const uint16_t helf_arm_machines[]=
{
    HELF_EM_ARM,
};

bool helf_arm_is_arm(helf_file_input_t *input_file)
{
    return helf_arch_is(input_file,helf_arm_machines,sizeof(helf_arm_machines)/sizeof(helf_arm_machines[0]),HELF_ELFCLASS_32);
}

bool helf_arm_is_big_endian(helf_file_input_t *input_file)
{
    if(!helf_arm_is_arm(input_file))
    {
        return false;
    }
    return helf_file_input_is_big_endian(input_file);
}

bool helf_arm_is_executable(helf_file_input_t *input_file)
{
    return helf_arch_is_executable(input_file,helf_arm_machines,sizeof(helf_arm_machines)/sizeof(helf_arm_machines[0]),HELF_ELFCLASS_32);
}

bool helf_arm_entry_get(helf_file_input_t *input_file,uint32_t *entry)
{
    if(entry==NULL)
    {
        return false;
    }
    if(!helf_arm_is_arm(input_file))
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

size_t helf_arm_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_arm_is_arm(input_file))
    {
        return 0;
    }
    return helf_arch_section_count_get(input_file);
}

bool helf_arm_section_info_get(helf_file_input_t *input_file,size_t index,helf_arm_section_info_t *info)
{
    if(!helf_arm_is_arm(input_file))
    {
        return false;
    }
    return helf_arch_section_info_get(input_file,index,info);
}

bool helf_arm_is_rx_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_arm_is_arm(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_section(index,input_file);
}

intptr_t helf_arm_rx_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_arm_is_rx_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rx_section_offset_get(index,input_file);
}

size_t helf_arm_rx_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_arm_is_arm(input_file))
    {
        return 0;
    }
    return helf_arch_rx_section_size_get(input_file);
}

bool helf_arm_is_ro_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_arm_is_arm(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_section(index,input_file);
}

intptr_t helf_arm_ro_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_arm_is_ro_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_ro_section_offset_get(index,input_file);
}

size_t helf_arm_ro_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_arm_is_arm(input_file))
    {
        return 0;
    }
    return helf_arch_ro_section_size_get(input_file);
}

bool helf_arm_is_rw_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_arm_is_arm(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_section(index,input_file);
}

intptr_t helf_arm_rw_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_arm_is_rw_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rw_section_offset_get(index,input_file);
}

size_t helf_arm_rw_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_arm_is_arm(input_file))
    {
        return 0;
    }
    return helf_arch_rw_section_size_get(input_file);
}

bool helf_arm_is_bss_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_arm_is_arm(input_file))
    {
        return false;
    }
    return helf_arch_is_bss_section(index,input_file);
}

size_t helf_arm_program_header_count_get(helf_file_input_t *input_file)
{
    if(!helf_arm_is_arm(input_file))
    {
        return 0;
    }
    return helf_arch_program_header_count_get(input_file);
}

bool helf_arm_program_header_get(helf_file_input_t *input_file,size_t index,helf_elf32_program_header_t *programheader)
{
    if(programheader==NULL)
    {
        return false;
    }
    if(!helf_arm_is_arm(input_file))
    {
        return false;
    }
    if(index>=helf_arch_program_header_count_get(input_file))
    {
        return false;
    }
    return helf_file_input_32_bits_program_header_get(input_file,index,programheader);
}

size_t helf_arm_segment_count_get(helf_file_input_t *input_file)
{
    return helf_arm_program_header_count_get(input_file);
}

bool helf_arm_segment_info_get(helf_file_input_t *input_file,size_t index,helf_arm_segment_info_t *info)
{
    if(!helf_arm_is_arm(input_file))
    {
        return false;
    }
    return helf_arch_segment_info_get(input_file,index,info);
}

bool helf_arm_is_load_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_arm_is_arm(input_file))
    {
        return false;
    }
    return helf_arch_is_load_segment(index,input_file);
}

bool helf_arm_is_rx_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_arm_is_arm(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_segment(index,input_file);
}

bool helf_arm_is_ro_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_arm_is_arm(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_segment(index,input_file);
}

bool helf_arm_is_rw_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_arm_is_arm(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_segment(index,input_file);
}

size_t helf_arm_load_segment_count_get(helf_file_input_t *input_file)
{
    if(!helf_arm_is_arm(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_count_get(input_file);
}

bool helf_arm_load_segment_info_get(helf_file_input_t *input_file,size_t index,helf_arm_segment_info_t *info)
{
    if(!helf_arm_is_arm(input_file))
    {
        return false;
    }
    return helf_arch_load_segment_info_get(input_file,index,info);
}

size_t helf_arm_load_segment_total_filesz_get(helf_file_input_t *input_file)
{
    if(!helf_arm_is_arm(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_filesz_get(input_file);
}

size_t helf_arm_load_segment_total_memsz_get(helf_file_input_t *input_file)
{
    if(!helf_arm_is_arm(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_memsz_get(input_file);
}

size_t helf_arm_relocation_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_arm_is_arm(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_section_count_get(input_file);
}

bool helf_arm_relocation_section_info_get(helf_file_input_t *input_file,size_t index,helf_arch_relocation_section_info_t *info)
{
    if(!helf_arm_is_arm(input_file))
    {
        return false;
    }
    return helf_arch_relocation_section_info_get(input_file,index,info);
}

size_t helf_arm_relocation_count_get(helf_file_input_t *input_file,size_t section_index)
{
    if(!helf_arm_is_arm(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_count_get(input_file,section_index);
}

bool helf_arm_relocation_get(helf_file_input_t *input_file,size_t section_index,size_t index,helf_arch_relocation_info_t *info)
{
    if(!helf_arm_is_arm(input_file))
    {
        return false;
    }
    if(!helf_arch_relocation_get(input_file,section_index,index,info))
    {
        return false;
    }
    return true;
}

bool helf_arm_relocation_symbol_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_symbol_info_t *info)
{
    if(!helf_arm_is_arm(input_file))
    {
        return false;
    }
    return helf_arch_relocation_symbol_info_get(input_file,relocation,info);
}

static const helf_arch_relocation_type_name_t helf_arm_relocation_type_names[]=
{
    {0,0,"R_ARM_NONE"},
    {1,4,"R_ARM_PC24"},
    {2,4,"R_ARM_ABS32"},
    {3,4,"R_ARM_REL32"},
    {4,1,"R_ARM_LDR_PC_G0"},
    {5,2,"R_ARM_ABS16"},
    {6,4,"R_ARM_ABS12"},
    {7,2,"R_ARM_THM_ABS5"},
    {8,1,"R_ARM_ABS8"},
    {9,4,"R_ARM_SBREL32"},
    {10,4,"R_ARM_THM_CALL"},
    {11,2,"R_ARM_THM_PC8"},
    {12,2,"R_ARM_BREL_ADJ"},
    {13,4,"R_ARM_TLS_DESC"},
    {14,0,"R_ARM_THM_SWI8"},
    {15,4,"R_ARM_XPC25"},
    {16,4,"R_ARM_THM_XPC22"},
    {17,4,"R_ARM_TLS_DTPMOD32"},
    {18,4,"R_ARM_TLS_DTPOFF32"},
    {19,4,"R_ARM_TLS_TPOFF32"},
    {20,4,"R_ARM_COPY"},
    {21,4,"R_ARM_GLOB_DAT"},
    {22,4,"R_ARM_JUMP_SLOT"},
    {23,4,"R_ARM_RELATIVE"},
    {24,4,"R_ARM_GOTOFF32"},
    {25,0,"R_ARM_BASE_PREL"},
    {26,0,"R_ARM_GOT_BREL"},
    {27,4,"R_ARM_PLT32"},
    {28,4,"R_ARM_CALL"},
    {29,4,"R_ARM_JUMP24"},
    {30,4,"R_ARM_THM_JUMP24"},
    {31,4,"R_ARM_BASE_ABS"},
    {32,4,"R_ARM_ALU_PCREL7_0"},
    {33,4,"R_ARM_ALU_PCREL15_8"},
    {34,4,"R_ARM_ALU_PCREL23_15"},
    {35,4,"R_ARM_LDR_SBREL_11_0"},
    {36,4,"R_ARM_ALU_SBREL_19_12"},
    {37,4,"R_ARM_ALU_SBREL_27_20"},
    {38,4,"R_ARM_TARGET1"},
    {39,0,"R_ARM_SBREL31"},
    {40,4,"R_ARM_V4BX"},
    {41,4,"R_ARM_TARGET2"},
    {42,4,"R_ARM_PREL31"},
    {43,4,"R_ARM_MOVW_ABS_NC"},
    {44,4,"R_ARM_MOVT_ABS"},
    {45,4,"R_ARM_MOVW_PREL_NC"},
    {46,4,"R_ARM_MOVT_PREL"},
    {47,4,"R_ARM_THM_MOVW_ABS_NC"},
    {48,4,"R_ARM_THM_MOVT_ABS"},
    {49,4,"R_ARM_THM_MOVW_PREL_NC"},
    {50,4,"R_ARM_THM_MOVT_PREL"},
    {51,4,"R_ARM_THM_JUMP19"},
    {52,2,"R_ARM_THM_JUMP6"},
    {53,4,"R_ARM_THM_ALU_PREL_11_0"},
    {54,4,"R_ARM_THM_PC12"},
    {55,4,"R_ARM_ABS32_NOI"},
    {56,4,"R_ARM_REL32_NOI"},
    {57,4,"R_ARM_ALU_PC_G0_NC"},
    {58,4,"R_ARM_ALU_PC_G0"},
    {59,4,"R_ARM_ALU_PC_G1_NC"},
    {60,4,"R_ARM_ALU_PC_G1"},
    {61,4,"R_ARM_ALU_PC_G2"},
    {62,4,"R_ARM_LDR_PC_G1"},
    {63,4,"R_ARM_LDR_PC_G2"},
    {64,4,"R_ARM_LDRS_PC_G0"},
    {65,4,"R_ARM_LDRS_PC_G1"},
    {66,4,"R_ARM_LDRS_PC_G2"},
    {67,4,"R_ARM_LDC_PC_G0"},
    {68,4,"R_ARM_LDC_PC_G1"},
    {69,4,"R_ARM_LDC_PC_G2"},
    {70,4,"R_ARM_ALU_SB_G0_NC"},
    {71,4,"R_ARM_ALU_SB_G0"},
    {72,4,"R_ARM_ALU_SB_G1_NC"},
    {73,4,"R_ARM_ALU_SB_G1"},
    {74,4,"R_ARM_ALU_SB_G2"},
    {75,4,"R_ARM_LDR_SB_G0"},
    {76,4,"R_ARM_LDR_SB_G1"},
    {77,4,"R_ARM_LDR_SB_G2"},
    {78,4,"R_ARM_LDRS_SB_G0"},
    {79,4,"R_ARM_LDRS_SB_G1"},
    {80,4,"R_ARM_LDRS_SB_G2"},
    {81,4,"R_ARM_LDC_SB_G0"},
    {82,4,"R_ARM_LDC_SB_G1"},
    {83,4,"R_ARM_LDC_SB_G2"},
    {84,4,"R_ARM_MOVW_BREL_NC"},
    {85,4,"R_ARM_MOVT_BREL"},
    {86,4,"R_ARM_MOVW_BREL"},
    {87,4,"R_ARM_THM_MOVW_BREL_NC"},
    {88,4,"R_ARM_THM_MOVT_BREL"},
    {89,4,"R_ARM_THM_MOVW_BREL"},
    {90,4,"R_ARM_TLS_GOTDESC"},
    {91,4,"R_ARM_TLS_CALL"},
    {92,4,"R_ARM_TLS_DESCSEQ"},
    {93,4,"R_ARM_THM_TLS_CALL"},
    {94,4,"R_ARM_PLT32_ABS"},
    {95,4,"R_ARM_GOT_ABS"},
    {96,4,"R_ARM_GOT_PREL"},
    {97,4,"R_ARM_GOT_BREL12"},
    {98,4,"R_ARM_GOTOFF12"},
    {99,0,"R_ARM_GOTRELAX"},
    {100,4,"R_ARM_GNU_VTENTRY"},
    {101,4,"R_ARM_GNU_VTINHERIT"},
    {102,2,"R_ARM_THM_JUMP11"},
    {103,2,"R_ARM_THM_JUMP8"},
    {104,4,"R_ARM_TLS_GD32"},
    {105,4,"R_ARM_TLS_LDM32"},
    {106,4,"R_ARM_TLS_LDO32"},
    {107,4,"R_ARM_TLS_IE32"},
    {108,4,"R_ARM_TLS_LE32"},
    {109,4,"R_ARM_TLS_LDO12"},
    {110,4,"R_ARM_TLS_LE12"},
    {111,4,"R_ARM_TLS_IE12GP"},
    {128,0,"R_ARM_ME_TOO"},
    {129,2,"R_ARM_THM_TLS_DESCSEQ"},
    {132,2,"R_ARM_THM_ALU_ABS_G0_NC"},
    {133,2,"R_ARM_THM_ALU_ABS_G1_NC"},
    {134,2,"R_ARM_THM_ALU_ABS_G2_NC"},
    {135,2,"R_ARM_THM_ALU_ABS_G3_NC"},
    {136,2,"R_ARM_THM_BF16"},
    {137,2,"R_ARM_THM_BF12"},
    {138,2,"R_ARM_THM_BF18"},
    {160,4,"R_ARM_IRELATIVE"},
    {161,4,"R_ARM_GOTFUNCDESC"},
    {162,4,"R_ARM_GOTOFFFUNCDESC"},
    {163,4,"R_ARM_FUNCDESC"},
    {164,4,"R_ARM_FUNCDESC_VALUE"},
    {165,4,"R_ARM_TLS_GD32_FDPIC"},
    {166,4,"R_ARM_TLS_LDM32_FDPIC"},
    {167,4,"R_ARM_TLS_IE32_FDPIC"},
    {249,0,"R_ARM_RXPC25"},
    {250,4,"R_ARM_RSBREL32"},
    {251,0,"R_ARM_THM_RPC22"},
    {252,0,"R_ARM_RREL32"},
    {253,0,"R_ARM_RABS32"},
    {254,0,"R_ARM_RPC24"},
    {255,0,"R_ARM_RBASE"},
};

const char *helf_arm_relocation_type_name_get(uint32_t type)
{
    return helf_arch_relocation_type_name_get(helf_arm_relocation_type_names,sizeof(helf_arm_relocation_type_names)/sizeof(helf_arm_relocation_type_names[0]),type);
}

uint8_t helf_arm_relocation_size_get(uint32_t type)
{
    return helf_arch_relocation_type_size_get(helf_arm_relocation_type_names,sizeof(helf_arm_relocation_type_names)/sizeof(helf_arm_relocation_type_names[0]),type);
}

static const helf_arch_section_type_name_t helf_arm_section_type_names[]=
{
    {1879048193,"SHT_ARM_EXIDX"},
    {1879048194,"SHT_ARM_PREEMPTMAP"},
    {1879048195,"SHT_ARM_ATTRIBUTES"},
    {1879048196,"SHT_ARM_DEBUGOVERLAY"},
    {1879048197,"SHT_ARM_OVERLAYSECTION"},
};

const char *helf_arm_section_type_name_get(uint32_t type)
{
    return helf_arch_section_type_name_get(helf_arm_section_type_names,sizeof(helf_arm_section_type_names)/sizeof(helf_arm_section_type_names[0]),type);
}

bool helf_arm_relocation_address_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_relocation_address_info_t *info)
{
    if(!helf_arm_is_arm(input_file) || relocation==NULL)
    {
        return false;
    }
    uint8_t size=helf_arm_relocation_size_get(relocation->type);
    if(size==0)
    {
        return false;
    }
    return helf_arch_relocation_address_info_get(input_file,relocation,size,info);
}

size_t helf_arm_relocation_data_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,void *buffer,size_t buffer_length)
{
    if(!helf_arm_is_arm(input_file) || relocation==NULL)
    {
        return 0;
    }
    uint8_t size=helf_arm_relocation_size_get(relocation->type);
    if(size==0)
    {
        return 0;
    }
    return helf_arch_relocation_data_get(input_file,relocation,size,buffer,buffer_length);
}

bool helf_arm_is_shared_object(helf_file_input_t *input_file)
{
    return helf_arch_is_shared_object(input_file,helf_arm_machines,sizeof(helf_arm_machines)/sizeof(helf_arm_machines[0]),HELF_ELFCLASS_32);
}

bool helf_arm_is_relocatable(helf_file_input_t *input_file)
{
    return helf_arch_is_relocatable(input_file,helf_arm_machines,sizeof(helf_arm_machines)/sizeof(helf_arm_machines[0]),HELF_ELFCLASS_32);
}

size_t helf_arm_dynamic_count_get(helf_file_input_t *input_file)
{
    if(!helf_arm_is_arm(input_file))
    {
        return 0;
    }
    return helf_arch_dynamic_count_get(input_file);
}

bool helf_arm_dynamic_info_get(helf_file_input_t *input_file,size_t index,helf_arch_dynamic_info_t *info)
{
    if(!helf_arm_is_arm(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_info_get(input_file,index,info);
}

bool helf_arm_dynamic_string_get(helf_file_input_t *input_file,uint64_t offset,char *buffer,size_t buffer_length)
{
    if(!helf_arm_is_arm(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_string_get(input_file,offset,buffer,buffer_length);
}
