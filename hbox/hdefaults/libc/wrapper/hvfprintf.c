/***************************************************************
 * Name:      hvfprintf.c
 * Purpose:   实现hvfprintf接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-06-21
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hvfprintf.h"
#include "hdefaults.h"
#include "stdlib.h"

#ifdef HVFPRINTF
extern int HVFPRINTF(FILE* stream,const char* format,va_list vlist);
#endif // HVFPRINTF

int hvfprintf(FILE* stream,const char* format,va_list vlist)
{
#if defined(HVFPRINTF)
    return HVFPRINTF(stream,format,vlist);
#else
    return vfprintf(stream,format,vlist);
#endif
}


