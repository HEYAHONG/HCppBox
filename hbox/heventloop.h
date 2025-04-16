/***************************************************************
 * Name:      heventloop.h
 * Purpose:   声明heventloop接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2023-07-14
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#ifndef __HEVENTLOOP_H_INCLUDED__
#define __HEVENTLOOP_H_INCLUDED__


/*
本文件用于辅助处理事件循环,封装一些常用功能(如使用单向链表添加事件)
    本文件主要用于将一个事件转移到另一个线程中执行或者创建基于事件驱动的任务。
*/

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"

struct heventloop; /**< heventloop前置声明 */
typedef struct heventloop heventloop_t; /**< heventloop_t类型定义 */

/** \brief 获取使用内部堆的heventloop的最小大小
 *
 * \return size_t 使用内部堆的heventloop的最小大小
 *
 */
size_t heventloop_with_internal_heap_min_size(void);

/** \brief 初始化heventloop_t
 *
 * \param  usr void* 用户指针
 * \param  mutex_lock 加锁,不要使用第一个参数
 * \param  mutex_unlock 解锁,不要使用第一个参数
 * \param  mem 内存（此内存可静态分配，大小不小于heventloop_with_internal_heap_min_size()返回的值）
 * \param  length 内存大小
 * \return heventloop_t* heventloop_t指针
 *
 */
heventloop_t * heventloop_with_internal_heap_init(void *usr,void (*mutex_lock)(void *),void (*mutex_unlock)(void *),void *mem,size_t length);



/** \brief 创建heventloop_t
 *
 * \param  usr void* 用户指针
 * \param  mem_alloc 内存分配函数,第一个参数是待分配的字节数，第二个参数是用户指针
 * \param  mem_free  内存释放函数,第一个参数是已分配的指针，第二个参数是用户指针
 * \param  mutex_lock 加锁,第一个参数是用户指针
 * \param  mutex_unlock 解锁,第一个参数是用户指针
 * \return heventloop_t* heventloop_t指针
 *
 */
heventloop_t * heventloop_new_with_memmang_and_lock(void *usr,void *(*mem_alloc)(size_t,void *),void (*mem_free)(void *,void *),void (*mutex_lock)(void *),void (*mutex_unlock)(void *));

/** \brief 创建heventloop_t
 *
 * \param  usr void* 用户指针
 * \param  mem_alloc 内存分配函数,第一个参数是待分配的字节数，第二个参数是用户指针
 * \param  mem_free  内存释放函数,第一个参数是已分配的指针，第二个参数是用户指针
 * \return heventloop_t* heventloop_t指针
 *
 */
heventloop_t * heventloop_new_with_memmang(void *usr,void *(*mem_alloc)(size_t,void *),void (*mem_free)(void *,void *));

/** \brief 创建heventloop_t
 *
 * \param  usr void* 用户指针
 * \param  mutex_lock 加锁,第一个参数是用户指针
 * \param  mutex_unlock 解锁,第一个参数是用户指针
 * \return heventloop_t* heventloop_t指针
 *
 */
heventloop_t * heventloop_new_with_lock(void *usr,void (*mutex_lock)(void *),void (*mutex_unlock)(void *));

/** \brief 创建heventloop_t
 *
 * \param  usr void* 用户指针
 * \return heventloop_t* heventloop_t指针
 *
 */
heventloop_t * heventloop_new(void *usr);

/** \brief 获取heventloop_t的用户指针(usr)
 *
 * \param loop heventloop_t* heventloop_t指针
 * \return void* usr指针
 *
 */
void * heventloop_get_usr_ptr(heventloop_t *loop);

/** \brief 释放heventloop_t
 *
 * \param loop heventloop_t* heventloop_t指针
 *
 */
void heventloop_free(heventloop_t *loop);

/** \brief 判断heventloop_t中是否有未处理的事件
 *
 * \param loop heventloop_t* heventloop_t指针
 * \return bool 是否有未处理的事件
 *
 */
bool heventloop_has_events(heventloop_t *loop);


/** \brief  处理heventloop_t中的事件。通常在事件处理线程中循环调用
 *
 * \param loop heventloop_t* heventloop_t指针
 *
 */
void heventloop_process_event(heventloop_t *loop);

/** \brief 添加事件到事件循环,一般用于将事件处理通过heventloop_t转移到另外的线程中执行。
 *
 * \param  loop heventloop_t* heventloop_t指针
 * \param  event_usr void* 事件中的用户参数
 * \param  event_process 事件处理,第一个参数为用户参数。
 * \param  event_onfree  事件释放回调,第一个参数为用户参数。通常用于释放用户参数。
 * \return bool 是否添加成功,当内存不足或者到达最大事件数时将添加失败。
 *
 */
