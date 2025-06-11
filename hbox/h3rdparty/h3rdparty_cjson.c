/***************************************************************
 * Name:      h3rdparty_cjson.c
 * Purpose:   引入第三方源代码文件
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-09-27
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

#ifndef  H3RDPARTY_USING_SYSTEM_CJSON
#ifdef __ARMCC_VERSION
#ifdef __GNUC__
#undef __GNUC__
#endif // __GNUC__
#endif // __ARMCC_VERSION
#include "3rdparty/cJSON/cJSON.c"
#endif // H3RDPARTY_USING_SYSTEM_CJSON


