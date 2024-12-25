/***************************************************************
 * Name:      hgui_gui_event.h
 * Purpose:   声明event接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-12-25
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HGUI_GUI_EVENT_H__
#define __HGUI_GUI_EVENT_H__
#include "hgui_gui_common.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/** \brief 事件处理回调
 *
 * \param type uint8_t 事件类型，见hgui_gui_event_type_t。
 * \param eventparam void* 事件参数
 * \param eventparam_length size_t 事件参数长度
 * \param usr void* 用户参数
 * \return bool 事件是否处理成功，一般用于用户事件
 *
 */
typedef bool (*hgui_gui_event_callback_t)(uint8_t type,void *eventparam,size_t eventparam_length,void *usr);

typedef enum
{
    HGUI_GUI_EVENT_TYPE_KEY=0,//按键事件
    HGUI_GUI_EVENT_TYPE_USR_START=100,//用户事件起始，大于此值的事件类型用户可自行使用。
} hgui_gui_event_type_t;



#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __HGUI_GUI_EVENT_H__
