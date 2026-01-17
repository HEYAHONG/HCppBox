/***************************************************************
 * Name:      hstrlen.c
 * Purpose:   实现hstrlen接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-11-18
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hstrlen.h"
#include "hdefaults.h"
#include "stdlib.h"

#ifdef HSTRLEN
extern size_t HSTRLEN(const char* str);
#endif // HSTRLEN

size_t hstrlen(const char* str)
{
#if defined(HSTRLEN)
    return HSTRLEN(str);
#elif HDEFAULTS_LIBC_TINY_STRLEN > 0
    {
        size_t ret=0;
        if(str!=NULL)
        {
            while((*(str++))!='\0')
            {
                ret++;
            }
        }
        return ret;
    }
#else
    return strlen(str);
#endif
}


