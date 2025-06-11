/***************************************************************
 * Name:      hdefaults_libc_picolibc.h
 * Purpose:   定义一些默认宏定义
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-01-15
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HDEFAULTS_LIBC_PICOLIBC_H__
#define __HDEFAULTS_LIBC_PICOLIBC_H__
#include "stdlib.h"
#include "stdint.h"

#ifndef __RTTHREAD__
#ifndef ssize_t
typedef intptr_t ssize_t;
#endif // ssize_t
#endif // __RTTHREAD__

#endif
