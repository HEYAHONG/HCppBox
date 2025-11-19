/***************************************************************
 * Name:      hdefaults_os.h
 * Purpose:   定义一些默认宏定义
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-11-19
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HDEFAULTS_OS_H__
#define __HDEFAULTS_OS_H__

//定义操作系统

//RT-Thread
#ifdef __RTTHREAD__
#include "hdefaults_os_rtthread.h"
#ifndef HDEFAULTS_OS_RTTHREAD
#define HDEFAULTS_OS_RTTHREAD 1
#endif // HDEFAULTS_OS_RTTHREAD
#ifndef HDEFAULTS_HAVE_OS
#define HDEFAULTS_HAVE_OS 1
#endif
#endif // __RTTHREAD__

//NuttX
#ifdef __NuttX__
#include "hdefaults_os_nuttx.h"
#ifndef HDEFAULTS_OS_NUTTX
#define HDEFAULTS_OS_NUTTX 1
#endif // HDEFAULTS_OS_NUTTX
#ifndef HDEFAULTS_HAVE_OS
#define HDEFAULTS_HAVE_OS 1
#endif
#endif // __NuttX__

//Windows
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
#ifdef WIN32
#include "hdefaults_os_windows.h"
#ifndef HDEFAULTS_OS_WINDOWS
#define HDEFAULTS_OS_WINDOWS 1
#endif // HDEFAULTS_OS_WINDOWS
#ifndef HDEFAULTS_HAVE_OS
#define HDEFAULTS_HAVE_OS 1
#endif
#endif // WIN32

//(类)unix,注意：Linux也使用此配置。
#ifdef __unix__
#include "hdefaults_os_unix.h"
#ifndef HDEFAULTS_OS_UNIX
#define HDEFAULTS_OS_UNIX 1
#endif // HDEFAULTS_OS_UNIX
#ifndef HDEFAULTS_HAVE_OS
#define HDEFAULTS_HAVE_OS 1
#endif
#endif // __unix__

//linux
#ifdef __linux__
#include "hdefaults_os_linux.h"
#ifndef HDEFAULTS_OS_LINUX
#define HDEFAULTS_OS_LINUX 1
#endif // HDEFAULTS_OS_LINUX
#ifndef HDEFAULTS_HAVE_OS
#define HDEFAULTS_HAVE_OS 1
#endif
#endif

//FreeBSD
#ifdef __FreeBSD__
#include "hdefaults_os_freebsd.h"
#ifndef HDEFAULTS_OS_FREEBSD
#define HDEFAULTS_OS_FREEBSD 1
#endif // HDEFAULTS_OS_FREEBSD
#ifndef HDEFAULTS_HAVE_OS
#define HDEFAULTS_HAVE_OS 1
#endif
#endif

//Android
#ifdef __ANDROID__
#include "hdefaults_os_android.h"
#ifndef HDEFAULTS_OS_ANDROID
#define HDEFAULTS_OS_ANDROID 1
#endif // HDEFAULTS_OS_ANDROID
#ifndef HDEFAULTS_HAVE_OS
#define HDEFAULTS_HAVE_OS 1
#endif
#endif

//Cygwin
#ifdef __CYGWIN__
#include "hdefaults_os_cygwin.h"
#ifndef HDEFAULTS_OS_CYGWIN
#define HDEFAULTS_OS_CYGWIN 1
#endif // HDEFAULTS_OS_CYGWIN
#ifndef HDEFAULTS_HAVE_OS
#define HDEFAULTS_HAVE_OS 1
#endif
#endif // __CYGWIN__

//emscripten
#ifdef __EMSCRIPTEN__
#include "hdefaults_os_emscripten.h"
#ifndef HDEFAULTS_OS_EMSCRIPTEN
#define HDEFAULTS_OS_EMSCRIPTEN 1
#endif // HDEFAULTS_OS_EMSCRIPTEN
#ifndef HDEFAULTS_HAVE_OS
#define HDEFAULTS_HAVE_OS 1
#endif
#endif // __EMSCRIPTEN__


//无操作系统(裸机)
#ifndef HDEFAULTS_HAVE_OS
#include "hdefaults_os_none.h"
#ifndef HDEFAULTS_OS_NONE
#define HDEFAULTS_OS_NONE 1
#endif // HDEFAULTS_OS_NONE
#endif


/*
 * 根据操作系统定义hdefaults_tick_t
 */
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

#endif
