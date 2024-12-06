/***************************************************************
 * Name:      hgui_gui_common.c
 * Purpose:   实现common接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-11-24
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hgui_gui_common.h"
#include "hdefaults.h"

static hgui_gui_malloc_t malloc_hook=NULL;
void * hgui_gui_malloc(size_t nBytes)
{
    if(malloc_hook!=NULL)
    {
        return malloc_hook(nBytes);
    }
    else
    {
        return hgui_gui_default_malloc(nBytes);
    }
}

void hgui_gui_malloc_set(hgui_gui_malloc_t hook)
{
    malloc_hook=hook;
}

void * hgui_gui_default_malloc(size_t nBytes)
{
    if(nBytes==0)
    {
        return NULL;
    }
    return hdefaults_malloc(nBytes,NULL);
}

hgui_gui_free_t free_hook=NULL;
void hgui_gui_free(void *ptr)
{
    if(free_hook!=NULL)
    {
        free_hook(ptr);
    }
    else
    {
        hgui_gui_default_free(ptr);
    }
}

void hgui_gui_free_set(hgui_gui_free_t hook)
{
    free_hook=hook;
}

void hgui_gui_default_free(void *ptr)
{
    if(ptr==NULL)
    {
        return;
    }
    hdefaults_free(ptr,NULL);
}
