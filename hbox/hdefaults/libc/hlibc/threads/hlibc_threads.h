/***************************************************************
 * Name:      hlibc_threads.h
 * Purpose:   声明hlibc_threads接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-01-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HHLIBC_THREADS_H__
#define __HHLIBC_THREADS_H__
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "../../wrapper/stdatomic/hatomic_flag.h"
#include "../../wrapper/threads/hthrd.h"


typedef hatomic_flag_t hlibc_once_flag_t;
#define HLIBC_ONCE_FLAG_INIT HATOMIC_FLAG_INIT

typedef void (*hlibc_call_once_func_t)(void);

void hlibc_call_once(hlibc_once_flag_t *flag,hlibc_call_once_func_t func);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HHLIBC_THREADS_H__
