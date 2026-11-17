/***************************************************************
 * Name:      helf_arch.c
 * Purpose:   实现helf_arch接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "helf_arch.h"

bool helf_arch_is(helf_file_input_t *input_file,const uint16_t *machines,size_t machine_count,uint8_t elfclass)
{
    if(input_file==NULL || machines==NULL || machine_count==0)
    {
        return false;
    }
    uint16_t machine=0;
    if(helf_file_input_is_32_bits(input_file))
    {
        if(elfclass!=0 && elfclass!=HELF_ELFCLASS_32)
        {
            return false;
        }
        helf_elf32_file_header_t header;
        if(!helf_file_input_32_bits_header_get(input_file,&header))
        {
            return false;
        }
        machine=header.e_machine;
    }
    else if(helf_file_input_is_64_bits(input_file))
    {
        if(elfclass!=0 && elfclass!=HELF_ELFCLASS_64)
        {
            return false;
        }
        helf_elf64_file_header_t header;
        if(!helf_file_input_64_bits_header_get(input_file,&header))
        {
            return false;
        }
        machine=header.e_machine;
    }
    else
    {
        return false;
    }
    for(size_t i=0; i<machine_count; i++)
    {
        if(machines[i]==machine)
        {
            return true;
        }
    }
    return false;
}

bool helf_arch_is_executable(helf_file_input_t *input_file,const uint16_t *machines,size_t machine_count,uint8_t elfclass)
{
    if(!helf_arch_is(input_file,machines,machine_count,elfclass))
    {
        return false;
    }
    if(helf_file_input_is_32_bits(input_file))
    {
        helf_elf32_file_header_t header;
        if(!helf_file_input_32_bits_header_get(input_file,&header))
        {
            return false;
        }
        return header.e_type==HELF_ET_EXEC;
    }
    helf_elf64_file_header_t header;
    if(!helf_file_input_64_bits_header_get(input_file,&header))
    {
        return false;
    }
    return header.e_type==HELF_ET_EXEC;
}

bool helf_arch_type_get(helf_file_input_t *input_file,uint16_t *type)
{
    if(input_file==NULL || type==NULL)
    {
        return false;
    }
    if(helf_file_input_is_32_bits(input_file))
    {
        helf_elf32_file_header_t header;
        if(!helf_file_input_32_bits_header_get(input_file,&header))
        {
            return false;
        }
        *type=header.e_type;
        return true;
    }
    if(helf_file_input_is_64_bits(input_file))
    {
        helf_elf64_file_header_t header;
        if(!helf_file_input_64_bits_header_get(input_file,&header))
        {
            return false;
        }
        *type=header.e_type;
        return true;
    }
    return false;
}

bool helf_arch_is_type(helf_file_input_t *input_file,const uint16_t *machines,size_t machine_count,uint8_t elfclass,uint16_t type)
{
    if(!helf_arch_is(input_file,machines,machine_count,elfclass))
    {
        return false;
    }
    uint16_t file_type=0;
    if(!helf_arch_type_get(input_file,&file_type))
    {
        return false;
    }
    return file_type==type;
}

bool helf_arch_is_shared_object(helf_file_input_t *input_file,const uint16_t *machines,size_t machine_count,uint8_t elfclass)
{
    return helf_arch_is_type(input_file,machines,machine_count,elfclass,HELF_ET_DYN);
}

bool helf_arch_is_relocatable(helf_file_input_t *input_file,const uint16_t *machines,size_t machine_count,uint8_t elfclass)
{
    return helf_arch_is_type(input_file,machines,machine_count,elfclass,HELF_ET_REL);
}

bool helf_arch_entry_get(helf_file_input_t *input_file,uint64_t *entry)
{
    if(input_file==NULL || entry==NULL)
    {
        return false;
    }
    if(helf_file_input_is_32_bits(input_file))
    {
        helf_elf32_file_header_t header;
        if(!helf_file_input_32_bits_header_get(input_file,&header))
        {
            return false;
        }
        *entry=header.e_entry;
        return true;
    }
    if(helf_file_input_is_64_bits(input_file))
    {
        helf_elf64_file_header_t header;
        if(!helf_file_input_64_bits_header_get(input_file,&header))
        {
            return false;
        }
        *entry=header.e_entry;
        return true;
    }
    return false;
}

size_t helf_arch_section_count_get(helf_file_input_t *input_file)
{
    if(input_file==NULL)
    {
        return 0;
    }
    if(helf_file_input_is_32_bits(input_file))
    {
        helf_elf32_file_header_t header;
        if(!helf_file_input_32_bits_header_get(input_file,&header))
        {
            return 0;
        }
        return header.e_shnum;
    }
    if(helf_file_input_is_64_bits(input_file))
    {
        helf_elf64_file_header_t header;
        if(!helf_file_input_64_bits_header_get(input_file,&header))
        {
            return 0;
        }
        return header.e_shnum;
    }
    return 0;
}

bool helf_arch_section_info_get(helf_file_input_t *input_file,size_t index,helf_arch_section_info_t *info)
{
    if(input_file==NULL || info==NULL)
    {
        return false;
    }
    memset(info,0,sizeof(*info));
    if(helf_file_input_is_32_bits(input_file))
    {
        helf_elf32_section_header_t section;
        if(!helf_file_input_32_bits_section_header_get(input_file,index,&section))
        {
            return false;
        }
        if(!helf_file_input_32_bits_section_header_name_get(input_file,&section,info->name,sizeof(info->name)))
        {
            info->name[0]=0;
        }
        info->name[sizeof(info->name)-1]=0;
        info->name_offset=section.sh_name;
        info->type=section.sh_type;
        info->flags=section.sh_flags;
        info->address=section.sh_addr;
        info->offset=section.sh_offset;
        info->size=section.sh_size;
        info->link=section.sh_link;
        info->info=section.sh_info;
        info->addralign=section.sh_addralign;
        info->entsize=section.sh_entsize;
        return true;
    }
    if(helf_file_input_is_64_bits(input_file))
    {
        helf_elf64_section_header_t section;
        if(!helf_file_input_64_bits_section_header_get(input_file,index,&section))
        {
            return false;
        }
        if(!helf_file_input_64_bits_section_header_name_get(input_file,&section,info->name,sizeof(info->name)))
        {
            info->name[0]=0;
        }
        info->name[sizeof(info->name)-1]=0;
        info->name_offset=section.sh_name;
        info->type=section.sh_type;
        info->flags=section.sh_flags;
        info->address=section.sh_addr;
        info->offset=section.sh_offset;
        info->size=section.sh_size;
        info->link=section.sh_link;
        info->info=section.sh_info;
        info->addralign=section.sh_addralign;
        info->entsize=section.sh_entsize;
        return true;
    }
    return false;
}

bool helf_arch_section_is_rx(const helf_arch_section_info_t *info)
{
    if(info==NULL)
    {
        return false;
    }
    if((info->flags&HELF_SHF_ALLOC)==0 || (info->flags&HELF_SHF_EXECINSTR)==0)
    {
        return false;
    }
    return info->type!=HELF_SHT_NOBITS;
}

bool helf_arch_section_is_ro(const helf_arch_section_info_t *info)
{
    if(info==NULL)
    {
        return false;
    }
    if((info->flags&HELF_SHF_ALLOC)==0)
    {
        return false;
    }
    if((info->flags&(HELF_SHF_WRITE|HELF_SHF_EXECINSTR))!=0)
    {
        return false;
    }
    return info->type!=HELF_SHT_NULL;
}

bool helf_arch_section_is_rw(const helf_arch_section_info_t *info)
{
    if(info==NULL)
    {
        return false;
    }
    return ((info->flags&HELF_SHF_ALLOC)!=0) && ((info->flags&HELF_SHF_WRITE)!=0);
}

bool helf_arch_section_is_bss(const helf_arch_section_info_t *info)
{
    if(info==NULL)
    {
        return false;
    }
    return info->type==HELF_SHT_NOBITS;
}

bool helf_arch_is_rx_section(size_t index,helf_file_input_t *input_file)
{
    helf_arch_section_info_t info;
    if(!helf_arch_section_info_get(input_file,index,&info))
    {
        return false;
    }
    return helf_arch_section_is_rx(&info);
}

intptr_t helf_arch_rx_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_arch_is_rx_section(index,input_file))
    {
        return -1;
    }
    helf_arch_section_info_t info;
    if(!helf_arch_section_info_get(input_file,index,&info))
    {
        return -1;
    }
    return (intptr_t)info.offset;
}

size_t helf_arch_rx_section_size_get(helf_file_input_t *input_file)
{
    size_t count=helf_arch_section_count_get(input_file);
    size_t ret=0;
    for(size_t i=0; i<count; i++)
    {
        helf_arch_section_info_t info;
        if(!helf_arch_section_info_get(input_file,i,&info))
        {
            return 0;
        }
        if(helf_arch_section_is_rx(&info))
        {
            ret+=info.size;
        }
    }
    return ret;
}

bool helf_arch_is_ro_section(size_t index,helf_file_input_t *input_file)
{
    helf_arch_section_info_t info;
    if(!helf_arch_section_info_get(input_file,index,&info))
    {
        return false;
    }
    return helf_arch_section_is_ro(&info);
}

intptr_t helf_arch_ro_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_arch_is_ro_section(index,input_file))
    {
        return -1;
    }
    helf_arch_section_info_t info;
    if(!helf_arch_section_info_get(input_file,index,&info))
    {
        return -1;
    }
    return (intptr_t)info.offset;
}

size_t helf_arch_ro_section_size_get(helf_file_input_t *input_file)
{
    size_t count=helf_arch_section_count_get(input_file);
    size_t ret=0;
    for(size_t i=0; i<count; i++)
    {
        helf_arch_section_info_t info;
        if(!helf_arch_section_info_get(input_file,i,&info))
        {
            return 0;
        }
        if(helf_arch_section_is_ro(&info))
        {
            ret+=info.size;
        }
    }
    return ret;
}

bool helf_arch_is_rw_section(size_t index,helf_file_input_t *input_file)
{
    helf_arch_section_info_t info;
    if(!helf_arch_section_info_get(input_file,index,&info))
    {
        return false;
    }
    return helf_arch_section_is_rw(&info);
}

intptr_t helf_arch_rw_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_arch_is_rw_section(index,input_file))
    {
        return -1;
    }
    helf_arch_section_info_t info;
    if(!helf_arch_section_info_get(input_file,index,&info))
    {
        return -1;
    }
    return (intptr_t)info.offset;
}

size_t helf_arch_rw_section_size_get(helf_file_input_t *input_file)
{
    size_t count=helf_arch_section_count_get(input_file);
    size_t ret=0;
    for(size_t i=0; i<count; i++)
    {
        helf_arch_section_info_t info;
        if(!helf_arch_section_info_get(input_file,i,&info))
        {
            return 0;
        }
        if(helf_arch_section_is_rw(&info))
        {
            ret+=info.size;
        }
    }
    return ret;
}

bool helf_arch_is_bss_section(size_t index,helf_file_input_t *input_file)
{
    helf_arch_section_info_t info;
    if(!helf_arch_section_info_get(input_file,index,&info))
    {
        return false;
    }
    return helf_arch_section_is_bss(&info);
}

size_t helf_arch_program_header_count_get(helf_file_input_t *input_file)
{
    if(input_file==NULL)
    {
        return 0;
    }
    if(helf_file_input_is_32_bits(input_file))
    {
        helf_elf32_file_header_t header;
        if(!helf_file_input_32_bits_header_get(input_file,&header))
        {
            return 0;
        }
        return header.e_phnum;
    }
    if(helf_file_input_is_64_bits(input_file))
    {
        helf_elf64_file_header_t header;
        if(!helf_file_input_64_bits_header_get(input_file,&header))
        {
            return 0;
        }
        return header.e_phnum;
    }
    return 0;
}

size_t helf_arch_segment_count_get(helf_file_input_t *input_file)
{
    return helf_arch_program_header_count_get(input_file);
}

bool helf_arch_segment_info_get(helf_file_input_t *input_file,size_t index,helf_arch_segment_info_t *info)
{
    if(input_file==NULL || info==NULL)
    {
        return false;
    }
    memset(info,0,sizeof(*info));
    if(helf_file_input_is_32_bits(input_file))
    {
        helf_elf32_program_header_t programheader;
        if(!helf_file_input_32_bits_program_header_get(input_file,index,&programheader))
        {
            return false;
        }
        info->type=programheader.p_type;
        info->flags=programheader.p_flags;
        info->offset=programheader.p_offset;
        info->vaddr=programheader.p_vaddr;
        info->paddr=programheader.p_paddr;
        info->filesz=programheader.p_filesz;
        info->memsz=programheader.p_memsz;
        info->addralign=programheader.p_align;
        return true;
    }
    if(helf_file_input_is_64_bits(input_file))
    {
        helf_elf64_program_header_t programheader;
        if(!helf_file_input_64_bits_program_header_get(input_file,index,&programheader))
        {
            return false;
        }
        info->type=programheader.p_type;
        info->flags=programheader.p_flags;
        info->offset=programheader.p_offset;
        info->vaddr=programheader.p_vaddr;
        info->paddr=programheader.p_paddr;
        info->filesz=programheader.p_filesz;
        info->memsz=programheader.p_memsz;
        info->addralign=programheader.p_align;
        return true;
    }
    return false;
}

bool helf_arch_is_load_segment(size_t index,helf_file_input_t *input_file)
{
    helf_arch_segment_info_t info;
    if(!helf_arch_segment_info_get(input_file,index,&info))
    {
        return false;
    }
    return info.type==HELF_PT_LOAD;
}

bool helf_arch_is_rx_segment(size_t index,helf_file_input_t *input_file)
{
    helf_arch_segment_info_t info;
    if(!helf_arch_segment_info_get(input_file,index,&info))
    {
        return false;
    }
    return info.type==HELF_PT_LOAD && ((info.flags&HELF_PF_X)!=0);
}

bool helf_arch_is_ro_segment(size_t index,helf_file_input_t *input_file)
{
    helf_arch_segment_info_t info;
    if(!helf_arch_segment_info_get(input_file,index,&info))
    {
        return false;
    }
    if(info.type!=HELF_PT_LOAD)
    {
        return false;
    }
    if((info.flags&(HELF_PF_W|HELF_PF_X))!=0)
    {
        return false;
    }
    return (info.flags&HELF_PF_R)!=0;
}

bool helf_arch_is_rw_segment(size_t index,helf_file_input_t *input_file)
{
    helf_arch_segment_info_t info;
    if(!helf_arch_segment_info_get(input_file,index,&info))
    {
        return false;
    }
    return info.type==HELF_PT_LOAD && ((info.flags&HELF_PF_W)!=0);
}

size_t helf_arch_load_segment_count_get(helf_file_input_t *input_file)
{
    size_t count=helf_arch_segment_count_get(input_file);
    size_t ret=0;
    for(size_t i=0; i<count; i++)
    {
        if(helf_arch_is_load_segment(i,input_file))
        {
            ret++;
        }
    }
    return ret;
}

bool helf_arch_load_segment_info_get(helf_file_input_t *input_file,size_t index,helf_arch_segment_info_t *info)
{
    if(info==NULL)
    {
        return false;
    }
    size_t count=helf_arch_segment_count_get(input_file);
    size_t current=0;
    for(size_t i=0; i<count; i++)
    {
        if(!helf_arch_is_load_segment(i,input_file))
        {
            continue;
        }
        if(current==index)
        {
            return helf_arch_segment_info_get(input_file,i,info);
        }
        current++;
    }
    return false;
}

size_t helf_arch_load_segment_total_filesz_get(helf_file_input_t *input_file)
{
    size_t count=helf_arch_segment_count_get(input_file);
    size_t ret=0;
    for(size_t i=0; i<count; i++)
    {
        if(!helf_arch_is_load_segment(i,input_file))
        {
            continue;
        }
        helf_arch_segment_info_t info;
        if(!helf_arch_segment_info_get(input_file,i,&info))
        {
            return 0;
        }
        ret+=info.filesz;
    }
    return ret;
}

size_t helf_arch_load_segment_total_memsz_get(helf_file_input_t *input_file)
{
    size_t count=helf_arch_segment_count_get(input_file);
    size_t ret=0;
    for(size_t i=0; i<count; i++)
    {
        if(!helf_arch_is_load_segment(i,input_file))
        {
            continue;
        }
        helf_arch_segment_info_t info;
        if(!helf_arch_segment_info_get(input_file,i,&info))
        {
            return 0;
        }
        ret+=info.memsz;
    }
    return ret;
}

static uint16_t helf_arch_read_uint16(const uint8_t *data,bool big_endian)
{
    if(big_endian)
    {
        return ((uint16_t)data[0] << 8) | ((uint16_t)data[1] << 0);
    }
    return ((uint16_t)data[1] << 8) | ((uint16_t)data[0] << 0);
}

static uint32_t helf_arch_read_uint32(const uint8_t *data,bool big_endian)
{
    if(big_endian)
    {
        return ((uint32_t)data[0] << 24) | ((uint32_t)data[1] << 16) | ((uint32_t)data[2] << 8) | ((uint32_t)data[3] << 0);
    }
    return ((uint32_t)data[3] << 24) | ((uint32_t)data[2] << 16) | ((uint32_t)data[1] << 8) | ((uint32_t)data[0] << 0);
}

static uint64_t helf_arch_read_uint64(const uint8_t *data,bool big_endian)
{
    if(big_endian)
    {
        return ((uint64_t)data[0] << 56) | ((uint64_t)data[1] << 48) | ((uint64_t)data[2] << 40) | ((uint64_t)data[3] << 32)
               | ((uint64_t)data[4] << 24) | ((uint64_t)data[5] << 16) | ((uint64_t)data[6] << 8) | ((uint64_t)data[7] << 0);
    }
    return ((uint64_t)data[7] << 56) | ((uint64_t)data[6] << 48) | ((uint64_t)data[5] << 40) | ((uint64_t)data[4] << 32)
           | ((uint64_t)data[3] << 24) | ((uint64_t)data[2] << 16) | ((uint64_t)data[1] << 8) | ((uint64_t)data[0] << 0);
}

static bool helf_arch_section_header_get_raw(helf_file_input_t *input_file,size_t index,helf_arch_section_info_t *info)
{
    if(input_file==NULL || info==NULL)
    {
        return false;
    }
    memset(info,0,sizeof(*info));
    if(helf_file_input_is_32_bits(input_file))
    {
        helf_elf32_section_header_t section;
        if(!helf_file_input_32_bits_section_header_get(input_file,index,&section))
        {
            return false;
        }
        info->type=section.sh_type;
        info->flags=section.sh_flags;
        info->address=section.sh_addr;
        info->offset=section.sh_offset;
        info->size=section.sh_size;
        info->link=section.sh_link;
        info->info=section.sh_info;
        info->addralign=section.sh_addralign;
        info->entsize=section.sh_entsize;
        return true;
    }
    if(helf_file_input_is_64_bits(input_file))
    {
        helf_elf64_section_header_t section;
        if(!helf_file_input_64_bits_section_header_get(input_file,index,&section))
        {
            return false;
        }
        info->type=section.sh_type;
        info->flags=section.sh_flags;
        info->address=section.sh_addr;
        info->offset=section.sh_offset;
        info->size=section.sh_size;
        info->link=section.sh_link;
        info->info=section.sh_info;
        info->addralign=section.sh_addralign;
        info->entsize=section.sh_entsize;
        return true;
    }
    return false;
}

const char *helf_arch_relocation_type_name_get(const helf_arch_relocation_type_name_t *table,size_t count,uint32_t type)
{
    if(table==NULL)
    {
        return NULL;
    }
    for(size_t i=0; i<count; i++)
    {
        if(table[i].type==type)
        {
            return table[i].name;
        }
    }
    return NULL;
}

const char *helf_arch_section_type_name_get(const helf_arch_section_type_name_t *table,size_t count,uint32_t type)
{
    if(table==NULL)
    {
        return NULL;
    }
    for(size_t i=0; i<count; i++)
    {
        if(table[i].type==type)
        {
            return table[i].name;
        }
    }
    return NULL;
}

uint8_t helf_arch_relocation_type_size_get(const helf_arch_relocation_type_name_t *table,size_t count,uint32_t type)
{
    if(table==NULL)
    {
        return 0;
    }
    for(size_t i=0; i<count; i++)
    {
        if(table[i].type==type)
        {
            return table[i].size;
        }
    }
    return 0;
}

size_t helf_arch_relocation_section_count_get(helf_file_input_t *input_file)
{
    size_t section_count=helf_arch_section_count_get(input_file);
    size_t ret=0;
    for(size_t i=0; i<section_count; i++)
    {
        helf_arch_section_info_t section;
        if(!helf_arch_section_header_get_raw(input_file,i,&section))
        {
            continue;
        }
        if(section.type==HELF_SHT_REL || section.type==HELF_SHT_RELA)
        {
            ret++;
        }
    }
    return ret;
}

bool helf_arch_relocation_section_info_get_by_section(helf_file_input_t *input_file,size_t section_index,helf_arch_relocation_section_info_t *info)
{
    if(input_file==NULL || info==NULL)
    {
        return false;
    }
    helf_arch_section_info_t section;
    if(!helf_arch_section_header_get_raw(input_file,section_index,&section))
    {
        return false;
    }
    if(section.type!=HELF_SHT_REL && section.type!=HELF_SHT_RELA)
    {
        return false;
    }
    bool is_64_bits=helf_file_input_is_64_bits(input_file);
    size_t min_entsize=0;
    if(section.type==HELF_SHT_RELA)
    {
        min_entsize=is_64_bits?24:12;
    }
    else
    {
        min_entsize=is_64_bits?16:8;
    }
    uint64_t entsize=section.entsize;
    if(entsize==0)
    {
        entsize=min_entsize;
    }
    if(entsize<min_entsize)
    {
        return false;
    }
    memset(info,0,sizeof(*info));
    info->index=section_index;
    info->type=section.type;
    info->offset=section.offset;
    info->size=section.size;
    info->link=section.link;
    info->info=section.info;
    info->addralign=section.addralign;
    info->entsize=entsize;
    info->count=(size_t)(section.size/entsize);
    return true;
}

bool helf_arch_relocation_section_info_get(helf_file_input_t *input_file,size_t index,helf_arch_relocation_section_info_t *info)
{
    if(input_file==NULL || info==NULL)
    {
        return false;
    }
    size_t section_count=helf_arch_section_count_get(input_file);
    size_t current=0;
    for(size_t i=0; i<section_count; i++)
    {
        helf_arch_section_info_t section;
        if(!helf_arch_section_header_get_raw(input_file,i,&section))
        {
            continue;
        }
        if(section.type!=HELF_SHT_REL && section.type!=HELF_SHT_RELA)
        {
            continue;
        }
        if(current==index)
        {
            return helf_arch_relocation_section_info_get_by_section(input_file,i,info);
        }
        current++;
    }
    return false;
}

size_t helf_arch_relocation_count_get(helf_file_input_t *input_file,size_t section_index)
{
    helf_arch_relocation_section_info_t info;
    if(!helf_arch_relocation_section_info_get_by_section(input_file,section_index,&info))
    {
        return 0;
    }
    return info.count;
}

bool helf_arch_relocation_get(helf_file_input_t *input_file,size_t section_index,size_t index,helf_arch_relocation_info_t *info)
{
    if(input_file==NULL || info==NULL)
    {
        return false;
    }
    helf_arch_relocation_section_info_t section;
    if(!helf_arch_relocation_section_info_get_by_section(input_file,section_index,&section))
    {
        return false;
    }
    if(index>=section.count)
    {
        return false;
    }
    bool is_64_bits=helf_file_input_is_64_bits(input_file);
    bool big_endian=helf_file_input_is_big_endian(input_file);
    bool has_addend=(section.type==HELF_SHT_RELA);
    size_t need_bytes=0;
    if(is_64_bits)
    {
        need_bytes=has_addend?24:16;
    }
    else
    {
        need_bytes=has_addend?12:8;
    }
    uint8_t buffer[24]= {0};
    uint64_t address=section.offset+(uint64_t)index*section.entsize;
    if(helf_file_input_read(input_file,(uintptr_t)address,buffer,need_bytes) < need_bytes)
    {
        return false;
    }
    memset(info,0,sizeof(*info));
    info->section_index=(uint32_t)section_index;
    info->symbol_table_index=section.link;
    info->target_section_index=section.info;
    info->index=index;
    info->has_addend=has_addend;
    if(is_64_bits)
    {
        info->offset=helf_arch_read_uint64(&buffer[0],big_endian);
        info->info=helf_arch_read_uint64(&buffer[8],big_endian);
        if(has_addend)
        {
            info->addend=(int64_t)helf_arch_read_uint64(&buffer[16],big_endian);
        }
        info->symbol=(uint32_t)(info->info >> 32);
        info->type=(uint32_t)(info->info & 0xFFFFFFFFULL);
    }
    else
    {
        info->offset=helf_arch_read_uint32(&buffer[0],big_endian);
        info->info=helf_arch_read_uint32(&buffer[4],big_endian);
        if(has_addend)
        {
            info->addend=(int32_t)helf_arch_read_uint32(&buffer[8],big_endian);
        }
        info->symbol=(uint32_t)(info->info >> 8);
        info->type=(uint32_t)(info->info & 0xFFULL);
    }
    return true;
}

size_t helf_arch_symbol_section_count_get(helf_file_input_t *input_file,size_t symbol_table_index)
{
    helf_arch_section_info_t section;
    if(!helf_arch_section_header_get_raw(input_file,symbol_table_index,&section))
    {
        return 0;
    }
    if(section.type!=HELF_SHT_SYMTAB && section.type!=HELF_SHT_DYNSYM)
    {
        return 0;
    }
    bool is_64_bits=helf_file_input_is_64_bits(input_file);
    uint64_t min_entsize=is_64_bits?24:16;
    uint64_t entsize=section.entsize;
    if(entsize==0)
    {
        entsize=min_entsize;
    }
    if(entsize<min_entsize)
    {
        return 0;
    }
    return (size_t)(section.size/entsize);
}

bool helf_arch_symbol_section_info_get(helf_file_input_t *input_file,size_t symbol_table_index,size_t index,helf_arch_symbol_info_t *info)
{
    if(input_file==NULL || info==NULL)
    {
        return false;
    }
    helf_arch_section_info_t symbol_section;
    if(!helf_arch_section_header_get_raw(input_file,symbol_table_index,&symbol_section))
    {
        return false;
    }
    if(symbol_section.type!=HELF_SHT_SYMTAB && symbol_section.type!=HELF_SHT_DYNSYM)
    {
        return false;
    }
    bool is_64_bits=helf_file_input_is_64_bits(input_file);
    bool big_endian=helf_file_input_is_big_endian(input_file);
    uint64_t min_entsize=is_64_bits?24:16;
    uint64_t entsize=symbol_section.entsize;
    if(entsize==0)
    {
        entsize=min_entsize;
    }
    if(entsize<min_entsize)
    {
        return false;
    }
    size_t symbol_count=(size_t)(symbol_section.size/entsize);
    if(index>=symbol_count)
    {
        return false;
    }
    uint8_t buffer[24]= {0};
    uint64_t address=symbol_section.offset+(uint64_t)index*entsize;
    size_t need_bytes=is_64_bits?24:16;
    if(helf_file_input_read(input_file,(uintptr_t)address,buffer,need_bytes) < need_bytes)
    {
        return false;
    }
    memset(info,0,sizeof(*info));
    uint32_t st_name=0;
    if(is_64_bits)
    {
        st_name=helf_arch_read_uint32(&buffer[0],big_endian);
        info->info=buffer[4];
        info->other=buffer[5];
        info->shndx=helf_arch_read_uint16(&buffer[6],big_endian);
        info->value=helf_arch_read_uint64(&buffer[8],big_endian);
        info->size=helf_arch_read_uint64(&buffer[16],big_endian);
    }
    else
    {
        st_name=helf_arch_read_uint32(&buffer[0],big_endian);
        info->value=helf_arch_read_uint32(&buffer[4],big_endian);
        info->size=helf_arch_read_uint32(&buffer[8],big_endian);
        info->info=buffer[12];
        info->other=buffer[13];
        info->shndx=helf_arch_read_uint16(&buffer[14],big_endian);
    }
    helf_arch_section_info_t strtab_section;
    if(st_name!=0 && helf_arch_section_header_get_raw(input_file,symbol_section.link,&strtab_section) && strtab_section.type==HELF_SHT_STRTAB)
    {
        if(st_name < strtab_section.size)
        {
            uint64_t name_address=strtab_section.offset+st_name;
            uint64_t name_remain=strtab_section.size-st_name;
            size_t max_length=sizeof(info->name)-1;
            if(name_remain < max_length)
            {
                max_length=(size_t)name_remain;
            }
            size_t i=0;
            for(; i<max_length; i++)
            {
                uint8_t ch=0;
                if(helf_file_input_read(input_file,(uintptr_t)(name_address+i),&ch,sizeof(ch)) != sizeof(ch))
                {
                    break;
                }
                info->name[i]=(char)ch;
                if(ch==0)
                {
                    break;
                }
            }
            info->name[i]=0;
        }
    }
    info->name[sizeof(info->name)-1]=0;
    if(info->name[0]==0 && st_name==0 && HELF_ELF32_ST_TYPE(info->info)==HELF_STT_SECTION
       && info->shndx!=0 && info->shndx<0xFF00)
    {
        helf_arch_section_info_t named_section;
        if(helf_arch_section_info_get(input_file,info->shndx,&named_section))
        {
            size_t i=0;
            for(; i<sizeof(info->name)-1 && named_section.name[i]!=0; i++)
            {
                info->name[i]=named_section.name[i];
            }
            info->name[i]=0;
        }
    }
    return true;
}

static bool helf_arch_symbol_table_index_get(helf_file_input_t *input_file,size_t *symbol_table_index)
{
    if(input_file==NULL || symbol_table_index==NULL)
    {
        return false;
    }
    size_t section_count=helf_arch_section_count_get(input_file);
    for(size_t i=0; i<section_count; i++)
    {
        helf_arch_section_info_t section;
        if(!helf_arch_section_header_get_raw(input_file,i,&section))
        {
            continue;
        }
        if(section.type==HELF_SHT_SYMTAB)
        {
            *symbol_table_index=i;
            return true;
        }
    }
    return false;
}

size_t helf_arch_symbol_count_get(helf_file_input_t *input_file)
{
    size_t symbol_table_index=0;
    if(!helf_arch_symbol_table_index_get(input_file,&symbol_table_index))
    {
        return 0;
    }
    return helf_arch_symbol_section_count_get(input_file,symbol_table_index);
}

bool helf_arch_symbol_info_get(helf_file_input_t *input_file,size_t index,helf_arch_symbol_info_t *info)
{
    size_t symbol_table_index=0;
    if(!helf_arch_symbol_table_index_get(input_file,&symbol_table_index))
    {
        return false;
    }
    return helf_arch_symbol_section_info_get(input_file,symbol_table_index,index,info);
}

bool helf_arch_relocation_symbol_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_symbol_info_t *info)
{
    if(input_file==NULL || relocation==NULL || info==NULL)
    {
        return false;
    }
    memset(info,0,sizeof(*info));
    if(relocation->symbol==0)
    {
        return true;
    }
    return helf_arch_symbol_section_info_get(input_file,relocation->symbol_table_index,relocation->symbol,info);
}

static bool helf_arch_address_to_file_offset(helf_file_input_t *input_file,uint64_t address,size_t size,uint64_t *file_offset,bool *in_file,bool *is_bss,bool *in_segment,bool *in_section)
{
    if(input_file==NULL || file_offset==NULL || in_file==NULL || is_bss==NULL)
    {
        return false;
    }
    *file_offset=0;
    *in_file=false;
    *is_bss=false;
    if(in_segment!=NULL)
    {
        *in_segment=false;
    }
    if(in_section!=NULL)
    {
        *in_section=false;
    }
    size_t program_header_count=helf_arch_program_header_count_get(input_file);
    for(size_t i=0; i<program_header_count; i++)
    {
        helf_arch_segment_info_t segment;
        if(!helf_arch_segment_info_get(input_file,i,&segment))
        {
            continue;
        }
        if(segment.type!=HELF_PT_LOAD)
        {
            continue;
        }
        if(address < segment.vaddr)
        {
            continue;
        }
        uint64_t delta=address-segment.vaddr;
        if(delta>=segment.memsz)
        {
            continue;
        }
        if(delta<segment.filesz)
        {
            if((uint64_t)size<=segment.filesz-delta)
            {
                *file_offset=segment.offset+delta;
                *in_file=true;
                if(in_segment!=NULL)
                {
                    *in_segment=true;
                }
                return true;
            }
            continue;
        }
        if((uint64_t)size<=segment.memsz-delta)
        {
            *in_file=false;
            *is_bss=true;
            if(in_segment!=NULL)
            {
                *in_segment=true;
            }
            return true;
        }
    }
    size_t section_count=helf_arch_section_count_get(input_file);
    for(size_t i=0; i<section_count; i++)
    {
        helf_arch_section_info_t section;
        if(!helf_arch_section_header_get_raw(input_file,i,&section))
        {
            continue;
        }
        if((section.flags&HELF_SHF_ALLOC)==0)
        {
            continue;
        }
        if(address < section.address)
        {
            continue;
        }
        uint64_t delta=address-section.address;
        if(delta>=section.size)
        {
            continue;
        }
        if((uint64_t)size>section.size-delta)
        {
            continue;
        }
        if(in_section!=NULL)
        {
            *in_section=true;
        }
        if(section.type==HELF_SHT_NOBITS)
        {
            *in_file=false;
            *is_bss=true;
            return true;
        }
        *file_offset=section.offset+delta;
        *in_file=true;
        return true;
    }
    return false;
}

bool helf_arch_relocation_address_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,size_t size,helf_arch_relocation_address_info_t *info)
{
    if(input_file==NULL || relocation==NULL || info==NULL || size==0)
    {
        return false;
    }
    uint16_t type=0;
    if(!helf_arch_type_get(input_file,&type))
    {
        return false;
    }
    memset(info,0,sizeof(*info));
    info->address=relocation->offset;
    info->size=size;
    if(type==HELF_ET_REL)
    {
        helf_arch_section_info_t section;
        if(!helf_arch_section_header_get_raw(input_file,relocation->target_section_index,&section))
        {
            return false;
        }
        if(relocation->offset>section.size)
        {
            return false;
        }
        uint64_t delta=relocation->offset;
        if((uint64_t)size>section.size-delta)
        {
            return false;
        }
        info->in_section=true;
        if(section.type==HELF_SHT_NOBITS)
        {
            info->in_file=false;
            info->is_bss=true;
        }
        else
        {
            info->file_offset=section.offset+delta;
            info->in_file=true;
        }
        return true;
    }
    uint64_t file_offset=0;
    bool in_file=false;
    bool is_bss=false;
    bool in_segment=false;
    bool in_section=false;
    if(!helf_arch_address_to_file_offset(input_file,relocation->offset,size,&file_offset,&in_file,&is_bss,&in_segment,&in_section))
    {
        return false;
    }
    info->file_offset=file_offset;
    info->in_file=in_file;
    info->is_bss=is_bss;
    info->in_segment=in_segment;
    info->in_section=in_section;
    return true;
}

size_t helf_arch_relocation_data_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,size_t size,void *buffer,size_t buffer_length)
{
    if(input_file==NULL || relocation==NULL || size==0 || buffer==NULL || buffer_length<size)
    {
        return 0;
    }
    helf_arch_relocation_address_info_t info;
    if(!helf_arch_relocation_address_info_get(input_file,relocation,size,&info))
    {
        return 0;
    }
    if(info.is_bss || !info.in_file)
    {
        memset(buffer,0,size);
        return size;
    }
    if(helf_file_input_read(input_file,(uintptr_t)info.file_offset,buffer,size)<size)
    {
        return 0;
    }
    return size;
}

typedef struct helf_arch_dynamic_location
{
    uint64_t    offset;
    uint64_t    size;
    uint64_t    entsize;
    size_t      count;
} helf_arch_dynamic_location_t;

static bool helf_arch_dynamic_location_get(helf_file_input_t *input_file,helf_arch_dynamic_location_t *location)
{
    if(input_file==NULL || location==NULL)
    {
        return false;
    }
    memset(location,0,sizeof(*location));
    bool is_64_bits=helf_file_input_is_64_bits(input_file);
    uint64_t default_entsize=is_64_bits?16:8;
    size_t section_count=helf_arch_section_count_get(input_file);
    for(size_t i=0; i<section_count; i++)
    {
        helf_arch_section_info_t section;
        if(!helf_arch_section_header_get_raw(input_file,i,&section))
        {
            continue;
        }
        if(section.type!=HELF_SHT_DYNAMIC)
        {
            continue;
        }
        uint64_t entsize=section.entsize;
        if(entsize==0)
        {
            entsize=default_entsize;
        }
        if(entsize<default_entsize)
        {
            continue;
        }
        location->offset=section.offset;
        location->size=section.size;
        location->entsize=entsize;
        location->count=(size_t)(section.size/entsize);
        return true;
    }
    size_t program_header_count=helf_arch_program_header_count_get(input_file);
    for(size_t i=0; i<program_header_count; i++)
    {
        helf_arch_segment_info_t segment;
        if(!helf_arch_segment_info_get(input_file,i,&segment))
        {
            continue;
        }
        if(segment.type!=HELF_PT_DYNAMIC)
        {
            continue;
        }
        location->offset=segment.offset;
        location->size=segment.filesz;
        location->entsize=default_entsize;
        location->count=(size_t)(segment.filesz/default_entsize);
        return true;
    }
    return false;
}

size_t helf_arch_dynamic_count_get(helf_file_input_t *input_file)
{
    helf_arch_dynamic_location_t location;
    if(!helf_arch_dynamic_location_get(input_file,&location))
    {
        return 0;
    }
    return location.count;
}

bool helf_arch_dynamic_info_get(helf_file_input_t *input_file,size_t index,helf_arch_dynamic_info_t *info)
{
    if(input_file==NULL || info==NULL)
    {
        return false;
    }
    helf_arch_dynamic_location_t location;
    if(!helf_arch_dynamic_location_get(input_file,&location))
    {
        return false;
    }
    if(index>=location.count)
    {
        return false;
    }
    bool is_64_bits=helf_file_input_is_64_bits(input_file);
    bool big_endian=helf_file_input_is_big_endian(input_file);
    size_t need_bytes=is_64_bits?16:8;
    uint8_t buffer[16]= {0};
    uint64_t address=location.offset+(uint64_t)index*location.entsize;
    if(helf_file_input_read(input_file,(uintptr_t)address,buffer,need_bytes)<need_bytes)
    {
        return false;
    }
    memset(info,0,sizeof(*info));
    if(is_64_bits)
    {
        info->tag=(int64_t)helf_arch_read_uint64(&buffer[0],big_endian);
        info->value=helf_arch_read_uint64(&buffer[8],big_endian);
    }
    else
    {
        info->tag=(int32_t)helf_arch_read_uint32(&buffer[0],big_endian);
        info->value=helf_arch_read_uint32(&buffer[4],big_endian);
    }
    return true;
}

bool helf_arch_dynamic_string_get(helf_file_input_t *input_file,uint64_t offset,char *buffer,size_t buffer_length)
{
    if(input_file==NULL || buffer==NULL || buffer_length==0)
    {
        return false;
    }
    uint64_t strtab_address=0;
    uint64_t strtab_size=0;
    bool has_strtab=false;
    bool has_strsz=false;
    size_t count=helf_arch_dynamic_count_get(input_file);
    for(size_t i=0; i<count; i++)
    {
        helf_arch_dynamic_info_t dynamic;
        if(!helf_arch_dynamic_info_get(input_file,i,&dynamic))
        {
            break;
        }
        if(dynamic.tag==HELF_DT_STRTAB)
        {
            strtab_address=dynamic.value;
            has_strtab=true;
        }
        else if(dynamic.tag==HELF_DT_STRSZ)
        {
            strtab_size=dynamic.value;
            has_strsz=true;
        }
        else if(dynamic.tag==HELF_DT_NULL)
        {
            break;
        }
    }
    if(!has_strtab || !has_strsz || offset>=strtab_size)
    {
        return false;
    }
    uint64_t file_offset=0;
    bool in_file=false;
    bool is_bss=false;
    if(!helf_arch_address_to_file_offset(input_file,strtab_address,1,&file_offset,&in_file,&is_bss,NULL,NULL))
    {
        return false;
    }
    if(!in_file || is_bss)
    {
        return false;
    }
    uint64_t name_address=file_offset+offset;
    uint64_t name_remain=strtab_size-offset;
    size_t max_length=buffer_length-1;
    if(name_remain<max_length)
    {
        max_length=(size_t)name_remain;
    }
    size_t i=0;
    for(; i<max_length; i++)
    {
        uint8_t ch=0;
        if(helf_file_input_read(input_file,(uintptr_t)(name_address+i),&ch,sizeof(ch))!=sizeof(ch))
        {
            break;
        }
        buffer[i]=(char)ch;
        if(ch==0)
        {
            break;
        }
    }
    buffer[i]=0;
    return true;
}
