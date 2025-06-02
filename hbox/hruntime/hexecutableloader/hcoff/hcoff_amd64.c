/***************************************************************
 * Name:      hcoff_amd64.c
 * Purpose:   实现hcoff_amd64接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-23
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hcoff_amd64.h"


const hcoff_amd64_section_relocation_info_t * hcoff_amd64_section_relocation_info_get(const hcoff_amd64_section_relocation_t *relocation)
{
    static const hcoff_amd64_section_relocation_info_t null_relocation_info=
    {
        "",
        0,
        0,
        0,
        0
    };
    const hcoff_amd64_section_relocation_info_t *ret=&null_relocation_info;
    if(relocation!=NULL)
    {
        switch(relocation->r_type)
        {
        case HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_DIR64:
        {
            static const hcoff_amd64_section_relocation_info_t relocation_info=
            {
                "IMAGE_REL_AMD64_ADDR64",
                0xFFFFFFFFFFFFFFFF,
                0xFFFFFFFFFFFFFFFF,
                HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_DIR64,
                8
            };
            ret=&relocation_info;

        }
        break;
        case HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_DIR32:
        {
            static const hcoff_amd64_section_relocation_info_t relocation_info=
            {
                "IMAGE_REL_AMD64_ADDR32",
                0xFFFFFFFF,
                0xFFFFFFFF,
                HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_DIR32,
                4
            };
            ret=&relocation_info;

        }
        break;
        case HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_IMAGEBASE:
        {
            static const hcoff_amd64_section_relocation_info_t relocation_info=
            {
                "IMAGE_REL_AMD64_ADDR32NB",
                0xFFFFFFFF,
                0xFFFFFFFF,
                HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_IMAGEBASE,
                4
            };
            ret=&relocation_info;

        }
        break;
        case HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_PCRLONG :
        {
            static const hcoff_amd64_section_relocation_info_t relocation_info=
            {
                "IMAGE_REL_AMD64_REL32",
                0xFFFFFFFF,
                0xFFFFFFFF,
                HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_PCRLONG,
                4
            };
            ret=&relocation_info;
        }
        break;
        case HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_PCRLONG_1:
        {
            static const hcoff_amd64_section_relocation_info_t relocation_info=
            {
                "IMAGE_REL_AMD64_REL32_1",
                0xFFFFFFFF,
                0xFFFFFFFF,
                HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_PCRLONG_1,
                4
            };
            ret=&relocation_info;
        }
        break;
        case HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_PCRLONG_2:
        {
            static const hcoff_amd64_section_relocation_info_t relocation_info=
            {
                "IMAGE_REL_AMD64_REL32_2",
                0xFFFFFFFF,
                0xFFFFFFFF,
                HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_PCRLONG_2,
                4
            };
            ret=&relocation_info;
        }
        break;
        case HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_PCRLONG_3:
        {
            static const hcoff_amd64_section_relocation_info_t relocation_info=
            {
                "IMAGE_REL_AMD64_REL32_3",
                0xFFFFFFFF,
                0xFFFFFFFF,
                HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_PCRLONG_3,
                4
            };
            ret=&relocation_info;
        }
        break;
        case HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_PCRLONG_4:
        {
            static const hcoff_amd64_section_relocation_info_t relocation_info=
            {
                "IMAGE_REL_AMD64_REL32_4",
                0xFFFFFFFF,
                0xFFFFFFFF,
                HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_PCRLONG_4,
                4
            };
            ret=&relocation_info;
        }
        break;
        case HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_PCRLONG_5:
        {
            static const hcoff_amd64_section_relocation_info_t relocation_info=
            {
                "IMAGE_REL_AMD64_REL32_5",
                0xFFFFFFFF,
                0xFFFFFFFF,
                HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_PCRLONG_5,
                4
            };
            ret=&relocation_info;
        }
        break;
        case HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_SECTION:
        {
            static const hcoff_amd64_section_relocation_info_t relocation_info=
            {
                "IMAGE_REL_AMD64_SECTION",
                0x0000FFFF,
                0x0000FFFF,
                HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_SECTION,
                2
            };
            ret=&relocation_info;
        }
        break;
        case HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_SECREL :
        {
            static const hcoff_amd64_section_relocation_info_t relocation_info=
            {
                "MAGE_REL_AMD64_SECREL",
                0xFFFFFFFF,
                0xFFFFFFFF,
                HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_SECREL,
                4
            };
            ret=&relocation_info;
        }
        break;
        case HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_PCRQUAD :
        {
            static const hcoff_amd64_section_relocation_info_t relocation_info=
            {
                "R_X86_64_PC64",
                0xFFFFFFFFFFFFFFFF,
                0xFFFFFFFFFFFFFFFF,
                HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_DIR64,
                8
            };
            ret=&relocation_info;
        }
        break;
        case HCOFF_SECTION_RELOCATION_R_TYPE_I386_RELBYTE:
        {
            static const hcoff_amd64_section_relocation_info_t relocation_info=
            {
                "R_X86_64_8",
                0x000000FF,
                0x000000FF,
                HCOFF_SECTION_RELOCATION_R_TYPE_I386_RELBYTE,
                1
            };
            ret=&relocation_info;
        }
        break;
        case HCOFF_SECTION_RELOCATION_R_TYPE_I386_RELWORD:
        {
            static const hcoff_amd64_section_relocation_info_t relocation_info=
            {
                "R_X86_64_16",
                0x0000FFFF,
                0x0000FFFF,
                HCOFF_SECTION_RELOCATION_R_TYPE_I386_RELWORD,
                2
            };
            ret=&relocation_info;
        }
        break;
        case HCOFF_SECTION_RELOCATION_R_TYPE_I386_RELLONG:
        {
            static const hcoff_amd64_section_relocation_info_t relocation_info=
            {
                "R_X86_64_32S",
                0xFFFFFFFF,
                0xFFFFFFFF,
                HCOFF_SECTION_RELOCATION_R_TYPE_I386_RELLONG,
                4
            };
            ret=&relocation_info;
        }
        break;
        case HCOFF_SECTION_RELOCATION_R_TYPE_I386_PCRBYTE:
        {
            static const hcoff_amd64_section_relocation_info_t relocation_info=
            {
                "R_X86_64_PC8",
                0x000000FF,
                0x000000FF,
                HCOFF_SECTION_RELOCATION_R_TYPE_I386_PCRBYTE,
                1
            };
            ret=&relocation_info;
        }
        break;
        case HCOFF_SECTION_RELOCATION_R_TYPE_I386_PCRWORD:
        {
            static const hcoff_amd64_section_relocation_info_t relocation_info=
            {
                "R_X86_64_PC16",
                0x0000FFFF,
                0x0000FFFF,
                HCOFF_SECTION_RELOCATION_R_TYPE_I386_PCRWORD,
                2
            };
            ret=&relocation_info;
        }
        break;
        case HCOFF_SECTION_RELOCATION_R_TYPE_I386_PCRLONG:
        {
            static const hcoff_amd64_section_relocation_info_t relocation_info=
            {
                "R_X86_64_PC32",
                0xFFFFFFFF,
                0xFFFFFFFF,
                HCOFF_SECTION_RELOCATION_R_TYPE_I386_PCRLONG,
                4
            };
            ret=&relocation_info;
        }
        break;
        default:
        {

        }
        break;
        }
    }
    return ret;
}

bool hcoff_amd64_section_relocation_doit(uint8_t *relocation_addr,uint64_t diff,const hcoff_amd64_section_relocation_t *relocation)
{
    bool ret=false;
    if(relocation_addr==NULL || relocation ==NULL)
    {
        return ret;
    };
    const hcoff_amd64_section_relocation_info_t *info=hcoff_amd64_section_relocation_info_get(relocation);
    if(info==NULL || info->size_bytes==0 )
    {
        return ret;
    }
    ret=true;
    /*
     * amd64通常采用小端模式
     */
    switch(info->size_bytes)
    {
    case 1:
    {
        uint8_t x=relocation_addr[0];
        x=((x & ~info->dst_mask) | (((x & info->src_mask) + diff) & info->dst_mask));
        relocation_addr[0]=x;
    }
    break;
    case 2:
    {
        uint16_t x=relocation_addr[0]+relocation_addr[1]*(1ULL << 8);
        x=((x & ~info->dst_mask) | (((x & info->src_mask) + diff) & info->dst_mask));
        relocation_addr[0]=(x >> 0)&0xFF;
        relocation_addr[1]=(x >> 8)&0xFF;
    }
    break;
    case 4:
    {
        uint32_t x=relocation_addr[0]+relocation_addr[1]*(1ULL << 8)+relocation_addr[2]*(1ULL << 16)+relocation_addr[3]*(1ULL << 24);
        x=((x & ~info->dst_mask) | (((x & info->src_mask) + diff) & info->dst_mask));
        relocation_addr[0]=(x >> 0)&0xFF;
        relocation_addr[1]=(x >> 8)&0xFF;
        relocation_addr[2]=(x >> 16)&0xFF;
        relocation_addr[3]=(x >> 24)&0xFF;
    }
    break;
    case 8:
    {
        uint64_t x=relocation_addr[0]+relocation_addr[1]*(1ULL << 8)+relocation_addr[2]*(1ULL << 16)+relocation_addr[3]*(1ULL << 24)+relocation_addr[4]*(1ULL << 32)+relocation_addr[5]*(1ULL << 40)+relocation_addr[6]*(1ULL << 48)+relocation_addr[7]*(1ULL << 56);
        x=((x & ~info->dst_mask) | (((x & info->src_mask) + diff) & info->dst_mask));
        relocation_addr[0]=(x >> 0)&0xFF;
        relocation_addr[1]=(x >> 8)&0xFF;
        relocation_addr[2]=(x >> 16)&0xFF;
        relocation_addr[3]=(x >> 24)&0xFF;
        relocation_addr[4]=(x >> 32)&0xFF;
        relocation_addr[5]=(x >> 40)&0xFF;
        relocation_addr[6]=(x >> 48)&0xFF;
        relocation_addr[7]=(x >> 56)&0xFF;
    }
    break;
    default:
    {
        ret=false;
    }
    break;
    }
    return ret;
}

