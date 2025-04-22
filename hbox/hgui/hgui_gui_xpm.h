/***************************************************************
 * Name:      hgui_gui_xpm.h
 * Purpose:   声明xpm接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-11-30
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HGUI_GUI_XPM_H__
#define __HGUI_GUI_XPM_H__
#include "hgui_gui_common.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

typedef struct
{
    size_t  width;//宽
    size_t  height;//高
    size_t  ncolors;//颜色数
    size_t  cpp;//每个颜色所占字节
    size_t  x_hotspot;//热点X,可选
    size_t  y_hotspot;//热点Y，可选
    bool    XPMEXT;//是否有XPMEXT
} hgui_gui_xpm_header_t;

/** \brief 获取XPM的头，主要用于获取尺寸（如宽、高等信息）
 *
 * \param *xpm const char*const xpm图像
 * \return hgui_gui_xpm_header_t xpm头
 *
 */
hgui_gui_xpm_header_t hgui_gui_xpm_header_get(const char *const *xpm);


/** \brief 获取XPM图像颜色信息
 *
 * \param *xpm const char*const xpm图像
 * \param x size_t X坐标(图像)
 * \param y size_t Y坐标(图像)
 * \return const char* XPM颜色描述字符串(不含用于检索颜色的字符)，失败返回NULL
 *
 */
const char *hgui_gui_xpm_pixel_color_get(const char *const *xpm,size_t x,size_t y);


/** \brief XPM画像素点
 *
 * \param x size_t X坐标
 * \param y size_t Y坐标
 * \param color uint32_t 颜色(以#号开头的字符串给出)
 * \param usr void* 用户参数
 * \return bool 是否成功
 *
 */
typedef bool (*hgui_gui_xpm_draw_color_pixel_t)(size_t x,size_t y,uint32_t color,void *usr);

/** \brief XPM画彩色图像（色彩以#号开头的字符串给出）
 *
 * \param *xpm const char*const xpm图像
 * \param x size_t X坐标
 * \param y size_t Y坐标
 * \param draw_pixel hgui_gui_xpm_draw_color_pixel_t 画像素
 * \param usr void* 用户参数
 * \return bool 是否画成功
 *
 */
bool hgui_gui_xpm_draw_color(const char *const *xpm,size_t x,size_t y,hgui_gui_xpm_draw_color_pixel_t draw_pixel,void *usr);

/*
 * 测试图像 xpm.xpm
 */
extern const char * const hgui_gui_xpm_xpm_xpm[];

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __HGUI_GUI_XPM_H__
