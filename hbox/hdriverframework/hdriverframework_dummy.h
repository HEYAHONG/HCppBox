/***************************************************************
 * Name:      hdriverframework_dummy.h
 * Purpose:   声明hdriverframework_dummy接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-29
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HDRIVERFRAMEWORK_DUMMY_H__
#define __HDRIVERFRAMEWORK_DUMMY_H__
#include "hdriverframework_dummy.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus


typedef struct hdriverframework_driver_dummy hdriverframework_driver_dummy_t;



struct hdriverframework_driver_dummy
{
    hdriverframework_driver_base_t base;                    /**< 基础驱动 */
};

/** \brief 获取驱动名称
 *
 * \param drv hdriverframework_driver_dummy_t* 驱动指针
 * \return const char* 名称,失败将返回NULL
 *
 */
const char * hdriverframework_driver_dummy_getname(hdriverframework_driver_dummy_t *drv);

/** \brief 驱动初始化
 *
 * \param drv hdriverframework_driver_dummy_t* 驱动指针
 * \return int 错误代码
 *
 */
int hdriverframework_driver_dummy_init(hdriverframework_driver_dummy_t *drv);
#define hdriverframework_driver_dummy_probe hdriverframework_driver_dummy_init

/** \brief 驱动反初始化
 *
 * \param drv hdriverframework_driver_dummy_t* 驱动指针
 * \return int 错误代码
 *
 */
int hdriverframework_driver_dummy_deinit(hdriverframework_driver_dummy_t *drv);
#define hdriverframework_driver_dummy_remove hdriverframework_driver_dummy_deinit


/** \brief 获取设备树兼容字符串数组(以NULL结尾)
 *
 * \param drv hdriverframework_driver_dummy_t* 驱动指针
 * \return const char** 设备树兼容字符串数组(以NULL结尾)，失败返回NULL
 *
 */
const char ** hdriverframework_driver_dummy_get_of_compatible(hdriverframework_driver_dummy_t *drv);



#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HDRIVERFRAMEWORK_DUMMY_H__