bool hcoff_amd64_is_rx_section(size_t index,hcoff_file_input_t *input_file)
{
    bool ret=false;
    hcoff_sectionheader_t shdr= {0};
    if(!hcoff_sectionheader_read(&shdr,index,input_file))
    {
        return ret;
    }
    if(!(hcoff_sectionheader_name_match(HCOFF_SECTIONHEADER_S_NAME_TEXT,&shdr,input_file)))
    {
        return ret;
    }
    ret=true;
    return ret;
}

intptr_t hcoff_amd64_rx_section_offset_get(size_t index,hcoff_file_input_t *input_file)
{
    intptr_t ret=-1;
    if(input_file==NULL)
    {
        return ret;
    }
    hcoff_fileheader_t fhdr= {0};
    {
        uint8_t buffer[sizeof(hcoff_fileheader_t)]= {0};
        if(sizeof(buffer) > hcoff_file_input_read(input_file,0,buffer,sizeof(buffer)))
        {
            return ret;
        }
        if(!hcoff_fileheader_read(&fhdr,buffer,sizeof(buffer)))
        {
            return ret;
        }
    }
    if(fhdr.f_nscns <= index)
    {
        return ret;
    }
    if(!hcoff_amd64_is_rx_section(index,input_file))
    {
        return ret;
    }
    ret=0;
    for(size_t i=0; i<index; i++)
    {
        hcoff_sectionheader_t shdr= {0};
        if(!hcoff_sectionheader_read(&shdr,i,input_file))
        {
            ret=-1;
            break;
        }
        if(hcoff_amd64_is_rx_section(i,input_file))
        {
            //默认大小8字节对齐
            ret+=shdr.s_size/8*8+((shdr.s_size%8)!=0?8:0);
        }
    }
    return ret;
}

size_t hcoff_amd64_rx_section_size_get(hcoff_file_input_t *input_file)
{
    size_t ret=0;
    if(input_file==NULL)
    {
        return ret;
    }
    hcoff_fileheader_t fhdr= {0};
    {
        uint8_t buffer[sizeof(hcoff_fileheader_t)]= {0};
        if(sizeof(buffer) > hcoff_file_input_read(input_file,0,buffer,sizeof(buffer)))
        {
            return ret;
        }
        if(!hcoff_fileheader_read(&fhdr,buffer,sizeof(buffer)))
        {
            return ret;
        }
    }
    for(size_t i=0; i<fhdr.f_nscns; i++)
    {
        hcoff_sectionheader_t shdr= {0};
        if(!hcoff_sectionheader_read(&shdr,i,input_file))
        {
            ret=0;
            break;
        }
        if(hcoff_amd64_is_rx_section(i,input_file))
        {
            //默认大小8字节对齐
            ret+=shdr.s_size/8*8+((shdr.s_size%8)!=0?8:0);
        }
    }
    return ret;
}

bool hcoff_amd64_is_ro_section(size_t index,hcoff_file_input_t *input_file)
{
    bool ret=false;
    hcoff_sectionheader_t shdr= {0};
    if(!hcoff_sectionheader_read(&shdr,index,input_file))
    {
        return ret;
    }
    if(!(hcoff_sectionheader_name_match(HCOFF_SECTIONHEADER_S_NAME_RDATA,&shdr,input_file)))
    {
        return ret;
    }
    ret=true;
    return ret;
}

intptr_t hcoff_amd64_ro_section_offset_get(size_t index,hcoff_file_input_t *input_file)
{
    intptr_t ret=-1;
    if(input_file==NULL)
    {
        return ret;
    }
    hcoff_fileheader_t fhdr= {0};
    {
        uint8_t buffer[sizeof(hcoff_fileheader_t)]= {0};
        if(sizeof(buffer) > hcoff_file_input_read(input_file,0,buffer,sizeof(buffer)))
        {
            return ret;
        }
        if(!hcoff_fileheader_read(&fhdr,buffer,sizeof(buffer)))
        {
            return ret;
        }
    }
    if(fhdr.f_nscns <= index)
    {
        return ret;
    }
    if(!hcoff_amd64_is_ro_section(index,input_file))
    {
        return ret;
    }
    ret=0;
    for(size_t i=0; i<index; i++)
    {
        hcoff_sectionheader_t shdr= {0};
        if(!hcoff_sectionheader_read(&shdr,i,input_file))
        {
            ret=-1;
            break;
        }
        if(hcoff_amd64_is_ro_section(i,input_file))
        {
            //默认大小8字节对齐
            ret+=shdr.s_size/8*8+((shdr.s_size%8)!=0?8:0);
        }
    }
    return ret;
}

