/***************************************************************
 * Name:      hcoff_common.c
 * Purpose:   实现hcoff_common接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-15
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hcoff_common.h"
#include "stdlib.h"
#include "string.h"

const char *hcoff_machine_name(uint16_t magic)
{
    const char *ret="unknown";
    switch(magic)
    {
    case HCOFF_FILEHEADER_F_MAGIC_UNKNOWN:
        ret="unknown";
        break;
    case HCOFF_FILEHEADER_F_MAGIC_ALPHA:
        ret="alpha";
        break;
    case HCOFF_FILEHEADER_F_MAGIC_ALPHA64:
        ret="alpha64";
        break;
    case HCOFF_FILEHEADER_F_MAGIC_AM33:
        ret="am33";
        break;
    case HCOFF_FILEHEADER_F_MAGIC_AMD64:
        ret="amd64";
        break;
    case HCOFF_FILEHEADER_F_MAGIC_ARM:
        ret="arm";
        break;
    case HCOFF_FILEHEADER_F_MAGIC_ARM64:
        ret="arm64";
        break;
    case HCOFF_FILEHEADER_F_MAGIC_ARMNT:
        ret="armnt";
        break;
    case HCOFF_FILEHEADER_F_MAGIC_CEE:
        ret="cee";
        break;
    case HCOFF_FILEHEADER_F_MAGIC_CEF:
        ret="cef";
        break;
    case HCOFF_FILEHEADER_F_MAGIC_EBC:
        ret="ebc";
        break;
    case HCOFF_FILEHEADER_F_MAGIC_I386:
        ret="i386";
        break;
    case HCOFF_FILEHEADER_F_MAGIC_IA64:
        ret="ia64";
        break;
    case HCOFF_FILEHEADER_F_MAGIC_LOONGARCH64:
        ret="loongarch64";
        break;
    case HCOFF_FILEHEADER_F_MAGIC_M32R:
        ret="m32r";
        break;
    case HCOFF_FILEHEADER_F_MAGIC_M68K:
        ret="m68k";
        break;
    case HCOFF_FILEHEADER_F_MAGIC_MIPS16:
        ret="mips16";
        break;
    case HCOFF_FILEHEADER_F_MAGIC_MIPSFPU:
        ret="mipsfpu";
        break;
    case HCOFF_FILEHEADER_F_MAGIC_MIPSFPU16:
        ret="mipsfpu16";
        break;
    case HCOFF_FILEHEADER_F_MAGIC_POWERPC:
        ret="powerpc";
        break;
    case HCOFF_FILEHEADER_F_MAGIC_POWERPCFP:
        ret="powerpcfp";
        break;
    case HCOFF_FILEHEADER_F_MAGIC_R10000:
        ret="r10000";
        break;
    case HCOFF_FILEHEADER_F_MAGIC_R3000:
        ret="r3000";
        break;
    case HCOFF_FILEHEADER_F_MAGIC_R4000:
        ret="r4000";
        break;
    case HCOFF_FILEHEADER_F_MAGIC_RISCV32:
        ret="riscv32";
        break;
    case HCOFF_FILEHEADER_F_MAGIC_RISCV64:
        ret="riscv64";
        break;
    case HCOFF_FILEHEADER_F_MAGIC_SH3:
        ret="sh3";
        break;
    case HCOFF_FILEHEADER_F_MAGIC_SH3DSP:
        ret="sh3dsp";
        break;
    case HCOFF_FILEHEADER_F_MAGIC_SH3E:
        ret="sh3e";
        break;
    case HCOFF_FILEHEADER_F_MAGIC_SH4:
        ret="sh4";
        break;
    case HCOFF_FILEHEADER_F_MAGIC_SH5:
        ret="sh5";
        break;
    case HCOFF_FILEHEADER_F_MAGIC_THUMB:
        ret="thumb";
        break;
    case HCOFF_FILEHEADER_F_MAGIC_TRICORE:
        ret="tricore";
        break;
    case HCOFF_FILEHEADER_F_MAGIC_WCEMIPSV2:
        ret="wcemipsv2";
        break;
    default:
        break;
    }
    return ret;
}

bool hcoff_machine_is_64bit(uint16_t magic)
{
    bool ret=false;
    switch(magic)
    {
    case HCOFF_FILEHEADER_F_MAGIC_ALPHA64:
    case HCOFF_FILEHEADER_F_MAGIC_AMD64:
    case HCOFF_FILEHEADER_F_MAGIC_ARM64:
    case HCOFF_FILEHEADER_F_MAGIC_IA64:
    case HCOFF_FILEHEADER_F_MAGIC_LOONGARCH64:
    case HCOFF_FILEHEADER_F_MAGIC_RISCV64:
    case HCOFF_FILEHEADER_F_MAGIC_SH5:
        ret=true;
        break;
    default:
        break;
    }
    return ret;
}

size_t hcoff_machine_relocation_entry_size(uint16_t magic)
{
    size_t ret=10;
    switch(magic)
    {
    case HCOFF_FILEHEADER_F_MAGIC_ALPHA:
    case HCOFF_FILEHEADER_F_MAGIC_ALPHA64:
        ret=16;
        break;
    case HCOFF_FILEHEADER_F_MAGIC_LOONGARCH64:
    case HCOFF_FILEHEADER_F_MAGIC_RISCV32:
    case HCOFF_FILEHEADER_F_MAGIC_RISCV64:
        //PE COFF使用r_vaddr+r_symndx+r_type+r_offset布局
        ret=14;
        break;
    case HCOFF_FILEHEADER_F_MAGIC_MIPS16:
    case HCOFF_FILEHEADER_F_MAGIC_MIPSFPU:
    case HCOFF_FILEHEADER_F_MAGIC_MIPSFPU16:
    case HCOFF_FILEHEADER_F_MAGIC_R3000:
    case HCOFF_FILEHEADER_F_MAGIC_R4000:
    case HCOFF_FILEHEADER_F_MAGIC_R10000:
    case HCOFF_FILEHEADER_F_MAGIC_WCEMIPSV2:
        //MIPS ECOFF使用8字节压缩格式
        ret=8;
        break;
    case HCOFF_FILEHEADER_F_MAGIC_SH3:
    case HCOFF_FILEHEADER_F_MAGIC_SH3DSP:
    case HCOFF_FILEHEADER_F_MAGIC_SH3E:
    case HCOFF_FILEHEADER_F_MAGIC_SH4:
        //非PE的SH COFF使用16字节重定位入口
        ret=16;
        break;
    default:
        ret=10;
        break;
    }
    return ret;
}

uint16_t hcoff_get_uint16_le(const void *buffer)
{
    const uint8_t *p=(const uint8_t *)buffer;
    return (uint16_t)(p[0] | ((uint16_t)p[1] << 8));
}

uint16_t hcoff_get_uint16_be(const void *buffer)
{
    const uint8_t *p=(const uint8_t *)buffer;
    return (uint16_t)(((uint16_t)p[0] << 8) | p[1]);
}

uint32_t hcoff_get_uint32_le(const void *buffer)
{
    const uint8_t *p=(const uint8_t *)buffer;
    return (uint32_t)p[0] | ((uint32_t)p[1] << 8) | ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
}

uint32_t hcoff_get_uint32_be(const void *buffer)
{
    const uint8_t *p=(const uint8_t *)buffer;
    return ((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16) | ((uint32_t)p[2] << 8) | (uint32_t)p[3];
}

uint64_t hcoff_get_uint64_le(const void *buffer)
{
    const uint8_t *p=(const uint8_t *)buffer;
    return ((uint64_t)hcoff_get_uint32_le(p)) | (((uint64_t)hcoff_get_uint32_le(p+4)) << 32);
}

uint64_t hcoff_get_uint64_be(const void *buffer)
{
    const uint8_t *p=(const uint8_t *)buffer;
    return (((uint64_t)hcoff_get_uint32_be(p)) << 32) | ((uint64_t)hcoff_get_uint32_be(p+4));
}

uint16_t hcoff_get_uint16(const void *buffer,bool is_big_endian)
{
    return is_big_endian?hcoff_get_uint16_be(buffer):hcoff_get_uint16_le(buffer);
}

uint32_t hcoff_get_uint32(const void *buffer,bool is_big_endian)
{
    return is_big_endian?hcoff_get_uint32_be(buffer):hcoff_get_uint32_le(buffer);
}

uint64_t hcoff_get_uint64(const void *buffer,bool is_big_endian)
{
    return is_big_endian?hcoff_get_uint64_be(buffer):hcoff_get_uint64_le(buffer);
}

void hcoff_put_uint16_le(void *buffer,uint16_t value)
{
    uint8_t *p=(uint8_t *)buffer;
    p[0]=(uint8_t)(value & 0xFF);
    p[1]=(uint8_t)((value >> 8) & 0xFF);
}

void hcoff_put_uint32_le(void *buffer,uint32_t value)
{
    uint8_t *p=(uint8_t *)buffer;
    p[0]=(uint8_t)(value & 0xFF);
    p[1]=(uint8_t)((value >> 8) & 0xFF);
    p[2]=(uint8_t)((value >> 16) & 0xFF);
    p[3]=(uint8_t)((value >> 24) & 0xFF);
}

void hcoff_put_uint64_le(void *buffer,uint64_t value)
{
    uint8_t *p=(uint8_t *)buffer;
    hcoff_put_uint32_le(p,(uint32_t)(value & 0xFFFFFFFF));
    hcoff_put_uint32_le(p+4,(uint32_t)((value >> 32) & 0xFFFFFFFF));
}

void hcoff_put_uint16(void *buffer,uint16_t value,bool is_big_endian)
{
    if(is_big_endian)
    {
        uint8_t *p=(uint8_t *)buffer;
        p[0]=(uint8_t)((value >> 8) & 0xFF);
        p[1]=(uint8_t)(value & 0xFF);
    }
    else
    {
        hcoff_put_uint16_le(buffer,value);
    }
}

void hcoff_put_uint32(void *buffer,uint32_t value,bool is_big_endian)
{
    if(is_big_endian)
    {
        uint8_t *p=(uint8_t *)buffer;
        p[0]=(uint8_t)((value >> 24) & 0xFF);
        p[1]=(uint8_t)((value >> 16) & 0xFF);
        p[2]=(uint8_t)((value >> 8) & 0xFF);
        p[3]=(uint8_t)(value & 0xFF);
    }
    else
    {
        hcoff_put_uint32_le(buffer,value);
    }
}

void hcoff_put_uint64(void *buffer,uint64_t value,bool is_big_endian)
{
    if(is_big_endian)
    {
        uint8_t *p=(uint8_t *)buffer;
        hcoff_put_uint32(p,(uint32_t)((value >> 32) & 0xFFFFFFFF),true);
        hcoff_put_uint32(p+4,(uint32_t)(value & 0xFFFFFFFF),true);
    }
    else
    {
        hcoff_put_uint64_le(buffer,value);
    }
}

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

typedef struct
{
    unsigned char r_vaddr[4];         /* Virtual address of reference */
    unsigned char r_symndx[4];        /* Index into symbol table	*/
    unsigned char r_type[2];          /* Relocation type		*/
} hcoff_section_relocation_bytes_t;

