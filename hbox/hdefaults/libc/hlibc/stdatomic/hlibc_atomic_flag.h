/***************************************************************
 * Name:      hlibc_atomic_flag.h
 * Purpose:   声明hlibc_atomic_flag接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-01-20
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HLIBC_ATOMIC_FLAG_H__
#define __HLIBC_ATOMIC_FLAG_H__
#include "../../wrapper/stdatomic/hatomic_flag.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus


bool hlibc_atomic_flag_test_and_set_explicit(volatile hatomic_flag_t *__object,hmemory_order_t __order);

void hlibc_atomic_flag_clear_explicit(volatile hatomic_flag_t *__object, hmemory_order_t __order);

static __inline bool hlibc_atomic_flag_test_and_set(volatile hatomic_flag_t *__object)
{
    return (hlibc_atomic_flag_test_and_set_explicit(__object,hmemory_order_seq_cst));
}

static __inline void hlibc_atomic_flag_clear(volatile hatomic_flag_t *__object)
{
    hlibc_atomic_flag_clear_explicit(__object, hmemory_order_seq_cst);
}

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HLIBC_ATOMIC_FLAG_H__
