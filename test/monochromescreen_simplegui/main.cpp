
#include "HCPPBox.h"
#include "hbox.h"
#include "hrc.h"
#include "stdint.h"
#include "time.h"
#include <thread>
#include <chrono>
#include "hsimplegui.h"
#include "SGUI_List.h"
#include "SGUI_Curve.h"
#include "SGUI_RealtimeGraph.h"
#include "SGUI_PolarCoordinates.h"
#include "SGUI_IconResource.h"

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
    GUI_SCREEN_ID_TEXT,
    GUI_SCREEN_ID_LIST,
    GUI_SCREEN_ID_MENU,
    GUI_SCREEN_ID_NOTICE,
    GUI_SCREEN_ID_BASIC,
    GUI_SCREEN_ID_VARIABLE_BOX,
    GUI_SCREEN_ID_CURVE,
    GUI_SCREEN_ID_REALTIME_GRAPH,
    GUI_SCREEN_ID_CLOCK,
};

enum
{
    GUI_ACTION_NONE=0,
    GUI_ACTION_BACK,
};

extern HMI_SCREEN_OBJECT screen_main;
extern HMI_SCREEN_OBJECT screen_text;
extern HMI_SCREEN_OBJECT screen_list;
extern HMI_SCREEN_OBJECT screen_menu;
extern HMI_SCREEN_OBJECT screen_notice;
extern HMI_SCREEN_OBJECT screen_basic;
extern HMI_SCREEN_OBJECT screen_variable_box;
extern HMI_SCREEN_OBJECT screen_curve;
extern HMI_SCREEN_OBJECT screen_realtime_graph;
extern HMI_SCREEN_OBJECT screen_clock;

