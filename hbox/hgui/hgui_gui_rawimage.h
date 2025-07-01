/***************************************************************
 * Name:      hgui_gui_rawimage.h
 * Purpose:   声明rawimage接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-07-01
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HGUI_GUI_RAWIMAGE_H__
#define __HGUI_GUI_RAWIMAGE_H__
#include "hgui_gui_common.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

typedef struct
{
    size_t  width;//宽
    size_t  height;//高
    size_t  cpp;//每个颜色所占字节
    const uint8_t *data;
} hgui_gui_rawimage_t;


/** \brief 原始图像画像素点
 *
 * \param rawimage const hgui_gui_rawimage_t * 原始图像
 * \param x size_t X坐标
 * \param y size_t Y坐标
 * \param color uint8_t*
 * \param usr void* 用户参数
 * \return bool 是否成功
 *
 */
typedef bool (*hgui_gui_rawimage_draw_color_pixel_t)(const hgui_gui_rawimage_t *rawimage,size_t x,size_t y,const uint8_t *color,void *usr);

/** \brief 原始图像画图像
 *
 * \param rawimage const hgui_gui_rawimage_t * 原始图像
 * \param x size_t X坐标
 * \param y size_t Y坐标
 * \param draw_pixel hgui_gui_rawimage_draw_color_pixel_t 画像素
 * \param usr void* 用户参数
 * \return bool 是否画成功
 *
 */
bool hgui_gui_rawimage_draw_color(const hgui_gui_rawimage_t *rawimage,size_t x,size_t y,hgui_gui_rawimage_draw_color_pixel_t draw_pixel,void *usr);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __HGUI_GUI_RAWIMAGE_H__
