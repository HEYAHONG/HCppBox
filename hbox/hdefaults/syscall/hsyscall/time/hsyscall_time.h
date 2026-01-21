/***************************************************************
 * Name:      hsyscall_time.h
 * Purpose:   声明hsyscall_time接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-01-09
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HHSYSCALL_TIME_H__
#define __HHSYSCALL_TIME_H__
#include "hsyscall_time_common.h"
#include "../../wrapper/hgettimeofday.h"
#include "../../wrapper/hsettimeofday.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/*
 *获取时间
 */
int hsyscall_gettimeofday (hgettimeofday_timeval_t *tv, hgettimeofday_timezone_t * tz);
/*
 * 设置时间
 */
int hsyscall_settimeofday (const hsettimeofday_timeval_t *tv,const  hsettimeofday_timezone_t * tz);

/*
 * 获取时钟精度
 */
int hsyscall_clock_getres(hclockid_t clockid, htimespec_t * res);
/*
 * 获取时钟
 */
int hsyscall_clock_gettime(hclockid_t clockid,htimespec_t *tp);
/*
 * 设置时钟
 */
int hsyscall_clock_settime(hclockid_t clockid, const htimespec_t *tp);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HHSYSCALL_TIME_H__
