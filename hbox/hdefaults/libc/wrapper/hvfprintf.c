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

#if (HDEFAULTS_LIBC_OPTIMIZE_LEVEL) > 0
extern int hvfctprintf(void (*out)(char character, void* arg), void* arg, const char* format, va_list va);
static void hvfprintf_out(char ch,void *arg)
{
    FILE *stream=(FILE *)arg;
    hfwrite(&ch,1,sizeof(ch),stream);
}
#endif

int hvfprintf(FILE* stream,const char* format,va_list vlist)
{
#if defined(HVFPRINTF)
    return HVFPRINTF(stream,format,vlist);
#elif (HDEFAULTS_LIBC_OPTIMIZE_LEVEL) > 0
    if(stream!=NULL && format!=NULL)
    {
        return hvfctprintf(hvfprintf_out,stream,format,vlist);
    }
    return -1;
#else
    return vfprintf(stream,format,vlist);
#endif
}


