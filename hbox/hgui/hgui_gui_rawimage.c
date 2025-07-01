/***************************************************************
 * Name:      hgui_gui_rawimage.c
 * Purpose:   实现rawimage接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-07-01
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hgui_gui_rawimage.h"

/*
 * 原始图像主要指直接存储像素值的数据结构,一般情况下3字节时存储RGB图像单字节时存储灰度图像(可用于单色屏)
 */

bool hgui_gui_rawimage_draw_color(const hgui_gui_rawimage_t *rawimage,size_t x,size_t y,hgui_gui_rawimage_draw_color_pixel_t draw_pixel,void *usr)
{
    bool ret=false;
    if(rawimage==NULL || rawimage->data==NULL || draw_pixel==NULL)
    {
        return ret;
    }
    for(size_t i=0;i<rawimage->width;i++)
    {
        for(size_t j=0;j<rawimage->height;j++)
        {
            if(!draw_pixel(rawimage,x+i,y+j,&rawimage->data[(i+j*rawimage->width)*rawimage->cpp],usr))
            {
                return ret;
            }
        }
    }
    ret=true;
    return ret;
}

