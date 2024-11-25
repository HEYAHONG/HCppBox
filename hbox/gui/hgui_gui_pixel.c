/***************************************************************
 * Name:      hgui_gui_pixel.c
 * Purpose:   实现pixel接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-11-24
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hgui_gui_pixel.h"

static hgui_pixel_mode_t pixel_mode=HGUI_PIXEL_MODE_24_BITS;//默认为24位的像素

hgui_pixel_mode_t hgui_pixel_global_mode_get(void)
{
    return pixel_mode;
}

void hgui_pixel_global_mode_set(hgui_pixel_mode_t mode)
{
    pixel_mode=mode;
}

