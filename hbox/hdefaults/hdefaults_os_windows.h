/***************************************************************
 * Name:      hdefaults_os_windows.h
 * Purpose:   定义一些默认宏定义
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-01-10
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HDEFAULTS_OS_WINDOWS_H__
#define __HDEFAULTS_OS_WINDOWS_H__
#include "hdefaults_common.h"
#include "winsock2.h"
#include "windows.h"

/*
 * 定义默认的堆大小，在使用hmemoryheap默认函数分配内存时生效
 */
#ifndef HMEMORYHEAP_DEFAULT_POOL_SIZE
#define HMEMORYHEAP_DEFAULT_POOL_SIZE 1*1024*1024
#endif // HMEMORYHEAP_DEFAULT_POOL_SIZE

/*
 * 系统调用号
 */

#include "syscall/hdefaults_os_windows_syscall.h"

#endif
