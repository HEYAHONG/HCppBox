/***************************************************************
 * Name:      helf_loader.c
 * Purpose:   实现helf_loader接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-21
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "helf_loader.h"

#define HELF_LOADER_SECTION_ALIGN 16
#ifndef HELF_LOADER_PAGE_ALIGN
#if defined(__sparc__) || defined(__sparc64__)
#define HELF_LOADER_PAGE_ALIGN 0x2000
#else
#define HELF_LOADER_PAGE_ALIGN 0x1000
#endif
#endif
#define HELF_LOADER_RELOC_DEFAULT_ADDRESS 0x10000000U
#define HELF_LOADER_RELOC_ADDRESS_STEP   0x00100000U

#define HELF_PPC64_R_BRANCH_REL24          10
#define HELF_PPC64_R_BRANCH_REL14          11
#define HELF_PPC64_R_BRANCH_REL14_BRTAKEN  12
#define HELF_PPC64_R_BRANCH_REL14_BRNTAKEN 13
#define HELF_PPC64_R_BRANCH_ADDR24         2
#define HELF_PPC64_R_BRANCH_REL24_NOTOC    116
#define HELF_PPC64_R_BRANCH_REL24_P9NOTOC  124

static size_t helf_loader_align_up(size_t value,size_t alignment)
{
    if(alignment<=1)
    {
        return value;
    }
    size_t remainder=value%alignment;
    if(remainder==0)
    {
        return value;
    }
    return value+(alignment-remainder);
}

uint16_t helf_namespace_native_machine_type(void)
{
#if defined(_M_IX86) || defined(__i386__) || defined(i386) || defined(__i686__) || defined(i686)
    return HELF_EM_386;
#elif defined(_M_X64) || defined(__x86_64__) || defined(__x86_64)
    return HELF_EM_X86_64;
#elif defined(_M_ARM64) || defined(__aarch64__) || defined(__AARCH64EL__)
    return HELF_EM_AARCH64;
#elif defined(_M_ARM) || defined(__arm__) || defined(__thumb__) || defined(__ARMEL__)
    return HELF_EM_ARM;
#elif defined(__riscv) && (__riscv_xlen == 64)
    return HELF_EM_RISCV;
#elif defined(__riscv) && (__riscv_xlen == 32)
    return HELF_EM_RISCV;
#elif defined(__loongarch64) || defined(__loongarch__)
    return HELF_EM_LOONGARCH;
#else
    return HELF_EM_NONE;
#endif
}

static bool helf_loader_file_info_get(helf_file_input_t *input_file,uint16_t *machine,uint8_t *elfclass,uint8_t *elfdata,uint16_t *type)
{
    if(input_file==NULL)
    {
        return false;
    }
    if(helf_file_input_is_64_bits(input_file))
    {
        helf_elf64_file_header_t header;
        if(!helf_file_input_64_bits_header_get(input_file,&header))
        {
            return false;
        }
        if(machine!=NULL)
        {
            *machine=header.e_machine;
        }
        if(elfclass!=NULL)
        {
            *elfclass=HELF_ELFCLASS_64;
        }
        if(type!=NULL)
        {
            *type=header.e_type;
        }
        if(elfdata!=NULL)
        {
            *elfdata=helf_file_input_is_big_endian(input_file)?HELF_ELFDATA_2_MSB:HELF_ELFDATA_2_LSB;
        }
        return true;
    }
    if(helf_file_input_is_32_bits(input_file))
    {
        helf_elf32_file_header_t header;
        if(!helf_file_input_32_bits_header_get(input_file,&header))
        {
            return false;
        }
        if(machine!=NULL)
        {
            *machine=header.e_machine;
        }
        if(elfclass!=NULL)
        {
            *elfclass=HELF_ELFCLASS_32;
        }
        if(type!=NULL)
        {
            *type=header.e_type;
        }
        if(elfdata!=NULL)
        {
            *elfdata=helf_file_input_is_big_endian(input_file)?HELF_ELFDATA_2_MSB:HELF_ELFDATA_2_LSB;
        }
        return true;
    }
    return false;
}

uint16_t helf_namespace_file_machine_type(helf_file_input_t *input_file)
{
    uint16_t machine=HELF_EM_NONE;
    helf_loader_file_info_get(input_file,&machine,NULL,NULL,NULL);
    return machine;
}

bool helf_namespace_is_native_file(helf_file_input_t *input_file)
{
    uint16_t native=helf_namespace_native_machine_type();
    return native!=HELF_EM_NONE && helf_namespace_file_machine_type(input_file)==native;
}

static helf_loaded_type_t helf_loader_type_from_elf_type(uint16_t type)
{
    switch(type)
    {
    case HELF_ET_REL:
        return HELF_LOADED_TYPE_RELOCATABLE;
    case HELF_ET_DYN:
        return HELF_LOADED_TYPE_SHARED_OBJECT;
    case HELF_ET_EXEC:
        return HELF_LOADED_TYPE_EXECUTABLE;
    default:
        return HELF_LOADED_TYPE_NONE;
    }
}

static int helf_loader_section_class_get(const helf_arch_section_info_t *info)
{
    if(info==NULL || info->type==HELF_SHT_NULL)
    {
        return 0;
    }
    if((info->flags&HELF_SHF_ALLOC)==0)
    {
        return 0;
    }
    if((info->flags&HELF_SHF_EXECINSTR)!=0)
    {
        return 1;
    }
    if((info->flags&HELF_SHF_WRITE)!=0)
    {
        return 2;
    }
    return 3;
}

static size_t helf_loader_section_alignment_get(const helf_arch_section_info_t *info)
{
    size_t alignment=(size_t)info->addralign;
    if(alignment<HELF_LOADER_SECTION_ALIGN)
    {
        alignment=HELF_LOADER_SECTION_ALIGN;
    }
    return alignment;
}

static size_t helf_loader_region_size_get(helf_file_input_t *input_file,int section_class)
{
    size_t ret=0;
    size_t count=helf_arch_section_count_get(input_file);
    for(size_t i=0; i<count; i++)
    {
        helf_arch_section_info_t info;
        if(!helf_arch_section_info_get(input_file,i,&info))
        {
            return 0;
        }
        if(helf_loader_section_class_get(&info)==section_class)
        {
            ret=helf_loader_align_up(ret,helf_loader_section_alignment_get(&info));
            ret+=(size_t)info.size;
        }
    }
    return ret;
}

static size_t helf_loader_section_offset_get(helf_file_input_t *input_file,size_t index,int section_class)
{
    size_t ret=0;
    size_t count=helf_arch_section_count_get(input_file);
    if(index>count)
    {
        return 0;
    }
    for(size_t i=0; i<index; i++)
    {
        helf_arch_section_info_t info;
        if(!helf_arch_section_info_get(input_file,i,&info))
        {
            return 0;
        }
        if(helf_loader_section_class_get(&info)==section_class)
        {
            ret=helf_loader_align_up(ret,helf_loader_section_alignment_get(&info));
            ret+=(size_t)info.size;
        }
    }
    return ret;
}

static size_t helf_loader_max_alignment_get(helf_file_input_t *input_file)
{
    size_t ret=HELF_LOADER_SECTION_ALIGN;
    size_t count=helf_arch_section_count_get(input_file);
    for(size_t i=0; i<count; i++)
    {
        helf_arch_section_info_t info;
        if(!helf_arch_section_info_get(input_file,i,&info))
        {
            break;
        }
        if(helf_loader_section_class_get(&info)==0)
        {
            continue;
        }
        size_t alignment=(size_t)info.addralign;
        if(alignment>ret)
        {
            ret=alignment;
        }
    }
    if(ret>HELF_LOADER_PAGE_ALIGN)
    {
        ret=HELF_LOADER_PAGE_ALIGN;
    }
    return ret;
}

static void *helf_loader_section_address(helf_loaded_t *loaded,helf_file_input_t *input_file,size_t index)
{
    if(loaded==NULL || input_file==NULL)
    {
        return NULL;
    }
    helf_arch_section_info_t info;
    if(!helf_arch_section_info_get(input_file,index,&info))
    {
        return NULL;
    }
    int section_class=helf_loader_section_class_get(&info);
    void *base=NULL;
    switch(section_class)
    {
    case 1:
        base=loaded->rx_image;
        break;
    case 2:
        base=loaded->rw_image;
        break;
    case 3:
        base=loaded->ro_image;
        break;
    default:
        return NULL;
    }
    if(base==NULL)
    {
        return NULL;
    }
    return (uint8_t *)base+helf_loader_section_offset_get(input_file,index,section_class);
}

static bool helf_loader_segments_range_get(helf_file_input_t *input_file,uintptr_t *min_vaddr,uintptr_t *max_vaddr)
{
    if(input_file==NULL || min_vaddr==NULL || max_vaddr==NULL)
    {
        return false;
    }
    bool found=false;
    uintptr_t min_value=0;
    uintptr_t max_value=0;
    size_t count=helf_arch_program_header_count_get(input_file);
    for(size_t i=0; i<count; i++)
    {
        helf_arch_segment_info_t segment;
        if(!helf_arch_segment_info_get(input_file,i,&segment))
        {
            continue;
        }
        if(segment.type!=HELF_PT_LOAD || segment.memsz==0)
        {
            continue;
        }
        uintptr_t end=(uintptr_t)(segment.vaddr+segment.memsz);
        if(!found)
        {
            min_value=(uintptr_t)segment.vaddr;
            max_value=end;
            found=true;
        }
        else
        {
            if((uintptr_t)segment.vaddr<min_value)
            {
                min_value=(uintptr_t)segment.vaddr;
            }
            if(end>max_value)
            {
                max_value=end;
            }
        }
    }
    if(!found)
    {
        return false;
    }
    *min_vaddr=min_value;
    *max_vaddr=max_value;
    return true;
}

static bool helf_loader_image_size_get(helf_file_input_t *input_file,helf_loaded_type_t type,uint64_t *image_size,size_t *rx_size,size_t *ro_size,size_t *rw_size,uintptr_t *min_vaddr)
{
    if(input_file==NULL || image_size==NULL)
    {
        return false;
    }
    if(type==HELF_LOADED_TYPE_RELOCATABLE)
    {
        size_t rx=helf_loader_region_size_get(input_file,1);
        size_t ro=helf_loader_region_size_get(input_file,3);
        size_t rw=helf_loader_region_size_get(input_file,2);
        if(rx==0 && ro==0 && rw==0)
        {
            return false;
        }
        if(rx_size!=NULL)
        {
            *rx_size=rx;
        }
        if(ro_size!=NULL)
        {
            *ro_size=ro;
        }
        if(rw_size!=NULL)
        {
            *rw_size=rw;
        }
        if(min_vaddr!=NULL)
        {
            *min_vaddr=0;
        }
        *image_size=(uint64_t)rx+(uint64_t)ro+(uint64_t)rw;
        return true;
    }
    uintptr_t min_value=0;
    uintptr_t max_value=0;
    if(!helf_loader_segments_range_get(input_file,&min_value,&max_value) || max_value<=min_value)
    {
        return false;
    }
    if(rx_size!=NULL)
    {
        *rx_size=0;
    }
    if(ro_size!=NULL)
    {
        *ro_size=0;
    }
    if(rw_size!=NULL)
    {
        *rw_size=0;
    }
    if(min_vaddr!=NULL)
    {
        *min_vaddr=min_value;
    }
    *image_size=(uint64_t)(max_value-min_value);
    return true;
}

void helf_load_param_init(helf_load_param_t *param)
{
    if(param!=NULL)
    {
        memset(param,0,sizeof(*param));
    }
}

static bool helf_loader_has_external_relocations(helf_file_input_t *input_file)
{
    size_t section_count=helf_arch_relocation_section_count_get(input_file);
    for(size_t i=0; i<section_count; i++)
    {
        helf_arch_relocation_section_info_t section;
        if(!helf_arch_relocation_section_info_get(input_file,i,&section))
        {
            continue;
        }
        for(size_t j=0; j<section.count; j++)
        {
            helf_arch_relocation_info_t relocation;
            if(!helf_arch_relocation_get(input_file,section.index,j,&relocation))
            {
                break;
            }
            if(relocation.symbol==0)
            {
                continue;
            }
            helf_arch_symbol_info_t symbol;
            if(!helf_arch_relocation_symbol_info_get(input_file,&relocation,&symbol))
            {
                continue;
            }
            if(symbol.shndx==HELF_SHN_UNDEF && symbol.name[0]!='\0')
            {
                return true;
            }
        }
    }
    return false;
}

bool helf_namespace_loaded_param_requirement_get(helf_file_input_t *input_file,helf_load_param_requirement_t *requirement)
{
    if(input_file==NULL || requirement==NULL)
    {
        return false;
    }
    memset(requirement,0,sizeof(*requirement));
    uint16_t machine=HELF_EM_NONE;
    uint8_t elfclass=0;
    uint8_t elfdata=0;
    uint16_t type=0;
    if(!helf_loader_file_info_get(input_file,&machine,&elfclass,&elfdata,&type))
    {
        return false;
    }
    helf_loaded_type_t loaded_type=helf_loader_type_from_elf_type(type);
    if(loaded_type==HELF_LOADED_TYPE_NONE)
    {
        return false;
    }
    uint64_t image_size=0;
    size_t rx_size=0;
    size_t ro_size=0;
    size_t rw_size=0;
    uintptr_t min_vaddr=0;
    if(!helf_loader_image_size_get(input_file,loaded_type,&image_size,&rx_size,&ro_size,&rw_size,&min_vaddr))
    {
        return false;
    }
    requirement->suggested_address=min_vaddr;
    if(loaded_type==HELF_LOADED_TYPE_EXECUTABLE)
    {
        /* ET_EXEC使用固定链接地址,执行时通常必须加载到该地址 */
        requirement->required.load_address=1;
        requirement->required.fixed_address=1;
    }
    if(helf_loader_has_external_relocations(input_file))
    {
        requirement->required.external_symbol=1;
    }
    return true;
}

