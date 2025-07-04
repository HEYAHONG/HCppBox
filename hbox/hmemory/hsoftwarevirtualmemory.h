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


/*
 * 存储在软件虚拟内存的环形缓冲区
 */
typedef struct hsoftwarevirtualmemory_ringbuf hsoftwarevirtualmemory_ringbuf_t;
struct hsoftwarevirtualmemory_ringbuf
{
    const hsoftwarevirtualmemory_map_item_t * map_table;            /**<  映射表 */
    uintptr_t                                 ringbuf_base;         /**<  环形缓冲区地址*/
    size_t                                    ringbuf_size;         /**<  环形缓冲区所占地址空间的大小*/
};

/** \brief 获取虚拟内存环形缓冲区
 *
 * \param map_table const hsoftwarevirtualmemory_map_item_t* 映射表，为NULL时采用默认映射表或者全局映射表
 * \param ringbuf_base uintptr_t 环形缓冲区地址
 * \param ringbuf_size size_t 环形缓冲区所占地址空间的大小
 * \return hsoftwarevirtualmemory_ringbuf_t 虚拟内存环形缓冲区
 *
 */
hsoftwarevirtualmemory_ringbuf_t hsoftwarevirtualmemory_ringbuf_get(const hsoftwarevirtualmemory_map_item_t * map_table,uintptr_t  ringbuf_base,size_t ringbuf_size);

/** \brief 设置虚拟内存环形缓冲区锁
 *
 * \param buff hsoftwarevirtualmemory_ringbuf_t 虚拟内存环形缓冲区
 * \param  usr void* 用户指针
 * \param  mutex_lock 加锁,第一个参数是用户指针
 * \param  mutex_unlock 解锁,第一个参数是用户指针
 */
void hsoftwarevirtualmemory_ringbuf_set_lock(hsoftwarevirtualmemory_ringbuf_t buff,void *usr,void (*mutex_lock)(void *),void (*mutex_unlock)(void *));

/** \brief 获取虚拟内存环形缓冲区的用户指针(usr)
 *
 * \param buff hsoftwarevirtualmemory_ringbuf_t 虚拟内存环形缓冲区
 * \return void* usr指针
 *
 */
void * hsoftwarevirtualmemory_ringbuf_get_usr_ptr(hsoftwarevirtualmemory_ringbuf_t buff);

/** \brief 获取虚拟内存环形缓冲区的缓冲区中数据长度
 *
 * \param buff hsoftwarevirtualmemory_ringbuf_t 虚拟内存环形缓冲区
 * \return size_t 缓冲区中数据长度
 *
 */
size_t hsoftwarevirtualmemory_ringbuf_get_length(hsoftwarevirtualmemory_ringbuf_t buff);

/** \brief 获取虚拟内存环形缓冲区的最大数据长度
 *
 * \param buff hsoftwarevirtualmemory_ringbuf_t 虚拟内存环形缓冲区
 * \return size_t 最大数据长度
 *
 */
size_t hsoftwarevirtualmemory_ringbuf_get_max_length(hsoftwarevirtualmemory_ringbuf_t buff);

/** \brief 向虚拟内存环形缓冲区写入数据
 *
 * \param buff hsoftwarevirtualmemory_ringbuf_t 虚拟内存环形缓冲区
 * \param data uint8_t* 数据指针
 * \param data_length size_t 数据长度
 * \return size_t 已写入的长度
 *
 */
size_t hsoftwarevirtualmemory_ringbuf_input(hsoftwarevirtualmemory_ringbuf_t buff,const uint8_t *data,size_t data_length);

/** \brief 从虚拟内存环形缓冲区读取数据
 *
 * \param buff hsoftwarevirtualmemory_ringbuf_t 虚拟内存环形缓冲区
 * \param data uint8_t* 数据指针
 * \param data_length size_t 数据长度
 * \return size_t 已读取的长度
 *
 */
size_t hsoftwarevirtualmemory_ringbuf_output(hsoftwarevirtualmemory_ringbuf_t buff,uint8_t *data,size_t data_length);


/** \brief 从虚拟内存环形缓冲区读取数据(不清除缓冲区的数据)
 *
 * \param buff hsoftwarevirtualmemory_ringbuf_t 虚拟内存环形缓冲区
 * \param data uint8_t* 数据指针
 * \param data_length size_t 数据长度
 * \return size_t 已读取的长度
 *
 */
size_t hsoftwarevirtualmemory_ringbuf_output_no_clear(hsoftwarevirtualmemory_ringbuf_t buff,uint8_t *data,size_t data_length);



#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HSOFTWAREVIRTUALMEMORY_H__
