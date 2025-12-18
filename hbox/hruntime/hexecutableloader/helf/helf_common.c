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
    {
        /*
         * 检查文件头
         */
        if(!helf_is_elf(header->e_ident) || header->e_ehsize < sizeof(*header))
        {
            return false;
        }
    }
    return ret;
}


typedef struct
{
    uint8_t    p_type[4];         /* Identifies program segment type */
    uint8_t    p_offset[4];       /* Segment file offset */
    uint8_t    p_vaddr[4];        /* Segment virtual address */
    uint8_t    p_paddr[4];        /* Segment physical address */
    uint8_t    p_filesz[4];       /* Segment size in file */
    uint8_t    p_memsz[4];        /* Segment size in memory */
    uint8_t    p_flags[4];        /* Segment flags */
    uint8_t    p_align[4];        /* Segment alignment, file & memory */
} helf_elf32_program_header_bytes_t;



bool helf_file_input_32_bits_program_header_get(helf_file_input_t *input_file,size_t index,helf_elf32_program_header_t *programheader)
{
    bool ret=false;
    if(input_file==NULL || programheader ==NULL)
    {
        return ret;
    }
    helf_elf32_file_header_t fileheader;
    if(!helf_file_input_32_bits_header_get(input_file,&fileheader))
    {
        return ret;
    }
    if(index >= fileheader.e_phnum)
    {
        return ret;
    }
    helf_elf32_program_header_bytes_t programheader_bytes;
    if(helf_file_input_read(input_file,index*fileheader.e_phentsize+fileheader.e_phoff,&programheader_bytes,sizeof(programheader_bytes)) < sizeof(programheader_bytes))
    {
        return ret;
    }
    ret=true;
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
        HELF_READ_32(programheader->p_type,programheader_bytes.p_type);
        HELF_READ_32(programheader->p_flags,programheader_bytes.p_flags);
        HELF_READ_32(programheader->p_offset,programheader_bytes.p_offset);
        HELF_READ_32(programheader->p_vaddr,programheader_bytes.p_vaddr);
        HELF_READ_32(programheader->p_paddr,programheader_bytes.p_paddr);
        HELF_READ_32(programheader->p_filesz,programheader_bytes.p_filesz);
        HELF_READ_32(programheader->p_memsz,programheader_bytes.p_memsz);
        HELF_READ_32(programheader->p_align,programheader_bytes.p_align);
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
        HELF_READ_32(programheader->p_type,programheader_bytes.p_type);
        HELF_READ_32(programheader->p_flags,programheader_bytes.p_flags);
        HELF_READ_32(programheader->p_offset,programheader_bytes.p_offset);
        HELF_READ_32(programheader->p_vaddr,programheader_bytes.p_vaddr);
        HELF_READ_32(programheader->p_paddr,programheader_bytes.p_paddr);
        HELF_READ_32(programheader->p_filesz,programheader_bytes.p_filesz);
        HELF_READ_32(programheader->p_memsz,programheader_bytes.p_memsz);
        HELF_READ_32(programheader->p_align,programheader_bytes.p_align);
#undef HELF_READ_16
#undef HELF_READ_32
    }

    return ret;
}

typedef struct
{
    uint8_t    sh_name[4];            /* Section name, index in string tbl */
    uint8_t    sh_type[4];            /* Type of section */
    uint8_t    sh_flags[4];           /* Miscellaneous section attributes */
    uint8_t    sh_addr[4];            /* Section virtual addr at execution */
    uint8_t    sh_offset[4];          /* Section file offset */
    uint8_t    sh_size[4];            /* Size of section in bytes */
    uint8_t    sh_link[4];            /* Index of another section */
    uint8_t    sh_info[4];            /* Additional section information */
    uint8_t    sh_addralign[4];       /* Section alignment */
    uint8_t    sh_entsize[4];         /* Entry size if section holds table */
} helf_elf32_section_header_bytes_t;


