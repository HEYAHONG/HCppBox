/***************************************************************
 * Name:      hcoff_relocatable.c
 * Purpose:   实现hcoff通用可重定位对象接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-11-20
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hcoff_relocatable.h"
#include "hdefaults.h"
#include "hmemory.h"
#include "hruntime.h"
#include "string.h"

typedef struct
{
    hcoff_sectionheader_t header;
    size_t offset;
    size_t size;
    bool alloc;
    bool copy;
} hcoff_relocatable_section_t;

#define HCOFF_RELOCATABLE_ALIGNMENT     16

static bool hcoff_relocatable_section_is_alloc(const hcoff_sectionheader_t *sectionheader,const char *name)
{
    if(sectionheader==NULL)
    {
        return false;
    }
    uint32_t flags=sectionheader->s_flags;
    if((flags & HCOFF_SECTIONHEADER_S_FLAGS_IMAGE_SCN_MEM_DISCARDABLE)!=0)
    {
        //丢弃节(通常为调试信息)不参与加载
        return false;
    }
    if(name!=NULL)
    {
        if(0==strncmp(name,".debug",6) || 0==strncmp(name,".llvm",5) || 0==strncmp(name,".drectve",8) || 0==strncmp(name,".note",5))
        {
            return false;
        }
    }
    if((flags & (HCOFF_SECTIONHEADER_S_FLAGS_IMAGE_SCN_CNT_CODE |
                 HCOFF_SECTIONHEADER_S_FLAGS_IMAGE_SCN_CNT_INITIALIZED_DATA |
                 HCOFF_SECTIONHEADER_S_FLAGS_IMAGE_SCN_CNT_UNINITIALIZED_DATA |
                 HCOFF_SECTIONHEADER_S_FLAGS_IMAGE_SCN_MEM_READ |
                 HCOFF_SECTIONHEADER_S_FLAGS_IMAGE_SCN_MEM_WRITE |
                 HCOFF_SECTIONHEADER_S_FLAGS_IMAGE_SCN_MEM_EXECUTE))!=0)
    {
        return true;
    }
    return false;
}

static size_t hcoff_relocatable_align(size_t value,size_t alignment)
{
    if(alignment==0)
    {
        return value;
    }
    return (value+alignment-1)/alignment*alignment;
}

static bool hcoff_relocatable_symbol_is_defined(const hcoff_symbol_entry_t *symbol_entry)
{
    if(symbol_entry==NULL)
    {
        return false;
    }
    if(symbol_entry->e_scnum > 0 || symbol_entry->e_scnum == (uint16_t)-1)
    {
        return true;
    }
    return false;
}

static bool hcoff_relocatable_section_read(hcoff_file_input_t *input_file,size_t index,hcoff_relocatable_section_t *section)
{
    if(input_file==NULL || section==NULL)
    {
        return false;
    }
    memset(section,0,sizeof(hcoff_relocatable_section_t));
    if(!hcoff_sectionheader_read(&section->header,index,input_file))
    {
        return false;
    }
    char section_name[64]= {0};
    hcoff_sectionheader_name_read(&section->header,input_file,section_name,sizeof(section_name));
    section->alloc=hcoff_relocatable_section_is_alloc(&section->header,section_name);
    section->size=section->header.s_size;
    if(section->size < section->header.s_paddr)
    {
        section->size=section->header.s_paddr;
    }
    section->copy=(section->header.s_size!=0 && section->header.s_scnptr!=0);
    return true;
}

static bool hcoff_relocatable_section_layout_get(hcoff_file_input_t *input_file,size_t index,hcoff_relocatable_section_t *section)
{
    size_t offset=0;
    for(size_t i=0; ; i++)
    {
        hcoff_relocatable_section_t current= {0};
        if(!hcoff_relocatable_section_read(input_file,i,&current))
        {
            return false;
        }
        if(current.alloc)
        {
            offset=hcoff_relocatable_align(offset,HCOFF_RELOCATABLE_ALIGNMENT);
            current.offset=offset;
            offset+=current.size;
        }
        if(i==index)
        {
            *section=current;
            return true;
        }
    }
}

static bool hcoff_relocatable_external_symbol_address_get(hcoff_relocatable_t *relocatable,const char *name,uintptr_t *address)
{
    if(relocatable==NULL || name==NULL || address==NULL || relocatable->extern_symbol==NULL)
    {
        return false;
    }
    const struct hruntime_symbol *symbol=relocatable->extern_symbol(name,relocatable->usr);
    if(symbol==NULL && name[0]=='_')
    {
        symbol=relocatable->extern_symbol(name+1,relocatable->usr);
    }
    if(symbol==NULL && name[0]!='_')
    {
        char buffer[256]= {0};
        if(strlen(name)+2 <= sizeof(buffer))
        {
            buffer[0]='_';
            strcpy(buffer+1,name);
            symbol=relocatable->extern_symbol(buffer,relocatable->usr);
        }
    }
    if(symbol==NULL)
    {
        return false;
    }
    *address=symbol->symbol_addr;
    return true;
}

static bool hcoff_relocatable_symbol_address_get(hcoff_relocatable_t *relocatable,uint32_t symbol_index,uintptr_t *address)
{
    if(relocatable==NULL || address==NULL || relocatable->input_file==NULL)
    {
        return false;
    }
    hcoff_symbol_entry_t symbol_entry= {0};
    if(!hcoff_symbol_entry_read_at(&symbol_entry,symbol_index,relocatable->input_file))
    {
        return false;
    }
    if(symbol_entry.e_scnum > 0)
    {
        hcoff_fileheader_t fileheader= {0};
        if(!hcoff_fileheader_read_at(&fileheader,relocatable->input_file,0))
        {
            return false;
        }
        if(symbol_entry.e_scnum > fileheader.f_nscns)
        {
            return false;
        }
        hcoff_relocatable_section_t section= {0};
        if(!hcoff_relocatable_section_layout_get(relocatable->input_file,symbol_entry.e_scnum-1,&section))
        {
            return false;
        }
        if(!section.alloc)
        {
            return false;
        }
        *address=(uintptr_t)relocatable->image+section.offset+(symbol_entry.e_value-section.header.s_vaddr);
        return true;
    }
    if(symbol_entry.e_scnum == (uint16_t)-1)
    {
        //绝对符号
        *address=(uintptr_t)symbol_entry.e_value;
        return true;
    }
    if(symbol_entry.e_scnum == (uint16_t)-2)
    {
        //调试符号
        *address=0;
        return true;
    }
    if(symbol_entry.e_scnum == 0)
    {
        char name[256]= {0};
        if(!hcoff_symbol_entry_name_read(&symbol_entry,relocatable->input_file,name,sizeof(name)))
        {
            return false;
        }
        return hcoff_relocatable_external_symbol_address_get(relocatable,name,address);
    }
    return false;
}

static bool hcoff_relocatable_relocation_apply(hcoff_relocatable_t *relocatable)
{
    hcoff_fileheader_t fileheader= {0};
    if(!hcoff_fileheader_read_at(&fileheader,relocatable->input_file,0))
    {
        return false;
    }
    for(size_t i=0; i<fileheader.f_nscns; i++)
    {
        hcoff_relocatable_section_t section= {0};
        if(!hcoff_relocatable_section_layout_get(relocatable->input_file,i,&section))
        {
            return false;
        }
        if(!section.alloc)
        {
            continue;
        }
        size_t relocation_count=hcoff_section_relocation_count_get(&section.header,relocatable->input_file);
        for(size_t j=0; j<relocation_count; j++)
        {
            hcoff_section_relocation_t relocation= {0};
            if(!hcoff_section_relocation_read(&relocation,j,&section.header,relocatable->input_file))
            {
                return false;
            }
            const hcoff_arch_relocation_info_t *relocation_info=hcoff_arch_relocation_info_get(relocatable->machine_type,relocation.r_type);
            if((relocation.r_type==0) || (relocation_info!=NULL && (relocation_info->flags & HCOFF_ARCH_RELOCATION_FLAG_IGNORED)!=0))
            {
                //绝对重定位(0)仅为填充条目
                continue;
            }
            if(relocation_info==NULL)
            {
                //不支持的重定位类型
                return false;
            }
            uintptr_t symbol_address=0;
            if(!hcoff_relocatable_symbol_address_get(relocatable,relocation.r_symndx,&symbol_address))
            {
                return false;
            }
            if(relocation.r_vaddr < section.header.s_vaddr)
            {
                return false;
            }
            size_t relocation_offset=section.offset+(relocation.r_vaddr-section.header.s_vaddr);
            if(relocation_offset >= relocatable->image_size)
            {
                return false;
            }
            uint8_t *relocation_addr=(uint8_t *)relocatable->image+relocation_offset;
            if(relocatable->arch->relocation_doit==NULL)
            {
                return false;
            }
            if(!relocatable->arch->relocation_doit(relocation_addr,symbol_address,(uintptr_t)relocation_addr,(uintptr_t)relocatable->image,relocation.r_type))
            {
                return false;
            }
        }
    }
    return true;
}

size_t hcoff_relocatable_image_size_get(hcoff_file_input_t *input_file,uint16_t machine)
{
    if(input_file==NULL)
    {
        return 0;
    }
    hcoff_fileheader_t fileheader= {0};
    if(!hcoff_fileheader_read_at(&fileheader,input_file,0))
    {
        return 0;
    }
    if(fileheader.f_magic != machine || !hcoff_fileheader_is_relocatable_object_file(&fileheader))
    {
        return 0;
    }
    size_t image_size=0;
    for(size_t i=0; i<fileheader.f_nscns; i++)
    {
        hcoff_relocatable_section_t section= {0};
        if(!hcoff_relocatable_section_read(input_file,i,&section))
        {
            return 0;
        }
        if(!section.alloc)
        {
            continue;
        }
        image_size=hcoff_relocatable_align(image_size,HCOFF_RELOCATABLE_ALIGNMENT);
        image_size+=section.size;
    }
    if(image_size==0)
    {
        image_size=1;
    }
    return image_size;
}

bool hcoff_relocatable_init(hcoff_relocatable_t *relocatable,hcoff_file_input_t *input_file,uint16_t machine,void *buffer,size_t buffer_size,hcoff_relocatable_external_symbol_find_t extern_symbol,void *usr)
{
    if(relocatable==NULL || input_file==NULL || buffer==NULL)
    {
        return false;
    }
    memset(relocatable,0,sizeof(hcoff_relocatable_t));
    hcoff_fileheader_t fileheader= {0};
    if(!hcoff_fileheader_read_at(&fileheader,input_file,0))
    {
        return false;
    }
    if(fileheader.f_magic != machine || !hcoff_fileheader_is_relocatable_object_file(&fileheader))
    {
        return false;
    }
    const hcoff_arch_ops_t *arch=hcoff_arch_ops_get(machine);
    if(arch==NULL)
    {
        return false;
    }
    size_t image_size=hcoff_relocatable_image_size_get(input_file,machine);
    if(image_size==0 || image_size > buffer_size)
    {
        return false;
    }
    relocatable->machine_type=machine;
    relocatable->arch=arch;
    relocatable->input_file=input_file;
    relocatable->image=buffer;
    relocatable->image_size=image_size;
    relocatable->buffer_size=buffer_size;
    relocatable->extern_symbol=extern_symbol;
    relocatable->usr=usr;
    for(size_t i=0; i<fileheader.f_nscns; i++)
    {
        hcoff_relocatable_section_t section= {0};
        if(!hcoff_relocatable_section_layout_get(input_file,i,&section))
        {
            hcoff_relocatable_deinit(relocatable);
            return false;
        }
        if(!section.alloc || !section.copy)
        {
            continue;
        }
        if(section.header.s_size > hcoff_file_input_read(input_file,section.header.s_scnptr,(uint8_t *)relocatable->image+section.offset,section.header.s_size))
        {
            hcoff_relocatable_deinit(relocatable);
            return false;
        }
    }
    if(!hcoff_relocatable_relocation_apply(relocatable))
    {
        hcoff_relocatable_deinit(relocatable);
        return false;
    }
    relocatable->initialized=true;
    return true;
}

void hcoff_relocatable_deinit(hcoff_relocatable_t *relocatable)
{
    if(relocatable!=NULL)
    {
        memset(relocatable,0,sizeof(hcoff_relocatable_t));
    }
}

const hcoff_relocatable_t *hcoff_relocatable_new_with_usr(hcoff_file_input_t *input_file,uint16_t machine,hcoff_relocatable_external_symbol_find_t extern_symbol,void *usr)
{
    if(input_file==NULL)
    {
        return NULL;
    }
    size_t image_size=hcoff_relocatable_image_size_get(input_file,machine);
    if(image_size==0)
    {
        return NULL;
    }
    hcoff_relocatable_t *relocatable=(hcoff_relocatable_t *)hmalloc(sizeof(hcoff_relocatable_t));
    if(relocatable==NULL)
    {
        return NULL;
    }
    memset(relocatable,0,sizeof(hcoff_relocatable_t));
    void *image=hmmap(NULL,image_size,HMMAN_PROT_READ | HMMAN_PROT_WRITE | HMMAN_PROT_EXEC,HMMAN_MAP_ANONYMOUS | HMMAN_MAP_SHARED,(-1),0);
    if(image==NULL)
    {
        hfree(relocatable);
        return NULL;
    }
    if(!hcoff_relocatable_init(relocatable,input_file,machine,image,image_size,extern_symbol,usr))
    {
        hmunmap(image,image_size);
        hfree(relocatable);
        return NULL;
    }
    relocatable->buffer_owned=true;
    return relocatable;
}

void hcoff_relocatable_delete(const hcoff_relocatable_t * relocatable)
{
    hcoff_relocatable_t *ptr=(hcoff_relocatable_t *)relocatable;
    if(ptr!=NULL)
    {
        void *image=ptr->image;
        size_t image_size=ptr->image_size;
        bool buffer_owned=ptr->buffer_owned;
        hcoff_relocatable_deinit(ptr);
        if(buffer_owned && image!=NULL && image_size!=0)
        {
            hmunmap(image,image_size);
        }
        hfree(ptr);
    }
}

static struct hruntime_symbol hcoff_relocatable_symbol_cache;
static char hcoff_relocatable_symbol_name_cache[256];

const struct hruntime_symbol *hcoff_relocatable_symbol_find(hcoff_relocatable_t *relocatable,const char *name)
{
    if(relocatable==NULL || name==NULL || relocatable->input_file==NULL)
    {
        return NULL;
    }
    hcoff_fileheader_t fileheader= {0};
    if(!hcoff_fileheader_read_at(&fileheader,relocatable->input_file,0))
    {
        return NULL;
    }
    char symbol_name[256]= {0};
    for(size_t i=0; i<fileheader.f_nsyms; )
    {
        hcoff_symbol_entry_t symbol_entry= {0};
        if(!hcoff_symbol_entry_read_at(&symbol_entry,i,relocatable->input_file))
        {
            break;
        }
        if(hcoff_relocatable_symbol_is_defined(&symbol_entry))
        {
            if(hcoff_symbol_entry_name_read(&symbol_entry,relocatable->input_file,symbol_name,sizeof(symbol_name)))
            {
                if(symbol_name[0]!='\0' && 0==strcmp(symbol_name,name))
                {
                    uintptr_t symbol_address=0;
                    if(hcoff_relocatable_symbol_address_get(relocatable,(uint32_t)i,&symbol_address))
                    {
                        size_t name_length=strlen(name);
                        if(name_length >= sizeof(hcoff_relocatable_symbol_name_cache))
                        {
                            name_length=sizeof(hcoff_relocatable_symbol_name_cache)-1;
                        }
                        memcpy(hcoff_relocatable_symbol_name_cache,name,name_length);
                        hcoff_relocatable_symbol_name_cache[name_length]='\0';
                        hcoff_relocatable_symbol_cache.symbol_name=hcoff_relocatable_symbol_name_cache;
                        hcoff_relocatable_symbol_cache.symbol_addr=symbol_address;
                        return &hcoff_relocatable_symbol_cache;
                    }
                    return NULL;
                }
            }
        }
        i+=1+symbol_entry.e_numaux;
    }
    return NULL;
}