size_t hcoff_amd64_ro_section_size_get(hcoff_file_input_t *input_file)
{
    size_t ret=0;
    if(input_file==NULL)
    {
        return ret;
    }
    hcoff_fileheader_t fhdr= {0};
    {
        uint8_t buffer[sizeof(hcoff_fileheader_t)]= {0};
        if(sizeof(buffer) > hcoff_file_input_read(input_file,0,buffer,sizeof(buffer)))
        {
            return ret;
        }
        if(!hcoff_fileheader_read(&fhdr,buffer,sizeof(buffer)))
        {
            return ret;
        }
    }
    for(size_t i=0; i<fhdr.f_nscns; i++)
    {
        hcoff_sectionheader_t shdr= {0};
        if(!hcoff_sectionheader_read(&shdr,i,input_file))
        {
            ret=0;
            break;
        }
        if(hcoff_amd64_is_ro_section(i,input_file))
        {
            //默认大小8字节对齐
            ret+=shdr.s_size/8*8+((shdr.s_size%8)!=0?8:0);
        }
    }
    return ret;
}

bool hcoff_amd64_is_rw_section(size_t index,hcoff_file_input_t *input_file)
{
    bool ret=false;
    hcoff_sectionheader_t shdr= {0};
    if(!hcoff_sectionheader_read(&shdr,index,input_file))
    {
        return ret;
    }
    if(!(hcoff_sectionheader_name_match(HCOFF_SECTIONHEADER_S_NAME_DATA,&shdr,input_file) || hcoff_sectionheader_name_match(HCOFF_SECTIONHEADER_S_NAME_BSS,&shdr,input_file)))
    {
        return ret;
    }
    ret=true;
    return ret;
}

intptr_t hcoff_amd64_rw_section_offset_get(size_t index,hcoff_file_input_t *input_file)
{
    intptr_t ret=-1;
    if(input_file==NULL)
    {
        return ret;
    }
    hcoff_fileheader_t fhdr= {0};
    {
        uint8_t buffer[sizeof(hcoff_fileheader_t)]= {0};
        if(sizeof(buffer) > hcoff_file_input_read(input_file,0,buffer,sizeof(buffer)))
        {
            return ret;
        }
        if(!hcoff_fileheader_read(&fhdr,buffer,sizeof(buffer)))
        {
            return ret;
        }
    }
    if(fhdr.f_nscns <= index)
    {
        return ret;
    }
    if(!hcoff_amd64_is_rw_section(index,input_file))
    {
        return ret;
    }
    ret=0;
    for(size_t i=0; i<index; i++)
    {
        hcoff_sectionheader_t shdr= {0};
        if(!hcoff_sectionheader_read(&shdr,i,input_file))
        {
            ret=-1;
            break;
        }
        if(hcoff_amd64_is_rw_section(i,input_file))
        {
            //默认大小8字节对齐
            ret+=shdr.s_size/8*8+((shdr.s_size%8)!=0?8:0);
        }
    }
    return ret;
}

size_t hcoff_amd64_rw_section_size_get(hcoff_file_input_t *input_file)
{
    size_t ret=0;
    if(input_file==NULL)
    {
        return ret;
    }
    hcoff_fileheader_t fhdr= {0};
    {
        uint8_t buffer[sizeof(hcoff_fileheader_t)]= {0};
        if(sizeof(buffer) > hcoff_file_input_read(input_file,0,buffer,sizeof(buffer)))
        {
            return ret;
        }
        if(!hcoff_fileheader_read(&fhdr,buffer,sizeof(buffer)))
        {
            return ret;
        }
    }
    for(size_t i=0; i<fhdr.f_nscns; i++)
    {
        hcoff_sectionheader_t shdr= {0};
        if(!hcoff_sectionheader_read(&shdr,i,input_file))
        {
            ret=0;
            break;
        }
        if(hcoff_amd64_is_rw_section(i,input_file))
        {
            //默认大小8字节对齐
            ret+=shdr.s_size/8*8+((shdr.s_size%8)!=0?8:0);
        }
    }
    return ret;
}

static const hruntime_symbol_t *hcoff_amd64_external_symbol_find_with_usr(const char *name,void *usr)
{
    if(usr!=NULL)
    {
        hcoff_amd64_external_symbol_find_t extern_symbol=(hcoff_amd64_external_symbol_find_t)usr;
        if(extern_symbol!=NULL)
        {
            return extern_symbol(name);
        }
    }
    return NULL;
}
const hcoff_amd64_relocatable_t *hcoff_amd64_relocatable_new(hcoff_file_input_t *input_file,hcoff_amd64_external_symbol_find_t extern_symbol)
{
    return hcoff_amd64_relocatable_new_with_usr(input_file,hcoff_amd64_external_symbol_find_with_usr,extern_symbol);
}

