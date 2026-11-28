/***************************************************************
 * Name:      android_hgui_driver.cpp
 * Purpose:   Android平台下的hgui驱动（双缓冲）。
 * Author:    HYH (hyhsystem.cn)
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "android_hgui_driver.h"

#include "hbox.h"

#include <mutex>
#include <vector>
#include <string.h>

namespace
{

std::recursive_mutex g_driver_mutex;

/*
 * 绘图缓冲区：hgui的所有绘图操作都写入此缓冲区。
 */
int g_draw_width=320;
int g_draw_height=240;
std::vector<uint32_t> g_draw_buffer(320*240,0xFF000000);

/*
 * 显示缓冲区：只有调用android_hgui_driver_present后才会更新，
 * Java层只读取此缓冲区，从而避免C++绘图过程中出现闪烁。
 */
int g_present_width=320;
int g_present_height=240;
std::vector<uint32_t> g_present_buffer(320*240,0xFF000000);
uint64_t g_present_frame_id=0;

uint32_t hgui_pixel_to_argb(hgui_pixel_t pixel,size_t x,size_t y)
{
    pixel=hgui_pixel_bits_get(pixel,(ssize_t)x,(ssize_t)y);
    switch(pixel.mode)
    {
    case HGUI_PIXEL_MODE_1_BIT:
    {
        return pixel.pixel_1_bit?0xFFFFFFFF:0xFF000000;
    }
    break;
    case HGUI_PIXEL_MODE_2_BIT:
    {
        //CGA模式，此处简单映射为黑白
        return pixel.pixel_2_bits?0xFFFFFFFF:0xFF000000;
    }
    break;
    case HGUI_PIXEL_MODE_4_BIT:
    {
        //EGA模式，此处简单映射为灰度
        uint8_t value=(pixel.pixel_4_bits&0x0F);
        value=(value<<4)|value;
        return 0xFF000000|(((uint32_t)value)<<16)|(((uint32_t)value)<<8)|value;
    }
    break;
    case HGUI_PIXEL_MODE_8_BITS:
    {
        return 0xFF000000|(((uint32_t)pixel.pixel_8_bits)<<16)|(((uint32_t)pixel.pixel_8_bits)<<8)|pixel.pixel_8_bits;
    }
    break;
    case HGUI_PIXEL_MODE_16_BITS:
    {
        return HGUI_RGB_16_to_32(pixel.pixel_16_bits);
    }
    break;
    case HGUI_PIXEL_MODE_24_BITS:
    case HGUI_PIXEL_MODE_32_BITS:
    default:
    {
        return pixel.pixel_32_bits|0xFF000000;
    }
    break;
    }
}

void driver_resize_buffer(int w,int h)
{
    if(w<=0)
    {
        w=1;
    }
    if(h<=0)
    {
        h=1;
    }
    g_draw_width=w;
    g_draw_height=h;
    g_draw_buffer.assign((size_t)w*(size_t)h,0xFF000000);
}

bool driver_fill_rectangle(hgui_driver_t *driver,size_t x,size_t y,size_t w,size_t h,hgui_pixel_t pixel)
{
    (void)driver;
    std::lock_guard<std::recursive_mutex> lock(g_driver_mutex);
    for(size_t j=0; j<h; j++)
    {
        size_t py=y+j;
        if(py>=(size_t)g_draw_height)
        {
            break;
        }
        for(size_t i=0; i<w; i++)
        {
            size_t px=x+i;
            if(px>=(size_t)g_draw_width)
            {
                break;
            }
            g_draw_buffer[py*(size_t)g_draw_width+px]=hgui_pixel_to_argb(pixel,px,py);
        }
    }
    return true;
}

bool driver_draw_pixel(hgui_driver_t *driver,size_t x,size_t y,hgui_pixel_t pixel)
{
    return driver_fill_rectangle(driver,x,y,1,1,pixel);
}

bool driver_reset(hgui_driver_t *driver)
{
    (void)driver;
    std::lock_guard<std::recursive_mutex> lock(g_driver_mutex);
    g_draw_buffer.assign((size_t)g_draw_width*(size_t)g_draw_height,0xFF000000);
    return true;
}

hgui_pixel_mode_t driver_pixel_mode(hgui_driver_t *driver,hgui_pixel_mode_t *new_mode)
{
    (void)driver;
    (void)new_mode;
    return HGUI_PIXEL_MODE_32_BITS;
}

bool driver_resize(hgui_driver_t *driver,ssize_t *w,ssize_t *h)
{
    (void)driver;
    std::lock_guard<std::recursive_mutex> lock(g_driver_mutex);
    if(w==NULL || h==NULL)
    {
        return false;
    }
    int new_w=(*w)>0?((int)(*w)):g_draw_width;
    int new_h=(*h)>0?((int)(*h)):g_draw_height;
    if(new_w>4096)
    {
        new_w=4096;
    }
    if(new_h>4096)
    {
        new_h=4096;
    }
    driver_resize_buffer(new_w,new_h);
    (*w)=new_w;
    (*h)=new_h;
    return true;
}

bool driver_is_ok(hgui_driver_t *driver)
{
    (void)driver;
    return true;
}

} // namespace

extern "C"
bool android_hgui_driver_register(void)
{
    std::lock_guard<std::recursive_mutex> lock(g_driver_mutex);
    hgui_driver_t *driver=hgui_driver_default_get();
    if(driver==NULL)
    {
        return false;
    }
    driver->usr=NULL;
    driver->draw_pixel=driver_draw_pixel;
    driver->fill_rectangle=driver_fill_rectangle;
    driver->reset=driver_reset;
    driver->pixel_mode=driver_pixel_mode;
    driver->resize=driver_resize;
    driver->is_ok=driver_is_ok;
    hgui_pixel_global_mode_set(HGUI_PIXEL_MODE_32_BITS);
    return true;
}

extern "C"
void android_hgui_driver_present(void)
{
    std::lock_guard<std::recursive_mutex> lock(g_driver_mutex);
    int new_width=g_draw_width;
    int new_height=g_draw_height;
    size_t pixel_count=(size_t)new_width*(size_t)new_height;
    if(new_width!=g_present_width || new_height!=g_present_height || g_present_buffer.size()!=pixel_count)
    {
        g_present_width=new_width;
        g_present_height=new_height;
        g_present_buffer=g_draw_buffer;
    }
    else
    {
        if(pixel_count>0)
        {
            memcpy(g_present_buffer.data(),g_draw_buffer.data(),pixel_count*sizeof(uint32_t));
        }
    }
    g_present_frame_id++;
}

extern "C"
int android_hgui_driver_width(void)
{
    std::lock_guard<std::recursive_mutex> lock(g_driver_mutex);
    return g_present_width;
}

extern "C"
int android_hgui_driver_height(void)
{
    std::lock_guard<std::recursive_mutex> lock(g_driver_mutex);
    return g_present_height;
}

extern "C"
uint64_t android_hgui_driver_frame_id(void)
{
    std::lock_guard<std::recursive_mutex> lock(g_driver_mutex);
    return g_present_frame_id;
}

extern "C"
size_t android_hgui_driver_copy_frame(uint32_t *dst,size_t capacity)
{
    if(dst==NULL)
    {
        return 0;
    }
    std::lock_guard<std::recursive_mutex> lock(g_driver_mutex);
    size_t pixel_count=(size_t)g_present_width*(size_t)g_present_height;
    if(pixel_count>capacity)
    {
        pixel_count=capacity;
    }
    if(pixel_count>0)
    {
        memcpy(dst,g_present_buffer.data(),pixel_count*sizeof(uint32_t));
    }
    return pixel_count;
}
