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


#if !defined(HDEFAULTS_LIBC_MSVC) && !defined(HDEFAULTS_LIBC_MINGW)

//文件描述符相关函数
#ifdef close
#undef close
#endif
#define close hclose
#ifdef read
#undef read
#endif
#define read hread
#ifdef write
#undef write
#endif
#define write hwrite
#ifdef lseek
#undef lseek
#endif
#define lseek hlseek
#ifdef lseek64
#undef lseek64
#endif
#define lseek64 hlseek64
#ifdef open
#undef open
#endif
#define open hopen

#endif

#include "stdarg.h"

#ifndef  H3RDPARTY_USING_SYSTEM_ZLIB

#include "3rdparty/zlib/gzclose.c"

#endif // H3RDPARTY_USING_SYSTEM_ZLIB


