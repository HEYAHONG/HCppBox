/***************************************************************
 * Name:      hmemorystaticallocator.h
 * Purpose:   声明hmemorystaticallocator接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-07-06
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HMEMORYSTATICALLOCATOR_H__
#define __HMEMORYSTATICALLOCATOR_H__
#include "hsoftwarevirtualmemory.h"
#include "stdint.h"
#include "stdlib.h"
#include "stdbool.h"
#include "string.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

struct hmemorystaticallocator_lock;
typedef struct hmemorystaticallocator_lock hmemorystaticallocator_lock_t;
struct hmemorystaticallocator_lock
{
    void *usr;
    void (*mutex_lock)(void *);             /**<  锁,参数为用户参数。*/
    void (*mutex_unlock)(void *);           /**<  解锁,参数为用户参数。*/
};

extern const hmemorystaticallocator_lock_t hmemorystaticallocator_global_lock;

struct hmemorystaticallocator_item;
typedef struct hmemorystaticallocator_item hmemorystaticallocator_item_t;
struct hmemorystaticallocator_item
{
    uintptr_t       address;                  /**< 起始地址(注意：此地址为相对地址，实际起始地址=传入静态分配器的地址+此地址)，当此地址为0时,表示上一项的结束地址，注意：用户自行保证对齐 */
    size_t          obj_size;                 /**< 单个对象的地址,当此值为0时表示分配表结束，注意：用户自行保证对齐 */
    size_t          obj_count;                /**< 总对象数量，当此值为0时表示数量由下一项的起始地址决定，若下一项的起始地址小于当前地址或者为0,则可分配的数量为0 */
    const char *    name;                     /**< 名称,为NULL表示匿名（不可以通过名称查找，但可使用alloc分配），注意：用户需要手动保持名称的唯一性，否则某些项会无效*/
    uint32_t        flags;                    /**< 标志，为0表示默认值 */
};

enum
{
    HMEMORYSTATICALLOCATOR_FLAGS_NO_ALLOC       =       (1UL<<1),       /**<  不进行分配，分配时直接返回（不标记已分配） */
};

extern const hmemorystaticallocator_item_t * hmemorystaticallocator_global_table;       /**< 全局分配表，当分配表为NULL时使用 */


struct hmemorystaticallocator_bitmap;
typedef struct hmemorystaticallocator_bitmap hmemorystaticallocator_bitmap_t;
struct hmemorystaticallocator_bitmap
{
    hmemorystaticallocator_lock_t *         lock;               /**< 位图锁 */
    hsoftwarevirtualmemory_map_item_t*      map_table;          /**< 软件虚拟内存映射表 */
    uint8_t *                               bitmap_address;     /**< 位图地址,注意：此地址支持软件虚拟内存访问 */
    size_t                                  bitmap_size;        /**< 位图大小 */
};

/** \brief 获取位图设置
 *
 * \param bitmap const hmemorystaticallocator_bitmap_t* 位图
 * \param index size_t 引索
 * \return bool 是否被设置
 *
 */
bool hmemorystaticallocator_bitmap_get(const hmemorystaticallocator_bitmap_t *bitmap,size_t index);

/** \brief 尝试设置位图
 *
 * \param bitmap const hmemorystaticallocator_bitmap_t* 位图
 * \param index size_t 引索
 * \return bool 是否成功设置
 *
 */
bool hmemorystaticallocator_bitmap_try_set(const hmemorystaticallocator_bitmap_t *bitmap,size_t index);

/** \brief 尝试清除位图
 *
 * \param bitmap const hmemorystaticallocator_bitmap_t* 位图
 * \param index size_t 引索
 * \return bool 是否成功清除
 *
 */
bool hmemorystaticallocator_bitmap_try_clear(const hmemorystaticallocator_bitmap_t *bitmap,size_t index);

/** \brief 获取静态分配表某项
 *
 * \param table const hmemorystaticallocator_item_t* 分配表指针
 * \param name const char* 名称
 * \return const hmemoryallocator_item_t* 原始配置项
 *
 */
const hmemorystaticallocator_item_t * hmemorystaticallocator_get_table_item(const hmemorystaticallocator_item_t * table,const char *name);

/*
 * 内存布局： 可分配空间+位图(用于保存分配情况)
 */

/** \brief 获取静态分配表可分配空间大小。
 *
 * \param table const hmemorystaticallocator_item_t* 分配表指针
 * \return size_t 大小(字节)，失败返回0
 *
 */
size_t hmemorystaticallocator_get_table_alloc_size(const hmemorystaticallocator_item_t * table);


