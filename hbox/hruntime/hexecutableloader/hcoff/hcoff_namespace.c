/***************************************************************
 * Name:      hcoff_namespace.c
 * Purpose:   实现hcoff_namespace接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-27
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hcoff_namespace.h"
#include "hcoff_arch_i386.h"
#include "hcoff_arch_amd64.h"
#include "hcoff_arch_aarch64.h"
#include "hcoff_arch_arm.h"
#include "hcoff_arch_riscv.h"
#include "hcoff_arch_loongarch.h"
#include "hcoff_pe.h"

uint16_t hcoff_namespace_native_machine_type(void)
{
#if defined(_M_IX86) || defined(__i386__) || defined(i386) || defined(__i686__) || defined(i686)
    return HCOFF_FILEHEADER_F_MAGIC_I386;
#elif  defined(_M_X64) || defined(__x86_64) || defined(__x86_64__)
    return  HCOFF_FILEHEADER_F_MAGIC_AMD64;
#elif  defined(_M_ARM64) || defined(__aarch64__) || defined(__AARCH64EL__)
    return  HCOFF_FILEHEADER_F_MAGIC_ARM64;
#elif  defined(_M_ARM) || defined(__arm__) || defined(__thumb__) || defined(__ARMEL__)
    return  HCOFF_FILEHEADER_F_MAGIC_ARM;
#elif  defined(__riscv) && (__riscv_xlen == 64)
    return  HCOFF_FILEHEADER_F_MAGIC_RISCV64;
#elif  defined(__riscv) && (__riscv_xlen == 32)
    return  HCOFF_FILEHEADER_F_MAGIC_RISCV32;
#elif  defined(__loongarch64) || defined(__loongarch__)
    return  HCOFF_FILEHEADER_F_MAGIC_LOONGARCH64;
#else
    return HCOFF_FILEHEADER_F_MAGIC_UNKNOWN;
#endif
}

uint16_t hcoff_namespace_file_machine_type(hcoff_file_input_t *input_file)
{
    uint16_t ret=HCOFF_FILEHEADER_F_MAGIC_UNKNOWN;
    if(input_file!=NULL)
    {
        if(hcoff_pe_is_pe(input_file))
        {
            hcoff_pe_header_t pe_header= {0};
            if(hcoff_pe_header_read(&pe_header,input_file))
            {
                ret=pe_header.coff_header.f_magic;
            }
            return ret;
        }
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
        ret=filehdr.f_magic;
    }
    return ret;
}

bool hcoff_namespace_is_native_relocatable_file(hcoff_file_input_t *input_file)
{
    bool ret=false;
    if(input_file==NULL || hcoff_pe_is_pe(input_file))
    {
        //PE文件不是可重定位对象文件
        return ret;
    }
    ret=(hcoff_namespace_file_machine_type(input_file)==hcoff_namespace_native_machine_type()) && (hcoff_namespace_native_machine_type()!=HCOFF_FILEHEADER_F_MAGIC_UNKNOWN);
    return ret;
}

static  const hruntime_symbol_t *hcoff_namespace_external_symbol_find_with_usr(const char *name,void *usr)
{
    hcoff_namespace_t *ns=(hcoff_namespace_t *)usr;
    const hruntime_symbol_t *ret=hcoff_namespace_symbol_find(ns,name);
    if(ret==NULL && ns!=NULL)
    {
        if(ns->machine_type==hcoff_namespace_native_machine_type())
        {
            /*
             * 本地可重定向对象可使用运行时符号
             */
            ret=hruntime_symbol_find(name);
        }
    }
    return ret;
}

static bool hcoff_namespace_machine_is_generic(uint16_t machine_type)
{
    switch(machine_type)
    {
    case HCOFF_FILEHEADER_F_MAGIC_ARM:
    case HCOFF_FILEHEADER_F_MAGIC_ARMNT:
    case HCOFF_FILEHEADER_F_MAGIC_THUMB:
    case HCOFF_FILEHEADER_F_MAGIC_RISCV32:
    case HCOFF_FILEHEADER_F_MAGIC_RISCV64:
    case HCOFF_FILEHEADER_F_MAGIC_LOONGARCH64:
        return true;
    default:
        return false;
    }
}

