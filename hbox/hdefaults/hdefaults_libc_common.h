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
#if defined(__STDC_VERSION__) && ((__STDC_VERSION__) >= 201112L)

#if !defined(__STDC_NO_THREADS__)
#include "threads.h"
#define  HDEFAULTS_LIBC_HAVE_THREAD  1
#endif

#if !defined(__STDC_NO_ATOMICS__)
#include "stdatomic.h"
#define  HDEFAULTS_LIBC_HAVE_ATOMICS  1
#endif

#endif

#endif
