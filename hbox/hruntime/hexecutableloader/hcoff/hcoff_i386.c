/***************************************************************
 * Name:      hcoff_i386.c
 * Purpose:   实现hcoff_i386接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-21
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hcoff_i386.h"


const hcoff_i386_section_relocation_info_t * hcoff_i386_section_relocation_info_get(const hcoff_i386_section_relocation_t *relocation)
{
    static const hcoff_i386_section_relocation_info_t null_relocation_info=
    {
        "",
        0,
        0,
        0,
        0
    };
    const hcoff_i386_section_relocation_info_t *ret=&null_relocation_info;
    if(relocation!=NULL)
    {
        switch(relocation->r_type)
        {
        case HCOFF_SECTION_RELOCATION_R_TYPE_I386_DIR32:
        {
            static const hcoff_i386_section_relocation_info_t relocation_info=
            {
                "dir32",
                0xFFFFFFFF,
                0xFFFFFFFF,
                HCOFF_SECTION_RELOCATION_R_TYPE_I386_DIR32,
                4
            };
            ret=&relocation_info;
        }
        break;
        case HCOFF_SECTION_RELOCATION_R_TYPE_I386_IMAGEBASE:
        {
            static const hcoff_i386_section_relocation_info_t relocation_info=
            {
                "rva32",
                0xFFFFFFFF,
                0xFFFFFFFF,
                HCOFF_SECTION_RELOCATION_R_TYPE_I386_IMAGEBASE,
                4
            };
            ret=&relocation_info;
        }
        break;
        case HCOFF_SECTION_RELOCATION_R_TYPE_I386_SECTION:
        {
            static const hcoff_i386_section_relocation_info_t relocation_info=
            {
                "secidx",
                0xFFFFFFFF,
                0xFFFFFFFF,
                HCOFF_SECTION_RELOCATION_R_TYPE_I386_SECTION,
                2
            };
            ret=&relocation_info;
        }
        break;
        case HCOFF_SECTION_RELOCATION_R_TYPE_I386_SECREL32:
        {
            static const hcoff_i386_section_relocation_info_t relocation_info=
            {
                "secrel32",
                0xFFFFFFFF,
                0xFFFFFFFF,
                HCOFF_SECTION_RELOCATION_R_TYPE_I386_SECREL32,
                4
            };
            ret=&relocation_info;
        }
        break;
        case HCOFF_SECTION_RELOCATION_R_TYPE_I386_RELBYTE:
        {
            static const hcoff_i386_section_relocation_info_t relocation_info=
            {
                "8",
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
            static const hcoff_i386_section_relocation_info_t relocation_info=
            {
                "16",
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
            static const hcoff_i386_section_relocation_info_t relocation_info=
            {
                "32",
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
            static const hcoff_i386_section_relocation_info_t relocation_info=
            {
                "DISP8",
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
            static const hcoff_i386_section_relocation_info_t relocation_info=
            {
                "DISP16",
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
            static const hcoff_i386_section_relocation_info_t relocation_info=
            {
                "DISP32",
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

bool hcoff_i386_section_relocation_doit(uint8_t *relocation_addr,uint32_t diff,const hcoff_i386_section_relocation_t *relocation)
{
    bool ret=false;
    if(relocation_addr==NULL || relocation ==NULL)
    {
        return ret;
    };
    const hcoff_i386_section_relocation_info_t *info=hcoff_i386_section_relocation_info_get(relocation);
    if(info==NULL || info->size_bytes==0 )
    {
        return ret;
    }
    ret=true;
    /*
     * i386通常采用小端模式
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
    default:
    {
        ret=false;
    }
    break;
    }
    return ret;
}
