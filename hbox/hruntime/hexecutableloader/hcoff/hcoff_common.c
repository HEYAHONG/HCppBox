/***************************************************************
 * Name:      hcoff_common.c
 * Purpose:   实现hcoff_common接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-15
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hcoff_common.h"

static bool hcoff_fileheader_is_magic_number(uint16_t magic)
{
    bool ret=false;
    switch(magic)
    {
    case  HCOFF_FILEHEADER_F_MAGIC_UNKNOWN:
    case  HCOFF_FILEHEADER_F_MAGIC_ALPHA:
    case  HCOFF_FILEHEADER_F_MAGIC_ALPHA64:
    case  HCOFF_FILEHEADER_F_MAGIC_AM33:
    case  HCOFF_FILEHEADER_F_MAGIC_AMD64:
    case  HCOFF_FILEHEADER_F_MAGIC_ARM:
    case  HCOFF_FILEHEADER_F_MAGIC_ARM64:
    case  HCOFF_FILEHEADER_F_MAGIC_ARMNT:
    case  HCOFF_FILEHEADER_F_MAGIC_CEE:
    case  HCOFF_FILEHEADER_F_MAGIC_CEF:
    case  HCOFF_FILEHEADER_F_MAGIC_EBC:
    case  HCOFF_FILEHEADER_F_MAGIC_I386:
    case  HCOFF_FILEHEADER_F_MAGIC_IA64:
    case  HCOFF_FILEHEADER_F_MAGIC_LOONGARCH64:
    case  HCOFF_FILEHEADER_F_MAGIC_M32R:
    case  HCOFF_FILEHEADER_F_MAGIC_M68K:
    case  HCOFF_FILEHEADER_F_MAGIC_MIPS16:
    case  HCOFF_FILEHEADER_F_MAGIC_MIPSFPU:
    case  HCOFF_FILEHEADER_F_MAGIC_MIPSFPU16:
    case  HCOFF_FILEHEADER_F_MAGIC_POWERPC:
    case  HCOFF_FILEHEADER_F_MAGIC_POWERPCFP:
    case  HCOFF_FILEHEADER_F_MAGIC_R10000:
    case  HCOFF_FILEHEADER_F_MAGIC_R3000:
    case  HCOFF_FILEHEADER_F_MAGIC_R4000:
    case  HCOFF_FILEHEADER_F_MAGIC_RISCV32:
    case  HCOFF_FILEHEADER_F_MAGIC_RISCV64:
    case  HCOFF_FILEHEADER_F_MAGIC_SH3:
    case  HCOFF_FILEHEADER_F_MAGIC_SH3DSP:
    case  HCOFF_FILEHEADER_F_MAGIC_SH3E:
    case  HCOFF_FILEHEADER_F_MAGIC_SH4:
    case  HCOFF_FILEHEADER_F_MAGIC_SH5:
    case  HCOFF_FILEHEADER_F_MAGIC_THUMB:
    case  HCOFF_FILEHEADER_F_MAGIC_TRICORE:
    case  HCOFF_FILEHEADER_F_MAGIC_WCEMIPSV2:
    {
        ret=true;
    }
    break;
    default:
    {

    }
    break;
    }
    return ret;
}

typedef struct
{
    unsigned char f_magic[2];	/* magic number			*/
    unsigned char f_nscns[2];	/* number of sections		*/
    unsigned char f_timdat[4];	/* time & date stamp		*/
    unsigned char f_symptr[4];	/* file pointer to symtab	*/
    unsigned char f_nsyms[4];	/* number of symtab entries	*/
    unsigned char f_opthdr[2];	/* sizeof(optional hdr)		*/
    unsigned char f_flags[2];	/* flags			*/
} hcoff_fileheader_bytes_t;


bool hcoff_fileheader_read(hcoff_fileheader_t *fileheader,uint8_t* fileheader_bytes,size_t fileheader_bytes_length)
{
    bool ret=false;
    if(fileheader==NULL || fileheader_bytes == NULL || fileheader_bytes_length < sizeof(hcoff_fileheader_bytes_t))
    {
        return ret;
    }
    hcoff_fileheader_bytes_t hdr;
    memcpy(&hdr,fileheader_bytes,sizeof(hcoff_fileheader_bytes_t));
    if(!ret)
    {
        uint16_t magic_little=hdr.f_magic[0]*(1UL << 0)+hdr.f_magic[1]*(1UL << 8);
        if(hcoff_fileheader_is_magic_number(magic_little))
        {
            fileheader->f_magic=magic_little;
            fileheader->f_nscns=hdr.f_nscns[0]*(1UL << 0)+hdr.f_nscns[1]*(1UL << 8);
            fileheader->f_timdat=hdr.f_timdat[0]*(1UL << 0)+hdr.f_timdat[1]*(1UL << 8)+hdr.f_timdat[2]*(1UL << 16)+hdr.f_timdat[3]*(1UL << 24);
            fileheader->f_symptr=hdr.f_symptr[0]*(1UL << 0)+hdr.f_symptr[1]*(1UL << 8)+hdr.f_symptr[2]*(1UL << 16)+hdr.f_symptr[3]*(1UL << 24);
            fileheader->f_nsyms=hdr.f_nsyms[0]*(1UL << 0)+hdr.f_nsyms[1]*(1UL << 8)+hdr.f_nsyms[2]*(1UL << 16)+hdr.f_nsyms[3]*(1UL << 24);
            fileheader->f_opthdr=hdr.f_opthdr[0]*(1UL << 0)+hdr.f_opthdr[1]*(1UL << 8);
            fileheader->f_flags=hdr.f_flags[0]*(1UL << 0)+hdr.f_flags[1]*(1UL << 8);
            ret=true;
        }
    }
    if(!ret)
    {
        uint16_t magic_big=hdr.f_magic[1]*(1UL << 0)+hdr.f_magic[0]*(1UL << 8);
        if(hcoff_fileheader_is_magic_number(magic_big))
        {
            fileheader->f_magic=magic_big;
            fileheader->f_nscns=hdr.f_nscns[1]*(1UL << 0)+hdr.f_nscns[0]*(1UL << 8);
            fileheader->f_timdat=hdr.f_timdat[3]*(1UL << 0)+hdr.f_timdat[2]*(1UL << 8)+hdr.f_timdat[1]*(1UL << 16)+hdr.f_timdat[0]*(1UL << 24);
            fileheader->f_symptr=hdr.f_symptr[3]*(1UL << 0)+hdr.f_symptr[2]*(1UL << 8)+hdr.f_symptr[1]*(1UL << 16)+hdr.f_symptr[0]*(1UL << 24);
            fileheader->f_nsyms=hdr.f_nsyms[3]*(1UL << 0)+hdr.f_nsyms[2]*(1UL << 8)+hdr.f_nsyms[1]*(1UL << 16)+hdr.f_nsyms[0]*(1UL << 24);
            fileheader->f_opthdr=hdr.f_opthdr[1]*(1UL << 0)+hdr.f_opthdr[0]*(1UL << 8);
            fileheader->f_flags=hdr.f_flags[1]*(1UL << 0)+hdr.f_flags[0]*(1UL << 8);
            ret=true;
        }
    }
    return ret;
}