bool helf_file_input_32_bits_section_header_get(helf_file_input_t *input_file,size_t index,helf_elf32_section_header_t *sectionheader)
{
    bool ret=false;
    if(input_file==NULL || sectionheader ==NULL)
    {
        return ret;
    }
    helf_elf32_file_header_t fileheader;
    if(!helf_file_input_32_bits_header_get(input_file,&fileheader))
    {
        return ret;
    }
    if(index >= fileheader.e_shnum)
    {
        return ret;
    }
    helf_elf32_section_header_bytes_t sectionheader_bytes;
    if(helf_file_input_read(input_file,index*fileheader.e_shentsize+fileheader.e_shoff,&sectionheader_bytes,sizeof(sectionheader_bytes)) < sizeof(sectionheader_bytes))
    {
        return ret;
    }
    ret=true;
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
        HELF_READ_32(sectionheader->sh_name,sectionheader_bytes.sh_name);
        HELF_READ_32(sectionheader->sh_type,sectionheader_bytes.sh_type);
        HELF_READ_32(sectionheader->sh_flags,sectionheader_bytes.sh_flags);
        HELF_READ_32(sectionheader->sh_addr,sectionheader_bytes.sh_addr);
        HELF_READ_32(sectionheader->sh_offset,sectionheader_bytes.sh_offset);
        HELF_READ_32(sectionheader->sh_size,sectionheader_bytes.sh_size);
        HELF_READ_32(sectionheader->sh_link,sectionheader_bytes.sh_link);
        HELF_READ_32(sectionheader->sh_info,sectionheader_bytes.sh_info);
        HELF_READ_32(sectionheader->sh_addralign,sectionheader_bytes.sh_addralign);
        HELF_READ_32(sectionheader->sh_entsize,sectionheader_bytes.sh_entsize);
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
        HELF_READ_32(sectionheader->sh_name,sectionheader_bytes.sh_name);
        HELF_READ_32(sectionheader->sh_type,sectionheader_bytes.sh_type);
        HELF_READ_32(sectionheader->sh_flags,sectionheader_bytes.sh_flags);
        HELF_READ_32(sectionheader->sh_addr,sectionheader_bytes.sh_addr);
        HELF_READ_32(sectionheader->sh_offset,sectionheader_bytes.sh_offset);
        HELF_READ_32(sectionheader->sh_size,sectionheader_bytes.sh_size);
        HELF_READ_32(sectionheader->sh_link,sectionheader_bytes.sh_link);
        HELF_READ_32(sectionheader->sh_info,sectionheader_bytes.sh_info);
        HELF_READ_32(sectionheader->sh_addralign,sectionheader_bytes.sh_addralign);
        HELF_READ_32(sectionheader->sh_entsize,sectionheader_bytes.sh_entsize);
#undef HELF_READ_16
#undef HELF_READ_32
    }

    return ret;
}

bool helf_file_input_32_bits_section_header_shstr_get(helf_file_input_t *input_file,helf_elf32_section_header_t *sectionheader)
{
    bool ret=false;
    if(input_file==NULL || sectionheader ==NULL)
    {
        return ret;
    }
    helf_elf32_file_header_t fileheader;
    if(!helf_file_input_32_bits_header_get(input_file,&fileheader))
    {
        return ret;
    }
    return helf_file_input_32_bits_section_header_get(input_file,fileheader.e_shstrndx,sectionheader);
}

bool helf_file_input_32_bits_section_header_name_get(helf_file_input_t *input_file,const helf_elf32_section_header_t *sectionheader,char *namebuf,size_t namebuf_length)
{
    bool ret=false;
    if(input_file==NULL || sectionheader ==NULL || namebuf == NULL || namebuf_length == 0)
    {
        return ret;
    }
    helf_elf32_section_header_t shstr_header;
    if(!helf_file_input_32_bits_section_header_shstr_get(input_file,&shstr_header))
    {
        return ret;
    }
    ret=true;
    for(size_t i=0 ; i<namebuf_length; i++)
    {
        char buff=0;
        if(helf_file_input_read(input_file,shstr_header.sh_offset+sectionheader->sh_name+i,&buff,sizeof(buff)) == sizeof(buff))
        {
            namebuf[i]=buff;
        }
        else
        {
            break;
        }
        if(buff=='\0')
        {
            break;
        }
    }

    return ret;
}

