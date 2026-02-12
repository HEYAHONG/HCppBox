/***************************************************************
 * Name:      hferror.c
 * Purpose:   实现hferror接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-06-22
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hferror.h"
#include "hdefaults.h"
#include "stdlib.h"

#ifdef HFERROR
extern int HFERROR(FILE *stream );
#endif // HFERROR

int hferror( FILE *stream )
{
#if defined(HFERROR)
    return HFERROR(stream);
#elif ((HDEFAULTS_LIBC_OPTIMIZE_LEVEL) > 0) && !defined(HDEFAULTS_LIBC_TINY) && !defined(HLIBC_NO_STDIO)
    return hlibc_ferror((hfile_t *)stream);
#else
    return ferror(stream);
#endif
}


