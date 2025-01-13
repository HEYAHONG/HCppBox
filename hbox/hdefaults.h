/***************************************************************
 * Name:      hdefaults.h
 * Purpose:   定义一些默认宏定义及函数
 * Author:    HYH (hyhsystem.cn)
 * Created:   2023-07-14
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#ifndef __HDEFAULTS_H_INCLUDED__
#define __HDEFAULTS_H_INCLUDED__

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


//提高windows下的兼容性
#ifdef _WIN32
#ifndef WIN32
#define WIN32 1
#endif // WIN32
#endif // _WIN32
#ifdef __WIN32
#ifndef WIN32
#define WIN32 1
#endif // WIN32
#endif // __WIN32
#ifdef __WIN32__
#ifndef WIN32
#define WIN32 1
#endif // WIN32
#endif // __WIN32__

//定义操作系统
#ifdef __RTTHREAD__
#include "hdefaults/hdefaults_os_rtthread.h"
#ifndef HDEFAULTS_OS_RTTHREAD
#define HDEFAULTS_OS_RTTHREAD 1
#endif // HDEFAULTS_OS_RTTHREAD
#ifndef HDEFAULTS_HAVE_OS
#define HDEFAULTS_HAVE_OS 1
#endif
#endif // __RTTHREAD__
#ifdef WIN32
#include "hdefaults/hdefaults_os_windows.h"
#ifndef HDEFAULTS_OS_WINDOWS
#define HDEFAULTS_OS_WINDOWS 1
#endif // HDEFAULTS_OS_WINDOWS
#ifndef HDEFAULTS_HAVE_OS
#define HDEFAULTS_HAVE_OS 1
#endif
#endif // WIN32
#ifdef __unix__
#include "hdefaults/hdefaults_os_unix.h"
#ifndef HDEFAULTS_OS_UNIX
#define HDEFAULTS_OS_UNIX 1
#endif // HDEFAULTS_OS_UNIX
#ifndef HDEFAULTS_HAVE_OS
#define HDEFAULTS_HAVE_OS 1
#endif
#endif // __unix__
#ifdef __ANDROID__
#include "hdefaults/hdefaults_os_android.h"
#ifndef HDEFAULTS_OS_ANDROID
#define HDEFAULTS_OS_ANDROID 1
#endif // HDEFAULTS_OS_ANDROID
#ifndef HDEFAULTS_HAVE_OS
#define HDEFAULTS_HAVE_OS 1
#endif
#endif
#ifdef __CYGWIN__
#include "hdefaults/hdefaults_os_cygwin.h"
#ifndef HDEFAULTS_OS_CYGWIN
#define HDEFAULTS_OS_CYGWIN 1
#endif // HDEFAULTS_OS_CYGWIN
#ifndef HDEFAULTS_HAVE_OS
#define HDEFAULTS_HAVE_OS 1
#endif
#endif // __CYGWIN__

#ifdef __EMSCRIPTEN__
#include "hdefaults/hdefaults_os_emscripten.h"
#ifndef HDEFAULTS_OS_EMSCRIPTEN
#define HDEFAULTS_OS_EMSCRIPTEN 1
#endif // HDEFAULTS_OS_EMSCRIPTEN
#ifndef HDEFAULTS_HAVE_OS
#define HDEFAULTS_HAVE_OS 1
#endif
#endif // __EMSCRIPTEN__


//无操作系统(裸机)
#ifndef HDEFAULTS_HAVE_OS
#include "hdefaults/hdefaults_os_none.h"
#ifndef HDEFAULTS_OS_NONE
#define HDEFAULTS_OS_NONE 1
#endif // HDEFAULTS_OS_NONE
#endif

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#ifndef hdefaults_xstr
#define hdefaults_xstr(s) hdefaults_str(s)
#endif // hdefaults_xstr


#ifndef hdefaults_str
#define hdefaults_str(s) #s
#endif // hdefaults_str

#ifdef HBOX_CONFIG_HEADER
#include hdefaults_xstr(HBOX_CONFIG_HEADER)
#endif


#ifdef HDEFAULTS_OS_RTTHREAD
#define hdefaults_tick_t rt_tick_t
#endif // HDEFAULTS_OS_RTTHREAD

#ifdef HDEFAULTS_OS_WINDOWS
#define hdefaults_tick_t DWORD
#endif // HDEFAULTS_OS_WINDOWS

#ifdef HDEFAULTS_OS_UNIX
#include <time.h>
#include <sys/time.h>
#endif // HDEFAULTS_OS_UNIX

#ifndef hdefaults_tick_t
#define hdefaults_tick_t uint32_t
#endif // hdefaults_tick_t

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

/*
 *  hdefaults默认API表
 */
typedef struct hdefaults_api_table
{
    hdefaults_tick_t (*tick_get)(void);
    void *(*mem_alloc)(size_t,void *);
    void (*mem_free)(void *,void *);
    void (*mutex_lock)(void *);
    void (*mutex_unlock)(void *);
} hdefaults_api_table_t;

/** \brief 默认API表
 *
 * \return const hdefaults_api_table_t*
 *
 */
const hdefaults_api_table_t * hdefaults_get_api_table(void);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // HDEFAULTS_H_INCLUDED
