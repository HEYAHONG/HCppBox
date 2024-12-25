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
                    for(size_t j=y; j<y+h; j++)
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

hgui_pixel_mode_t  hgui_driver_pixel_mode(hgui_driver_t *driver,hgui_pixel_mode_t* new_mode)
{
    if(driver==NULL)
    {
        driver=hgui_driver_default_get();
    }

    if(driver->pixel_mode!=NULL)
    {
        return driver->pixel_mode(driver,new_mode);
    }
    else
    {
        return hgui_pixel_global_mode_get();
    }
}

bool hgui_driver_resize(hgui_driver_t* driver,ssize_t *w,ssize_t *h)
{
    if(driver==NULL)
    {
        driver=hgui_driver_default_get();
    }

    if(driver->resize!=NULL)
    {
        return driver->resize(driver,w,h);
    }

    return false;
}


bool  hgui_driver_is_ok(hgui_driver_t *driver)
{
    if(driver==NULL)
    {
        driver=hgui_driver_default_get();
    }

    if(driver->is_ok!=NULL)
    {
        return driver->is_ok(driver);
    }

    return false;
}

bool  hgui_driver_event_input(hgui_driver_t *driver,uint8_t event_type,void *event_param,size_t event_param_length,void *usr)
{
    if(driver==NULL)
    {
        driver=hgui_driver_default_get();
    }

    if(driver->event_cb!=NULL)
    {
        return driver->event_cb(event_type,event_param,event_param_length,usr);
    }

    return false;
}

bool hgui_driver_event_input_helper(uint8_t type,void *eventparam,size_t eventparam_length,void *usr)
{
    return hgui_driver_event_input((hgui_driver_t *)usr,type,eventparam,eventparam_length,NULL);
}

void hgui_driver_event_callback_set(hgui_driver_t *driver,hgui_gui_event_callback_t event_cb)
{
    if(driver==NULL)
    {
        driver=hgui_driver_default_get();
    }
    driver->event_cb=event_cb;
}