typedef struct
{
    union
    {
        unsigned char e_name[8];

        struct
        {
            unsigned char e_zeroes[4];
            unsigned char e_offset[4];
        } e;
    } e;

    unsigned char e_value[4];
    unsigned char e_scnum[2];
    unsigned char e_type[2];
    unsigned char e_sclass[1];
    unsigned char e_numaux[1];
} hcoff_symbol_entry_bytes_t;

/*
 * Aux大小与symbol一致，均存储在符号表中
 */
typedef union
{
    /* Aux sym format 4: file.  */
    union
    {
        char x_fname[18];
        struct
        {
            unsigned char x_zeroes[4];
            unsigned char x_offset[4];
        } x_n;
    } x_file;
    /* Aux sym format 5: section.  */
    struct
    {
        unsigned char x_scnlen[4];		/* section length		*/
        unsigned char x_nreloc[2];		/* # relocation entries		*/
        unsigned char x_nlinno[2];		/* # line numbers		*/
        unsigned char x_checksum[4];	/* section COMDAT checksum	*/
        unsigned char x_associated[2];	/* COMDAT assoc section index	*/
        unsigned char x_comdat[1];		/* COMDAT selection number	*/
    } x_scn;
} hcoff_symbol_aux_entry_bytes_t;


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

bool hcoff_fileheader_read_at(hcoff_fileheader_t *fileheader,hcoff_file_input_t *input_file,uintptr_t offset)
{
    bool ret=false;
    if(fileheader==NULL || input_file==NULL)
    {
        return ret;
    }
    uint8_t buffer[sizeof(hcoff_fileheader_t)]= {0};
    if(sizeof(buffer) > hcoff_file_input_read(input_file,offset,buffer,sizeof(buffer)))
    {
        return ret;
    }
    ret=hcoff_fileheader_read(fileheader,buffer,sizeof(buffer));
    return ret;
}

