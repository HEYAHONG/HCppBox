/***************************************************************
 * Name:      hcall_once.h
 * Purpose:   声明hcall_once接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-01-29
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HHCALL_ONCE_H__
#define __HHCALL_ONCE_H__
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "hthrd.h"

#if !defined(HCALL_ONCE_SIZE)
#define HCALL_ONCE_SIZE (1)
#endif
#if (HCALL_ONCE_SIZE) < (1)
#undef  HCALL_ONCE_SIZE
#define HCALL_ONCE_SIZE (1)
#endif

struct honce_flag
{
    /*
     * 存储once_flag信息,默认应当清零。
     */
    uintptr_t storage[HCALL_ONCE_SIZE];
};

typedef struct honce_flag honce_flag_t;
#define HONCE_FLAG_INIT   {{0}}

typedef void (*hcall_once_func_t)(void);

void hcall_once(honce_flag_t *flag,hcall_once_func_t func);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HHCALL_ONCE_H__
