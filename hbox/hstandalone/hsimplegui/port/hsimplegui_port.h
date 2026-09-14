/***************************************************************
 * Name:      hsimplegui_port.h
 * Purpose:   声明hsimplegui_port接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-09-14
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HSIMPLEGUI_PORT_H__
#define __HSIMPLEGUI_PORT_H__
#include "hdefaults.h"
#include "hgui.h"
#include "SGUI_Basic.h"
#include "SGUI_Text.h"
#include "SGUI_Menu.h"
#include "SGUI_Notice.h"
#include "SGUI_VariableBox.h"
#include "HMI_Engine.h"


#ifdef __cplusplus
extern "C"
{
#endif

#ifndef HSIMPLEGUI_BMP_DATA_BUFFER_SIZE
#define HSIMPLEGUI_BMP_DATA_BUFFER_SIZE (1024)
#endif

struct hsimplegui;
typedef struct hsimplegui hsimplegui_t;
struct hsimplegui
{
    HMI_ENGINE_OBJECT           Engine;
    SGUI_SCR_DEV                DeviceInterface;
    SGUI_BYTE                   BmpDataBuffer[HSIMPLEGUI_BMP_DATA_BUFFER_SIZE];
    uintptr_t                   usr;
};

/** \brief simplegui初始化
 *
 * \param gui hsimplegui_t* simplegui指针
 * \param w size_t 宽(与驱动初始参数一致)
 * \param h size_t 高(与驱动初始参数一致)
 * \param screen HMI_SCREEN_OBJECT** 屏幕列表
 * \param screen_count size_t 屏幕列表长度
 * \param main_screen_id int 主屏幕ID
 * \param usr void* 用户参数
 *
 */
void hsimplegui_init(hsimplegui_t *gui,size_t w,size_t h,HMI_SCREEN_OBJECT** screen,size_t screen_count,int main_screen_id,void *usr);


/** \brief simplegui更新
 *
 * \param gui hsimplegui_t* simplegui指针
 *
 */
void hsimplegui_update(hsimplegui_t *gui);

enum
{
    HHMI_EVENT_HGUI=0,           /**< HGUI事件（包含按键事件等，具体见hgui） */
    HHMI_EVENT_UPDATE,           /**< 更新事件，调用hsimplegui_update时调用 */
};

typedef struct
{
    uint8_t type;
    void   *eventparam;
    size_t  eventparam_length;
} hhmi_event_data_hgui_t;

HMI_EVENT_TYPE_DECLARE(hhmi_event_hgui_t,hhmi_event_data_hgui_t);

typedef struct
{
    hsimplegui_t *gui;
} hhmi_event_data_update_t;

HMI_EVENT_TYPE_DECLARE(hhmi_event_update_t,hhmi_event_data_update_t);

#ifdef __cplusplus
}
#endif

#endif