/** \brief 获取静态分配表位图大小。
 *
 * \param table const hmemorystaticallocator_item_t* 分配表指针
 * \return size_t 大小(字节)，失败返回0
 *
 */
size_t hmemorystaticallocator_get_table_bitmap_size(const hmemorystaticallocator_item_t * table);

/** \brief 获取静态分配表所需内存大小。通常用于申请分配所需空间
 *
 * \param table const hmemorystaticallocator_item_t* 分配表指针
 * \return size_t 大小(字节)，失败返回0
 *
 */
size_t hmemorystaticallocator_get_table_size(const hmemorystaticallocator_item_t * table);


/** \brief 获取静态分配表中某种对象的起始地址
 *
 * \param table const hmemorystaticallocator_item_t* 分配表指针
 * \param name const char* 名称
 * \return uintptr_t 大小
 *
 */
uintptr_t hmemorystaticallocator_get_object_address(const hmemorystaticallocator_item_t * table,const char *name);

/** \brief 获取静态分配表中某种对象的大小
 *
 * \param table const hmemorystaticallocator_item_t* 分配表指针
 * \param name const char* 名称
 * \return size_t 大小
 *
 */
size_t hmemorystaticallocator_get_object_size(const hmemorystaticallocator_item_t * table,const char *name);

/** \brief 获取静态分配表中某种对象的数量
 *
 * \param table const hmemorystaticallocator_item_t* 分配表指针
 * \param name const char* 名称
 * \return size_t 大小
 *
 */
size_t hmemorystaticallocator_get_object_count(const hmemorystaticallocator_item_t * table,const char *name);

/** \brief 获取静态分配表中某种对象的标志
 *
 * \param table const hmemorystaticallocator_item_t* 分配表指针
 * \param name const char* 名称
 * \return uint32_t 标志
 *
 */
uint32_t hmemorystaticallocator_get_object_flags(const hmemorystaticallocator_item_t * table,const char *name);

/** \brief 分配静态分配表中的对象
 *
 * \param table const hmemorystaticallocator_item_t* 分配表指针
 * \param bitmap const hmemorystaticallocator_bitmap_t* 位图(记录分配情况)
 * \param name const char*  名称
 * \param base_address void * 待分配空间的基址
 * \return void * 分配的地址,失败返回NULL
 *
 */
void * hmemorystaticallocator_alloc_object(const hmemorystaticallocator_item_t * table,const hmemorystaticallocator_bitmap_t *bitmap,const char *name,void *base_address);

/** \brief 释放静态分配表中的对象
 *
 * \param table const hmemorystaticallocator_item_t* 分配表指针
 * \param bitmap const hmemorystaticallocator_bitmap_t* 位图(记录分配情况)
 * \param name const char* 名称
 * \param base_address void* 待分配空间的基址
 * \param address void* 已分配的地址
 *
 */
void  hmemorystaticallocator_free_object(const hmemorystaticallocator_item_t * table,const hmemorystaticallocator_bitmap_t *bitmap,const char *name,void *base_address,void *address);


/** \brief 分配静态分配表中的匿名对象
 *
 * \param table const hmemorystaticallocator_item_t* 分配表指针
 * \param bitmap const hmemorystaticallocator_bitmap_t* 位图(记录分配情况)
 * \param object_size size_t 对象大小(仅当大小相等时进行分配)
 * \param base_address void*  待分配空间的基址
 * \return void* 分配的地址,失败返回NULL
 *
 */
void * hmemorystaticallocator_alloc_anonymous_object(const hmemorystaticallocator_item_t * table,const hmemorystaticallocator_bitmap_t *bitmap,size_t object_size,void *base_address);


/** \brief 释放静态分配表中的匿名对象
 *
 * \param table const hmemorystaticallocator_item_t* 分配表指针
 * \param bitmap const hmemorystaticallocator_bitmap_t* 位图(记录分配情况)
 * \param object_size size_t 对象大小(仅当大小相等时进行分配)
 * \param base_address void* 待分配空间的基址
 * \param address void* 已分配的地址
 *
 */
void hmemorystaticallocator_free_anonymous_object(const hmemorystaticallocator_item_t * table,const hmemorystaticallocator_bitmap_t *bitmap,size_t object_size,void *base_address,void *address);

/*
 * 使用4K大小的内存用作缓存的分配表，可分配1、2、4、8、16、32、64、128、256字节空间，可用于快速分配空间
 * 注意：需要额外的10字节空间用作位图标记已分配的空间
 */
extern const hmemorystaticallocator_item_t * const hmemorystaticallocator_cache_table;

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HMEMORYSTATICALLOCATOR_H__
