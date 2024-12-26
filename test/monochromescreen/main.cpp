
#include "HCPPBox.h"
#include "hbox.h"
#include "hrc.h"
#include "stdint.h"
#include "time.h"
#include <thread>
#include <chrono>

static const size_t w=128;//宽
static const size_t h=64;//高
static const size_t w_bytes=w/8+((w%8)?1:0);
static const size_t h_bytes=h;
static uint8_t VRAM[w_bytes][h_bytes]= {0};
static hgui_pixel_t pixel= {0};
static uint32_t backcolor=0xFFFAF208;//背景色
static uint32_t frontcolor=0xFF000000;//前景色
static uint32_t dotsize=2;//点大小

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

    {
        //将上1/8与下1/8像素设置为置位
        for(size_t i=0; i<h; i++)
        {
            for(size_t j=0; j<w; j++)
            {
                if(i<h/8 || i>=(h-h/8))
                {
                    monochromescreen_set_pixel(j,i,true);
                }
            }
        }
    }
}


static void  monochromscreen_screen_clear()
{
    for(size_t i=0; i<w; i++)
    {
        for(size_t j=0; j<h; j++)
        {
            monochromescreen_set_pixel(i,j,false);
        }
    }

    hgui_scene1_app_need_refresh(&g_hgui_scene1_app);
}

static void monochromscreen_pixel_init()
{
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
}

static void monochromscreen_screen_size_adjust()
{
    ssize_t new_w=w*dotsize,new_h=h*dotsize;
    hgui_driver_resize(NULL,&new_w,&new_h);
}

static void monochromscreen_init()
{
    HCPPGuiInit();
    {
        //初始化屏幕颜色
        monochromscreen_screen_clear();

        //初始化像素回调
        monochromscreen_pixel_init();

        //调整屏幕大小(用于桌面平台模拟时调整窗口大小)
        monochromscreen_screen_size_adjust();

        //显示启动logo
        monochromescreen_bootlogo();

        hgui_scene1_app_need_refresh(&g_hgui_scene1_app);
    }
}

/*
 * 屏幕
 */

