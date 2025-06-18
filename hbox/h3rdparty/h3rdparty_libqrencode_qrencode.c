/***************************************************************
 * Name:      h3rdparty_libqrencode.c
 * Purpose:   引入第三方源代码文件
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-06-19
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"
#include "h3rdparty.h"

//定义内存分配函数
#define malloc hmalloc
#define free   hfree
#define calloc hcalloc
#define realloc hrealloc

#include "stdarg.h"

#if HAVE_CONFIG_H
# include "config.h"
#endif

#ifndef STATIC_IN_RELEASE
#define STATIC_IN_RELEASE static
#endif

#ifndef MAJOR_VERSION
#define MAJOR_VERSION 4
#endif

#ifndef MINOR_VERSION
#define MINOR_VERSION 1
#endif

#ifndef MICRO_VERSION
#define MICRO_VERSION 1
#endif

#ifndef VERSION
#define VERSION "4.1.1"
#endif

#ifndef  H3RDPARTY_USING_SYSTEM_LIBQRENCODE

#include "3rdparty/libqrencode/qrencode.c"

#endif // H3RDPARTY_USING_SYSTEM_LIBQRENCODE


