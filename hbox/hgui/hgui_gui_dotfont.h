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
extern const hgui_gui_dotfont_t hgui_gui_dotfont_unicode_dummy_1212;
extern const hgui_gui_dotfont_t hgui_gui_dotfont_unicode_dummy_1616;
extern const hgui_gui_dotfont_t hgui_gui_dotfont_unicode_dummy_2424;
extern const hgui_gui_dotfont_t hgui_gui_dotfont_unicode_dummy_3232;

/** \brief 获取某个字体的dummy字体
 *
 * \param dotfont const hgui_gui_dotfont_t* 原字体
 * \return hgui_gui_dotfont_t dummy字体
 *
 */
hgui_gui_dotfont_t hgui_gui_dotfont_add_dummy(const hgui_gui_dotfont_t * dotfont);

/* 采用dotfontscan生成的字体文件,需要添加dotfontscan生成的文件到源代码构建列表*/
extern const uint32_t hdotfont_char_set_12_size;
extern const uint8_t *const hdotfont_char_set_12[];
extern const uint32_t hdotfont_char_set_13_size;
extern const uint8_t *const hdotfont_char_set_13[];
extern const uint32_t hdotfont_char_set_14_size;
extern const uint8_t *const hdotfont_char_set_14[];
extern const uint32_t hdotfont_char_set_15_size;
extern const uint8_t *const hdotfont_char_set_15[];
extern const uint32_t hdotfont_char_set_16_size;
extern const uint8_t *const hdotfont_char_set_16[];
extern const uint32_t hdotfont_char_set_17_size;
extern const uint8_t *const hdotfont_char_set_17[];
extern const uint32_t hdotfont_char_set_18_size;
extern const uint8_t *const hdotfont_char_set_18[];
extern const uint32_t hdotfont_char_set_19_size;
extern const uint8_t *const hdotfont_char_set_19[];
extern const uint32_t hdotfont_char_set_20_size;
extern const uint8_t *const hdotfont_char_set_20[];
extern const uint32_t hdotfont_char_set_21_size;
extern const uint8_t *const hdotfont_char_set_21[];
extern const uint32_t hdotfont_char_set_22_size;
extern const uint8_t *const hdotfont_char_set_22[];
extern const uint32_t hdotfont_char_set_23_size;
extern const uint8_t *const hdotfont_char_set_23[];
extern const uint32_t hdotfont_char_set_24_size;
extern const uint8_t *const hdotfont_char_set_24[];
extern const uint32_t hdotfont_char_set_25_size;
extern const uint8_t *const hdotfont_char_set_25[];
extern const uint32_t hdotfont_char_set_26_size;
extern const uint8_t *const hdotfont_char_set_26[];
extern const uint32_t hdotfont_char_set_27_size;
extern const uint8_t *const hdotfont_char_set_27[];
extern const uint32_t hdotfont_char_set_28_size;
extern const uint8_t *const hdotfont_char_set_28[];
extern const uint32_t hdotfont_char_set_29_size;
extern const uint8_t *const hdotfont_char_set_29[];
extern const uint32_t hdotfont_char_set_30_size;
extern const uint8_t *const hdotfont_char_set_30[];
extern const uint32_t hdotfont_char_set_31_size;
extern const uint8_t *const hdotfont_char_set_31[];
extern const uint32_t hdotfont_char_set_32_size;
extern const uint8_t *const hdotfont_char_set_32[];

typedef struct hgui_gui_dotfont_hdotfont hgui_gui_dotfont_hdotfont_t;
struct hgui_gui_dotfont_hdotfont
{
    hgui_gui_dotfont_t font;
    size_t char_set_size;//字符集大小
};

/** \brief 从dotfontscan获取字体。
 *
 * \param const*char_set const uint8_t* hdotfont字符集
 * \param font_size size_t 字体大小
 * \return hgui_gui_dotfont_hdotfont_t 字体(其指针可转换为hgui_gui_dotfont_t指针)
 *
 */
hgui_gui_dotfont_hdotfont_t hgui_gui_dotfont_hdotfont(const uint8_t *const char_set[],uint32_t char_set_size,size_t font_size);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __HGUI_GUI_DOTFONT_H__


