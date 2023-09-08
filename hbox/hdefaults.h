/***************************************************************
 * Name:      hdefaults.h
 * Purpose:   定义一些默认宏定义及函数
 * Author:    HYH (hyhsystem.cn)
 * Created:   2023-07-14
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#ifndef HDEFAULTS_H_INCLUDED
#define HDEFAULTS_H_INCLUDED

/*
本文件主要定义一些默认相关宏定义及函数。
*/

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "stdlib.h"
#include "stdint.h"
#include "string.h"
#include "stdbool.h"

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

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // HDEFAULTS_H_INCLUDED
