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
#include "../hsyscall/time/hsyscall_time_common.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

typedef htimezone_t hgettimeofday_timezone_t;

typedef htimeval_t hgettimeofday_timeval_t;


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
