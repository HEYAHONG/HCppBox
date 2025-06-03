/***************************************************************
 * Name:      h3rdparty_zlib.c
 * Purpose:   引入第三方源代码文件
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-06-03
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"
#include "h3rdparty.h"

//定义内存分配函数
#define malloc hmalloc
#define free   hfree
#define calloc hcalloc

#if defined(HDEFAULTS_LIBC_NEWLIB) || defined(HDEFAULTS_LIBC_PICOLIBC) || defined(HDEFAULTS_OS_UNIX) ||  defined(HDEFAULTS_OS_RTTHREAD) || defined(HDEFAULTS_OS_NUTTX)
#include "unistd.h"
#endif 

#if defined(HDEFAULTS_OS_NONE)
#ifndef Z_SOLO
#define Z_SOLO 1
#endif
#endif

#include "stdarg.h"

#ifndef  H3RDPARTY_USING_SYSTEM_ZLIB

#include "3rdparty/zlib/uncompr.c"

#endif // H3RDPARTY_USING_SYSTEM_ZLIB


