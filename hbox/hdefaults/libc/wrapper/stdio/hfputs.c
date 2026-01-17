/***************************************************************
 * Name:      hfputs.c
 * Purpose:   实现hfputs接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-06-21
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hfputs.h"
#include "hdefaults.h"
#include "stdlib.h"

#ifdef HFPUTS
extern int HFPUTS(const char* str, FILE*          stream);
#endif // HFPUTS

int hfputs( const char* str, FILE* stream)
{
#if defined(HFPUTS)
    return HFPUTS(str,stream);
#elif (HDEFAULTS_LIBC_OPTIMIZE_LEVEL) > 0
    if(str!=NULL)
    {
        size_t len=strlen(str);
        return (fwrite(str,1,len,stream)!=len)?-1:0;
    }
    return -1;
#else
    return fputs(str,stream);
#endif
}


