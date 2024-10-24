/***************************************************************
 * Name:      hcompiler.c
 * Purpose:   实现编译器特性相关函数
 * Author:    HYH (hyhsystem.cn)
 * Created:   2023-07-14
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#include "hcompiler.h"
#include "stdlib.h"
#include "stdio.h"

long hcompiler_get_stdc_version()
{
#ifdef __STDC_VERSION__
    return __STDC_VERSION__;
#else
    return 199409L;
#endif // __STDC_VERSION__
}

