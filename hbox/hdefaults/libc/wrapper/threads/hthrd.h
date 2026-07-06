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

#include "hdefaults.h"
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

#if !defined(HTHRD_SIZE_FORM_BYTES)
#define HTHRD_SIZE_FORM_BYTES(nbytes)  (((nbytes)+sizeof(uintptr_t)-1)/(sizeof(uintptr_t)))
#endif

#if defined(HDEFAULTS_LIBC_HAVE_THREADS)
#undef HTHRD_SIZE
/*
 * 采用C11的threads头文件
 */
#define HTHRD_SIZE  HTHRD_SIZE_FORM_BYTES(sizeof(thrd_t))
#elif defined(HDEFAULTS_OS_WINDOWS)
#undef HTHRD_SIZE
/*
 * 见hthrd_os_windows.c
 */
#define HTHRD_SIZE  HTHRD_SIZE_FORM_BYTES(sizeof(HANDLE)+sizeof(DWORD))
#elif (defined(HDEFAULTS_OS_UNIX) || defined(HTHRD_USING_PTHREAD))
#undef HTHRD_SIZE
/*
 * 见hthrd_pthread.c
 */
#define HTHRD_SIZE HTHRD_SIZE_FORM_BYTES(sizeof(pthread_t)+sizeof(void*))
#elif ((defined(FREERTOS_KERNEL)) || (defined(FREERTOS) || defined(HTHRD_USING_FREERTOS)))
#undef HTHRD_SIZE
/*
 * 见hthrd_rtos_freertos.c,注意：hthrd_t仅用于容纳TaskHandle_t(等效于struct tskTaskControlBlock* )
 */
#define HTHRD_SIZE HTHRD_SIZE_FORM_BYTES(sizeof(void*))
#endif


struct hthrd
{
    /*
     * 存储线程信息,默认应当清零。
     */
    uintptr_t storage[HTHRD_SIZE];
};

typedef struct hthrd hthrd_t;

typedef int (*hthrd_start_t)(void *arg);

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