bool helf_namespace_load_param_set(helf_namespace_t *ns,const helf_load_param_t *param)
{
    if(ns==NULL)
    {
        return false;
    }
    if(param==NULL)
    {
        memset(&ns->load_param,0,sizeof(ns->load_param));
        ns->flags.load_param_set=0;
    }
    else
    {
        ns->load_param=*param;
        ns->flags.load_param_set=1;
    }
    return true;
}

const helf_load_param_t *helf_namespace_load_param_get(helf_namespace_t *ns)
{
    if(ns==NULL || !ns->flags.load_param_set)
    {
        return NULL;
    }
    return &ns->load_param;
}

static bool helf_loader_symbol_is_exported(const helf_arch_symbol_info_t *symbol)
{
    if(symbol==NULL || symbol->name[0]=='\0')
    {
        return false;
    }
    uint8_t bind=HELF_ELF32_ST_BIND(symbol->info);
    uint8_t type=HELF_ELF32_ST_TYPE(symbol->info);
    if(bind!=HELF_STB_GLOBAL && bind!=HELF_STB_WEAK)
    {
        return false;
    }
    if(symbol->shndx==HELF_SHN_UNDEF || symbol->shndx==HELF_SHN_XINDEX)
    {
        return false;
    }
    if(type==HELF_STT_SECTION || type==HELF_STT_FILE || type==HELF_STT_TLS)
    {
        return false;
    }
    return true;
}

static bool helf_loader_symbols_size_get(helf_file_input_t *input_file,size_t *symbol_count,size_t *names_size)
{
    if(input_file==NULL || symbol_count==NULL || names_size==NULL)
    {
        return false;
    }
    size_t count=0;
    size_t names=0;
    size_t section_count=helf_arch_section_count_get(input_file);
    for(size_t i=0; i<section_count; i++)
    {
        helf_arch_section_info_t section;
        if(!helf_arch_section_info_get(input_file,i,&section))
        {
            continue;
        }
        if(section.type!=HELF_SHT_SYMTAB && section.type!=HELF_SHT_DYNSYM)
        {
            continue;
        }
        size_t symbol_count_in_section=helf_arch_symbol_section_count_get(input_file,i);
        for(size_t j=0; j<symbol_count_in_section; j++)
        {
            helf_arch_symbol_info_t symbol;
            if(!helf_arch_symbol_section_info_get(input_file,i,j,&symbol))
            {
                continue;
            }
            if(!helf_loader_symbol_is_exported(&symbol))
            {
                continue;
            }
            count++;
            names+=strlen(symbol.name)+1;
        }
    }
    *symbol_count=count;
    *names_size=names;
    return true;
}

static bool helf_loader_symbol_address_get(helf_loaded_t *loaded,helf_file_input_t *input_file,const helf_arch_symbol_info_t *symbol,uintptr_t *address)
{
    if(loaded==NULL || input_file==NULL || symbol==NULL || address==NULL)
    {
        return false;
    }
    if(symbol->shndx==HELF_SHN_UNDEF || symbol->shndx==HELF_SHN_XINDEX)
    {
        return false;
    }
    if(symbol->shndx==HELF_SHN_ABS)
    {
        *address=(uintptr_t)symbol->value;
        return true;
    }
    if(symbol->shndx==HELF_SHN_COMMON)
    {
        /* 暂不分配COMMON符号 */
        return false;
    }
    if(loaded->type==HELF_LOADED_TYPE_RELOCATABLE)
    {
        if(symbol->shndx>=helf_arch_section_count_get(input_file))
        {
            return false;
        }
        helf_arch_section_info_t section;
        if(!helf_arch_section_info_get(input_file,symbol->shndx,&section))
        {
            return false;
        }
        void *section_address=helf_loader_section_address(loaded,input_file,symbol->shndx);
        if(section_address==NULL)
        {
            return false;
        }
        *address=(uintptr_t)section_address+(uintptr_t)(symbol->value-section.address);
        return true;
    }
    *address=loaded->load_bias+(uintptr_t)symbol->value;
    return true;
}

static bool helf_loader_symbols_fill(helf_loaded_t *loaded,helf_file_input_t *input_file,hruntime_symbol_t *symbol_table,size_t symbol_table_size,char *symbol_names,size_t symbol_names_size)
{
    if(loaded==NULL || input_file==NULL)
    {
        return false;
    }
    if(symbol_table==NULL || symbol_table_size==0)
    {
        loaded->symbol_table_size=0;
        return true;
    }
    size_t table_index=0;
    size_t name_index=0;
    size_t section_count=helf_arch_section_count_get(input_file);
    for(size_t i=0; i<section_count; i++)
    {
        helf_arch_section_info_t section;
        if(!helf_arch_section_info_get(input_file,i,&section))
        {
            continue;
        }
        if(section.type!=HELF_SHT_SYMTAB && section.type!=HELF_SHT_DYNSYM)
        {
            continue;
        }
        size_t count=helf_arch_symbol_section_count_get(input_file,i);
        for(size_t j=0; j<count; j++)
        {
            helf_arch_symbol_info_t symbol;
            if(!helf_arch_symbol_section_info_get(input_file,i,j,&symbol))
            {
                continue;
            }
            if(!helf_loader_symbol_is_exported(&symbol))
            {
                continue;
            }
            uintptr_t address=0;
            if(!helf_loader_symbol_address_get(loaded,input_file,&symbol,&address))
            {
                continue;
            }
            size_t name_length=strlen(symbol.name)+1;
            if(table_index>=symbol_table_size || name_index+name_length>symbol_names_size)
            {
                break;
            }
            memcpy(&symbol_names[name_index],symbol.name,name_length);
            symbol_table[table_index].symbol_name=&symbol_names[name_index];
            symbol_table[table_index].symbol_addr=address;
            table_index++;
            name_index+=name_length;
        }
    }
    loaded->symbol_table_size=table_index;
    return true;
}

static void helf_loader_store_uint64(uint8_t *address,uint64_t value,bool big_endian)
{
    if(big_endian)
    {
        address[0]=(uint8_t)((value>>56)&0xFF);
        address[1]=(uint8_t)((value>>48)&0xFF);
        address[2]=(uint8_t)((value>>40)&0xFF);
        address[3]=(uint8_t)((value>>32)&0xFF);
        address[4]=(uint8_t)((value>>24)&0xFF);
        address[5]=(uint8_t)((value>>16)&0xFF);
        address[6]=(uint8_t)((value>>8)&0xFF);
        address[7]=(uint8_t)((value>>0)&0xFF);
    }
    else
    {
        address[0]=(uint8_t)((value>>0)&0xFF);
        address[1]=(uint8_t)((value>>8)&0xFF);
        address[2]=(uint8_t)((value>>16)&0xFF);
        address[3]=(uint8_t)((value>>24)&0xFF);
        address[4]=(uint8_t)((value>>32)&0xFF);
        address[5]=(uint8_t)((value>>40)&0xFF);
        address[6]=(uint8_t)((value>>48)&0xFF);
        address[7]=(uint8_t)((value>>56)&0xFF);
    }
}

