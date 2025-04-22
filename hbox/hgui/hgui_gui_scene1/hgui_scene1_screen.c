/***************************************************************
 * Name:      hgui_scene1_screen.h
 * Purpose:   实现screen接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-12-20
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hgui_scene1_screen.h"


void hgui_scene1_screen_base_enter(hgui_scene1_screen_base_t *screen,const hgui_scene1_app_t *app)
{
    if(screen!=NULL)
    {
        if(screen->enter!=NULL)
        {
            screen->enter(screen,app);
        }
    }
}

void hgui_scene1_screen_base_leave(hgui_scene1_screen_base_t *screen,const hgui_scene1_app_t *app)
{
    if(screen!=NULL)
    {
        if(screen->leave!=NULL)
        {
            screen->leave(screen,app);
        }
    }
}

void hgui_scene1_screen_base_update(hgui_scene1_screen_base_t *screen,const hgui_scene1_app_t *app)
{
    if(screen!=NULL)
    {
        if(screen->update!=NULL)
        {
            screen->update(screen,app);
        }
    }
}

