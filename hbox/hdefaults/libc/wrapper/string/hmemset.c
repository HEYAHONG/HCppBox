/***************************************************************
 * Name:      hmemset.c
 * Purpose:   实现hmemset接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-11-18
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hmemset.h"
#include "hdefaults.h"
#include "stdlib.h"

#ifdef HMEMSET
extern void *HMEMSET(void* dest, int ch, size_t dest_count);
#endif // HMEMSET

void* hmemset(void* dest, int ch, size_t dest_count)
{
#if defined(HMEMSET)
    return HMEMSET(dest,ch,dest_count);
#elif HDEFAULTS_LIBC_TINY_MEMSET > 0
    {
        char * buffer=(char *)dest;
        if(buffer!=NULL)
        {
            for(size_t i=0;i<dest_count;i++)
            {
                buffer[i]=ch;
            }
        }
        return dest;
    }
#else
    return memset(dest,ch,dest_count);
#endif
}


