/***************************************************************
 * Name:      hfopen.c
 * Purpose:   实现hfopen接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-02-13
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hfopen.h"
#include "hdefaults.h"
#include "stdlib.h"

#ifdef HFOPEN
extern FILE * HFOPEN(const char* filename, const char* mode );
#endif // HFOPEN

FILE * hfopen(const char* filename, const char* mode)
{
#if defined(HFOPEN)
    return HFOPEN(filename,mode);
#elif ((HDEFAULTS_LIBC_OPTIMIZE_LEVEL) > 0) && !defined(HDEFAULTS_LIBC_TINY) && !defined(HLIBC_NO_STDIO)
    return (FILE *)hlibc_fopen(filename,mode);
#else
    return fopen(filename,mode);
#endif
}


