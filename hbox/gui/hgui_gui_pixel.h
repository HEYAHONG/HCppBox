/***************************************************************
 * Name:      hgui_gui_pixel.h
 * Purpose:   声明pixel接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-11-24
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HGUI_GUI_PIXEL_H__
#define __HGUI_GUI_PIXEL_H__
#include "hgui_gui_common.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

typedef union
{
    uint8_t     pixel_1_bit;    //单色
    uint8_t     pixel_2_bits;   //2位色
    uint8_t     pixel_4_bits;   //4位色
    uint8_t     pixel_8_bits;   //8位色,如RGB233
    uint16_t    pixel_16_bits;  //16位色,如RGB565等
    uint32_t    pixel_24_bits;  //24位色,如RGB888等
    uint32_t    pixel_32_bits;  //32位色（用于像素处理），如ARGB8888等
} hgui_pixel_t; /**< 像素定义 */


/** \brief 32位色ARGB8888(24位色RGB888)转16位色RGB565
 *
 * \param rgb 32位色ARGB8888(24位色RGB888)
 *
 */
#ifndef HGUI_RGB_32_to_16
#define HGUI_RGB_32_to_16(rgb) (((((unsigned int)(rgb)) & 0xFF) >> 3) | ((((unsigned int)(rgb)) & 0xFC00) >> 5) | ((((unsigned int)(rgb)) & 0xF80000) >> 8))
#endif

/** \brief 16位色RGB565转32位色ARGB8888(24位色RGB888)
 *
 * \param rgb 16位色RGB565
 *
 */
#ifndef HGUI_RGB_16_to_32
#define HGUI_RGB_16_to_32(rgb) ((0xFF << 24) | ((((unsigned int)(rgb)) & 0x1F) << 3) | ((((unsigned int)(rgb)) & 0x7E0) << 5) | ((((unsigned int)(rgb)) & 0xF800) << 8))
#endif



typedef enum
{
    HGUI_PIXEL_MODE_NONE=0, //模式位未设置，获取全局设置
    HGUI_PIXEL_MODE_1_BIT,
    HGUI_PIXEL_MODE_MONOCHROME=HGUI_PIXEL_MODE_1_BIT,
    HGUI_PIXEL_MODE_2_BIT,
    HGUI_PIXEL_MODE_CGA=HGUI_PIXEL_MODE_2_BIT,
    HGUI_PIXEL_MODE_4_BIT,
    HGUI_PIXEL_MODE_EGA=HGUI_PIXEL_MODE_4_BIT,
    HGUI_PIXEL_MODE_8_BITS,
    HGUI_PIXEL_MODE_VGA=HGUI_PIXEL_MODE_8_BITS,
    HGUI_PIXEL_MODE_16_BITS,
    HGUI_PIXEL_MODE_XGA=HGUI_PIXEL_MODE_16_BITS,
    HGUI_PIXEL_MODE_24_BITS,
    HGUI_PIXEL_MODE_SVGA=HGUI_PIXEL_MODE_24_BITS,
    HGUI_PIXEL_MODE_32_BITS
}  hgui_pixel_mode_t;

/** \brief 获取全局像素设置
 *
 * \return hgui_pixel_mode_t 全局像素设置
 *
 */
hgui_pixel_mode_t hgui_pixel_global_mode_get(void);

/** \brief  设置全局像素设置
 *
 * \param mode hgui_pixel_mode_t 全局像素设置
 *
 */
void hgui_pixel_global_mode_set(hgui_pixel_mode_t mode);



#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __HGUI_GUI_PIXEL_H__
