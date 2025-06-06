/***************************************************************
 * Name:      h3rdparty_lz4.c
 * Purpose:   引入第三方源代码文件
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-06-06
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"
#include "h3rdparty.h"

//定义内存分配函数
#define malloc hmalloc
#define free   hfree
#define calloc hcalloc


#include "stdarg.h"

#ifndef  H3RDPARTY_USING_SYSTEM_LZ4

#include "3rdparty/lz4/lz4.c"

#endif // H3RDPARTY_USING_SYSTEM_LZ4


