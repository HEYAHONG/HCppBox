/***************************************************************
 * Name:      hfeof.c
 * Purpose:   实现hfeof接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-08-26
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hfeof.h"
#include "hdefaults.h"
#include "stdlib.h"

#ifdef HFEOF
extern int HFEOF(FILE *stream );
#endif // HFEOF

int hfeof( FILE *stream )
{
#if defined(HFEOF)
    return HFEOF(stream);
#elif ((HDEFAULTS_LIBC_OPTIMIZE_LEVEL) > 0) && !defined(HDEFAULTS_LIBC_TINY) && !defined(HLIBC_NO_STDIO)
    return hlibc_feof((hfile_t *)stream);
#else
    return feof(stream);
#endif
}


