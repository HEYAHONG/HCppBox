/***************************************************************
 * Name:      hclock_gettime.h
 * Purpose:   声明hclock_gettime接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-01-21
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HCLOCK_GETTIME_H__
#define __HCLOCK_GETTIME_H__
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#include "../hsyscall/time/hsyscall_time_common.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus


/** \brief 获取时钟
 *
 * \param clockid hclockid_t 时钟id
 * \param tp htimespec_t* 时间
 * \return int  0=成功，-1=失败
 *
 */
int hclock_gettime(hclockid_t clockid, htimespec_t * tp);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HCLOCK_GETTIME_H__
