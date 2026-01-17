/***************************************************************
 * Name:      hstrcmp.c
 * Purpose:   实现hstrcmp接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hstrcmp.h"
#include "hdefaults.h"
#include "stdlib.h"

#ifdef HSTRCMP
extern int HSTRCMP(const char* lhs,const char* rhs);
#endif // HSTRCMP

int hstrcmp(const char* lhs,const char* rhs)
{
#if defined(HSTRCMP)
    return HSTRCMP(lhs,rhs);
#elif HDEFAULTS_LIBC_TINY_STRCMP > 0
    {
        if(lhs==NULL || rhs==NULL )
        {
            /*
             * 未定义行为，此处返回-1
             */
            return -1;
        }
        while (*lhs && *lhs == *rhs)
        {
            lhs++;
            rhs++;
        }
        return (*lhs - *rhs);
    }
#else
    return strcmp(lhs,rhs);
#endif
}


