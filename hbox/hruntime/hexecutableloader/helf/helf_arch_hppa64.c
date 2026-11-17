/***************************************************************
 * Name:      helf_arch_hppa64.c
 * Purpose:   实现helf_arch_hppa64接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "helf_arch_hppa64.h"

static const uint16_t helf_hppa64_machines[]=
{
    HELF_EM_PARISC,
};

bool helf_hppa64_is_hppa64(helf_file_input_t *input_file)
{
    return helf_arch_is(input_file,helf_hppa64_machines,sizeof(helf_hppa64_machines)/sizeof(helf_hppa64_machines[0]),HELF_ELFCLASS_64);
}

bool helf_hppa64_is_big_endian(helf_file_input_t *input_file)
{
    if(!helf_hppa64_is_hppa64(input_file))
    {
        return false;
    }
    return helf_file_input_is_big_endian(input_file);
}

bool helf_hppa64_is_executable(helf_file_input_t *input_file)
{
    return helf_arch_is_executable(input_file,helf_hppa64_machines,sizeof(helf_hppa64_machines)/sizeof(helf_hppa64_machines[0]),HELF_ELFCLASS_64);
}

bool helf_hppa64_entry_get(helf_file_input_t *input_file,uint64_t *entry)
{
    if(entry==NULL)
    {
        return false;
    }
    if(!helf_hppa64_is_hppa64(input_file))
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

size_t helf_hppa64_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_hppa64_is_hppa64(input_file))
    {
        return 0;
    }
    return helf_arch_section_count_get(input_file);
}

bool helf_hppa64_section_info_get(helf_file_input_t *input_file,size_t index,helf_hppa64_section_info_t *info)
{
    if(!helf_hppa64_is_hppa64(input_file))
    {
        return false;
    }
    return helf_arch_section_info_get(input_file,index,info);
}

bool helf_hppa64_is_rx_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_hppa64_is_hppa64(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_section(index,input_file);
}

intptr_t helf_hppa64_rx_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_hppa64_is_rx_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rx_section_offset_get(index,input_file);
}

size_t helf_hppa64_rx_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_hppa64_is_hppa64(input_file))
    {
        return 0;
    }
    return helf_arch_rx_section_size_get(input_file);
}

bool helf_hppa64_is_ro_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_hppa64_is_hppa64(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_section(index,input_file);
}

intptr_t helf_hppa64_ro_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_hppa64_is_ro_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_ro_section_offset_get(index,input_file);
}

size_t helf_hppa64_ro_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_hppa64_is_hppa64(input_file))
    {
        return 0;
    }
    return helf_arch_ro_section_size_get(input_file);
}

bool helf_hppa64_is_rw_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_hppa64_is_hppa64(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_section(index,input_file);
}

intptr_t helf_hppa64_rw_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_hppa64_is_rw_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rw_section_offset_get(index,input_file);
}

size_t helf_hppa64_rw_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_hppa64_is_hppa64(input_file))
    {
        return 0;
    }
    return helf_arch_rw_section_size_get(input_file);
}

bool helf_hppa64_is_bss_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_hppa64_is_hppa64(input_file))
    {
        return false;
    }
    return helf_arch_is_bss_section(index,input_file);
}

size_t helf_hppa64_program_header_count_get(helf_file_input_t *input_file)
{
    if(!helf_hppa64_is_hppa64(input_file))
    {
        return 0;
    }
    return helf_arch_program_header_count_get(input_file);
}

bool helf_hppa64_program_header_get(helf_file_input_t *input_file,size_t index,helf_elf64_program_header_t *programheader)
{
    if(programheader==NULL)
    {
        return false;
    }
    if(!helf_hppa64_is_hppa64(input_file))
    {
        return false;
    }
    if(index>=helf_arch_program_header_count_get(input_file))
    {
        return false;
    }
    return helf_file_input_64_bits_program_header_get(input_file,index,programheader);
}

size_t helf_hppa64_segment_count_get(helf_file_input_t *input_file)
{
    return helf_hppa64_program_header_count_get(input_file);
}

bool helf_hppa64_segment_info_get(helf_file_input_t *input_file,size_t index,helf_hppa64_segment_info_t *info)
{
    if(!helf_hppa64_is_hppa64(input_file))
    {
        return false;
    }
    return helf_arch_segment_info_get(input_file,index,info);
}

bool helf_hppa64_is_load_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_hppa64_is_hppa64(input_file))
    {
        return false;
    }
    return helf_arch_is_load_segment(index,input_file);
}

bool helf_hppa64_is_rx_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_hppa64_is_hppa64(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_segment(index,input_file);
}

bool helf_hppa64_is_ro_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_hppa64_is_hppa64(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_segment(index,input_file);
}

bool helf_hppa64_is_rw_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_hppa64_is_hppa64(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_segment(index,input_file);
}

size_t helf_hppa64_load_segment_count_get(helf_file_input_t *input_file)
{
    if(!helf_hppa64_is_hppa64(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_count_get(input_file);
}

bool helf_hppa64_load_segment_info_get(helf_file_input_t *input_file,size_t index,helf_hppa64_segment_info_t *info)
{
    if(!helf_hppa64_is_hppa64(input_file))
    {
        return false;
    }
    return helf_arch_load_segment_info_get(input_file,index,info);
}

size_t helf_hppa64_load_segment_total_filesz_get(helf_file_input_t *input_file)
{
    if(!helf_hppa64_is_hppa64(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_filesz_get(input_file);
}

size_t helf_hppa64_load_segment_total_memsz_get(helf_file_input_t *input_file)
{
    if(!helf_hppa64_is_hppa64(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_memsz_get(input_file);
}

size_t helf_hppa64_relocation_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_hppa64_is_hppa64(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_section_count_get(input_file);
}

bool helf_hppa64_relocation_section_info_get(helf_file_input_t *input_file,size_t index,helf_arch_relocation_section_info_t *info)
{
    if(!helf_hppa64_is_hppa64(input_file))
    {
        return false;
    }
    return helf_arch_relocation_section_info_get(input_file,index,info);
}

size_t helf_hppa64_relocation_count_get(helf_file_input_t *input_file,size_t section_index)
{
    if(!helf_hppa64_is_hppa64(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_count_get(input_file,section_index);
}

bool helf_hppa64_relocation_get(helf_file_input_t *input_file,size_t section_index,size_t index,helf_arch_relocation_info_t *info)
{
    if(!helf_hppa64_is_hppa64(input_file))
    {
        return false;
    }
    if(!helf_arch_relocation_get(input_file,section_index,index,info))
    {
        return false;
    }
    return true;
}

bool helf_hppa64_relocation_symbol_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_symbol_info_t *info)
{
    if(!helf_hppa64_is_hppa64(input_file))
    {
        return false;
    }
    return helf_arch_relocation_symbol_info_get(input_file,relocation,info);
}

static const helf_arch_relocation_type_name_t helf_hppa64_relocation_type_names[]=
{
    {0,0,"R_PARISC_NONE"},
    {1,4,"R_PARISC_DIR32"},
    {2,0,"R_PARISC_DIR21L"},
    {3,0,"R_PARISC_DIR17R"},
    {4,0,"R_PARISC_DIR17F"},
    {6,0,"R_PARISC_DIR14R"},
    {7,0,"R_PARISC_DIR14F"},
    {8,0,"R_PARISC_PCREL12F"},
    {9,4,"R_PARISC_PCREL32"},
    {10,0,"R_PARISC_PCREL21L"},
    {11,0,"R_PARISC_PCREL17R"},
    {12,0,"R_PARISC_PCREL17F"},
    {13,0,"R_PARISC_PCREL17C"},
    {14,0,"R_PARISC_PCREL14R"},
    {15,0,"R_PARISC_PCREL14F"},
    {18,0,"R_PARISC_DPREL21L"},
    {19,0,"R_PARISC_DPREL14WR"},
    {20,0,"R_PARISC_DPREL14DR"},
    {22,0,"R_PARISC_DPREL14R"},
    {23,0,"R_PARISC_DPREL14F"},
    {26,0,"R_PARISC_DLTREL21L"},
    {30,0,"R_PARISC_DLTREL14R"},
    {31,0,"R_PARISC_DLTREL14F"},
    {34,0,"R_PARISC_DLTIND21L"},
    {38,0,"R_PARISC_DLTIND14R"},
    {39,0,"R_PARISC_DLTIND14F"},
    {40,0,"R_PARISC_SETBASE"},
    {41,4,"R_PARISC_SECREL32"},
    {42,0,"R_PARISC_BASEREL21L"},
    {43,0,"R_PARISC_BASEREL17R"},
    {44,0,"R_PARISC_BASEREL17F"},
    {46,0,"R_PARISC_BASEREL14R"},
    {47,0,"R_PARISC_BASEREL14F"},
    {48,0,"R_PARISC_SEGBASE"},
    {49,4,"R_PARISC_SEGREL32"},
    {50,0,"R_PARISC_PLTOFF21L"},
    {54,0,"R_PARISC_PLTOFF14R"},
    {55,0,"R_PARISC_PLTOFF14F"},
    {57,4,"R_PARISC_LTOFF_FPTR32"},
    {58,0,"R_PARISC_LTOFF_FPTR21L"},
    {62,0,"R_PARISC_LTOFF_FPTR14R"},
    {64,8,"R_PARISC_FPTR64"},
    {65,4,"R_PARISC_PLABEL32"},
    {66,0,"R_PARISC_PLABEL21L"},
    {70,0,"R_PARISC_PLABEL14R"},
    {72,8,"R_PARISC_PCREL64"},
    {73,0,"R_PARISC_PCREL22C"},
    {74,0,"R_PARISC_PCREL22F"},
    {75,0,"R_PARISC_PCREL14WR"},
    {76,0,"R_PARISC_PCREL14DR"},
    {77,0,"R_PARISC_PCREL16F"},
    {78,0,"R_PARISC_PCREL16WF"},
    {79,0,"R_PARISC_PCREL16DF"},
    {80,8,"R_PARISC_DIR64"},
    {83,0,"R_PARISC_DIR14WR"},
    {84,0,"R_PARISC_DIR14DR"},
    {85,0,"R_PARISC_DIR16F"},
    {86,0,"R_PARISC_DIR16WF"},
    {87,0,"R_PARISC_DIR16DF"},
    {88,8,"R_PARISC_GPREL64"},
    {91,0,"R_PARISC_DLTREL14WR"},
    {92,0,"R_PARISC_DLTREL14DR"},
    {93,0,"R_PARISC_GPREL16F"},
    {94,0,"R_PARISC_GPREL16WF"},
    {95,0,"R_PARISC_GPREL16DF"},
    {96,8,"R_PARISC_LTOFF64"},
    {99,0,"R_PARISC_DLTIND14WR"},
    {100,0,"R_PARISC_DLTIND14DR"},
    {101,0,"R_PARISC_LTOFF16F"},
    {102,0,"R_PARISC_LTOFF16WF"},
    {103,0,"R_PARISC_LTOFF16DF"},
    {104,8,"R_PARISC_SECREL64"},
    {107,0,"R_PARISC_BASEREL14WR"},
    {108,0,"R_PARISC_BASEREL14DR"},
    {112,8,"R_PARISC_SEGREL64"},
    {115,0,"R_PARISC_PLTOFF14WR"},
    {116,0,"R_PARISC_PLTOFF14DR"},
    {117,0,"R_PARISC_PLTOFF16F"},
    {118,0,"R_PARISC_PLTOFF16WF"},
    {119,0,"R_PARISC_PLTOFF16DF"},
    {120,8,"R_PARISC_LTOFF_FPTR64"},
    {123,0,"R_PARISC_LTOFF_FPTR14WR"},
    {124,0,"R_PARISC_LTOFF_FPTR14DR"},
    {125,0,"R_PARISC_LTOFF_FPTR16F"},
    {126,0,"R_PARISC_LTOFF_FPTR16WF"},
    {127,0,"R_PARISC_LTOFF_FPTR16DF"},
    {128,0,"R_PARISC_COPY"},
    {129,0,"R_PARISC_IPLT"},
    {130,0,"R_PARISC_EPLT"},
    {153,4,"R_PARISC_TPREL32"},
    {154,0,"R_PARISC_TPREL21L"},
    {158,0,"R_PARISC_TPREL14R"},
    {162,0,"R_PARISC_LTOFF_TP21L"},
    {166,0,"R_PARISC_LTOFF_TP14R"},
    {167,0,"R_PARISC_LTOFF_TP14F"},
    {216,8,"R_PARISC_TPREL64"},
    {219,0,"R_PARISC_TPREL14WR"},
    {220,0,"R_PARISC_TPREL14DR"},
    {221,0,"R_PARISC_TPREL16F"},
    {222,0,"R_PARISC_TPREL16WF"},
    {223,0,"R_PARISC_TPREL16DF"},
    {224,8,"R_PARISC_LTOFF_TP64"},
    {227,0,"R_PARISC_LTOFF_TP14WR"},
    {228,0,"R_PARISC_LTOFF_TP14DR"},
    {229,0,"R_PARISC_LTOFF_TP16F"},
    {230,0,"R_PARISC_LTOFF_TP16WF"},
    {231,0,"R_PARISC_LTOFF_TP16DF"},
    {232,0,"R_PARISC_GNU_VTENTRY"},
    {233,0,"R_PARISC_GNU_VTINHERIT"},
    {234,0,"R_PARISC_TLS_GD21L"},
    {235,0,"R_PARISC_TLS_GD14R"},
    {236,0,"R_PARISC_TLS_GDCALL"},
    {237,0,"R_PARISC_TLS_LDM21L"},
    {238,0,"R_PARISC_TLS_LDM14R"},
    {239,0,"R_PARISC_TLS_LDMCALL"},
    {240,0,"R_PARISC_TLS_LDO21L"},
    {241,0,"R_PARISC_TLS_LDO14R"},
    {242,4,"R_PARISC_TLS_DTPMOD32"},
    {243,8,"R_PARISC_TLS_DTPMOD64"},
    {244,4,"R_PARISC_TLS_DTPOFF32"},
    {245,8,"R_PARISC_TLS_DTPOFF64"},
};

const char *helf_hppa64_relocation_type_name_get(uint32_t type)
{
    return helf_arch_relocation_type_name_get(helf_hppa64_relocation_type_names,sizeof(helf_hppa64_relocation_type_names)/sizeof(helf_hppa64_relocation_type_names[0]),type);
}

uint8_t helf_hppa64_relocation_size_get(uint32_t type)
{
    return helf_arch_relocation_type_size_get(helf_hppa64_relocation_type_names,sizeof(helf_hppa64_relocation_type_names)/sizeof(helf_hppa64_relocation_type_names[0]),type);
}

static const helf_arch_section_type_name_t helf_hppa64_section_type_names[]=
{
    {1879048192,"SHT_PARISC_EXT"},
    {1879048193,"SHT_PARISC_UNWIND"},
    {1879048194,"SHT_PARISC_DOC"},
    {1879048195,"SHT_PARISC_ANNOT"},
    {1879048196,"SHT_PARISC_DLKM"},
    {1879048200,"SHT_PARISC_SYMEXTN"},
    {1879048201,"SHT_PARISC_STUBS"},
    {1610612736,"SHT_HP_OVLBITS"},
    {1610612737,"SHT_HP_DLKM"},
    {1610612738,"SHT_HP_COMDAT"},
    {1610612739,"SHT_HP_OBJDICT"},
    {1610612740,"SHT_HP_ANNOT"},
};

const char *helf_hppa64_section_type_name_get(uint32_t type)
{
    return helf_arch_section_type_name_get(helf_hppa64_section_type_names,sizeof(helf_hppa64_section_type_names)/sizeof(helf_hppa64_section_type_names[0]),type);
}

bool helf_hppa64_relocation_address_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_relocation_address_info_t *info)
{
    if(!helf_hppa64_is_hppa64(input_file) || relocation==NULL)
    {
        return false;
    }
    uint8_t size=helf_hppa64_relocation_size_get(relocation->type);
    if(size==0)
    {
        return false;
    }
    return helf_arch_relocation_address_info_get(input_file,relocation,size,info);
}

size_t helf_hppa64_relocation_data_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,void *buffer,size_t buffer_length)
{
    if(!helf_hppa64_is_hppa64(input_file) || relocation==NULL)
    {
        return 0;
    }
    uint8_t size=helf_hppa64_relocation_size_get(relocation->type);
    if(size==0)
    {
        return 0;
    }
    return helf_arch_relocation_data_get(input_file,relocation,size,buffer,buffer_length);
}

bool helf_hppa64_is_shared_object(helf_file_input_t *input_file)
{
    return helf_arch_is_shared_object(input_file,helf_hppa64_machines,sizeof(helf_hppa64_machines)/sizeof(helf_hppa64_machines[0]),HELF_ELFCLASS_64);
}

bool helf_hppa64_is_relocatable(helf_file_input_t *input_file)
{
    return helf_arch_is_relocatable(input_file,helf_hppa64_machines,sizeof(helf_hppa64_machines)/sizeof(helf_hppa64_machines[0]),HELF_ELFCLASS_64);
}

size_t helf_hppa64_dynamic_count_get(helf_file_input_t *input_file)
{
    if(!helf_hppa64_is_hppa64(input_file))
    {
        return 0;
    }
    return helf_arch_dynamic_count_get(input_file);
}

bool helf_hppa64_dynamic_info_get(helf_file_input_t *input_file,size_t index,helf_arch_dynamic_info_t *info)
{
    if(!helf_hppa64_is_hppa64(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_info_get(input_file,index,info);
}

bool helf_hppa64_dynamic_string_get(helf_file_input_t *input_file,uint64_t offset,char *buffer,size_t buffer_length)
{
    if(!helf_hppa64_is_hppa64(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_string_get(input_file,offset,buffer,buffer_length);
}
