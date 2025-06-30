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

/** \brief 匹配设备树兼容字符串
 *
 * \param drv hdriverframework_driver_dummy_t* 驱动指针
 * \param of_compatible const char* 待匹配的设备树兼容字符串
 * \return bool 是否匹配
 *
 */
bool hdriverframework_driver_dummy_match_of_compatible(hdriverframework_driver_dummy_t *drv,const char *of_compatible);


/** \brief 虚拟内存读
 *
 * \param drv hdriverframework_driver_dummy_t* 驱动指针
 * \param map_item const hsoftwarevirtualmemory_map_item_t* 虚拟内存的映射表项
 * \param address uintptr_t 虚拟内存地址
 * \param data uint8_t* 读取缓冲地址
 * \param length size_t 读取缓冲长度
 * \return size_t 已读取的大小
 *
 */
size_t hdriverframework_driver_dummy_vm_read(hdriverframework_driver_dummy_t *drv,const hsoftwarevirtualmemory_map_item_t *map_item,uintptr_t address,uint8_t *data,size_t length);

/** \brief 虚拟内存写
 *
 * \param drv hdriverframework_driver_dummy_t* 驱动指针
 * \param map_item const hsoftwarevirtualmemory_map_item_t* 虚拟内存的映射表项
 * \param address uintptr_t 虚拟内存地址
 * \param data const uint8_t* 写入缓冲地址
 * \param length size_t 写入缓冲长度
 * \return size_t 已写入的大小
 *
 */
size_t hdriverframework_driver_dummy_vm_write(hdriverframework_driver_dummy_t *drv,const hsoftwarevirtualmemory_map_item_t *map_item,uintptr_t address,const uint8_t *data,size_t length);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HDRIVERFRAMEWORK_DUMMY_H__
