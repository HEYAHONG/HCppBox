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

bool hgui_gui_event_mouse_motion_emit(const hgui_gui_event_mouse_motion_t *motion,hgui_gui_event_callback_t event_cb,void *usr)
{
    if(motion!=NULL && event_cb!=NULL)
    {
        hgui_gui_event_mouse_motion_t motion_event=(*motion);
        return event_cb((uint8_t)HGUI_GUI_EVENT_TYPE_MOUSE_MOTION,(void *)&motion_event,sizeof(motion_event),usr);
    }
    return false;
}

hgui_gui_event_mouse_motion_t *hgui_gui_event_mouse_motion_get(hgui_gui_event_mouse_motion_t *motion,uint8_t type,void *eventparam,size_t eventparam_length,void *usr)
{
    if(motion==NULL)
    {
        return NULL;
    }
    if(type==(uint8_t)HGUI_GUI_EVENT_TYPE_MOUSE_MOTION)
    {
        if(eventparam!=NULL && (eventparam_length >= sizeof(*motion)))
        {
            (*motion)=(*(hgui_gui_event_mouse_motion_t *)eventparam);
            return motion;
        }
    }

    return NULL;
}

bool hgui_gui_event_mouse_button_emit(const hgui_gui_event_mouse_button_t *button,hgui_gui_event_callback_t event_cb,void *usr)
{
    if(button!=NULL && event_cb!=NULL)
    {
        hgui_gui_event_mouse_button_t button_event=(*button);
        return event_cb((uint8_t)HGUI_GUI_EVENT_TYPE_MOUSE_BUTTON,(void *)&button_event,sizeof(button_event),usr);
    }
    return false;
}

hgui_gui_event_mouse_button_t *hgui_gui_event_mouse_button_get(hgui_gui_event_mouse_button_t *button,uint8_t type,void *eventparam,size_t eventparam_length,void *usr)
{
    if(button==NULL)
    {
        return NULL;
    }
    if(type==(uint8_t)HGUI_GUI_EVENT_TYPE_MOUSE_BUTTON)
    {
        if(eventparam!=NULL && (eventparam_length >= sizeof(*button)))
        {
            (*button)=(*(hgui_gui_event_mouse_button_t *)eventparam);
            return button;
        }
    }

    return NULL;
}

bool hgui_gui_event_touch_emit(const hgui_gui_event_touch_t *touch,hgui_gui_event_callback_t event_cb,void *usr)
{
    if(touch!=NULL && event_cb!=NULL)
    {
        hgui_gui_event_touch_t touch_event=(*touch);
        return event_cb((uint8_t)HGUI_GUI_EVENT_TYPE_TOUCH,(void *)&touch_event,sizeof(touch_event),usr);
    }
    return false;
}

hgui_gui_event_touch_t *hgui_gui_event_touch_get(hgui_gui_event_touch_t *touch,uint8_t type,void *eventparam,size_t eventparam_length,void *usr)
{
    if(touch==NULL)
    {
        return NULL;
    }
    if(type==(uint8_t)HGUI_GUI_EVENT_TYPE_TOUCH)
    {
        if(eventparam!=NULL && (eventparam_length >= sizeof(*touch)))
        {
            (*touch)=(*(hgui_gui_event_touch_t *)eventparam);
            return touch;
        }
    }

    return NULL;
}

bool hgui_gui_event_joy_axis_emit(const hgui_gui_event_joy_axis_t *axis,hgui_gui_event_callback_t event_cb,void *usr)
{
    if(axis!=NULL && event_cb!=NULL)
    {
        hgui_gui_event_joy_axis_t axis_event=(*axis);
        return event_cb((uint8_t)HGUI_GUI_EVENT_TYPE_JOY_AXIS,(void *)&axis_event,sizeof(axis_event),usr);
    }
    return false;
}

hgui_gui_event_joy_axis_t *hgui_gui_event_joy_axis_get(hgui_gui_event_joy_axis_t *axis,uint8_t type,void *eventparam,size_t eventparam_length,void *usr)
{
    if(axis==NULL)
    {
        return NULL;
    }
    if(type==(uint8_t)HGUI_GUI_EVENT_TYPE_JOY_AXIS)
    {
        if(eventparam!=NULL && (eventparam_length >= sizeof(*axis)))
        {
            (*axis)=(*(hgui_gui_event_joy_axis_t *)eventparam);
            return axis;
        }
    }

    return NULL;
}

bool hgui_gui_event_joy_ball_emit(const hgui_gui_event_joy_ball_t *ball,hgui_gui_event_callback_t event_cb,void *usr)
{
    if(ball!=NULL && event_cb!=NULL)
    {
        hgui_gui_event_joy_ball_t ball_event=(*ball);
        return event_cb((uint8_t)HGUI_GUI_EVENT_TYPE_JOY_BALL,(void *)&ball_event,sizeof(ball_event),usr);
    }
    return false;
}

hgui_gui_event_joy_ball_t *hgui_gui_event_joy_ball_get(hgui_gui_event_joy_ball_t *ball,uint8_t type,void *eventparam,size_t eventparam_length,void *usr)
{
    if(ball==NULL)
    {
        return NULL;
    }
    if(type==(uint8_t)HGUI_GUI_EVENT_TYPE_JOY_BALL)
    {
        if(eventparam!=NULL && (eventparam_length >= sizeof(*ball)))
        {
            (*ball)=(*(hgui_gui_event_joy_ball_t *)eventparam);
            return ball;
        }
    }

    return NULL;
}

bool hgui_gui_event_joy_hat_emit(const hgui_gui_event_joy_hat_t *hat,hgui_gui_event_callback_t event_cb,void *usr)
{
    if(hat!=NULL && event_cb!=NULL)
    {
        hgui_gui_event_joy_hat_t hat_event=(*hat);
        return event_cb((uint8_t)HGUI_GUI_EVENT_TYPE_JOY_HAT,(void *)&hat_event,sizeof(hat_event),usr);
    }
    return false;
}

hgui_gui_event_joy_hat_t *hgui_gui_event_joy_hat_get(hgui_gui_event_joy_hat_t *hat,uint8_t type,void *eventparam,size_t eventparam_length,void *usr)
{
    if(hat==NULL)
    {
        return NULL;
    }
    if(type==(uint8_t)HGUI_GUI_EVENT_TYPE_JOY_HAT)
    {
        if(eventparam!=NULL && (eventparam_length >= sizeof(*hat)))
        {
            (*hat)=(*(hgui_gui_event_joy_hat_t *)eventparam);
            return hat;
        }
    }

    return NULL;
}

bool hgui_gui_event_joy_button_emit(const hgui_gui_event_joy_button_t *button,hgui_gui_event_callback_t event_cb,void *usr)
{
    if(button!=NULL && event_cb!=NULL)
    {
        hgui_gui_event_joy_button_t button_event=(*button);
        return event_cb((uint8_t)HGUI_GUI_EVENT_TYPE_JOY_BUTTON,(void *)&button_event,sizeof(button_event),usr);
    }
    return false;
}

hgui_gui_event_joy_button_t *hgui_gui_event_joy_button_get(hgui_gui_event_joy_button_t *button,uint8_t type,void *eventparam,size_t eventparam_length,void *usr)
{
    if(button==NULL)
    {
        return NULL;
    }
    if(type==(uint8_t)HGUI_GUI_EVENT_TYPE_JOY_BUTTON)
    {
        if(eventparam!=NULL && (eventparam_length >= sizeof(*button)))
        {
            (*button)=(*(hgui_gui_event_joy_button_t *)eventparam);
            return button;
        }
    }

    return NULL;
}

