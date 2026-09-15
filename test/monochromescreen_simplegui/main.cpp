
#include "HCPPBox.h"
#include "hbox.h"
#include "hrc.h"
#include "stdint.h"
#include "time.h"
#include <thread>
#include <chrono>
#include "hsimplegui.h"

static hsimplegui_t gui;
static ssize_t       gui_w=128;
static ssize_t       gui_h=64;

static void hgui_init()
{
    HCPPGuiInit();
    hgui_driver_reset(NULL);
    hgui_driver_resize(NULL,&gui_w,&gui_h);
}

enum
{
    GUI_SCREEN_ID_MIAN=1000,
};

extern HMI_SCREEN_OBJECT screen_main;
static HMI_SCREEN_OBJECT *screen_ptr_list[]
{
    &screen_main,
};

static void init(void)
{
    hgui_init();
    hsimplegui_init(&gui,gui_w,gui_h,screen_ptr_list,sizeof(screen_ptr_list)/sizeof(screen_ptr_list[0]),GUI_SCREEN_ID_MIAN,NULL);
}

static void loop(void)
{
     hsimplegui_update(&gui);
}

int main()
{
    init();
    while(hgui_driver_update(NULL))
    {
        loop();
#ifdef WIN32
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
#else
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
#endif // WIN32
    }
    return 0;
}

/*
 * 主屏幕
 */
static HMI_ENGINE_RESULT screen_main_action_initialize(SGUI_SCR_DEV* Interface)
{
    return HMI_RET_NORMAL;
}
static HMI_ENGINE_RESULT screen_main_action_prepare(SGUI_SCR_DEV* Interface, const void* pstParameters)
{
    /* Clean screen display. */
    SGUI_Basic_ClearScreen(Interface);
    /* Refresh display. */
    SGUI_Basic_ResetMask(Interface);

    return HMI_RET_NORMAL;
}
static HMI_ENGINE_RESULT screen_main_action_repaint(SGUI_SCR_DEV* Interface, const void* pstParameters)
{
    size_t offset_y=0;
    size_t max_y=Interface->stSize.iHeight;
    size_t max_x=Interface->stSize.iWidth;
    /* Refresh display. */
    SGUI_Basic_ResetMask(Interface);

    {
        /*
         * 显示标题
         */
        char buffer[64]= {0};
        hsprintf(buffer," 主屏幕 ");
        SGUI_AREA_SIZE title_area= {0};
        SGUI_Text_GetTextExtent(buffer,&hsimplegui_font_chinese_12,&title_area);
        SGUI_Text_DrawMultipleLinesText(Interface,buffer,&hsimplegui_font_chinese_12,(Interface->stSize.iWidth-title_area.iWidth)/2,offset_y,Interface->stSize.iWidth,SGUI_DRAW_REVERSE);
        offset_y+=title_area.iHeight;
    }

    {
        /*
         * 显示时间
         */
        time_t current_time=htime(NULL);
        htm_t current_tm= {0};
        hlibc_localtime_r(&current_time,&current_tm);
        char buffer[64]= {0};
        hsprintf(buffer," %04d年%02d月%02d日 ",current_tm.tm_year+1900,current_tm.tm_mon+1,current_tm.tm_mday);
        SGUI_AREA_SIZE date_area= {0};
        SGUI_Text_GetTextExtent(buffer,&hsimplegui_font_chinese_12,&date_area);
        SGUI_Text_DrawMultipleLinesText(Interface,buffer,&hsimplegui_font_chinese_12,(Interface->stSize.iWidth-date_area.iWidth)/2,offset_y,Interface->stSize.iWidth,SGUI_DRAW_NORMAL);
        offset_y+=date_area.iHeight;
        hsprintf(buffer," %02d时%02d分%02d秒 ",current_tm.tm_hour,current_tm.tm_min,current_tm.tm_sec);
        SGUI_AREA_SIZE time_area= {0};
        SGUI_Text_GetTextExtent(buffer,&hsimplegui_font_chinese_12,&time_area);
        SGUI_Text_DrawMultipleLinesText(Interface,buffer,&hsimplegui_font_chinese_12,(Interface->stSize.iWidth-time_area.iWidth)/2,offset_y,Interface->stSize.iWidth,SGUI_DRAW_NORMAL);
        offset_y+=time_area.iHeight;
    }


    return HMI_RET_NORMAL;
}
static HMI_ENGINE_RESULT  screen_main_action_processevent(SGUI_SCR_DEV* Interface, const HMI_EVENT_BASE* pstEvent, SGUI_INT* piActionID)
{
    if(Interface==NULL || pstEvent==NULL)
    {
        return HMI_RET_INVALID_DATA;
    }

    if(pstEvent->iID == HHMI_EVENT_HGUI && SGUI_TRUE == HMI_EVENT_SIZE_CHK(*(hhmi_event_hgui_t*)pstEvent, hhmi_event_hgui_t))
    {
        /*
         * HGUI事件
         */
        hhmi_event_hgui_t* evt=(hhmi_event_hgui_t*)pstEvent;
        hgui_gui_event_key_t key;
        if(hgui_gui_event_key_get(&key,evt->Data.type,evt->Data.eventparam,evt->Data.eventparam_length,NULL)!=NULL)
        {
            /*
             * 按键事件
             */
            if(key.key_press_or_release==1)
            {
                switch(key.key_value)
                {
                default:
                {

                }
                break;
                }
            }
        }

    }

    if(pstEvent->iID == HHMI_EVENT_UPDATE && SGUI_TRUE == HMI_EVENT_SIZE_CHK(*(hhmi_event_update_t*)pstEvent, hhmi_event_update_t))
    {
        /*
         * Update事件
         */
         screen_main_action_repaint(Interface,NULL);
    }

    return HMI_RET_NORMAL;
}
static HMI_ENGINE_RESULT screen_main_action_postprocess(SGUI_SCR_DEV* Interface, HMI_ENGINE_RESULT eProcResult, SGUI_INT iActionID)
{
    if(HMI_PROCESS_SUCCESSFUL(eProcResult))
    {

    }

    return HMI_RET_NORMAL;
}
static HMI_SCREEN_ACTION screen_main_actions=
{
    screen_main_action_initialize,
    screen_main_action_prepare,
    screen_main_action_repaint,
    screen_main_action_processevent,
    screen_main_action_postprocess,
};
HMI_SCREEN_OBJECT screen_main=
{
    GUI_SCREEN_ID_MIAN,
    &screen_main_actions
};