static bool hcoff_file_input_fileheader_read(hcoff_file_input_t *input_file,hcoff_fileheader_t *fileheader)
{
    if(input_file==NULL || fileheader==NULL)
    {
        return false;
    }
    //常规COFF文件
    if(hcoff_fileheader_read_at(fileheader,input_file,0))
    {
        return true;
    }
    //PE文件:DOS头+"PE\0\0"+COFF文件头
    uint8_t dos_header[64]= {0};
    if(sizeof(dos_header) > hcoff_file_input_read(input_file,0,dos_header,sizeof(dos_header)))
    {
        return false;
    }
    if(hcoff_get_uint16_le(dos_header) != 0x5A4D)
    {
        return false;
    }
    uint32_t nt_offset=hcoff_get_uint32_le(dos_header+0x3C);
    if(nt_offset < 0x40)
    {
        return false;
    }
    uint8_t signature[4]= {0};
    if(sizeof(signature) > hcoff_file_input_read(input_file,nt_offset,signature,sizeof(signature)))
    {
        return false;
    }
    if(hcoff_get_uint32_le(signature) != 0x00004550)
    {
        return false;
    }
    return hcoff_fileheader_read_at(fileheader,input_file,nt_offset+4);
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
        input->size=0;
        input->size_valid=false;
    }
}

