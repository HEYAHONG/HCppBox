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

htm_t * hgmtime_r(const htime_t *tim_p,htm_t * res);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HHLIBC_TIME_H__
