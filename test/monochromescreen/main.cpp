
#include "HCPPBox.h"
#include "hbox.h"
#include "hrc.h"
#include "stdint.h"
#include <thread>
#include <chrono>

static const size_t w=128;//宽
static const size_t h=64;//高
static const size_t w_bytes=w/8+((w%8)?1:0);
static const size_t h_bytes=h;
static uint8_t VRAM[w_bytes][h_bytes]= {0};
static hgui_pixel_t pixel= {0};
static uint32_t backcolor=0xFFFAF208;//背景色
static uint32_t frontcolor=0x0;//前景色
static uint32_t dotsize=5;//点大小

static void monochromescreen_refresh()
{
    hgui_driver_fill_rectangle(NULL,0,0,w*dotsize,h*dotsize,pixel);
}

static void monochromescreen_set_pixel(uint32_t x,uint32_t y,bool dot)
{
    if(x >= w || y >=h)
    {
        return ;
    }
    if(dot)
    {
        VRAM[x/8][y]|=(1ULL<<(x%8));
    }
    else
    {
        VRAM[x/8][y]&=(~(1ULL<<(x%8)));
    }
}

static void monochromescreen_bootlogo()
{
    auto draw_pixel=[](const hgui_gui_dotfont_t * dotfont,size_t x,size_t y,bool point,void *usr)->bool
    {
        (void)dotfont;
        (void)usr;
        if((x<w) && (y<h))
        {
            monochromescreen_set_pixel(x,y,point);
        }
        return true;
    };
    hgui_gui_dotfont_show_ascii_string(&hgui_gui_dotfont_ascii_0806,"Booting",(w-6*7)/2,(h-8)/2,w,draw_pixel,NULL);
}


static void monochromscreen_init()
{
    HCPPGuiInit();
    {
        //初始化屏幕颜色
        for(size_t i=0; i<w; i++)
        {
            for(size_t j=0; j<h; j++)
            {
                monochromescreen_set_pixel(i,j,false);
            }
        }
        //初始化像素回调
        pixel.mode=HGUI_PIXEL_MODE_CALLBACK;
        pixel.pixel=[](ssize_t x,ssize_t y) -> hgui_pixel_t
        {
            hgui_pixel_t ret={0};
            ret.mode=HGUI_PIXEL_MODE_32_BITS;
            x/=dotsize;
            y/=dotsize;
            if(x/8<w_bytes && y <h_bytes)
            {
                if((VRAM[x/8][y]&(1ULL<<(x%8)))!=0)
                {
                    ret.pixel_32_bits=frontcolor;
                }
                else
                {
                    ret.pixel_32_bits=backcolor;
                }
            }
            return ret;
        };

        {
            ssize_t new_w=w*dotsize,new_h=h*dotsize;
            hgui_driver_resize(NULL,&new_w,&new_h);
        }

        monochromescreen_bootlogo();

        monochromescreen_refresh();
    }
}

/*
 * GUI应用
 */
//定义Gui驱动指针
#define HGUI_SCENE1_APP_DRIVER      NULL

//定义屏幕宽度
#define HGUI_SCENE1_APP_WIDTH       w

//定义屏幕高度
#define HGUI_SCENE1_APP_HEIGHT      h

//定义初始化成功时执行的语句
#define HGUI_SCENE1_APP_ON_INIT_SUCCESS     {   monochromscreen_init(); }

//定义初始化失败时执行的语句
#define HGUI_SCENE1_APP_ON_INIT_FAILURE     {}

//定义初始化开始时执行的语句
#define HGUI_SCENE1_APP_ON_UPDATE_BEGIN     {}

//定义初始化结束时执行的语句
#define HGUI_SCENE1_APP_ON_UPDATE_END       {}

//定义初始化更新成功时执行的语句
#define HGUI_SCENE1_APP_ON_UPDATE_SUCCESS   {   monochromescreen_refresh(); }

//定义初始化更新失败时执行的语句
#define HGUI_SCENE1_APP_ON_UPDATE_FAILURE   {}

//实现APP
#include HGUI_SCENE1_APP_IMPLEMENT

int main()
{
    if(hgui_scene1_app_init(&g_hgui_scene1_app,NULL))
    {
        size_t i=0;
        while(hgui_scene1_app_update(&g_hgui_scene1_app,NULL))
        {
            i++;
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }
    return 0;
}