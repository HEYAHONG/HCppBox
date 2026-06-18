/***************************************************************
 * Name:      hdefaults_os_zephyr.h
 * Purpose:   定义一些默认宏定义
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-06-18
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HDEFAULTS_OS_ZEPHYR_H__
#define __HDEFAULTS_OS_ZEPHYR_H__
#include "hdefaults_common.h"
#include "zephyr/kernel.h"


/*
 * Zephyr具有C++支持
 */
#ifndef HCPPRT_NO_NEW_AND_DELETE_OVERRIDE
#define HCPPRT_NO_NEW_AND_DELETE_OVERRIDE 1
#endif // HCPPRT_NO_NEW_AND_DELETE_OVERRIDE



/*
 * 系统调用号
 */
#include "syscall/hdefaults_os_zephyr_syscall.h"


#endif
