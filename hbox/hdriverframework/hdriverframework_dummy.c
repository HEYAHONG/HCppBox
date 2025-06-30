/***************************************************************
 * Name:      hdriverframework_dummy.c
 * Purpose:   实现hdriverframework_dummy接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-29
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdriverframework_dummy.h"
#include "hmemory.h"
#include "stdarg.h"

static int hdriverframework_driver_base_dummy_process(hdriverframework_driver_base_t *drv,int op,...)
{
    hdriverframework_driver_dummy_t *drv_dummy=(hdriverframework_driver_dummy_t *)GET_STRUCT_PTR_BY_MEMBER_PTR(drv,hdriverframework_driver_dummy_t,base);
    int ret=HDRIVERFRAMEWORK_EPERM;
    if(drv_dummy==NULL)
    {
        return ret;
    }
    va_list va;
    va_start(va,op);
    switch(op)
    {
    case HDRIVERFRAMEWORK_OP_GETNAME:
    {
        const char **name_ptr=va_arg(va,const char **);
        if(name_ptr!=NULL)
        {
            (*name_ptr)="dummy";
            ret=HDRIVERFRAMEWORK_EOK;
        }
    }
    break;
    case HDRIVERFRAMEWORK_OP_INIT:
    {
        ret=HDRIVERFRAMEWORK_EOK;
    }
    break;
    case HDRIVERFRAMEWORK_OP_DEINIT:
    {
        ret=HDRIVERFRAMEWORK_EOK;
    }
    break;
    case HDRIVERFRAMEWORK_OP_GET_OF_COMPATIBLE:
    {
        /*
         * 设备树兼容性
         */
        static const char *of_compatible[]=
        {
            "hdriverframework,dummy",
            NULL
        };
        const char ***of_compatible_ptr=va_arg(va,const char ***);
        if(of_compatible_ptr!=NULL)
        {
            (*of_compatible_ptr)=of_compatible;
            ret=HDRIVERFRAMEWORK_EOK;
        }
    }
    break;
    case HDRIVERFRAMEWORK_OP_VM_READ:
    {
        size_t *ret_ptr=va_arg(va,size_t *);
        const hsoftwarevirtualmemory_map_item_t *map_item=va_arg(va,const hsoftwarevirtualmemory_map_item_t *);
        uintptr_t address=va_arg(va,uintptr_t);
        uint8_t *data=va_arg(va,uint8_t *);
        size_t length=va_arg(va,size_t);
        if(ret_ptr!=NULL && map_item!=NULL && address >= map_item->map_address && (address+length) <= (map_item->map_address+map_item->map_size) && data != NULL)
        {
            /*
             * 此处处理读取请求，不支持需要返回0
             */
            (*ret_ptr)=0;
            ret=HDRIVERFRAMEWORK_EOK;
        }
    }
    break;
    case HDRIVERFRAMEWORK_OP_VM_WRITE:
    {
        size_t *ret_ptr=va_arg(va,size_t *);
        const hsoftwarevirtualmemory_map_item_t *map_item=va_arg(va,const hsoftwarevirtualmemory_map_item_t *);
        uintptr_t address=va_arg(va,uintptr_t);
        const uint8_t *data=va_arg(va,const uint8_t *);
        size_t length=va_arg(va,size_t);
        if(ret_ptr!=NULL && map_item!=NULL && address >= map_item->map_address && (address+length) <= (map_item->map_address+map_item->map_size) && data != NULL)
        {
            /*
             * 此处处理读取请求，不支持需要返回0
             */
            (*ret_ptr)=0;
            ret=HDRIVERFRAMEWORK_EOK;
        }
    }
    break;
    default:
    {

    }
    break;
    }
    va_end(va);
    return ret;
}

static void hdriverframework_driver_base_dummy_check(hdriverframework_driver_dummy_t *drv)
{
    if(drv!=NULL)
    {
        if(drv->base.process==NULL)
        {
            hdriverframework_driver_base_set(&drv->base,hdriverframework_driver_base_dummy_process,NULL);
        }
    }
}

const char * hdriverframework_driver_dummy_getname(hdriverframework_driver_dummy_t *drv)
{
    hdriverframework_driver_base_dummy_check(drv);
    if(drv==NULL)
    {
        return NULL;
    }
    return hdriverframework_driver_base_getname(&drv->base);
}

int hdriverframework_driver_dummy_init(hdriverframework_driver_dummy_t *drv)
{
    hdriverframework_driver_base_dummy_check(drv);
    if(drv==NULL)
    {
        return HDRIVERFRAMEWORK_EPERM;
    }
    return hdriverframework_driver_base_init(&drv->base);
}

int hdriverframework_driver_dummy_deinit(hdriverframework_driver_dummy_t *drv)
{
    hdriverframework_driver_base_dummy_check(drv);
    if(drv==NULL)
    {
        return HDRIVERFRAMEWORK_EPERM;
    }
    return hdriverframework_driver_base_deinit(&drv->base);
}

const char ** hdriverframework_driver_dummy_get_of_compatible(hdriverframework_driver_dummy_t *drv)
{
    hdriverframework_driver_base_dummy_check(drv);
    if(drv==NULL)
    {
        return NULL;
    }
    return hdriverframework_driver_base_get_of_compatible(&drv->base);
}

bool hdriverframework_driver_dummy_match_of_compatible(hdriverframework_driver_dummy_t *drv,const char *of_compatible)
{
    hdriverframework_driver_base_dummy_check(drv);
    if(drv==NULL)
    {
        return false;
    }
    return hdriverframework_driver_base_match_of_compatible(&drv->base,of_compatible);
}

size_t hdriverframework_driver_dummy_vm_read(hdriverframework_driver_dummy_t *drv,const hsoftwarevirtualmemory_map_item_t *map_item,uintptr_t address,uint8_t *data,size_t length)
{
    hdriverframework_driver_base_dummy_check(drv);
    if(drv==NULL)
    {
        return 0;
    }
    return hdriverframework_driver_base_vm_read(&drv->base,map_item,address,data,length);
}


size_t hdriverframework_driver_dummy_vm_write(hdriverframework_driver_dummy_t *drv,const hsoftwarevirtualmemory_map_item_t *map_item,uintptr_t address,const uint8_t *data,size_t length)
{
    hdriverframework_driver_base_dummy_check(drv);
    if(drv==NULL)
    {
        return 0;
    }
    return hdriverframework_driver_base_vm_write(&drv->base,map_item,address,data,length);
}