static void helf_loader_store_uint32(uint8_t *address,uint32_t value,bool big_endian)
{
    if(big_endian)
    {
        address[0]=(uint8_t)((value>>24)&0xFF);
        address[1]=(uint8_t)((value>>16)&0xFF);
        address[2]=(uint8_t)((value>>8)&0xFF);
        address[3]=(uint8_t)((value>>0)&0xFF);
    }
    else
    {
        address[0]=(uint8_t)((value>>0)&0xFF);
        address[1]=(uint8_t)((value>>8)&0xFF);
        address[2]=(uint8_t)((value>>16)&0xFF);
        address[3]=(uint8_t)((value>>24)&0xFF);
    }
}

static void helf_loader_store_uint16(uint8_t *address,uint16_t value,bool big_endian)
{
    if(big_endian)
    {
        address[0]=(uint8_t)((value>>8)&0xFF);
        address[1]=(uint8_t)((value>>0)&0xFF);
    }
    else
    {
        address[0]=(uint8_t)((value>>0)&0xFF);
        address[1]=(uint8_t)((value>>8)&0xFF);
    }
}

static void helf_loader_store_uint24(uint8_t *address,uint32_t value,bool big_endian)
{
    if(big_endian)
    {
        address[0]=(uint8_t)((value>>16)&0xFF);
        address[1]=(uint8_t)((value>>8)&0xFF);
        address[2]=(uint8_t)((value>>0)&0xFF);
    }
    else
    {
        address[0]=(uint8_t)((value>>0)&0xFF);
        address[1]=(uint8_t)((value>>8)&0xFF);
        address[2]=(uint8_t)((value>>16)&0xFF);
    }
}

static uint32_t helf_loader_load_uint24(const uint8_t *address,bool big_endian)
{
    if(big_endian)
    {
        return ((uint32_t)address[0]<<16)|((uint32_t)address[1]<<8)|((uint32_t)address[2]);
    }
    return ((uint32_t)address[2]<<16)|((uint32_t)address[1]<<8)|((uint32_t)address[0]);
}

static uint64_t helf_loader_load_uint64(const uint8_t *address,bool big_endian)
{
    if(big_endian)
    {
        return ((uint64_t)address[0]<<56)|((uint64_t)address[1]<<48)|((uint64_t)address[2]<<40)|((uint64_t)address[3]<<32)
               |((uint64_t)address[4]<<24)|((uint64_t)address[5]<<16)|((uint64_t)address[6]<<8)|((uint64_t)address[7]);
    }
    return ((uint64_t)address[7]<<56)|((uint64_t)address[6]<<48)|((uint64_t)address[5]<<40)|((uint64_t)address[4]<<32)
           |((uint64_t)address[3]<<24)|((uint64_t)address[2]<<16)|((uint64_t)address[1]<<8)|((uint64_t)address[0]);
}

static uint32_t helf_loader_load_uint32(const uint8_t *address,bool big_endian)
{
    if(big_endian)
    {
        return ((uint32_t)address[0]<<24)|((uint32_t)address[1]<<16)|((uint32_t)address[2]<<8)|((uint32_t)address[3]);
    }
    return ((uint32_t)address[3]<<24)|((uint32_t)address[2]<<16)|((uint32_t)address[1]<<8)|((uint32_t)address[0]);
}

static uint16_t helf_loader_load_uint16(const uint8_t *address,bool big_endian)
{
    if(big_endian)
    {
        return (uint16_t)(((uint16_t)address[0]<<8)|((uint16_t)address[1]));
    }
    return (uint16_t)(((uint16_t)address[1]<<8)|((uint16_t)address[0]));
}

static uint64_t helf_loader_reloc_field_load(const uint8_t *address,size_t size,bool big_endian)
{
    switch(size)
    {
    case 1:
        return address[0];
    case 2:
        return helf_loader_load_uint16(address,big_endian);
    case 3:
        return helf_loader_load_uint24(address,big_endian);
    case 4:
        return helf_loader_load_uint32(address,big_endian);
    case 8:
        return helf_loader_load_uint64(address,big_endian);
    default:
        return 0;
    }
}

static void helf_loader_reloc_field_store(uint8_t *address,size_t size,uint64_t value,bool big_endian)
{
    switch(size)
    {
    case 1:
        address[0]=(uint8_t)value;
        break;
    case 2:
        helf_loader_store_uint16(address,(uint16_t)value,big_endian);
        break;
    case 3:
        helf_loader_store_uint24(address,(uint32_t)value,big_endian);
        break;
    case 4:
        helf_loader_store_uint32(address,(uint32_t)value,big_endian);
        break;
    case 8:
        helf_loader_store_uint64(address,value,big_endian);
        break;
    default:
        break;
    }
}

/**
 * 按BFD HOWTO语义把重定位值写入目标。
 * raw_value为符号地址+加数(或已有最终值)。
 */
static bool helf_loader_reloc_apply_value(const helf_arch_relocation_howto_t *howto,uint8_t *target,uintptr_t place,uintptr_t section_base,uint64_t raw_value,bool big_endian)
{
    if(howto==NULL || target==NULL)
    {
        return false;
    }
    size_t size=howto->size;
    if(size==0)
    {
        return true;
    }
    if(size>8)
    {
        return false;
    }
    uint64_t value=raw_value;
    if((howto->flags&HELF_ARCH_RELOC_HOWTO_PC_RELATIVE)!=0)
    {
        uintptr_t base=((howto->flags&HELF_ARCH_RELOC_HOWTO_PCREL_OFFSET)!=0)?place:section_base;
        value-=(uint64_t)base;
    }
    if((howto->flags&HELF_ARCH_RELOC_HOWTO_NEGATE)!=0)
    {
        value=(uint64_t)(-(int64_t)value);
    }
    if(howto->rightshift<64)
    {
        value>>=howto->rightshift;
    }
    if(howto->bitpos<64)
    {
        value<<=howto->bitpos;
    }
    uint64_t dst_mask=howto->dst_mask;
    if(dst_mask==0)
    {
        uint64_t size_mask=(size>=8)?UINT64_MAX:((1ULL<<(size*8))-1);
        helf_loader_reloc_field_store(target,size,value&size_mask,big_endian);
        return true;
    }
    uint64_t old=helf_loader_reloc_field_load(target,size,big_endian);
    uint64_t src_mask=howto->src_mask;
    uint64_t new_value=(old&~dst_mask)|(((old&src_mask)+value)&dst_mask);
    helf_loader_reloc_field_store(target,size,new_value,big_endian);
    return true;
}

static const hruntime_symbol_t *helf_loader_external_symbol_find(helf_namespace_t *ns,helf_loaded_t *loaded,const char *name)
{
    const hruntime_symbol_t *ret=helf_namespace_symbol_find(ns,name);
    if(ret==NULL && loaded!=NULL && loaded->external_symbol_find!=NULL)
    {
        ret=loaded->external_symbol_find(name,loaded->external_usr);
    }
    return ret;
}

typedef struct
{
    helf_namespace_t *ns;
    helf_loaded_t *loaded;
} helf_loader_reloc_context_usr_t;

static bool helf_loader_reloc_address_is_executable(void *usr,uintptr_t address)
{
    helf_loader_reloc_context_usr_t *context=(helf_loader_reloc_context_usr_t *)usr;
    if(context==NULL)
    {
        return false;
    }
    helf_loaded_t *items[2]= {context->loaded,NULL};
    for(size_t i=0; i<2; i++)
    {
        helf_loaded_t *item=items[i];
        if(item!=NULL && item->rx_image!=NULL
                && address>=(uintptr_t)item->rx_image
                && address<(uintptr_t)item->rx_image+item->rx_size)
        {
            return true;
        }
    }
    if(context->ns!=NULL)
    {
        HDOUBLYLINKEDLIST_FOREACH(context->ns->loaded_list_head,list_item)
        {
            helf_loaded_t *item=(helf_loaded_t *)GET_STRUCT_PTR_BY_MEMBER_PTR(list_item,helf_loaded_t,list_head);
            if(item->rx_image!=NULL
                    && address>=(uintptr_t)item->rx_image
                    && address<(uintptr_t)item->rx_image+item->rx_size)
            {
                return true;
            }
        }
    }
    return false;
}

static void helf_loader_reloc_context_init(helf_arch_relocation_context_t *context,helf_loader_reloc_context_usr_t *usr,helf_namespace_t *ns,helf_loaded_t *loaded)
{
    memset(context,0,sizeof(*context));
    memset(usr,0,sizeof(*usr));
    usr->ns=ns;
    usr->loaded=loaded;
    context->machine=loaded->machine;
    context->elfclass=loaded->elfclass;
    context->elfdata=loaded->elfdata;
    context->big_endian=(loaded->elfdata==HELF_ELFDATA_2_MSB);
    context->toc_base=loaded->toc_base;
    context->usr=usr;
    context->address_is_executable=helf_loader_reloc_address_is_executable;
}

static size_t helf_loader_toc_bytes_get(helf_file_input_t *input_file,uint16_t machine,uint8_t elfclass,helf_loaded_type_t type)
{
    if(type!=HELF_LOADED_TYPE_RELOCATABLE)
    {
        return 0;
    }
    return helf_arch_relocation_toc_size_get(machine,elfclass,input_file);
}

static bool helf_loader_relocation_symbol_address_get(helf_namespace_t *ns,helf_loaded_t *loaded,helf_file_input_t *input_file,const helf_arch_relocation_info_t *relocation,const helf_arch_symbol_info_t *symbol,uintptr_t *address)
{
    if(loaded==NULL || input_file==NULL || symbol==NULL || address==NULL)
    {
        return false;
    }
    if(symbol->shndx==HELF_SHN_UNDEF)
    {
        if(strcmp(symbol->name,".TOC.")==0 && loaded->toc_base!=0)
        {
            *address=loaded->toc_base;
            return true;
        }
        const hruntime_symbol_t *external=helf_loader_external_symbol_find(ns,loaded,symbol->name);
        if(external!=NULL)
        {
            *address=external->symbol_addr;
            if(loaded->machine==HELF_EM_PPC64 && relocation!=NULL)
            {
                helf_arch_relocation_context_t context;
                helf_loader_reloc_context_usr_t context_usr;
                helf_loader_reloc_context_init(&context,&context_usr,ns,loaded);
                helf_arch_relocation_symbol_address(loaded->machine,loaded->elfclass,relocation->type,address,&context);
            }
            return true;
        }
        if(HELF_ELF32_ST_BIND(symbol->info)==HELF_STB_WEAK)
        {
            *address=0;
            return true;
        }
        return false;
    }
    if(!helf_loader_symbol_address_get(loaded,input_file,symbol,address))
    {
        return false;
    }
    if(loaded->machine==HELF_EM_PPC64 && relocation!=NULL)
    {
        helf_arch_relocation_context_t context;
        helf_loader_reloc_context_usr_t context_usr;
        helf_loader_reloc_context_init(&context,&context_usr,ns,loaded);
        helf_arch_relocation_symbol_address(loaded->machine,loaded->elfclass,relocation->type,address,&context);
    }
    return true;
}

