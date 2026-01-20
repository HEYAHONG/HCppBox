/***************************************************************
 * Name:      hatomic_flag.c
 * Purpose:   实现hatomic_flag接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-01-20
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"
#include "hatomic_flag.h"
#include "stdlib.h"

#if defined(HATOMIC_FLAG_TEST_AND_SET_EXPLICIT)
extern bool HATOMIC_FLAG_TEST_AND_SET_EXPLICIT(volatile hatomic_flag_t *__object,hmemory_order_t __order);
#endif

bool hatomic_flag_test_and_set_explicit(volatile hatomic_flag_t *__object,hmemory_order_t __order)
{
    /*
     * 默认一般情况下可使用atomic_flag实现锁，返回true表示未获得锁
     */
    bool ret=true;
#if defined(HATOMIC_FLAG_TEST_AND_SET_EXPLICIT)
    ret=HATOMIC_FLAG_TEST_AND_SET_EXPLICIT(__object,__order);
#elif defined(HDEFAULTS_LIBC_HAVE_STDATOMIC)
    ret=atomic_flag_test_and_set_explicit((volatile atomic_flag *)__object,(memory_order)__order);
#else
    ret=hlibc_atomic_flag_test_and_set_explicit(__object,__order);
#endif
    return ret;
}

#if defined(HATOMIC_FLAG_CLEAR_EXPLICIT)
extern void HATOMIC_FLAG_CLEAR_EXPLICIT(volatile hatomic_flag_t *__object, hmemory_order_t __order);
#endif

void hatomic_flag_clear_explicit(volatile hatomic_flag_t *__object, hmemory_order_t __order)
{

#if defined(HATOMIC_FLAG_CLEAR_EXPLICIT)
    HATOMIC_FLAG_CLEAR_EXPLICIT(__object,__order);
#elif defined(HDEFAULTS_LIBC_HAVE_STDATOMIC)
    atomic_flag_clear_explicit((volatile atomic_flag *)__object,(memory_order)__order);
#else
    hlibc_atomic_flag_clear_explicit(__object,__order);
#endif

}

