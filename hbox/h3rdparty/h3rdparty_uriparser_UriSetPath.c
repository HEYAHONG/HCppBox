/***************************************************************
 * Name:      h3rdparty_uriparser.c
 * Purpose:   引入第三方源代码文件
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-12-20
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#define H3RDPARTY_URIPARSER_IMPLEMENTATION  1

#include "hdefaults.h"
#include "h3rdparty.h"

#ifdef printf
#undef printf
#endif
#define printf hprintf

#include "h3rdparty/patch/heap.c"

#include "h3rdparty/patch/armcc_diag_suppress.c"




#ifndef  H3RDPARTY_USING_SYSTEM_URIPARSER

/* 
 * 默认情况下不导出第三方库符号
 */
#ifndef URI_STATIC_BUILD
#define URI_STATIC_BUILD  1
#endif

#include "3rdparty/uriparser/UriSetPath.c"

#endif // H3RDPARTY_USING_SYSTEM_URIPARSER