const hcoff_amd64_relocatable_t *hcoff_amd64_relocatable_new_with_usr(hcoff_file_input_t *input_file,hcoff_amd64_external_symbol_find_with_usr_t extern_symbol,void *usr)
{
    if(input_file==NULL)
    {
        return NULL;
    }
    hcoff_fileheader_t fhdr= {0};
    {
        uint8_t buffer[sizeof(hcoff_fileheader_t)]= {0};
        if(sizeof(buffer) > hcoff_file_input_read(input_file,0,buffer,sizeof(buffer)))
        {
            return NULL;
        }
        if(!hcoff_fileheader_read(&fhdr,buffer,sizeof(buffer)))
        {
            return NULL;
        }
    }

    if(fhdr.f_magic!=HCOFF_FILEHEADER_F_MAGIC_AMD64 || !hcoff_fileheader_is_relocatable_object_file(&fhdr))
    {
        return NULL;
    }

    hcoff_amd64_relocatable_t *ret=(hcoff_amd64_relocatable_t *)hmalloc(sizeof(hcoff_amd64_relocatable_t));
    if(ret==NULL)
    {
        return ret;
    }
    memset(ret,0,sizeof(hcoff_amd64_relocatable_t));
    ret->rx_sections_size=hcoff_amd64_rx_section_size_get(input_file);
    ret->ro_sections_size=hcoff_amd64_ro_section_size_get(input_file);
    ret->rw_sections_size=hcoff_amd64_rw_section_size_get(input_file);
    if(ret->rx_sections_size == 0 && ret->ro_sections_size == 0 && ret->rw_sections_size == 0)
    {
        hcoff_amd64_relocatable_delete(ret);
        return NULL;
    }
    /*
     * 第一次申请时均采用RWX/RW权限，写入完成后再采用其它权限
     */
    if(ret->rx_sections_size != 0)
    {
        ret->rx_sections_ptr=hmmap(NULL,ret->rx_sections_size,HMMAN_PROT_READ | HMMAN_PROT_WRITE | HMMAN_PROT_EXEC,   HMMAN_MAP_ANONYMOUS | HMMAN_MAP_SHARED,(-1),0);
        if(ret->rx_sections_ptr==NULL)
        {
            hcoff_amd64_relocatable_delete(ret);
            return NULL;
        }
    }
    if(ret->ro_sections_size != 0)
    {
        ret->ro_sections_ptr=hmmap(NULL,ret->ro_sections_size,HMMAN_PROT_READ | HMMAN_PROT_WRITE , HMMAN_MAP_ANONYMOUS | HMMAN_MAP_SHARED,(-1),0);
        if(ret->ro_sections_ptr==NULL)
        {
            hcoff_amd64_relocatable_delete(ret);
            return NULL;
        }
    }
    if(ret->rw_sections_size != 0)
    {
        ret->rw_sections_ptr=hmmap(NULL,ret->rw_sections_size,HMMAN_PROT_READ | HMMAN_PROT_WRITE, HMMAN_MAP_ANONYMOUS | HMMAN_MAP_SHARED,(-1),0);
        if(ret->rw_sections_ptr==NULL)
        {
            hcoff_amd64_relocatable_delete(ret);
            return NULL;
        }
    }

    /*
     * 读取节中的内容并重定位
     */
    for(size_t i=0; i< fhdr.f_nscns; i++)
    {
        hcoff_sectionheader_t shdr= {0};
        if(!hcoff_sectionheader_read(&shdr,i,input_file))
        {
            hcoff_amd64_relocatable_delete(ret);
            return NULL;
        }
        if(hcoff_amd64_is_rx_section(i,input_file))
        {
            intptr_t section_offset=hcoff_amd64_rx_section_offset_get(i,input_file);
            if( section_offset < 0)
            {
                hcoff_amd64_relocatable_delete(ret);
                return NULL;
            }
            size_t section_size=hcoff_amd64_rx_section_size_get(input_file);
            if(section_size!=0 && shdr.s_size!=0)
            {
                if(shdr.s_size > hcoff_file_input_read(input_file,shdr.s_scnptr,((uint8_t *)ret->rx_sections_ptr)+section_offset,shdr.s_size))
                {
                    hcoff_amd64_relocatable_delete(ret);
                    return NULL;
                }
                uint8_t * section_vm_address=((uint8_t *)ret->rx_sections_ptr)+section_offset;
                for(size_t j=0; j < shdr.s_nreloc; j++)
                {
                    hcoff_section_relocation_t relocation= {0};
                    if(!hcoff_section_relocation_read(&relocation,j,&shdr,input_file))
                    {
                        hcoff_amd64_relocatable_delete(ret);
                        return NULL;
                    }
                    uint8_t *relocation_vm_address=section_vm_address+relocation.r_vaddr-shdr.s_vaddr;
                    if(!hcoff_symbol_is_symbol(relocation.r_symndx,input_file))
                    {
                        hcoff_amd64_relocatable_delete(ret);
                        return NULL;
                    }
                    hcoff_symbol_entry_t symbol_entry= {0};
                    if(!hcoff_symbol_entry_read(&symbol_entry,relocation.r_symndx,input_file))
                    {
                        hcoff_amd64_relocatable_delete(ret);
                        return NULL;
                    }
                    uint8_t *symbol_vm_address=NULL;
                    if(symbol_entry.e_scnum > 0)
                    {
                        //可重定位对象内部的符号
                        size_t symbol_section_index=symbol_entry.e_scnum-1;
                        if(hcoff_amd64_is_rx_section(symbol_section_index,input_file))
                        {
                            hcoff_sectionheader_t symbol_shdr= {0};
                            if(!hcoff_sectionheader_read(&symbol_shdr,symbol_section_index,input_file))
                            {
                                hcoff_amd64_relocatable_delete(ret);
                                return NULL;
                            }
                            intptr_t symbol_section_offset=hcoff_amd64_rx_section_offset_get(symbol_section_index,input_file);
                            if( symbol_section_offset < 0)
                            {
                                hcoff_amd64_relocatable_delete(ret);
                                return NULL;
                            }
                            uint8_t * symbol_section_vm_address=((uint8_t *)ret->rx_sections_ptr)+symbol_section_offset;
                            symbol_vm_address=symbol_section_vm_address+symbol_entry.e_value-symbol_shdr.s_vaddr;
                        }
                        if(hcoff_amd64_is_ro_section(symbol_section_index,input_file))
                        {
                            hcoff_sectionheader_t symbol_shdr= {0};
                            if(!hcoff_sectionheader_read(&symbol_shdr,symbol_section_index,input_file))
                            {
                                hcoff_amd64_relocatable_delete(ret);
                                return NULL;
                            }
                            intptr_t symbol_section_offset=hcoff_amd64_ro_section_offset_get(symbol_section_index,input_file);
                            if( symbol_section_offset < 0)
                            {
                                hcoff_amd64_relocatable_delete(ret);
                                return NULL;
                            }
                            uint8_t * symbol_section_vm_address=((uint8_t *)ret->ro_sections_ptr)+symbol_section_offset;
                            symbol_vm_address=symbol_section_vm_address+symbol_entry.e_value-symbol_shdr.s_vaddr;
                        }
                        if(hcoff_amd64_is_rw_section(symbol_section_index,input_file))
                        {
                            hcoff_sectionheader_t symbol_shdr= {0};
                            if(!hcoff_sectionheader_read(&symbol_shdr,symbol_section_index,input_file))
                            {
                                hcoff_amd64_relocatable_delete(ret);
                                return NULL;
                            }
                            intptr_t symbol_section_offset=hcoff_amd64_rw_section_offset_get(symbol_section_index,input_file);
                            if( symbol_section_offset < 0)
                            {
                                hcoff_amd64_relocatable_delete(ret);
                                return NULL;
                            }
                            uint8_t * symbol_section_vm_address=((uint8_t *)ret->rw_sections_ptr)+symbol_section_offset;
                            symbol_vm_address=symbol_section_vm_address+symbol_entry.e_value-symbol_shdr.s_vaddr;
                        }
                    }
                    else
                    {
                        if((symbol_entry.e_type&HCOFF_SYMBOL_ENTRY_E_TYPE_SYMBOL_KEEP)==0)
                        {
                            hcoff_amd64_relocatable_delete(ret);
                            return NULL;
                        }
                        char name[64]= {0};
                        if(!hcoff_symbol_entry_name_read(&symbol_entry,input_file,name,sizeof(name)))
                        {
                            hcoff_amd64_relocatable_delete(ret);
                            return NULL;
                        }
                        if(extern_symbol==NULL)
                        {
                            hcoff_amd64_relocatable_delete(ret);
                            return NULL;
                        }
                        const hruntime_symbol_t *runtime_symbol=extern_symbol(name,usr);
                        if(runtime_symbol==NULL)
                        {
                            //符号名称可能经过修饰
                            runtime_symbol=extern_symbol(&name[1],usr);
                        }
                        if(runtime_symbol!=NULL)
                        {
                            symbol_vm_address=(uint8_t *)runtime_symbol->symbol_addr;
                        }
                    }
                    if(symbol_vm_address==NULL)
                    {
                        hcoff_amd64_relocatable_delete(ret);
                        return NULL;
                    }
                    switch(relocation.r_type)
                    {
                    case  HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_DIR64:
                    {
                        if(!hcoff_amd64_section_relocation_doit(relocation_vm_address,(uintptr_t)symbol_vm_address,&relocation))
                        {
                            hcoff_amd64_relocatable_delete(ret);
                            return NULL;
                        }
                    }
                    break;
                    case  HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_DIR32:
                    {
                        if(!hcoff_amd64_section_relocation_doit(relocation_vm_address,(uintptr_t)symbol_vm_address,&relocation))
                        {
                            hcoff_amd64_relocatable_delete(ret);
                            return NULL;
                        }
                    }
                    break;
                    case  HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_PCRLONG:
                    case  HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_PCRLONG_1:
                    case  HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_PCRLONG_2:
                    case  HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_PCRLONG_3:
                    case  HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_PCRLONG_4:
                    case  HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_PCRLONG_5:
                    {
                        if(!hcoff_amd64_section_relocation_doit(relocation_vm_address,(uintptr_t)symbol_vm_address-((uintptr_t)relocation_vm_address+4+relocation.r_type-HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_PCRLONG),&relocation))
                        {
                            hcoff_amd64_relocatable_delete(ret);
                            return NULL;
                        }
                    }
                    break;
                    case  HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_PCRQUAD:
                    {
                        if(!hcoff_amd64_section_relocation_doit(relocation_vm_address,(uintptr_t)symbol_vm_address-((uintptr_t)relocation_vm_address+8),&relocation))
                        {
                            hcoff_amd64_relocatable_delete(ret);
                            return NULL;
                        }
                    }
                    break;
                    case  HCOFF_SECTION_RELOCATION_R_TYPE_I386_PCRBYTE:
                    {
                        if(!hcoff_i386_section_relocation_doit(relocation_vm_address,(uintptr_t)symbol_vm_address-((uintptr_t)relocation_vm_address+1),&relocation))
                        {
                            hcoff_amd64_relocatable_delete(ret);
                            return NULL;
                        }
                    }
                    break;
                    case  HCOFF_SECTION_RELOCATION_R_TYPE_I386_PCRWORD:
                    {
                        if(!hcoff_i386_section_relocation_doit(relocation_vm_address,(uintptr_t)symbol_vm_address-((uintptr_t)relocation_vm_address+2),&relocation))
                        {
                            hcoff_amd64_relocatable_delete(ret);
                            return NULL;
                        }
                    }
                    break;
                    case  HCOFF_SECTION_RELOCATION_R_TYPE_I386_PCRLONG:
                    {
                        if(!hcoff_i386_section_relocation_doit(relocation_vm_address,(uintptr_t)symbol_vm_address-((uintptr_t)relocation_vm_address+4),&relocation))
                        {
                            hcoff_amd64_relocatable_delete(ret);
                            return NULL;
                        }
                    }
                    break;
                    default:
                    {
                        //暂时不支持的重定位类型
                        hcoff_amd64_relocatable_delete(ret);
                        ret=NULL;
                    }
                    break;
                    }
                    if(ret==NULL)
                    {
                        return ret;
                    }
                }
            }
        }

        if(hcoff_amd64_is_ro_section(i,input_file))
        {
            intptr_t section_offset=hcoff_amd64_ro_section_offset_get(i,input_file);
            if( section_offset < 0)
            {
                hcoff_amd64_relocatable_delete(ret);
                return NULL;
            }
            size_t section_size=hcoff_amd64_ro_section_size_get(input_file);
            if(section_size!=0 && shdr.s_size!=0)
            {
                if(shdr.s_size > hcoff_file_input_read(input_file,shdr.s_scnptr,((uint8_t *)ret->ro_sections_ptr)+section_offset,shdr.s_size))
                {
                    hcoff_amd64_relocatable_delete(ret);
                    return NULL;
                }
                uint8_t * section_vm_address=((uint8_t *)ret->ro_sections_ptr)+section_offset;
                for(size_t j=0; j < shdr.s_nreloc; j++)
                {
                    hcoff_section_relocation_t relocation= {0};
                    if(!hcoff_section_relocation_read(&relocation,j,&shdr,input_file))
                    {
                        hcoff_amd64_relocatable_delete(ret);
                        return NULL;
                    }
                    uint8_t *relocation_vm_address=section_vm_address+relocation.r_vaddr-shdr.s_vaddr;
                    if(!hcoff_symbol_is_symbol(relocation.r_symndx,input_file))
                    {
                        hcoff_amd64_relocatable_delete(ret);
                        return NULL;
                    }
                    hcoff_symbol_entry_t symbol_entry= {0};
                    if(!hcoff_symbol_entry_read(&symbol_entry,relocation.r_symndx,input_file))
                    {
                        hcoff_amd64_relocatable_delete(ret);
                        return NULL;
                    }
                    uint8_t *symbol_vm_address=NULL;
                    if(symbol_entry.e_scnum > 0)
                    {
                        //可重定位对象内部的符号
                        size_t symbol_section_index=symbol_entry.e_scnum-1;
                        if(hcoff_amd64_is_rx_section(symbol_section_index,input_file))
                        {
                            hcoff_sectionheader_t symbol_shdr= {0};
                            if(!hcoff_sectionheader_read(&symbol_shdr,symbol_section_index,input_file))
                            {
                                hcoff_amd64_relocatable_delete(ret);
                                return NULL;
                            }
                            intptr_t symbol_section_offset=hcoff_amd64_rx_section_offset_get(symbol_section_index,input_file);
                            if( symbol_section_offset < 0)
                            {
                                hcoff_amd64_relocatable_delete(ret);
                                return NULL;
                            }
                            uint8_t * symbol_section_vm_address=((uint8_t *)ret->rx_sections_ptr)+symbol_section_offset;
                            symbol_vm_address=symbol_section_vm_address+symbol_entry.e_value-symbol_shdr.s_vaddr;
                        }
                        if(hcoff_amd64_is_ro_section(symbol_section_index,input_file))
                        {
                            hcoff_sectionheader_t symbol_shdr= {0};
                            if(!hcoff_sectionheader_read(&symbol_shdr,symbol_section_index,input_file))
                            {
                                hcoff_amd64_relocatable_delete(ret);
                                return NULL;
                            }
                            intptr_t symbol_section_offset=hcoff_amd64_ro_section_offset_get(symbol_section_index,input_file);
                            if( symbol_section_offset < 0)
                            {
                                hcoff_amd64_relocatable_delete(ret);
                                return NULL;
                            }
                            uint8_t * symbol_section_vm_address=((uint8_t *)ret->ro_sections_ptr)+symbol_section_offset;
                            symbol_vm_address=symbol_section_vm_address+symbol_entry.e_value-symbol_shdr.s_vaddr;
                        }
                        if(hcoff_amd64_is_rw_section(symbol_section_index,input_file))
                        {
                            hcoff_sectionheader_t symbol_shdr= {0};
                            if(!hcoff_sectionheader_read(&symbol_shdr,symbol_section_index,input_file))
                            {
                                hcoff_amd64_relocatable_delete(ret);
                                return NULL;
                            }
                            intptr_t symbol_section_offset=hcoff_amd64_rw_section_offset_get(symbol_section_index,input_file);
                            if( symbol_section_offset < 0)
                            {
                                hcoff_amd64_relocatable_delete(ret);
                                return NULL;
                            }
                            uint8_t * symbol_section_vm_address=((uint8_t *)ret->rw_sections_ptr)+symbol_section_offset;
                            symbol_vm_address=symbol_section_vm_address+symbol_entry.e_value-symbol_shdr.s_vaddr;
                        }
                    }
                    else
                    {
                        if((symbol_entry.e_type&HCOFF_SYMBOL_ENTRY_E_TYPE_SYMBOL_KEEP)==0)
                        {
                            hcoff_amd64_relocatable_delete(ret);
                            return NULL;
                        }
                        char name[64]= {0};
                        if(!hcoff_symbol_entry_name_read(&symbol_entry,input_file,name,sizeof(name)))
                        {
                            hcoff_amd64_relocatable_delete(ret);
                            return NULL;
                        }
                        if(extern_symbol==NULL)
                        {
                            hcoff_amd64_relocatable_delete(ret);
                            return NULL;
                        }
                        const hruntime_symbol_t *runtime_symbol=extern_symbol(name,usr);
                        if(runtime_symbol==NULL)
                        {
                            //符号名称可能经过修饰
                            runtime_symbol=extern_symbol(&name[1],usr);
                        }
                        if(runtime_symbol!=NULL)
                        {
                            symbol_vm_address=(uint8_t *)runtime_symbol->symbol_addr;
                        }
                    }
                    if(symbol_vm_address==NULL)
                    {
                        hcoff_amd64_relocatable_delete(ret);
                        return NULL;
                    }
                    switch(relocation.r_type)
                    {
                    case  HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_DIR64:
                    {
                        if(!hcoff_amd64_section_relocation_doit(relocation_vm_address,(uintptr_t)symbol_vm_address,&relocation))
                        {
                            hcoff_amd64_relocatable_delete(ret);
                            return NULL;
                        }
                    }
                    break;
                    case  HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_DIR32:
                    {
                        if(!hcoff_amd64_section_relocation_doit(relocation_vm_address,(uintptr_t)symbol_vm_address,&relocation))
                        {
                            hcoff_amd64_relocatable_delete(ret);
                            return NULL;
                        }
                    }
                    break;
                    case  HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_PCRLONG:
                    case  HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_PCRLONG_1:
                    case  HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_PCRLONG_2:
                    case  HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_PCRLONG_3:
                    case  HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_PCRLONG_4:
                    case  HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_PCRLONG_5:
                    {
                        if(!hcoff_amd64_section_relocation_doit(relocation_vm_address,(uintptr_t)symbol_vm_address-((uintptr_t)relocation_vm_address+4+relocation.r_type-HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_PCRLONG),&relocation))
                        {
                            hcoff_amd64_relocatable_delete(ret);
                            return NULL;
                        }
                    }
                    break;
                    case  HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_PCRQUAD:
                    {
                        if(!hcoff_amd64_section_relocation_doit(relocation_vm_address,(uintptr_t)symbol_vm_address-((uintptr_t)relocation_vm_address+8),&relocation))
                        {
                            hcoff_amd64_relocatable_delete(ret);
                            return NULL;
                        }
                    }
                    break;
                    case  HCOFF_SECTION_RELOCATION_R_TYPE_I386_PCRBYTE:
                    {
                        if(!hcoff_i386_section_relocation_doit(relocation_vm_address,(uintptr_t)symbol_vm_address-((uintptr_t)relocation_vm_address+1),&relocation))
                        {
                            hcoff_amd64_relocatable_delete(ret);
                            return NULL;
                        }
                    }
                    break;
                    case  HCOFF_SECTION_RELOCATION_R_TYPE_I386_PCRWORD:
                    {
                        if(!hcoff_i386_section_relocation_doit(relocation_vm_address,(uintptr_t)symbol_vm_address-((uintptr_t)relocation_vm_address+2),&relocation))
                        {
                            hcoff_amd64_relocatable_delete(ret);
                            return NULL;
                        }
                    }
                    break;
                    case  HCOFF_SECTION_RELOCATION_R_TYPE_I386_PCRLONG:
                    {
                        if(!hcoff_i386_section_relocation_doit(relocation_vm_address,(uintptr_t)symbol_vm_address-((uintptr_t)relocation_vm_address+4),&relocation))
                        {
                            hcoff_amd64_relocatable_delete(ret);
                            return NULL;
                        }
                    }
                    break;
                    default:
                    {
                        //暂时不支持的重定位类型
                        hcoff_amd64_relocatable_delete(ret);
                        ret=NULL;
                    }
                    break;
                    }
                    if(ret==NULL)
                    {
                        return ret;
                    }
                }
            }
        }
        if(hcoff_amd64_is_rw_section(i,input_file))
        {
            intptr_t section_offset=hcoff_amd64_rw_section_offset_get(i,input_file);
            if( section_offset < 0)
            {
                hcoff_amd64_relocatable_delete(ret);
                return NULL;
            }
            size_t section_size=hcoff_amd64_rw_section_size_get(input_file);
            if(section_size!=0 && shdr.s_size!=0)
            {
                //加载除BSS之外的数据
                if(!hcoff_sectionheader_name_match(HCOFF_SECTIONHEADER_S_NAME_BSS,&shdr,input_file))
                {
                    if(shdr.s_size > hcoff_file_input_read(input_file,shdr.s_scnptr,((uint8_t *)ret->rw_sections_ptr)+section_offset,shdr.s_size))
                    {
                        hcoff_amd64_relocatable_delete(ret);
                        return NULL;
                    }
                }

                uint8_t * section_vm_address=((uint8_t *)ret->rw_sections_ptr)+section_offset;
                for(size_t j=0; j < shdr.s_nreloc; j++)
                {
                    hcoff_section_relocation_t relocation= {0};
                    if(!hcoff_section_relocation_read(&relocation,j,&shdr,input_file))
                    {
                        hcoff_amd64_relocatable_delete(ret);
                        return NULL;
                    }
                    uint8_t *relocation_vm_address=section_vm_address+relocation.r_vaddr-shdr.s_vaddr;
                    if(!hcoff_symbol_is_symbol(relocation.r_symndx,input_file))
                    {
                        hcoff_amd64_relocatable_delete(ret);
                        return NULL;
                    }
                    hcoff_symbol_entry_t symbol_entry= {0};
                    if(!hcoff_symbol_entry_read(&symbol_entry,relocation.r_symndx,input_file))
                    {
                        hcoff_amd64_relocatable_delete(ret);
                        return NULL;
                    }
                    uint8_t *symbol_vm_address=NULL;
                    if(symbol_entry.e_scnum > 0)
                    {
                        //可重定位对象内部的符号
                        size_t symbol_section_index=symbol_entry.e_scnum-1;
                        if(hcoff_amd64_is_rx_section(symbol_section_index,input_file))
                        {
                            hcoff_sectionheader_t symbol_shdr= {0};
                            if(!hcoff_sectionheader_read(&symbol_shdr,symbol_section_index,input_file))
                            {
                                hcoff_amd64_relocatable_delete(ret);
                                return NULL;
                            }
                            intptr_t symbol_section_offset=hcoff_amd64_rx_section_offset_get(symbol_section_index,input_file);
                            if( symbol_section_offset < 0)
                            {
                                hcoff_amd64_relocatable_delete(ret);
                                return NULL;
                            }
                            uint8_t * symbol_section_vm_address=((uint8_t *)ret->rx_sections_ptr)+symbol_section_offset;
                            symbol_vm_address=symbol_section_vm_address+symbol_entry.e_value-symbol_shdr.s_vaddr;
                        }
                        if(hcoff_amd64_is_ro_section(symbol_section_index,input_file))
                        {
                            hcoff_sectionheader_t symbol_shdr= {0};
                            if(!hcoff_sectionheader_read(&symbol_shdr,symbol_section_index,input_file))
                            {
                                hcoff_amd64_relocatable_delete(ret);
                                return NULL;
                            }
                            intptr_t symbol_section_offset=hcoff_amd64_ro_section_offset_get(symbol_section_index,input_file);
                            if( symbol_section_offset < 0)
                            {
                                hcoff_amd64_relocatable_delete(ret);
                                return NULL;
                            }
                            uint8_t * symbol_section_vm_address=((uint8_t *)ret->ro_sections_ptr)+symbol_section_offset;
                            symbol_vm_address=symbol_section_vm_address+symbol_entry.e_value-symbol_shdr.s_vaddr;
                        }
                        if(hcoff_amd64_is_rw_section(symbol_section_index,input_file))
                        {
                            hcoff_sectionheader_t symbol_shdr= {0};
                            if(!hcoff_sectionheader_read(&symbol_shdr,symbol_section_index,input_file))
                            {
                                hcoff_amd64_relocatable_delete(ret);
                                return NULL;
                            }
                            intptr_t symbol_section_offset=hcoff_amd64_rw_section_offset_get(symbol_section_index,input_file);
                            if( symbol_section_offset < 0)
                            {
                                hcoff_amd64_relocatable_delete(ret);
                                return NULL;
                            }
                            uint8_t * symbol_section_vm_address=((uint8_t *)ret->rw_sections_ptr)+symbol_section_offset;
                            symbol_vm_address=symbol_section_vm_address+symbol_entry.e_value-symbol_shdr.s_vaddr;
                        }
                    }
                    else
                    {
                        if((symbol_entry.e_type&HCOFF_SYMBOL_ENTRY_E_TYPE_SYMBOL_KEEP)==0)
                        {
                            hcoff_amd64_relocatable_delete(ret);
                            return NULL;
                        }
                        char name[64]= {0};
                        if(!hcoff_symbol_entry_name_read(&symbol_entry,input_file,name,sizeof(name)))
                        {
                            hcoff_amd64_relocatable_delete(ret);
                            return NULL;
                        }
                        if(extern_symbol==NULL)
                        {
                            hcoff_amd64_relocatable_delete(ret);
                            return NULL;
                        }
                        const hruntime_symbol_t *runtime_symbol=extern_symbol(name,usr);
                        if(runtime_symbol==NULL)
                        {
                            //符号名称可能经过修饰
                            runtime_symbol=extern_symbol(&name[1],usr);
                        }
                        if(runtime_symbol!=NULL)
                        {
                            symbol_vm_address=(uint8_t *)runtime_symbol->symbol_addr;
                        }
                    }
                    if(symbol_vm_address==NULL)
                    {
                        hcoff_amd64_relocatable_delete(ret);
                        return NULL;
                    }
                    switch(relocation.r_type)
                    {
                    case  HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_DIR64:
                    {
                        if(!hcoff_amd64_section_relocation_doit(relocation_vm_address,(uintptr_t)symbol_vm_address,&relocation))
                        {
                            hcoff_amd64_relocatable_delete(ret);
                            return NULL;
                        }
                    }
                    break;
                    case  HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_DIR32:
                    {
                        if(!hcoff_amd64_section_relocation_doit(relocation_vm_address,(uintptr_t)symbol_vm_address,&relocation))
                        {
                            hcoff_amd64_relocatable_delete(ret);
                            return NULL;
                        }
                    }
                    break;
                    case  HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_PCRLONG:
                    case  HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_PCRLONG_1:
                    case  HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_PCRLONG_2:
                    case  HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_PCRLONG_3:
                    case  HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_PCRLONG_4:
                    case  HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_PCRLONG_5:
                    {
                        if(!hcoff_amd64_section_relocation_doit(relocation_vm_address,(uintptr_t)symbol_vm_address-((uintptr_t)relocation_vm_address+4+relocation.r_type-HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_PCRLONG),&relocation))
                        {
                            hcoff_amd64_relocatable_delete(ret);
                            return NULL;
                        }
                    }
                    break;
                    case  HCOFF_SECTION_RELOCATION_R_TYPE_X86_64_AMD64_PCRQUAD:
                    {
                        if(!hcoff_amd64_section_relocation_doit(relocation_vm_address,(uintptr_t)symbol_vm_address-((uintptr_t)relocation_vm_address+8),&relocation))
                        {
                            hcoff_amd64_relocatable_delete(ret);
                            return NULL;
                        }
                    }
                    break;
                    case  HCOFF_SECTION_RELOCATION_R_TYPE_I386_PCRBYTE:
                    {
                        if(!hcoff_i386_section_relocation_doit(relocation_vm_address,(uintptr_t)symbol_vm_address-((uintptr_t)relocation_vm_address+1),&relocation))
                        {
                            hcoff_amd64_relocatable_delete(ret);
                            return NULL;
                        }
                    }
                    break;
                    case  HCOFF_SECTION_RELOCATION_R_TYPE_I386_PCRWORD:
                    {
                        if(!hcoff_i386_section_relocation_doit(relocation_vm_address,(uintptr_t)symbol_vm_address-((uintptr_t)relocation_vm_address+2),&relocation))
                        {
                            hcoff_amd64_relocatable_delete(ret);
                            return NULL;
                        }
                    }
                    break;
                    case  HCOFF_SECTION_RELOCATION_R_TYPE_I386_PCRLONG:
                    {
                        if(!hcoff_i386_section_relocation_doit(relocation_vm_address,(uintptr_t)symbol_vm_address-((uintptr_t)relocation_vm_address+4),&relocation))
                        {
                            hcoff_amd64_relocatable_delete(ret);
                            return NULL;
                        }
                    }
                    break;
                    default:
                    {
                        //暂时不支持的重定位类型
                        hcoff_amd64_relocatable_delete(ret);
                        ret=NULL;
                    }
                    break;
                    }
                    if(ret==NULL)
                    {
                        return ret;
                    }
                }
            }
        }
    }


    /*
     * 设定符号表
     */
    {
        ret->symbol_table_size=0;
        size_t symbol_string_tab_size=0;
        for(size_t i=0; i<fhdr.f_nsyms; i++)
        {
            if(hcoff_symbol_is_symbol(i,input_file))
            {
                hcoff_symbol_entry_t symbol_entry= {0};
                if(!hcoff_symbol_entry_read(&symbol_entry,i,input_file))
                {
                    hcoff_amd64_relocatable_delete(ret);
                    return NULL;
                }
                if(symbol_entry.e_sclass!=HCOFF_SYMBOL_ENTRY_E_SCLASS_EXT  || symbol_entry.e_scnum <= 0)
                {
                    continue;
                }
                ret->symbol_table_size++;
                char name[64]= {0};
                if(!hcoff_symbol_entry_name_read(&symbol_entry,input_file,name,sizeof(name)))
                {
                    hcoff_amd64_relocatable_delete(ret);
                    return NULL;
                }
                symbol_string_tab_size+=(strlen(name)+1);
            }
        }
        if(ret->symbol_table_size==0)
        {
            //没有被导出的符号
            hcoff_amd64_relocatable_delete(ret);
            return NULL;
        }
        ret->symbol_table=(hruntime_symbol_t *)hmalloc(ret->symbol_table_size*sizeof(hruntime_symbol_t)+symbol_string_tab_size);
        if(ret->symbol_table==NULL)
        {
            hcoff_amd64_relocatable_delete(ret);
            return NULL;
        }
        memset(ret->symbol_table,0,ret->symbol_table_size*sizeof(hruntime_symbol_t)+symbol_string_tab_size);
        char *strtab_base=(char *)&ret->symbol_table[ret->symbol_table_size];
        size_t hruntime_symbol_index=0;
        symbol_string_tab_size=0;
        for(size_t i=0; i<fhdr.f_nsyms; i++)
        {
            if(hcoff_symbol_is_symbol(i,input_file))
            {
                hcoff_symbol_entry_t symbol_entry= {0};
                if(!hcoff_symbol_entry_read(&symbol_entry,i,input_file))
                {
                    hcoff_amd64_relocatable_delete(ret);
                    return NULL;
                }
                if(symbol_entry.e_sclass!=HCOFF_SYMBOL_ENTRY_E_SCLASS_EXT  || symbol_entry.e_scnum <= 0)
                {
                    continue;
                }
                uint8_t *symbol_vm_address=NULL;
                {
                    size_t symbol_section_index=symbol_entry.e_scnum-1;
                    if(hcoff_amd64_is_rx_section(symbol_section_index,input_file))
                    {
                        hcoff_sectionheader_t symbol_shdr= {0};
                        if(!hcoff_sectionheader_read(&symbol_shdr,symbol_section_index,input_file))
                        {
                            hcoff_amd64_relocatable_delete(ret);
                            return NULL;
                        }
                        intptr_t symbol_section_offset=hcoff_amd64_rx_section_offset_get(symbol_section_index,input_file);
                        if( symbol_section_offset < 0)
                        {
                            hcoff_amd64_relocatable_delete(ret);
                            return NULL;
                        }
                        uint8_t * symbol_section_vm_address=((uint8_t *)ret->rx_sections_ptr)+symbol_section_offset;
                        symbol_vm_address=symbol_section_vm_address+symbol_entry.e_value-symbol_shdr.s_vaddr;
                    }
                    if(hcoff_amd64_is_ro_section(symbol_section_index,input_file))
                    {
                        hcoff_sectionheader_t symbol_shdr= {0};
                        if(!hcoff_sectionheader_read(&symbol_shdr,symbol_section_index,input_file))
                        {
                            hcoff_amd64_relocatable_delete(ret);
                            return NULL;
                        }
                        intptr_t symbol_section_offset=hcoff_amd64_ro_section_offset_get(symbol_section_index,input_file);
                        if( symbol_section_offset < 0)
                        {
                            hcoff_amd64_relocatable_delete(ret);
                            return NULL;
                        }
                        uint8_t * symbol_section_vm_address=((uint8_t *)ret->ro_sections_ptr)+symbol_section_offset;
                        symbol_vm_address=symbol_section_vm_address+symbol_entry.e_value-symbol_shdr.s_vaddr;
                    }
                    if(hcoff_amd64_is_rw_section(symbol_section_index,input_file))
                    {
                        hcoff_sectionheader_t symbol_shdr= {0};
                        if(!hcoff_sectionheader_read(&symbol_shdr,symbol_section_index,input_file))
                        {
                            hcoff_amd64_relocatable_delete(ret);
                            return NULL;
                        }
                        intptr_t symbol_section_offset=hcoff_amd64_rw_section_offset_get(symbol_section_index,input_file);
                        if( symbol_section_offset < 0)
                        {
                            hcoff_amd64_relocatable_delete(ret);
                            return NULL;
                        }
                        uint8_t * symbol_section_vm_address=((uint8_t *)ret->rw_sections_ptr)+symbol_section_offset;
                        symbol_vm_address=symbol_section_vm_address+symbol_entry.e_value-symbol_shdr.s_vaddr;
                    }
                }
                ret->symbol_table[hruntime_symbol_index].symbol_addr=(uintptr_t)symbol_vm_address;
                char name[64]= {0};
                if(!hcoff_symbol_entry_name_read(&symbol_entry,input_file,name,sizeof(name)))
                {
                    hcoff_amd64_relocatable_delete(ret);
                    return NULL;
                }
                memcpy(&strtab_base[symbol_string_tab_size],name,strlen(name));
                ret->symbol_table[hruntime_symbol_index].symbol_name=&strtab_base[symbol_string_tab_size];
                symbol_string_tab_size+=(strlen(name)+1);
                hruntime_symbol_index++;
            }
        }
    }


    /*
     * 设定最终的权限
     */
    if(ret->rx_sections_size != 0)
    {
        hmsync(ret->rx_sections_ptr,ret->rx_sections_size,HMMAN_MS_SYNC);
        hmprotect(ret->rx_sections_ptr,ret->rx_sections_size,HMMAN_PROT_READ | HMMAN_PROT_EXEC);
    }
    if(ret->ro_sections_size != 0)
    {
        hmsync(ret->ro_sections_ptr,ret->ro_sections_size,HMMAN_MS_SYNC);
        hmprotect(ret->ro_sections_ptr,ret->ro_sections_size,HMMAN_PROT_READ);
    }
    if(ret->rw_sections_size != 0)
    {
        hmsync(ret->rw_sections_ptr,ret->rw_sections_size,HMMAN_MS_SYNC);
        hmprotect(ret->rw_sections_ptr,ret->rw_sections_size,HMMAN_PROT_READ | HMMAN_PROT_WRITE);
    }

    return ret;
}


void hcoff_amd64_relocatable_delete(const hcoff_amd64_relocatable_t * amd64_relocatable)
{
    hcoff_amd64_relocatable_t *ptr   =   (hcoff_amd64_relocatable_t *)amd64_relocatable;
    if(ptr!=NULL)
    {
        if(ptr->rx_sections_ptr!=NULL && ptr->rx_sections_size!=0)
        {
            hmunmap(ptr->rx_sections_ptr,ptr->rx_sections_size);
        }
        if(ptr->ro_sections_ptr!=NULL && ptr->ro_sections_size!=0)
        {
            hmunmap(ptr->ro_sections_ptr,ptr->ro_sections_size);
        }
        if(ptr->rw_sections_ptr!=NULL && ptr->rw_sections_size!=0)
        {
            hmunmap(ptr->rw_sections_ptr,ptr->rw_sections_size);
        }
        if(ptr->symbol_table!=NULL)
        {
            hfree(ptr->symbol_table);
        }
        hfree(ptr);
    }
}


