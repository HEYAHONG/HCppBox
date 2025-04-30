/***************************************************************
 * Name:      hgettimeofday.h
 * Purpose:   声明hgettimeofday接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-04-30
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HGETTIMEOFDAY_H__
#define __HGETTIMEOFDAY_H__
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

typedef struct hgettimeofday_timezone hgettimeofday_timezone_t;
struct hgettimeofday_timezone
{
    int tz_minuteswest;		/* Minutes west of GMT.  */
    int tz_dsttime;		/* Nonzero if DST is ever in effect.  */
};

typedef struct hgettimeofday_timeval hgettimeofday_timeval_t;
struct hgettimeofday_timeval
{
    uint64_t tv_sec;	/* Seconds.  */
    uint64_t tv_usec;	/* Microseconds.  */
};


/** \brief 获取当前时间
 *
 * \param tv hgettimeofday_timeval_t * 当前时间,可为NULL
 * \param tz hgettimeofday_timezone_t* 当前时区,可为NULL
 * \return int 0=成功，-1=失败
 *
 */
int hgettimeofday (hgettimeofday_timeval_t *tv, hgettimeofday_timezone_t * tz);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HGETTIMEOFDAY_H__
