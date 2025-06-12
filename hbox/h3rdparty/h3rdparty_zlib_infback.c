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

//文件描述符相关函数
#define close hclose

#include "stdarg.h"

#ifndef  H3RDPARTY_USING_SYSTEM_ZLIB

#include "3rdparty/zlib/infback.c"

#endif // H3RDPARTY_USING_SYSTEM_ZLIB


