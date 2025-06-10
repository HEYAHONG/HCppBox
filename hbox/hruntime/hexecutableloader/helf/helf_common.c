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


bool helf_file_input_is_32_bits(helf_file_input_t *input_file)
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
    ret=(HELF_ELFCLASS_32==helf_elfclass_get(ident));
    return ret;
}

typedef struct
{
    uint8_t e_ident[16];        /* ELF "magic number" */
    uint8_t e_type[2];          /* Identifies object file type */
    uint8_t e_machine[2];       /* Specifies required architecture */
    uint8_t e_version[4];       /* Identifies object file version */
    uint8_t e_entry[4];         /* Entry point virtual address */
    uint8_t e_phoff[4];         /* Program header table file offset */
    uint8_t e_shoff[4];         /* Section header table file offset */
    uint8_t e_flags[4];         /* Processor-specific flags */
    uint8_t e_ehsize[2];        /* ELF header size in bytes */
    uint8_t e_phentsize[2];     /* Program header table entry size */
    uint8_t e_phnum[2];         /* Program header table entry count */
    uint8_t e_shentsize[2];     /* Section header table entry size */
    uint8_t e_shnum[2];         /* Section header table entry count */
    uint8_t e_shstrndx[2];      /* Section header string table index */
} helf_elf32_file_header_bytes_t;


bool helf_file_input_32_bits_header_get(helf_file_input_t *input_file,helf_elf32_file_header_t *header)
{
    bool ret=false;
    if(header==NULL)
    {
        return ret;
    }
    if(!helf_file_input_is_32_bits(input_file))
    {
        return ret;
    }
    helf_elf32_file_header_bytes_t header_bytes= {0};
    if(helf_file_input_read(input_file,0,&header_bytes,sizeof(header_bytes)) < sizeof(header_bytes))
    {
        return ret;
    }
    ret=true;
    memcpy(header->e_ident,header_bytes.e_ident,sizeof(header_bytes.e_ident));
    if(helf_file_input_is_big_endian(input_file))
    {
#ifdef HELF_READ_16
#undef HELF_READ_16
#endif // HELF_READ_16
#ifdef HELF_READ_32
#undef HELF_READ_32
#endif // HELF_READ_32
#define HELF_READ_16(v,a) v=a[0]*(1ULL << 8)+a[1]*(1ULL << 0)
#define HELF_READ_32(v,a) v=a[0]*(1ULL << 24)+a[1]*(1ULL << 16)+a[2]*(1ULL << 8)+a[3]*(1ULL << 0)

        HELF_READ_16(header->e_type,header_bytes.e_type);
        HELF_READ_16(header->e_machine,header_bytes.e_machine);
        HELF_READ_32(header->e_version,header_bytes.e_version);
        HELF_READ_32(header->e_entry,header_bytes.e_entry);
        HELF_READ_32(header->e_phoff,header_bytes.e_phoff);
        HELF_READ_32(header->e_shoff,header_bytes.e_shoff);
        HELF_READ_32(header->e_flags,header_bytes.e_flags);
        HELF_READ_16(header->e_ehsize,header_bytes.e_ehsize);
        HELF_READ_16(header->e_phentsize,header_bytes.e_phentsize);
        HELF_READ_16(header->e_phnum,header_bytes.e_phnum);
        HELF_READ_16(header->e_shentsize,header_bytes.e_shentsize);
        HELF_READ_16(header->e_shnum,header_bytes.e_shnum);
        HELF_READ_16(header->e_shstrndx,header_bytes.e_shstrndx);

#undef HELF_READ_16
#undef HELF_READ_32
    }
    else
    {
#ifdef HELF_READ_16
#undef HELF_READ_16
#endif // HELF_READ_16
#ifdef HELF_READ_32
#undef HELF_READ_32
#endif // HELF_READ_32
#define HELF_READ_16(v,a) v=a[1]*(1ULL << 8)+a[0]*(1ULL << 0)
#define HELF_READ_32(v,a) v=a[3]*(1ULL << 24)+a[2]*(1ULL << 16)+a[1]*(1ULL << 8)+a[0]*(1ULL << 0)

        HELF_READ_16(header->e_type,header_bytes.e_type);
        HELF_READ_16(header->e_machine,header_bytes.e_machine);
        HELF_READ_32(header->e_version,header_bytes.e_version);
        HELF_READ_32(header->e_entry,header_bytes.e_entry);
        HELF_READ_32(header->e_phoff,header_bytes.e_phoff);
        HELF_READ_32(header->e_shoff,header_bytes.e_shoff);
        HELF_READ_32(header->e_flags,header_bytes.e_flags);
        HELF_READ_16(header->e_ehsize,header_bytes.e_ehsize);
        HELF_READ_16(header->e_phentsize,header_bytes.e_phentsize);
        HELF_READ_16(header->e_phnum,header_bytes.e_phnum);
        HELF_READ_16(header->e_shentsize,header_bytes.e_shentsize);
        HELF_READ_16(header->e_shnum,header_bytes.e_shnum);
        HELF_READ_16(header->e_shstrndx,header_bytes.e_shstrndx);

#undef HELF_READ_16
#undef HELF_READ_32
    }
    return ret;
}


bool helf_file_input_is_64_bits(helf_file_input_t *input_file)
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
    ret=(HELF_ELFCLASS_64==helf_elfclass_get(ident));
    return ret;
}