static unsigned int helf_loader_reloc_kind(const helf_arch_relocation_howto_t *howto)
{
    return (howto->flags&HELF_ARCH_RELOC_HOWTO_KIND_MASK)>>HELF_ARCH_RELOC_HOWTO_KIND_SHIFT;
}

static bool helf_loader_apply_relocations_pass(helf_namespace_t *ns,helf_loaded_t *loaded,helf_file_input_t *input_file,bool opd_pass)
{
    if(loaded==NULL || input_file==NULL)
    {
        return false;
    }
    bool big_endian=(loaded->elfdata==HELF_ELFDATA_2_MSB);
    size_t relocation_section_count=helf_arch_relocation_section_count_get(input_file);
    size_t section_count=helf_arch_section_count_get(input_file);
    for(size_t i=0; i<relocation_section_count; i++)
    {
        helf_arch_relocation_section_info_t section_info;
        if(!helf_arch_relocation_section_info_get(input_file,i,&section_info))
        {
            return false;
        }
        bool is_priority=false;
        if(section_info.info<section_count)
        {
            helf_arch_section_info_t target_section;
            if(helf_arch_section_info_get(input_file,section_info.info,&target_section))
            {
                is_priority=helf_arch_relocation_section_is_priority(loaded->machine,loaded->elfclass,target_section.name);
            }
        }
        if(is_priority!=opd_pass)
        {
            continue;
        }
        for(size_t j=0; j<section_info.count; j++)
        {
            helf_arch_relocation_info_t relocation;
            if(!helf_arch_relocation_get(input_file,section_info.index,j,&relocation))
            {
                return false;
            }
            if(relocation.target_section_index>=section_count)
            {
                return false;
            }
            helf_arch_section_info_t target_section;
            if(!helf_arch_section_info_get(input_file,relocation.target_section_index,&target_section))
            {
                return false;
            }
            if(helf_loader_section_class_get(&target_section)==0)
            {
                /* 不加载的节(如调试节)无需重定位 */
                continue;
            }
            helf_arch_relocation_howto_t howto;
            if(!helf_arch_relocation_howto_get(loaded->machine,loaded->elfclass,relocation.type,&howto))
            {
                return false;
            }
            unsigned int kind=helf_loader_reloc_kind(&howto);
            if(kind==HELF_ARCH_RELOC_KIND_NONE)
            {
                continue;
            }
            if(kind==HELF_ARCH_RELOC_KIND_UNSUPPORTED)
            {
                return false;
            }
            helf_arch_symbol_info_t symbol;
            memset(&symbol,0,sizeof(symbol));
            uintptr_t symbol_address=0;
            if(relocation.symbol!=0)
            {
                if(!helf_arch_relocation_symbol_info_get(input_file,&relocation,&symbol))
                {
                    return false;
                }
                if(!helf_loader_relocation_symbol_address_get(ns,loaded,input_file,&relocation,&symbol,&symbol_address))
                {
                    return false;
                }
            }
            void *section_address=helf_loader_section_address(loaded,input_file,relocation.target_section_index);
            if(section_address==NULL)
            {
                return false;
            }
            uint8_t *target=(uint8_t *)section_address+(uintptr_t)relocation.offset;
            uintptr_t place=(uintptr_t)target;
            uintptr_t section_base=(uintptr_t)section_address;
            int64_t addend=relocation.addend;
            helf_arch_relocation_context_t arch_context;
            helf_loader_reloc_context_usr_t arch_context_usr;
            helf_loader_reloc_context_init(&arch_context,&arch_context_usr,ns,loaded);
            if(helf_arch_relocation_apply(loaded->machine,loaded->elfclass,relocation.type,target,place,symbol_address,addend,&arch_context))
            {
                continue;
            }
            if((howto.flags&HELF_ARCH_RELOC_HOWTO_GOT)!=0)
            {
                return false;
            }
            if(kind==HELF_ARCH_RELOC_KIND_COPY)
            {
                if(symbol_address==0 || symbol.size==0)
                {
                    continue;
                }
                if(symbol.size>SIZE_MAX)
                {
                    return false;
                }
                memcpy(target,(const void *)symbol_address,(size_t)symbol.size);
                continue;
            }
            uint64_t raw_value=0;
            switch(kind)
            {
            case HELF_ARCH_RELOC_KIND_RELATIVE:
                raw_value=(uint64_t)(loaded->load_bias+(uintptr_t)addend);
                break;
            case HELF_ARCH_RELOC_KIND_IRELATIVE:
                {
                    uintptr_t (*resolver)(void)=(uintptr_t (*)(void))(loaded->load_bias+(uintptr_t)addend);
                    raw_value=(uint64_t)resolver();
                }
                break;
            case HELF_ARCH_RELOC_KIND_GLOB_DAT:
            case HELF_ARCH_RELOC_KIND_JUMP_SLOT:
                raw_value=(uint64_t)symbol_address;
                break;
            default:
                if((howto.flags&HELF_ARCH_RELOC_HOWTO_USE_SYMBOL_SIZE)!=0)
                {
                    raw_value=symbol.size;
                }
                else
                {
                    raw_value=(uint64_t)(symbol_address+(uintptr_t)addend);
                }
                break;
            }
            if(!helf_loader_reloc_apply_value(&howto,target,place,section_base,raw_value,big_endian))
            {
                return false;
            }
        }
    }
    return true;
}

static bool helf_loader_apply_relocations(helf_namespace_t *ns,helf_loaded_t *loaded,helf_file_input_t *input_file)
{
    /* .opd先处理,保证ELFv1函数描述符在分支重定位前已完成 */
    if(!helf_loader_apply_relocations_pass(ns,loaded,input_file,true))
    {
        return false;
    }
    return helf_loader_apply_relocations_pass(ns,loaded,input_file,false);
}

/*
 * 动态段信息
 */
typedef struct helf_loader_dynamic_info
{
    uint64_t    rela;               /**< DT_RELA */
    uint64_t    relasz;             /**< DT_RELASZ */
    uint64_t    relaent;            /**< DT_RELAENT */
    uint64_t    rel;                /**< DT_REL */
    uint64_t    relsz;              /**< DT_RELSZ */
    uint64_t    relent;             /**< DT_RELENT */
    uint64_t    jmprel;             /**< DT_JMPREL */
    uint64_t    pltrelsz;           /**< DT_PLTRELSZ */
    uint64_t    pltrel;             /**< DT_PLTREL */
    uint64_t    symtab;             /**< DT_SYMTAB */
    uint64_t    syment;             /**< DT_SYMENT */
    uint64_t    strtab;             /**< DT_STRTAB */
    uint64_t    strsz;              /**< DT_STRSZ */
} helf_loader_dynamic_info_t;

static uint8_t *helf_loader_vaddr_to_pointer(helf_loaded_t *loaded,uint64_t vaddr,size_t size)
{
    if(loaded==NULL || loaded->image==NULL)
    {
        return NULL;
    }
    if(vaddr<loaded->min_vaddr)
    {
        return NULL;
    }
    uint64_t offset=vaddr-loaded->min_vaddr;
    if(offset>loaded->image_size || size>loaded->image_size-(size_t)offset)
    {
        return NULL;
    }
    return (uint8_t *)loaded->image+(size_t)offset;
}

static void helf_loader_dynamic_info_get(helf_file_input_t *input_file,uint8_t elfclass,helf_loader_dynamic_info_t *dynamic)
{
    if(dynamic==NULL)
    {
        return;
    }
    memset(dynamic,0,sizeof(*dynamic));
    if(input_file==NULL)
    {
        return;
    }
    size_t count=helf_arch_dynamic_count_get(input_file);
    for(size_t i=0; i<count; i++)
    {
        helf_arch_dynamic_info_t info;
        if(!helf_arch_dynamic_info_get(input_file,i,&info))
        {
            break;
        }
        switch(info.tag)
        {
        case HELF_DT_NULL:
            i=count;
            break;
        case HELF_DT_RELA:
            dynamic->rela=info.value;
            break;
        case HELF_DT_RELASZ:
            dynamic->relasz=info.value;
            break;
        case HELF_DT_RELAENT:
            dynamic->relaent=info.value;
            break;
        case HELF_DT_REL:
            dynamic->rel=info.value;
            break;
        case HELF_DT_RELSZ:
            dynamic->relsz=info.value;
            break;
        case HELF_DT_RELENT:
            dynamic->relent=info.value;
            break;
        case HELF_DT_JMPREL:
            dynamic->jmprel=info.value;
            break;
        case HELF_DT_PLTRELSZ:
            dynamic->pltrelsz=info.value;
            break;
        case HELF_DT_PLTREL:
            dynamic->pltrel=info.value;
            break;
        case HELF_DT_SYMTAB:
            dynamic->symtab=info.value;
            break;
        case HELF_DT_SYMENT:
            dynamic->syment=info.value;
            break;
        case HELF_DT_STRTAB:
            dynamic->strtab=info.value;
            break;
        case HELF_DT_STRSZ:
            dynamic->strsz=info.value;
            break;
        default:
            break;
        }
    }
    if(dynamic->relaent==0)
    {
        dynamic->relaent=(elfclass==HELF_ELFCLASS_64)?sizeof(helf_elf64_rela_t):12;
    }
    if(dynamic->relent==0)
    {
        dynamic->relent=(elfclass==HELF_ELFCLASS_64)?sizeof(helf_elf64_rel_t):8;
    }
    if(dynamic->syment==0)
    {
        dynamic->syment=(elfclass==HELF_ELFCLASS_64)?sizeof(helf_elf64_symbol_t):16;
    }
}

