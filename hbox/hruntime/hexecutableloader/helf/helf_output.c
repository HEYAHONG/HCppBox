/***************************************************************
 * Name:      helf_output.c
 * Purpose:   实现helf_output接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "helf_output.h"

typedef struct
{
    uint8_t e_ident[16];
    uint8_t e_type[2];
    uint8_t e_machine[2];
    uint8_t e_version[4];
    uint8_t e_entry[4];
    uint8_t e_phoff[4];
    uint8_t e_shoff[4];
    uint8_t e_flags[4];
    uint8_t e_ehsize[2];
    uint8_t e_phentsize[2];
    uint8_t e_phnum[2];
    uint8_t e_shentsize[2];
    uint8_t e_shnum[2];
    uint8_t e_shstrndx[2];
} helf_output_elf32_file_header_bytes_t;

typedef struct
{
    uint8_t e_ident[16];
    uint8_t e_type[2];
    uint8_t e_machine[2];
    uint8_t e_version[4];
    uint8_t e_entry[8];
    uint8_t e_phoff[8];
    uint8_t e_shoff[8];
    uint8_t e_flags[4];
    uint8_t e_ehsize[2];
    uint8_t e_phentsize[2];
    uint8_t e_phnum[2];
    uint8_t e_shentsize[2];
    uint8_t e_shnum[2];
    uint8_t e_shstrndx[2];
} helf_output_elf64_file_header_bytes_t;

typedef struct
{
    uint8_t sh_name[4];
    uint8_t sh_type[4];
    uint8_t sh_flags[4];
    uint8_t sh_addr[4];
    uint8_t sh_offset[4];
    uint8_t sh_size[4];
    uint8_t sh_link[4];
    uint8_t sh_info[4];
    uint8_t sh_addralign[4];
    uint8_t sh_entsize[4];
} helf_output_elf32_section_header_bytes_t;

typedef struct
{
    uint8_t sh_name[4];
    uint8_t sh_type[4];
    uint8_t sh_flags[8];
    uint8_t sh_addr[8];
    uint8_t sh_offset[8];
    uint8_t sh_size[8];
    uint8_t sh_link[4];
    uint8_t sh_info[4];
    uint8_t sh_addralign[8];
    uint8_t sh_entsize[8];
} helf_output_elf64_section_header_bytes_t;

typedef struct
{
    uint8_t p_type[4];
    uint8_t p_offset[4];
    uint8_t p_vaddr[4];
    uint8_t p_paddr[4];
    uint8_t p_filesz[4];
    uint8_t p_memsz[4];
    uint8_t p_flags[4];
    uint8_t p_align[4];
} helf_output_elf32_program_header_bytes_t;

typedef struct
{
    uint8_t p_type[4];
    uint8_t p_flags[4];
    uint8_t p_offset[8];
    uint8_t p_vaddr[8];
    uint8_t p_paddr[8];
    uint8_t p_filesz[8];
    uint8_t p_memsz[8];
    uint8_t p_align[8];
} helf_output_elf64_program_header_bytes_t;

static void helf_output_store_uint16(uint8_t *data,uint16_t value,bool big_endian)
{
    if(big_endian)
    {
        data[0]=(uint8_t)((value >> 8) & 0xFF);
        data[1]=(uint8_t)((value >> 0) & 0xFF);
    }
    else
    {
        data[0]=(uint8_t)((value >> 0) & 0xFF);
        data[1]=(uint8_t)((value >> 8) & 0xFF);
    }
}

static void helf_output_store_uint32(uint8_t *data,uint32_t value,bool big_endian)
{
    if(big_endian)
    {
        data[0]=(uint8_t)((value >> 24) & 0xFF);
        data[1]=(uint8_t)((value >> 16) & 0xFF);
        data[2]=(uint8_t)((value >> 8) & 0xFF);
        data[3]=(uint8_t)((value >> 0) & 0xFF);
    }
    else
    {
        data[0]=(uint8_t)((value >> 0) & 0xFF);
        data[1]=(uint8_t)((value >> 8) & 0xFF);
        data[2]=(uint8_t)((value >> 16) & 0xFF);
        data[3]=(uint8_t)((value >> 24) & 0xFF);
    }
}

static void helf_output_store_uint64(uint8_t *data,uint64_t value,bool big_endian)
{
    if(big_endian)
    {
        data[0]=(uint8_t)((value >> 56) & 0xFF);
        data[1]=(uint8_t)((value >> 48) & 0xFF);
        data[2]=(uint8_t)((value >> 40) & 0xFF);
        data[3]=(uint8_t)((value >> 32) & 0xFF);
        data[4]=(uint8_t)((value >> 24) & 0xFF);
        data[5]=(uint8_t)((value >> 16) & 0xFF);
        data[6]=(uint8_t)((value >> 8) & 0xFF);
        data[7]=(uint8_t)((value >> 0) & 0xFF);
    }
    else
    {
        data[0]=(uint8_t)((value >> 0) & 0xFF);
        data[1]=(uint8_t)((value >> 8) & 0xFF);
        data[2]=(uint8_t)((value >> 16) & 0xFF);
        data[3]=(uint8_t)((value >> 24) & 0xFF);
        data[4]=(uint8_t)((value >> 32) & 0xFF);
        data[5]=(uint8_t)((value >> 40) & 0xFF);
        data[6]=(uint8_t)((value >> 48) & 0xFF);
        data[7]=(uint8_t)((value >> 56) & 0xFF);
    }
}

static bool helf_output_ident_is_big_endian(const uint8_t *e_ident)
{
    return e_ident!=NULL && e_ident[HELF_EI_DATA]==HELF_ELFDATA_2_MSB;
}

typedef struct
{
    helf_file_output_t *output;
} helf_output_input_adapter_t;

static size_t helf_output_input_adapter_read(helf_file_input_t *input,uintptr_t address,void *buffer,size_t buffer_length)
{
    helf_output_input_adapter_t *adapter=(helf_output_input_adapter_t *)input->usr;
    return helf_file_output_read(adapter->output,address,buffer,buffer_length);
}

bool helf_file_output_32_bits_header_set(helf_file_output_t *output,const helf_elf32_file_header_t *header)
{
    if(output==NULL || header==NULL || !helf_is_elf((uint8_t *)header->e_ident)
       || helf_elfclass_get((uint8_t *)header->e_ident)!=HELF_ELFCLASS_32)
    {
        return false;
    }
    bool big_endian=helf_output_ident_is_big_endian(header->e_ident);
    helf_output_elf32_file_header_bytes_t bytes;
    memset(&bytes,0,sizeof(bytes));
    memcpy(bytes.e_ident,header->e_ident,sizeof(bytes.e_ident));
    helf_output_store_uint16(bytes.e_type,header->e_type,big_endian);
    helf_output_store_uint16(bytes.e_machine,header->e_machine,big_endian);
    helf_output_store_uint32(bytes.e_version,header->e_version,big_endian);
    helf_output_store_uint32(bytes.e_entry,header->e_entry,big_endian);
    helf_output_store_uint32(bytes.e_phoff,header->e_phoff,big_endian);
    helf_output_store_uint32(bytes.e_shoff,header->e_shoff,big_endian);
    helf_output_store_uint32(bytes.e_flags,header->e_flags,big_endian);
    helf_output_store_uint16(bytes.e_ehsize,header->e_ehsize,big_endian);
    helf_output_store_uint16(bytes.e_phentsize,header->e_phentsize,big_endian);
    helf_output_store_uint16(bytes.e_phnum,header->e_phnum,big_endian);
    helf_output_store_uint16(bytes.e_shentsize,header->e_shentsize,big_endian);
    helf_output_store_uint16(bytes.e_shnum,header->e_shnum,big_endian);
    helf_output_store_uint16(bytes.e_shstrndx,header->e_shstrndx,big_endian);
    return helf_file_output_write(output,0,&bytes,sizeof(bytes))==sizeof(bytes);
}

bool helf_file_output_64_bits_header_set(helf_file_output_t *output,const helf_elf64_file_header_t *header)
{
    if(output==NULL || header==NULL || !helf_is_elf((uint8_t *)header->e_ident)
       || helf_elfclass_get((uint8_t *)header->e_ident)!=HELF_ELFCLASS_64)
    {
        return false;
    }
    bool big_endian=helf_output_ident_is_big_endian(header->e_ident);
    helf_output_elf64_file_header_bytes_t bytes;
    memset(&bytes,0,sizeof(bytes));
    memcpy(bytes.e_ident,header->e_ident,sizeof(bytes.e_ident));
    helf_output_store_uint16(bytes.e_type,header->e_type,big_endian);
    helf_output_store_uint16(bytes.e_machine,header->e_machine,big_endian);
    helf_output_store_uint32(bytes.e_version,header->e_version,big_endian);
    helf_output_store_uint64(bytes.e_entry,header->e_entry,big_endian);
    helf_output_store_uint64(bytes.e_phoff,header->e_phoff,big_endian);
    helf_output_store_uint64(bytes.e_shoff,header->e_shoff,big_endian);
    helf_output_store_uint32(bytes.e_flags,header->e_flags,big_endian);
    helf_output_store_uint16(bytes.e_ehsize,header->e_ehsize,big_endian);
    helf_output_store_uint16(bytes.e_phentsize,header->e_phentsize,big_endian);
    helf_output_store_uint16(bytes.e_phnum,header->e_phnum,big_endian);
    helf_output_store_uint16(bytes.e_shentsize,header->e_shentsize,big_endian);
    helf_output_store_uint16(bytes.e_shnum,header->e_shnum,big_endian);
    helf_output_store_uint16(bytes.e_shstrndx,header->e_shstrndx,big_endian);
    return helf_file_output_write(output,0,&bytes,sizeof(bytes))==sizeof(bytes);
}

static bool helf_output_get_file_input(helf_file_output_t *output,helf_file_input_t *input,helf_output_input_adapter_t *adapter)
{
    if(output==NULL || input==NULL || adapter==NULL)
    {
        return false;
    }
    adapter->output=output;
    helf_file_input_init(input,helf_output_input_adapter_read,adapter);
    return true;
}

bool helf_file_output_32_bits_section_header_set(helf_file_output_t *output,size_t index,const helf_elf32_section_header_t *sectionheader)
{
    if(output==NULL || sectionheader==NULL)
    {
        return false;
    }
    helf_file_input_t input;
    helf_output_input_adapter_t adapter;
    if(!helf_output_get_file_input(output,&input,&adapter))
    {
        return false;
    }
    helf_elf32_file_header_t fileheader;
    if(!helf_file_input_32_bits_header_get(&input,&fileheader))
    {
        return false;
    }
    if(index>=fileheader.e_shnum)
    {
        return false;
    }
    bool big_endian=helf_file_input_is_big_endian(&input);
    helf_output_elf32_section_header_bytes_t bytes;
    memset(&bytes,0,sizeof(bytes));
    helf_output_store_uint32(bytes.sh_name,sectionheader->sh_name,big_endian);
    helf_output_store_uint32(bytes.sh_type,sectionheader->sh_type,big_endian);
    helf_output_store_uint32(bytes.sh_flags,sectionheader->sh_flags,big_endian);
    helf_output_store_uint32(bytes.sh_addr,sectionheader->sh_addr,big_endian);
    helf_output_store_uint32(bytes.sh_offset,sectionheader->sh_offset,big_endian);
    helf_output_store_uint32(bytes.sh_size,sectionheader->sh_size,big_endian);
    helf_output_store_uint32(bytes.sh_link,sectionheader->sh_link,big_endian);
    helf_output_store_uint32(bytes.sh_info,sectionheader->sh_info,big_endian);
    helf_output_store_uint32(bytes.sh_addralign,sectionheader->sh_addralign,big_endian);
    helf_output_store_uint32(bytes.sh_entsize,sectionheader->sh_entsize,big_endian);
    uintptr_t address=(uintptr_t)fileheader.e_shoff+index*fileheader.e_shentsize;
    return helf_file_output_write(output,address,&bytes,sizeof(bytes))==sizeof(bytes);
}

bool helf_file_output_64_bits_section_header_set(helf_file_output_t *output,size_t index,const helf_elf64_section_header_t *sectionheader)
{
    if(output==NULL || sectionheader==NULL)
    {
        return false;
    }
    helf_file_input_t input;
    helf_output_input_adapter_t adapter;
    if(!helf_output_get_file_input(output,&input,&adapter))
    {
        return false;
    }
    helf_elf64_file_header_t fileheader;
    if(!helf_file_input_64_bits_header_get(&input,&fileheader))
    {
        return false;
    }
    if(index>=fileheader.e_shnum)
    {
        return false;
    }
    bool big_endian=helf_file_input_is_big_endian(&input);
    helf_output_elf64_section_header_bytes_t bytes;
    memset(&bytes,0,sizeof(bytes));
    helf_output_store_uint32(bytes.sh_name,sectionheader->sh_name,big_endian);
    helf_output_store_uint32(bytes.sh_type,sectionheader->sh_type,big_endian);
    helf_output_store_uint64(bytes.sh_flags,sectionheader->sh_flags,big_endian);
    helf_output_store_uint64(bytes.sh_addr,sectionheader->sh_addr,big_endian);
    helf_output_store_uint64(bytes.sh_offset,sectionheader->sh_offset,big_endian);
    helf_output_store_uint64(bytes.sh_size,sectionheader->sh_size,big_endian);
    helf_output_store_uint32(bytes.sh_link,sectionheader->sh_link,big_endian);
    helf_output_store_uint32(bytes.sh_info,sectionheader->sh_info,big_endian);
    helf_output_store_uint64(bytes.sh_addralign,sectionheader->sh_addralign,big_endian);
    helf_output_store_uint64(bytes.sh_entsize,sectionheader->sh_entsize,big_endian);
    uintptr_t address=(uintptr_t)fileheader.e_shoff+index*fileheader.e_shentsize;
    return helf_file_output_write(output,address,&bytes,sizeof(bytes))==sizeof(bytes);
}

bool helf_file_output_32_bits_program_header_set(helf_file_output_t *output,size_t index,const helf_elf32_program_header_t *programheader)
{
    if(output==NULL || programheader==NULL)
    {
        return false;
    }
    helf_file_input_t input;
    helf_output_input_adapter_t adapter;
    if(!helf_output_get_file_input(output,&input,&adapter))
    {
        return false;
    }
    helf_elf32_file_header_t fileheader;
    if(!helf_file_input_32_bits_header_get(&input,&fileheader))
    {
        return false;
    }
    if(index>=fileheader.e_phnum)
    {
        return false;
    }
    bool big_endian=helf_file_input_is_big_endian(&input);
    helf_output_elf32_program_header_bytes_t bytes;
    memset(&bytes,0,sizeof(bytes));
    helf_output_store_uint32(bytes.p_type,programheader->p_type,big_endian);
    helf_output_store_uint32(bytes.p_offset,programheader->p_offset,big_endian);
    helf_output_store_uint32(bytes.p_vaddr,programheader->p_vaddr,big_endian);
    helf_output_store_uint32(bytes.p_paddr,programheader->p_paddr,big_endian);
    helf_output_store_uint32(bytes.p_filesz,programheader->p_filesz,big_endian);
    helf_output_store_uint32(bytes.p_memsz,programheader->p_memsz,big_endian);
    helf_output_store_uint32(bytes.p_flags,programheader->p_flags,big_endian);
    helf_output_store_uint32(bytes.p_align,programheader->p_align,big_endian);
    uintptr_t address=(uintptr_t)fileheader.e_phoff+index*fileheader.e_phentsize;
    return helf_file_output_write(output,address,&bytes,sizeof(bytes))==sizeof(bytes);
}

bool helf_file_output_64_bits_program_header_set(helf_file_output_t *output,size_t index,const helf_elf64_program_header_t *programheader)
{
    if(output==NULL || programheader==NULL)
    {
        return false;
    }
    helf_file_input_t input;
    helf_output_input_adapter_t adapter;
    if(!helf_output_get_file_input(output,&input,&adapter))
    {
        return false;
    }
    helf_elf64_file_header_t fileheader;
    if(!helf_file_input_64_bits_header_get(&input,&fileheader))
    {
        return false;
    }
    if(index>=fileheader.e_phnum)
    {
        return false;
    }
    bool big_endian=helf_file_input_is_big_endian(&input);
    helf_output_elf64_program_header_bytes_t bytes;
    memset(&bytes,0,sizeof(bytes));
    helf_output_store_uint32(bytes.p_type,programheader->p_type,big_endian);
    helf_output_store_uint32(bytes.p_flags,programheader->p_flags,big_endian);
    helf_output_store_uint64(bytes.p_offset,programheader->p_offset,big_endian);
    helf_output_store_uint64(bytes.p_vaddr,programheader->p_vaddr,big_endian);
    helf_output_store_uint64(bytes.p_paddr,programheader->p_paddr,big_endian);
    helf_output_store_uint64(bytes.p_filesz,programheader->p_filesz,big_endian);
    helf_output_store_uint64(bytes.p_memsz,programheader->p_memsz,big_endian);
    helf_output_store_uint64(bytes.p_align,programheader->p_align,big_endian);
    uintptr_t address=(uintptr_t)fileheader.e_phoff+index*fileheader.e_phentsize;
    return helf_file_output_write(output,address,&bytes,sizeof(bytes))==sizeof(bytes);
}

static uint64_t helf_output_align_up(uint64_t value,uint64_t alignment)
{
    if(alignment<=1)
    {
        return value;
    }
    uint64_t remainder=value%alignment;
    if(remainder==0)
    {
        return value;
    }
    return value+(alignment-remainder);
}

static uint64_t helf_output_section_offset(helf_output_editor_t *editor,uint64_t current,const helf_output_section_t *section)
{
    uint64_t alignment=section->addralign;
    if(alignment==0)
    {
        alignment=1;
    }
    uint64_t offset=helf_output_align_up(current,alignment);
    if(editor->page_size>1 && (section->flags&HELF_SHF_ALLOC)!=0 && section->address!=0)
    {
        uint64_t page_size=editor->page_size;
        uint64_t address_mod=section->address%page_size;
        uint64_t page_base=helf_output_align_up(offset,page_size);
        offset=page_base+address_mod;
        if(offset<current)
        {
            offset+=page_size;
        }
        if(section->addralign>1 && (offset%section->addralign)!=0)
        {
            offset=helf_output_align_up(offset,section->addralign);
            uint64_t offset_mod=offset%page_size;
            if(offset_mod!=address_mod)
            {
                offset+=(address_mod+page_size-offset_mod)%page_size;
            }
        }
    }
    return offset;
}

static uint64_t helf_output_segment_offset(helf_output_editor_t *editor,uint64_t current,uint64_t vaddr)
{
    if(editor->page_size<=1)
    {
        return current;
    }
    uint64_t page_size=editor->page_size;
    uint64_t page_base=helf_output_align_up(current,page_size);
    uint64_t address_mod=vaddr%page_size;
    uint64_t offset=page_base+address_mod;
    if(offset<current)
    {
        offset+=page_size;
    }
    return offset;
}

static bool helf_output_is_big_endian(helf_output_editor_t *editor)
{
    return editor!=NULL && editor->elfdata==HELF_ELFDATA_2_MSB;
}

void helf_output_editor_init(helf_output_editor_t *editor,helf_file_output_t *output,uint8_t elfclass,uint8_t elfdata,helf_output_section_t *sections,size_t section_capacity,helf_output_segment_t *segments,size_t segment_capacity)
{
    if(editor==NULL)
    {
        return;
    }
    memset(editor,0,sizeof(*editor));
    editor->output=output;
    editor->elfclass=elfclass;
    editor->elfdata=elfdata;
    editor->header.osabi=HELF_ELFOSABI_NONE;
    editor->header.abiversion=0;
    editor->header.type=HELF_ET_EXEC;
    editor->header.machine=HELF_EM_NONE;
    editor->header.version=HELF_EV_CURRENT;
    editor->header.entry=0;
    editor->header.flags=0;
    editor->sections=sections;
    editor->section_capacity=section_capacity;
    editor->section_count=0;
    if(editor->sections!=NULL && editor->section_capacity>0)
    {
        memset(&editor->sections[0],0,sizeof(editor->sections[0]));
        editor->sections[0].type=HELF_SHT_NULL;
        editor->sections[0].present=true;
        editor->section_count=1;
    }
    editor->segments=segments;
    editor->segment_capacity=segment_capacity;
    editor->segment_count=0;
    editor->laid_out=false;
    editor->page_size=0x1000;
}

bool helf_output_editor_load(helf_output_editor_t *editor,helf_file_input_t *input)
{
    if(editor==NULL || input==NULL)
    {
        return false;
    }
    bool is_64_bits=helf_file_input_is_64_bits(input);
    bool is_32_bits=helf_file_input_is_32_bits(input);
    if(!is_64_bits && !is_32_bits)
    {
        return false;
    }
    uint16_t section_count=0;
    uint16_t program_header_count=0;
    uint64_t program_header_offset=0;
    uint64_t section_header_offset=0;
    uint16_t program_header_entry_size=0;
    uint16_t section_header_entry_size=0;
    editor->header.osabi=HELF_ELFOSABI_NONE;
    editor->header.abiversion=0;
    editor->header.type=HELF_ET_NONE;
    editor->header.machine=HELF_EM_NONE;
    editor->header.version=HELF_EV_CURRENT;
    editor->header.entry=0;
    editor->header.flags=0;
    if(is_64_bits)
    {
        helf_elf64_file_header_t header;
        if(!helf_file_input_64_bits_header_get(input,&header))
        {
            return false;
        }
        editor->elfclass=HELF_ELFCLASS_64;
        editor->header.osabi=header.e_ident[HELF_EI_OSABI];
        editor->header.abiversion=header.e_ident[HELF_EI_ABIVERSION];
        editor->header.type=header.e_type;
        editor->header.machine=header.e_machine;
        editor->header.version=header.e_version;
        editor->header.entry=header.e_entry;
        editor->header.flags=header.e_flags;
        section_count=header.e_shnum;
        program_header_count=header.e_phnum;
        program_header_offset=header.e_phoff;
        section_header_offset=header.e_shoff;
        program_header_entry_size=header.e_phentsize;
        section_header_entry_size=header.e_shentsize;
        editor->loaded_section_name_table_index=header.e_shstrndx;
    }
    else
    {
        helf_elf32_file_header_t header;
        if(!helf_file_input_32_bits_header_get(input,&header))
        {
            return false;
        }
        editor->elfclass=HELF_ELFCLASS_32;
        editor->header.osabi=header.e_ident[HELF_EI_OSABI];
        editor->header.abiversion=header.e_ident[HELF_EI_ABIVERSION];
        editor->header.type=header.e_type;
        editor->header.machine=header.e_machine;
        editor->header.version=header.e_version;
        editor->header.entry=header.e_entry;
        editor->header.flags=header.e_flags;
        section_count=header.e_shnum;
        program_header_count=header.e_phnum;
        program_header_offset=header.e_phoff;
        section_header_offset=header.e_shoff;
        program_header_entry_size=header.e_phentsize;
        section_header_entry_size=header.e_shentsize;
        editor->loaded_section_name_table_index=header.e_shstrndx;
    }
    editor->elfdata=helf_file_input_is_big_endian(input)?HELF_ELFDATA_2_MSB:HELF_ELFDATA_2_LSB;
    if(section_count>editor->section_capacity || program_header_count>editor->segment_capacity)
    {
        return false;
    }
    for(size_t i=0; i<section_count; i++)
    {
        helf_arch_section_info_t info;
        if(!helf_arch_section_info_get(input,i,&info))
        {
            return false;
        }
        helf_output_section_t *section=&editor->sections[i];
        memset(section,0,sizeof(*section));
        size_t name_length=strlen(info.name);
        if(name_length>=sizeof(section->name))
        {
            name_length=sizeof(section->name)-1;
        }
        memcpy(section->name,info.name,name_length);
        section->name[name_length]=0;
        section->type=info.type;
        section->flags=info.flags;
        section->address=info.address;
        section->size=info.size;
        section->link=info.link;
        section->info=info.info;
        section->addralign=info.addralign;
        section->entsize=info.entsize;
        section->present=true;
        section->data=NULL;
        section->data_size=(info.type==HELF_SHT_NOBITS)?0:info.size;
        section->data_capacity=0;
        section->source=input;
        section->source_offset=info.offset;
        section->original_offset=info.offset;
        section->original_size=info.size;
        section->original_name_offset=info.name_offset;
        section->loaded=true;
        section->modified=false;
    }
    editor->section_count=section_count;
    if(editor->section_count==0 && editor->sections!=NULL && editor->section_capacity>0)
    {
        memset(&editor->sections[0],0,sizeof(editor->sections[0]));
        editor->sections[0].type=HELF_SHT_NULL;
        editor->sections[0].present=true;
        editor->section_count=1;
    }
    for(size_t i=0; i<program_header_count; i++)
    {
        helf_arch_segment_info_t info;
        if(!helf_arch_segment_info_get(input,i,&info))
        {
            return false;
        }
        helf_output_segment_t *segment=&editor->segments[i];
        memset(segment,0,sizeof(*segment));
        segment->type=info.type;
        segment->flags=info.flags;
        segment->offset=info.offset;
        segment->vaddr=info.vaddr;
        segment->paddr=info.paddr;
        segment->filesz=info.filesz;
        segment->memsz=info.memsz;
        segment->addralign=info.addralign;
        segment->present=true;
        segment->auto_layout=false;
        segment->loaded=true;
        segment->modified=false;
    }
    editor->segment_count=program_header_count;
    editor->source=input;
    editor->loaded_program_header_offset=program_header_offset;
    editor->loaded_program_header_count=program_header_count;
    editor->loaded_section_header_offset=section_header_offset;
    editor->loaded_section_header_count=section_count;
    editor->loaded_program_header_entry_size=program_header_entry_size;
    editor->loaded_section_header_entry_size=section_header_entry_size;
    editor->preserve_layout=false;
    editor->strict_preserve=false;
    uint64_t source_file_size=0;
    if(program_header_offset!=0 && program_header_entry_size!=0)
    {
        source_file_size=program_header_offset+(uint64_t)program_header_count*program_header_entry_size;
    }
    if(section_header_offset!=0 && section_header_entry_size!=0)
    {
        uint64_t end=section_header_offset+(uint64_t)section_count*section_header_entry_size;
        if(end>source_file_size)
        {
            source_file_size=end;
        }
    }
    for(size_t i=0; i<section_count; i++)
    {
        if(editor->sections[i].type!=HELF_SHT_NOBITS && editor->sections[i].present)
        {
            uint64_t end=editor->sections[i].original_offset+editor->sections[i].original_size;
            if(end>source_file_size)
            {
                source_file_size=end;
            }
        }
    }
    for(size_t i=0; i<program_header_count; i++)
    {
        if(editor->segments[i].present)
        {
            uint64_t end=editor->segments[i].offset+editor->segments[i].filesz;
            if(end>source_file_size)
            {
                source_file_size=end;
            }
        }
    }
    editor->source_file_size=source_file_size;
    editor->laid_out=false;
    return true;
}

bool helf_output_editor_page_size_set(helf_output_editor_t *editor,uint64_t page_size)
{
    if(editor==NULL)
    {
        return false;
    }
    editor->page_size=page_size;
    editor->laid_out=false;
    return true;
}

bool helf_output_editor_preserve_layout_set(helf_output_editor_t *editor,bool preserve)
{
    if(editor==NULL)
    {
        return false;
    }
    editor->preserve_layout=preserve;
    editor->laid_out=false;
    return true;
}

bool helf_output_editor_file_header_set(helf_output_editor_t *editor,const helf_output_file_header_t *header)
{
    if(editor==NULL || header==NULL)
    {
        return false;
    }
    editor->header=*header;
    editor->laid_out=false;
    return true;
}

bool helf_output_editor_file_header_get(helf_output_editor_t *editor,helf_output_file_header_t *header)
{
    if(editor==NULL || header==NULL)
    {
        return false;
    }
    *header=editor->header;
    return true;
}

size_t helf_output_editor_section_count_get(helf_output_editor_t *editor)
{
    if(editor==NULL)
    {
        return 0;
    }
    return editor->section_count;
}

intptr_t helf_output_editor_section_add(helf_output_editor_t *editor,const helf_output_section_t *section)
{
    if(editor==NULL || section==NULL || editor->sections==NULL)
    {
        return -1;
    }
    if(editor->section_count>=editor->section_capacity)
    {
        return -1;
    }
    editor->sections[editor->section_count]=*section;
    editor->sections[editor->section_count].name[sizeof(editor->sections[editor->section_count].name)-1]=0;
    editor->sections[editor->section_count].present=true;
    editor->sections[editor->section_count].modified=true;
    editor->sections[editor->section_count].file_offset=0;
    editor->sections[editor->section_count].name_offset=0;
    editor->laid_out=false;
    return (intptr_t)editor->section_count++;
}

bool helf_output_editor_section_set(helf_output_editor_t *editor,size_t index,const helf_output_section_t *section)
{
    if(editor==NULL || section==NULL || index>=editor->section_count)
    {
        return false;
    }
    editor->sections[index]=*section;
    editor->sections[index].name[sizeof(editor->sections[index].name)-1]=0;
    editor->sections[index].modified=true;
    editor->sections[index].file_offset=0;
    editor->sections[index].name_offset=0;
    editor->laid_out=false;
    return true;
}

bool helf_output_editor_section_get(helf_output_editor_t *editor,size_t index,helf_output_section_t *section)
{
    if(editor==NULL || section==NULL || index>=editor->section_count)
    {
        return false;
    }
    *section=editor->sections[index];
    return true;
}

bool helf_output_editor_section_delete(helf_output_editor_t *editor,size_t index)
{
    if(editor==NULL || index>=editor->section_count)
    {
        return false;
    }
    memset(&editor->sections[index],0,sizeof(editor->sections[index]));
    editor->sections[index].type=HELF_SHT_NULL;
    editor->sections[index].present=false;
    editor->sections[index].modified=true;
    editor->laid_out=false;
    return true;
}

intptr_t helf_output_editor_section_find(helf_output_editor_t *editor,const char *name)
{
    if(editor==NULL || name==NULL)
    {
        return -1;
    }
    for(size_t i=0; i<editor->section_count; i++)
    {
        if(editor->sections[i].present && strcmp(editor->sections[i].name,name)==0)
        {
            return (intptr_t)i;
        }
    }
    return -1;
}

bool helf_output_editor_section_data_set(helf_output_editor_t *editor,size_t index,void *data,uint64_t size,uint64_t capacity)
{
    if(editor==NULL || index>=editor->section_count)
    {
        return false;
    }
    editor->sections[index].data=data;
    editor->sections[index].data_size=size;
    editor->sections[index].data_capacity=capacity;
    editor->sections[index].size=size;
    editor->sections[index].modified=true;
    editor->laid_out=false;
    return true;
}

size_t helf_output_editor_section_data_get(helf_output_editor_t *editor,size_t index,void *buffer,size_t buffer_length)
{
    if(editor==NULL || buffer==NULL || index>=editor->section_count)
    {
        return 0;
    }
    helf_output_section_t *section=&editor->sections[index];
    if(section->type==HELF_SHT_NOBITS)
    {
        size_t length=(section->size<buffer_length)?(size_t)section->size:buffer_length;
        memset(buffer,0,length);
        return length;
    }
    if(section->data==NULL)
    {
        return 0;
    }
    uint64_t length=section->data_size;
    if(length>section->size)
    {
        length=section->size;
    }
    if(length>buffer_length)
    {
        length=buffer_length;
    }
    memcpy(buffer,section->data,(size_t)length);
    return (size_t)length;
}

size_t helf_output_editor_segment_count_get(helf_output_editor_t *editor)
{
    if(editor==NULL)
    {
        return 0;
    }
    return editor->segment_count;
}

intptr_t helf_output_editor_segment_add(helf_output_editor_t *editor,const helf_output_segment_t *segment)
{
    if(editor==NULL || segment==NULL || editor->segments==NULL)
    {
        return -1;
    }
    if(editor->segment_count>=editor->segment_capacity)
    {
        return -1;
    }
    editor->segments[editor->segment_count]=*segment;
    editor->segments[editor->segment_count].present=true;
    editor->segments[editor->segment_count].loaded=false;
    editor->segments[editor->segment_count].modified=true;
    editor->laid_out=false;
    return (intptr_t)editor->segment_count++;
}

bool helf_output_editor_segment_set(helf_output_editor_t *editor,size_t index,const helf_output_segment_t *segment)
{
    if(editor==NULL || segment==NULL || index>=editor->segment_count)
    {
        return false;
    }
    editor->segments[index]=*segment;
    editor->segments[index].modified=true;
    editor->laid_out=false;
    return true;
}

bool helf_output_editor_segment_get(helf_output_editor_t *editor,size_t index,helf_output_segment_t *segment)
{
    if(editor==NULL || segment==NULL || index>=editor->segment_count)
    {
        return false;
    }
    *segment=editor->segments[index];
    return true;
}

bool helf_output_editor_segment_delete(helf_output_editor_t *editor,size_t index)
{
    if(editor==NULL || index>=editor->segment_count)
    {
        return false;
    }
    memset(&editor->segments[index],0,sizeof(editor->segments[index]));
    editor->segments[index].type=HELF_PT_NULL;
    editor->segments[index].present=false;
    editor->segments[index].modified=true;
    editor->laid_out=false;
    return true;
}

bool helf_output_editor_segment_from_section(helf_output_editor_t *editor,size_t index,size_t section_index,uint64_t section_offset)
{
    if(editor==NULL || index>=editor->segment_count || section_index>=editor->section_count)
    {
        return false;
    }
    editor->segments[index].auto_layout=true;
    editor->segments[index].section_index=(uint32_t)section_index;
    editor->segments[index].section_offset=section_offset;
    editor->segments[index].modified=true;
    if(editor->segments[index].type==0)
    {
        editor->segments[index].type=HELF_PT_LOAD;
    }
    editor->laid_out=false;
    return true;
}

bool helf_output_editor_string_add(helf_output_editor_t *editor,size_t section_index,const char *string,uint32_t *offset)
{
    if(editor==NULL || section_index>=editor->section_count)
    {
        return false;
    }
    helf_output_section_t *section=&editor->sections[section_index];
    if(!section->present || section->type!=HELF_SHT_STRTAB || section->data==NULL || section->data_capacity==0)
    {
        return false;
    }
    uint8_t *data=(uint8_t *)section->data;
    if(section->data_size==0)
    {
        if(section->data_capacity<1)
        {
            return false;
        }
        data[0]=0;
        section->data_size=1;
        section->size=1;
    }
    if(string==NULL || string[0]==0)
    {
        if(offset!=NULL)
        {
            *offset=0;
        }
        return true;
    }
    size_t string_length=strlen(string)+1;
    if(section->data_size+string_length>section->data_capacity)
    {
        return false;
    }
    if(section->data_size>0xFFFFFFFFULL)
    {
        return false;
    }
    if(offset!=NULL)
    {
        *offset=(uint32_t)section->data_size;
    }
    memcpy(&data[section->data_size],string,string_length);
    section->data_size+=string_length;
    section->size=section->data_size;
    section->modified=true;
    editor->laid_out=false;
    return true;
}

bool helf_output_editor_symbol_add(helf_output_editor_t *editor,size_t section_index,const helf_arch_symbol_info_t *symbol,uint32_t *symbol_index)
{
    if(editor==NULL || symbol==NULL || section_index>=editor->section_count)
    {
        return false;
    }
    helf_output_section_t *section=&editor->sections[section_index];
    if(!section->present || (section->type!=HELF_SHT_SYMTAB && section->type!=HELF_SHT_DYNSYM))
    {
        return false;
    }
    bool is_64_bits=(editor->elfclass==HELF_ELFCLASS_64);
    uint64_t default_entsize=is_64_bits?24:16;
    uint64_t entsize=section->entsize;
    if(entsize==0)
    {
        entsize=default_entsize;
    }
    if(entsize<default_entsize || section->data==NULL || section->data_size+entsize>section->data_capacity)
    {
        return false;
    }
    if(section->data_size%entsize!=0)
    {
        return false;
    }
    uint32_t st_name=0;
    if(symbol->name[0]!=0)
    {
        char name_buffer[HELF_ARCH_SYMBOL_NAME_MAX+1];
        size_t name_length=0;
        while(name_length<HELF_ARCH_SYMBOL_NAME_MAX && symbol->name[name_length]!=0)
        {
            name_length++;
        }
        memcpy(name_buffer,symbol->name,name_length);
        name_buffer[name_length]=0;
        if(!helf_output_editor_string_add(editor,section->link,name_buffer,&st_name))
        {
            return false;
        }
    }
    bool big_endian=helf_output_is_big_endian(editor);
    uint8_t *data=(uint8_t *)section->data;
    uint8_t *entry=&data[section->data_size];
    memset(entry,0,(size_t)entsize);
    if(is_64_bits)
    {
        helf_output_store_uint32(&entry[0],st_name,big_endian);
        entry[4]=symbol->info;
        entry[5]=symbol->other;
        helf_output_store_uint16(&entry[6],symbol->shndx,big_endian);
        helf_output_store_uint64(&entry[8],symbol->value,big_endian);
        helf_output_store_uint64(&entry[16],symbol->size,big_endian);
    }
    else
    {
        helf_output_store_uint32(&entry[0],st_name,big_endian);
        helf_output_store_uint32(&entry[4],(uint32_t)symbol->value,big_endian);
        helf_output_store_uint32(&entry[8],(uint32_t)symbol->size,big_endian);
        entry[12]=symbol->info;
        entry[13]=symbol->other;
        helf_output_store_uint16(&entry[14],symbol->shndx,big_endian);
    }
    if(symbol_index!=NULL)
    {
        uint64_t index=section->data_size/entsize;
        if(index>0xFFFFFFFFULL)
        {
            return false;
        }
        *symbol_index=(uint32_t)index;
    }
    section->data_size+=entsize;
    section->size=section->data_size;
    section->modified=true;
    editor->laid_out=false;
    return true;
}

bool helf_output_editor_relocation_add(helf_output_editor_t *editor,size_t section_index,const helf_arch_relocation_info_t *relocation)
{
    if(editor==NULL || relocation==NULL || section_index>=editor->section_count)
    {
        return false;
    }
    helf_output_section_t *section=&editor->sections[section_index];
    if(!section->present || (section->type!=HELF_SHT_REL && section->type!=HELF_SHT_RELA))
    {
        return false;
    }
    bool is_64_bits=(editor->elfclass==HELF_ELFCLASS_64);
    bool has_addend=(section->type==HELF_SHT_RELA);
    uint64_t default_entsize=0;
    if(is_64_bits)
    {
        default_entsize=has_addend?24:16;
    }
    else
    {
        default_entsize=has_addend?12:8;
    }
    uint64_t entsize=section->entsize;
    if(entsize==0)
    {
        entsize=default_entsize;
    }
    if(entsize<default_entsize || section->data==NULL || section->data_size+entsize>section->data_capacity)
    {
        return false;
    }
    if(section->data_size%entsize!=0)
    {
        return false;
    }
    bool big_endian=helf_output_is_big_endian(editor);
    uint8_t *data=(uint8_t *)section->data;
    uint8_t *entry=&data[section->data_size];
    memset(entry,0,(size_t)entsize);
    if(is_64_bits)
    {
        uint64_t info=0;
        if(relocation->type2!=0 || relocation->type3!=0 || relocation->special_symbol!=0)
        {
            info=((uint64_t)relocation->symbol << 32)
                 | ((uint64_t)(relocation->special_symbol & 0xFF) << 24)
                 | ((uint64_t)(relocation->type3 & 0xFF) << 16)
                 | ((uint64_t)(relocation->type2 & 0xFF) << 8)
                 | ((uint64_t)(relocation->type & 0xFF));
        }
        else
        {
            info=((uint64_t)relocation->symbol << 32) | (uint64_t)relocation->type;
        }
        helf_output_store_uint64(&entry[0],relocation->offset,big_endian);
        helf_output_store_uint64(&entry[8],info,big_endian);
        if(has_addend)
        {
            helf_output_store_uint64(&entry[16],(uint64_t)relocation->addend,big_endian);
        }
    }
    else
    {
        if(relocation->symbol>0xFFFFFFUL || relocation->type>0xFFUL)
        {
            return false;
        }
        uint32_t info=((uint32_t)relocation->symbol << 8) | ((uint32_t)relocation->type & 0xFF);
        helf_output_store_uint32(&entry[0],(uint32_t)relocation->offset,big_endian);
        helf_output_store_uint32(&entry[4],info,big_endian);
        if(has_addend)
        {
            helf_output_store_uint32(&entry[8],(uint32_t)relocation->addend,big_endian);
        }
    }
    section->data_size+=entsize;
    section->size=section->data_size;
    section->modified=true;
    editor->laid_out=false;
    return true;
}

bool helf_output_editor_dynamic_add(helf_output_editor_t *editor,size_t section_index,int64_t tag,uint64_t value)
{
    if(editor==NULL || section_index>=editor->section_count)
    {
        return false;
    }
    helf_output_section_t *section=&editor->sections[section_index];
    if(!section->present || section->type!=HELF_SHT_DYNAMIC)
    {
        return false;
    }
    bool is_64_bits=(editor->elfclass==HELF_ELFCLASS_64);
    uint64_t default_entsize=is_64_bits?16:8;
    uint64_t entsize=section->entsize;
    if(entsize==0)
    {
        entsize=default_entsize;
    }
    if(entsize<default_entsize || section->data==NULL || section->data_size+entsize>section->data_capacity)
    {
        return false;
    }
    if(section->data_size%entsize!=0)
    {
        return false;
    }
    bool big_endian=helf_output_is_big_endian(editor);
    uint8_t *data=(uint8_t *)section->data;
    uint8_t *entry=&data[section->data_size];
    memset(entry,0,(size_t)entsize);
    if(is_64_bits)
    {
        helf_output_store_uint64(&entry[0],(uint64_t)tag,big_endian);
        helf_output_store_uint64(&entry[8],value,big_endian);
    }
    else
    {
        helf_output_store_uint32(&entry[0],(uint32_t)tag,big_endian);
        helf_output_store_uint32(&entry[4],(uint32_t)value,big_endian);
    }
    section->data_size+=entsize;
    section->size=section->data_size;
    section->modified=true;
    editor->laid_out=false;
    return true;
}

static bool helf_output_editor_layout_compute(helf_output_editor_t *editor)
{
    if(editor==NULL || editor->output==NULL)
    {
        return false;
    }
    if(editor->elfclass!=HELF_ELFCLASS_32 && editor->elfclass!=HELF_ELFCLASS_64)
    {
        return false;
    }
    if(editor->elfdata!=HELF_ELFDATA_2_LSB && editor->elfdata!=HELF_ELFDATA_2_MSB)
    {
        return false;
    }
    if(editor->section_capacity>0 && editor->sections==NULL)
    {
        return false;
    }
    if(editor->segment_capacity>0 && editor->segments==NULL)
    {
        return false;
    }
    while(editor->section_count>0 && !editor->sections[editor->section_count-1].present)
    {
        editor->section_count--;
    }
    bool is_64_bits=(editor->elfclass==HELF_ELFCLASS_64);
    uint64_t ehsize=is_64_bits?64:52;
    uint64_t phentsize=is_64_bits?56:32;
    uint64_t shentsize=is_64_bits?64:40;
    bool no_changes=editor->preserve_layout && editor->source!=NULL;
    if(no_changes)
    {
        if(editor->section_count!=editor->loaded_section_header_count)
        {
            no_changes=false;
        }
        if(editor->loaded_section_header_entry_size!=shentsize)
        {
            no_changes=false;
        }
        if(editor->loaded_program_header_count>0 && editor->loaded_program_header_entry_size!=phentsize)
        {
            no_changes=false;
        }
        for(size_t i=0; no_changes && i<editor->section_count; i++)
        {
            if(!editor->sections[i].present || !editor->sections[i].loaded || editor->sections[i].modified)
            {
                no_changes=false;
            }
        }
        for(size_t i=0; no_changes && i<editor->segment_count; i++)
        {
            if(!editor->segments[i].present || !editor->segments[i].loaded || editor->segments[i].modified)
            {
                no_changes=false;
            }
        }
    }
    intptr_t shstr_index=-1;
    uint64_t shstr_size=0;
    if(no_changes)
    {
        editor->strict_preserve=true;
        uint16_t index=editor->loaded_section_name_table_index;
        if(index<editor->section_count && editor->sections[index].present)
        {
            shstr_index=(intptr_t)index;
            shstr_size=editor->sections[index].size;
        }
        for(size_t i=0; i<editor->section_count; i++)
        {
            editor->sections[i].name_offset=editor->sections[i].original_name_offset;
            editor->sections[i].file_offset=editor->sections[i].original_offset;
        }
        editor->section_name_table_index=editor->loaded_section_name_table_index;
    }
    else
    {
        editor->strict_preserve=false;
        shstr_index=helf_output_editor_section_find(editor,".shstrtab");
        if(shstr_index<0)
        {
            helf_output_section_t section;
            memset(&section,0,sizeof(section));
            const char *name=".shstrtab";
            memcpy(section.name,name,strlen(name)+1);
            section.type=HELF_SHT_STRTAB;
            section.addralign=1;
            section.present=true;
            shstr_index=helf_output_editor_section_add(editor,&section);
            if(shstr_index<0)
            {
                return false;
            }
        }
        if(editor->sections[shstr_index].type!=HELF_SHT_STRTAB)
        {
            return false;
        }
        editor->section_name_table_index=(uint16_t)shstr_index;
        uint64_t name_offset=1;
        for(size_t i=0; i<editor->section_count; i++)
        {
            if(!editor->sections[i].present)
            {
                editor->sections[i].name_offset=0;
                continue;
            }
            if(editor->sections[i].name[0]!=0)
            {
                editor->sections[i].name_offset=(uint32_t)name_offset;
                name_offset+=(uint64_t)strlen(editor->sections[i].name)+1;
            }
            else
            {
                editor->sections[i].name_offset=0;
            }
        }
        shstr_size=name_offset;
        editor->sections[shstr_index].size=shstr_size;
        editor->sections[shstr_index].data_size=shstr_size;
    }
    size_t program_header_count=0;
    for(size_t i=0; i<editor->segment_count; i++)
    {
        if(editor->segments[i].present)
        {
            program_header_count++;
        }
    }
    if(editor->section_count>0xFFFF || program_header_count>0xFFFF || (uint64_t)shstr_index>0xFFFF)
    {
        return false;
    }
    uint64_t program_header_offset=0;
    uint64_t section_header_offset=0;
    uint64_t file_size=0;
    if(editor->strict_preserve)
    {
        if(program_header_count!=editor->loaded_program_header_count || editor->section_count!=editor->loaded_section_header_count)
        {
            return false;
        }
        if(program_header_count>0)
        {
            program_header_offset=editor->loaded_program_header_offset;
        }
        section_header_offset=editor->loaded_section_header_offset;
        file_size=editor->source_file_size;
    }
    else if(editor->preserve_layout)
    {
        uint64_t current=editor->source_file_size;
        for(size_t si=0; si<editor->segment_count; si++)
        {
            helf_output_segment_t *segment=&editor->segments[si];
            if(!segment->present || segment->type!=HELF_PT_LOAD || segment->auto_layout || segment->copy_appended)
            {
                continue;
            }
            bool has_modified=false;
            for(size_t i=0; i<editor->section_count; i++)
            {
                helf_output_section_t *section=&editor->sections[i];
                if(!section->present || section->type==HELF_SHT_NULL || !section->loaded || !section->modified || section->segment_copied)
                {
                    continue;
                }
                if((section->flags&HELF_SHF_ALLOC)==0)
                {
                    continue;
                }
                if(section->type==HELF_SHT_NOBITS)
                {
                    if(section->address>=segment->vaddr && (section->address-segment->vaddr)<segment->memsz)
                    {
                        has_modified=true;
                        break;
                    }
                    continue;
                }
                if(section->original_offset<segment->offset)
                {
                    continue;
                }
                if(section->original_offset>=segment->offset+segment->filesz)
                {
                    continue;
                }
                has_modified=true;
                break;
            }
            if(!has_modified)
            {
                continue;
            }
            uint64_t segment_original_offset=segment->offset;
            uint64_t segment_original_filesz=segment->filesz;
            uint64_t copy_filesz=segment->filesz;
            uint64_t copy_memsz=segment->memsz;
            for(size_t i=0; i<editor->section_count; i++)
            {
                helf_output_section_t *section=&editor->sections[i];
                if(!section->present || section->type==HELF_SHT_NULL || !section->loaded)
                {
                    continue;
                }
                if((section->flags&HELF_SHF_ALLOC)==0)
                {
                    continue;
                }
                if(section->type==HELF_SHT_NOBITS)
                {
                    if(section->address<segment->vaddr)
                    {
                        continue;
                    }
                    uint64_t virtual_delta=section->address-segment->vaddr;
                    if(virtual_delta>=segment->memsz)
                    {
                        continue;
                    }
                    uint64_t memory_end=virtual_delta+section->size;
                    if(memory_end>copy_memsz)
                    {
                        copy_memsz=memory_end;
                    }
                    continue;
                }
                if(section->original_offset<segment_original_offset)
                {
                    continue;
                }
                uint64_t file_delta=section->original_offset-segment_original_offset;
                if(file_delta>=segment_original_filesz)
                {
                    continue;
                }
                uint64_t file_end=file_delta+section->size;
                if(file_end>copy_filesz)
                {
                    copy_filesz=file_end;
                }
                if(section->address>=segment->vaddr)
                {
                    uint64_t virtual_delta=section->address-segment->vaddr;
                    uint64_t memory_end=virtual_delta+section->size;
                    if(memory_end>copy_memsz)
                    {
                        copy_memsz=memory_end;
                    }
                }
            }
            uint64_t new_offset=helf_output_segment_offset(editor,current,segment->vaddr);
            for(size_t i=0; i<editor->section_count; i++)
            {
                helf_output_section_t *section=&editor->sections[i];
                if(!section->present || section->type==HELF_SHT_NULL || !section->loaded || section->segment_copied)
                {
                    continue;
                }
                if((section->flags&HELF_SHF_ALLOC)==0)
                {
                    continue;
                }
                if(section->type==HELF_SHT_NOBITS)
                {
                    if(section->address<segment->vaddr)
                    {
                        continue;
                    }
                    uint64_t virtual_delta=section->address-segment->vaddr;
                    if(virtual_delta>=segment->memsz)
                    {
                        continue;
                    }
                    section->file_offset=new_offset+virtual_delta;
                    section->segment_copied=true;
                    continue;
                }
                if(section->original_offset<segment_original_offset)
                {
                    continue;
                }
                uint64_t file_delta=section->original_offset-segment_original_offset;
                if(file_delta>=segment_original_filesz)
                {
                    continue;
                }
                section->file_offset=new_offset+file_delta;
                section->segment_copied=true;
            }
            segment->original_offset=segment_original_offset;
            segment->original_filesz=segment_original_filesz;
            segment->copy_appended=true;
            segment->offset=new_offset;
            segment->filesz=copy_filesz;
            segment->memsz=copy_memsz;
            current=new_offset+copy_filesz;
        }
        bool need_new_program_header_table=false;
        if(program_header_count>0)
        {
            if(program_header_count>editor->loaded_program_header_count || editor->loaded_program_header_offset==0)
            {
                need_new_program_header_table=true;
            }
            else
            {
                program_header_offset=editor->loaded_program_header_offset;
            }
        }
        for(size_t i=0; i<editor->section_count; i++)
        {
            helf_output_section_t *section=&editor->sections[i];
            if(!section->present || section->type==HELF_SHT_NULL)
            {
                section->file_offset=0;
                continue;
            }
            if(section->segment_copied)
            {
                if(section->type!=HELF_SHT_NOBITS)
                {
                    uint64_t end=section->file_offset+section->size;
                    if(end>current)
                    {
                        current=end;
                    }
                }
                else if(section->file_offset>current)
                {
                    current=section->file_offset;
                }
                continue;
            }
            bool keep_offset=false;
            if((size_t)shstr_index!=i && section->loaded && !section->modified && section->original_offset!=0)
            {
                keep_offset=true;
            }
            if(keep_offset)
            {
                section->file_offset=section->original_offset;
                if(section->type!=HELF_SHT_NOBITS)
                {
                    uint64_t end=section->file_offset+section->size;
                    if(end>current)
                    {
                        current=end;
                    }
                }
                else if(section->file_offset>current)
                {
                    current=section->file_offset;
                }
            }
            else
            {
                uint64_t offset=helf_output_section_offset(editor,current,section);
                section->file_offset=offset;
                if(section->type!=HELF_SHT_NOBITS)
                {
                    current=offset+section->size;
                }
                else
                {
                    current=offset;
                }
            }
        }
        for(size_t si=0; si<editor->segment_count; si++)
        {
            helf_output_segment_t *segment=&editor->segments[si];
            if(!segment->present || segment->type==HELF_PT_LOAD || segment->type==HELF_PT_NULL
               || segment->auto_layout || segment->filesz==0)
            {
                continue;
            }
            for(size_t i=0; i<editor->section_count; i++)
            {
                helf_output_section_t *section=&editor->sections[i];
                if(!section->present || section->type==HELF_SHT_NULL || section->type==HELF_SHT_NOBITS || !section->loaded)
                {
                    continue;
                }
                if(section->address==0 || segment->vaddr<section->address)
                {
                    continue;
                }
                uint64_t virtual_delta=segment->vaddr-section->address;
                if(virtual_delta>=section->size)
                {
                    continue;
                }
                segment->offset=section->file_offset+virtual_delta;
                segment->original_offset=segment->offset;
                segment->original_filesz=segment->filesz;
                segment->copy_appended=false;
                break;
            }
        }
        section_header_offset=helf_output_align_up(current,8);
        current=section_header_offset+editor->section_count*shentsize;
        if(need_new_program_header_table)
        {
            program_header_offset=helf_output_align_up(current,8);
            current=program_header_offset+program_header_count*phentsize;
        }
        file_size=current;
    }
    else
    {
        for(size_t i=0; i<editor->section_count; i++)
        {
            editor->sections[i].segment_copied=false;
        }
        for(size_t i=0; i<editor->segment_count; i++)
        {
            editor->segments[i].copy_appended=false;
        }
        if(program_header_count>0)
        {
            program_header_offset=helf_output_align_up(ehsize,is_64_bits?8:4);
        }
        uint64_t current=program_header_offset+program_header_count*phentsize;
        if(program_header_count==0)
        {
            current=ehsize;
        }
        for(size_t si=0; si<editor->segment_count; si++)
        {
            helf_output_segment_t *segment=&editor->segments[si];
            if(!segment->present || !segment->loaded || segment->type!=HELF_PT_LOAD || segment->auto_layout)
            {
                continue;
            }
            uint64_t base=0;
            if(segment->original_offset!=0 || segment->offset!=0)
            {
                base=helf_output_segment_offset(editor,current,segment->vaddr);
            }
            bool assigned_any=false;
            uint64_t max_file_delta=0;
            uint64_t max_mem_delta=0;
            for(;;)
            {
                helf_output_section_t *best=NULL;
                for(size_t i=0; i<editor->section_count; i++)
                {
                    helf_output_section_t *section=&editor->sections[i];
                    if(!section->present || section->type==HELF_SHT_NULL || section->segment_copied)
                    {
                        continue;
                    }
                    if((section->flags&HELF_SHF_ALLOC)==0)
                    {
                        continue;
                    }
                    if(section->address<segment->vaddr)
                    {
                        continue;
                    }
                    uint64_t virtual_delta=section->address-segment->vaddr;
                    if(segment->memsz!=0 && virtual_delta>=segment->memsz)
                    {
                        continue;
                    }
                    if(best==NULL || section->address<best->address)
                    {
                        best=section;
                    }
                }
                if(best==NULL)
                {
                    break;
                }
                uint64_t virtual_delta=best->address-segment->vaddr;
                best->file_offset=base+virtual_delta;
                best->segment_copied=true;
                assigned_any=true;
                uint64_t memory_end=virtual_delta+best->size;
                if(memory_end>max_mem_delta)
                {
                    max_mem_delta=memory_end;
                }
                if(best->type!=HELF_SHT_NOBITS)
                {
                    uint64_t file_end=virtual_delta+best->size;
                    if(file_end>max_file_delta)
                    {
                        max_file_delta=file_end;
                    }
                }
            }
            if(assigned_any)
            {
                segment->offset=base;
                segment->filesz=max_file_delta;
                if(max_mem_delta>segment->memsz)
                {
                    segment->memsz=max_mem_delta;
                }
                if(base+max_file_delta>current)
                {
                    current=base+max_file_delta;
                }
            }
        }
        for(size_t i=0; i<editor->section_count; i++)
        {
            helf_output_section_t *section=&editor->sections[i];
            if(!section->present || section->type==HELF_SHT_NULL)
            {
                section->file_offset=0;
                continue;
            }
            if(section->segment_copied)
            {
                if(section->type!=HELF_SHT_NOBITS)
                {
                    uint64_t end=section->file_offset+section->size;
                    if(end>current)
                    {
                        current=end;
                    }
                }
                else if(section->file_offset>current)
                {
                    current=section->file_offset;
                }
                continue;
            }
            uint64_t offset=helf_output_section_offset(editor,current,section);
            section->file_offset=offset;
            if(section->type==HELF_SHT_NOBITS)
            {
                current=offset;
                continue;
            }
            if((size_t)shstr_index==i)
            {
                section->size=shstr_size;
            }
            current=offset+section->size;
        }
        for(size_t si=0; si<editor->segment_count; si++)
        {
            helf_output_segment_t *segment=&editor->segments[si];
            if(!segment->present || !segment->loaded || segment->type==HELF_PT_LOAD
               || segment->type==HELF_PT_NULL || segment->auto_layout)
            {
                continue;
            }
            for(size_t i=0; i<editor->section_count; i++)
            {
                helf_output_section_t *section=&editor->sections[i];
                if(!section->present || section->type==HELF_SHT_NULL || section->type==HELF_SHT_NOBITS)
                {
                    continue;
                }
                if((section->flags&HELF_SHF_ALLOC)==0)
                {
                    continue;
                }
                if(section->address==0 || segment->vaddr<section->address)
                {
                    continue;
                }
                uint64_t virtual_delta=segment->vaddr-section->address;
                if(virtual_delta>=section->size)
                {
                    continue;
                }
                segment->offset=section->file_offset+virtual_delta;
                break;
            }
        }
        if(editor->section_count>0)
        {
            section_header_offset=helf_output_align_up(current,8);
        }
        file_size=section_header_offset+editor->section_count*shentsize;
        bool program_header_overlap=false;
        if(program_header_count>0 && program_header_offset<file_size)
        {
            uint64_t program_header_end=program_header_offset+program_header_count*phentsize;
            for(size_t i=0; i<editor->section_count; i++)
            {
                helf_output_section_t *section=&editor->sections[i];
                if(!section->present || section->type==HELF_SHT_NULL || section->file_offset==0)
                {
                    continue;
                }
                uint64_t section_end=section->file_offset+((section->type==HELF_SHT_NOBITS)?0:section->size);
                if(section->file_offset<program_header_end && section_end>program_header_offset)
                {
                    program_header_overlap=true;
                    break;
                }
            }
        }
        if(program_header_overlap)
        {
            program_header_offset=helf_output_align_up(file_size,8);
            file_size=program_header_offset+program_header_count*phentsize;
            for(size_t i=0; i<editor->segment_count; i++)
            {
                helf_output_segment_t *segment=&editor->segments[i];
                if(segment->present && segment->type==HELF_PT_PHDR)
                {
                    segment->offset=program_header_offset;
                    segment->vaddr=program_header_offset;
                    segment->paddr=program_header_offset;
                    segment->filesz=program_header_count*phentsize;
                    segment->memsz=program_header_count*phentsize;
                    segment->addralign=8;
                }
            }
        }
    }
    if(!is_64_bits)
    {
        if(program_header_offset>0xFFFFFFFFULL || section_header_offset>0xFFFFFFFFULL || file_size>0xFFFFFFFFULL)
        {
            return false;
        }
        for(size_t i=0; i<editor->section_count; i++)
        {
            helf_output_section_t *section=&editor->sections[i];
            if(!section->present || section->type==HELF_SHT_NULL)
            {
                continue;
            }
            if(section->file_offset>0xFFFFFFFFULL || section->size>0xFFFFFFFFULL || section->address>0xFFFFFFFFULL
               || section->addralign>0xFFFFFFFFULL || section->entsize>0xFFFFFFFFULL)
            {
                return false;
            }
        }
    }
    for(size_t i=0; i<editor->segment_count; i++)
    {
        helf_output_segment_t *segment=&editor->segments[i];
        if(!segment->present || !segment->auto_layout)
        {
            continue;
        }
        if(segment->section_index>=editor->section_count)
        {
            continue;
        }
        helf_output_section_t *section=&editor->sections[segment->section_index];
        if(!section->present || section->type==HELF_SHT_NULL)
        {
            continue;
        }
        if(segment->section_offset>section->size)
        {
            continue;
        }
        segment->offset=section->file_offset+segment->section_offset;
        segment->filesz=(section->type==HELF_SHT_NOBITS)?0:(section->size-segment->section_offset);
        segment->memsz=section->size-segment->section_offset;
        if(segment->vaddr==0)
        {
            segment->vaddr=section->address+segment->section_offset;
        }
        if(segment->paddr==0)
        {
            segment->paddr=segment->vaddr;
        }
        if(segment->addralign==0)
        {
            segment->addralign=section->addralign;
            if(segment->addralign==0)
            {
                segment->addralign=1;
            }
        }
        if(segment->type==0)
        {
            segment->type=HELF_PT_LOAD;
        }
    }
    editor->program_header_offset=program_header_offset;
    editor->section_header_offset=section_header_offset;
    editor->program_header_count=(uint16_t)program_header_count;
    editor->section_header_count=(uint16_t)editor->section_count;
    editor->file_size=file_size;
    editor->laid_out=true;
    return true;
}

static bool helf_output_editor_write_internal(helf_output_editor_t *editor)
{
    if(editor==NULL || editor->output==NULL)
    {
        return false;
    }
    if(editor->preserve_layout && editor->source!=NULL)
    {
        uint8_t copy_buffer[128];
        uint64_t remaining=editor->source_file_size;
        uint64_t address=0;
        while(remaining>0)
        {
            size_t chunk=(remaining>sizeof(copy_buffer))?sizeof(copy_buffer):(size_t)remaining;
            if(helf_file_input_read(editor->source,(uintptr_t)address,copy_buffer,chunk)!=chunk)
            {
                return false;
            }
            if(helf_file_output_write(editor->output,(uintptr_t)address,copy_buffer,chunk)!=chunk)
            {
                return false;
            }
            address+=chunk;
            remaining-=chunk;
        }
    }
    if(editor->source!=NULL)
    {
        uint8_t copy_buffer[128];
        uint8_t zero_copy[32];
        memset(zero_copy,0,sizeof(zero_copy));
        for(size_t i=0; i<editor->segment_count; i++)
        {
            helf_output_segment_t *segment=&editor->segments[i];
            if(!segment->present || !segment->copy_appended)
            {
                continue;
            }
            uint64_t copy_length=segment->original_filesz;
            if(copy_length>segment->filesz)
            {
                copy_length=segment->filesz;
            }
            uint64_t source_address=segment->original_offset;
            uint64_t output_address=segment->offset;
            uint64_t remaining=copy_length;
            while(remaining>0)
            {
                size_t chunk=(remaining>sizeof(copy_buffer))?sizeof(copy_buffer):(size_t)remaining;
                if(helf_file_input_read(editor->source,(uintptr_t)source_address,copy_buffer,chunk)!=chunk)
                {
                    return false;
                }
                if(helf_file_output_write(editor->output,(uintptr_t)output_address,copy_buffer,chunk)!=chunk)
                {
                    return false;
                }
                source_address+=chunk;
                output_address+=chunk;
                remaining-=chunk;
            }
            remaining=segment->filesz-copy_length;
            while(remaining>0)
            {
                size_t chunk=(remaining>sizeof(zero_copy))?sizeof(zero_copy):(size_t)remaining;
                if(helf_file_output_write(editor->output,(uintptr_t)output_address,zero_copy,chunk)!=chunk)
                {
                    return false;
                }
                output_address+=chunk;
                remaining-=chunk;
            }
        }
    }
    bool is_64_bits=(editor->elfclass==HELF_ELFCLASS_64);
    bool big_endian=helf_output_is_big_endian(editor);
    (void)big_endian;
    helf_e_ident_t e_ident;
    memset(e_ident,0,sizeof(e_ident));
    e_ident[HELF_EI_MAG0]=0x7F;
    e_ident[HELF_EI_MAG1]='E';
    e_ident[HELF_EI_MAG2]='L';
    e_ident[HELF_EI_MAG3]='F';
    e_ident[HELF_EI_CLASS]=editor->elfclass;
    e_ident[HELF_EI_DATA]=editor->elfdata;
    e_ident[HELF_EI_VERSION]=HELF_EV_CURRENT;
    e_ident[HELF_EI_OSABI]=editor->header.osabi;
    e_ident[HELF_EI_ABIVERSION]=editor->header.abiversion;
    uint32_t version=editor->header.version;
    if(version==0)
    {
        version=HELF_EV_CURRENT;
    }
    if(is_64_bits)
    {
        if(editor->header.entry>0xFFFFFFFFFFFFFFFFULL)
        {
            return false;
        }
        helf_elf64_file_header_t header;
        memset(&header,0,sizeof(header));
        memcpy(header.e_ident,e_ident,sizeof(header.e_ident));
        header.e_type=editor->header.type;
        header.e_machine=editor->header.machine;
        header.e_version=version;
        header.e_entry=editor->header.entry;
        header.e_phoff=editor->program_header_offset;
        header.e_shoff=editor->section_header_offset;
        header.e_flags=editor->header.flags;
        header.e_ehsize=64;
        header.e_phentsize=editor->strict_preserve?editor->loaded_program_header_entry_size:56;
        header.e_phnum=editor->program_header_count;
        header.e_shentsize=editor->strict_preserve?editor->loaded_section_header_entry_size:64;
        header.e_shnum=editor->section_header_count;
        header.e_shstrndx=editor->section_name_table_index;
        if(!helf_file_output_64_bits_header_set(editor->output,&header))
        {
            return false;
        }
    }
    else
    {
        if(editor->header.entry>0xFFFFFFFFULL || editor->program_header_offset>0xFFFFFFFFULL || editor->section_header_offset>0xFFFFFFFFULL)
        {
            return false;
        }
        helf_elf32_file_header_t header;
        memset(&header,0,sizeof(header));
        memcpy(header.e_ident,e_ident,sizeof(header.e_ident));
        header.e_type=editor->header.type;
        header.e_machine=editor->header.machine;
        header.e_version=version;
        header.e_entry=(uint32_t)editor->header.entry;
        header.e_phoff=(uint32_t)editor->program_header_offset;
        header.e_shoff=(uint32_t)editor->section_header_offset;
        header.e_flags=editor->header.flags;
        header.e_ehsize=52;
        header.e_phentsize=editor->strict_preserve?editor->loaded_program_header_entry_size:32;
        header.e_phnum=editor->program_header_count;
        header.e_shentsize=editor->strict_preserve?editor->loaded_section_header_entry_size:40;
        header.e_shnum=editor->section_header_count;
        header.e_shstrndx=editor->section_name_table_index;
        if(!helf_file_output_32_bits_header_set(editor->output,&header))
        {
            return false;
        }
    }
    size_t program_header_index=0;
    for(size_t i=0; i<editor->segment_count; i++)
    {
        helf_output_segment_t *segment=&editor->segments[i];
        if(!segment->present)
        {
            continue;
        }
        if(is_64_bits)
        {
            helf_elf64_program_header_t programheader;
            memset(&programheader,0,sizeof(programheader));
            programheader.p_type=segment->type;
            programheader.p_flags=segment->flags;
            programheader.p_offset=segment->offset;
            programheader.p_vaddr=segment->vaddr;
            programheader.p_paddr=segment->paddr;
            programheader.p_filesz=segment->filesz;
            programheader.p_memsz=segment->memsz;
            programheader.p_align=segment->addralign;
            if(!helf_file_output_64_bits_program_header_set(editor->output,program_header_index,&programheader))
            {
                return false;
            }
        }
        else
        {
            if(segment->offset>0xFFFFFFFFULL || segment->vaddr>0xFFFFFFFFULL || segment->paddr>0xFFFFFFFFULL
               || segment->filesz>0xFFFFFFFFULL || segment->memsz>0xFFFFFFFFULL || segment->addralign>0xFFFFFFFFULL)
            {
                return false;
            }
            helf_elf32_program_header_t programheader;
            memset(&programheader,0,sizeof(programheader));
            programheader.p_type=segment->type;
            programheader.p_flags=segment->flags;
            programheader.p_offset=(uint32_t)segment->offset;
            programheader.p_vaddr=(uint32_t)segment->vaddr;
            programheader.p_paddr=(uint32_t)segment->paddr;
            programheader.p_filesz=(uint32_t)segment->filesz;
            programheader.p_memsz=(uint32_t)segment->memsz;
            programheader.p_align=(uint32_t)segment->addralign;
            if(!helf_file_output_32_bits_program_header_set(editor->output,program_header_index,&programheader))
            {
                return false;
            }
        }
        program_header_index++;
    }
    uint8_t zero_buffer[32];
    memset(zero_buffer,0,sizeof(zero_buffer));
    for(size_t i=0; i<editor->section_count; i++)
    {
        helf_output_section_t *section=&editor->sections[i];
        if(!section->present || section->type==HELF_SHT_NULL || section->type==HELF_SHT_NOBITS)
        {
            continue;
        }
        if(!editor->strict_preserve && (size_t)editor->section_name_table_index==i)
        {
            if(helf_file_output_write(editor->output,(uintptr_t)section->file_offset,zero_buffer,1)!=1)
            {
                return false;
            }
            for(size_t j=0; j<editor->section_count; j++)
            {
                helf_output_section_t *named_section=&editor->sections[j];
                if(!named_section->present || named_section->name[0]==0)
                {
                    continue;
                }
                size_t name_length=strlen(named_section->name)+1;
                if(helf_file_output_write(editor->output,(uintptr_t)(section->file_offset+named_section->name_offset),named_section->name,name_length)!=name_length)
                {
                    return false;
                }
            }
            continue;
        }
        if(section->data!=NULL)
        {
            uint64_t write_length=section->data_size;
            if(write_length>section->size)
            {
                write_length=section->size;
            }
            if(write_length>0)
            {
                if(helf_file_output_write(editor->output,(uintptr_t)section->file_offset,section->data,(size_t)write_length)!=write_length)
                {
                    return false;
                }
            }
            uint64_t remaining=section->size-write_length;
            uint64_t address=section->file_offset+write_length;
            while(remaining>0)
            {
                size_t chunk=(remaining>sizeof(zero_buffer))?sizeof(zero_buffer):(size_t)remaining;
                if(helf_file_output_write(editor->output,(uintptr_t)address,zero_buffer,chunk)!=chunk)
                {
                    return false;
                }
                address+=chunk;
                remaining-=chunk;
            }
        }
        else if(section->source!=NULL)
        {
            uint8_t copy_buffer[64];
            uint64_t copy_length=section->size;
            if(section->loaded && section->original_size<copy_length)
            {
                copy_length=section->original_size;
            }
            uint64_t remaining=copy_length;
            uint64_t source_address=section->source_offset;
            uint64_t output_address=section->file_offset;
            while(remaining>0)
            {
                size_t chunk=(remaining>sizeof(copy_buffer))?sizeof(copy_buffer):(size_t)remaining;
                if(helf_file_input_read(section->source,(uintptr_t)source_address,copy_buffer,chunk)!=chunk)
                {
                    return false;
                }
                if(helf_file_output_write(editor->output,(uintptr_t)output_address,copy_buffer,chunk)!=chunk)
                {
                    return false;
                }
                source_address+=chunk;
                output_address+=chunk;
                remaining-=chunk;
            }
            remaining=section->size-copy_length;
            while(remaining>0)
            {
                size_t chunk=(remaining>sizeof(zero_buffer))?sizeof(zero_buffer):(size_t)remaining;
                if(helf_file_output_write(editor->output,(uintptr_t)output_address,zero_buffer,chunk)!=chunk)
                {
                    return false;
                }
                output_address+=chunk;
                remaining-=chunk;
            }
        }
        else
        {
            uint64_t remaining=section->size;
            uint64_t address=section->file_offset;
            while(remaining>0)
            {
                size_t chunk=(remaining>sizeof(zero_buffer))?sizeof(zero_buffer):(size_t)remaining;
                if(helf_file_output_write(editor->output,(uintptr_t)address,zero_buffer,chunk)!=chunk)
                {
                    return false;
                }
                address+=chunk;
                remaining-=chunk;
            }
        }
    }
    for(size_t i=0; i<editor->section_count; i++)
    {
        helf_output_section_t *section=&editor->sections[i];
        if(!section->present || section->type==HELF_SHT_NULL)
        {
            if(is_64_bits)
            {
                helf_elf64_section_header_t sectionheader;
                memset(&sectionheader,0,sizeof(sectionheader));
                if(!helf_file_output_64_bits_section_header_set(editor->output,i,&sectionheader))
                {
                    return false;
                }
            }
            else
            {
                helf_elf32_section_header_t sectionheader;
                memset(&sectionheader,0,sizeof(sectionheader));
                if(!helf_file_output_32_bits_section_header_set(editor->output,i,&sectionheader))
                {
                    return false;
                }
            }
            continue;
        }
        if(is_64_bits)
        {
            helf_elf64_section_header_t sectionheader;
            memset(&sectionheader,0,sizeof(sectionheader));
            sectionheader.sh_name=section->name_offset;
            sectionheader.sh_type=section->type;
            sectionheader.sh_flags=section->flags;
            sectionheader.sh_addr=section->address;
            sectionheader.sh_offset=section->file_offset;
            sectionheader.sh_size=section->size;
            sectionheader.sh_link=section->link;
            sectionheader.sh_info=section->info;
            sectionheader.sh_addralign=section->addralign;
            sectionheader.sh_entsize=section->entsize;
            if(!helf_file_output_64_bits_section_header_set(editor->output,i,&sectionheader))
            {
                return false;
            }
        }
        else
        {
            if(section->flags>0xFFFFFFFFULL || section->address>0xFFFFFFFFULL || section->file_offset>0xFFFFFFFFULL
               || section->size>0xFFFFFFFFULL || section->addralign>0xFFFFFFFFULL || section->entsize>0xFFFFFFFFULL)
            {
                return false;
            }
            helf_elf32_section_header_t sectionheader;
            memset(&sectionheader,0,sizeof(sectionheader));
            sectionheader.sh_name=section->name_offset;
            sectionheader.sh_type=section->type;
            sectionheader.sh_flags=(uint32_t)section->flags;
            sectionheader.sh_addr=(uint32_t)section->address;
            sectionheader.sh_offset=(uint32_t)section->file_offset;
            sectionheader.sh_size=(uint32_t)section->size;
            sectionheader.sh_link=section->link;
            sectionheader.sh_info=section->info;
            sectionheader.sh_addralign=(uint32_t)section->addralign;
            sectionheader.sh_entsize=(uint32_t)section->entsize;
            if(!helf_file_output_32_bits_section_header_set(editor->output,i,&sectionheader))
            {
                return false;
            }
        }
    }
    return true;
}

uint64_t helf_output_editor_file_size_get(helf_output_editor_t *editor)
{
    if(editor==NULL)
    {
        return 0;
    }
    return editor->file_size;
}

bool helf_output_editor_layout(helf_output_editor_t *editor)
{
    if(!helf_output_editor_layout_compute(editor))
    {
        return false;
    }
    return helf_output_editor_write_internal(editor);
}

bool helf_output_editor_write(helf_output_editor_t *editor)
{
    return helf_output_editor_layout(editor);
}

bool helf_output_editor_source_file_size_set(helf_output_editor_t *editor,uint64_t size)
{
    if(editor==NULL)
    {
        return false;
    }
    editor->source_file_size=size;
    editor->laid_out=false;
    return true;
}

uint64_t helf_output_editor_source_file_size_get(helf_output_editor_t *editor)
{
    if(editor==NULL)
    {
        return 0;
    }
    return editor->source_file_size;
}
