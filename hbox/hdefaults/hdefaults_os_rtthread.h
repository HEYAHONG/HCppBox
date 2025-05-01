/***************************************************************
 * Name:      hdefaults_os_rtthread.h
 * Purpose:   定义一些默认宏定义
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-01-10
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HDEFAULTS_OS_RTTHREAD_H__
#define __HDEFAULTS_OS_RTTHREAD_H__
#include "hdefaults_common.h"
#include "rtthread.h"

#ifdef RT_USING_CPLUSPLUS

/*
 * RT-Thread会重载new与delete
 */
#ifndef HCPPRT_NO_NEW_AND_DELETE_OVERRIDE
#define HCPPRT_NO_NEW_AND_DELETE_OVERRIDE 1
#endif // HCPPRT_NO_NEW_AND_DELETE_OVERRIDE

#endif // RT_USING_CPLUSPLUS


/*
 * 系统调用号
 */
#include "syscall/hdefaults_os_rtthread_syscall.h"


#endif
