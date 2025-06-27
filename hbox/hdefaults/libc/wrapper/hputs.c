/***************************************************************
 * Name:      hputs.c
 * Purpose:   实现hputs接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-06-21
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hputs.h"
#include "hdefaults.h"
#include "stdlib.h"

#ifdef HPUTS
extern int HPUTS(const char* str);
#endif // HPUTS

int hputs( const char* str)
{
#if defined(HPUTS)
    return HPUTS(str);
#elif (HDEFAULTS_LIBC_OPTIMIZE_LEVEL) > 0
    if(str!=NULL)
    {
        while((*str)!='\0')
        {
            if(hputchar(*(uint8_t *)str) < 0)
            {
                return -1;
            }
            str++;
        }
    }
    return 0;
#else
    return puts(str);
#endif
}