bool helf_file_input_32_bits_section_header_strtab_get(helf_file_input_t *input_file,helf_elf32_section_header_t *sectionheader)
{
    bool ret=false;
    if(input_file==NULL || sectionheader ==NULL)
    {
        return ret;
    }
    helf_elf32_file_header_t fileheader;
    if(!helf_file_input_32_bits_header_get(input_file,&fileheader))
    {
        return ret;
    }
    for(size_t i=0; i<fileheader.e_shnum; i++)
    {
        helf_elf32_section_header_t sh;
        char namebuf[16]= {0};
        if(helf_file_input_32_bits_section_header_get(input_file,i,&sh) && helf_file_input_32_bits_section_header_name_get(input_file,&sh,namebuf,sizeof(namebuf)-1))
        {
            if(sh.sh_type==HELF_SHT_STRTAB && namebuf[0]=='.' && namebuf[1]=='s' &&namebuf[2]=='t' &&namebuf[3]=='r' &&namebuf[4]=='t' &&namebuf[5]=='a' &&namebuf[6]=='b' &&namebuf[7]=='\0')
            {
                (*sectionheader)=sh;
                ret=true;
                break;
            }
        }
    }
    return ret;
}

bool helf_file_input_32_bits_section_header_symtab_get(helf_file_input_t *input_file,helf_elf32_section_header_t *sectionheader)
{
    bool ret=false;
    if(input_file==NULL || sectionheader ==NULL)
    {
        return ret;
    }
    helf_elf32_file_header_t fileheader;
    if(!helf_file_input_32_bits_header_get(input_file,&fileheader))
    {
        return ret;
    }
    for(size_t i=0; i<fileheader.e_shnum; i++)
    {
        helf_elf32_section_header_t sh;
        char namebuf[16]= {0};
        if(helf_file_input_32_bits_section_header_get(input_file,i,&sh) && helf_file_input_32_bits_section_header_name_get(input_file,&sh,namebuf,sizeof(namebuf)-1))
        {
            if(sh.sh_type==HELF_SHT_SYMTAB && namebuf[0]=='.' && namebuf[1]=='s' &&namebuf[2]=='y' &&namebuf[3]=='m' &&namebuf[4]=='t' &&namebuf[5]=='a' &&namebuf[6]=='b' &&namebuf[7]=='\0')
            {
                (*sectionheader)=sh;
                ret=true;
                break;
            }
        }
    }
    return ret;
}

size_t helf_file_input_32_bits_symbol_count_get(helf_file_input_t *input_file)
{
    helf_elf32_section_header_t symbol_section_header;
    if(helf_file_input_32_bits_section_header_symtab_get(input_file,&symbol_section_header))
    {
        size_t symbol_size=symbol_section_header.sh_entsize;
        if(symbol_size!=0)
        {
            return symbol_section_header.sh_size/symbol_size;
        }
    }
    return 0;
}


typedef struct
{
    uint8_t   st_name[4];                /* Symbol name (string tbl index) */
    uint8_t   st_value[4];               /* Symbol value */
    uint8_t   st_size[4];                /* Symbol size */
    uint8_t   st_info[1];                /* Symbol type and binding */
    uint8_t   st_other[1];               /* Symbol visibility */
    uint8_t   st_shndx[2];               /* Section index */
} helf_elf32_symbol_bytes_t;

