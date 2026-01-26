/***************************************************************
 * Name:      hthrd.h
 * Purpose:   声明hthrd接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-01-26
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HTHRD_H__
#define __HTHRD_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "hthreads_common.h"
#include "stdint.h"
#include "../time/htime.h"

/*
 * 定义hthrd_t大小，单位为指针大小的倍数
 */
#ifndef HTHRD_SIZE
#define HTHRD_SIZE (4)
#else
#if (HTHRD_SIZE) < (1)
#undef  HTHRD_SIZE
#define HTHRD_SIZE (4)
#endif
#endif // HTHRD_SIZE

struct hthrd
{
    /*
     * 存储线程信息,默认应当清零。
     */
    uintptr_t storage[HTHRD_SIZE];
};

typedef struct hthrd hthrd_t;

typedef int (hthrd_start_t)(void *arg);

int hthrd_create(hthrd_t *thr,hthrd_start_t func,void *arg );
int hthrd_equal(hthrd_t lhs,hthrd_t rhs );
hthrd_t hthrd_current(void);
int hthrd_sleep(const htimespec_t* duration,htimespec_t * remaining);
void hthrd_yield(void);
void hthrd_exit(int res);
int hthrd_detach(hthrd_t thr);
int hthrd_join(hthrd_t thr, int *res);


#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HTHRD_H__
