
#include "HCPPBox.h"
#include "hbox.h"
#include "hrc.h"
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
        while(hgui_driver_update(NULL))
        {
            i++;
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            if(i==100)
            {
                ssize_t w=::w,h=::h;
                {
                    hgui_driver_resize(NULL,&w,&h);
                }
                hgui_driver_fill_rectangle(NULL,0,0,w,h,pixel);
            }
            if(i==200)
            {
                auto draw_pixel=[](const hgui_gui_dotfont_t * dotfont,size_t x,size_t y,bool point,void *usr)->bool
                {
                    (void)dotfont;
                    (void)usr;
                    if((x<w) && (y<h)&&point)
                    {
                        VRAM[x][y]=0xFF000000;
                    }
                    return true;
                };
                hgui_gui_dotfont_show_ascii_string(&hgui_gui_dotfont_acs2_0806,"Booting",0,0,w,draw_pixel,NULL);
                hgui_gui_dotfont_show_ascii_string(&hgui_gui_dotfont_acs2_1206,"Booting",0,8,w,draw_pixel,NULL);
                hgui_gui_dotfont_show_ascii_string(&hgui_gui_dotfont_acs2_1608,"Booting",0,8+12,w,draw_pixel,NULL);
                hgui_gui_dotfont_show_ascii_string(&hgui_gui_dotfont_acs2_2416,"Booting",0,8+12+16,w,draw_pixel,NULL);
                hgui_driver_fill_rectangle(NULL,0,0,w,h,pixel);
            }
            if (i == 300)
            {
                auto draw_pixel = [](const hgui_gui_dotfont_t* dotfont, size_t x, size_t y, bool point, void* usr)->bool
                {
                    (void)dotfont;
                    (void)usr;
                    if ((x < w) && (y < h) && point)
                    {
                        VRAM[x][y] = 0xFF000000;
                    }
                    return true;
                };
                const char* banner = (const char *)RCGetHandle("banner");
                if (banner != NULL)
                {
                    for (size_t i = 0; i < w; i++)
                    {
                        for (size_t j = 0; j < h; j++)
                        {
                            VRAM[i][j] = 0xFFFF0000;
                        }
                    }
                    hgui_gui_dotfont_show_ascii_string(&hgui_gui_dotfont_acs2_1206, banner, 0, 0, w, draw_pixel, NULL);
                    hgui_driver_fill_rectangle(NULL, 0, 0, w, h, pixel);
                }
            }
        }
    }

    return 0;
}