bool heventloop_add_event(heventloop_t *loop,void *event_usr,void(*event_process)(void *),void(*event_onfree)(void *));


/** \brief 添加事件到事件循环(扩展事件类型1),一般用于将事件处理通过heventloop_t转移到另外的线程中执行。
 *
 * \param  loop heventloop_t* heventloop_t指针
 * \param  event_usr void* 事件中的用户参数
 * \param  event_process 事件处理,第一个参数为用户参数,第二个参数为heventloop_t指针。
 * \param  event_onfree  事件释放回调,第一个参数为用户参数,第二个参数为heventloop_t指针。通常用于释放用户参数。
 * \return bool 是否添加成功,当内存不足或者到达最大事件数时将添加失败。
 *
 */
bool heventloop_add_event_ex1(heventloop_t *loop,void *event_usr,void(*event_process)(void *,heventloop_t *),void(*event_onfree)(void *,heventloop_t *));

/** \brief 获取待处理的事件数
 *
 * \param loop heventloop_t* heventloop_t指针
 * \return uint32_t 待处理的事件数
 *
 */
uint32_t heventloop_get_events_number(heventloop_t *loop);

/** \brief 获取最大的事件数
 *
 * \param loop heventloop_t* heventloop_t指针
 * \return uint32_t 最大的事件数
 *
 */
uint32_t heventloop_get_max_events_number(heventloop_t *loop);

/** \brief 设置最大的事件数
 *
 * \param loop heventloop_t* heventloop_t指针
 * \param max_event_number uint32_t 最大的事件数
 *
 */
void  heventloop_set_max_events_number(heventloop_t *loop,uint32_t max_event_number);

#ifndef HEVENTLOOP_USER_LOOP_COUNT
#define HEVENTLOOP_USER_LOOP_COUNT 0
#endif // HEVENTLOOP_USER_LOOP_COUNT

enum
{
    HEVENTLOOP_SYSTEM_LOOP_0=0,                                     /**< 循环0 */
#ifdef HEVENTLOOP_NO_SLIM
    HEVENTLOOP_SYSTEM_LOOP_1,                                       /**< 循环1 */
    HEVENTLOOP_SYSTEM_LOOP_2,                                       /**< 循环2 */
    HEVENTLOOP_SYSTEM_LOOP_3,                                       /**< 循环3 */
    HEVENTLOOP_SYSTEM_LOOP_4,                                       /**< 循环4 */
    HEVENTLOOP_SYSTEM_LOOP_5,                                       /**< 循环5 */
    HEVENTLOOP_SYSTEM_LOOP_6,                                       /**< 循环6 */
    HEVENTLOOP_SYSTEM_LOOP_7,                                       /**< 循环7 */
    HEVENTLOOP_SYSTEM_LOOP_8,                                       /**< 循环8 */
    HEVENTLOOP_SYSTEM_LOOP_9,                                       /**< 循环9 */
#endif
    HEVENTLOOP_SYSTEM_LOOP_COUNT,                                   /**< 系统事件循环数量*/
    HEVENTLOOP_USER_LOOP_START=HEVENTLOOP_SYSTEM_LOOP_COUNT,        /**< 用户循环起始 */
    HEVENTLOOP_SYSTEM_LOOP_WORKQUEUE=HEVENTLOOP_SYSTEM_LOOP_0,      /**< 工作队列，默认使用循环0，默认在hcpprt组件中使用 */
};

/** \brief 从表中获取事件循环
 *
 * \param id int 标识，见HEVENTLOOP_SYSTEM_LOOP_*或者用户自定义（使用HEVENTLOOP_USER_LOOP_START+偏移）
 * \return heventloop_t* heventloop_t指针,若未设置，将返回NULL
 *
 */
heventloop_t *heventloop_get_loop_from_table(int id);


/** \brief 将事件循环设置到表中
 *
 * \param id int 标识，见HEVENTLOOP_SYSTEM_LOOP_*或者用户自定义（使用HEVENTLOOP_USER_LOOP_START+偏移）
 * \param loop heventloop_t* heventloop_t指针，若为NULL，将使用heventloop_new创建新heventloop_t
 *
 */
void heventloop_set_loop_to_table(int id,heventloop_t *loop);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // HEVENTLOOP_H_INCLUDED
