/***************************************************************
 * Name:      hstrncmp.c
 * Purpose:   实现hstrncmp接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-11-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hstrncmp.h"
#include "hdefaults.h"
#include "stdlib.h"

#ifdef HSTRNCMP
extern int HSTRNCMP(const char* lhs,const char* rhs,size_t max_count);
#endif // HSTRNCMP

int hstrncmp(const char* lhs,const char* rhs,size_t max_count)
{
#if defined(HSTRNCMP)
    return HSTRNCMP(lhs,rhs,max_count);
#elif HDEFAULTS_LIBC_TINY_STRNCMP > 0
    {
        if(max_count ==0)
        {
            return 0;
        }
        if(lhs==NULL || rhs==NULL )
        {
            /*
             * 未定义行为，此处返回-1
             */
            return -1;
        }
        int ret=(*lhs - *rhs);
        while (*lhs && *lhs == *rhs)
        {
            lhs++;
            rhs++;
            ret=(*lhs - *rhs);
            max_count--;
            if(max_count ==0)
            {
                break;
            }
        }
        return ret;
    }
#else
    return strncmp(lhs,rhs,max_count);
#endif
}


