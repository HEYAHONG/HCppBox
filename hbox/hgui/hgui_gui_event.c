/***************************************************************
 * Name:      hgui_gui_event.c
 * Purpose:   实现event接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-12-25
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hgui_gui_event.h"

bool hgui_gui_event_key_emit(const hgui_gui_event_key_t *key,hgui_gui_event_callback_t event_cb,void *usr)
{
    if(key!=NULL && event_cb!=NULL)
    {
        hgui_gui_event_key_t key_event=(*key);
        return event_cb((uint8_t)HGUI_GUI_EVENT_TYPE_KEY,(void *)&key_event,sizeof(key_event),usr);
    }
    return false;
}

hgui_gui_event_key_t *hgui_gui_event_key_get(hgui_gui_event_key_t *key,uint8_t type,void *eventparam,size_t eventparam_length,void *usr)
{
    if(key==NULL)
    {
        return NULL;
    }
    if(type==(uint8_t)HGUI_GUI_EVENT_TYPE_KEY)
    {
        if(eventparam!=NULL && (eventparam_length >= sizeof(*key)))
        {
            (*key)=(*(hgui_gui_event_key_t *)eventparam);
            return key;
        }
    }

    return NULL;
}

