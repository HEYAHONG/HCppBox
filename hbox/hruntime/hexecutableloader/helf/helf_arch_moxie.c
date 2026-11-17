/***************************************************************
 * Name:      helf_arch_moxie.c
 * Purpose:   实现helf_arch_moxie接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-16
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "helf_arch_moxie.h"

static const uint16_t helf_moxie_machines[]=
{
    HELF_EM_MOXIE,
    HELF_EM_MOXIE_OLD,
};

static bool helf_moxie_header_get(helf_file_input_t *input_file,helf_elf32_file_header_t *header)
{
    if(input_file==NULL || header==NULL)
    {
        return false;
    }
    if(!helf_file_input_32_bits_header_get(input_file,header))
    {
        return false;
    }
    if(header->e_machine!=HELF_EM_MOXIE &&
       header->e_machine!=HELF_EM_MOXIE_OLD)
    {
        return false;
    }
    return true;
}

bool helf_moxie_is_moxie(helf_file_input_t *input_file)
{
    helf_elf32_file_header_t header;
    return helf_moxie_header_get(input_file,&header);
}

bool helf_moxie_is_big_endian(helf_file_input_t *input_file)
{
    if(!helf_moxie_is_moxie(input_file))
    {
        return false;
    }
    return helf_file_input_is_big_endian(input_file);
}

bool helf_moxie_is_executable(helf_file_input_t *input_file)
{
    helf_elf32_file_header_t header;
    if(!helf_moxie_header_get(input_file,&header))
    {
        return false;
    }
    return header.e_type==HELF_ET_EXEC;
}

bool helf_moxie_entry_get(helf_file_input_t *input_file,uint32_t *entry)
{
    if(entry==NULL)
    {
        return false;
    }
    helf_elf32_file_header_t header;
    if(!helf_moxie_header_get(input_file,&header))
    {
        return false;
    }
    *entry=header.e_entry;
    return true;
}

size_t helf_moxie_section_count_get(helf_file_input_t *input_file)
{
    helf_elf32_file_header_t header;
    if(!helf_moxie_header_get(input_file,&header))
    {
        return 0;
    }
    return header.e_shnum;
}

bool helf_moxie_section_info_get(helf_file_input_t *input_file,size_t index,helf_moxie_section_info_t *info)
{
    if(info==NULL)
    {
        return false;
    }
    helf_elf32_file_header_t header;
    if(!helf_moxie_header_get(input_file,&header))
    {
        return false;
    }
    if(index>=header.e_shnum)
    {
        return false;
    }
    helf_elf32_section_header_t section;
    if(!helf_file_input_32_bits_section_header_get(input_file,index,&section))
    {
        return false;
    }
    memset(info,0,sizeof(*info));
    if(!helf_file_input_32_bits_section_header_name_get(input_file,&section,info->name,sizeof(info->name)))
    {
        info->name[0]=0;
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

static bool helf_moxie_section_info_is_rx(const helf_moxie_section_info_t *info)
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

static bool helf_moxie_section_info_is_ro(const helf_moxie_section_info_t *info)
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

static bool helf_moxie_section_info_is_rw(const helf_moxie_section_info_t *info)
{
    if(info==NULL)
    {
        return false;
    }
    return ((info->flags&HELF_SHF_ALLOC)!=0) && ((info->flags&HELF_SHF_WRITE)!=0);
}

bool helf_moxie_is_rx_section(size_t index,helf_file_input_t *input_file)
{
    helf_moxie_section_info_t info;
    if(!helf_moxie_section_info_get(input_file,index,&info))
    {
        return false;
    }
    return helf_moxie_section_info_is_rx(&info);
}

intptr_t helf_moxie_rx_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_moxie_is_rx_section(index,input_file))
    {
        return -1;
    }
    helf_moxie_section_info_t info;
    if(!helf_moxie_section_info_get(input_file,index,&info))
    {
        return -1;
    }
    return (intptr_t)info.offset;
}

size_t helf_moxie_rx_section_size_get(helf_file_input_t *input_file)
{
    size_t count=helf_moxie_section_count_get(input_file);
    size_t ret=0;
    for(size_t i=0;i<count;i++)
    {
        helf_moxie_section_info_t info;
        if(!helf_moxie_section_info_get(input_file,i,&info))
        {
            return 0;
        }
        if(helf_moxie_section_info_is_rx(&info))
        {
            ret+=info.size;
        }
    }
    return ret;
}

bool helf_moxie_is_ro_section(size_t index,helf_file_input_t *input_file)
{
    helf_moxie_section_info_t info;
    if(!helf_moxie_section_info_get(input_file,index,&info))
    {
        return false;
    }
    return helf_moxie_section_info_is_ro(&info);
}

intptr_t helf_moxie_ro_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_moxie_is_ro_section(index,input_file))
    {
        return -1;
    }
    helf_moxie_section_info_t info;
    if(!helf_moxie_section_info_get(input_file,index,&info))
    {
        return -1;
    }
    return (intptr_t)info.offset;
}

size_t helf_moxie_ro_section_size_get(helf_file_input_t *input_file)
{
    size_t count=helf_moxie_section_count_get(input_file);
    size_t ret=0;
    for(size_t i=0;i<count;i++)
    {
        helf_moxie_section_info_t info;
        if(!helf_moxie_section_info_get(input_file,i,&info))
        {
            return 0;
        }
        if(helf_moxie_section_info_is_ro(&info))
        {
            ret+=info.size;
        }
    }
    return ret;
}

bool helf_moxie_is_rw_section(size_t index,helf_file_input_t *input_file)
{
    helf_moxie_section_info_t info;
    if(!helf_moxie_section_info_get(input_file,index,&info))
    {
        return false;
    }
    return helf_moxie_section_info_is_rw(&info);
}

intptr_t helf_moxie_rw_section_offset_get(size_t index,helf_file_input_t *input_file)
{
    if(!helf_moxie_is_rw_section(index,input_file))
    {
        return -1;
    }
    helf_moxie_section_info_t info;
    if(!helf_moxie_section_info_get(input_file,index,&info))
    {
        return -1;
    }
    return (intptr_t)info.offset;
}

size_t helf_moxie_rw_section_size_get(helf_file_input_t *input_file)
{
    size_t count=helf_moxie_section_count_get(input_file);
    size_t ret=0;
    for(size_t i=0;i<count;i++)
    {
        helf_moxie_section_info_t info;
        if(!helf_moxie_section_info_get(input_file,i,&info))
        {
            return 0;
        }
        if(helf_moxie_section_info_is_rw(&info))
        {
            ret+=info.size;
        }
    }
    return ret;
}

bool helf_moxie_is_bss_section(size_t index,helf_file_input_t *input_file)
{
    helf_moxie_section_info_t info;
    if(!helf_moxie_section_info_get(input_file,index,&info))
    {
        return false;
    }
    return info.type==HELF_SHT_NOBITS;
}

size_t helf_moxie_program_header_count_get(helf_file_input_t *input_file)
{
    helf_elf32_file_header_t header;
    if(!helf_moxie_header_get(input_file,&header))
    {
        return 0;
    }
    return header.e_phnum;
}

bool helf_moxie_program_header_get(helf_file_input_t *input_file,size_t index,helf_elf32_program_header_t *programheader)
{
    if(programheader==NULL)
    {
        return false;
    }
    helf_elf32_file_header_t header;
    if(!helf_moxie_header_get(input_file,&header))
    {
        return false;
    }
    if(index>=header.e_phnum)
    {
        return false;
    }
    return helf_file_input_32_bits_program_header_get(input_file,index,programheader);
}

/* ----------------------------------------------------------------------------
 * 程序段(Program Header/Segment)视角的接口
 * ------------------------------------------------------------------------- */

