/***************************************************************
 * Name:      hgui_scene1_screen.h
 * Purpose:   声明screen接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-12-20
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HGUI_SCENE1_SCREEN_H__
#define __HGUI_SCENE1_SCREEN_H__
#include "hgui_scene1_app.h"
#include "../hgui_gui_event.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/*
 * 在本场景中，每一块界面都是全屏的，故而采用屏幕(screen)作为顶级界面对象。
 */


struct hgui_scene1_screen_base;
//已在hgui_scene1_app.h定义
//typedef struct hgui_scene1_screen_base hgui_scene1_screen_base_t;

struct hgui_scene1_screen_base
{
    /** \brief 进入屏幕，一般用于初始化屏幕
     *
     * \param screen hgui_scene1_screen_base_t* 屏幕指针
     * \param app hgui_scene1_app_t* APP指针
     *
     */
    void (*enter)(hgui_scene1_screen_base_t *screen,const hgui_scene1_app_t *app);
    /** \brief 离开屏幕，一般用于清理屏幕
     *
     * \param screen hgui_scene1_screen_base_t* 屏幕指针
     * \param app hgui_scene1_app_t* APP指针
     *
     */
    void (*leave)(hgui_scene1_screen_base_t *screen,const hgui_scene1_app_t *app);
    /** \brief 更新屏幕，一般用于绘制屏幕
    *
    * \param screen hgui_scene1_screen_base_t* 屏幕指针
    * \param app hgui_scene1_app_t* APP指针
    *
    */
    void (*update)(hgui_scene1_screen_base_t *screen,const hgui_scene1_app_t *app);

    /** \brief 事件处理回调(一般由App调用)
     *
     * \param screen hgui_scene1_screen_base_t* 屏幕指针
     * \param app hgui_scene1_app_t* APP指针
     * \param type uint8_t 事件类型，见hgui_gui_event_type_t。
     * \param eventparam void* 事件参数
     * \param eventparam_length size_t 事件参数长度
     * \param usr void* 用户参数
     * \return bool 事件是否处理成功，一般返回true
     *
     */
    bool (*event_cb)(hgui_scene1_screen_base_t *screen,const hgui_scene1_app_t *app,uint8_t type,void *eventparam,size_t eventparam_length,void *usr);

    /** \brief 用户自定义参数
     *
     *
     */
    void *usr;
};


/** \brief 进入屏幕，一般用于初始化屏幕
 *
 * \param screen hgui_scene1_screen_base_t* 屏幕指针
 * \param app hgui_scene1_app_t* APP指针
 *
 */
void hgui_scene1_screen_base_enter(hgui_scene1_screen_base_t *screen,const hgui_scene1_app_t *app);


/** \brief 离开屏幕，一般用于清理屏幕
 *
 * \param screen hgui_scene1_screen_base_t* 屏幕指针
 * \param app hgui_scene1_app_t* APP指针
 *
 */
void hgui_scene1_screen_base_leave(hgui_scene1_screen_base_t *screen,const hgui_scene1_app_t *app);


/** \brief 更新屏幕，一般用于绘制屏幕
*
* \param screen hgui_scene1_screen_base_t* 屏幕指针
* \param app hgui_scene1_app_t* APP指针
*
*/
void hgui_scene1_screen_base_update(hgui_scene1_screen_base_t *screen,const hgui_scene1_app_t *app);


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __HGUI_SCENE1_SCREEN_H__