//主屏幕
hdefaults_tick_t main_screen_tick=hdefaults_tick_get();
static  int main_screen_key_count=0;
static  const hgui_scene1_screen_base_t main_screen=
{
    //进入屏幕
    [](hgui_scene1_screen_base_t *screen,const hgui_scene1_app_t *app)
    {
        main_screen_tick=hdefaults_tick_get();
    },
    //离开屏幕
    [](hgui_scene1_screen_base_t *screen,const hgui_scene1_app_t *app)
    {

    },
    //更新屏幕
    [](hgui_scene1_screen_base_t *screen,const hgui_scene1_app_t *app)
    {
        if((hdefaults_tick_get()-main_screen_tick)>500)
        {
            main_screen_tick=hdefaults_tick_get();
            monochromscreen_screen_clear();
            {
                //将上1/8与下1/8像素设置为置位
                for(size_t i=0; i<h; i++)
                {
                    for(size_t j=0; j<w; j++)
                    {
                        if(i<h/8 || i>=(h-h/8))
                        {
                            monochromescreen_set_pixel(j,i,true);
                        }
                    }
                }
            }
            {
                auto draw_pixel=[](const hgui_gui_dotfont_t * dotfont,size_t x,size_t y,bool point,void *usr)->bool
                {
                    (void)dotfont;
                    (void)usr;
                    if((x<w) && (y<h))
                    {
                        monochromescreen_set_pixel(x,y,!point);
                    }
                    return true;
                };
                {
                    //显示标题
                    hgui_gui_dotfont_show_ascii_string(&hgui_gui_dotfont_ascii_0806,"Main",(w-6*4)/2,0,w,draw_pixel,NULL);
                }
                {
                    //显示内容
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
                    char str[256];
                    {
                        sprintf(str,"This is %dx%d screen\nmonochrome screen\r\nkey_count=%d\r\nhit UP(w) or DOWN(s)\r\n",(int)w,(int)h,main_screen_key_count);
                    }
                    hgui_gui_dotfont_show_ascii_string(&hgui_gui_dotfont_ascii_0806,str,0,h/8+4,w,draw_pixel,NULL);
                }
                {
                    //显示时间
                    time_t now=time(NULL);
                    struct tm tm_now=*localtime(&now);
                    std::string  asctime_str{asctime(&tm_now)};
                    asctime_str=asctime_str.substr(0,20);
                    hgui_gui_dotfont_show_ascii_string(&hgui_gui_dotfont_ascii_0806,asctime_str.c_str(),4,(h-h/8),w,draw_pixel,NULL);
                }
            }
            hgui_scene1_app_need_refresh(app);
        }
    },
    //事件处理
    [](hgui_scene1_screen_base_t *screen,const hgui_scene1_app_t *app,uint8_t type,void *eventparam,size_t eventparam_length,void *usr) -> bool
    {
        {
            hgui_gui_event_key_t key;
            if(hgui_gui_event_key_get(&key,type,eventparam,eventparam_length,usr)!=NULL)
            {
                //按键事件
                if(key.key_press_or_release==1)
                {
                    //按键按下
                    switch(key.key_value)
                    {
                    case HGUI_GUI_EVENT_KEY_VALUE_w:
                    case HGUI_GUI_EVENT_KEY_VALUE_UP:
                    {
                        //上键
                        main_screen_key_count++;
                    }
                    break;
                    case HGUI_GUI_EVENT_KEY_VALUE_s:
                    case HGUI_GUI_EVENT_KEY_VALUE_DOWN:
                    {
                        //下键
                        main_screen_key_count--;
                    }
                    break;
                    default:
                    {

                    }
                    break;
                    }
                }

                hgui_scene1_app_need_refresh(&g_hgui_scene1_app);
            }
        }
        return true;
    },
    NULL
};

/*
 * GUI应用
 */
//定义Gui驱动指针
#define HGUI_SCENE1_APP_DRIVER      NULL

//定义屏幕宽度
#define HGUI_SCENE1_APP_WIDTH       w

//定义屏幕高度
#define HGUI_SCENE1_APP_HEIGHT      h

//定义屏幕栈深度
#define HGUI_SCENE1_APP_SCREEN_STACK_DEPTH      8

//定义初始化成功时执行的语句
#define HGUI_SCENE1_APP_ON_INIT_SUCCESS     {   monochromscreen_init(); hgui_scene1_app_screen_stack_push(&g_hgui_scene1_app,(hgui_scene1_screen_base_t *)&main_screen); }

//定义初始化失败时执行的语句
#define HGUI_SCENE1_APP_ON_INIT_FAILURE     {}

//定义更新开始时执行的语句
#define HGUI_SCENE1_APP_ON_UPDATE_BEGIN     {}

//定义更新结束时执行的语句
#define HGUI_SCENE1_APP_ON_UPDATE_END       {}

//定义更新成功时执行的语句
#define HGUI_SCENE1_APP_ON_UPDATE_SUCCESS   {}

//定义更新成功且需要刷新时执行的语句
#define HGUI_SCENE1_APP_ON_UPDATE_SUCCESS_NEED_REFRESH { monochromescreen_refresh(); }

//定义更新失败时执行的语句
#define HGUI_SCENE1_APP_ON_UPDATE_FAILURE   {}

//实现APP
#include HGUI_SCENE1_APP_IMPLEMENT

int main()
{

    size_t i=0;
    while(hgui_scene1_app_update(&g_hgui_scene1_app,NULL))
    {
        i++;
#ifdef WIN32
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
#else
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        //非Windows可实时刷新
        hgui_scene1_app_need_refresh(&g_hgui_scene1_app);
#endif // WIN32
    }
    return 0;
}
