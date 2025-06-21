/***************************************************************
 * Name:      h3rdparty_libqrencode.c
 * Purpose:   引入第三方源代码文件
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-06-19
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hcompiler.h"
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

#ifndef HAVE_STRDUP
#define HAVE_STRDUP 1
#endif

__USED
static char *qrencode_strdup(const char *s)
{
	size_t len = strlen(s) + 1;
	void *newstring = malloc(len);
	if(newstring == NULL) return NULL;
	return (char *)memcpy(newstring, s, len);
}
#ifdef strdup
#undef strdup
#endif
#define strdup qrencode_strdup

/*
 * 修复armcc下的警告
 */
#ifdef __ARMCC_VERSION
#ifdef __GNUC__
#undef __GNUC__
#endif // __GNUC__
#ifndef __clang__
#pragma diag_suppress 186
#endif
#endif // __ARMCC_VERSION

#ifndef  H3RDPARTY_USING_SYSTEM_LIBQRENCODE

#include "3rdparty/libqrencode/rsecc.c"

#endif // H3RDPARTY_USING_SYSTEM_LIBQRENCODE


