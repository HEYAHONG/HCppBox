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
 * 将本地日历转换为时间，注意:与C运行库的实现相比，本函数不会修正非法值且不支持小于0的时间戳，将返回(htime_t)-1。
 */
htime_t hlibc_mktime(const htm_t *res);

/*
 *  获取时间
 */
htime_t hlibc_time(htime_t* arg);

/*
 *  获取时钟
 */
hclock_t hlibc_clock(void);

/*
 *  将日历时间转换为文本字符串(缓冲区至少27字节,(年份占4字节时),且缓冲区应当初始化为0)
 */
char * hlibc_asctime_r (const htm_t *tim_p,char * result);

/*
 *  将时间转换为文本字符串(缓冲区至少27字节,(年份占4字节时),且缓冲区应当初始化为0)
 */
char * hlibc_ctime_r(const time_t * tim_p,char * result);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HHLIBC_TIME_H__
