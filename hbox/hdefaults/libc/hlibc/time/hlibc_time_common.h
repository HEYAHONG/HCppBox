/***************************************************************
 * Name:      hlibc_time_common.h
 * Purpose:   声明hlibc_time_common接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-01-22
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HHLIBC_TIME_COMMON_H__
#define __HHLIBC_TIME_COMMON_H__
#include "stdint.h"
#include "time.h"
#include "../../../syscall/hsyscall/time/hsyscall_time_common.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus


typedef struct tm     htm_t;

typedef time_t        htime_t;

typedef clock_t       hclock_t;

/*
 * htimespec_t已在hsyscall_time_common.h定义
 */

#ifdef CLOCKS_PER_SEC
#define HCLOCKS_PER_SEC CLOCKS_PER_SEC
#else
#define HCLOCKS_PER_SEC (1000)
#endif // CLOCKS_PER_SEC

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HHLIBC_TIME_COMMON_H__