static void hcoff_namespace_relocatable_link(hcoff_namespace_t *ns,hcoff_namespace_relocatable_t *namespace_relocatable)
{
    if(ns==NULL || namespace_relocatable==NULL)
    {
        return;
    }
    //添加至链表
    if(ns->relocatable_list_head==NULL)
    {
        ns->relocatable_list_head=&namespace_relocatable->relocatable_list_head;
    }
    else
    {
        hdoublylinkedlist_add_front(ns->relocatable_list_head,&namespace_relocatable->relocatable_list_head);
    }
}

static void hcoff_namespace_relocatable_unlink(hcoff_namespace_t *ns,hcoff_namespace_relocatable_t *namespace_relocatable)
{
    if(namespace_relocatable==NULL)
    {
        return;
    }
    if(ns!=NULL && &namespace_relocatable->relocatable_list_head==ns->relocatable_list_head)
    {
        ns->relocatable_list_head=ns->relocatable_list_head->next;
        if(&namespace_relocatable->relocatable_list_head==ns->relocatable_list_head)
        {
            ns->relocatable_list_head=NULL;
        }
    }
    hdoublylinkedlist_remove(&namespace_relocatable->relocatable_list_head);
}

bool hcoff_namespace_relocatable_init(hcoff_namespace_t *ns,hcoff_namespace_relocatable_t *namespace_relocatable,hcoff_file_input_t *input_file,void *buffer,size_t buffer_size)
{
    if(ns==NULL || namespace_relocatable==NULL || input_file==NULL || buffer==NULL)
    {
        return false;
    }
    memset(namespace_relocatable,0,sizeof(hcoff_namespace_relocatable_t));
    hdoublylinkedlist_init(&namespace_relocatable->relocatable_list_head);
    uint16_t machine_type=hcoff_namespace_file_machine_type(input_file);
    if(ns->machine_type!=HCOFF_FILEHEADER_F_MAGIC_UNKNOWN && ns->machine_type!=machine_type)
    {
        return false;
    }
    if(!hcoff_namespace_machine_is_generic(machine_type))
    {
        //此接口仅支持通用可重定位对象,旧架构请使用hcoff_namespace_relocatable_new
        return false;
    }
    namespace_relocatable->machine_type=machine_type;
    namespace_relocatable->relocatable_handle=&namespace_relocatable->generic_relocatable;
    if(!hcoff_relocatable_init(&namespace_relocatable->generic_relocatable,input_file,machine_type,buffer,buffer_size,hcoff_namespace_external_symbol_find_with_usr,ns))
    {
        memset(namespace_relocatable,0,sizeof(hcoff_namespace_relocatable_t));
        return false;
    }
    if(ns->machine_type==HCOFF_FILEHEADER_F_MAGIC_UNKNOWN)
    {
        ns->machine_type=machine_type;
    }
    hcoff_namespace_relocatable_link(ns,namespace_relocatable);
    return true;
}

void hcoff_namespace_relocatable_deinit(hcoff_namespace_t *ns,hcoff_namespace_relocatable_t *namespace_relocatable)
{
    if(namespace_relocatable==NULL)
    {
        return;
    }
    hcoff_namespace_relocatable_unlink(ns,namespace_relocatable);
    if(namespace_relocatable->relocatable_handle!=NULL)
    {
        void *handle=(void *)namespace_relocatable->relocatable_handle;
        namespace_relocatable->relocatable_handle=NULL;
        if(hcoff_namespace_machine_is_generic(namespace_relocatable->machine_type))
        {
            hcoff_relocatable_t *generic_relocatable=(hcoff_relocatable_t *)handle;
            void *image=generic_relocatable->image;
            size_t image_size=generic_relocatable->image_size;
            bool buffer_owned=namespace_relocatable->relocatable_buffer_owned;
            hcoff_relocatable_deinit(generic_relocatable);
            if(buffer_owned && image!=NULL && image_size!=0)
            {
                hmunmap(image,image_size);
            }
        }
        else
        {
            switch(namespace_relocatable->machine_type)
            {
            case HCOFF_FILEHEADER_F_MAGIC_I386:
            {
                hcoff_i386_relocatable_delete((hcoff_i386_relocatable_t *)handle);
            }
            break;
            case HCOFF_FILEHEADER_F_MAGIC_AMD64:
            {
                hcoff_amd64_relocatable_delete((hcoff_amd64_relocatable_t *)handle);
            }
            break;
            case HCOFF_FILEHEADER_F_MAGIC_ARM64:
            {
                hcoff_aarch64_relocatable_delete((hcoff_aarch64_relocatable_t *)handle);
            }
            break;
            default:
            {
            }
            break;
            }
        }
    }
    memset(namespace_relocatable,0,sizeof(hcoff_namespace_relocatable_t));
}

