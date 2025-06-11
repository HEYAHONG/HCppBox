/***************************************************************
 * Name:      h3rdparty_pb_encode.c
 * Purpose:   引入第三方源代码文件
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-09-30
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "h3rdparty.h"

//定义内存分配函数
#define malloc hmalloc
#define free   hfree
#define calloc hcalloc
#define realloc hrealloc

#ifndef  H3RDPARTY_USING_SYSTEM_NANOPB
#include "3rdparty/nanopb/pb_encode.c"
#endif // H3RDPARTY_USING_SYSTEM_NANOPB


