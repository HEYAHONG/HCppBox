/***************************************************************
 * Name:      android_hgui_driver.h
 * Purpose:   Android平台下的hgui驱动。
 * Author:    HYH (hyhsystem.cn)
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __ANDROID_HGUI_DRIVER_H__
#define __ANDROID_HGUI_DRIVER_H__

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/** \brief 注册Android平台下的默认hgui驱动
 *
 * 注意：本函数只是设置hgui默认驱动，并不会创建Android的View。
 *
 * \return bool 是否注册成功
 *
 */
bool android_hgui_driver_register(void);

/** \brief 获取当前帧缓冲区宽度
 *
 * \return int 宽度
 *
 */
int android_hgui_driver_width(void);

/** \brief 获取当前帧缓冲区高度
 *
 * \return int 高度
 *
 */
int android_hgui_driver_height(void);

/** \brief 将绘图缓冲区提交到显示缓冲区
 *
 * 只有调用此函数后，Java层才能看到最新画面。
 * 在C++的一次完整界面操作（如一次App更新）完成后调用，可避免画面闪烁。
 *
 */
void android_hgui_driver_present(void);

/** \brief 获取当前帧序号（每次调用android_hgui_driver_present时改变）
 *
 * \return uint64_t 帧序号
 *
 */
uint64_t android_hgui_driver_frame_id(void);

/** \brief 拷贝帧缓冲区（32位ARGB8888）
 *
 * \param dst uint32_t* 目标缓冲区
 * \param capacity size_t 目标缓冲区可容纳的像素数
 * \return size_t 实际拷贝的像素数
 *
 */
size_t android_hgui_driver_copy_frame(uint32_t *dst,size_t capacity);

bool android_hgui_driver_touch_event(uint8_t action,uint32_t id,int32_t x,int32_t y,int32_t dx,int32_t dy,uint16_t pressure);

bool android_hgui_driver_mouse_motion_event(int32_t x,int32_t y,int32_t xrel,int32_t yrel,uint16_t button_state);

bool android_hgui_driver_mouse_button_event(uint8_t button,uint8_t press_or_release,int32_t x,int32_t y);

bool android_hgui_driver_mouse_wheel_event(int32_t x,int32_t y,int32_t delta);

bool android_hgui_driver_joystick_axis_event(uint8_t which,uint8_t axis,int16_t value);

bool android_hgui_driver_joystick_hat_event(uint8_t which,uint8_t hat,uint8_t value);

bool android_hgui_driver_joystick_button_event(uint8_t which,uint8_t button,uint8_t press_or_release);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __ANDROID_HGUI_DRIVER_H__
