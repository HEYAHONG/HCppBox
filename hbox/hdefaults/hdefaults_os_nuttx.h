/***************************************************************
 * Name:      hdefaults_os_nuttx.h
 * Purpose:   定义一些默认宏定义
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-01-16
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HDEFAULTS_OS_NUTTX_H__
#define __HDEFAULTS_OS_NUTTX_H__
#include "hdefaults_common.h"


#if !defined(HAVE_UNISTD_H)
#define HAVE_UNISTD_H  1
#include "unistd.h"
#include "pthread.h"
#endif

#if !defined(HAVE_FCNTL_H)
#define HAVE_FCNTL_H   1
#define HAVE_NO_OPENAT 1
#endif

/*
 * NuttX采用pthread管理线程
 */
#if !defined(HTHRD_USING_PTHREAD)
#define HTHRD_USING_PTHREAD 1
#endif

/*
 * NuttX采用pthread管理锁
 */
#if !defined(HMTX_USING_PTHREAD)
#define HMTX_USING_PTHREAD 1
#endif


/*
 * NuttX支持具有完善的C++支持
 */
#ifndef HCPPRT_NO_NEW_AND_DELETE_OVERRIDE
#define HCPPRT_NO_NEW_AND_DELETE_OVERRIDE 1
#endif // HCPPRT_NO_NEW_AND_DELETE_OVERRIDE


/*
 * Nuttx Apps支持argtable3,需要通过配置启用
 */
#if defined(CONFIG_SYSTEM_ARGTABLE3)
/*
 * 使用系统argtable3
 */
#if !defined(H3RDPARTY_USING_SYSTEM_ARGTABLE3)
#define H3RDPARTY_USING_SYSTEM_ARGTABLE3 1
#endif
#endif



/*
 * 系统调用号
 */
#include "syscall/hdefaults_os_nuttx_syscall.h"


#endif