bool helf_file_input_32_bits_symbol_get(helf_file_input_t *input_file,size_t index,helf_elf32_symbol_t *symbol)
{
    bool ret=false;
    if(input_file==NULL || symbol==NULL)
    {
        return ret;
    }
    helf_elf32_section_header_t symbol_section_header;
    if(!helf_file_input_32_bits_section_header_symtab_get(input_file,&symbol_section_header))
    {
        return ret;
    }

    if(symbol_section_header.sh_entsize < sizeof(*symbol) || index*symbol_section_header.sh_entsize >= symbol_section_header.sh_size)
    {
        return ret;
    }

    helf_elf32_symbol_bytes_t symbol_bytes;
    if(helf_file_input_read(input_file,index*symbol_section_header.sh_entsize+symbol_section_header.sh_offset,&symbol_bytes,sizeof(symbol_bytes)) < sizeof(symbol_bytes))
    {
        return ret;
    }
    ret=true;
    if(helf_file_input_is_big_endian(input_file))
    {
#ifdef HELF_READ_8
#undef HELF_READ_8
#endif // HELF_READ_8
#ifdef HELF_READ_16
#undef HELF_READ_16
#endif // HELF_READ_16
#ifdef HELF_READ_32
#undef HELF_READ_32
#endif // HELF_READ_32
#define HELF_READ_8(v,a)  v=a[0]
#define HELF_READ_16(v,a) v=a[0]*(1ULL << 8)+a[1]*(1ULL << 0)
#define HELF_READ_32(v,a) v=a[0]*(1ULL << 24)+a[1]*(1ULL << 16)+a[2]*(1ULL << 8)+a[3]*(1ULL << 0)
        HELF_READ_32(symbol->st_name,symbol_bytes.st_name);
        HELF_READ_32(symbol->st_value,symbol_bytes.st_value);
        HELF_READ_32(symbol->st_size,symbol_bytes.st_size);
        HELF_READ_8(symbol->st_info,symbol_bytes.st_info);
        HELF_READ_8(symbol->st_other,symbol_bytes.st_other);
        HELF_READ_16(symbol->st_shndx,symbol_bytes.st_shndx);
#undef HELF_READ_8
#undef HELF_READ_16
#undef HELF_READ_32
    }
    else
    {
#ifdef HELF_READ_8
#undef HELF_READ_8
#endif // HELF_READ_8
#ifdef HELF_READ_16
#undef HELF_READ_16
#endif // HELF_READ_16
#ifdef HELF_READ_32
#undef HELF_READ_32
#endif // HELF_READ_32
#define HELF_READ_8(v,a)  v=a[0]
#define HELF_READ_16(v,a) v=a[1]*(1ULL << 8)+a[0]*(1ULL << 0)
#define HELF_READ_32(v,a) v=a[3]*(1ULL << 24)+a[2]*(1ULL << 16)+a[1]*(1ULL << 8)+a[0]*(1ULL << 0)
        HELF_READ_32(symbol->st_name,symbol_bytes.st_name);
        HELF_READ_32(symbol->st_value,symbol_bytes.st_value);
        HELF_READ_32(symbol->st_size,symbol_bytes.st_size);
        HELF_READ_8(symbol->st_info,symbol_bytes.st_info);
        HELF_READ_8(symbol->st_other,symbol_bytes.st_other);
        HELF_READ_16(symbol->st_shndx,symbol_bytes.st_shndx);
#undef HELF_READ_8
#undef HELF_READ_16
#undef HELF_READ_32
    }


    return ret;
}

