/***************************************************************
 * Name:      hgui_gui_driver.c
 * Purpose:   实现driver接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-11-24
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hgui_gui_driver.h"
static hgui_driver_t driver=HGUI_DRIVER_INITIALIZER;

hgui_driver_t *hgui_driver_default_get(void)
{
    return &driver;
}

bool hgui_driver_draw_pixel(hgui_driver_t *driver,size_t x,size_t y,hgui_pixel_t pixel)
{
    if(driver==NULL)
    {
        driver=hgui_driver_default_get();
    }

    {
        if(driver->draw_pixel!=NULL)
        {
            return driver->draw_pixel(driver,x,y,pixel);
        }
        else
        {
            if(driver->fill_rectangle!=NULL)
            {
                return driver->fill_rectangle(driver,x,y,1,1,pixel);
            }
        }
    }
    return false;
}

bool hgui_driver_fill_rectangle(hgui_driver_t *driver,size_t x,size_t y,size_t w,size_t h,hgui_pixel_t pixel)
{
    if(driver==NULL)
    {
        driver=hgui_driver_default_get();
    }

    {
        if(driver->fill_rectangle!=NULL)
        {
            return driver->fill_rectangle(driver,x,y,w,h,pixel);
        }
        else
        {
            if(driver->draw_pixel!=NULL)
            {
                bool ret=true;
                for(size_t i=x; i<x+w; i++)
                {
                    for(size_t j=y; y<y+h; j++)
                    {
                        ret=driver->draw_pixel(driver,i,j,pixel);
                        if(!ret)
                        {
                            return ret;
                        }
                    }
                }
            }
        }
    }
    return false;
}


bool hgui_driver_reset(hgui_driver_t *driver)
{
    if(driver==NULL)
    {
        driver=hgui_driver_default_get();
    }

    {
        if(driver->reset!=NULL)
        {
            return driver->reset(driver);
        }
    }

    return false;
}

