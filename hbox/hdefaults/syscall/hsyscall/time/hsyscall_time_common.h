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

struct timespec;
typedef struct htimespec htimespec_t;
struct htimespec
{
    uint64_t tv_sec;    /* Seconds.  */
    uint64_t tv_nsec;   /* Nanoseconds [0, 999999999]  */
};

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
#define HCLOCK_MONOTONIC 1
#endif



#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HHSYSCALL_TIME_COMMON_H__