bool helf_file_input_32_bits_symbol_name_get(helf_file_input_t *input_file,const helf_elf32_symbol_t *symbol,char *namebuf,size_t namebuf_length)
{
    bool ret=false;
    if(input_file==NULL || symbol ==NULL || namebuf == NULL || namebuf_length == 0)
    {
        return ret;
    }
    helf_elf32_section_header_t strtab_header;
    if(!helf_file_input_32_bits_section_header_strtab_get(input_file,&strtab_header))
    {
        return ret;
    }
    ret=true;
    for(size_t i=0 ; i<namebuf_length; i++)
    {
        char buff=0;
        if(helf_file_input_read(input_file,strtab_header.sh_offset+symbol->st_name+i,&buff,sizeof(buff)) == sizeof(buff))
        {
            namebuf[i]=buff;
        }
        else
        {
            break;
        }
        if(buff=='\0')
        {
            break;
        }
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
    {
        /*
         * 检查文件头
         */
        if(!helf_is_elf(header->e_ident) || header->e_ehsize < sizeof(*header))
        {
            return false;
        }
    }
    return ret;
}

typedef struct
{
    uint32_t    p_type[4];         /* Identifies program segment type */
    uint32_t    p_flags[4];        /* Segment flags */
    uint64_t    p_offset[8];       /* Segment file offset */
    uint64_t    p_vaddr[8];        /* Segment virtual address */
    uint64_t    p_paddr[8];        /* Segment physical address */
    uint64_t    p_filesz[8];       /* Segment size in file */
    uint64_t    p_memsz[8];        /* Segment size in memory */
    uint64_t    p_align[8];        /* Segment alignment, file & memory */
}  helf_elf64_program_header_bytes_t;

bool helf_file_input_64_bits_program_header_get(helf_file_input_t *input_file,size_t index,helf_elf64_program_header_t *programheader)
{
    bool ret=false;
    if(input_file==NULL || programheader ==NULL)
    {
        return ret;
    }
    helf_elf64_file_header_t fileheader;
    if(!helf_file_input_64_bits_header_get(input_file,&fileheader))
    {
        return ret;
    }
    if(index >= fileheader.e_phnum)
    {
        return ret;
    }
    helf_elf64_program_header_bytes_t programheader_bytes;
    if(helf_file_input_read(input_file,index*fileheader.e_phentsize+fileheader.e_phoff,&programheader_bytes,sizeof(programheader_bytes)) < sizeof(programheader_bytes))
    {
        return ret;
    }
    ret=true;
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
        HELF_READ_32(programheader->p_type,programheader_bytes.p_type);
        HELF_READ_32(programheader->p_flags,programheader_bytes.p_flags);
        HELF_READ_64(programheader->p_offset,programheader_bytes.p_offset);
        HELF_READ_64(programheader->p_vaddr,programheader_bytes.p_vaddr);
        HELF_READ_64(programheader->p_paddr,programheader_bytes.p_paddr);
        HELF_READ_64(programheader->p_filesz,programheader_bytes.p_filesz);
        HELF_READ_64(programheader->p_memsz,programheader_bytes.p_memsz);
        HELF_READ_64(programheader->p_align,programheader_bytes.p_align);
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
        HELF_READ_32(programheader->p_type,programheader_bytes.p_type);
        HELF_READ_32(programheader->p_flags,programheader_bytes.p_flags);
        HELF_READ_64(programheader->p_offset,programheader_bytes.p_offset);
        HELF_READ_64(programheader->p_vaddr,programheader_bytes.p_vaddr);
        HELF_READ_64(programheader->p_paddr,programheader_bytes.p_paddr);
        HELF_READ_64(programheader->p_filesz,programheader_bytes.p_filesz);
        HELF_READ_64(programheader->p_memsz,programheader_bytes.p_memsz);
        HELF_READ_64(programheader->p_align,programheader_bytes.p_align);
#undef HELF_READ_16
#undef HELF_READ_32
#undef HELF_READ_64
    }

    return ret;
}

typedef struct
{
    uint8_t    sh_name[4];        /* Section name, index in string tbl */
    uint8_t    sh_type[4];        /* Type of section */
    uint8_t    sh_flags[8];       /* Miscellaneous section attributes */
    uint8_t    sh_addr[8];        /* Section virtual addr at execution */
    uint8_t    sh_offset[8];      /* Section file offset */
    uint8_t    sh_size[8];        /* Size of section in bytes */
    uint8_t    sh_link[4];        /* Index of another section */
    uint8_t    sh_info[4];        /* Additional section information */
    uint8_t    sh_addralign[8];   /* Section alignment */
    uint8_t    sh_entsize[8];     /* Entry size if section holds table */
} helf_elf64_section_header_bytes_t;