size_t helf_moxie_segment_count_get(helf_file_input_t *input_file)
{
    return helf_moxie_program_header_count_get(input_file);
}

bool helf_moxie_segment_info_get(helf_file_input_t *input_file,size_t index,helf_moxie_segment_info_t *info)
{
    if(info==NULL)
    {
        return false;
    }
    helf_elf32_program_header_t programheader;
    if(!helf_moxie_program_header_get(input_file,index,&programheader))
    {
        return false;
    }
    memset(info,0,sizeof(*info));
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

bool helf_moxie_is_load_segment(size_t index,helf_file_input_t *input_file)
{
    helf_moxie_segment_info_t info;
    if(!helf_moxie_segment_info_get(input_file,index,&info))
    {
        return false;
    }
    return info.type==HELF_PT_LOAD;
}

bool helf_moxie_is_rx_segment(size_t index,helf_file_input_t *input_file)
{
    helf_moxie_segment_info_t info;
    if(!helf_moxie_segment_info_get(input_file,index,&info))
    {
        return false;
    }
    return info.type==HELF_PT_LOAD && ((info.flags&HELF_PF_X)!=0);
}

bool helf_moxie_is_ro_segment(size_t index,helf_file_input_t *input_file)
{
    helf_moxie_segment_info_t info;
    if(!helf_moxie_segment_info_get(input_file,index,&info))
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

bool helf_moxie_is_rw_segment(size_t index,helf_file_input_t *input_file)
{
    helf_moxie_segment_info_t info;
    if(!helf_moxie_segment_info_get(input_file,index,&info))
    {
        return false;
    }
    return info.type==HELF_PT_LOAD && ((info.flags&HELF_PF_W)!=0);
}

size_t helf_moxie_load_segment_count_get(helf_file_input_t *input_file)
{
    size_t count=helf_moxie_segment_count_get(input_file);
    size_t ret=0;
    for(size_t i=0;i<count;i++)
    {
        if(helf_moxie_is_load_segment(i,input_file))
        {
            ret++;
        }
    }
    return ret;
}

bool helf_moxie_load_segment_info_get(helf_file_input_t *input_file,size_t index,helf_moxie_segment_info_t *info)
{
    if(info==NULL)
    {
        return false;
    }
    size_t count=helf_moxie_segment_count_get(input_file);
    size_t current=0;
    for(size_t i=0;i<count;i++)
    {
        if(!helf_moxie_is_load_segment(i,input_file))
        {
            continue;
        }
        if(current==index)
        {
            return helf_moxie_segment_info_get(input_file,i,info);
        }
        current++;
    }
    return false;
}

size_t helf_moxie_load_segment_total_filesz_get(helf_file_input_t *input_file)
{
    size_t count=helf_moxie_segment_count_get(input_file);
    size_t ret=0;
    for(size_t i=0;i<count;i++)
    {
        if(!helf_moxie_is_load_segment(i,input_file))
        {
            continue;
        }
        helf_moxie_segment_info_t info;
        if(!helf_moxie_segment_info_get(input_file,i,&info))
        {
            return 0;
        }
        ret+=info.filesz;
    }
    return ret;
}

size_t helf_moxie_load_segment_total_memsz_get(helf_file_input_t *input_file)
{
    size_t count=helf_moxie_segment_count_get(input_file);
    size_t ret=0;
    for(size_t i=0;i<count;i++)
    {
        if(!helf_moxie_is_load_segment(i,input_file))
        {
            continue;
        }
        helf_moxie_segment_info_t info;
        if(!helf_moxie_segment_info_get(input_file,i,&info))
        {
            return 0;
        }
        ret+=info.memsz;
    }
    return ret;
}

size_t helf_moxie_relocation_section_count_get(helf_file_input_t *input_file)
{
    if(!helf_moxie_is_moxie(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_section_count_get(input_file);
}

bool helf_moxie_relocation_section_info_get(helf_file_input_t *input_file,size_t index,helf_arch_relocation_section_info_t *info)
{
    if(!helf_moxie_is_moxie(input_file))
    {
        return false;
    }
    return helf_arch_relocation_section_info_get(input_file,index,info);
}

size_t helf_moxie_relocation_count_get(helf_file_input_t *input_file,size_t section_index)
{
    if(!helf_moxie_is_moxie(input_file))
    {
        return 0;
    }
    return helf_arch_relocation_count_get(input_file,section_index);
}

bool helf_moxie_relocation_get(helf_file_input_t *input_file,size_t section_index,size_t index,helf_arch_relocation_info_t *info)
{
    if(!helf_moxie_is_moxie(input_file))
    {
        return false;
    }
    return helf_arch_relocation_get(input_file,section_index,index,info);
}

bool helf_moxie_relocation_symbol_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_symbol_info_t *info)
{
    if(!helf_moxie_is_moxie(input_file))
    {
        return false;
    }
    return helf_arch_relocation_symbol_info_get(input_file,relocation,info);
}

static const helf_arch_relocation_type_name_t helf_moxie_relocation_type_names[]=
{
    {0,0,"R_MOXIE_NONE"},
    {1,4,"R_MOXIE_32"},
    {2,2,"R_MOXIE_PCREL10"},
};

const char *helf_moxie_relocation_type_name_get(uint32_t type)
{
    return helf_arch_relocation_type_name_get(helf_moxie_relocation_type_names,sizeof(helf_moxie_relocation_type_names)/sizeof(helf_moxie_relocation_type_names[0]),type);
}

uint8_t helf_moxie_relocation_size_get(uint32_t type)
{
    return helf_arch_relocation_type_size_get(helf_moxie_relocation_type_names,sizeof(helf_moxie_relocation_type_names)/sizeof(helf_moxie_relocation_type_names[0]),type);
}

bool helf_moxie_relocation_address_info_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,helf_arch_relocation_address_info_t *info)
{
    if(!helf_moxie_is_moxie(input_file) || relocation==NULL)
    {
        return false;
    }
    uint8_t size=helf_moxie_relocation_size_get(relocation->type);
    if(size==0)
    {
        return false;
    }
    return helf_arch_relocation_address_info_get(input_file,relocation,size,info);
}

size_t helf_moxie_relocation_data_get(helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,void *buffer,size_t buffer_length)
{
    if(!helf_moxie_is_moxie(input_file) || relocation==NULL)
    {
        return 0;
    }
    uint8_t size=helf_moxie_relocation_size_get(relocation->type);
    if(size==0)
    {
        return 0;
    }
    return helf_arch_relocation_data_get(input_file,relocation,size,buffer,buffer_length);
}

bool helf_moxie_is_shared_object(helf_file_input_t *input_file)
{
    return helf_arch_is_shared_object(input_file,helf_moxie_machines,sizeof(helf_moxie_machines)/sizeof(helf_moxie_machines[0]),HELF_ELFCLASS_32);
}

bool helf_moxie_is_relocatable(helf_file_input_t *input_file)
{
    return helf_arch_is_relocatable(input_file,helf_moxie_machines,sizeof(helf_moxie_machines)/sizeof(helf_moxie_machines[0]),HELF_ELFCLASS_32);
}

size_t helf_moxie_dynamic_count_get(helf_file_input_t *input_file)
{
    if(!helf_moxie_is_moxie(input_file))
    {
        return 0;
    }
    return helf_arch_dynamic_count_get(input_file);
}

bool helf_moxie_dynamic_info_get(helf_file_input_t *input_file,size_t index,helf_arch_dynamic_info_t *info)
{
    if(!helf_moxie_is_moxie(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_info_get(input_file,index,info);
}

bool helf_moxie_dynamic_string_get(helf_file_input_t *input_file,uint64_t offset,char *buffer,size_t buffer_length)
{
    if(!helf_moxie_is_moxie(input_file))
    {
        return false;
    }
    return helf_arch_dynamic_string_get(input_file,offset,buffer,buffer_length);
}
