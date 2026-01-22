/***************************************************************
 * Name:      hlibc_time.h
 * Purpose:   声明hlibc_time接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-01-22
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HHLIBC_TIME_H__
#define __HHLIBC_TIME_H__
#include "hlibc_time_common.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/*
 * 将时间转换为GMT日历
 */
htm_t * hlibc_gmtime_r(const htime_t *tim_p,htm_t * res);

/*
 * 时间转换为本地日历
 */
htm_t * hlibc_localtime_r(const htime_t *tim_p,htm_t * res);

/*
 * 将本地日历转换为时间，注意:与C运行库的实现相比，本函数不会修正非法值，将返回0
 */
htime_t hlibc_mktime(const htm_t *res);


#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HHLIBC_TIME_H__
