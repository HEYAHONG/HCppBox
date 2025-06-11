/***************************************************************
 * Name:      hgui_gui_common.h
 * Purpose:   声明common接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-11-24
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HGUI_GUI_COMMON_H__
#define __HGUI_GUI_COMMON_H__
#include "stdbool.h"
#include "stdint.h"
#include "string.h"
#include "stdlib.h"
#include "wchar.h"
#include "hdefaults.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus



/** \brief GUI内存分配
 *
 * \param nBytes size_t 大小
 * \return void* 指针，失败为NULL
 *
 */
void * hgui_gui_malloc(size_t nBytes);


typedef void * (*hgui_gui_malloc_t)(size_t nBytes);

/** \brief 设置GUI内存分配钩子
 *
 * \param hook hgui_gui_malloc_t malloc 钩子
 *
 */
void hgui_gui_malloc_set(hgui_gui_malloc_t hook);


/** \brief GUI内存默认分配(不受钩子影响)
 *
 * \param nBytes size_t 大小
 * \return void* 指针，失败为NULL
 *
 */
void * hgui_gui_default_malloc(size_t nBytes);


/** \brief GUI内存释放
 *
 * \param ptr void* 指针
 *
 */
void hgui_gui_free(void *ptr);


typedef void (*hgui_gui_free_t)(void *ptr);

/** \brief  设置GUI内存释放钩子
 *
 * \param hook hgui_gui_free_t free钩子
 *
 */
void hgui_gui_free_set(hgui_gui_free_t hook);

/** \brief GUI内存默认释放(不受钩子影响)
 *
 * \param ptr void* 指针
 *
 */
void hgui_gui_default_free(void *ptr);

typedef struct
{
    ssize_t x;//X坐标,负数表示特殊坐标
    ssize_t y;//Y坐标,负数表示特殊坐标
    size_t  w;//宽度
    size_t  h;//高度
} hgui_gui_common_rectangle_t;/**< 矩形 */

#define HGUI_GUI_COMMON_RECTANGLE_X_LEFT            (-1)   /**< 矩形在父矩形的左边*/
#define HGUI_GUI_COMMON_RECTANGLE_X_CENTER          (-2)   /**< 矩形在父矩形的中间*/
#define HGUI_GUI_COMMON_RECTANGLE_X_RIGHT           (-3)   /**< 矩形在父矩形的右边*/

#define HGUI_GUI_COMMON_RECTANGLE_Y_TOP             (-1)   /**< 矩形在父矩形的顶部*/
#define HGUI_GUI_COMMON_RECTANGLE_Y_CENTER          (-2)   /**< 矩形在父矩形的中间*/
#define HGUI_GUI_COMMON_RECTANGLE_Y_BOTTOM          (-3)   /**< 矩形在父矩形的底部*/

/** \brief 将矩形坐标转化为绝对坐标
 *
 * \param rect hgui_gui_common_rectangle_t 待转化的矩形
 * \param x size_t X坐标
 * \param y size_t Y坐标
 * \param w size_t 宽度
 * \param h size_t 高度
 * \return hgui_gui_common_rectangle_t 转化后的坐标
 *
 */
hgui_gui_common_rectangle_t hgui_gui_common_rectangle_absolute_convert(hgui_gui_common_rectangle_t rect,size_t x,size_t y,size_t w,size_t h);



#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __HGUI_GUI_COMMON_H__
