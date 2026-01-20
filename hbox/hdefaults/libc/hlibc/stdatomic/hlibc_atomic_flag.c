/***************************************************************
 * Name:      hlibc_atomic_flag.c
 * Purpose:   实现hlibc_atomic_flag接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-01-20
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"
#include "hlibc_atomic_flag.h"
#include "stdlib.h"

struct hlibc_atomic_flag
{
    bool __flag;
};
typedef struct hlibc_atomic_flag hlibc_atomic_flag_t;

bool hlibc_atomic_flag_test_and_set_explicit(volatile hatomic_flag_t *__object,hmemory_order_t __order)
{
    (void)__order;
    bool ret=true;
    hdefaults_mutex_lock(NULL);
    ret=((volatile hlibc_atomic_flag_t *)__object)->__flag;
    ((volatile hlibc_atomic_flag_t *)__object)->__flag=true;
    hdefaults_mutex_unlock(NULL);
    return ret;
}

void hlibc_atomic_flag_clear_explicit(volatile hatomic_flag_t *__object, hmemory_order_t __order)
{
    (void)__order;
    hdefaults_mutex_lock(NULL);
    ((volatile hlibc_atomic_flag_t *)__object)->__flag=false;
    hdefaults_mutex_unlock(NULL);
}



