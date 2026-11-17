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
