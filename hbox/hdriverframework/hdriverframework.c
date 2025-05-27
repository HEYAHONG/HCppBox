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

