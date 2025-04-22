/***************************************************************
 * Name:      hgui_scene1_app.h
 * Purpose:   声明app接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-12-19
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HGUI_SCENE1_APP_H__
#define __HGUI_SCENE1_APP_H__
#include "../hgui_gui_common.h"
#include "../hgui_gui_driver.h"
#include "../hgui_gui_event.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

struct hgui_scene1_app;
typedef struct hgui_scene1_app hgui_scene1_app_t; /**< APP对象类型 */

extern const hgui_scene1_app_t g_hgui_scene1_app;  /**<  全局APP对象,APP对象全局唯一*/


/** \brief  APP实现文件,用户需要在自己的C语言文件中通过#include HGUI_SCENE1_APP_IMPLEMENT实现APP。 \
 *          在实现APP前，用户可定义一些宏定义配置APP。
 *
 *
 */
#ifndef HGUI_SCENE1_APP_IMPLEMENT
#define HGUI_SCENE1_APP_IMPLEMENT  "hgui/hgui_gui_scene1/hgui_scene1_app.c"
#endif // HGUI_SCENE1_APP_IMPLEMENT

/** \brief 获取驱动指针
 *
 * \param app const hgui_scene1_app_t* App指针
 * \return const hgui_driver_t* GUI驱动指针
 *
 */
hgui_driver_t * hgui_scene1_app_driver_get(const hgui_scene1_app_t *app);

/** \brief 获取App宽度
 *
 * \param app const hgui_scene1_app_t* App指针
 * \return size_t 宽度
 *
 */
size_t hgui_scene1_app_width_get(const hgui_scene1_app_t *app);


/** \brief 获取App高度
 *
 * \param app const hgui_scene1_app_t* App指针
 * \return size_t 高度
 *
 */
size_t hgui_scene1_app_height_get(const hgui_scene1_app_t *app);


/** \brief App初始化
 *
 * \param app const hgui_scene1_app_t* App指针
 * \param usr void* 用户自定义参数
 * \return bool 是否成功初始化
 *
 */
bool hgui_scene1_app_init(const hgui_scene1_app_t *app,void *usr);

/** \brief App是否初始化
 *
 * \param app const hgui_scene1_app_t* App指针
 * \return bool 是否成功初始化
 *
 */
bool hgui_scene1_app_was_init(const hgui_scene1_app_t *app);

/** \brief App更新,注意：在某些场景下不进行更新将导致异常。
 *
 * \param app const hgui_scene1_app_t* App指针
 * \param usr void* 用户自定义参数
 * \return bool 是否成功
 *
 */
bool hgui_scene1_app_update(const hgui_scene1_app_t *app,void *usr);

/** \brief App设置刷新标志，调用此函数后下次App更新时将进行App刷新操作。
 *
 * \param app const hgui_scene1_app_t* App指针
 *
 */
void hgui_scene1_app_need_refresh(const hgui_scene1_app_t *app);


struct hgui_scene1_screen_base;
typedef struct hgui_scene1_screen_base hgui_scene1_screen_base_t;

/** \brief App进入某一块屏幕，注意：调用此函数后上一块屏幕已被清理。
 *
 * \param app const hgui_scene1_app_t* App指针
 * \param screen hgui_scene1_screen_base_t* 屏幕指针
 * \return bool 是否进入成功
 *
 */
bool hgui_scene1_app_screen_stack_push(const hgui_scene1_app_t *app,hgui_scene1_screen_base_t *screen);

/** \brief App退出某一块屏幕,注意：调用此函数后该屏幕已被清理。
 *
 * \param app const hgui_scene1_app_t* App指针
 * \return bool 是否进入成功
 *
 */
bool hgui_scene1_app_screen_stack_pop(const hgui_scene1_app_t *app);

/** \brief App获取当前屏幕
 *
 * \param app const hgui_scene1_app_t* App指针
 * \return hgui_scene1_screen_base_t* 当前屏幕指针
 *
 */
hgui_scene1_screen_base_t * hgui_scene1_app_current_screen_get(const hgui_scene1_app_t *app);


/** \brief 设置事件回调，此回调在所有界面均有效。注意：通常用于在后台处理按键事件。
 *
 * \param app const hgui_scene1_app_t* App指针
 * \param cb hgui_gui_event_callback_t 事件回调
 *
 */
void hgui_scene1_app_event_callback_set(const hgui_scene1_app_t *app,hgui_gui_event_callback_t cb);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __HGUI_SCENE1_APP_H__
