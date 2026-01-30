/***************************************************************
 * Name:      hsyscall_time_common.h
 * Purpose:   声明hsyscall_time_common接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-01-20
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HHSYSCALL_TIME_COMMON_H__
#define __HHSYSCALL_TIME_COMMON_H__
#include "stdint.h"
#include "time.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

struct htimezone;
typedef struct htimezone htimezone_t;
struct htimezone
{
    int tz_minuteswest; /* Minutes west of GMT.  */
    int tz_dsttime;     /* Nonzero if DST is ever in effect.  */
};

struct htimeval;
typedef struct htimeval htimeval_t;
struct htimeval
{
    uint64_t tv_sec;    /* Seconds.  */
    uint64_t tv_usec;   /* Microseconds.  */
};

static inline int htimeval_compare(htimeval_t ltv,htimeval_t rtv)
{
    if((ltv.tv_sec==rtv.tv_sec) && (ltv.tv_usec==rtv.tv_usec))
    {
        return 0;
    }
    if((ltv.tv_sec > rtv.tv_sec) || ((ltv.tv_sec==rtv.tv_sec) && (ltv.tv_usec > rtv.tv_usec)))
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

struct timespec;
typedef struct htimespec htimespec_t;
struct htimespec
{
    uint64_t tv_sec;    /* Seconds.  */
    uint64_t tv_nsec;   /* Nanoseconds [0, 999999999]  */
};

static inline htimespec_t htimeval_convert_timespec(htimeval_t tv)
{
    htimespec_t ret;
    ret.tv_sec=tv.tv_sec;
    ret.tv_nsec=tv.tv_usec*1000;
    return ret;
}

static inline htimeval_t htimespec_convert_timeval(htimespec_t tv)
{
    htimeval_t ret;
    ret.tv_sec=tv.tv_sec;
    ret.tv_usec=tv.tv_nsec/1000;
    return ret;
}

static inline int htimespec_compare(htimespec_t ltv,htimespec_t rtv)
{
    if((ltv.tv_sec==rtv.tv_sec) && (ltv.tv_nsec==rtv.tv_nsec))
    {
        return 0;
    }
    if((ltv.tv_sec > rtv.tv_sec) || ((ltv.tv_sec==rtv.tv_sec) && (ltv.tv_nsec > rtv.tv_nsec)))
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

typedef size_t hclockid_t;

/*
 * 实时时钟
 */
#if defined(CLOCK_REALTIME)
#define HCLOCK_REALTIME CLOCK_REALTIME
#else
#define HCLOCK_REALTIME 0
#endif

/*
 * 单调时钟
 */
#if defined(CLOCK_MONOTONIC)
#define HCLOCK_MONOTONIC CLOCK_MONOTONIC
#else
#define HCLOCK_MONOTONIC (HCLOCK_REALTIME+1)
#endif



#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HHSYSCALL_TIME_COMMON_H__
