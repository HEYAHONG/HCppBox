/***************************************************************
 * Name:      h3rdparty_libfdt.c
 * Purpose:   引入第三方源代码文件
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-06-06
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hcompiler.h"
#include "hdefaults.h"
#include "h3rdparty.h"


/*
 * 修复armcc下的警告
 */
#ifdef __ARMCC_VERSION
#ifndef __clang__
#pragma diag_suppress 1293
#endif
#endif // __ARMCC_VERSION

#include "3rdparty/libfdt/libfdt_env.h"

#ifdef strnlen
#undef strnlen
#endif
#define strnlen libfdt_strnlen

/*
 * fdt_strnlen: returns the length of a string or max_count - which ever is
 * smallest.
 * Input 1 string: the string whose size is to be determined
 * Input 2 max_count: the maximum value returned by this function
 * Output: length of the string or max_count (the smallest of the two)
 */
__USED
static inline size_t libfdt_strnlen(const char *string, size_t max_count)
{
    const char *p = memchr(string, 0, max_count);
    return p ? p - string : max_count;
}


#ifndef  H3RDPARTY_USING_SYSTEM_LIBFDT

#include "3rdparty/libfdt/fdt_strerror.c"

#endif // H3RDPARTY_USING_SYSTEM_LIBFDT


