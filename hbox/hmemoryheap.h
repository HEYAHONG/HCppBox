/***************************************************************
 * Name:      hmemoryheap.h
 * Purpose:   声明hmemoryheap接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2023-07-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#ifndef __HMEMORYHEAP_H_INCLUDED__
#define __HMEMORYHEAP_H_INCLUDED__

/*
本文件主要用于实现一个简易的内存堆(最大可支持2^31-HMEMORYHEAP_ALIGNED_SIZE字节)，用于在操作系统没有动态内存分配时提供简易的支持。
    内存堆由内存堆结构体(头部)+内存块池组成。
    内存块池由多个内存块组成。
    内存块由内存块结构体(头部)+实际分配空间构成。
*/

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"

/** \brief 内存对齐字节数大小
 *        必须是2的幂(如2,4,8,16)且为整数，否则将出现异常。
 *
 *
 */
#define HMEMORYHEAP_ALIGNED_SIZE (sizeof(void*))

/** \brief 魔数,为32位数值
 *
 *
 *
 */
#define HMEMORYHEAP_MAGIC_NUMBER (0xcccc55aa)

struct hmemoryheap_pool; /**< hmemoryheap_pool前置声明 */
typedef struct hmemoryheap_pool hmemoryheap_pool_t; /**< hmemoryheap_pool_t类型定义 */



/** \brief 将数组格式化为hmemoryheap_pool_t
 *
 * \param  usr void*用户参数
 * \param  mutex_lock 加锁,第一个参数是用户指针
 * \param  mutex_unlock 解锁,第一个参数是用户指针
 * \param  pool_store uint8_t* 用于存储数据的数组指针
 * \param  pool_store_size 用于存储数据的数组大小
 * \return hmemoryheap_pool_t * hmemoryheap_pool_t指针,失败返回NULL;
 *
 */
hmemoryheap_pool_t *hmemoryheap_pool_format(void *usr,void (*mutex_lock)(void *),void (*mutex_unlock)(void *),uint8_t *pool_store,size_t pool_store_size);

/** \brief 将数组格式化为hmemoryheap_pool_t
 *
 * \param  usr void*用户参数
 * \param  pool_store uint8_t* 用于存储数据的数组指针
 * \param  pool_store_size 用于存储数据的数组大小
 * \return hmemoryheap_pool_t * hmemoryheap_pool_t指针,失败返回NULL;
 *
 */
hmemoryheap_pool_t *hmemoryheap_pool_format_with_default_lock(void *usr,uint8_t *pool_store,size_t pool_store_size);

/** \brief 获取hmemoryheap_pool_t的用户指针(usr)
 *
 * \param pool hmemoryheap_pool_t* hmemoryheap_pool_t指针
 * \return void* usr指针
 *
 */
void * hmemoryheap_pool_get_usr_ptr(hmemoryheap_pool_t *pool);

/** \brief 获取hmemoryheap_pool_t的信息
 *
 * \param pool hmemoryheap_pool_t* hmemoryheap_pool_t指针
 * \param total_size size_t* 总大小
 * \param free_size size_t* 空闲大小
 *
 */
void hmemoryheap_pool_get_info(hmemoryheap_pool_t *pool,size_t *total_size,size_t *free_size);

typedef enum
{
    HMEMORYHEAP_EXCEPTION_OOM=1, /**< 内存用尽 */
    HMEMORYHEAP_EXCEPTION_DOUBLE_FREE,/**< 释放不存在的内存 */
    HMEMORYHEAP_EXCEPTION_POOL_BROKEN /**< 堆已经被破坏,通常是由于用户写入了超过他申请的长度 */
} hmemoryheap_exception_type_t; /**< hmemoryheap_exception_type_t类型定义 */

/** \brief 设置异常处理(一般是报告后重启)
 *
 * \param pool hmemoryheap_pool_t * hmemoryheap_pool_t指针
 * \param excetion 异常处理回调
 *
 */
void  hmemoryheap_pool_set_exception(hmemoryheap_pool_t *pool,void (*onexception)(struct hmemoryheap_pool *pool,hmemoryheap_exception_type_t excetion));


/** \brief 申请内存
 *
 * \param pool hmemoryheap_pool_t* hmemoryheap_pool_t指针
 * \param nbytes size_t 待申请的大小
 * \return void* 已申请的指针
 *
 */
void * hmemoryheap_pool_malloc(hmemoryheap_pool_t *pool,size_t nbytes);

/** \brief 指针是否在对应的堆
 *
 * \param pool hmemoryheap_pool_t* hmemoryheap_pool_t指针
 * \param ptr void* 指针
 * \return bool 是否在堆中
 *
 */
bool hmemoryheap_is_ptr_in_pool(hmemoryheap_pool_t *pool,void *ptr);

/** \brief 释放内存
 *
 * \param pool hmemoryheap_pool_t* hmemoryheap_pool_t指针
 * \param ptr void* 指针
 *
 */
void hmemoryheap_pool_free(hmemoryheap_pool_t *pool,void *ptr);

/*
以下是默认堆相关的API
*/

/** \brief 设置默认堆
 *
 * \param pool hmemoryheap_pool_t* hmemoryheap_pool_t指针
 *
 */
void hmemoryheap_set_defalut_pool(hmemoryheap_pool_t *pool);

/** \brief 获取默认堆
 *
 * \param pool hmemoryheap_pool_t** hmemoryheap_pool_t指针的指针
 *
 */
void hmemoryheap_get_defalut_pool(hmemoryheap_pool_t **pool_ptr);

/** \brief 获取默认堆的信息
 *
 * \param total_size size_t* 总大小
 * \param free_size size_t* 空闲大小
 *
 */
void hmemoryheap_get_info(size_t *total_size,size_t *free_size);

/** \brief 指针是否在默认的堆
 *
 * \param ptr void* 指针
 * \return bool 是否在堆中
 *
 */
bool hmemoryheap_is_ptr_in_default_pool(void *ptr);

/** \brief 申请内存(默认堆中)
 *
 * \param nbytes size_t 待申请的大小
 * \return void*已申请的指针
 *
 */
void *hmemoryheap_malloc(size_t nbytes);

/** \brief 释放内存(默认堆中)
 *
 * \param ptr void* 指针
 *
 */
void hmemoryheap_free(void *ptr);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // HMEMORYHEAP_H_INCLUDED