typedef struct
{
    uint8_t e_ident[16];        /* ELF "magic number" */
    uint8_t e_type[2];          /* Identifies object file type */
    uint8_t e_machine[2];       /* Specifies required architecture */
    uint8_t e_version[4];       /* Identifies object file version */
    uint8_t e_entry[8];         /* Entry point virtual address */
    uint8_t e_phoff[8];         /* Program header table file offset */
    uint8_t e_shoff[8];         /* Section header table file offset */
    uint8_t e_flags[4];         /* Processor-specific flags */
    uint8_t e_ehsize[2];        /* ELF header size in bytes */
    uint8_t e_phentsize[2];     /* Program header table entry size */
    uint8_t e_phnum[2];         /* Program header table entry count */
    uint8_t e_shentsize[2];     /* Section header table entry size */
    uint8_t e_shnum[2];         /* Section header table entry count */
    uint8_t e_shstrndx[2];      /* Section header string table index */
} helf_elf64_file_header_bytes_t;

bool helf_file_input_64_bits_header_get(helf_file_input_t *input_file,helf_elf64_file_header_t *header)
{
    bool ret=false;
    if(header==NULL)
    {
        return ret;
    }
    if(!helf_file_input_is_64_bits(input_file))
    {
        return ret;
    }
    helf_elf64_file_header_bytes_t header_bytes= {0};
    if(helf_file_input_read(input_file,0,&header_bytes,sizeof(header_bytes)) < sizeof(header_bytes))
    {
        return ret;
    }
    ret=true;
    memcpy(header->e_ident,header_bytes.e_ident,sizeof(header_bytes.e_ident));
    if(helf_file_input_is_big_endian(input_file))
    {
#ifdef HELF_READ_16
#undef HELF_READ_16
#endif // HELF_READ_16
#ifdef HELF_READ_32
#undef HELF_READ_32
#endif // HELF_READ_32
#ifdef HELF_READ_64
#undef HELF_READ_64
#endif // HELF_READ_64
#define HELF_READ_16(v,a) v=a[0]*(1ULL << 8)+a[1]*(1ULL << 0)
#define HELF_READ_32(v,a) v=a[0]*(1ULL << 24)+a[1]*(1ULL << 16)+a[2]*(1ULL << 8)+a[3]*(1ULL << 0)
#define HELF_READ_64(v,a) v=a[0]*(1ULL << 56)+a[1]*(1ULL << 48)+a[2]*(1ULL << 40)+a[3]*(1ULL << 32)+a[4]*(1ULL << 24)+a[5]*(1ULL << 16)+a[6]*(1ULL << 8)+a[7]*(1ULL << 0)


        HELF_READ_16(header->e_type,header_bytes.e_type);
        HELF_READ_16(header->e_machine,header_bytes.e_machine);
        HELF_READ_32(header->e_version,header_bytes.e_version);
        HELF_READ_64(header->e_entry,header_bytes.e_entry);
        HELF_READ_64(header->e_phoff,header_bytes.e_phoff);
        HELF_READ_64(header->e_shoff,header_bytes.e_shoff);
        HELF_READ_32(header->e_flags,header_bytes.e_flags);
        HELF_READ_16(header->e_ehsize,header_bytes.e_ehsize);
        HELF_READ_16(header->e_phentsize,header_bytes.e_phentsize);
        HELF_READ_16(header->e_phnum,header_bytes.e_phnum);
        HELF_READ_16(header->e_shentsize,header_bytes.e_shentsize);
        HELF_READ_16(header->e_shnum,header_bytes.e_shnum);
        HELF_READ_16(header->e_shstrndx,header_bytes.e_shstrndx);

#undef HELF_READ_16
#undef HELF_READ_32
#undef HELF_READ_64
    }
    else
    {
#ifdef HELF_READ_16
#undef HELF_READ_16
#endif // HELF_READ_16
#ifdef HELF_READ_32
#undef HELF_READ_32
#endif // HELF_READ_32
#ifdef HELF_READ_64
#undef HELF_READ_64
#endif // HELF_READ_64
#define HELF_READ_16(v,a) v=a[1]*(1ULL << 8)+a[0]*(1ULL << 0)
#define HELF_READ_32(v,a) v=a[3]*(1ULL << 24)+a[2]*(1ULL << 16)+a[1]*(1ULL << 8)+a[0]*(1ULL << 0)
#define HELF_READ_64(v,a) v=a[7]*(1ULL << 56)+a[6]*(1ULL << 48)+a[5]*(1ULL << 40)+a[4]*(1ULL << 32)+a[3]*(1ULL << 24)+a[2]*(1ULL << 16)+a[1]*(1ULL << 8)+a[0]*(1ULL << 0)

        HELF_READ_16(header->e_type,header_bytes.e_type);
        HELF_READ_16(header->e_machine,header_bytes.e_machine);
        HELF_READ_32(header->e_version,header_bytes.e_version);
        HELF_READ_64(header->e_entry,header_bytes.e_entry);
        HELF_READ_64(header->e_phoff,header_bytes.e_phoff);
        HELF_READ_64(header->e_shoff,header_bytes.e_shoff);
        HELF_READ_32(header->e_flags,header_bytes.e_flags);
        HELF_READ_16(header->e_ehsize,header_bytes.e_ehsize);
        HELF_READ_16(header->e_phentsize,header_bytes.e_phentsize);
        HELF_READ_16(header->e_phnum,header_bytes.e_phnum);
        HELF_READ_16(header->e_shentsize,header_bytes.e_shentsize);
        HELF_READ_16(header->e_shnum,header_bytes.e_shnum);
        HELF_READ_16(header->e_shstrndx,header_bytes.e_shstrndx);

#undef HELF_READ_16
#undef HELF_READ_32
#undef HELF_READ_64
    }
    return ret;
}


