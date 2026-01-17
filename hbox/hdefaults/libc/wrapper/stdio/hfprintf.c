/***************************************************************
 * Name:      hfprintf.c
 * Purpose:   实现hfprintf接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-07-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hfprintf.h"
#include "hdefaults.h"
#include "stdlib.h"

int hfprintf(FILE*  stream,const char*  format, ... )
{
    int ret=0;
    va_list va;
    va_start(va,format);
    ret=hvfprintf(stream,format,va);
    va_end(va);
    return ret;
}


