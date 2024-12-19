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
#define HGUI_SCENE1_APP_IMPLEMENT  "gui/hgui_gui_scene1/hgui_scene1_app.c"
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

/** \brief App更新,注意：此函数只能在成功初始化后使用，在某些场景下不进行更新将导致异常。
 *
 * \param app const hgui_scene1_app_t* App指针
 * \param usr void* 用户自定义参数
 * \return bool 是否成功
 *
 */
bool hgui_scene1_app_update(const hgui_scene1_app_t *app,void *usr);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __HGUI_SCENE1_APP_H__
