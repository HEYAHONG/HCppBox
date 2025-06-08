/***************************************************************
 * Name:      hdefaults_os_unix.h
 * Purpose:   定义一些默认宏定义
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-01-10
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HDEFAULTS_OS_UNIX_H__
#define __HDEFAULTS_OS_UNIX_H__
#include "hdefaults_common.h"
//在unix上常使用pthread库
#include "pthread.h"
//unix标准头文件
#include "unistd.h"

/*
 * 定义默认的堆大小，在使用hmemoryheap默认函数分配内存时生效
 */
#ifndef HMEMORYHEAP_DEFAULT_POOL_SIZE
#define HMEMORYHEAP_DEFAULT_POOL_SIZE 100*1024
#endif // HMEMORYHEAP_DEFAULT_POOL_SIZE

#endif
