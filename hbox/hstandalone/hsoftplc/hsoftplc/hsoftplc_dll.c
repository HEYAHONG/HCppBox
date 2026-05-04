/***************************************************************
 * Name:      hsoftplc_dll.c
 * Purpose:   实现hsoftplc_dll接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-12-01
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hsoftplc_dll.h"
#include "stdlib.h"
#include "stdint.h"

hsoftplc_dll_import_handle_t hsoftplc_dll_load(const char *dll_path)
{
    hsoftplc_dll_import_handle_t ret = {NULL,NULL};
    if(dll_path==NULL)
    {
        return ret;
    }
    ret.dll_handle=hdlopen(dll_path,HRTLD_NOW);
    if(ret.dll_handle==NULL)
    {
        return ret;
    }
    ret.hsoftplc_interface=(hsoftplc_public_interface_t *)hdlsym(ret.dll_handle,"hsoftplc_interface");
    if(ret.hsoftplc_interface==NULL)
    {
        hdlclose(ret.dll_handle);
        ret.dll_handle=NULL;
        return ret;
    }
    if(ret.hsoftplc_interface->interface_length != sizeof(*ret.hsoftplc_interface))
    {
        hdlclose(ret.dll_handle);
        ret.dll_handle=NULL;
        ret.hsoftplc_interface=NULL;
        return ret;
    }
    return ret;
}


void hsoftplc_dll_unload(hsoftplc_dll_import_handle_t *handle)
{
    if(handle!=NULL && handle->dll_handle!=NULL)
    {
        hdlclose(handle->dll_handle);
        handle->dll_handle=NULL;
        handle->hsoftplc_interface=NULL;
    }
}