bool helf_file_input_64_bits_section_header_get(helf_file_input_t *input_file,size_t index,helf_elf64_section_header_t *sectionheader)
{
    bool ret=false;
    if(input_file==NULL || sectionheader ==NULL)
    {
        return ret;
    }
    helf_elf64_file_header_t fileheader;
    if(!helf_file_input_64_bits_header_get(input_file,&fileheader))
    {
        return ret;
    }
    if(index >= fileheader.e_shnum)
    {
        return ret;
    }
    helf_elf64_section_header_bytes_t sectionheader_bytes;
    if(helf_file_input_read(input_file,index*fileheader.e_shentsize+fileheader.e_shoff,&sectionheader_bytes,sizeof(sectionheader_bytes)) < sizeof(sectionheader_bytes))
    {
        return ret;
    }
    ret=true;
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
#define HELF_READ_64(v,a) v=a[0]*(1ULL << 56)+a[1]*(1ULL << 48)+a[2]*(1ULL << 40)+a[3]*(1ULL << 32)+a[4]*(1ULL << 24)+a[5]*(1ULL << 16)+a[6]*(1ULL << 8)+a[7]*(1ULL << 0)
        HELF_READ_32(sectionheader->sh_name,sectionheader_bytes.sh_name);
        HELF_READ_32(sectionheader->sh_type,sectionheader_bytes.sh_type);
        HELF_READ_64(sectionheader->sh_flags,sectionheader_bytes.sh_flags);
        HELF_READ_64(sectionheader->sh_addr,sectionheader_bytes.sh_addr);
        HELF_READ_64(sectionheader->sh_offset,sectionheader_bytes.sh_offset);
        HELF_READ_64(sectionheader->sh_size,sectionheader_bytes.sh_size);
        HELF_READ_32(sectionheader->sh_link,sectionheader_bytes.sh_link);
        HELF_READ_32(sectionheader->sh_info,sectionheader_bytes.sh_info);
        HELF_READ_64(sectionheader->sh_addralign,sectionheader_bytes.sh_addralign);
        HELF_READ_64(sectionheader->sh_entsize,sectionheader_bytes.sh_entsize);
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
#define HELF_READ_16(v,a) v=a[1]*(1ULL << 8)+a[0]*(1ULL << 0)
#define HELF_READ_32(v,a) v=a[3]*(1ULL << 24)+a[2]*(1ULL << 16)+a[1]*(1ULL << 8)+a[0]*(1ULL << 0)
#define HELF_READ_64(v,a) v=a[7]*(1ULL << 56)+a[6]*(1ULL << 48)+a[5]*(1ULL << 40)+a[4]*(1ULL << 32)+a[3]*(1ULL << 24)+a[2]*(1ULL << 16)+a[1]*(1ULL << 8)+a[0]*(1ULL << 0)
        HELF_READ_32(sectionheader->sh_name,sectionheader_bytes.sh_name);
        HELF_READ_32(sectionheader->sh_type,sectionheader_bytes.sh_type);
        HELF_READ_64(sectionheader->sh_flags,sectionheader_bytes.sh_flags);
        HELF_READ_64(sectionheader->sh_addr,sectionheader_bytes.sh_addr);
        HELF_READ_64(sectionheader->sh_offset,sectionheader_bytes.sh_offset);
        HELF_READ_64(sectionheader->sh_size,sectionheader_bytes.sh_size);
        HELF_READ_32(sectionheader->sh_link,sectionheader_bytes.sh_link);
        HELF_READ_32(sectionheader->sh_info,sectionheader_bytes.sh_info);
        HELF_READ_64(sectionheader->sh_addralign,sectionheader_bytes.sh_addralign);
        HELF_READ_64(sectionheader->sh_entsize,sectionheader_bytes.sh_entsize);
#undef HELF_READ_16
#undef HELF_READ_32
#undef HELF_READ_64
    }

    return ret;
}

bool helf_file_input_64_bits_section_header_shstr_get(helf_file_input_t *input_file,helf_elf64_section_header_t *sectionheader)
{
    bool ret=false;
    if(input_file==NULL || sectionheader ==NULL)
    {
        return ret;
    }
    helf_elf64_file_header_t fileheader;
    if(!helf_file_input_64_bits_header_get(input_file,&fileheader))
    {
        return ret;
    }
    return helf_file_input_64_bits_section_header_get(input_file,fileheader.e_shstrndx,sectionheader);
}

bool helf_file_input_64_bits_section_header_name_get(helf_file_input_t *input_file,const helf_elf64_section_header_t *sectionheader,char *namebuf,size_t namebuf_length)
{
    bool ret=false;
    if(input_file==NULL || sectionheader ==NULL || namebuf == NULL || namebuf_length == 0)
    {
        return ret;
    }
    helf_elf64_section_header_t shstr_header;
    if(!helf_file_input_64_bits_section_header_shstr_get(input_file,&shstr_header))
    {
        return ret;
    }
    ret=true;
    for(size_t i=0 ; i<namebuf_length; i++)
    {
        char buff=0;
        if(helf_file_input_read(input_file,shstr_header.sh_offset+sectionheader->sh_name+i,&buff,sizeof(buff)) == sizeof(buff))
        {
            namebuf[i]=buff;
        }
        else
        {
            break;
        }
        if(buff=='\0')
        {
            break;
        }
    }

    return ret;
}

