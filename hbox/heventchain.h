/***************************************************************
 * Name:      heventchain.h
 * Purpose:   声明heventchain接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2023-07-14
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/


#ifndef HEVENTCHAIN_H_INCLUDED
#define HEVENTCHAIN_H_INCLUDED

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

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // HEVENTCHAIN_H_INCLUDED
