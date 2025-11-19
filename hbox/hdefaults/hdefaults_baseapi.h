/***************************************************************
 * Name:      hdefaults_baseapi.h
 * Purpose:   定义一些默认宏定义
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-11-19
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HDEFAULTS_BASEAPI_H__
#define __HDEFAULTS_BASEAPI_H__
#include "stdint.h"
#include "stdlib.h"

#ifdef __RTTHREAD__
#include "rtthread.h"
#ifndef hdefaults_tick_t
#define hdefaults_tick_t rt_tick_t
#endif // hdefaults_tick_t
#endif // __RTTHREAD__
#ifndef hdefaults_tick_t
#define hdefaults_tick_t uint32_t
#endif // hdefaults_tick_t

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/** \brief 获取当前节拍
 *
 * \return hdefaults_tick_t 当前节拍(毫秒),注意，并非启动时间
 *
 */
hdefaults_tick_t hdefaults_tick_get(void);

/** \brief 默认内存分配
 *
 * \param nBytes size_t 待分配的字节数
 * \param usr void* 用户指针(一般不用)
 * \return void* 分配的指针
 *
 */
void * hdefaults_malloc(size_t nBytes,void *usr);

/** \brief 默认内存释放
 *
 * \param ptr void* 待释放的指针
 * \param usr void* 用户指针(一般不用)
 *
 */
void hdefaults_free(void *ptr,void *usr);

/** \brief 互斥锁加锁(默认是通过进出临界区实现,而非直接创建互斥锁)
 *
 * \param usr void*用户指针(一般不用)
 *
 */
void  hdefaults_mutex_lock(void *usr);

/** \brief 互斥锁解锁(默认是通过进出临界区实现,而非直接创建互斥锁)
 *
 * \param usr void*用户指针(一般不用)
 *
 */
void  hdefaults_mutex_unlock(void *usr);


/** \brief 通过符号名称获取符号地址
 *
 * \param symbol_name const char* 符号名称
 * \return void * 符号地址，为NULL表示失败
 *
 */
void * hdefaults_symbol_find(const char * symbol_name);


/*
 *  hdefaults默认API表
 */
typedef struct hdefaults_api_table
{
    hdefaults_tick_t    (*tick_get)(void);
    void *              (*mem_alloc)(size_t,void *);
    void                (*mem_free)(void *,void *);
    void                (*mutex_lock)(void *);
    void                (*mutex_unlock)(void *);
    intptr_t            (*usercall)(uintptr_t,...);
    void *              (*symbol_find)(const char * );
} hdefaults_api_table_t;

/** \brief 获取API表
 *
 * \return const hdefaults_api_table_t* 当前API表地址
 *
 */
const hdefaults_api_table_t * hdefaults_get_api_table(void);

/** \brief 设置API表
 *  注意：在运行过程中改变API表是危险操作
 *
 * \param new_api_table const hdefaults_api_table_t* 新API表地址
 * \return const hdefaults_api_table_t* 原API表地址
 *
 */
const hdefaults_api_table_t * hdefaults_set_api_table(const hdefaults_api_table_t* new_api_table);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif
