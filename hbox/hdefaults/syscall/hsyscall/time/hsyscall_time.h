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

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HHSYSCALL_TIME_H__
