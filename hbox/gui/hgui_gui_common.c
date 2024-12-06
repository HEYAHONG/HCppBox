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

static hgui_gui_free_t free_hook=NULL;
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


hgui_gui_common_rectangle_t hgui_gui_common_rectangle_absolute_convert(hgui_gui_common_rectangle_t rect,size_t x,size_t y,size_t w,size_t h)
{
    if(rect.x < 0)
    {
        switch(rect.x)
        {
        case HGUI_GUI_COMMON_RECTANGLE_X_LEFT:
        {
            rect.x=x;
        }
        break;
        case HGUI_GUI_COMMON_RECTANGLE_X_CENTER:
        {
            if(rect.w < w)
            {
                rect.x=x+(w-rect.w)/2;
            }
            else
            {
                rect.x=x;
            }
        }
        break;
        case HGUI_GUI_COMMON_RECTANGLE_X_RIGHT:
        {
            if(rect.w < w)
            {
                rect.x=x+(w-rect.w);
            }
            else
            {
                rect.x=x;
            }
        }
        break;
        default:
        {

        }
        break;
        }
    }
    if(rect.y < 0)
    {
        switch(rect.y)
        {
        case HGUI_GUI_COMMON_RECTANGLE_Y_TOP:
        {
            rect.y=y;
        }
        break;
        case HGUI_GUI_COMMON_RECTANGLE_Y_CENTER:
        {
            if(rect.h<h)
            {
                rect.y=y+(h-rect.h)/2;
            }
            else
            {
                rect.y=y;
            }
        }
        break;
        case HGUI_GUI_COMMON_RECTANGLE_Y_BOTTOM:
        {
            if(rect.h<h)
            {
                rect.y=y+(h-rect.h);
            }
            else
            {
                rect.y=y;
            }
        }
        break;
        default:
        {

        }
        break;
        }
    }
    return rect;
}