static bool helf_loader_dynamic_symbol_get(helf_loaded_t *loaded,const helf_loader_dynamic_info_t *dynamic,size_t index,helf_arch_symbol_info_t *symbol)
{
    if(loaded==NULL || dynamic==NULL || symbol==NULL)
    {
        return false;
    }
    size_t symbol_entry_size=(loaded->elfclass==HELF_ELFCLASS_64)?sizeof(helf_elf64_symbol_t):16;
    if(dynamic->symtab==0 || dynamic->syment<symbol_entry_size)
    {
        return false;
    }
    if(index>SIZE_MAX/(size_t)dynamic->syment)
    {
        return false;
    }
    uint64_t symbol_offset=(uint64_t)index*dynamic->syment;
    if(dynamic->symtab>UINT64_MAX-symbol_offset)
    {
        return false;
    }
    uint8_t *entry=helf_loader_vaddr_to_pointer(loaded,dynamic->symtab+symbol_offset,symbol_entry_size);
    if(entry==NULL)
    {
        return false;
    }
    bool big_endian=(loaded->elfdata==HELF_ELFDATA_2_MSB);
    uint32_t name_offset=(uint32_t)helf_loader_load_uint32(entry,big_endian);
    memset(symbol,0,sizeof(*symbol));
    if(loaded->elfclass==HELF_ELFCLASS_64)
    {
        symbol->info=entry[4];
        symbol->other=entry[5];
        symbol->shndx=(uint16_t)helf_loader_load_uint16(entry+6,big_endian);
        symbol->value=helf_loader_load_uint64(entry+8,big_endian);
        symbol->size=helf_loader_load_uint64(entry+16,big_endian);
    }
    else
    {
        symbol->value=helf_loader_load_uint32(entry+4,big_endian);
        symbol->size=helf_loader_load_uint32(entry+8,big_endian);
        symbol->info=entry[12];
        symbol->other=entry[13];
        symbol->shndx=(uint16_t)helf_loader_load_uint16(entry+14,big_endian);
    }
    if(dynamic->strtab==0)
    {
        return name_offset==0;
    }
    if(dynamic->strsz!=0 && name_offset>=dynamic->strsz)
    {
        return false;
    }
    if(dynamic->strtab>UINT64_MAX-name_offset)
    {
        return false;
    }
    uint64_t name_vaddr=dynamic->strtab+name_offset;
    if(name_vaddr<loaded->min_vaddr || name_vaddr-loaded->min_vaddr>=loaded->image_size)
    {
        return false;
    }
    uint8_t *name=helf_loader_vaddr_to_pointer(loaded,name_vaddr,1);
    if(name==NULL)
    {
        return false;
    }
    size_t name_limit=(size_t)(loaded->image_size-(name_vaddr-loaded->min_vaddr));
    if(dynamic->strsz>name_offset)
    {
        size_t strtab_limit=(size_t)(dynamic->strsz-name_offset);
        if(strtab_limit<name_limit)
        {
            name_limit=strtab_limit;
        }
    }
    size_t name_length=0;
    while(name_length<name_limit && name_length+1<sizeof(symbol->name) && name[name_length]!='\0')
    {
        symbol->name[name_length]=(char)name[name_length];
        name_length++;
    }
    symbol->name[name_length]='\0';
    return true;
}

static bool helf_loader_dynamic_symbol_address_get(helf_namespace_t *ns,helf_loaded_t *loaded,helf_file_input_t *input_file,const helf_arch_symbol_info_t *symbol,uintptr_t *address)
{
    if(loaded==NULL || symbol==NULL || address==NULL)
    {
        return false;
    }
    if(symbol->shndx==HELF_SHN_UNDEF)
    {
        const hruntime_symbol_t *external=helf_loader_external_symbol_find(ns,loaded,symbol->name);
        if(external!=NULL)
        {
            *address=external->symbol_addr;
            return true;
        }
        if(HELF_ELF32_ST_BIND(symbol->info)==HELF_STB_WEAK)
        {
            /* 弱符号未找到时按0处理 */
            *address=0;
            return true;
        }
        return false;
    }
    return helf_loader_symbol_address_get(loaded,input_file,symbol,address);
}


static bool helf_loader_apply_dynamic_table(helf_namespace_t *ns,helf_loaded_t *loaded,helf_file_input_t *input_file,const helf_loader_dynamic_info_t *dynamic,uint64_t table_vaddr,uint64_t table_size,bool has_addend)
{
    if(table_vaddr==0 || table_size==0)
    {
        return true;
    }
    bool is64=(loaded->elfclass==HELF_ELFCLASS_64);
    size_t entry_size=has_addend?(is64?24:12):(is64?16:8);
    uint8_t *table=helf_loader_vaddr_to_pointer(loaded,table_vaddr,(size_t)table_size);
    if(table==NULL)
    {
        return false;
    }
    bool big_endian=(loaded->elfdata==HELF_ELFDATA_2_MSB);
    size_t count=(size_t)table_size/entry_size;
    for(size_t i=0; i<count; i++)
    {
        uint8_t *entry=table+i*entry_size;
        uint64_t r_offset=is64?helf_loader_load_uint64(entry,big_endian):helf_loader_load_uint32(entry,big_endian);
        uint64_t r_info=is64?helf_loader_load_uint64(entry+8,big_endian):helf_loader_load_uint32(entry+4,big_endian);
        int64_t addend=0;
        if(has_addend)
        {
            addend=is64?(int64_t)helf_loader_load_uint64(entry+16,big_endian):(int64_t)(int32_t)helf_loader_load_uint32(entry+8,big_endian);
        }
        uint32_t symbol_index=(uint32_t)(is64?(r_info>>32):(r_info>>8));
        uint32_t type=(uint32_t)(is64?(r_info&0xFFFFFFFF):(r_info&0xFF));
        helf_arch_relocation_howto_t howto;
        if(!helf_arch_relocation_howto_get(loaded->machine,loaded->elfclass,type,&howto))
        {
            return false;
        }
        unsigned int kind=helf_loader_reloc_kind(&howto);
        if(kind==HELF_ARCH_RELOC_KIND_NONE)
        {
            continue;
        }
        if(kind==HELF_ARCH_RELOC_KIND_UNSUPPORTED || (howto.flags&HELF_ARCH_RELOC_HOWTO_GOT)!=0)
        {
            return false;
        }
        if(howto.size==0 && kind!=HELF_ARCH_RELOC_KIND_COPY)
        {
            /* 大小为0的重定位仅作为标记 */
            continue;
        }
        helf_arch_symbol_info_t symbol;
        memset(&symbol,0,sizeof(symbol));
        uintptr_t symbol_address=0;
        bool need_symbol=(kind!=HELF_ARCH_RELOC_KIND_RELATIVE && kind!=HELF_ARCH_RELOC_KIND_IRELATIVE);
        if(need_symbol && symbol_index!=0)
        {
            if(!helf_loader_dynamic_symbol_get(loaded,dynamic,symbol_index,&symbol))
            {
                return false;
            }
            if(!helf_loader_dynamic_symbol_address_get(ns,loaded,input_file,&symbol,&symbol_address))
            {
                if(kind==HELF_ARCH_RELOC_KIND_GLOB_DAT || kind==HELF_ARCH_RELOC_KIND_JUMP_SLOT)
                {
                    /* 未解析的外部符号在GOT/PLT中留0，调用时由用户保证符号已注册 */
                    symbol_address=0;
                }
                else
                {
                    return false;
                }
            }
        }
        if(kind==HELF_ARCH_RELOC_KIND_JUMP_SLOT
                && loaded->machine==HELF_EM_PPC64
                && loaded->elfdata==HELF_ELFDATA_2_MSB
                && symbol_address!=0)
        {
            /* PPC64 ELFv1: GOT[0]=代码入口, GOT[1]=目标TOC */
            uint8_t *descriptor=(uint8_t *)symbol_address;
            uint64_t code=helf_loader_load_uint64(descriptor,true);
            uint64_t toc=helf_loader_load_uint64(descriptor+8,true);
            if(code!=0)
            {
                symbol_address=(uintptr_t)code;
                uint8_t *toc_slot=helf_loader_vaddr_to_pointer(loaded,r_offset+sizeof(uint64_t),sizeof(uint64_t));
                if(toc_slot!=NULL)
                {
                    helf_loader_store_uint64(toc_slot,toc,true);
                }
            }
        }
        if(kind==HELF_ARCH_RELOC_KIND_COPY)
        {
            if(symbol.size==0 || symbol_address==0 || symbol.size>SIZE_MAX)
            {
                continue;
            }
            uint8_t *target=helf_loader_vaddr_to_pointer(loaded,r_offset,(size_t)symbol.size);
            if(target==NULL)
            {
                return false;
            }
            memcpy(target,(const void *)symbol_address,(size_t)symbol.size);
            continue;
        }
        uint8_t *target=helf_loader_vaddr_to_pointer(loaded,r_offset,howto.size);
        if(target==NULL)
        {
            return false;
        }
        uintptr_t place=(uintptr_t)target;
        uint64_t raw_value=0;
        switch(kind)
        {
        case HELF_ARCH_RELOC_KIND_RELATIVE:
            raw_value=(uint64_t)(loaded->load_bias+(uintptr_t)addend);
            break;
        case HELF_ARCH_RELOC_KIND_IRELATIVE:
            {
                uintptr_t (*resolver)(void)=(uintptr_t (*)(void))(loaded->load_bias+(uintptr_t)addend);
                raw_value=(uint64_t)resolver();
            }
            break;
        case HELF_ARCH_RELOC_KIND_GLOB_DAT:
        case HELF_ARCH_RELOC_KIND_JUMP_SLOT:
            raw_value=(uint64_t)symbol_address;
            break;
        default:
            if((howto.flags&HELF_ARCH_RELOC_HOWTO_USE_SYMBOL_SIZE)!=0)
            {
                raw_value=symbol.size;
            }
            else
            {
                raw_value=(uint64_t)(symbol_address+(uintptr_t)addend);
            }
            break;
        }
        helf_arch_relocation_howto_t apply_howto=howto;
        if(kind==HELF_ARCH_RELOC_KIND_GLOB_DAT
                || kind==HELF_ARCH_RELOC_KIND_JUMP_SLOT
                || kind==HELF_ARCH_RELOC_KIND_IRELATIVE)
        {
            /* 这些类型直接写入符号/解析地址,不使用原字段作为加数 */
            apply_howto.src_mask=0;
        }
        if(!helf_loader_reloc_apply_value(&apply_howto,target,place,place,raw_value,big_endian))
        {
            return false;
        }
    }
    return true;
}

