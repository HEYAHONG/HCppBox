
#include "HCPPBox.h"
#include "hbox.h"
#include "stdint.h"
#include <thread>
#include <chrono>

static const size_t w=320;
static const size_t h=240;
static uint32_t VRAM[w][h]= {0};
static hgui_pixel_t pixel;

int main()
{
    HCPPGuiInit();

    {
        //初始化屏幕颜色
        for(size_t i=0; i<w; i++)
        {
            for(size_t j=0; j<h; j++)
            {
                VRAM[i][j]=0xFF0000FF;
            }
        }
        //初始化像素回调
        pixel.mode=HGUI_PIXEL_MODE_CALLBACK;
        pixel.pixel=[](ssize_t x,ssize_t y) -> hgui_pixel_t
        {
            hgui_pixel_t ret={0};
            ret.mode=HGUI_PIXEL_MODE_32_BITS;
            if(x<w && y <h)
            {
                ret.pixel_32_bits=VRAM[x][y];
            }
            return ret;
        };
    }

    if(hgui_driver_reset(NULL))
    {
        size_t i=0;
        while(hgui_driver_is_ok(NULL))
        {
            i++;
            std::this_thread::sleep_for(std::chrono::microseconds(1));
            if(i==100)
            {
                ssize_t w=::w,h=::h;
                {
                    hgui_driver_resize(NULL,&w,&h);
                }
                hgui_driver_fill_rectangle(NULL,0,0,w,h,pixel);
            }
        }
    }

    return 0;
}
