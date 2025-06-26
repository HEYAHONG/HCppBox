/***************************************************************
 * Name:      hdefaults_os_none.h
 * Purpose:   定义一些默认宏定义
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-01-11
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HDEFAULTS_OS_NONE_H__
#define __HDEFAULTS_OS_NONE_H__
#include "hdefaults_common.h"

#ifndef HCPPRT_USE_CTORS
//使用此行对C++构造函数执行情况进行检查,需要调用hcpprt_init函数。
//#define HCPPRT_USE_CTORS 1
#endif // HCPPRT_USE_CTORS

/*
 * 系统调用号
 */
#include "syscall/hdefaults_os_none_syscall.h"


/*
 * 默认优化级别
 */
#ifndef HDEFAULTS_LIBC_OPTIMIZE_LEVEL
#define HDEFAULTS_LIBC_OPTIMIZE_LEVEL     1
#endif // HDEFAULTS_LIBC_OPTIMIZE_LEVEL


#endif
