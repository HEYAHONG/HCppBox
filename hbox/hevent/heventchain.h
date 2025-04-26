/***************************************************************
 * Name:      heventchain.h
 * Purpose:   声明heventchain接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2023-07-14
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/


#ifndef __HEVENTCHAIN_H_INCLUDED__
#define __HEVENTCHAIN_H_INCLUDED__

/*
本文件用于实现钩子(Hook)回调函数功能,通常配合heventslots.h中的函数类似
    本文件中钩子函数与一般回调函数有以下不同。
        -链式传递。当成功处理后不再向后传递。调用者也可获知是否成功处理。
        -具有优先级设定，回调函数的顺序在一定程度上可调。

*/

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"

struct heventchain; /**< heventchain前置声明 */
typedef struct heventchain heventchain_t; /**< heventchain_t定义 */

/** \brief 获取使用内部堆的heventchain的最小大小
 *
 * \return size_t 使用内部堆的heventchain的最小大小
 *
 */
size_t heventchain_with_internal_heap_min_size(void);

/** \brief 初始化heventchain_t
 *
 * \param  usr void* 用户指针
 * \param  mutex_lock 加锁,不要使用第一个参数
 * \param  mutex_unlock 解锁,不要使用第一个参数
 * \param  mem 内存（此内存可静态分配，大小不小于heventchain_with_internal_heap_min_size()返回的值）
 * \param  length 内存大小
 * \return heventchain_t* heventchain_t指针
 *
 */
heventchain_t * heventchain_with_internal_heap_init(void *usr,void (*mutex_lock)(void *),void (*mutex_unlock)(void *),void *mem,size_t length);

/** \brief 创建heventchain_t
 *
 * \param  usr void* 用户指针
 * \param  mem_alloc 内存分配函数,第一个参数是待分配的字节数，第二个参数是用户指针
 * \param  mem_free  内存释放函数,第一个参数是已分配的指针，第二个参数是用户指针
 * \param  mutex_lock 加锁,第一个参数是用户指针
 * \param  mutex_unlock 解锁,第一个参数是用户指针
 * \return heventchain_t* heventchain_t指针
 *
 */
heventchain_t * heventchain_new_with_memmang_and_lock(void *usr,void *(*mem_alloc)(size_t,void *),void (*mem_free)(void *,void *),void (*mutex_lock)(void *),void (*mutex_unlock)(void *));

/** \brief 创建heventchain_t
 *
 * \param  usr void* 用户指针
 * \param  mem_alloc 内存分配函数,第一个参数是待分配的字节数，第二个参数是用户指针
 * \param  mem_free  内存释放函数,第一个参数是已分配的指针，第二个参数是用户指针
 * \return heventchain_t* heventchain_t指针
 *
 */
heventchain_t * heventchain_new_with_memmang(void *usr,void *(*mem_alloc)(size_t,void *),void (*mem_free)(void *,void *));

/** \brief 创建heventchain_t
 *
 * \param  usr void* 用户指针
 * \param  mutex_lock 加锁,第一个参数是用户指针
 * \param  mutex_unlock 解锁,第一个参数是用户指针
 * \return heventchain_t* heventchain_t指针
 *
 */
heventchain_t * heventchain_new_with_lock(void *usr,void (*mutex_lock)(void *),void (*mutex_unlock)(void *));

/** \brief 创建heventchain_t
 *
 * \param  usr void* 用户指针
 * \return heventchain_t* heventchain_t指针
 *
 */
heventchain_t * heventchain_new(void *usr);

/** \brief 获取heventchain_t的用户指针(usr)
 *
 * \param chain heventchain_t* heventchain_t指针
 * \return void* usr指针
 *
 */

void * heventchain_get_usr_ptr(heventchain_t *chain);

/** \brief 释放heventchain_t
 *
 * \param chain heventchain_t* heventchain_t指针
 *
 */
void heventchain_free(heventchain_t *chain);


