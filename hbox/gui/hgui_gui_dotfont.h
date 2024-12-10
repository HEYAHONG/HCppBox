/***************************************************************
 * Name:      hgui_gui_dotfont.h
 * Purpose:   声明dotfont接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-11-29
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HGUI_GUI_DOTFONT_H__
#define __HGUI_GUI_DOTFONT_H__
#include "hgui_gui_common.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

struct hgui_gui_dotfont;
typedef struct hgui_gui_dotfont hgui_gui_dotfont_t;


/** \brief 点阵字体画像素点
 *
 * \param dotfont const hgui_gui_dotfont_t *字体
 * \param x size_t X坐标
 * \param y size_t Y坐标
 * \param point bool 点，true=应在此处画点
 * \param usr void* 用户参数
 * \return bool 是否成功
 *
 */
typedef bool (*hgui_gui_dotfont_draw_pixel_t)(const hgui_gui_dotfont_t * dotfont,size_t x,size_t y,bool point,void *usr);


struct hgui_gui_dotfont
{
    const uint8_t *font;   //字体数据
    /** \brief 显示字符
     *
     * \param dotfont const hgui_gui_dotfont_t* 字体
     * \param Char uint32_t 字体编码，一般是UNICODE码
     * \param x size_t X坐标,字体左上角
     * \param y size_t Y坐标,字体左上角
     * \param draw_pixel hgui_gui_dotfont_draw_pixel_t 点阵字体画像素点
     * \param usr void* 用户参数，将传入draw_pixel
     * \return bool 是否显示成功
     *
     */
    bool    (*show_char)(const hgui_gui_dotfont_t * dotfont,uint32_t Char,size_t x,size_t y,hgui_gui_dotfont_draw_pixel_t draw_pixel,void *usr);
    uint8_t w;              //字体宽度
    uint8_t h;              //字体高度
};

/** \brief 显示点阵字符
 *
 * \param dotfont const hgui_gui_dotfont_t* 字体
 * \param Char uint32_t 字体编码，一般是UNICODE码
 * \param x size_t X坐标,字体左上角
 * \param y size_t Y坐标,字体左上角
 * \param draw_pixel hgui_gui_dotfont_draw_pixel_t 点阵字体画像素点
 * \param usr void* 用户参数，将传入draw_pixel
 * \return bool 是否显示成功
 *
 */
bool hgui_gui_dotfont_show_char(const hgui_gui_dotfont_t * dotfont,uint32_t Char,size_t x,size_t y,hgui_gui_dotfont_draw_pixel_t draw_pixel,void *usr);

/** \brief 显示点阵字符串
 *
 * \param dotfont const hgui_gui_dotfont_t* 字体
 * \param ascii_string const char* ASCII字符串(注意:大于等于0x80的字符视为无效字符,将直接忽略)，需要以NULL结尾
 * \param x size_t X坐标,字体左上角
 * \param y size_t Y坐标,字体左上角
 * \param w size_t 屏幕宽度,超过屏幕宽度时将自动换行。
 * \param draw_pixel hgui_gui_dotfont_draw_pixel_t 点阵字体画像素点
 * \param usr void* 用户参数，将传入draw_pixel
 * \return bool 是否显示成功
 *
 */
bool hgui_gui_dotfont_show_ascii_string(const hgui_gui_dotfont_t * dotfont,const char *ascii_string,size_t x,size_t y,size_t w,hgui_gui_dotfont_draw_pixel_t draw_pixel,void *usr);


/** \brief 显示点阵字符串
 *
 * \param dotfont const hgui_gui_dotfont_t* 字体
 * \param unicode_string const wchar_t* UNICODE字符串，需要以NULL结尾
 * \param x size_t X坐标,字体左上角
 * \param y size_t Y坐标,字体左上角
 * \param w size_t 屏幕宽度,超过屏幕宽度时将自动换行。
 * \param draw_pixel hgui_gui_dotfont_draw_pixel_t 点阵字体画像素点
 * \param usr void* 用户参数，将传入draw_pixel
 * \return bool 是否显示成功
 *
 */
bool hgui_gui_dotfont_show_unicode_string(const hgui_gui_dotfont_t * dotfont,const wchar_t *unicode_string,size_t x,size_t y,size_t w,hgui_gui_dotfont_draw_pixel_t draw_pixel,void *usr);


extern const hgui_gui_dotfont_t hgui_gui_dotfont_ascii_0806;
extern const hgui_gui_dotfont_t hgui_gui_dotfont_ascii_1206;
extern const hgui_gui_dotfont_t hgui_gui_dotfont_ascii_1608;
extern const hgui_gui_dotfont_t hgui_gui_dotfont_ascii_2416;

//假字体，一般用于调试
/*
 * 对于dummy字体而言，可构造一个临时字体并将字体参数(font)可设置为其它字体(hgui_gui_dotfont_t * 指针),这样就可以实现当其它字体失败时显示dummy字体。
 */
extern const hgui_gui_dotfont_t hgui_gui_dotfont_unicode_dummy_1616;
extern const hgui_gui_dotfont_t hgui_gui_dotfont_unicode_dummy_2424;
extern const hgui_gui_dotfont_t hgui_gui_dotfont_unicode_dummy_3232;

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __HGUI_GUI_DOTFONT_H__


