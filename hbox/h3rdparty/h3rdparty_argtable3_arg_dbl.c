/***************************************************************
 * Name:      h3rdparty_argtable3.c
 * Purpose:   引入第三方源代码文件
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-06-21
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

#if !defined(HDEFAULTS_LIBC_MSVC)

//定义打印函数
#define vsnprintf hvsnprintf

#endif

//定义环境变量相关函数
#define getenv hgetenv

//对默认函数进行重命名
#define dbg_printf argtable3_dbg_printf

#define ARG_REPLACE_GETOPT 1

#ifndef  H3RDPARTY_USING_SYSTEM_ARGTABLE3

#include "3rdparty/argtable3/arg_dbl.c"

#endif // H3RDPARTY_USING_SYSTEM_ARGTABLE3