hcoff_namespace_relocatable_t *hcoff_namespace_relocatable_new(hcoff_namespace_t *ns,hcoff_file_input_t *input_file)
{
    if(ns==NULL || input_file==NULL)
    {
        return NULL;
    }
    uint16_t machine_type=hcoff_namespace_file_machine_type(input_file);
    if(ns->machine_type!=HCOFF_FILEHEADER_F_MAGIC_UNKNOWN && machine_type!=ns->machine_type)
    {
        return NULL;
    }
    hcoff_namespace_relocatable_t *ret=(hcoff_namespace_relocatable_t *)hmalloc(sizeof(hcoff_namespace_relocatable_t));
    if(ret==NULL)
    {
        return ret;
    }
    memset(ret,0,sizeof(hcoff_namespace_relocatable_t));
    hdoublylinkedlist_init(&ret->relocatable_list_head);
    ret->machine_type=machine_type;
    if(hcoff_namespace_machine_is_generic(machine_type))
    {
        size_t image_size=hcoff_relocatable_image_size_get(input_file,machine_type);
        if(image_size==0)
        {
            hfree(ret);
            return NULL;
        }
        void *image=hmmap(NULL,image_size,HMMAN_PROT_READ | HMMAN_PROT_WRITE | HMMAN_PROT_EXEC,HMMAN_MAP_ANONYMOUS | HMMAN_MAP_SHARED,(-1),0);
        if(image==NULL)
        {
            hfree(ret);
            return NULL;
        }
        if(!hcoff_namespace_relocatable_init(ns,ret,input_file,image,image_size))
        {
            hmunmap(image,image_size);
            hfree(ret);
            return NULL;
        }
        ret->relocatable_buffer_owned=true;
        return ret;
    }
    switch(ret->machine_type)
    {
    case HCOFF_FILEHEADER_F_MAGIC_I386:
    {
        ret->relocatable_handle=hcoff_i386_relocatable_new_with_usr(input_file,hcoff_namespace_external_symbol_find_with_usr,ns);
    }
    break;
    case HCOFF_FILEHEADER_F_MAGIC_AMD64:
    {
        ret->relocatable_handle=hcoff_amd64_relocatable_new_with_usr(input_file,hcoff_namespace_external_symbol_find_with_usr,ns);
    }
    break;
    case HCOFF_FILEHEADER_F_MAGIC_ARM64:
    {
        ret->relocatable_handle=hcoff_aarch64_relocatable_new_with_usr(input_file,hcoff_namespace_external_symbol_find_with_usr,ns);
    }
    break;
    default:
    {
    }
    break;
    }
    if(ret->relocatable_handle==NULL)
    {
        hfree(ret);
        return NULL;
    }
    if(ns->machine_type==HCOFF_FILEHEADER_F_MAGIC_UNKNOWN)
    {
        ns->machine_type=ret->machine_type;
    }
    hcoff_namespace_relocatable_link(ns,ret);
    return ret;
}

void hcoff_namespace_relocatable_delete(hcoff_namespace_t *ns,hcoff_namespace_relocatable_t *namespace_relocatable)
{
    if(namespace_relocatable!=NULL)
    {
        hcoff_namespace_relocatable_deinit(ns,namespace_relocatable);
        hfree(namespace_relocatable);
    }
}

hcoff_namespace_t * hcoff_namespace_new(void *usr)
{
    hcoff_namespace_t * ret=(hcoff_namespace_t *)hmalloc(sizeof(hcoff_namespace_t));
    if(ret!=NULL)
    {
        memset(ret,0,sizeof(hcoff_namespace_t));
        ret->usr=usr;
    }
    return ret;
}

