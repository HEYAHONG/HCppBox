/***************************************************************
 * Name:      gui_driver.h
 * Purpose:   声明driver接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-11-24
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HGUI_GUI_DRIVER_H__
#define __HGUI_GUI_DRIVER_H__
#include "hgui_gui_pixel.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

struct hgui_driver;
typedef struct hgui_driver hgui_driver_t;

struct hgui_driver
{
    void *usr;//用户参数
    /** \brief 画像素，注意：画像素与填充矩形至少需要实现一个
     *
     * \param driver hgui_driver_t* 驱动指针
     * \param x size_t 像素的x坐标
     * \param y size_t 像素的y坐标
     * \param pixel hgui_pixel_t 像素
     * \return bool 是否成功画像素
     *
     */
    bool (*draw_pixel)(hgui_driver_t *driver,size_t x,size_t y,hgui_pixel_t pixel);

    /** \brief 填充矩形，注意：画像素与填充矩形至少需要实现一个，在某些设备上（如LCD），填充矩形更常见。
     *
     * \param driver hgui_driver_t* 驱动指针
     * \param x size_t 矩形的x坐标
     * \param y size_t 矩形的y坐标
     * \param w size_t 宽（X轴）,至少为1
     * \param h size_t 高（Y轴），至少为1
     * \param pixel hgui_pixel_t 像素
     * \return bool 是成功填充矩形
     *
     */
    bool (*fill_rectangle)(hgui_driver_t *driver,size_t x,size_t y,size_t w,size_t h,hgui_pixel_t pixel);

    /** \brief 复位显示
     *
     * \param driver hgui_driver_t* 驱动指针
     * \return bool 是否成功复位
     *
     */
    bool (*reset)(hgui_driver_t *driver);

    /** \brief 获取像素模式
     *
     * \param driver hgui_driver_t* 驱动指针
     * \param new_mode hgui_pixel_mode_t* 新模式，NULL表示仅读取。
     * \return hgui_pixel_mode_t 像素模式
     *
     */
    hgui_pixel_mode_t  (*pixel_mode)(hgui_driver_t *driver,hgui_pixel_mode_t *new_mode);

    /** \brief 重设大小
     *
     * \param driver hgui_driver_t* 驱动指针
     * \param w ssize_t* 宽度指针，传入待重设的值，返回重设后的值。传入负数表示读取数据。
     * \param h ssize_t* 高度指针，传入待重设的值，返回重设后的值。传入负数表示读取数据。
     * \return bool 是否重设成功
     *
     */
    bool (*resize)(hgui_driver_t *driver,ssize_t *w,ssize_t *h);

    /** \brief 是否正常工作,在某些情况下（如windows中）需要在主线程中调用此函数更新driver内部状态
     *
     * \param driver hgui_driver_t* 驱动指针
     * \return bool 是否正常工作
     *
     */
    bool (*is_ok)(hgui_driver_t *driver);
};

#define HGUI_DRIVER_INITIALIZER {0} /**< hgui_driver_t初始化项 */


/** \brief 获取默认驱动指针,经典应用为只有一个屏幕时
 *
 * \return hgui_driver_t* 默认驱动指针
 *
 */
hgui_driver_t *hgui_driver_default_get(void);

/** \brief 画像素，注意：画像素与填充矩形至少需要实现一个
 *
 * \param driver hgui_driver_t* 驱动指针
 * \param x size_t 像素的x坐标
 * \param y size_t 像素的y坐标
 * \param pixel hgui_pixel_t 像素
 * \return bool 是否成功画像素
 *
 */
bool hgui_driver_draw_pixel(hgui_driver_t *driver,size_t x,size_t y,hgui_pixel_t pixel);


/** \brief 填充矩形，注意：画像素与填充矩形至少需要实现一个，在某些设备上（如LCD），填充矩形更常见。
*
* \param driver hgui_driver_t* 驱动指针
* \param x size_t 矩形的x坐标
* \param y size_t 矩形的y坐标
* \param w size_t 宽（X轴）,至少为1
* \param h size_t 高（Y轴），至少为1
* \param pixel hgui_pixel_t 像素
* \return bool 是成功填充矩形
*
*/
bool hgui_driver_fill_rectangle(hgui_driver_t *driver,size_t x,size_t y,size_t w,size_t h,hgui_pixel_t pixel);


/** \brief 复位显示
 *
 * \param driver hgui_driver_t* 驱动指针
 * \return bool 是否成功复位
 *
 */
bool hgui_driver_reset(hgui_driver_t *driver);

/** \brief 获取像素模式
 *
 * \param driver hgui_driver_t* 驱动指针
 * \param new_mode hgui_pixel_mode_t* 新模式，NULL表示仅读取。
 * \return hgui_pixel_mode_t 像素模式
 *
 */
hgui_pixel_mode_t  hgui_driver_pixel_mode(hgui_driver_t *driver,hgui_pixel_mode_t* new_mode);

/** \brief 重设大小
 *
 * \param driver hgui_driver_t* 驱动指针
 * \param w ssize_t* 宽度指针，传入待重设的值，返回重设后的值。传入负数表示读取数据。
 * \param h ssize_t* 高度指针，传入待重设的值，返回重设后的值。传入负数表示读取数据。
 * \return bool 是否重设成功
 *
 */
bool hgui_driver_resize(hgui_driver_t* driver,ssize_t *w,ssize_t *h);


/** \brief 是否正常工作
 *
 * \param driver hgui_driver_t* 驱动指针
 * \return bool 是否正常工作
 *
 */
bool  hgui_driver_is_ok(hgui_driver_t *driver);

/** \brief 更新驱动内部状态(某些情况下使用)
 *
 */
#ifndef hgui_driver_update
#define hgui_driver_update hgui_driver_is_ok
#endif // hgui_driver_update

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __HGUI_GUI_DRIVER_H__