void hcoff_file_input_init_with_size(hcoff_file_input_t *input,hcoff_file_input_read_t read,void *usr,size_t size)
{
    if(input!=NULL)
    {
        input->read=read;
        input->usr=usr;
        input->size=size;
        input->size_valid=true;
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

bool hcoff_file_input_is_big_endian(hcoff_file_input_t *input_file)
{
    bool ret=false;
    {
        hcoff_fileheader_t filehdr= {0};
        uint8_t buffer[2]= {0};
        if(!hcoff_file_input_fileheader_read(input_file,&filehdr))
        {
            return ret;
        }
        if(sizeof(buffer) > hcoff_file_input_read(input_file,0,buffer,sizeof(buffer)))
        {
            return ret;
        }
        if(buffer[0]=='M' && buffer[1]=='Z')
        {
            //PE文件固定为小端模式
            return false;
        }
        ret=((filehdr.f_magic&0xFF) != buffer[0]);
    }
    return ret;
}

bool hcoff_sectionheader_read_at(hcoff_sectionheader_t *sectionheader,size_t index,uintptr_t section_table_offset,hcoff_file_input_t *input_file)
{
    bool ret=false;
    if(sectionheader==NULL || input_file==NULL || section_table_offset == 0)
    {
        return ret;
    }
    bool is_big_endian=false;
    {
        hcoff_fileheader_t filehdr;
        uint8_t buffer[sizeof(hcoff_fileheader_t)]= {0};
        if(sizeof(buffer) <= hcoff_file_input_read(input_file,0,buffer,sizeof(buffer)))
        {
            if(hcoff_fileheader_read(&filehdr,buffer,sizeof(buffer)))
            {
                is_big_endian=((filehdr.f_magic&0xFF) != buffer[0]);
            }
            else
            {
                //非常规COFF(如PE文件),默认采用小端模式
                is_big_endian=false;
            }
        }
    }

    hcoff_sectionheader_bytes_t sectionheader_bytes= {0};

    if(sizeof(sectionheader_bytes) > hcoff_file_input_read(input_file,section_table_offset+index*sizeof(sectionheader_bytes),&sectionheader_bytes,sizeof(sectionheader_bytes)))
    {
        return ret;
    }

    ret=true;
    memcpy(sectionheader->s_name,sectionheader_bytes.s_name,sizeof(sectionheader->s_name));
    sectionheader->s_paddr=hcoff_get_uint32(sectionheader_bytes.s_paddr,is_big_endian);
    sectionheader->s_vaddr=hcoff_get_uint32(sectionheader_bytes.s_vaddr,is_big_endian);
    sectionheader->s_size=hcoff_get_uint32(sectionheader_bytes.s_size,is_big_endian);
    sectionheader->s_scnptr=hcoff_get_uint32(sectionheader_bytes.s_scnptr,is_big_endian);
    sectionheader->s_relptr=hcoff_get_uint32(sectionheader_bytes.s_relptr,is_big_endian);
    sectionheader->s_lnnoptr=hcoff_get_uint32(sectionheader_bytes.s_lnnoptr,is_big_endian);
    sectionheader->s_nreloc=hcoff_get_uint16(sectionheader_bytes.s_nreloc,is_big_endian);
    sectionheader->s_nlnno=hcoff_get_uint16(sectionheader_bytes.s_nlnno,is_big_endian);
    sectionheader->s_flags=hcoff_get_uint32(sectionheader_bytes.s_flags,is_big_endian);
    return ret;
}

bool hcoff_sectionheader_read(hcoff_sectionheader_t *sectionheader,size_t index,hcoff_file_input_t *input_file)
{
    bool ret=false;
    if(sectionheader==NULL || input_file==NULL)
    {
        return ret;
    }
    uintptr_t section_offset=0;
    size_t section_count=0;
    {
        hcoff_fileheader_t filehdr= {0};
        if(!hcoff_file_input_fileheader_read(input_file,&filehdr))
        {
            return ret;
        }
        uintptr_t fileheader_offset=0;
        uint8_t dos_header[64]= {0};
        if(sizeof(dos_header) <= hcoff_file_input_read(input_file,0,dos_header,sizeof(dos_header)) &&
                hcoff_get_uint16_le(dos_header) == 0x5A4D &&
                hcoff_get_uint32_le(dos_header+0x3C) >= 0x40)
        {
            //PE文件,节表位于NT头+COFF文件头+可选头之后
            fileheader_offset=hcoff_get_uint32_le(dos_header+0x3C)+4;
        }
        section_offset=fileheader_offset+sizeof(hcoff_fileheader_bytes_t)+filehdr.f_opthdr;
        section_count=hcoff_fileheader_section_count_get(&filehdr);
    }

    if(index >= section_count || section_offset == 0)
    {
        return ret;
    }
    return hcoff_sectionheader_read_at(sectionheader,index,section_offset,input_file);
}

const char *hcoff_sectionheader_name_read(const hcoff_sectionheader_t *sectionheader,hcoff_file_input_t *input_file,void *namebuf,size_t namebulen)
{
    if(sectionheader==NULL)
    {
        return NULL;
    }
    if(sectionheader->s_name[0]!='/')
    {
        if(namebuf==NULL || namebulen <= sizeof(sectionheader->s_name))
        {
            return NULL;
        }
        memcpy(namebuf,sectionheader->s_name,sizeof(sectionheader->s_name));
        ((uint8_t *)namebuf)[sizeof(sectionheader->s_name)]='\0';
        return (const char *)namebuf;
    }
    else
    {
        size_t str_index=0;
        {
            char str_index_str[8]= {0};
            memcpy(str_index_str,&sectionheader->s_name[1],sizeof(sectionheader->s_name)-1);
            str_index=strtoull(str_index_str,NULL,10);
        }
        if(str_index < 4 )
        {
            return NULL;
        }
        uintptr_t strtab_offset=0;
        size_t strtab_size=0;
        {
            hcoff_fileheader_t filehdr= {0};
            if(!hcoff_file_input_fileheader_read(input_file,&filehdr))
            {
                return NULL;
            }
            if(filehdr.f_symptr==0)
            {
                //PE映像可能不包含符号表,此时无法解析长名称
                return NULL;
            }
            strtab_offset=filehdr.f_symptr+filehdr.f_nsyms*sizeof(hcoff_symbol_entry_bytes_t);
            uint8_t strtab_size_buffer[4]= {0};
            if(sizeof(strtab_size_buffer) > hcoff_file_input_read(input_file,strtab_offset,strtab_size_buffer,sizeof(strtab_size_buffer)))
            {
                return NULL;
            }
            strtab_size=hcoff_get_uint32(strtab_size_buffer,hcoff_file_input_is_big_endian(input_file));
        }
        if(str_index >= strtab_size || namebuf==NULL || namebulen < 2)
        {
            return NULL;
        }
        if(str_index+namebulen > strtab_size)
        {
            namebulen=strtab_size-str_index;
        }
        if(0 < hcoff_file_input_read(input_file,strtab_offset+str_index,namebuf,namebulen-1))
        {
            ((uint8_t *)namebuf)[namebulen-1]='\0';
            return (const char *)namebuf;
        }
    }
    return NULL;
}

bool hcoff_sectionheader_name_match(const char *section_name,const hcoff_sectionheader_t *sectionheader,hcoff_file_input_t *input_file)
{
    bool ret=false;
    if(section_name==NULL || strlen(section_name)==0 || sectionheader == NULL)
    {
        return ret;
    }
    char namebuff[64]= {0};
    if(NULL==hcoff_sectionheader_name_read(sectionheader,input_file,namebuff,sizeof(namebuff)))
    {
        return ret;
    }
    size_t namelen=strlen(section_name);
    if(namelen > sizeof(namebuff))
    {
        return ret;
    }
    if(namebuff[namelen]=='$' || namebuff[namelen]=='.')
    {
        namebuff[namelen]='\0';
    }
    ret=(0==strcmp(section_name,namebuff));
    return ret;
}

size_t hcoff_section_relocation_count_get(const hcoff_sectionheader_t *sectionheader,hcoff_file_input_t *input_file)
{
    size_t ret=0;
    if(sectionheader==NULL || input_file==NULL)
    {
        return ret;
    }
    ret=sectionheader->s_nreloc;
    if(sectionheader->s_nreloc == 0xFFFF && (sectionheader->s_flags & HCOFF_SECTIONHEADER_S_FLAGS_IMAGE_SCN_LNK_NRELOC_OVFL)!=0)
    {
        //扩展重定位:第一个重定位入口的r_vaddr存储实际数量
        bool is_big_endian=hcoff_file_input_is_big_endian(input_file);
        uint8_t buffer[4]= {0};
        if(4 <= hcoff_file_input_read(input_file,sectionheader->s_relptr,buffer,sizeof(buffer)))
        {
            uint32_t overflow_count=hcoff_get_uint32(buffer,is_big_endian);
            //实际数量需要减去存储数量的第一个重定位入口
            ret=(overflow_count > 0)?(size_t)(overflow_count-1):0;
        }
    }
    return ret;
}

bool hcoff_section_relocation_read(hcoff_section_relocation_t *relocation,size_t index,const hcoff_sectionheader_t *sectionheader,hcoff_file_input_t *input_file)
{
    bool ret=false;
    if(relocation==NULL || sectionheader == NULL || input_file==NULL)
    {
        return ret;
    }
    size_t relocation_count=hcoff_section_relocation_count_get(sectionheader,input_file);
    if(index >= relocation_count)
    {
        return ret;
    }
    bool is_big_endian=false;
    uint16_t machine=HCOFF_FILEHEADER_F_MAGIC_UNKNOWN;
    {
        hcoff_fileheader_t filehdr= {0};
        if(!hcoff_file_input_fileheader_read(input_file,&filehdr))
        {
            return ret;
        }
        is_big_endian=hcoff_file_input_is_big_endian(input_file);
        machine=filehdr.f_magic;
    }

    size_t entry_size=hcoff_machine_relocation_entry_size(machine);
    size_t entry_index=index;
    if(sectionheader->s_nreloc == 0xFFFF && (sectionheader->s_flags & HCOFF_SECTIONHEADER_S_FLAGS_IMAGE_SCN_LNK_NRELOC_OVFL)!=0)
    {
        //跳过扩展重定位的数量入口
        entry_index++;
    }
    uint8_t relocation_bytes[16]= {0};
    if(entry_size > sizeof(relocation_bytes) || entry_size > hcoff_file_input_read(input_file,sectionheader->s_relptr+entry_index*entry_size,relocation_bytes,entry_size))
    {
        return ret;
    }
    ret=true;
    switch(machine)
    {
    case HCOFF_FILEHEADER_F_MAGIC_LOONGARCH64:
    case HCOFF_FILEHEADER_F_MAGIC_RISCV32:
    case HCOFF_FILEHEADER_F_MAGIC_RISCV64:
    {
        //r_vaddr(4)+r_symndx(4)+r_type(2)+r_offset(4)
        relocation->r_vaddr=hcoff_get_uint32(relocation_bytes,is_big_endian);
        relocation->r_symndx=hcoff_get_uint32(relocation_bytes+4,is_big_endian);
        relocation->r_type=hcoff_get_uint16(relocation_bytes+8,is_big_endian);
    }
    break;
    case HCOFF_FILEHEADER_F_MAGIC_SH3:
    case HCOFF_FILEHEADER_F_MAGIC_SH3DSP:
    case HCOFF_FILEHEADER_F_MAGIC_SH3E:
    case HCOFF_FILEHEADER_F_MAGIC_SH4:
    {
        //r_vaddr(4)+r_symndx(4)+r_offset(4)+r_type(2)+r_stuff(2)
        relocation->r_vaddr=hcoff_get_uint32(relocation_bytes,is_big_endian);
        relocation->r_symndx=hcoff_get_uint32(relocation_bytes+4,is_big_endian);
        relocation->r_type=hcoff_get_uint16(relocation_bytes+12,is_big_endian);
    }
    break;
    default:
    {
        //r_vaddr(4)+r_symndx(4)+r_type(2)
        relocation->r_vaddr=hcoff_get_uint32(relocation_bytes,is_big_endian);
        relocation->r_symndx=hcoff_get_uint32(relocation_bytes+4,is_big_endian);
        relocation->r_type=hcoff_get_uint16(relocation_bytes+8,is_big_endian);
    }
    break;
    }
    return ret;
}

bool hcoff_symbol_is_symbol(size_t index,hcoff_file_input_t *input_file)
{
    bool ret=false;
    hcoff_fileheader_t filehdr= {0};
    if(!hcoff_file_input_fileheader_read(input_file,&filehdr))
    {
        return ret;
    }

    if(index >= filehdr.f_nsyms)
    {
        return ret;
    }

    {
        uintptr_t symboltable_offset=filehdr.f_symptr;
        for(size_t i=0; i<filehdr.f_nsyms; i++)
        {
            if(i==index)
            {
                ret=true;
                break;
            }
            hcoff_symbol_entry_bytes_t symbol_entry_bytes= {0};
            if(sizeof(hcoff_symbol_entry_bytes_t) > hcoff_file_input_read(input_file,symboltable_offset+i*sizeof(hcoff_symbol_entry_bytes_t),&symbol_entry_bytes,sizeof(symbol_entry_bytes)))
            {
                return ret;
            }

            //跳过辅助信息
            i+=symbol_entry_bytes.e_numaux[0];
        }
    }

    return ret;
}

bool hcoff_symbol_entry_read(hcoff_symbol_entry_t *symbol_entry,size_t index,hcoff_file_input_t *input_file)
{
    if(!hcoff_symbol_is_symbol(index,input_file))
    {
        return false;
    }
    return hcoff_symbol_entry_read_at(symbol_entry,index,input_file);
}

bool hcoff_symbol_entry_read_at(hcoff_symbol_entry_t *symbol_entry,size_t index,hcoff_file_input_t *input_file)
{
    bool ret=false;
    if(symbol_entry==NULL || input_file==NULL)
    {
        return ret;
    }
    bool is_big_endian=false;
    uintptr_t symboltable_offset=0;
    size_t symboltable_count=0;
    {
        hcoff_fileheader_t filehdr= {0};
        if(!hcoff_file_input_fileheader_read(input_file,&filehdr))
        {
            return ret;
        }
        is_big_endian=hcoff_file_input_is_big_endian(input_file);
        symboltable_offset=filehdr.f_symptr;
        symboltable_count=filehdr.f_nsyms;
    }

    if(index >= symboltable_count)
    {
        return ret;
    }

    hcoff_symbol_entry_bytes_t symbol_entry_bytes= {0};
    if(sizeof(hcoff_symbol_entry_bytes_t) > hcoff_file_input_read(input_file,symboltable_offset+index*sizeof(hcoff_symbol_entry_bytes_t),&symbol_entry_bytes,sizeof(symbol_entry_bytes)))
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

        if(symbol_entry_bytes.e.e_name[0]=='\0')
        {
            //符号存储在字符串表中
            HCOFF_COMMON_GETUINT32(symbol_entry->e.e.e_zeroes,symbol_entry_bytes.e.e.e_zeroes);
            HCOFF_COMMON_GETUINT32(symbol_entry->e.e.e_offset,symbol_entry_bytes.e.e.e_offset);
        }
        else
        {
            //字符串存储在e_name中
            memcpy(symbol_entry->e.e_name,symbol_entry_bytes.e.e_name,sizeof(symbol_entry->e.e_name));
        }

        HCOFF_COMMON_GETUINT32(symbol_entry->e_value,symbol_entry_bytes.e_value);
        HCOFF_COMMON_GETUINT16(symbol_entry->e_scnum,symbol_entry_bytes.e_scnum);
        HCOFF_COMMON_GETUINT16(symbol_entry->e_type,symbol_entry_bytes.e_type);
        symbol_entry->e_sclass=symbol_entry_bytes.e_sclass[0];
        symbol_entry->e_numaux=symbol_entry_bytes.e_numaux[0];

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

        if(symbol_entry_bytes.e.e_name[0]=='\0')
        {
            //符号存储在字符串表中
            HCOFF_COMMON_GETUINT32(symbol_entry->e.e.e_zeroes,symbol_entry_bytes.e.e.e_zeroes);
            HCOFF_COMMON_GETUINT32(symbol_entry->e.e.e_offset,symbol_entry_bytes.e.e.e_offset);
        }
        else
        {
            //字符串存储在e_name中
            memcpy(symbol_entry->e.e_name,symbol_entry_bytes.e.e_name,sizeof(symbol_entry->e.e_name));
        }

        HCOFF_COMMON_GETUINT32(symbol_entry->e_value,symbol_entry_bytes.e_value);
        HCOFF_COMMON_GETUINT16(symbol_entry->e_scnum,symbol_entry_bytes.e_scnum);
        HCOFF_COMMON_GETUINT16(symbol_entry->e_type,symbol_entry_bytes.e_type);
        symbol_entry->e_sclass=symbol_entry_bytes.e_sclass[0];
        symbol_entry->e_numaux=symbol_entry_bytes.e_numaux[0];

#undef HCOFF_COMMON_GETUINT32
#undef HCOFF_COMMON_GETUINT16

    }

    return ret;
}

const char *hcoff_symbol_entry_name_read(const hcoff_symbol_entry_t *symbol_entry,hcoff_file_input_t *input_file,void *namebuf,size_t namebulen)
{
    if(symbol_entry==NULL)
    {
        return NULL;
    }
    if(symbol_entry->e.e_name[0]!='\0')
    {
        if(namebuf==NULL || namebulen <= sizeof(symbol_entry->e.e_name))
        {
            return NULL;
        }
        memcpy(namebuf,symbol_entry->e.e_name,sizeof(symbol_entry->e.e_name));
        ((uint8_t *)namebuf)[sizeof(symbol_entry->e.e_name)]='\0';
        return (const char *)namebuf;
    }
    else
    {
        size_t str_index=symbol_entry->e.e.e_offset;
        if(str_index < 4 )
        {
            return NULL;
        }
        uintptr_t strtab_offset=0;
        size_t strtab_size=0;
        {
            hcoff_fileheader_t filehdr= {0};
            if(!hcoff_file_input_fileheader_read(input_file,&filehdr))
            {
                return NULL;
            }
            if(filehdr.f_symptr==0)
            {
                //PE映像可能不包含符号表,此时无法解析长名称
                return NULL;
            }
            strtab_offset=filehdr.f_symptr+filehdr.f_nsyms*sizeof(hcoff_symbol_entry_bytes_t);
            uint8_t strtab_size_buffer[4]= {0};
            if(sizeof(strtab_size_buffer) > hcoff_file_input_read(input_file,strtab_offset,strtab_size_buffer,sizeof(strtab_size_buffer)))
            {
                return NULL;
            }
            strtab_size=hcoff_get_uint32(strtab_size_buffer,hcoff_file_input_is_big_endian(input_file));
        }
        if(str_index >= strtab_size || namebuf==NULL || namebulen < 2)
        {
            return NULL;
        }
        if(str_index+namebulen > strtab_size)
        {
            namebulen=strtab_size-str_index;
        }
        if(0 < hcoff_file_input_read(input_file,strtab_offset+str_index,namebuf,namebulen-1))
        {
            ((uint8_t *)namebuf)[namebulen-1]='\0';
            return (const char *)namebuf;
        }
    }
    return NULL;
}