static HMI_SCREEN_OBJECT *screen_ptr_list[]
{
    &screen_main,
    &screen_text,
    &screen_list,
    &screen_menu,
    &screen_notice,
    &screen_basic,
    &screen_variable_box,
    &screen_curve,
    &screen_realtime_graph,
    &screen_clock,
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
 * 通用操作
 */
static bool screen_key_is_pressed(const HMI_EVENT_BASE* pstEvent,hgui_gui_event_key_t* key)
{
    if(pstEvent==NULL || key==NULL)
    {
        return false;
    }
    if(pstEvent->iID != HHMI_EVENT_HGUI || SGUI_TRUE != HMI_EVENT_SIZE_CHK(*(hhmi_event_hgui_t*)pstEvent, hhmi_event_hgui_t))
    {
        return false;
    }
    {
        hhmi_event_hgui_t* evt=(hhmi_event_hgui_t*)pstEvent;
        if(hgui_gui_event_key_get(key,evt->Data.type,evt->Data.eventparam,evt->Data.eventparam_length,NULL)==NULL)
        {
            return false;
        }
    }
    return key->key_press_or_release==1;
}

static bool screen_is_update_event(const HMI_EVENT_BASE* pstEvent)
{
    return (pstEvent!=NULL && pstEvent->iID == HHMI_EVENT_UPDATE && SGUI_TRUE == HMI_EVENT_SIZE_CHK(*(hhmi_event_update_t*)pstEvent, hhmi_event_update_t));
}

static void screen_draw_header(SGUI_SCR_DEV* Interface,const char* title)
{
    if(Interface==NULL || title==NULL)
    {
        return;
    }
    SGUI_Basic_ResetMask(Interface);
    {
        char buffer[64]= {0};
        hsprintf(buffer," %s ",title);
        SGUI_AREA_SIZE title_area= {0};
        SGUI_Text_GetTextExtent(buffer,&hsimplegui_font_chinese_12,&title_area);
        SGUI_Text_DrawMultipleLinesText(Interface,buffer,&hsimplegui_font_chinese_12,(Interface->stSize.iWidth-title_area.iWidth)/2,0,Interface->stSize.iWidth,SGUI_DRAW_REVERSE);
    }
}

static void screen_draw_footer(SGUI_SCR_DEV* Interface,const char* text,bool align_right)
{
    if(Interface==NULL || text==NULL)
    {
        return;
    }
    SGUI_Basic_ResetMask(Interface);
    {
        char buffer[64]= {0};
        hsprintf(buffer," %s ",text);
        SGUI_AREA_SIZE area= {0};
        SGUI_Text_GetTextExtent(buffer,&hsimplegui_font_chinese_12,&area);
        SGUI_Text_DrawMultipleLinesText(Interface,buffer,&hsimplegui_font_chinese_12,align_right?(Interface->stSize.iWidth-area.iWidth):0,Interface->stSize.iHeight-area.iHeight,Interface->stSize.iWidth,SGUI_DRAW_REVERSE);
    }
}

static void screen_draw_footer_pair(SGUI_SCR_DEV* Interface,const char* left,const char* right)
{
    screen_draw_footer(Interface,left,false);
    screen_draw_footer(Interface,right,true);
}

/*
 * 主屏幕
 */
static bool screen_main_menu_is_show=false;
static SGUI_MENU screen_main_menu= {};
static SGUI_ITEMS_ITEM screen_main_menu_item_list[]=
{
    {
        "文本显示",
        NULL
    },
    {
        "列表控件",
        NULL
    },
    {
        "菜单控件",
        NULL
    },
    {
        "提示框",
        NULL
    },
    {
        "基础绘图",
        NULL
    },
    {
        "变量框",
        NULL
    },
    {
        "曲线控件",
        NULL
    },
    {
        "实时曲线",
        NULL
    },
    {
        "时钟表盘",
        NULL
    }
};

static int screen_main_menu_screen_id(size_t index)
{
    switch(index)
    {
    case 0:
    {
        return GUI_SCREEN_ID_TEXT;
    }
    case 1:
    {
        return GUI_SCREEN_ID_LIST;
    }
    case 2:
    {
        return GUI_SCREEN_ID_MENU;
    }
    case 3:
    {
        return GUI_SCREEN_ID_NOTICE;
    }
    case 4:
    {
        return GUI_SCREEN_ID_BASIC;
    }
    case 5:
    {
        return GUI_SCREEN_ID_VARIABLE_BOX;
    }
    case 6:
    {
        return GUI_SCREEN_ID_CURVE;
    }
    case 7:
    {
        return GUI_SCREEN_ID_REALTIME_GRAPH;
    }
    case 8:
    {
        return GUI_SCREEN_ID_CLOCK;
    }
    default:
    {
        return GUI_SCREEN_ID_MIAN;
    }
    }
}

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

    screen_main_menu_is_show=false;

    {
        SGUI_RECT menu_layout= {};
        menu_layout.iX=0;
        menu_layout.iY=12;
        menu_layout.iWidth=64;
        menu_layout.iHeight=48;
        SGUI_Menu_Initialize(&screen_main_menu,&menu_layout,&hsimplegui_font_chinese_12,screen_main_menu_item_list,sizeof(screen_main_menu_item_list)/sizeof(screen_main_menu_item_list[0]));
    }

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

    {
        /*
         * 显示操作栏
         */
        if(!screen_main_menu_is_show)
        {
            /*
             * 未显示菜单
             */
            char buffer[64]= {0};
            hsprintf(buffer,"菜单(F1)");
            SGUI_AREA_SIZE area= {0};
            SGUI_Text_GetTextExtent(buffer,&hsimplegui_font_chinese_12,&area);
            SGUI_Text_DrawMultipleLinesText(Interface,buffer,&hsimplegui_font_chinese_12,0,max_y-area.iHeight,Interface->stSize.iWidth,SGUI_DRAW_REVERSE);
        }
        else
        {
            /*
             * 已显示菜单
             */
            char buffer[64]= {0};
            hsprintf(buffer,"返回(F3)");
            SGUI_AREA_SIZE area= {0};
            SGUI_Text_GetTextExtent(buffer,&hsimplegui_font_chinese_12,&area);
            SGUI_Text_DrawMultipleLinesText(Interface,buffer,&hsimplegui_font_chinese_12,max_x-area.iWidth,max_y-area.iHeight,Interface->stSize.iWidth,SGUI_DRAW_REVERSE);
        }
    }

    if(screen_main_menu_is_show)
    {
        /*
         * 画菜单
         */
        SGUI_Menu_Repaint(Interface,&screen_main_menu);
    }


    return HMI_RET_NORMAL;
}
static HMI_ENGINE_RESULT  screen_main_action_processevent(SGUI_SCR_DEV* Interface, const HMI_EVENT_BASE* pstEvent, SGUI_INT* piActionID)
{
    if(piActionID!=NULL)
    {
        *piActionID=GUI_ACTION_NONE;
    }
    if(Interface==NULL || pstEvent==NULL)
    {
        return HMI_RET_INVALID_DATA;
    }

    {
        hgui_gui_event_key_t key;
        if(screen_key_is_pressed(pstEvent,&key))
        {
            /*
             * 按键事件
             */
            switch(key.key_value)
            {
            case  HGUI_GUI_EVENT_KEY_VALUE_F1:
            {
                /* Clean screen display. */
                SGUI_Basic_ClearScreen(Interface);
                /* Refresh display. */
                SGUI_Basic_ResetMask(Interface);
                if(!screen_main_menu_is_show)
                {
                    screen_main_menu_is_show=true;
                }
            }
            break;
            case  HGUI_GUI_EVENT_KEY_VALUE_RETURN:
            {
                /* Clean screen display. */
                SGUI_Basic_ClearScreen(Interface);
                /* Refresh display. */
                SGUI_Basic_ResetMask(Interface);
                if(!screen_main_menu_is_show)
                {
                    screen_main_menu_is_show=true;
                }
                else
                {
                    size_t index=(size_t)SGUI_Menu_GetSelection((&screen_main_menu))->iIndex;
                    screen_main_menu_is_show=false;
                    HMI_GoToScreen(screen_main_menu_screen_id(index),NULL);
                }
            }
            break;
            case  HGUI_GUI_EVENT_KEY_VALUE_ESCAPE:
            case  HGUI_GUI_EVENT_KEY_VALUE_F3:
            {
                /* Clean screen display. */
                SGUI_Basic_ClearScreen(Interface);
                /* Refresh display. */
                SGUI_Basic_ResetMask(Interface);
                if(screen_main_menu_is_show)
                {
                    screen_main_menu_is_show=false;
                }
            }
            break;
            case  HGUI_GUI_EVENT_KEY_VALUE_UP:
            case  HGUI_GUI_EVENT_KEY_VALUE_w:
            {
                /* Clean screen display. */
                SGUI_Basic_ClearScreen(Interface);
                /* Refresh display. */
                SGUI_Basic_ResetMask(Interface);
                if(screen_main_menu_is_show)
                {
                    if(SGUI_Menu_GetSelection((&screen_main_menu))->iIndex > 0)
                    {
                        SGUI_Menu_Selecte((&screen_main_menu), SGUI_Menu_GetSelection((&screen_main_menu))->iIndex-1);
                        SGUI_Menu_Repaint(Interface, (&screen_main_menu));
                    }
                }
            }
            break;
            case  HGUI_GUI_EVENT_KEY_VALUE_DOWN:
            case  HGUI_GUI_EVENT_KEY_VALUE_s:
            {
                /* Clean screen display. */
                SGUI_Basic_ClearScreen(Interface);
                /* Refresh display. */
                SGUI_Basic_ResetMask(Interface);
                if(screen_main_menu_is_show)
                {
                    if(SGUI_Menu_GetSelection((&screen_main_menu))->iIndex < (&screen_main_menu)->stItems.iCount-1)
                    {
                        SGUI_Menu_Selecte((&screen_main_menu), SGUI_Menu_GetSelection((&screen_main_menu))->iIndex+1);
                        SGUI_Menu_Repaint(Interface, (&screen_main_menu));
                    }
                }
            }
            break;
            default:
            {

            }
            break;
            }
        }
    }

    if(screen_is_update_event(pstEvent))
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

/*
 * 文本显示屏幕
 */
static HMI_ENGINE_RESULT screen_text_action_prepare(SGUI_SCR_DEV* Interface, const void* pstParameters)
{
    /* Clean screen display. */
    SGUI_Basic_ClearScreen(Interface);
    /* Refresh display. */
    SGUI_Basic_ResetMask(Interface);
    return HMI_RET_NORMAL;
}
static HMI_ENGINE_RESULT screen_text_action_repaint(SGUI_SCR_DEV* Interface, const void* pstParameters)
{
    if(Interface==NULL)
    {
        return HMI_RET_INVALID_DATA;
    }

    /* Refresh display. */
    SGUI_Basic_ResetMask(Interface);

    screen_draw_header(Interface,"文本显示");

    SGUI_Text_DrawText(Interface,"8P: ABCabc123",&hsimplegui_font_ascii_8,0,13,SGUI_DRAW_NORMAL);
    SGUI_Text_DrawText(Interface,"12P: ABCabc",&hsimplegui_font_ascii_12,0,22,SGUI_DRAW_NORMAL);
    SGUI_Text_DrawText(Interface,"中文十二",&hsimplegui_font_chinese_12,0,38,SGUI_DRAW_NORMAL);

    SGUI_Text_DrawText(Interface,"中文",&hsimplegui_font_chinese_16,64,12,SGUI_DRAW_NORMAL);
    SGUI_Text_DrawText(Interface,"十六",&hsimplegui_font_chinese_16,64,30,SGUI_DRAW_NORMAL);

    screen_draw_footer(Interface,"返回(F3)",true);

    return HMI_RET_NORMAL;
}
static HMI_ENGINE_RESULT screen_text_action_processevent(SGUI_SCR_DEV* Interface, const HMI_EVENT_BASE* pstEvent, SGUI_INT* piActionID)
{
    if(piActionID!=NULL)
    {
        *piActionID=GUI_ACTION_NONE;
    }
    hgui_gui_event_key_t key;
    if(screen_key_is_pressed(pstEvent,&key))
    {
        switch(key.key_value)
        {
        case HGUI_GUI_EVENT_KEY_VALUE_ESCAPE:
        case HGUI_GUI_EVENT_KEY_VALUE_F3:
        {
            if(piActionID!=NULL)
            {
                *piActionID=GUI_ACTION_BACK;
            }
        }
        break;
        default:
        {
        }
        break;
        }
    }

    if(screen_is_update_event(pstEvent))
    {
        screen_text_action_repaint(Interface,NULL);
    }
    return HMI_RET_NORMAL;
}
static HMI_ENGINE_RESULT screen_text_action_postprocess(SGUI_SCR_DEV* Interface, HMI_ENGINE_RESULT eProcResult, SGUI_INT iActionID)
{
    if(HMI_PROCESS_SUCCESSFUL(eProcResult) && iActionID==GUI_ACTION_BACK)
    {
        HMI_GoBack(NULL);
    }
    return HMI_RET_NORMAL;
}
static HMI_SCREEN_ACTION screen_text_actions=
{
    NULL,
    screen_text_action_prepare,
    screen_text_action_repaint,
    screen_text_action_processevent,
    screen_text_action_postprocess,
};
HMI_SCREEN_OBJECT screen_text=
{
    GUI_SCREEN_ID_TEXT,
    &screen_text_actions
};

/*
 * 列表控件屏幕
 */
static SGUI_LIST screen_list_object= {};
static SGUI_ITEMS_ITEM screen_list_item_list[]=
{
    {
        "文本显示",
        NULL
    },
    {
        "列表控件",
        NULL
    },
    {
        "菜单控件",
        NULL
    },
    {
        "提示信息",
        NULL
    },
    {
        "基础绘图",
        NULL
    },
    {
        "变量框",
        NULL
    },
    {
        "曲线控件",
        NULL
    },
    {
        "实时曲线",
        NULL
    },
    {
        "时钟表盘",
        NULL
    },
    {
        "单色屏幕",
        NULL
    }
};

static void screen_list_initialize(SGUI_SCR_DEV* Interface)
{
    SGUI_RECT list_layout= {};
    list_layout.iX=0;
    list_layout.iY=12;
    list_layout.iWidth=Interface->stSize.iWidth;
    list_layout.iHeight=40;
    SGUI_List_Initialize(&screen_list_object,&list_layout,&hsimplegui_font_chinese_12,"列表演示",screen_list_item_list,sizeof(screen_list_item_list)/sizeof(screen_list_item_list[0]));
}

static HMI_ENGINE_RESULT screen_list_action_initialize(SGUI_SCR_DEV* Interface)
{
    screen_list_initialize(Interface);
    return HMI_RET_NORMAL;
}
static HMI_ENGINE_RESULT screen_list_action_prepare(SGUI_SCR_DEV* Interface, const void* pstParameters)
{
    /* Clean screen display. */
    SGUI_Basic_ClearScreen(Interface);
    /* Refresh display. */
    SGUI_Basic_ResetMask(Interface);

    screen_list_initialize(Interface);

    return HMI_RET_NORMAL;
}
static HMI_ENGINE_RESULT screen_list_action_repaint(SGUI_SCR_DEV* Interface, const void* pstParameters)
{
    if(Interface==NULL)
    {
        return HMI_RET_INVALID_DATA;
    }

    /* Refresh display. */
    SGUI_Basic_ResetMask(Interface);
    screen_draw_header(Interface,"列表控件");
    SGUI_List_Repaint(Interface,&screen_list_object);
    screen_draw_footer(Interface,"上下键选择 F3返回",true);

    return HMI_RET_NORMAL;
}
static HMI_ENGINE_RESULT screen_list_action_processevent(SGUI_SCR_DEV* Interface, const HMI_EVENT_BASE* pstEvent, SGUI_INT* piActionID)
{
    if(piActionID!=NULL)
    {
        *piActionID=GUI_ACTION_NONE;
    }
    hgui_gui_event_key_t key;
    if(screen_key_is_pressed(pstEvent,&key))
    {
        switch(key.key_value)
        {
        case HGUI_GUI_EVENT_KEY_VALUE_ESCAPE:
        case HGUI_GUI_EVENT_KEY_VALUE_F3:
        {
            if(piActionID!=NULL)
            {
                *piActionID=GUI_ACTION_BACK;
            }
        }
        break;
        case HGUI_GUI_EVENT_KEY_VALUE_UP:
        case HGUI_GUI_EVENT_KEY_VALUE_w:
        {
            if(SGUI_List_GetSelection(&screen_list_object)->iIndex > 0)
            {
                SGUI_List_Selecte(&screen_list_object,SGUI_List_GetSelection(&screen_list_object)->iIndex-1);
            }
            screen_list_action_repaint(Interface,NULL);
        }
        break;
        case HGUI_GUI_EVENT_KEY_VALUE_DOWN:
        case HGUI_GUI_EVENT_KEY_VALUE_s:
        {
            if(SGUI_List_GetSelection(&screen_list_object)->iIndex < screen_list_object.stItems.iCount-1)
            {
                SGUI_List_Selecte(&screen_list_object,SGUI_List_GetSelection(&screen_list_object)->iIndex+1);
            }
            screen_list_action_repaint(Interface,NULL);
        }
        break;
        case HGUI_GUI_EVENT_KEY_VALUE_HOME:
        {
            SGUI_List_Selecte(&screen_list_object,0);
            screen_list_action_repaint(Interface,NULL);
        }
        break;
        case HGUI_GUI_EVENT_KEY_VALUE_END:
        {
            SGUI_List_Selecte(&screen_list_object,screen_list_object.stItems.iCount-1);
            screen_list_action_repaint(Interface,NULL);
        }
        break;
        default:
        {
        }
        break;
        }
    }

    if(screen_is_update_event(pstEvent))
    {
        screen_list_action_repaint(Interface,NULL);
    }
    return HMI_RET_NORMAL;
}
static HMI_ENGINE_RESULT screen_list_action_postprocess(SGUI_SCR_DEV* Interface, HMI_ENGINE_RESULT eProcResult, SGUI_INT iActionID)
{
    if(HMI_PROCESS_SUCCESSFUL(eProcResult) && iActionID==GUI_ACTION_BACK)
    {
        HMI_GoBack(NULL);
    }
    return HMI_RET_NORMAL;
}
static HMI_SCREEN_ACTION screen_list_actions=
{
    screen_list_action_initialize,
    screen_list_action_prepare,
    screen_list_action_repaint,
    screen_list_action_processevent,
    screen_list_action_postprocess,
};
HMI_SCREEN_OBJECT screen_list=
{
    GUI_SCREEN_ID_LIST,
    &screen_list_actions
};

/*
 * 菜单控件屏幕
 */
static SGUI_MENU screen_menu_main= {};
static SGUI_MENU screen_menu_sub= {};
static SGUI_MENU* screen_menu_actived=&screen_menu_main;
static SGUI_ITEMS_ITEM screen_menu_item_list[]=
{
    {
        "第一项",
        NULL
    },
    {
        "第二项",
        NULL
    },
    {
        "第三项",
        NULL
    },
    {
        "第四项",
        NULL
    },
    {
        "第五项",
        NULL
    },
    {
        "第六项",
        NULL
    }
};
static SGUI_ITEMS_ITEM screen_menu_sub_item_list[]=
{
    {
        "子菜单一",
        NULL
    },
    {
        "子菜单二",
        NULL
    },
    {
        "子菜单三",
        NULL
    },
    {
        "子菜单四",
        NULL
    }
};

static void screen_menu_initialize(SGUI_SCR_DEV* Interface)
{
    SGUI_RECT menu_layout= {};
    menu_layout.iX=2;
    menu_layout.iY=12;
    menu_layout.iWidth=64;
    menu_layout.iHeight=40;
    SGUI_Menu_Initialize(&screen_menu_main,&menu_layout,&hsimplegui_font_chinese_12,screen_menu_item_list,sizeof(screen_menu_item_list)/sizeof(screen_menu_item_list[0]));
    SGUI_SystemIF_MemorySet(&screen_menu_sub,0x00,sizeof(screen_menu_sub));
    screen_menu_actived=&screen_menu_main;
}

static HMI_ENGINE_RESULT screen_menu_action_initialize(SGUI_SCR_DEV* Interface)
{
    screen_menu_initialize(Interface);
    return HMI_RET_NORMAL;
}
static HMI_ENGINE_RESULT screen_menu_action_prepare(SGUI_SCR_DEV* Interface, const void* pstParameters)
{
    /* Clean screen display. */
    SGUI_Basic_ClearScreen(Interface);
    /* Refresh display. */
    SGUI_Basic_ResetMask(Interface);

    screen_menu_initialize(Interface);

    return HMI_RET_NORMAL;
}
static HMI_ENGINE_RESULT screen_menu_action_repaint(SGUI_SCR_DEV* Interface, const void* pstParameters)
{
    if(Interface==NULL)
    {
        return HMI_RET_INVALID_DATA;
    }

    /* Refresh display. */
    SGUI_Basic_ResetMask(Interface);
    screen_draw_header(Interface,"菜单控件");
    screen_draw_footer_pair(Interface,"回车:子菜单","F3返回");
    SGUI_Menu_Repaint(Interface,screen_menu_actived);

    return HMI_RET_NORMAL;
}
static HMI_ENGINE_RESULT screen_menu_action_processevent(SGUI_SCR_DEV* Interface, const HMI_EVENT_BASE* pstEvent, SGUI_INT* piActionID)
{
    if(piActionID!=NULL)
    {
        *piActionID=GUI_ACTION_NONE;
    }
    hgui_gui_event_key_t key;
    if(screen_key_is_pressed(pstEvent,&key))
    {
        switch(key.key_value)
        {
        case HGUI_GUI_EVENT_KEY_VALUE_RETURN:
        {
            if(screen_menu_actived==&screen_menu_main)
            {
                SGUI_RECT item_area= {};
                SGUI_ItemsBase_GetItemExtent(&(screen_menu_main.stItems),SGUI_Menu_GetSelection((&screen_menu_main))->iIndex,&item_area);
                SGUI_Menu_Initialize(&screen_menu_sub,&item_area,&hsimplegui_font_chinese_12,screen_menu_sub_item_list,sizeof(screen_menu_sub_item_list)/sizeof(screen_menu_sub_item_list[0]));
                screen_menu_actived=&screen_menu_sub;
                SGUI_Menu_PopupSubMenu(Interface,&screen_menu_sub,&item_area);
                SGUI_Menu_Repaint(Interface,screen_menu_actived);
                SGUI_Basic_ResetMask(Interface);
            }
        }
        break;
        case HGUI_GUI_EVENT_KEY_VALUE_ESCAPE:
        {
            if(screen_menu_actived==&screen_menu_main)
            {
                if(piActionID!=NULL)
                {
                    *piActionID=GUI_ACTION_BACK;
                }
            }
            else
            {
                if(screen_menu_actived->stLayout.iWidth > 0)
                {
                    /* Cleanup sub-menu display. */
                    SGUI_Basic_SetMask4(Interface,&(screen_menu_actived->stLayout));
                    SGUI_Basic_DrawRectangle3(Interface,&(screen_menu_actived->stLayout),SGUI_COLOR_BKGCLR,SGUI_COLOR_BKGCLR);
                    screen_menu_actived->stLayout.iWidth=0;
                    screen_menu_actived=&screen_menu_main;
                }
                SGUI_Menu_Repaint(Interface,screen_menu_actived);
            }
        }
        break;
        case HGUI_GUI_EVENT_KEY_VALUE_F3:
        {
            if(piActionID!=NULL)
            {
                *piActionID=GUI_ACTION_BACK;
            }
        }
        break;
        case HGUI_GUI_EVENT_KEY_VALUE_UP:
        case HGUI_GUI_EVENT_KEY_VALUE_w:
        {
            if(SGUI_Menu_GetSelection(screen_menu_actived)->iIndex > 0)
            {
                SGUI_Menu_Selecte(screen_menu_actived,SGUI_Menu_GetSelection(screen_menu_actived)->iIndex-1);
                SGUI_Menu_Repaint(Interface,screen_menu_actived);
            }
        }
        break;
        case HGUI_GUI_EVENT_KEY_VALUE_DOWN:
        case HGUI_GUI_EVENT_KEY_VALUE_s:
        {
            if(SGUI_Menu_GetSelection(screen_menu_actived)->iIndex < screen_menu_actived->stItems.iCount-1)
            {
                SGUI_Menu_Selecte(screen_menu_actived,SGUI_Menu_GetSelection(screen_menu_actived)->iIndex+1);
                SGUI_Menu_Repaint(Interface,screen_menu_actived);
            }
        }
        break;
        default:
        {
        }
        break;
        }
    }

    if(screen_is_update_event(pstEvent))
    {
        screen_menu_action_repaint(Interface,NULL);
    }
    return HMI_RET_NORMAL;
}
static HMI_ENGINE_RESULT screen_menu_action_postprocess(SGUI_SCR_DEV* Interface, HMI_ENGINE_RESULT eProcResult, SGUI_INT iActionID)
{
    if(HMI_PROCESS_SUCCESSFUL(eProcResult) && iActionID==GUI_ACTION_BACK)
    {
        HMI_GoBack(NULL);
    }
    return HMI_RET_NORMAL;
}
static HMI_SCREEN_ACTION screen_menu_actions=
{
    screen_menu_action_initialize,
    screen_menu_action_prepare,
    screen_menu_action_repaint,
    screen_menu_action_processevent,
    screen_menu_action_postprocess,
};
HMI_SCREEN_OBJECT screen_menu=
{
    GUI_SCREEN_ID_MENU,
    &screen_menu_actions
};

/*
 * 提示框屏幕
 */
static SGUI_NOTICT_BOX screen_notice_box= {};
static char screen_notice_text[]="提示：这是一个通知框。按回车键关闭。";

static HMI_ENGINE_RESULT screen_notice_action_initialize(SGUI_SCR_DEV* Interface)
{
    screen_notice_box.pstIcon=&SGUI_RES_ICON_INFORMATION_16;
    screen_notice_box.cszNoticeText=screen_notice_text;
    screen_notice_box.stLayout.iX=2;
    screen_notice_box.stLayout.iY=12;
    screen_notice_box.stLayout.iWidth=Interface->stSize.iWidth-4;
    screen_notice_box.stLayout.iHeight=40;
    return HMI_RET_NORMAL;
}
static HMI_ENGINE_RESULT screen_notice_action_prepare(SGUI_SCR_DEV* Interface, const void* pstParameters)
{
    /* Clean screen display. */
    SGUI_Basic_ClearScreen(Interface);
    /* Refresh display. */
    SGUI_Basic_ResetMask(Interface);
    return HMI_RET_NORMAL;
}
static HMI_ENGINE_RESULT screen_notice_action_repaint(SGUI_SCR_DEV* Interface, const void* pstParameters)
{
    if(Interface==NULL)
    {
        return HMI_RET_INVALID_DATA;
    }

    /* Refresh display. */
    SGUI_Basic_ResetMask(Interface);
    screen_draw_header(Interface,"提示框");
    SGUI_Notice_Repaint(Interface,&screen_notice_box,&hsimplegui_font_chinese_12,0);
    screen_draw_footer(Interface,"回车或F3返回",true);

    return HMI_RET_NORMAL;
}
static HMI_ENGINE_RESULT screen_notice_action_processevent(SGUI_SCR_DEV* Interface, const HMI_EVENT_BASE* pstEvent, SGUI_INT* piActionID)
{
    if(piActionID!=NULL)
    {
        *piActionID=GUI_ACTION_NONE;
    }
    hgui_gui_event_key_t key;
    if(screen_key_is_pressed(pstEvent,&key))
    {
        switch(key.key_value)
        {
        case HGUI_GUI_EVENT_KEY_VALUE_RETURN:
        case HGUI_GUI_EVENT_KEY_VALUE_ESCAPE:
        case HGUI_GUI_EVENT_KEY_VALUE_F3:
        {
            if(piActionID!=NULL)
            {
                *piActionID=GUI_ACTION_BACK;
            }
        }
        break;
        default:
        {
        }
        break;
        }
    }

    if(screen_is_update_event(pstEvent))
    {
        screen_notice_action_repaint(Interface,NULL);
    }
    return HMI_RET_NORMAL;
}
static HMI_ENGINE_RESULT screen_notice_action_postprocess(SGUI_SCR_DEV* Interface, HMI_ENGINE_RESULT eProcResult, SGUI_INT iActionID)
{
    if(HMI_PROCESS_SUCCESSFUL(eProcResult) && iActionID==GUI_ACTION_BACK)
    {
        HMI_GoBack(NULL);
    }
    return HMI_RET_NORMAL;
}
static HMI_SCREEN_ACTION screen_notice_actions=
{
    screen_notice_action_initialize,
    screen_notice_action_prepare,
    screen_notice_action_repaint,
    screen_notice_action_processevent,
    screen_notice_action_postprocess,
};
HMI_SCREEN_OBJECT screen_notice=
{
    GUI_SCREEN_ID_NOTICE,
    &screen_notice_actions
};

/*
 * 基础绘图屏幕
 */
static int screen_basic_mode=0;

static void screen_basic_draw_base_row(SGUI_SCR_DEV* Interface,int y,SGUI_COLOR edge_color,SGUI_COLOR fill_color)
{
    /* 矩形 */
    SGUI_Basic_DrawRectangle1(Interface,2,y,26,14,edge_color,fill_color);
    /* 圆形 */
    SGUI_Basic_DrawCircle(Interface,42,y+7,7,edge_color,fill_color);
    /* 圆角矩形 */
    SGUI_Basic_DrawRoundedRectangle(Interface,54,y,26,14,4,edge_color,fill_color);
    /* 直线 */
    SGUI_Basic_DrawLine(Interface,84,y,124,y+13,SGUI_COLOR_FRGCLR);
    SGUI_Basic_DrawLine(Interface,84,y+13,124,y,SGUI_COLOR_FRGCLR);
}

static void screen_basic_draw_row(SGUI_SCR_DEV* Interface,int y,int style)
{
    switch(style)
    {
    case 0:
    {
        /* 绘图0 */
        screen_basic_draw_base_row(Interface,y,SGUI_COLOR_FRGCLR,SGUI_COLOR_TRANS);
    }
    break;
    case 1:
    {
        /* 绘图1 */
        screen_basic_draw_base_row(Interface,y,SGUI_COLOR_FRGCLR,SGUI_COLOR_BKGCLR);
    }
    break;
    case 2:
    {
        /* 绘图2 */
        screen_basic_draw_base_row(Interface,y,SGUI_COLOR_FRGCLR,SGUI_COLOR_FRGCLR);
    }
    break;
    default:
    {
        /*
         * 绘图3
         */
        screen_basic_draw_base_row(Interface,y,SGUI_COLOR_FRGCLR,SGUI_COLOR_FRGCLR);
        SGUI_Basic_DrawRectangle1(Interface,6,y+4,18,6,SGUI_COLOR_FRGCLR,SGUI_COLOR_BKGCLR);
        SGUI_Basic_DrawCircle(Interface,42,y+7,3,SGUI_COLOR_FRGCLR,SGUI_COLOR_BKGCLR);
        SGUI_Basic_DrawRoundedRectangle(Interface,58,y+4,18,6,2,SGUI_COLOR_FRGCLR,SGUI_COLOR_BKGCLR);
        SGUI_Basic_DrawRectangle1(Interface,84,y,40,14,SGUI_COLOR_FRGCLR,SGUI_COLOR_FRGCLR);
        SGUI_Basic_DrawLine(Interface,84,y,124,y+13,SGUI_COLOR_BKGCLR);
        SGUI_Basic_DrawLine(Interface,84,y+13,124,y,SGUI_COLOR_BKGCLR);
    }
    break;
    }
}

static HMI_ENGINE_RESULT screen_basic_action_prepare(SGUI_SCR_DEV* Interface, const void* pstParameters)
{
    /* Clean screen display. */
    SGUI_Basic_ClearScreen(Interface);
    /* Refresh display. */
    SGUI_Basic_ResetMask(Interface);

    screen_basic_mode=0;

    return HMI_RET_NORMAL;
}
static HMI_ENGINE_RESULT screen_basic_action_repaint(SGUI_SCR_DEV* Interface, const void* pstParameters)
{
    if(Interface==NULL)
    {
        return HMI_RET_INVALID_DATA;
    }

    /* Refresh display. */
    SGUI_Basic_ResetMask(Interface);
    screen_draw_header(Interface,"基础绘图");

    /*
     * 绘图
     */
    for(int y=12; y<52; y+=5)
    {
        SGUI_Basic_DrawHorizontalLine(Interface,0,Interface->stSize.iWidth-1,y,SGUI_COLOR_FRGCLR);
    }

    if(screen_basic_mode==0)
    {
        /*
         * 绘图1
         */
        screen_basic_draw_row(Interface,14,0);
        screen_basic_draw_row(Interface,32,1);
    }
    else
    {
        /*
         * 绘图2
         */
        screen_basic_draw_row(Interface,14,2);
        screen_basic_draw_row(Interface,32,3);
    }

    {
        char buffer[32]= {0};
        hsprintf(buffer,"切换%d(回车)",screen_basic_mode+1);
        screen_draw_footer_pair(Interface,buffer,"F3返回");
    }

    return HMI_RET_NORMAL;
}
static HMI_ENGINE_RESULT screen_basic_action_processevent(SGUI_SCR_DEV* Interface, const HMI_EVENT_BASE* pstEvent, SGUI_INT* piActionID)
{
    if(piActionID!=NULL)
    {
        *piActionID=GUI_ACTION_NONE;
    }
    hgui_gui_event_key_t key;
    if(screen_key_is_pressed(pstEvent,&key))
    {
        switch(key.key_value)
        {
        case HGUI_GUI_EVENT_KEY_VALUE_ESCAPE:
        case HGUI_GUI_EVENT_KEY_VALUE_F3:
        {
            if(piActionID!=NULL)
            {
                *piActionID=GUI_ACTION_BACK;
            }
        }
        break;
        case HGUI_GUI_EVENT_KEY_VALUE_RETURN:
        case HGUI_GUI_EVENT_KEY_VALUE_SPACE:
        case HGUI_GUI_EVENT_KEY_VALUE_F2:
        {
            screen_basic_mode=(screen_basic_mode+1)%2;
            /* Clean screen display. */
            SGUI_Basic_ClearScreen(Interface);
            /* Refresh display. */
            SGUI_Basic_ResetMask(Interface);
            screen_basic_action_repaint(Interface,NULL);
        }
        break;
        default:
        {
        }
        break;
        }
    }

    if(screen_is_update_event(pstEvent))
    {
        screen_basic_action_repaint(Interface,NULL);
    }
    return HMI_RET_NORMAL;
}
static HMI_ENGINE_RESULT screen_basic_action_postprocess(SGUI_SCR_DEV* Interface, HMI_ENGINE_RESULT eProcResult, SGUI_INT iActionID)
{
    if(HMI_PROCESS_SUCCESSFUL(eProcResult) && iActionID==GUI_ACTION_BACK)
    {
        HMI_GoBack(NULL);
    }
    return HMI_RET_NORMAL;
}
static HMI_SCREEN_ACTION screen_basic_actions=
{
    NULL,
    screen_basic_action_prepare,
    screen_basic_action_repaint,
    screen_basic_action_processevent,
    screen_basic_action_postprocess,
};
HMI_SCREEN_OBJECT screen_basic=
{
    GUI_SCREEN_ID_BASIC,
    &screen_basic_actions
};

/*
 * 变量框屏幕
 */
static SGUI_NUM_VARBOX_STRUCT screen_variable_number= {};
static SGUI_TEXT_VARBOX_STRUCT screen_variable_text= {};
static const char screen_variable_charset[]=" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
static char screen_variable_text_buffer[]="ABC123";
static int screen_variable_focus=0;

static HMI_ENGINE_RESULT screen_variable_box_action_initialize(SGUI_SCR_DEV* Interface)
{
    {
        SGUI_NUM_VARBOX_PARAM init_param= {};
        init_param.eAlignment=SGUI_CENTER;
        init_param.iMin=-50;
        init_param.iMax=100;
        init_param.pstFontRes=&hsimplegui_font_ascii_8;
        init_param.stLayout.iX=42;
        init_param.stLayout.iY=14;
        init_param.stLayout.iWidth=82;
        init_param.stLayout.iHeight=hsimplegui_font_ascii_8.iHeight;
        SGUI_NumberVariableBox_Initialize(&screen_variable_number,&init_param);
    }
    {
        SGUI_TEXT_VARBOX_PARAM init_param= {};
        init_param.pstFontRes=&hsimplegui_font_ascii_12;
        init_param.stLayout.iX=42;
        init_param.stLayout.iY=30;
        init_param.stLayout.iWidth=82;
        init_param.stLayout.iHeight=hsimplegui_font_ascii_12.iHeight;
        init_param.iTextLength=SGUI_SystemIF_StringLength(screen_variable_text_buffer);
        init_param.stCharSet.cszCharSet=screen_variable_charset;
        init_param.stCharSet.iSize=sizeof(screen_variable_charset)-1;
        SGUI_TextVariableBox_Initialize(&screen_variable_text,&init_param,screen_variable_text_buffer);
    }
    return HMI_RET_NORMAL;
}
static HMI_ENGINE_RESULT screen_variable_box_action_prepare(SGUI_SCR_DEV* Interface, const void* pstParameters)
{
    /* Clean screen display. */
    SGUI_Basic_ClearScreen(Interface);
    /* Refresh display. */
    SGUI_Basic_ResetMask(Interface);
    screen_variable_focus=0;
    return HMI_RET_NORMAL;
}
static HMI_ENGINE_RESULT screen_variable_box_action_repaint(SGUI_SCR_DEV* Interface, const void* pstParameters)
{
    if(Interface==NULL)
    {
        return HMI_RET_INVALID_DATA;
    }

    /* Refresh display. */
    SGUI_Basic_ResetMask(Interface);
    screen_draw_header(Interface,"变量框");

    SGUI_Text_DrawText(Interface,"数值",&hsimplegui_font_chinese_12,2,14,SGUI_DRAW_NORMAL);
    SGUI_Basic_DrawRectangle1(Interface,38,12,88,12,SGUI_COLOR_FRGCLR,SGUI_COLOR_BKGCLR);
    SGUI_NumberVariableBox_Repaint(Interface,&screen_variable_number,(screen_variable_focus==0)?SGUI_DRAW_REVERSE:SGUI_DRAW_NORMAL);

    SGUI_Basic_ResetMask(Interface);
    SGUI_Text_DrawText(Interface,"文本",&hsimplegui_font_chinese_12,2,30,SGUI_DRAW_NORMAL);
    SGUI_Basic_DrawRectangle1(Interface,38,28,88,16,SGUI_COLOR_FRGCLR,SGUI_COLOR_BKGCLR);
    SGUI_TextVariableBox_Repaint(Interface,&screen_variable_text,'\0',(screen_variable_focus==1)?SGUI_DRAW_REVERSE:SGUI_DRAW_NORMAL);

    screen_draw_footer_pair(Interface,"TAB切换焦点","F3返回");

    return HMI_RET_NORMAL;
}
static HMI_ENGINE_RESULT screen_variable_box_action_processevent(SGUI_SCR_DEV* Interface, const HMI_EVENT_BASE* pstEvent, SGUI_INT* piActionID)
{
    if(piActionID!=NULL)
    {
        *piActionID=GUI_ACTION_NONE;
    }
    hgui_gui_event_key_t key;
    if(screen_key_is_pressed(pstEvent,&key))
    {
        switch(key.key_value)
        {
        case HGUI_GUI_EVENT_KEY_VALUE_ESCAPE:
        case HGUI_GUI_EVENT_KEY_VALUE_F3:
        {
            if(piActionID!=NULL)
            {
                *piActionID=GUI_ACTION_BACK;
            }
        }
        break;
        case HGUI_GUI_EVENT_KEY_VALUE_TAB:
        {
            screen_variable_focus=(screen_variable_focus+1)%2;
            screen_variable_box_action_repaint(Interface,NULL);
        }
        break;
        case HGUI_GUI_EVENT_KEY_VALUE_UP:
        case HGUI_GUI_EVENT_KEY_VALUE_w:
        {
            if(screen_variable_focus==0)
            {
                int value=SGUI_NumberVariableBox_GetValue(&screen_variable_number)+1;
                SGUI_NumberVariableBox_SetValue(&screen_variable_number,value);
            }
            else
            {
                SGUI_TextVariableBox_IncreaseChar(&screen_variable_text);
            }
            screen_variable_box_action_repaint(Interface,NULL);
        }
        break;
        case HGUI_GUI_EVENT_KEY_VALUE_DOWN:
        case HGUI_GUI_EVENT_KEY_VALUE_s:
        {
            if(screen_variable_focus==0)
            {
                int value=SGUI_NumberVariableBox_GetValue(&screen_variable_number)-1;
                SGUI_NumberVariableBox_SetValue(&screen_variable_number,value);
            }
            else
            {
                SGUI_TextVariableBox_DecreaseChar(&screen_variable_text);
            }
            screen_variable_box_action_repaint(Interface,NULL);
        }
        break;
        case HGUI_GUI_EVENT_KEY_VALUE_LEFT:
        case HGUI_GUI_EVENT_KEY_VALUE_a:
        {
            if(screen_variable_focus==1)
            {
                SGUI_TextVariableBox_DecreaseIndex(&screen_variable_text);
                screen_variable_box_action_repaint(Interface,NULL);
            }
        }
        break;
        case HGUI_GUI_EVENT_KEY_VALUE_RIGHT:
        case HGUI_GUI_EVENT_KEY_VALUE_d:
        {
            if(screen_variable_focus==1)
            {
                SGUI_TextVariableBox_IncreaseIndex(&screen_variable_text);
                screen_variable_box_action_repaint(Interface,NULL);
            }
        }
        break;
        default:
        {
        }
        break;
        }
    }

    if(screen_is_update_event(pstEvent))
    {
        screen_variable_box_action_repaint(Interface,NULL);
    }
    return HMI_RET_NORMAL;
}
static HMI_ENGINE_RESULT screen_variable_box_action_postprocess(SGUI_SCR_DEV* Interface, HMI_ENGINE_RESULT eProcResult, SGUI_INT iActionID)
{
    if(HMI_PROCESS_SUCCESSFUL(eProcResult) && iActionID==GUI_ACTION_BACK)
    {
        HMI_GoBack(NULL);
    }
    return HMI_RET_NORMAL;
}
static HMI_SCREEN_ACTION screen_variable_box_actions=
{
    screen_variable_box_action_initialize,
    screen_variable_box_action_prepare,
    screen_variable_box_action_repaint,
    screen_variable_box_action_processevent,
    screen_variable_box_action_postprocess,
};
HMI_SCREEN_OBJECT screen_variable_box=
{
    GUI_SCREEN_ID_VARIABLE_BOX,
    &screen_variable_box_actions
};

/*
 * 曲线控件屏幕
 */
#define     CURVE_POINT_MUN_MAX                 (20)

static SGUI_CURVE_STRUCT screen_curve_object= {};
static SGUI_BOOL screen_curve_point_used[CURVE_POINT_MUN_MAX]= {0};
static SGUI_CURVE_POINT screen_curve_point_array[CURVE_POINT_MUN_MAX]= {0};
static SGUI_INT screen_curve_argument=0;

static SGUI_CURVE_POINT* screen_curve_new_point(void)
{
    for(size_t i=0; i<CURVE_POINT_MUN_MAX; i++)
    {
        if(screen_curve_point_used[i]==SGUI_FALSE)
        {
            screen_curve_point_used[i]=SGUI_TRUE;
            return &screen_curve_point_array[i];
        }
    }
    return NULL;
}
static void screen_curve_release_point(SGUI_CURVE_POINT* point)
{
    for(size_t i=0; i<CURVE_POINT_MUN_MAX; i++)
    {
        if(&screen_curve_point_array[i]==point)
        {
            SGUI_SystemIF_MemorySet(point,0x00,sizeof(SGUI_CURVE_POINT));
            screen_curve_point_used[i]=SGUI_FALSE;
            break;
        }
    }
}
static HMI_ENGINE_RESULT screen_curve_action_initialize(SGUI_SCR_DEV* Interface)
{
    SGUI_CURVE_PARAM init_param= {};
    SGUI_CURVE_POINT* point=NULL;

    init_param.stLayout.iX=1;
    init_param.stLayout.iY=12;
    init_param.stLayout.iWidth=Interface->stSize.iWidth-2;
    init_param.stLayout.iHeight=40;
    init_param.stXRange.iMin=-100;
    init_param.stXRange.iMax=100;
    init_param.stYRange.iMin=-100;
    init_param.stYRange.iMax=100;

    SGUI_Curve_Initialize(&screen_curve_object,&init_param);

    point=screen_curve_new_point();
    SGUI_Curve_InitializePoint(point,-100,-100);
    SGUI_Curve_AddPoint(&screen_curve_object,point);

    point=screen_curve_new_point();
    SGUI_Curve_InitializePoint(point,0,0);
    SGUI_Curve_AddPoint(&screen_curve_object,point);

    point=screen_curve_new_point();
    SGUI_Curve_InitializePoint(point,100,75);
    SGUI_Curve_AddPoint(&screen_curve_object,point);

    SGUI_Curve_FocusPoint(&screen_curve_object,1);

    point=screen_curve_new_point();
    SGUI_Curve_InitializePoint(point,-75,-100);
    SGUI_Curve_InsertBeforePoint(&screen_curve_object,point);

    point=screen_curve_new_point();
    SGUI_Curve_InitializePoint(point,50,25);
    SGUI_Curve_InsertAfterPoint(&screen_curve_object,point);

    screen_curve_argument=screen_curve_object.stData.stPoints.pstHead->stPoint.iX;

    return HMI_RET_NORMAL;
}
static HMI_ENGINE_RESULT screen_curve_action_prepare(SGUI_SCR_DEV* Interface, const void* pstParameters)
{
    /* Clean screen display. */
    SGUI_Basic_ClearScreen(Interface);
    /* Refresh display. */
    SGUI_Basic_ResetMask(Interface);

    SGUI_Curve_FocusPoint(&screen_curve_object,0);

    return HMI_RET_NORMAL;
}
static HMI_ENGINE_RESULT screen_curve_action_repaint(SGUI_SCR_DEV* Interface, const void* pstParameters)
{
    char buffer[32]= {0};

    if(Interface==NULL)
    {
        return HMI_RET_INVALID_DATA;
    }

    /* Refresh display. */
    SGUI_Basic_ResetMask(Interface);
    screen_draw_header(Interface,"曲线控件");

    SGUI_Curve_Repaint(Interface,&screen_curve_object);
    if(NULL != screen_curve_object.stData.pstFocused)
    {
        SGUI_Curve_HighlightFocus(Interface,&screen_curve_object);
        hsprintf(buffer,"%d,%d",screen_curve_object.stData.pstFocused->stPoint.iX,screen_curve_object.stData.pstFocused->stPoint.iY);
    }
    else
    {
        SGUI_INT line_x=(screen_curve_argument-screen_curve_object.stParam.stXRange.iMin) * SGUI_RECT_WIDTH(screen_curve_object.stParam.stLayout) / SGUI_RANGE_SIZE(screen_curve_object.stParam.stXRange) + screen_curve_object.stParam.stLayout.iX;
        SGUI_INT value=SGUI_Curve_GetDependentValue(&screen_curve_object,screen_curve_argument);
        SGUI_Basic_DrawVerticalLine(Interface,line_x,screen_curve_object.stParam.stLayout.iY,screen_curve_object.stParam.stLayout.iY+screen_curve_object.stParam.stLayout.iHeight-1,SGUI_COLOR_FRGCLR);
        hsprintf(buffer,"%d,%d",screen_curve_argument,value);
    }

    screen_draw_footer_pair(Interface,buffer,"F3返回");

    return HMI_RET_NORMAL;
}
static HMI_ENGINE_RESULT screen_curve_action_processevent(SGUI_SCR_DEV* Interface, const HMI_EVENT_BASE* pstEvent, SGUI_INT* piActionID)
{
    if(piActionID!=NULL)
    {
        *piActionID=GUI_ACTION_NONE;
    }
    hgui_gui_event_key_t key;
    if(screen_key_is_pressed(pstEvent,&key))
    {
        switch(key.key_value)
        {
        case HGUI_GUI_EVENT_KEY_VALUE_ESCAPE:
        case HGUI_GUI_EVENT_KEY_VALUE_F3:
        {
            if(piActionID!=NULL)
            {
                *piActionID=GUI_ACTION_BACK;
            }
        }
        break;
        case HGUI_GUI_EVENT_KEY_VALUE_TAB:
        {
            if(NULL == screen_curve_object.stData.pstFocused)
            {
                screen_curve_object.stData.pstFocused=screen_curve_object.stData.stPoints.pstHead;
            }
            else
            {
                screen_curve_object.stData.pstFocused=screen_curve_object.stData.pstFocused->pstNext;
            }
            screen_curve_action_repaint(Interface,NULL);
        }
        break;
        case HGUI_GUI_EVENT_KEY_VALUE_UP:
        case HGUI_GUI_EVENT_KEY_VALUE_w:
        {
            if(NULL != screen_curve_object.stData.pstFocused)
            {
                SGUI_Curve_UpdateFocusPoint(&screen_curve_object,screen_curve_object.stData.pstFocused->stPoint.iX,screen_curve_object.stData.pstFocused->stPoint.iY+1);
                screen_curve_action_repaint(Interface,NULL);
            }
        }
        break;
        case HGUI_GUI_EVENT_KEY_VALUE_DOWN:
        case HGUI_GUI_EVENT_KEY_VALUE_s:
        {
            if(NULL != screen_curve_object.stData.pstFocused)
            {
                SGUI_Curve_UpdateFocusPoint(&screen_curve_object,screen_curve_object.stData.pstFocused->stPoint.iX,screen_curve_object.stData.pstFocused->stPoint.iY-1);
                screen_curve_action_repaint(Interface,NULL);
            }
        }
        break;
        case HGUI_GUI_EVENT_KEY_VALUE_LEFT:
        case HGUI_GUI_EVENT_KEY_VALUE_a:
        {
            if(NULL != screen_curve_object.stData.pstFocused)
            {
                SGUI_Curve_UpdateFocusPoint(&screen_curve_object,screen_curve_object.stData.pstFocused->stPoint.iX-1,screen_curve_object.stData.pstFocused->stPoint.iY);
            }
            else
            {
                if(screen_curve_argument > screen_curve_object.stData.stPoints.pstHead->stPoint.iX)
                {
                    --screen_curve_argument;
                }
            }
            screen_curve_action_repaint(Interface,NULL);
        }
        break;
        case HGUI_GUI_EVENT_KEY_VALUE_RIGHT:
        case HGUI_GUI_EVENT_KEY_VALUE_d:
        {
            if(NULL != screen_curve_object.stData.pstFocused)
            {
                SGUI_Curve_UpdateFocusPoint(&screen_curve_object,screen_curve_object.stData.pstFocused->stPoint.iX+1,screen_curve_object.stData.pstFocused->stPoint.iY);
            }
            else
            {
                if(screen_curve_argument < screen_curve_object.stData.stPoints.pstEnd->stPoint.iX)
                {
                    ++screen_curve_argument;
                }
            }
            screen_curve_action_repaint(Interface,NULL);
        }
        break;
        case HGUI_GUI_EVENT_KEY_VALUE_INSERT:
        {
            SGUI_CURVE_POINT* point=screen_curve_new_point();
            if(NULL != point)
            {
                SGUI_Curve_InitializePoint(point,0,screen_curve_object.stParam.stYRange.iMin);
                SGUI_Curve_InsertAfterPoint(&screen_curve_object,point);
                screen_curve_action_repaint(Interface,NULL);
            }
        }
        break;
        case HGUI_GUI_EVENT_KEY_VALUE_DELETE:
        {
            SGUI_CURVE_POINT* point=SGUI_Curve_FocusedPoint(&screen_curve_object);
            if(NULL != point)
            {
                if(SGUI_Curve_RemovePoint(&screen_curve_object,point))
                {
                    screen_curve_release_point(point);
                }
                screen_curve_action_repaint(Interface,NULL);
            }
        }
        break;
        default:
        {
        }
        break;
        }
    }

    if(screen_is_update_event(pstEvent))
    {
        screen_curve_action_repaint(Interface,NULL);
    }
    return HMI_RET_NORMAL;
}
static HMI_ENGINE_RESULT screen_curve_action_postprocess(SGUI_SCR_DEV* Interface, HMI_ENGINE_RESULT eProcResult, SGUI_INT iActionID)
{
    if(HMI_PROCESS_SUCCESSFUL(eProcResult) && iActionID==GUI_ACTION_BACK)
    {
        HMI_GoBack(NULL);
    }
    return HMI_RET_NORMAL;
}
static HMI_SCREEN_ACTION screen_curve_actions=
{
    screen_curve_action_initialize,
    screen_curve_action_prepare,
    screen_curve_action_repaint,
    screen_curve_action_processevent,
    screen_curve_action_postprocess,
};
HMI_SCREEN_OBJECT screen_curve=
{
    GUI_SCREEN_ID_CURVE,
    &screen_curve_actions
};

/*
 * 实时曲线屏幕
 */
#define     REALTIME_GRAPH_POINT_COUNT          (25)

static SGUI_RTGRAPH_POINT screen_realtime_graph_point[REALTIME_GRAPH_POINT_COUNT]= {0};
static SGUI_RTGRAPH screen_realtime_graph_object= {};
static int screen_realtime_graph_value=0;
static unsigned int screen_realtime_graph_phase=0;
static std::chrono::steady_clock::time_point screen_realtime_graph_last_time;

static HMI_ENGINE_RESULT screen_realtime_graph_action_initialize(SGUI_SCR_DEV* Interface)
{
    screen_realtime_graph_object.stLayout.iX=1;
    screen_realtime_graph_object.stLayout.iY=12;
    screen_realtime_graph_object.stLayout.iWidth=Interface->stSize.iWidth-2;
    screen_realtime_graph_object.stLayout.iHeight=40;
    screen_realtime_graph_object.stData.stRecorder.iSize=REALTIME_GRAPH_POINT_COUNT;
    screen_realtime_graph_object.stData.stRecorder.arrValue=screen_realtime_graph_point;
    screen_realtime_graph_object.stData.BaseLineValue=0;
    screen_realtime_graph_object.stControl.yAxisMax=50;
    screen_realtime_graph_object.stControl.yAxisMin=-50;
    screen_realtime_graph_object.stControl.EnableBaseline=SGUI_TRUE;
    return HMI_RET_NORMAL;
}
static HMI_ENGINE_RESULT screen_realtime_graph_action_prepare(SGUI_SCR_DEV* Interface, const void* pstParameters)
{
    /* Clean screen display. */
    SGUI_Basic_ClearScreen(Interface);
    /* Refresh display. */
    SGUI_Basic_ResetMask(Interface);

    SGUI_RealtimeGraph_Initialize(&screen_realtime_graph_object);
    screen_realtime_graph_value=0;
    screen_realtime_graph_phase=0;
    screen_realtime_graph_last_time=std::chrono::steady_clock::now();

    return HMI_RET_NORMAL;
}
static HMI_ENGINE_RESULT screen_realtime_graph_action_repaint(SGUI_SCR_DEV* Interface, const void* pstParameters)
{
    char buffer[32]= {0};

    if(Interface==NULL)
    {
        return HMI_RET_INVALID_DATA;
    }

    /* Refresh display. */
    SGUI_Basic_ResetMask(Interface);
    screen_draw_header(Interface,"实时曲线");
    SGUI_RealtimeGraph_Repaint(Interface,&screen_realtime_graph_object);
    hsprintf(buffer,"值:%d",screen_realtime_graph_value);
    screen_draw_footer_pair(Interface,buffer,"F3返回");

    return HMI_RET_NORMAL;
}
static HMI_ENGINE_RESULT screen_realtime_graph_action_processevent(SGUI_SCR_DEV* Interface, const HMI_EVENT_BASE* pstEvent, SGUI_INT* piActionID)
{
    if(piActionID!=NULL)
    {
        *piActionID=GUI_ACTION_NONE;
    }
    hgui_gui_event_key_t key;
    if(screen_key_is_pressed(pstEvent,&key))
    {
        switch(key.key_value)
        {
        case HGUI_GUI_EVENT_KEY_VALUE_ESCAPE:
        case HGUI_GUI_EVENT_KEY_VALUE_F3:
        {
            if(piActionID!=NULL)
            {
                *piActionID=GUI_ACTION_BACK;
            }
        }
        break;
        default:
        {
        }
        break;
        }
    }

    if(screen_is_update_event(pstEvent))
    {
        std::chrono::steady_clock::time_point now_time=std::chrono::steady_clock::now();
        if(std::chrono::duration_cast<std::chrono::milliseconds>(now_time-screen_realtime_graph_last_time).count() >= 100)
        {
            screen_realtime_graph_last_time=now_time;
            screen_realtime_graph_phase=(screen_realtime_graph_phase+10)%200;
            if(screen_realtime_graph_phase < 100)
            {
                screen_realtime_graph_value=(int)screen_realtime_graph_phase-50;
            }
            else
            {
                screen_realtime_graph_value=150-(int)screen_realtime_graph_phase;
            }
            SGUI_RealtimeGraph_PushRear(&screen_realtime_graph_object,screen_realtime_graph_value);
        }
        screen_realtime_graph_action_repaint(Interface,NULL);
    }
    return HMI_RET_NORMAL;
}
static HMI_ENGINE_RESULT screen_realtime_graph_action_postprocess(SGUI_SCR_DEV* Interface, HMI_ENGINE_RESULT eProcResult, SGUI_INT iActionID)
{
    if(HMI_PROCESS_SUCCESSFUL(eProcResult) && iActionID==GUI_ACTION_BACK)
    {
        HMI_GoBack(NULL);
    }
    return HMI_RET_NORMAL;
}
static HMI_SCREEN_ACTION screen_realtime_graph_actions=
{
    screen_realtime_graph_action_initialize,
    screen_realtime_graph_action_prepare,
    screen_realtime_graph_action_repaint,
    screen_realtime_graph_action_processevent,
    screen_realtime_graph_action_postprocess,
};
HMI_SCREEN_OBJECT screen_realtime_graph=
{
    GUI_SCREEN_ID_REALTIME_GRAPH,
    &screen_realtime_graph_actions
};

/*
 * 时钟表盘屏幕
 */
static void screen_clock_draw_dial(SGUI_SCR_DEV* Interface,int hour,int minute,int second)
{
    const int center_x=27;
    const int center_y=32;
    const int radius=18;

    /* 清空表盘。 */
    SGUI_Basic_DrawCircle(Interface,center_x,center_y,radius+1,SGUI_COLOR_FRGCLR,SGUI_COLOR_TRANS);
    SGUI_Basic_DrawCircle(Interface,center_x,center_y,radius,SGUI_COLOR_FRGCLR,SGUI_COLOR_BKGCLR);

    /* 刻度。 */
    for(int angle=0; angle<360; angle+=30)
    {
        SGUI_PolarCoord_DrawLine(Interface,center_x,center_y,angle,radius-3,radius-1,SGUI_COLOR_FRGCLR);
    }

    /* 时针。 */
    {
        int angle=((hour%12)*30)+(minute/2);
        SGUI_PolarCoord_DrawLine(Interface,center_x,center_y,450-angle,0,radius-9,SGUI_COLOR_FRGCLR);
    }
    /* 分针。 */
    {
        int angle=(minute%60)*6;
        SGUI_PolarCoord_DrawLine(Interface,center_x,center_y,450-angle,0,radius-5,SGUI_COLOR_FRGCLR);
    }
    /* 秒针。 */
    {
        int angle=(second%60)*6;
        SGUI_PolarCoord_DrawLine(Interface,center_x,center_y,450-angle,0,radius-2,SGUI_COLOR_FRGCLR);
    }
}

static HMI_ENGINE_RESULT screen_clock_action_prepare(SGUI_SCR_DEV* Interface, const void* pstParameters)
{
    /* Clean screen display. */
    SGUI_Basic_ClearScreen(Interface);
    /* Refresh display. */
    SGUI_Basic_ResetMask(Interface);
    return HMI_RET_NORMAL;
}
static HMI_ENGINE_RESULT screen_clock_action_repaint(SGUI_SCR_DEV* Interface, const void* pstParameters)
{
    char buffer[32]= {0};

    if(Interface==NULL)
    {
        return HMI_RET_INVALID_DATA;
    }

    /* Refresh display. */
    SGUI_Basic_ResetMask(Interface);
    screen_draw_header(Interface,"时钟表盘");

    {
        time_t current_time=htime(NULL);
        htm_t current_tm= {0};
        hlibc_localtime_r(&current_time,&current_tm);

        screen_clock_draw_dial(Interface,current_tm.tm_hour,current_tm.tm_min,current_tm.tm_sec);

        SGUI_Basic_FillRectangleArea(Interface,50,16,78,32,SGUI_COLOR_BKGCLR);
        hsprintf(buffer,"%02d:%02d:%02d",current_tm.tm_hour,current_tm.tm_min,current_tm.tm_sec);
        SGUI_Text_DrawText(Interface,buffer,&hsimplegui_font_ascii_12,56,20,SGUI_DRAW_NORMAL);
        hsprintf(buffer,"%04d-%02d-%02d",current_tm.tm_year+1900,current_tm.tm_mon+1,current_tm.tm_mday);
        SGUI_Text_DrawText(Interface,buffer,&hsimplegui_font_ascii_12,56,34,SGUI_DRAW_NORMAL);
    }

    screen_draw_footer(Interface,"返回(F3)",true);

    return HMI_RET_NORMAL;
}
static HMI_ENGINE_RESULT screen_clock_action_processevent(SGUI_SCR_DEV* Interface, const HMI_EVENT_BASE* pstEvent, SGUI_INT* piActionID)
{
    if(piActionID!=NULL)
    {
        *piActionID=GUI_ACTION_NONE;
    }
    hgui_gui_event_key_t key;
    if(screen_key_is_pressed(pstEvent,&key))
    {
        switch(key.key_value)
        {
        case HGUI_GUI_EVENT_KEY_VALUE_ESCAPE:
        case HGUI_GUI_EVENT_KEY_VALUE_F3:
        {
            if(piActionID!=NULL)
            {
                *piActionID=GUI_ACTION_BACK;
            }
        }
        break;
        default:
        {
        }
        break;
        }
    }

    if(screen_is_update_event(pstEvent))
    {
        screen_clock_action_repaint(Interface,NULL);
    }
    return HMI_RET_NORMAL;
}
static HMI_ENGINE_RESULT screen_clock_action_postprocess(SGUI_SCR_DEV* Interface, HMI_ENGINE_RESULT eProcResult, SGUI_INT iActionID)
{
    if(HMI_PROCESS_SUCCESSFUL(eProcResult) && iActionID==GUI_ACTION_BACK)
    {
        HMI_GoBack(NULL);
    }
    return HMI_RET_NORMAL;
}
static HMI_SCREEN_ACTION screen_clock_actions=
{
    NULL,
    screen_clock_action_prepare,
    screen_clock_action_repaint,
    screen_clock_action_processevent,
    screen_clock_action_postprocess,
};
HMI_SCREEN_OBJECT screen_clock=
{
    GUI_SCREEN_ID_CLOCK,
    &screen_clock_actions
};