static bool helf_loader_apply_dynamic_relocations(helf_namespace_t *ns,helf_loaded_t *loaded,helf_file_input_t *input_file)
{
    if(loaded==NULL || input_file==NULL)
    {
        return false;
    }
    helf_loader_dynamic_info_t dynamic;
    helf_loader_dynamic_info_get(input_file,loaded->elfclass,&dynamic);
    if(!helf_loader_apply_dynamic_table(ns,loaded,input_file,&dynamic,dynamic.rela,dynamic.relasz,true))
    {
        return false;
    }
    if(dynamic.jmprel!=0 && dynamic.pltrelsz!=0)
    {
        bool has_addend=(dynamic.pltrel!=HELF_DT_REL);
        /* 若PLT重定位已包含在DT_RELA/DT_REL范围内，则无需重复处理 */
        bool covered=false;
        if(dynamic.rela!=0 && dynamic.relasz!=0 && dynamic.jmprel>=dynamic.rela && dynamic.jmprel+dynamic.pltrelsz<=dynamic.rela+dynamic.relasz)
        {
            covered=true;
        }
        if(dynamic.rel!=0 && dynamic.relsz!=0 && dynamic.jmprel>=dynamic.rel && dynamic.jmprel+dynamic.pltrelsz<=dynamic.rel+dynamic.relsz)
        {
            covered=true;
        }
        if(!covered && !helf_loader_apply_dynamic_table(ns,loaded,input_file,&dynamic,dynamic.jmprel,dynamic.pltrelsz,has_addend))
        {
            return false;
        }
    }
    if(!helf_loader_apply_dynamic_table(ns,loaded,input_file,&dynamic,dynamic.rel,dynamic.relsz,false))
    {
        return false;
    }
    return true;
}

static bool helf_loader_copy_sections(helf_loaded_t *loaded,helf_file_input_t *input_file)
{
    if(loaded==NULL || input_file==NULL)
    {
        return false;
    }
    size_t count=helf_arch_section_count_get(input_file);
    for(size_t i=0; i<count; i++)
    {
        helf_arch_section_info_t section;
        if(!helf_arch_section_info_get(input_file,i,&section))
        {
            return false;
        }
        if(helf_loader_section_class_get(&section)==0 || section.size==0)
        {
            continue;
        }
        void *address=helf_loader_section_address(loaded,input_file,i);
        if(address==NULL)
        {
            return false;
        }
        if(section.type==HELF_SHT_NOBITS)
        {
            memset(address,0,(size_t)section.size);
        }
        else
        {
            if(helf_file_input_read(input_file,(uintptr_t)section.offset,address,(size_t)section.size)!=section.size)
            {
                return false;
            }
        }
    }
    return true;
}

static bool helf_loader_copy_segments(helf_loaded_t *loaded,helf_file_input_t *input_file)
{
    if(loaded==NULL || input_file==NULL)
    {
        return false;
    }
    size_t count=helf_arch_program_header_count_get(input_file);
    for(size_t i=0; i<count; i++)
    {
        helf_arch_segment_info_t segment;
        if(!helf_arch_segment_info_get(input_file,i,&segment))
        {
            continue;
        }
        if(segment.type!=HELF_PT_LOAD || segment.memsz==0)
        {
            continue;
        }
        if(segment.vaddr<loaded->min_vaddr || segment.filesz>segment.memsz)
        {
            return false;
        }
        uint64_t segment_offset=segment.vaddr-loaded->min_vaddr;
        if(segment_offset>loaded->image_size || segment.memsz>loaded->image_size-segment_offset)
        {
            return false;
        }
        uint8_t *destination=(uint8_t *)loaded->image+(size_t)segment_offset;
        if(segment.filesz!=0)
        {
            if(helf_file_input_read(input_file,(uintptr_t)segment.offset,destination,(size_t)segment.filesz)!=segment.filesz)
            {
                return false;
            }
        }
        if(segment.memsz>segment.filesz)
        {
            memset(destination+segment.filesz,0,(size_t)(segment.memsz-segment.filesz));
        }
    }
    return true;
}

static bool helf_loader_mprotect_regions(helf_loaded_t *loaded)
{
    if(loaded==NULL)
    {
        return false;
    }
    if(loaded->rx_size!=0)
    {
        if(hmprotect(loaded->rx_image,helf_loader_align_up(loaded->rx_size,HELF_LOADER_PAGE_ALIGN),HMMAN_PROT_READ|HMMAN_PROT_EXEC)!=0)
        {
            return false;
        }
    }
    if(loaded->ro_size!=0)
    {
        if(hmprotect(loaded->ro_image,helf_loader_align_up(loaded->ro_size,HELF_LOADER_PAGE_ALIGN),HMMAN_PROT_READ)!=0)
        {
            return false;
        }
    }
    if(loaded->rw_size!=0)
    {
        if(hmprotect(loaded->rw_image,helf_loader_align_up(loaded->rw_size,HELF_LOADER_PAGE_ALIGN),HMMAN_PROT_READ|HMMAN_PROT_WRITE)!=0)
        {
            return false;
        }
    }
    return true;
}

static bool helf_loader_segments_protect(helf_loaded_t *loaded,helf_file_input_t *input_file)
{
    if(loaded==NULL || input_file==NULL || loaded->image==NULL || loaded->image_size==0)
    {
        return false;
    }
    size_t page_count=helf_loader_align_up(loaded->image_size,HELF_LOADER_PAGE_ALIGN)/HELF_LOADER_PAGE_ALIGN;
    if(page_count==0)
    {
        return true;
    }
    uint8_t *page_prot=(uint8_t *)hmalloc(page_count);
    if(page_prot==NULL)
    {
        return false;
    }
    memset(page_prot,0,page_count);
    size_t count=helf_arch_program_header_count_get(input_file);
    for(size_t i=0; i<count; i++)
    {
        helf_arch_segment_info_t segment;
        if(!helf_arch_segment_info_get(input_file,i,&segment))
        {
            continue;
        }
        if(segment.type!=HELF_PT_LOAD || segment.memsz==0 || segment.vaddr<loaded->min_vaddr)
        {
            continue;
        }
        uint64_t start=segment.vaddr-loaded->min_vaddr;
        if(start>=loaded->image_size)
        {
            continue;
        }
        uint64_t end=start+segment.memsz;
        if(end>loaded->image_size)
        {
            end=loaded->image_size;
        }
        if(end<=start)
        {
            continue;
        }
        size_t page_start=(size_t)(start/HELF_LOADER_PAGE_ALIGN);
        size_t page_end=(size_t)((end-1)/HELF_LOADER_PAGE_ALIGN);
        uint8_t prot=0;
        if((segment.flags&HELF_PF_R)!=0)
        {
            prot|=HMMAN_PROT_READ;
        }
        if((segment.flags&HELF_PF_W)!=0)
        {
            prot|=HMMAN_PROT_WRITE;
        }
        if((segment.flags&HELF_PF_X)!=0)
        {
            prot|=HMMAN_PROT_EXEC;
        }
        for(size_t page=page_start; page<=page_end && page<page_count; page++)
        {
            page_prot[page]|=prot;
        }
    }
    bool ret=true;
    for(size_t i=0; i<page_count && ret;)
    {
        size_t j=i+1;
        while(j<page_count && page_prot[j]==page_prot[i])
        {
            j++;
        }
        int prot=page_prot[i];
        if(prot==0)
        {
            prot=HMMAN_PROT_NONE;
        }
        if(hmprotect((uint8_t *)loaded->image+i*HELF_LOADER_PAGE_ALIGN,(j-i)*HELF_LOADER_PAGE_ALIGN,prot)!=0)
        {
            ret=false;
        }
        i=j;
    }
    hfree(page_prot);
    return ret;
}

static bool helf_loader_namespace_link(helf_namespace_t *ns,helf_loaded_t *loaded)
{
    if(ns==NULL || loaded==NULL)
    {
        return false;
    }
    hdoublylinkedlist_init(&loaded->list_head);
    if(ns->loaded_list_head==NULL)
    {
        ns->loaded_list_head=&loaded->list_head;
    }
    else
    {
        hdoublylinkedlist_add_front(ns->loaded_list_head,&loaded->list_head);
    }
    return true;
}

static void helf_loader_namespace_unlink(helf_namespace_t *ns,helf_loaded_t *loaded)
{
    if(loaded==NULL)
    {
        return;
    }
    if(ns!=NULL && ns->loaded_list_head==&loaded->list_head)
    {
        if(loaded->list_head.next==&loaded->list_head)
        {
            ns->loaded_list_head=NULL;
        }
        else
        {
            ns->loaded_list_head=loaded->list_head.next;
        }
    }
    hdoublylinkedlist_remove(&loaded->list_head);
}

static bool helf_loader_load_common(helf_namespace_t *ns,helf_loaded_t *loaded,helf_file_input_t *input_file,void *image,size_t image_size,void *rx_image,size_t rx_size,void *ro_image,size_t ro_size,void *rw_image,size_t rw_size,uintptr_t min_vaddr,hruntime_symbol_t *symbol_table,size_t symbol_table_size,char *symbol_names,size_t symbol_names_size,bool set_permissions,const helf_load_param_t *param)
{
    if(ns==NULL || loaded==NULL || input_file==NULL)
    {
        return false;
    }
    loaded->external_symbol_find=(param!=NULL)?param->external_symbol_find:NULL;
    loaded->external_usr=(param!=NULL)?param->usr:NULL;
    loaded->input_file=input_file;
    loaded->image=image;
    loaded->image_size=image_size;
    loaded->rx_image=rx_image;
    loaded->rx_size=rx_size;
    loaded->ro_image=ro_image;
    loaded->ro_size=ro_size;
    loaded->rw_image=rw_image;
    loaded->rw_size=rw_size;
    loaded->min_vaddr=min_vaddr;
    loaded->load_bias=(uintptr_t)image-min_vaddr;
    loaded->symbol_table=symbol_table;
    loaded->symbol_table_size=symbol_table_size;
    loaded->symbol_names=symbol_names;
    loaded->symbol_names_size=symbol_names_size;
    loaded->usr=ns->usr;
    if(loaded->type==HELF_LOADED_TYPE_RELOCATABLE)
    {
        if(!helf_loader_copy_sections(loaded,input_file))
        {
            return false;
        }
        if(!helf_loader_apply_relocations(ns,loaded,input_file))
        {
            return false;
        }
        if(set_permissions && !helf_loader_mprotect_regions(loaded))
        {
            return false;
        }
    }
    else
    {
        if(!helf_loader_copy_segments(loaded,input_file))
        {
            return false;
        }
        if(!helf_loader_apply_dynamic_relocations(ns,loaded,input_file))
        {
            return false;
        }
        if(set_permissions && !helf_loader_segments_protect(loaded,input_file))
        {
            return false;
        }
    }
    if(!helf_loader_symbols_fill(loaded,input_file,symbol_table,symbol_table_size,symbol_names,symbol_names_size))
    {
        return false;
    }
    if(ns->machine!=HELF_EM_NONE && (ns->machine!=loaded->machine || ns->elfclass!=loaded->elfclass || ns->elfdata!=loaded->elfdata))
    {
        return false;
    }
    if(!helf_loader_namespace_link(ns,loaded))
    {
        return false;
    }
    if(ns->machine==HELF_EM_NONE)
    {
        ns->machine=loaded->machine;
        ns->elfclass=loaded->elfclass;
        ns->elfdata=loaded->elfdata;
    }
    return true;
}