/** \brief 启动事件链（依次调用钩子）
 *
 * \param chain heventchain_t* heventchain_t指针
 * \param parameter void* 传入参数
 * \return bool 是否成功处理。true为已成功处理
 *
 */
bool heventchain_start(heventchain_t *chain,void *parameter);

/** \brief 安装钩子
 *
 * \param chain heventchain_t* heventchain_t指针
 * \param priority uint32_t 优先级,数值越小优先级越高,同一优先级后安装的优先级高。
 * \param hook_usr void*钩子中的用户参数
 * \param hook 钩子函数,第一个参数为传入参数(heventchain_start),第二个参数为用户参数,返回值为是否成功处理。
 * \param onfree 槽释放回调,第一个参数为用户参数。
 * \return uint32_t 注册id,为0表示失败
 *
 */

uint32_t heventchain_install_hook(heventchain_t *chain,uint32_t priority,void *hook_usr,bool (*hook)(void *,void *),void (*onfree)(void *));

/** \brief 卸载钩子
 *
 * \param chain heventchain_t* heventchain_t指针
 * \param id uint32_t 注册id,由安装时返回
 *
 */
void heventchain_uninstall_hook(heventchain_t *chain,uint32_t id);


/** \brief 卸载所有钩子
 *
 * \param chain heventchain_t* heventchain_t指针
 *
 */
void heventchain_uninstall_all_hook(heventchain_t *chain);

#ifndef HEVENTCHAIN_USER_CHAIN_COUNT
#define HEVENTCHAIN_USER_CHAIN_COUNT 0
#endif // HEVENTCHAIN_USER_CHAIN_COUNT

enum
{
    HEVENTCHAIN_SYSTEM_CHAIN_0=0,                                   /**< 链0 */
#ifdef  HEVENTCHAIN_NO_SLIM
    HEVENTCHAIN_SYSTEM_CHAIN_1,                                     /**< 链1 */
    HEVENTCHAIN_SYSTEM_CHAIN_2,                                     /**< 链2 */
    HEVENTCHAIN_SYSTEM_CHAIN_3,                                     /**< 链3 */
    HEVENTCHAIN_SYSTEM_CHAIN_4,                                     /**< 链4 */
    HEVENTCHAIN_SYSTEM_CHAIN_5,                                     /**< 链5 */
    HEVENTCHAIN_SYSTEM_CHAIN_6,                                     /**< 链6 */
    HEVENTCHAIN_SYSTEM_CHAIN_7,                                     /**< 链7 */
    HEVENTCHAIN_SYSTEM_CHAIN_8,                                     /**< 链8 */
    HEVENTCHAIN_SYSTEM_CHAIN_9,                                     /**< 链9 */
#endif
    HEVENTCHAIN_SYSTEM_CHAIN_COUNT,                                 /**< 系统事件链数量*/
    HEVENTCHAIN_USER_CHAIN_START=HEVENTCHAIN_SYSTEM_CHAIN_COUNT     /**< 用户链起始 */
};

/** \brief 从表中获取事件链
 *
 * \param id int 标识，见HEVENTCHAIN_SYSTEM_CHAIN_*或者用户自定义（使用HEVENTCHAIN_USER_CHAIN_START+偏移）
 * \return heventchain_t* heventchain_t指针,若未设置，将返回NULL
 *
 */
heventchain_t *heventchain_get_chain_from_table(int id);


/** \brief 将事件链设置到表中
 *
 * \param id int 标识，见HEVENTCHAIN_SYSTEM_CHAIN_*或者用户自定义（使用HEVENTCHAIN_USER_CHAIN_START+偏移）
 * \param chain heventchain_t* heventchain_t指针，若为NULL，将使用heventchain_new创建新heventchain_t
 *
 */
void heventchain_set_chain_to_table(int id,heventchain_t *chain);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // HEVENTCHAIN_H_INCLUDED
