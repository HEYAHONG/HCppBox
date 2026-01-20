/***************************************************************
 * Name:      hdefaults_libc_common.h
 * Purpose:   定义一些默认宏定义
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-01-18
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HDEFAULTS_LIBC_COMMON_H__
#define __HDEFAULTS_LIBC_COMMON_H__
#include "stdlib.h"

#ifndef HDEFAULTS_LIBC_COMMON_AUTO_INCLUDE

#if defined(HDEFAULTS_OS_UNIX) && !defined(__UCLIBC__)
#define HDEFAULTS_LIBC_COMMON_AUTO_INCLUDE 1
#endif

#endif // HDEFAULTS_LIBC_COMMON_AUTO_INCLUDE

#if defined(__STDC_VERSION__) && ((__STDC_VERSION__) >= 201112L)

#if !defined(__STDC_NO_THREADS__)
#if defined(HDEFAULTS_LIBC_COMMON_AUTO_INCLUDE) || defined(HAVE_THREADS_H)
#include "threads.h"
#define  HDEFAULTS_LIBC_HAVE_THREADS  1
#endif
#endif

#if !defined(__STDC_NO_ATOMICS__)

#if defined(HDEFAULTS_LIBC_COMMON_AUTO_INCLUDE) || defined(HAVE_STDATOMIC_H)
#include "stdatomic.h"
#define  HDEFAULTS_LIBC_HAVE_STDATOMIC  1
#endif

#endif

#endif

#endif