size_t helf_namespace_loaded_buffer_size_get(helf_file_input_t *input_file)
{
    if(input_file==NULL)
    {
        return 0;
    }
    uint16_t machine=HELF_EM_NONE;
    uint8_t elfclass=0;
    uint8_t elfdata=0;
    uint16_t type=0;
    if(!helf_loader_file_info_get(input_file,&machine,&elfclass,&elfdata,&type))
    {
        return 0;
    }
    helf_loaded_type_t loaded_type=helf_loader_type_from_elf_type(type);
    if(loaded_type==HELF_LOADED_TYPE_NONE)
    {
        return 0;
    }
    uint64_t image_size=0;
    size_t rx_size=0;
    size_t ro_size=0;
    size_t rw_size=0;
    uintptr_t min_vaddr=0;
    if(!helf_loader_image_size_get(input_file,loaded_type,&image_size,&rx_size,&ro_size,&rw_size,&min_vaddr))
    {
        return 0;
    }
    size_t symbol_count=0;
    size_t names_size=0;
    if(!helf_loader_symbols_size_get(input_file,&symbol_count,&names_size))
    {
        return 0;
    }
    if(symbol_count>SIZE_MAX/sizeof(hruntime_symbol_t))
    {
        return 0;
    }
    size_t symbol_table_bytes=symbol_count*sizeof(hruntime_symbol_t);
    size_t alignment=helf_loader_max_alignment_get(input_file);
    size_t total=alignment-1; /* buffer head alignment */
    if(loaded_type==HELF_LOADED_TYPE_RELOCATABLE)
    {
        total+=helf_loader_align_up(rx_size,alignment);
        total+=helf_loader_align_up(ro_size,alignment);
        total+=helf_loader_align_up(rw_size,alignment);
    }
    else
    {
        total+=helf_loader_align_up((size_t)image_size,alignment);
    }
    total+=sizeof(void *)-1; /* symbol table alignment */
    total+=symbol_table_bytes;
    total+=names_size;
    size_t toc_bytes=helf_loader_toc_bytes_get(input_file,machine,elfclass,loaded_type);
    total+=sizeof(uint64_t)-1; /* TOC alignment */
    total+=toc_bytes;
    return total;
}

size_t helf_namespace_loaded_buffer_total_size_get(helf_file_input_t *input_file)
{
    size_t data_size=helf_namespace_loaded_buffer_size_get(input_file);
    if(data_size==0)
    {
        return 0;
    }
    size_t alignment=helf_loader_max_alignment_get(input_file);
    if(alignment<sizeof(void *))
    {
        alignment=sizeof(void *);
    }
    size_t total=(sizeof(void *)-1); /* helf_loaded_t alignment */
    total+=sizeof(helf_loaded_t);
    total+=(alignment-1);            /* data alignment */
    total+=data_size;
    return total;
}

static const helf_load_param_t *helf_loader_param_get(helf_namespace_t *ns,const helf_load_param_t *param)
{
    if(param!=NULL)
    {
        return param;
    }
    if(ns!=NULL && ns->flags.load_param_set)
    {
        return &ns->load_param;
    }
    return NULL;
}

bool helf_namespace_loaded_init_ex(helf_namespace_t *ns,helf_loaded_t *loaded,helf_file_input_t *input_file,void *buffer,size_t buffer_size,const helf_load_param_t *param)
{
    if(ns==NULL || loaded==NULL || input_file==NULL || buffer==NULL)
    {
        return false;
    }
    const helf_load_param_t *load_param=helf_loader_param_get(ns,param);
    memset(loaded,0,sizeof(*loaded));
    hdoublylinkedlist_init(&loaded->list_head);
    uint16_t machine=HELF_EM_NONE;
    uint8_t elfclass=0;
    uint8_t elfdata=0;
    uint16_t type=0;
    if(!helf_loader_file_info_get(input_file,&machine,&elfclass,&elfdata,&type))
    {
        return false;
    }
    helf_loaded_type_t loaded_type=helf_loader_type_from_elf_type(type);
    if(loaded_type==HELF_LOADED_TYPE_NONE)
    {
        return false;
    }
    if(ns->machine!=HELF_EM_NONE && (ns->machine!=machine || ns->elfclass!=elfclass || ns->elfdata!=elfdata))
    {
        return false;
    }
    uint64_t image_size64=0;
    size_t rx_size=0;
    size_t ro_size=0;
    size_t rw_size=0;
    uintptr_t min_vaddr=0;
    if(!helf_loader_image_size_get(input_file,loaded_type,&image_size64,&rx_size,&ro_size,&rw_size,&min_vaddr))
    {
        return false;
    }
    size_t symbol_count=0;
    size_t names_size=0;
    if(!helf_loader_symbols_size_get(input_file,&symbol_count,&names_size))
    {
        return false;
    }
    size_t alignment=helf_loader_max_alignment_get(input_file);
    uintptr_t cursor=(uintptr_t)buffer;
    uintptr_t buffer_end=cursor+buffer_size;
    cursor=helf_loader_align_up(cursor,alignment);
    void *image=NULL;
    void *rx_image=NULL;
    void *ro_image=NULL;
    void *rw_image=NULL;
    size_t image_size=0;
    if(loaded_type==HELF_LOADED_TYPE_RELOCATABLE)
    {
        rx_image=(void *)cursor;
        cursor+=helf_loader_align_up(rx_size,alignment);
        ro_image=(void *)cursor;
        cursor+=helf_loader_align_up(ro_size,alignment);
        rw_image=(void *)cursor;
        cursor+=helf_loader_align_up(rw_size,alignment);
        image=rx_image;
        image_size=(size_t)(cursor-(uintptr_t)image);
    }
    else
    {
        image=(void *)cursor;
        image_size=helf_loader_align_up((size_t)image_size64,alignment);
        cursor+=image_size;
        rx_image=image;
        rx_size=image_size;
    }
    cursor=helf_loader_align_up(cursor,sizeof(void *));
    if(symbol_count>SIZE_MAX/sizeof(hruntime_symbol_t))
    {
        return false;
    }
    size_t symbol_table_bytes=symbol_count*sizeof(hruntime_symbol_t);
    hruntime_symbol_t *symbol_table=(hruntime_symbol_t *)cursor;
    cursor+=symbol_table_bytes;
    char *symbol_names=(char *)cursor;
    cursor+=names_size;
    size_t toc_bytes=helf_loader_toc_bytes_get(input_file,machine,elfclass,loaded_type);
    cursor=helf_loader_align_up(cursor,sizeof(uint64_t));
    loaded->toc=(toc_bytes!=0)?(void *)cursor:NULL;
    loaded->toc_size=toc_bytes;
    loaded->toc_used=0;
    loaded->toc_base=(uintptr_t)loaded->toc;
    cursor+=toc_bytes;
    if(cursor>buffer_end)
    {
        return false;
    }
    if(load_param!=NULL && load_param->flags.load_address && (uintptr_t)image!=load_param->load_address)
    {
        memset(loaded,0,sizeof(*loaded));
        return false;
    }
    loaded->type=loaded_type;
    loaded->machine=machine;
    loaded->elfclass=elfclass;
    loaded->elfdata=elfdata;
    if(!helf_loader_load_common(ns,loaded,input_file,image,image_size,rx_image,rx_size,ro_image,ro_size,rw_image,rw_size,min_vaddr,symbol_table,symbol_count,symbol_names,names_size,false,load_param))
    {
        memset(loaded,0,sizeof(*loaded));
        return false;
    }
    loaded->flags.image_owned=false;
    loaded->flags.symbols_owned=false;
    return true;
}

bool helf_namespace_loaded_init(helf_namespace_t *ns,helf_loaded_t *loaded,helf_file_input_t *input_file,void *buffer,size_t buffer_size)
{
    return helf_namespace_loaded_init_ex(ns,loaded,input_file,buffer,buffer_size,NULL);
}

helf_loaded_t *helf_namespace_loaded_buffer_new(helf_namespace_t *ns,helf_file_input_t *input_file,void *buffer,size_t buffer_size,const helf_load_param_t *param)
{
    if(ns==NULL || input_file==NULL || buffer==NULL)
    {
        return NULL;
    }
    size_t total=helf_namespace_loaded_buffer_total_size_get(input_file);
    if(total==0 || buffer_size<total)
    {
        return NULL;
    }
    size_t alignment=helf_loader_max_alignment_get(input_file);
    if(alignment<sizeof(void *))
    {
        alignment=sizeof(void *);
    }
    uintptr_t cursor=helf_loader_align_up((uintptr_t)buffer,sizeof(void *));
    uintptr_t end=(uintptr_t)buffer+buffer_size;
    if(cursor>end || sizeof(helf_loaded_t)>end-cursor)
    {
        return NULL;
    }
    helf_loaded_t *loaded=(helf_loaded_t *)cursor;
    cursor+=sizeof(helf_loaded_t);
    cursor=helf_loader_align_up(cursor,alignment);
    if(cursor>end)
    {
        return NULL;
    }
    if(!helf_namespace_loaded_init_ex(ns,loaded,input_file,(void *)cursor,(size_t)(end-cursor),param))
    {
        return NULL;
    }
    return loaded;
}

void helf_namespace_loaded_deinit(helf_namespace_t *ns,helf_loaded_t *loaded)
{
    if(loaded==NULL)
    {
        return;
    }
    helf_loader_namespace_unlink(ns,loaded);
    if(loaded->flags.image_owned && loaded->image!=NULL && loaded->image_size!=0)
    {
        hmunmap(loaded->image,loaded->image_size);
    }
    if(loaded->flags.symbols_owned && loaded->symbol_table!=NULL)
    {
        hfree(loaded->symbol_table);
    }
    if(loaded->flags.toc_owned && loaded->toc!=NULL)
    {
        hfree(loaded->toc);
    }
    memset(loaded,0,sizeof(*loaded));
}

