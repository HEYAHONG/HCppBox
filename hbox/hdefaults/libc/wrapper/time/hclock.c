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
    return ((uint64_t)hdefaults_tick_get())*CLOCKS_PER_SEC/1000;
#else
    return clock();
#endif
}


