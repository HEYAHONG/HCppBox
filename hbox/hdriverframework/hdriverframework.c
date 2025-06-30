/***************************************************************
 * Name:      hdriverframework.c
 * Purpose:   实现hdriverframework接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-27
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdriverframework.h"

hdriverframework_driver_base_t *hdriverframework_driver_base_set(hdriverframework_driver_base_t *drv,hdriverframework_driver_base_process_t process,void * usr)
{
    if(drv!=NULL)
    {
        drv->process=process;
        drv->usr=usr;
    }
    return drv;
}

const char * hdriverframework_driver_base_getname(hdriverframework_driver_base_t *drv)
{
    const char *ret=NULL;
    if(drv!=NULL && drv->process!=NULL)
    {
        drv->process(drv,HDRIVERFRAMEWORK_OP_GETNAME,&ret);
    }
    return ret;
}

int hdriverframework_driver_base_init(hdriverframework_driver_base_t *drv)
{
    if(drv!=NULL && drv->process!=NULL)
    {
        return drv->process(drv,HDRIVERFRAMEWORK_OP_INIT);
    }
    return HDRIVERFRAMEWORK_EPERM;
}


int hdriverframework_driver_base_deinit(hdriverframework_driver_base_t *drv)
{
    if(drv!=NULL && drv->process!=NULL)
    {
        return drv->process(drv,HDRIVERFRAMEWORK_OP_DEINIT);
    }
    return HDRIVERFRAMEWORK_EPERM;
}

const char ** hdriverframework_driver_base_get_of_compatible(hdriverframework_driver_base_t *drv)
{
    const char **ret=NULL;
    if(drv!=NULL && drv->process!=NULL)
    {
        drv->process(drv,HDRIVERFRAMEWORK_OP_GET_OF_COMPATIBLE,&ret);
    }
    return ret;
}

bool hdriverframework_driver_base_match_of_compatible(hdriverframework_driver_base_t *drv,const char *of_compatible_string)
{
    bool ret=false;
    const char **of_compatible=hdriverframework_driver_base_get_of_compatible(drv);
    if(of_compatible!=NULL && of_compatible_string!=NULL)
    {
        for(size_t i=0;of_compatible[i]!=NULL;i++)
        {
            if(0==strcmp(of_compatible_string,of_compatible[i]))
            {
                ret=true;
                break;
            }
        }
    }
    return ret;
}

size_t hdriverframework_driver_base_vm_read(hdriverframework_driver_base_t *drv,const hsoftwarevirtualmemory_map_item_t *map_item,uintptr_t address,uint8_t *data,size_t length)
{
    size_t ret=0;
    if(drv!=NULL && drv->process!=NULL)
    {
        drv->process(drv,HDRIVERFRAMEWORK_OP_VM_READ,&ret,map_item,address,data,length);
    }
    return ret;
}

size_t hdriverframework_driver_base_vm_write(hdriverframework_driver_base_t *drv,const hsoftwarevirtualmemory_map_item_t *map_item,uintptr_t address,const uint8_t *data,size_t length)
{
     size_t ret=0;
    if(drv!=NULL && drv->process!=NULL)
    {
        drv->process(drv,HDRIVERFRAMEWORK_OP_VM_WRITE,&ret,map_item,address,data,length);
    }
    return ret;
}