static void *helf_loader_image_map(const helf_load_param_t *param,size_t image_size)
{
    void *request_address=NULL;
    int map_flags=HMMAN_MAP_ANONYMOUS|HMMAN_MAP_SHARED;
    if(param!=NULL && param->flags.load_address)
    {
        if((param->load_address%HELF_LOADER_PAGE_ALIGN)!=0)
        {
            return HMMAN_MAP_FAILED;
        }
        request_address=(void *)param->load_address;
        if(param->flags.fixed_address)
        {
            map_flags|=HMMAN_MAP_FIXED;
        }
    }
    void *image=hmmap(request_address,image_size,HMMAN_PROT_READ|HMMAN_PROT_WRITE|HMMAN_PROT_EXEC,map_flags,-1,0);
    if(image==NULL || image==HMMAN_MAP_FAILED)
    {
        return HMMAN_MAP_FAILED;
    }
    if(request_address!=NULL && image!=request_address)
    {
        /* 地址未被满足 */
        hmunmap(image,image_size);
        return HMMAN_MAP_FAILED;
    }
    return image;
}

static void *helf_loader_image_map_choose(helf_namespace_t *ns,const helf_load_param_t *param,bool default_address,size_t image_size)
{
    if(!default_address || ns==NULL || param==NULL)
    {
        return helf_loader_image_map(param,image_size);
    }
    size_t step=helf_loader_align_up(image_size,HELF_LOADER_RELOC_ADDRESS_STEP);
    if(step<HELF_LOADER_RELOC_ADDRESS_STEP)
    {
        step=HELF_LOADER_RELOC_ADDRESS_STEP;
    }
    for(size_t attempt=0; attempt<64; attempt++)
    {
        helf_load_param_t current=*param;
        current.load_address=ns->reloc_load_address+attempt*step;
        void *image=helf_loader_image_map(&current,image_size);
        if(image!=NULL && image!=HMMAN_MAP_FAILED)
        {
            ns->reloc_load_address=current.load_address+step;
            return image;
        }
    }
    return HMMAN_MAP_FAILED;
}

helf_loaded_t *helf_namespace_loaded_new_ex(helf_namespace_t *ns,helf_file_input_t *input_file,const helf_load_param_t *param)
{
    if(ns==NULL || input_file==NULL)
    {
        return NULL;
    }
    const helf_load_param_t *load_param=helf_loader_param_get(ns,param);
    uint16_t machine=HELF_EM_NONE;
    uint8_t elfclass=0;
    uint8_t elfdata=0;
    uint16_t type=0;
    if(!helf_loader_file_info_get(input_file,&machine,&elfclass,&elfdata,&type))
    {
        return NULL;
    }
    helf_loaded_type_t loaded_type=helf_loader_type_from_elf_type(type);
    if(loaded_type==HELF_LOADED_TYPE_NONE)
    {
        return NULL;
    }
    if(ns->machine!=HELF_EM_NONE && (ns->machine!=machine || ns->elfclass!=elfclass || ns->elfdata!=elfdata))
    {
        return NULL;
    }
    uint64_t image_size64=0;
    size_t rx_size=0;
    size_t ro_size=0;
    size_t rw_size=0;
    uintptr_t min_vaddr=0;
    if(!helf_loader_image_size_get(input_file,loaded_type,&image_size64,&rx_size,&ro_size,&rw_size,&min_vaddr))
    {
        return NULL;
    }
    size_t symbol_count=0;
    size_t names_size=0;
    if(!helf_loader_symbols_size_get(input_file,&symbol_count,&names_size))
    {
        return NULL;
    }
    helf_loaded_t *loaded=(helf_loaded_t *)hmalloc(sizeof(helf_loaded_t));
    if(loaded==NULL)
    {
        return NULL;
    }
    memset(loaded,0,sizeof(*loaded));
    hdoublylinkedlist_init(&loaded->list_head);
    loaded->type=loaded_type;
    loaded->machine=machine;
    loaded->elfclass=elfclass;
    loaded->elfdata=elfdata;
    void *image=NULL;
    size_t image_size=0;
    void *rx_image=NULL;
    void *ro_image=NULL;
    void *rw_image=NULL;
    bool default_address=(loaded_type==HELF_LOADED_TYPE_RELOCATABLE
                          && (load_param==NULL || !load_param->flags.load_address));
    helf_load_param_t default_param;
    const helf_load_param_t *map_param=load_param;
    if(default_address)
    {
        if(load_param!=NULL)
        {
            default_param=*load_param;
        }
        else
        {
            helf_load_param_init(&default_param);
        }
        default_param.flags.load_address=1;
        default_param.flags.fixed_address=0;
        default_param.load_address=ns->reloc_load_address;
        map_param=&default_param;
    }
    size_t toc_bytes=helf_loader_toc_bytes_get(input_file,machine,elfclass,loaded_type);
    if(loaded_type==HELF_LOADED_TYPE_RELOCATABLE)
    {
        size_t rx_map_size=helf_loader_align_up(rx_size,HELF_LOADER_PAGE_ALIGN);
        size_t ro_map_size=helf_loader_align_up(ro_size,HELF_LOADER_PAGE_ALIGN);
        size_t rw_map_size=helf_loader_align_up(rw_size,HELF_LOADER_PAGE_ALIGN);
        size_t toc_map_size=helf_loader_align_up(toc_bytes,HELF_LOADER_PAGE_ALIGN);
        image_size=rx_map_size+ro_map_size+rw_map_size+toc_map_size;
        if(image_size==0)
        {
            hfree(loaded);
            return NULL;
        }
        image=helf_loader_image_map_choose(ns,map_param,default_address,image_size);
        if(image==NULL || image==HMMAN_MAP_FAILED)
        {
            hfree(loaded);
            return NULL;
        }
        rx_image=image;
        ro_image=(uint8_t *)image+rx_map_size;
        rw_image=(uint8_t *)image+rx_map_size+ro_map_size;
        loaded->toc=(toc_bytes!=0)?((uint8_t *)image+rx_map_size+ro_map_size+rw_map_size):NULL;
        loaded->toc_size=toc_bytes;
        loaded->toc_used=0;
        loaded->toc_base=(uintptr_t)loaded->toc;
    }
    else
    {
        image_size=helf_loader_align_up((size_t)image_size64,HELF_LOADER_PAGE_ALIGN);
        if(image_size==0)
        {
            hfree(loaded);
            return NULL;
        }
        image=helf_loader_image_map_choose(ns,map_param,default_address,image_size);
        if(image==NULL || image==HMMAN_MAP_FAILED)
        {
            hfree(loaded);
            return NULL;
        }
        rx_image=image;
        rx_size=image_size;
    }
    if(symbol_count>SIZE_MAX/sizeof(hruntime_symbol_t))
    {
        hmunmap(image,image_size);
        hfree(loaded);
        return NULL;
    }
    size_t symbol_table_bytes=symbol_count*sizeof(hruntime_symbol_t);
    hruntime_symbol_t *symbol_table=(hruntime_symbol_t *)hmalloc(symbol_table_bytes+names_size);
    if(symbol_table==NULL && (symbol_table_bytes+names_size)!=0)
    {
        hmunmap(image,image_size);
        hfree(loaded);
        return NULL;
    }
    char *symbol_names=(char *)((uint8_t *)symbol_table+symbol_table_bytes);
    if(!helf_loader_load_common(ns,loaded,input_file,image,image_size,rx_image,rx_size,ro_image,ro_size,rw_image,rw_size,min_vaddr,symbol_table,symbol_count,symbol_names,names_size,true,map_param))
    {
        hmunmap(image,image_size);
        if(symbol_table!=NULL)
        {
            hfree(symbol_table);
        }
        hfree(loaded);
        return NULL;
    }
    loaded->flags.image_owned=true;
    loaded->flags.symbols_owned=true;
    loaded->flags.loaded_owned=true;
    return loaded;
}

helf_loaded_t *helf_namespace_loaded_new(helf_namespace_t *ns,helf_file_input_t *input_file)
{
    return helf_namespace_loaded_new_ex(ns,input_file,NULL);
}

void helf_namespace_loaded_delete(helf_namespace_t *ns,helf_loaded_t *loaded)
{
    if(loaded==NULL)
    {
        return;
    }
    bool loaded_owned=loaded->flags.loaded_owned;
    helf_namespace_loaded_deinit(ns,loaded);
    if(loaded_owned)
    {
        hfree(loaded);
    }
}

helf_namespace_t *helf_namespace_new(void *usr)
{
    helf_namespace_t *ns=(helf_namespace_t *)hmalloc(sizeof(helf_namespace_t));
    if(ns!=NULL)
    {
        memset(ns,0,sizeof(*ns));
        ns->usr=usr;
        ns->reloc_load_address=HELF_LOADER_RELOC_DEFAULT_ADDRESS;
    }
    return ns;
}

void helf_namespace_delete(helf_namespace_t *ns)
{
    if(ns==NULL)
    {
        return;
    }
    hdoublylinkedlist_head_t *list_head=ns->loaded_list_head;
    ns->loaded_list_head=NULL;
    for(hdoublylinkedlist_head_t *it=list_head; it!=NULL;)
    {
        hdoublylinkedlist_head_t *next=it->next;
        if(next==list_head || next==NULL || next==it)
        {
            next=NULL;
        }
        helf_loaded_t *loaded=(helf_loaded_t *)GET_STRUCT_PTR_BY_MEMBER_PTR(it,helf_loaded_t,list_head);
        helf_namespace_loaded_delete(ns,loaded);
        it=next;
    }
    hfree(ns);
}

const hruntime_symbol_t *helf_namespace_symbol_find(helf_namespace_t *ns,const char *name)
{
    if(ns==NULL || name==NULL || name[0]=='\0')
    {
        return NULL;
    }
    const hruntime_symbol_t *ret=NULL;
    HDOUBLYLINKEDLIST_FOREACH(ns->loaded_list_head,list_item)
    {
        helf_loaded_t *loaded=(helf_loaded_t *)GET_STRUCT_PTR_BY_MEMBER_PTR(list_item,helf_loaded_t,list_head);
        if(loaded->symbol_table!=NULL)
        {
            for(size_t i=0; i<loaded->symbol_table_size; i++)
            {
                if(loaded->symbol_table[i].symbol_name!=NULL && strcmp(name,loaded->symbol_table[i].symbol_name)==0)
                {
                    ret=&loaded->symbol_table[i];
                }
            }
        }
    }
    if(ret==NULL && ns->machine==helf_namespace_native_machine_type())
    {
        ret=hruntime_symbol_find(name);
    }
    return ret;
}
