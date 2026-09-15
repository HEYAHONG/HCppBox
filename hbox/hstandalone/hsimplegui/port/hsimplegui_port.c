/***************************************************************
 * Name:      hsimplegui_port.c
 * Purpose:   声明hsimplegui_port接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-09-14
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#define HSIMPLEGUI_PORT_IMPLEMENTATION 1

#include "hsimplegui_port.h"

#ifndef HSIMPLEGUI_BKGCLR
#define HSIMPLEGUI_BKGCLR (0xFFFAF208)
#endif // HSIMPLEGUI_BKGCLR
#ifndef HSIMPLEGUI_FRGCLR
#define HSIMPLEGUI_FRGCLR (0xFF000000)
#endif // HSIMPLEGUI_FRGCLR

static SGUI_INT sgui_scr_dev_lcd_init(void)
{
    return 0;
}

static void sgui_scr_dev_lcd_set_pixel(SGUI_INT iX, SGUI_INT iY, SGUI_UINT iColor)
{
    hgui_pixel_t pixel;
    pixel.mode=HGUI_PIXEL_MODE_32_BITS;
    pixel.pixel_32_bits=(iColor==SGUI_COLOR_BKGCLR?(HSIMPLEGUI_BKGCLR):(HSIMPLEGUI_FRGCLR));
    hgui_driver_draw_pixel(NULL,iX,iY,pixel);
}

static void sgui_scr_dev_lcd_clear(void)
{
    ssize_t w=-1;
    ssize_t h=-1;
    hgui_driver_resize(NULL,&w,&h);
    if(w > 0 &&h > 0)
    {
        hgui_pixel_t pixel;
        pixel.mode=HGUI_PIXEL_MODE_32_BITS;
        pixel.pixel_32_bits=(HSIMPLEGUI_BKGCLR);
        hgui_driver_fill_rectangle(NULL,0,0,w,h,pixel);
    }
}


static void sgui_scr_dev_lcd_fillrect(SGUI_INT iX, SGUI_INT iY, SGUI_INT iWidth, SGUI_INT iHeight, SGUI_UINT iColor)
{
    hgui_pixel_t pixel;
    pixel.mode=HGUI_PIXEL_MODE_32_BITS;
    pixel.pixel_32_bits=(iColor==SGUI_COLOR_BKGCLR?(HSIMPLEGUI_BKGCLR):(HSIMPLEGUI_FRGCLR));
    hgui_driver_fill_rectangle(NULL,iX,iY,iWidth,iHeight,pixel);
}

static void sgui_scr_dev_lcd_sync(void)
{

}


bool hgui_gui_event_callback(uint8_t type,void *eventparam,size_t eventparam_length,void *usr)
{
    /*
     * 发送hgui事件，通常是按键事件
     */
    hhmi_event_hgui_t event;
    memset(&event,0,sizeof(event));
    HMI_EVENT_INIT(event);
    event.Head.iID=HHMI_EVENT_HGUI;
    event.Data.type=type;
    event.Data.eventparam=eventparam;
    event.Data.eventparam_length=eventparam_length;
    HMI_ProcessEvent((HMI_EVENT_BASE *)&event);
    return true;
}

void hsimplegui_init(hsimplegui_t *gui,size_t w,size_t h,HMI_SCREEN_OBJECT** screen,size_t screen_count,int main_screen_id,void *usr)
{
    if(gui == NULL || w == 0 || h == 0 ||screen == NULL || screen_count == 0 )
    {
        return;
    }

    memset(gui,0,sizeof(*gui));
    gui->usr=(uintptr_t)usr;
    /*
     * 设置默认驱动回调
     */
    hgui_driver_event_callback_set(NULL,hgui_gui_event_callback);

    gui->DeviceInterface.stSize.iWidth=w;
    gui->DeviceInterface.stSize.iHeight=h;
    gui->DeviceInterface.stBuffer.pBuffer=gui->BmpDataBuffer;
    gui->DeviceInterface.stBuffer.sSize=sizeof(gui->BmpDataBuffer);
    gui->DeviceInterface.fnInitialize=sgui_scr_dev_lcd_init;
    gui->DeviceInterface.fnSetPixel=sgui_scr_dev_lcd_set_pixel;
    gui->DeviceInterface.fnFillRect=sgui_scr_dev_lcd_fillrect;
    gui->DeviceInterface.fnClear=sgui_scr_dev_lcd_clear;
    gui->DeviceInterface.fnSyncBuffer=sgui_scr_dev_lcd_sync;

    SGUI_Basic_ResetMask(&gui->DeviceInterface);
    gui->Engine.ScreenCount = screen_count;
    gui->Engine.ScreenObjPtr = screen;
    gui->Engine.Interface = &gui->DeviceInterface;

    /* Initialize all screen object. */
    if(NULL != gui->Engine.ScreenObjPtr)
    {
        for(size_t iIndex=0; iIndex<gui->Engine.ScreenCount; iIndex++)
        {
            if( (NULL != gui->Engine.ScreenObjPtr[iIndex]) && (NULL != gui->Engine.ScreenObjPtr[iIndex]->pstActions)  && (NULL != gui->Engine.ScreenObjPtr[iIndex]->pstActions->Initialize))
            {
                gui->Engine.ScreenObjPtr[iIndex]->pstActions->Initialize(&gui->DeviceInterface);
                gui->Engine.ScreenObjPtr[iIndex]->pstPrevious = NULL;
            }
        }
    }

    /*
     * 设置主屏幕
     */
    HMI_ActiveEngine(&gui->Engine, main_screen_id);

    /*
     * 启动
     */
    HMI_StartEngine(NULL);

}

void hsimplegui_update(hsimplegui_t *gui)
{
    hgui_driver_update(NULL);
    {
        /*
         * 发送更新事件
         */
        hhmi_event_update_t event;
        memset(&event,0,sizeof(event));
        HMI_EVENT_INIT(event);
        event.Head.iID=HHMI_EVENT_UPDATE;
        event.Data.gui=gui;
        HMI_ProcessEvent((HMI_EVENT_BASE *)&event);
    }
}
