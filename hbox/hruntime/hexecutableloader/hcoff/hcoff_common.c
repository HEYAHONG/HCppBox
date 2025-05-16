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

typedef struct
{
    unsigned char s_name[8];	/* section name				*/
    unsigned char s_paddr[4];	/* physical address, aliased s_nlib 	*/
    unsigned char s_vaddr[4];	/* virtual address			*/
    unsigned char s_size[4];	/* section size				*/
    unsigned char s_scnptr[4];	/* file ptr to raw data for section 	*/
    unsigned char s_relptr[4];	/* file ptr to relocation		*/
    unsigned char s_lnnoptr[4];	/* file ptr to line numbers		*/
    unsigned char s_nreloc[2];	/* number of relocation entries		*/
    unsigned char s_nlnno[2];	/* number of line number entries	*/
    unsigned char s_flags[4];	/* flags				*/
} hcoff_sectionheader_bytes_t;


bool hcoff_fileheader_read(hcoff_fileheader_t *fileheader,const uint8_t* fileheader_bytes,size_t fileheader_bytes_length)
{
    bool ret=false;
    if(fileheader==NULL || fileheader_bytes == NULL || fileheader_bytes_length < sizeof(hcoff_fileheader_bytes_t))
    {
        return ret;
    }
    fileheader->f_magic=HCOFF_FILEHEADER_F_MAGIC_UNKNOWN;
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

uintptr_t hcoff_fileheader_section_offset_get(hcoff_fileheader_t *fileheader)
{
    uintptr_t ret=0;
    if(fileheader!=NULL)
    {
        return sizeof(hcoff_fileheader_bytes_t)+fileheader->f_opthdr;
    }
    return ret;
}

size_t hcoff_fileheader_section_count_get(hcoff_fileheader_t *fileheader)
{
    size_t ret=0;
    if(fileheader!=NULL)
    {
        ret=fileheader->f_nscns;
    }
    return ret;
}

bool hcoff_fileheader_is_relocatable_object_file(hcoff_fileheader_t *fileheader)
{
    if(fileheader!=NULL)
    {
        return (fileheader->f_flags&(HCOFF_FILEHEADER_F_FLAGS_F_EXEC | HCOFF_FILEHEADER_F_FLAGS_IMAGE_FILE_SYSTEM |HCOFF_FILEHEADER_F_FLAGS_IMAGE_FILE_DLL))==0;
    }
    return false;
}

void hcoff_file_input_init(hcoff_file_input_t *input,hcoff_file_input_read_t read,void *usr)
{
    if(input!=NULL)
    {
        input->read=read;
        input->usr=usr;
    }
}

size_t hcoff_file_input_read(hcoff_file_input_t *input,uintptr_t address,void *buffer,size_t buffer_length)
{
    if(input==NULL || input->read==NULL)
    {
        return 0;
    }
    return input->read(input,address,buffer,buffer_length);
}

bool hcoff_sectionheader_read(hcoff_sectionheader_t *sectionheader,size_t index,hcoff_file_input_t *input_file)
{
    bool ret=false;
    if(sectionheader==NULL)
    {
        return ret;
    }
    bool is_big_endian=false;
    uintptr_t section_offset=0;
    size_t section_count=0;
    {
        hcoff_fileheader_t filehdr;
        uint8_t buffer[sizeof(hcoff_fileheader_t)]= {0};
        if(sizeof(buffer) > hcoff_file_input_read(input_file,0,buffer,sizeof(buffer)))
        {
            return ret;
        }
        if(!hcoff_fileheader_read(&filehdr,buffer,sizeof(buffer)))
        {
            return ret;
        }
        is_big_endian=(filehdr.f_magic&0xFF != buffer[0]);
        section_offset=hcoff_fileheader_section_offset_get(&filehdr);
        section_count=hcoff_fileheader_section_count_get(&filehdr);
    }

    if(index >= section_count || section_offset == 0)
    {
        return ret;
    }

    hcoff_sectionheader_bytes_t sectionheader_bytes= {0};

    if(sizeof(sectionheader_bytes) > hcoff_file_input_read(input_file,section_offset+index*sizeof(sectionheader_bytes),&sectionheader_bytes,sizeof(sectionheader_bytes)))
    {
        return ret;
    }

    ret=true;

    if(is_big_endian)
    {
#ifdef HCOFF_COMMON_GETUINT32
#undef HCOFF_COMMON_GETUINT32
#endif // HCOFF_COMMON_GETUINT32
#define HCOFF_COMMON_GETUINT32(v,a) v=a[0]*(1ULL << 24) + a[1]*(1ULL << 16)+a[2]*(1ULL << 8) + a[3]*(1ULL << 0)
#ifdef HCOFF_COMMON_GETUINT16
#undef HCOFF_COMMON_GETUINT16
#endif // HCOFF_COMMON_GETUINT16
#define HCOFF_COMMON_GETUINT16(v,a) v=a[0]*(1ULL << 8) + a[1]*(1ULL << 0)

        memcpy(sectionheader->s_name,sectionheader_bytes.s_name,sizeof(sectionheader->s_name));
        HCOFF_COMMON_GETUINT32(sectionheader->s_paddr,sectionheader_bytes.s_paddr);
        HCOFF_COMMON_GETUINT32(sectionheader->s_vaddr,sectionheader_bytes.s_vaddr);
        HCOFF_COMMON_GETUINT32(sectionheader->s_size,sectionheader_bytes.s_size);
        HCOFF_COMMON_GETUINT32(sectionheader->s_scnptr,sectionheader_bytes.s_scnptr);
        HCOFF_COMMON_GETUINT32(sectionheader->s_relptr,sectionheader_bytes.s_relptr);
        HCOFF_COMMON_GETUINT32(sectionheader->s_lnnoptr,sectionheader_bytes.s_lnnoptr);
        HCOFF_COMMON_GETUINT16(sectionheader->s_nreloc,sectionheader_bytes.s_nreloc);
        HCOFF_COMMON_GETUINT16(sectionheader->s_nlnno,sectionheader_bytes.s_nlnno);
        HCOFF_COMMON_GETUINT32(sectionheader->s_flags,sectionheader_bytes.s_flags);


#undef HCOFF_COMMON_GETUINT32
#undef HCOFF_COMMON_GETUINT16
    }
    else
    {

#ifdef HCOFF_COMMON_GETUINT32
#undef HCOFF_COMMON_GETUINT32
#endif // HCOFF_COMMON_GETUINT32
#define HCOFF_COMMON_GETUINT32(v,a) v=a[0]*(1ULL << 0) + a[1]*(1ULL << 8)+a[2]*(1ULL << 16) + a[3]*(1ULL << 24)
#ifdef HCOFF_COMMON_GETUINT16
#undef HCOFF_COMMON_GETUINT16
#endif // HCOFF_COMMON_GETUINT16
#define HCOFF_COMMON_GETUINT16(v,a) v=a[0]*(1ULL << 0) + a[1]*(1ULL << 8)

        memcpy(sectionheader->s_name,sectionheader_bytes.s_name,sizeof(sectionheader->s_name));
        HCOFF_COMMON_GETUINT32(sectionheader->s_paddr,sectionheader_bytes.s_paddr);
        HCOFF_COMMON_GETUINT32(sectionheader->s_vaddr,sectionheader_bytes.s_vaddr);
        HCOFF_COMMON_GETUINT32(sectionheader->s_size,sectionheader_bytes.s_size);
        HCOFF_COMMON_GETUINT32(sectionheader->s_scnptr,sectionheader_bytes.s_scnptr);
        HCOFF_COMMON_GETUINT32(sectionheader->s_relptr,sectionheader_bytes.s_relptr);
        HCOFF_COMMON_GETUINT32(sectionheader->s_lnnoptr,sectionheader_bytes.s_lnnoptr);
        HCOFF_COMMON_GETUINT16(sectionheader->s_nreloc,sectionheader_bytes.s_nreloc);
        HCOFF_COMMON_GETUINT16(sectionheader->s_nlnno,sectionheader_bytes.s_nlnno);
        HCOFF_COMMON_GETUINT32(sectionheader->s_flags,sectionheader_bytes.s_flags);


#undef HCOFF_COMMON_GETUINT32
#undef HCOFF_COMMON_GETUINT16

    }

    return ret;
}
