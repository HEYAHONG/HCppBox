/***************************************************************
 * Name:      hsettimeofday.h
 * Purpose:   声明hsettimeofday接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-01
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HSETTIMEOFDAY_H__
#define __HSETTIMEOFDAY_H__
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#include "hgettimeofday.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

typedef hgettimeofday_timezone_t hsettimeofday_timezone_t;
typedef hgettimeofday_timeval_t hsettimeofday_timeval_t;


/** \brief 设置当前时间
 *
 * \param tv const hsettimeofday_timeval_t * 当前时间,可为NULL
 * \param tz const hsettimeofday_timezone_t* 当前时区,可为NULL
 * \return int 0=成功，-1=失败
 *
 */
int hsettimeofday (const hsettimeofday_timeval_t *tv,const hsettimeofday_timezone_t * tz);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HSETTIMEOFDAY_H__
