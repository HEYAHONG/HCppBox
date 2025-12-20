/***************************************************************
 * Name:      h3rdparty_uriparser.c
 * Purpose:   引入第三方源代码文件
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-12-20
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"
#include "h3rdparty.h"

#ifdef printf
#undef printf
#endif
#define printf hprintf

//定义内存分配函数
#define malloc hmalloc
#define free   hfree
#define calloc hcalloc
#define realloc hrealloc


/*
 * 修复armcc下的警告
 */
#ifdef __ARMCC_VERSION
#ifndef __clang__
#pragma diag_suppress 68
#endif
#endif // __ARMCC_VERSION



#ifndef  H3RDPARTY_USING_SYSTEM_URIPARSER

/* 
 * 默认情况下不导出第三方库符号
 */
#ifndef URI_STATIC_BUILD
#define URI_STATIC_BUILD  1
#endif

#include "3rdparty/uriparser/UriSetHostAuto.c"

#endif // H3RDPARTY_USING_SYSTEM_URIPARSER


