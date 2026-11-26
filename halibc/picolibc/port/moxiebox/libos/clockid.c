/***************************************************************
 * Name:      clockid.c
 * Purpose:   实现moxiebox时钟id转换
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-26
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#include <time.h>

#include "moxiebox_syscall.h"
#include "moxiebox_libos.h"

clockid_t
moxiebox_clockid(clockid_t id)
{
#ifdef CLOCK_REALTIME
    if (id == CLOCK_REALTIME)
        return MOXIEBOX_CLOCK_REALTIME;
#endif
#ifdef CLOCK_MONOTONIC
    if (id == CLOCK_MONOTONIC)
        return MOXIEBOX_CLOCK_MONOTONIC;
#endif
#ifdef CLOCK_PROCESS_CPUTIME_ID
    if (id == CLOCK_PROCESS_CPUTIME_ID)
        return MOXIEBOX_CLOCK_PROCESS_CPUTIME_ID;
#endif
#ifdef CLOCK_THREAD_CPUTIME_ID
    if (id == CLOCK_THREAD_CPUTIME_ID)
        return MOXIEBOX_CLOCK_THREAD_CPUTIME_ID;
#endif
#ifdef CLOCK_MONOTONIC_RAW
    if (id == CLOCK_MONOTONIC_RAW)
        return MOXIEBOX_CLOCK_MONOTONIC_RAW;
#endif
#ifdef CLOCK_REALTIME_COARSE
    if (id == CLOCK_REALTIME_COARSE)
        return MOXIEBOX_CLOCK_REALTIME_COARSE;
#endif
#ifdef CLOCK_MONOTONIC_COARSE
    if (id == CLOCK_MONOTONIC_COARSE)
        return MOXIEBOX_CLOCK_MONOTONIC_COARSE;
#endif
#ifdef CLOCK_BOOTTIME
    if (id == CLOCK_BOOTTIME)
        return MOXIEBOX_CLOCK_BOOTTIME;
#endif
#ifdef CLOCK_REALTIME_ALARM
    if (id == CLOCK_REALTIME_ALARM)
        return MOXIEBOX_CLOCK_REALTIME_ALARM;
#endif
#ifdef CLOCK_BOOTTIME_ALARM
    if (id == CLOCK_BOOTTIME_ALARM)
        return MOXIEBOX_CLOCK_BOOTTIME_ALARM;
#endif
    return id;
}