bool helf_file_input_64_bits_section_header_strtab_get(helf_file_input_t *input_file,helf_elf64_section_header_t *sectionheader)
{
    bool ret=false;
    if(input_file==NULL || sectionheader ==NULL)
    {
        return ret;
    }
    helf_elf64_file_header_t fileheader;
    if(!helf_file_input_64_bits_header_get(input_file,&fileheader))
    {
        return ret;
    }
    for(size_t i=0; i<fileheader.e_shnum; i++)
    {
        helf_elf64_section_header_t sh;
        char namebuf[16]= {0};
        if(helf_file_input_64_bits_section_header_get(input_file,i,&sh) && helf_file_input_64_bits_section_header_name_get(input_file,&sh,namebuf,sizeof(namebuf)-1))
        {
            if(sh.sh_type==HELF_SHT_STRTAB && namebuf[0]=='.' && namebuf[1]=='s' &&namebuf[2]=='t' &&namebuf[3]=='r' &&namebuf[4]=='t' &&namebuf[5]=='a' &&namebuf[6]=='b' &&namebuf[7]=='\0')
            {
                (*sectionheader)=sh;
                ret=true;
                break;
            }
        }
    }
    return ret;
}

bool helf_file_input_64_bits_section_header_symtab_get(helf_file_input_t *input_file,helf_elf64_section_header_t *sectionheader)
{
    bool ret=false;
    if(input_file==NULL || sectionheader ==NULL)
    {
        return ret;
    }
    helf_elf64_file_header_t fileheader;
    if(!helf_file_input_64_bits_header_get(input_file,&fileheader))
    {
        return ret;
    }
    for(size_t i=0; i<fileheader.e_shnum; i++)
    {
        helf_elf64_section_header_t sh;
        char namebuf[16]= {0};
        if(helf_file_input_64_bits_section_header_get(input_file,i,&sh) && helf_file_input_64_bits_section_header_name_get(input_file,&sh,namebuf,sizeof(namebuf)-1))
        {
            if(sh.sh_type==HELF_SHT_SYMTAB && namebuf[0]=='.' && namebuf[1]=='s' &&namebuf[2]=='y' &&namebuf[3]=='m' &&namebuf[4]=='t' &&namebuf[5]=='a' &&namebuf[6]=='b' &&namebuf[7]=='\0')
            {
                (*sectionheader)=sh;
                ret=true;
                break;
            }
        }
    }
    return ret;
}

size_t helf_file_input_64_bits_symbol_count_get(helf_file_input_t *input_file)
{
    helf_elf64_section_header_t symbol_section_header;
    if(helf_file_input_64_bits_section_header_symtab_get(input_file,&symbol_section_header))
    {
        size_t symbol_size=symbol_section_header.sh_entsize;
        if(symbol_size!=0)
        {
            return symbol_section_header.sh_size/symbol_size;
        }
    }
    return 0;
}


typedef struct
{
    uint8_t st_name[4];                 /* Symbol name (string tbl index) */
    uint8_t st_info[1];                  /* Symbol type and binding */
    uint8_t st_other[1];                 /* Symbol visibility */
    uint8_t st_shndx[2];                 /* Section index */
    uint8_t st_value[8];                 /* Symbol value */
    uint8_t st_size[8];                  /* Symbol size */
} helf_elf64_symbol_bytes_t;

