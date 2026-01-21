/***************************************************************
 * Name:      hclock_getres.h
 * Purpose:   声明hclock_getres接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-01-21
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HCLOCK_GETRES_H__
#define __HCLOCK_GETRES_H__
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#include "../hsyscall/time/hsyscall_time_common.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus


/** \brief 获取时钟精度
 *
 * \param clockid hclockid_t 时钟id
 * \param res htimespec_t* 时钟精度
 * \return int  0=成功，-1=失败
 *
 */
int hclock_getres(hclockid_t clockid, htimespec_t * res);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HCLOCK_GETRES_H__
