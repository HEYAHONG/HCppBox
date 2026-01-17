/***************************************************************
 * Name:      htime.c
 * Purpose:   实现htime接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-07-22
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "htime.h"
#include "hdefaults.h"
#include "stdlib.h"

#ifdef HTIME
extern time_t HTIME(time_t *arg);
#endif // HTIME

time_t htime( time_t* arg)
{
#if defined(HTIME)
    return HTIME(arg);
#elif HDEFAULTS_LIBC_OPTIMIZE_LEVEL > 0
    {
        hgettimeofday_timeval_t tv= {0};
        hgettimeofday(&tv,NULL);
        if(arg!=NULL)
        {
            (*arg)=tv.tv_sec;
        }
        return tv.tv_sec;
    }
#else
    return time(arg);
#endif
}


