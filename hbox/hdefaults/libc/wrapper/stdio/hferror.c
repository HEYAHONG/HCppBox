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
#else
    return ferror(stream);
#endif
}


