/***************************************************************
 * Name:      hdefaults_libc_msvc.h
 * Purpose:   定义一些默认宏定义
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-01-14
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HDEFAULTS_LIBC_MSVC_H__
#define __HDEFAULTS_LIBC_MSVC_H__
#include "stdlib.h"
#include "stdint.h"

/*
 * 定义ssize_t
 */
#include <BaseTsd.h>
#ifndef ssize_t
typedef SSIZE_T ssize_t;
#endif


#endif
