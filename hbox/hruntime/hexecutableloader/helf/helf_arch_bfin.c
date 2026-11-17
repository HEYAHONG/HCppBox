/***************************************************************
 * Name:      helf_arch_bfin.c
 * Purpose:   实现helf_arch_bfin接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "helf_arch_bfin.h"

static const uint16_t helf_bfin_machines[]=
{
    HELF_EM_BLACKFIN,
};

bool helf_bfin_is_bfin(helf_file_input_t *input_file)
{
    return helf_arch_is(input_file,helf_bfin_machines,sizeof(helf_bfin_machines)/sizeof(helf_bfin_machines[0]),HELF_ELFCLASS_32);
}

bool helf_bfin_is_big_endian(helf_file_input_t *input_file)
{
    if(!helf_bfin_is_bfin(input_file))
    {
        return false;
    }
    return helf_file_input_is_big_endian(input_file);
}

bool helf_bfin_is_executable(helf_file_input_t *input_file)
{
    return helf_arch_is_executable(input_file,helf_bfin_machines,sizeof(helf_bfin_machines)/sizeof(helf_bfin_machines[0]),HELF_ELFCLASS_32);
}

bool helf_bfin_entry_get(helf_file_input_t *input_file,uint32_t *entry)
{
    if(entry==NULL)
    {
        return false;
    }
    if(!helf_bfin_is_bfin(input_file))
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

size_t helf_bfin_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_bfin_is_bfin(input_file))
    {
        return 0;
    }
    return helf_arch_section_count_get(input_file);
}

bool helf_bfin_section_info_get(helf_file_input_t *input_file,size_t index,helf_bfin_section_info_t *info)
{
    if(!helf_bfin_is_bfin(input_file))
    {
        return false;
    }
    return helf_arch_section_info_get(input_file,index,info);
}

bool helf_bfin_is_rx_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_bfin_is_bfin(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_section(index,input_file);
}

intptr_t helf_bfin_rx_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_bfin_is_rx_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rx_section_offset_get(index,input_file);
}

size_t helf_bfin_rx_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_bfin_is_bfin(input_file))
    {
        return 0;
    }
    return helf_arch_rx_section_size_get(input_file);
}

bool helf_bfin_is_ro_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_bfin_is_bfin(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_section(index,input_file);
}

intptr_t helf_bfin_ro_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_bfin_is_ro_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_ro_section_offset_get(index,input_file);
}

size_t helf_bfin_ro_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_bfin_is_bfin(input_file))
    {
        return 0;
    }
    return helf_arch_ro_section_size_get(input_file);
}

bool helf_bfin_is_rw_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_bfin_is_bfin(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_section(index,input_file);
}

intptr_t helf_bfin_rw_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_bfin_is_rw_section(index,input_file))
    {
        return -1;
    }
    return helf_arch_rw_section_offset_get(index,input_file);
}

size_t helf_bfin_rw_section_size_get(helf_file_input_t *input_file)
{
    if(!helf_bfin_is_bfin(input_file))
    {
        return 0;
    }
    return helf_arch_rw_section_size_get(input_file);
}

bool helf_bfin_is_bss_section(size_t index,helf_file_input_t *input_file)
{
    if(!helf_bfin_is_bfin(input_file))
    {
        return false;
    }
    return helf_arch_is_bss_section(index,input_file);
}

size_t helf_bfin_program_header_count_get(helf_file_input_t *input_file)
{
    if(!helf_bfin_is_bfin(input_file))
    {
        return 0;
    }
    return helf_arch_program_header_count_get(input_file);
}

bool helf_bfin_program_header_get(helf_file_input_t *input_file,size_t index,helf_elf32_program_header_t *programheader)
{
    if(programheader==NULL)
    {
        return false;
    }
    if(!helf_bfin_is_bfin(input_file))
    {
        return false;
    }
    if(index>=helf_arch_program_header_count_get(input_file))
    {
        return false;
    }
    return helf_file_input_32_bits_program_header_get(input_file,index,programheader);
}

size_t helf_bfin_segment_count_get(helf_file_input_t *input_file)
{
    return helf_bfin_program_header_count_get(input_file);
}

bool helf_bfin_segment_info_get(helf_file_input_t *input_file,size_t index,helf_bfin_segment_info_t *info)
{
    if(!helf_bfin_is_bfin(input_file))
    {
        return false;
    }
    return helf_arch_segment_info_get(input_file,index,info);
}

bool helf_bfin_is_load_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_bfin_is_bfin(input_file))
    {
        return false;
    }
    return helf_arch_is_load_segment(index,input_file);
}

bool helf_bfin_is_rx_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_bfin_is_bfin(input_file))
    {
        return false;
    }
    return helf_arch_is_rx_segment(index,input_file);
}

bool helf_bfin_is_ro_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_bfin_is_bfin(input_file))
    {
        return false;
    }
    return helf_arch_is_ro_segment(index,input_file);
}

bool helf_bfin_is_rw_segment(size_t index,helf_file_input_t *input_file)
{
    if(!helf_bfin_is_bfin(input_file))
    {
        return false;
    }
    return helf_arch_is_rw_segment(index,input_file);
}

size_t helf_bfin_load_segment_count_get(helf_file_input_t *input_file)
{
    if(!helf_bfin_is_bfin(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_count_get(input_file);
}

bool helf_bfin_load_segment_info_get(helf_file_input_t *input_file,size_t index,helf_bfin_segment_info_t *info)
{
    if(!helf_bfin_is_bfin(input_file))
    {
        return false;
    }
    return helf_arch_load_segment_info_get(input_file,index,info);
}

size_t helf_bfin_load_segment_total_filesz_get(helf_file_input_t *input_file)
{
    if(!helf_bfin_is_bfin(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_filesz_get(input_file);
}

size_t helf_bfin_load_segment_total_memsz_get(helf_file_input_t *input_file)
{
    if(!helf_bfin_is_bfin(input_file))
    {
        return 0;
    }
    return helf_arch_load_segment_total_memsz_get(input_file);
}

size_t helf_bfin_relocation_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_bfin_is_bfin(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_section_count_get(input_file);
}

bool helf_bfin_relocation_section_info_get(helf_file_input_t *input_file,size_t index,helf_arch_relocation_section_info_t *info)
{
    if(!helf_bfin_is_bfin(input_file))
    {
        return false;
    }
    return helf_arch_relocation_section_info_get(input_file,index,info);
}

size_t helf_bfin_relocation_count_get(helf_file_input_t *input_file,size_t section_index)
{
    if(!helf_bfin_is_bfin(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_count_get(input_file,section_index);
}

bool helf_bfin_relocation_get(helf_file_input_t *input_file,size_t section_index,size_t index,helf_arch_relocation_info_t *info)
{
    if(!helf_bfin_is_bfin(input_file))
    {
        return false;
    }
    if(!helf_arch_relocation_get(input_file,section_index,index,info))
    {
        return false;
    }
    return true;
}

bool helf_bfin_relocation_symbol_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_symbol_info_t *info)
{
    if(!helf_bfin_is_bfin(input_file))
    {
        return false;
    }
    return helf_arch_relocation_symbol_info_get(input_file,relocation,info);
}

static const helf_arch_relocation_type_name_t helf_bfin_relocation_type_names[]=
{
    {0,0,"R_BFIN_UNUSED0"},
    {1,2,"R_BFIN_PCREL5M2"},
    {2,0,"R_BFIN_UNUSED1"},
    {3,2,"R_BFIN_PCREL10"},
    {4,2,"R_BFIN_PCREL12_JUMP"},
    {5,2,"R_BFIN_RIMM16"},
    {6,2,"R_BFIN_LUIMM16"},
    {7,2,"R_BFIN_HUIMM16"},
    {8,2,"R_BFIN_PCREL12_JUMP_S"},
    {9,4,"R_BFIN_PCREL24_JUMP_X"},
    {10,4,"R_BFIN_PCREL24"},
    {11,0,"R_BFIN_UNUSEDB"},
    {12,0,"R_BFIN_UNUSEDC"},
    {13,4,"R_BFIN_PCREL24_JUMP_L"},
    {14,4,"R_BFIN_PCREL24_CALL_X"},
    {15,4,"R_BFIN_VAR_EQ_SYMB"},
    {16,1,"R_BFIN_BYTE_DATA"},
    {17,2,"R_BFIN_BYTE2_DATA"},
    {18,4,"R_BFIN_BYTE4_DATA"},
    {19,2,"R_BFIN_PCREL11"},
    {20,2,"R_BFIN_GOT17M4"},
    {21,2,"R_BFIN_GOTHI"},
    {22,2,"R_BFIN_GOTLO"},
    {23,4,"R_BFIN_FUNCDESC"},
    {24,2,"R_BFIN_FUNCDESC_GOT17M4"},
    {25,2,"R_BFIN_FUNCDESC_GOTHI"},
    {26,2,"R_BFIN_FUNCDESC_GOTLO"},
    {27,4,"R_BFIN_FUNCDESC_VALUE"},
    {28,2,"R_BFIN_FUNCDESC_GOTOFF17M4"},
    {29,2,"R_BFIN_FUNCDESC_GOTOFFHI"},
    {30,2,"R_BFIN_FUNCDESC_GOTOFFLO"},
    {31,2,"R_BFIN_GOTOFF17M4"},
    {32,2,"R_BFIN_GOTOFFHI"},
    {33,2,"R_BFIN_GOTOFFLO"},
    {224,0,"R_BFIN_PUSH"},
    {225,0,"R_BFIN_CONST"},
    {226,0,"R_BFIN_ADD"},
    {227,0,"R_BFIN_SUB"},
    {228,0,"R_BFIN_MULT"},
    {229,0,"R_BFIN_DIV"},
    {230,0,"R_BFIN_MOD"},
    {231,0,"R_BFIN_LSHIFT"},
    {232,0,"R_BFIN_RSHIFT"},
    {233,0,"R_BFIN_AND"},
    {234,0,"R_BFIN_OR"},
    {235,0,"R_BFIN_XOR"},
    {236,0,"R_BFIN_LAND"},
    {237,0,"R_BFIN_LOR"},
    {238,0,"R_BFIN_LEN"},
    {239,0,"R_BFIN_NEG"},
    {240,0,"R_BFIN_COMP"},
    {241,0,"R_BFIN_PAGE"},
    {242,0,"R_BFIN_HWPAGE"},
    {243,0,"R_BFIN_ADDR"},
    {64,2,"R_BFIN_PLTPC"},
    {65,2,"R_BFIN_GOT"},
    {66,4,"R_BFIN_GNU_VTINHERIT"},
    {67,4,"R_BFIN_GNU_VTENTRY"},
};

const char *helf_bfin_relocation_type_name_get(uint32_t type)
{
    return helf_arch_relocation_type_name_get(helf_bfin_relocation_type_names,sizeof(helf_bfin_relocation_type_names)/sizeof(helf_bfin_relocation_type_names[0]),type);
}

uint8_t helf_bfin_relocation_size_get(uint32_t type)
{
    return helf_arch_relocation_type_size_get(helf_bfin_relocation_type_names,sizeof(helf_bfin_relocation_type_names)/sizeof(helf_bfin_relocation_type_names[0]),type);
}

const char *helf_bfin_section_type_name_get(uint32_t type)
{
    (void)type;
    return NULL;
}

bool helf_bfin_relocation_address_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_relocation_address_info_t *info)
{
    if(!helf_bfin_is_bfin(input_file) || relocation==NULL)
    {
        return false;
    }
    uint8_t size=helf_bfin_relocation_size_get(relocation->type);
    if(size==0)
    {
        return false;
    }
    return helf_arch_relocation_address_info_get(input_file,relocation,size,info);
}

size_t helf_bfin_relocation_data_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,void *buffer,size_t buffer_length)
{
    if(!helf_bfin_is_bfin(input_file) || relocation==NULL)
    {
        return 0;
    }
    uint8_t size=helf_bfin_relocation_size_get(relocation->type);
    if(size==0)
    {
        return 0;
    }
    return helf_arch_relocation_data_get(input_file,relocation,size,buffer,buffer_length);
}

bool helf_bfin_is_shared_object(helf_file_input_t *input_file)
{
    return helf_arch_is_shared_object(input_file,helf_bfin_machines,sizeof(helf_bfin_machines)/sizeof(helf_bfin_machines[0]),HELF_ELFCLASS_32);
}

bool helf_bfin_is_relocatable(helf_file_input_t *input_file)
{
    return helf_arch_is_relocatable(input_file,helf_bfin_machines,sizeof(helf_bfin_machines)/sizeof(helf_bfin_machines[0]),HELF_ELFCLASS_32);
}

size_t helf_bfin_dynamic_count_get(helf_file_input_t *input_file)
{
    if(!helf_bfin_is_bfin(input_file))
    {
        return 0;
    }
    return helf_arch_dynamic_count_get(input_file);
}

bool helf_bfin_dynamic_info_get(helf_file_input_t *input_file,size_t index,helf_arch_dynamic_info_t *info)
{
    if(!helf_bfin_is_bfin(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_info_get(input_file,index,info);
}

bool helf_bfin_dynamic_string_get(helf_file_input_t *input_file,uint64_t offset,char *buffer,size_t buffer_length)
{
    if(!helf_bfin_is_bfin(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_string_get(input_file,offset,buffer,buffer_length);
}
