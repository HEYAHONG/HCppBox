/***************************************************************
 * Name:      helf_common.c
 * Purpose:   实现helf_common接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-06-09
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "helf_common.h"

bool helf_is_elf(helf_e_ident_t e_ident)
{
    if(e_ident!=NULL)
    {
        if(e_ident[HELF_EI_MAG0] == 0x7F && e_ident[HELF_EI_MAG1] == 'E' && e_ident[HELF_EI_MAG2] == 'L' && e_ident[HELF_EI_MAG3] == 'F')
        {
            return true;
        }
    }
    return false;
}


uint8_t helf_elfclass_get(helf_e_ident_t e_ident)
{
    if(helf_is_elf(e_ident))
    {
        return e_ident[HELF_EI_CLASS];
    }
    return HELF_ELFCLASS_NONE;
}

uint8_t helf_elfdata_get(helf_e_ident_t e_ident)
{
    if(helf_is_elf(e_ident))
    {
        return e_ident[HELF_EI_DATA];
    }
    return HELF_ELFDATA_NONE;
}

uint8_t helf_elfversion_get(helf_e_ident_t e_ident)
{
    if(helf_is_elf(e_ident))
    {
        return e_ident[HELF_EI_VERSION];
    }
    return HELF_EV_NONE;
}

uint8_t helf_elfosabi_get(helf_e_ident_t e_ident)
{
    if(helf_is_elf(e_ident))
    {
        return e_ident[HELF_EI_OSABI];
    }
    return HELF_ELFOSABI_NONE;
}

uint8_t helf_elfabiversion_get(helf_e_ident_t e_ident)
{
    if(helf_is_elf(e_ident))
    {
        return e_ident[HELF_EI_ABIVERSION];
    }
    return 0;
}

void helf_file_input_init(helf_file_input_t *input,helf_file_input_read_t read,void *usr)
{
    if(input!=NULL)
    {
        input->read=read;
        input->usr=usr;
    }
}

size_t helf_file_input_read(helf_file_input_t *input,uintptr_t address,void *buffer,size_t buffer_length)
{
    if(input==NULL || input->read==NULL)
    {
        return 0;
    }
    return input->read(input,address,buffer,buffer_length);
}

bool helf_file_input_is_big_endian(helf_file_input_t *input_file)
{
    bool ret=false;
    if(input_file==NULL)
    {
        return ret;
    }
    helf_e_ident_t ident= {0};
    if(helf_file_input_read(input_file,0,ident,sizeof(ident)) < sizeof(ident))
    {
        return ret;
    }
    ret=(HELF_ELFDATA_2_MSB==helf_elfdata_get(ident));
    return ret;
}
