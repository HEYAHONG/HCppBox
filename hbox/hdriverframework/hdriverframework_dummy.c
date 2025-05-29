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