const hruntime_symbol_t *hcoff_namespace_symbol_find(hcoff_namespace_t * ns,const char *name)
{
    const hruntime_symbol_t *ret=NULL;
    if(ns==NULL || name==NULL || name[0]=='\0')
    {
        return ret;
    }
    HDOUBLYLINKEDLIST_FOREACH(ns->relocatable_list_head,list_item)
    {
        hcoff_namespace_relocatable_t * temp=(hcoff_namespace_relocatable_t *)GET_STRUCT_PTR_BY_MEMBER_PTR(list_item,hcoff_namespace_relocatable_t,relocatable_list_head);
        switch(temp->machine_type)
        {
        case HCOFF_FILEHEADER_F_MAGIC_I386:
        {
            const hcoff_i386_relocatable_t *relocatable=(const hcoff_i386_relocatable_t *)temp->relocatable_handle;
            if(relocatable!=NULL && relocatable->symbol_table!=NULL &&relocatable->symbol_table_size > 0)
            {
                for(size_t i=0; i<relocatable->symbol_table_size; i++)
                {
                    if((relocatable->symbol_table[i].symbol_name!=NULL) && (0==strcmp(name,relocatable->symbol_table[i].symbol_name)))
                    {
                        ret=&relocatable->symbol_table[i];
                    }
                }
            }
        }
        break;
        case HCOFF_FILEHEADER_F_MAGIC_AMD64:
        {
            const hcoff_amd64_relocatable_t *relocatable=(const hcoff_amd64_relocatable_t *)temp->relocatable_handle;
            if(relocatable!=NULL && relocatable->symbol_table!=NULL &&relocatable->symbol_table_size > 0)
            {
                for(size_t i=0; i<relocatable->symbol_table_size; i++)
                {
                    if((relocatable->symbol_table[i].symbol_name!=NULL) && (0==strcmp(name,relocatable->symbol_table[i].symbol_name)))
                    {
                        ret=&relocatable->symbol_table[i];
                    }
                }
            }
        }
        break;
        case HCOFF_FILEHEADER_F_MAGIC_ARM64:
        {
            const hcoff_aarch64_relocatable_t *relocatable=(const hcoff_aarch64_relocatable_t *)temp->relocatable_handle;
            if(relocatable!=NULL && relocatable->symbol_table!=NULL &&relocatable->symbol_table_size > 0)
            {
                for(size_t i=0; i<relocatable->symbol_table_size; i++)
                {
                    if((relocatable->symbol_table[i].symbol_name!=NULL) && (0==strcmp(name,relocatable->symbol_table[i].symbol_name)))
                    {
                        ret=&relocatable->symbol_table[i];
                    }
                }
            }
        }
        break;
        case HCOFF_FILEHEADER_F_MAGIC_ARM:
        case HCOFF_FILEHEADER_F_MAGIC_ARMNT:
        case HCOFF_FILEHEADER_F_MAGIC_THUMB:
        case HCOFF_FILEHEADER_F_MAGIC_RISCV32:
        case HCOFF_FILEHEADER_F_MAGIC_RISCV64:
        case HCOFF_FILEHEADER_F_MAGIC_LOONGARCH64:
        {
            hcoff_relocatable_t *relocatable=(hcoff_relocatable_t *)temp->relocatable_handle;
            const hruntime_symbol_t *symbol=hcoff_relocatable_symbol_find(relocatable,name);
            if(symbol!=NULL)
            {
                ret=symbol;
            }
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

void hcoff_namespace_delete(hcoff_namespace_t *ns)
{
    if(ns!=NULL)
    {
        hdoublylinkedlist_head_t *list_head=ns->relocatable_list_head;
        ns->relocatable_list_head=NULL;
        for(hdoublylinkedlist_head_t *it=list_head; it!=NULL;)
        {
            hdoublylinkedlist_head_t *next=it->next;
            if(next==list_head || next == NULL || next==it)
            {
                next=NULL;
            }
            hcoff_namespace_relocatable_t * temp=(hcoff_namespace_relocatable_t *)GET_STRUCT_PTR_BY_MEMBER_PTR(it,hcoff_namespace_relocatable_t,relocatable_list_head);
            hcoff_namespace_relocatable_delete(ns,temp);
            it=next;
        }
        hfree(ns);
    }
}
