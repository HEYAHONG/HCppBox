/***************************************************************
 * Name:      hclock.c
 * Purpose:   实现hclock接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-07-22
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hclock.h"
#include "hdefaults.h"
#include "stdlib.h"

#ifdef HCLOCK
extern clock_t HCLOCK(void);
#endif // HCLOCK

clock_t hclock(void)
{
#if defined(HCLOCK)
    return HCLOCK();
#elif HDEFAULTS_LIBC_OPTIMIZE_LEVEL > 0
    {
        hgettimeofday_timeval_t tv= {0};
        hgettimeofday(&tv,NULL);
        return tv.tv_sec*CLOCKS_PER_SEC+tv.tv_usec/(1000000/CLOCKS_PER_SEC);
    }
#else
    return clock();
#endif
}


