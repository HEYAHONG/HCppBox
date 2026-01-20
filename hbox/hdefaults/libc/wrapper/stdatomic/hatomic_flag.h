/***************************************************************
 * Name:      hatomic_flag.h
 * Purpose:   声明hatomic_flag接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-01-20
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HATOMIC_FLAG_H__
#define __HATOMIC_FLAG_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "hstdatomic_common.h"
#include "hdefaults/hdefaults_config.h"

/*
 * 定义hatomic_flag_t结构体与HATOMIC_FLAG_INIT宏
 */
#if !defined(HATOMIC_FLAG)
#if !defined(HDEFAULTS_LIBC_HAVE_STDATOMIC)
struct hatomic_flag
{
    bool __flag;
};
typedef struct hatomic_flag hatomic_flag_t;
#define HATOMIC_FLAG_INIT   {false}
#else
typedef atomic_flag hatomic_flag_t;
#define HATOMIC_FLAG_INIT   ATOMIC_FLAG_INIT
#endif
#else
/*
 * 采用用户的定义，此时用户需要自行定义HATOMIC_FLAG_INIT
 */
typedef HATOMIC_FLAG hatomic_flag_t;
#endif

bool hatomic_flag_test_and_set_explicit(volatile hatomic_flag_t *__object,hmemory_order_t __order);

void hatomic_flag_clear_explicit(volatile hatomic_flag_t *__object, hmemory_order_t __order);

static __inline bool hatomic_flag_test_and_set(volatile hatomic_flag_t *__object)
{
    return (hatomic_flag_test_and_set_explicit(__object,hmemory_order_seq_cst));
}

static __inline void hatomic_flag_clear(volatile hatomic_flag_t *__object)
{
    hatomic_flag_clear_explicit(__object, hmemory_order_seq_cst);
}

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HATOMIC_FLAG_H__
