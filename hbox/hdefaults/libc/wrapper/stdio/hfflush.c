/***************************************************************
 * Name:      hfflush.c
 * Purpose:   实现hfflush接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-08-14
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hfflush.h"
#include "hdefaults.h"
#include "stdlib.h"

#ifdef HFFLUSH
extern int HFFLUSH(FILE * stream );
#endif // HFFLUSH

int hfflush(FILE * stream)
{
#if defined(HFFLUSH)
    return HFFLUSH(stream);
#elif ((HDEFAULTS_LIBC_OPTIMIZE_LEVEL) > 0) && !defined(HDEFAULTS_LIBC_TINY) && !defined(HLIBC_NO_STDIO)
    return 0;
#else
    return fflush(stream);
#endif
}


