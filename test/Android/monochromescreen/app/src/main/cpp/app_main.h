/***************************************************************
 * Name:      app_main.h
 * Purpose:   Android应用入口（由Java层调用）。
 * Author:    HYH (hyhsystem.cn)
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __APP_MAIN_H__
#define __APP_MAIN_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/** \brief 应用初始化（在GUI线程调用）
 *
 * \return bool 是否初始化成功
 *
 */
bool app_gui_init(void);

/** \brief 应用更新一次（在GUI线程调用）
 *
 */
void app_gui_update(void);

/** \brief 应用释放（在GUI线程调用）
 *
 */
void app_gui_deinit(void);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __APP_MAIN_H__