bool helf_file_input_64_bits_symbol_get(helf_file_input_t *input_file,size_t index,helf_elf64_symbol_t *symbol)
{
    bool ret=false;
    if(input_file==NULL || symbol==NULL)
    {
        return ret;
    }
    helf_elf64_section_header_t symbol_section_header;
    if(!helf_file_input_64_bits_section_header_symtab_get(input_file,&symbol_section_header))
    {
        return ret;
    }

    if(symbol_section_header.sh_entsize < sizeof(*symbol) || index*symbol_section_header.sh_entsize >= symbol_section_header.sh_size)
    {
        return ret;
    }

    helf_elf64_symbol_bytes_t symbol_bytes;
    if(helf_file_input_read(input_file,index*symbol_section_header.sh_entsize+symbol_section_header.sh_offset,&symbol_bytes,sizeof(symbol_bytes)) < sizeof(symbol_bytes))
    {
        return ret;
    }
    ret=true;
    if(helf_file_input_is_big_endian(input_file))
    {
#ifdef HELF_READ_8
#undef HELF_READ_8
#endif // HELF_READ_8
#ifdef HELF_READ_16
#undef HELF_READ_16
#endif // HELF_READ_16
#ifdef HELF_READ_32
#undef HELF_READ_32
#endif // HELF_READ_32
#define HELF_READ_8(v,a)  v=a[0]
#define HELF_READ_16(v,a) v=a[0]*(1ULL << 8)+a[1]*(1ULL << 0)
#define HELF_READ_32(v,a) v=a[0]*(1ULL << 24)+a[1]*(1ULL << 16)+a[2]*(1ULL << 8)+a[3]*(1ULL << 0)
#define HELF_READ_64(v,a) v=a[0]*(1ULL << 56)+a[1]*(1ULL << 48)+a[2]*(1ULL << 40)+a[3]*(1ULL << 32)+a[4]*(1ULL << 24)+a[5]*(1ULL << 16)+a[6]*(1ULL << 8)+a[7]*(1ULL << 0)
        HELF_READ_32(symbol->st_name,symbol_bytes.st_name);
        HELF_READ_8(symbol->st_info,symbol_bytes.st_info);
        HELF_READ_8(symbol->st_other,symbol_bytes.st_other);
        HELF_READ_16(symbol->st_shndx,symbol_bytes.st_shndx);
        HELF_READ_64(symbol->st_value,symbol_bytes.st_value);
        HELF_READ_64(symbol->st_size,symbol_bytes.st_size);
#undef HELF_READ_8
#undef HELF_READ_16
#undef HELF_READ_32
#undef HELF_READ_64
    }
    else
    {
#ifdef HELF_READ_8
#undef HELF_READ_8
#endif // HELF_READ_8
#ifdef HELF_READ_16
#undef HELF_READ_16
#endif // HELF_READ_16
#ifdef HELF_READ_32
#undef HELF_READ_32
#endif // HELF_READ_32
#define HELF_READ_8(v,a)  v=a[0]
#define HELF_READ_16(v,a) v=a[1]*(1ULL << 8)+a[0]*(1ULL << 0)
#define HELF_READ_32(v,a) v=a[3]*(1ULL << 24)+a[2]*(1ULL << 16)+a[1]*(1ULL << 8)+a[0]*(1ULL << 0)
#define HELF_READ_64(v,a) v=a[7]*(1ULL << 56)+a[6]*(1ULL << 48)+a[5]*(1ULL << 40)+a[4]*(1ULL << 32)+a[3]*(1ULL << 24)+a[2]*(1ULL << 16)+a[1]*(1ULL << 8)+a[0]*(1ULL << 0)
        HELF_READ_32(symbol->st_name,symbol_bytes.st_name);
        HELF_READ_8(symbol->st_info,symbol_bytes.st_info);
        HELF_READ_8(symbol->st_other,symbol_bytes.st_other);
        HELF_READ_16(symbol->st_shndx,symbol_bytes.st_shndx);
        HELF_READ_64(symbol->st_value,symbol_bytes.st_value);
        HELF_READ_64(symbol->st_size,symbol_bytes.st_size);
#undef HELF_READ_8
#undef HELF_READ_16
#undef HELF_READ_32
#undef HELF_READ_64
    }


    return ret;
}

bool helf_file_input_64_bits_symbol_name_get(helf_file_input_t *input_file,const helf_elf64_symbol_t *symbol,char *namebuf,size_t namebuf_length)
{
    bool ret=false;
    if(input_file==NULL || symbol ==NULL || namebuf == NULL || namebuf_length == 0)
    {
        return ret;
    }
    helf_elf64_section_header_t strtab_header;
    if(!helf_file_input_64_bits_section_header_strtab_get(input_file,&strtab_header))
    {
        return ret;
    }
    ret=true;
    for(size_t i=0 ; i<namebuf_length; i++)
    {
        char buff=0;
        if(helf_file_input_read(input_file,strtab_header.sh_offset+symbol->st_name+i,&buff,sizeof(buff)) == sizeof(buff))
        {
            namebuf[i]=buff;
        }
        else
        {
            break;
        }
        if(buff=='\0')
        {
            break;
        }
    }

    return ret;
}
