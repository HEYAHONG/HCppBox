/***************************************************************
 * Name:      hsoftwarevirtualmemory.h
 * Purpose:   声明hsoftwarevirtualmemory接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-06-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HSOFTWAREVIRTUALMEMORY_H__
#define __HSOFTWAREVIRTUALMEMORY_H__
#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus


struct hsoftwarevirtualmemory_map_item;
typedef struct hsoftwarevirtualmemory_map_item hsoftwarevirtualmemory_map_item_t;

/** \brief 软件虚拟内存读
 *
 * \param map_item const hsoftwarevirtualmemory_map_item_t * 隐射项
 * \param address uintptr_t 待访问的地址
 * \param data uint8_t* 读取地址(缓冲地址)
 * \param length size_t 长度
 * \return size_t 已读取的长度
 *
 */
typedef size_t (*hsoftwarevirtualmemory_read_callback_t)(const hsoftwarevirtualmemory_map_item_t *map_item,uintptr_t address,uint8_t *data,size_t length);

/** \brief 软件虚拟内存写
 *
 * \param map_item const hsoftwarevirtualmemory_map_item_t * 隐射项
 * \param address uintptr_t 待访问的地址
 * \param data uint8_t* 写入地址(缓冲地址)
 * \param length size_t 长度
 * \return size_t 已写入的长度
 *
 */
typedef size_t (*hsoftwarevirtualmemory_write_callback_t)(const hsoftwarevirtualmemory_map_item_t *map_item,uintptr_t address,const uint8_t *data,size_t length);


struct hsoftwarevirtualmemory_map_item
{
    uintptr_t  map_address;
    size_t     map_size; /* 当map_address与map_size同时为0时，表示默认项且表示映射表结束 */
    hsoftwarevirtualmemory_read_callback_t   read_callback;
    hsoftwarevirtualmemory_write_callback_t  write_callback;
    void *                                   usr;
};

/** \brief 软件虚拟内存读
 *
 * \param map_table const hsoftwarevirtualmemory_map_item_t * 映射表(注意:表中最后一项map_address与map_size必须同时为0)
 * \param address uintptr_t 待访问的地址
 * \param data uint8_t* 读取地址(缓冲地址)
 * \param length size_t 长度
 * \return size_t 已读取的长度
 *
 */
size_t hsoftwarevirtualmemory_read(const hsoftwarevirtualmemory_map_item_t *map_table,uintptr_t address,uint8_t *data,size_t length);

/** \brief 软件虚拟内存写
 *
 * \param map_table const hsoftwarevirtualmemory_map_item_t * 映射表(注意:表中最后一项map_address与map_size必须同时为0)
 * \param address uintptr_t 待访问的地址
 * \param data uint8_t* 写入地址(缓冲地址)
 * \param length size_t 长度
 * \return size_t 已写入的长度
 *
 */
size_t hsoftwarevirtualmemory_write(const hsoftwarevirtualmemory_map_item_t *map_table,uintptr_t address,const uint8_t *data,size_t length);

/*
 * 全局映射表(默认为NULL，当被设置后，若读写的参数中映射表为NULL，则使用此映射表)
 */
extern const hsoftwarevirtualmemory_map_item_t * hsoftwarevirtualmemory_global_map_table;

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HSOFTWAREVIRTUALMEMORY_H__
