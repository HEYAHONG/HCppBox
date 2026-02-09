/***************************************************************
 * Name:      hthrd.c
 * Purpose:   实现hthrd接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-01-26
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"
#include "hthrd.h"
#include "stdlib.h"

#if defined(FREERTOS_KERNEL)
#include "hthrd_rtos_freertos.c"
#if !defined(HTHRD_USING_FREERTOS)
#define  HTHRD_USING_FREERTOS   1
#endif
#elif defined(HTHRD_USING_FREERTOS)
/*
 * 注意:使用外部FreerTOS内核时，需要在配置文件中包含FreeRTOS.h与task.h
 */
#if !defined(HTHRD_FREERTOS_FREERTOS_HEADER)
#define  HTHRD_FREERTOS_FREERTOS_HEADER "FreeRTOS.h"
#endif
#include HTHRD_FREERTOS_FREERTOS_HEADER
#if !defined(HTHRD_FREERTOS_TASK_HEADER)
#define  HTHRD_FREERTOS_TASK_HEADER "task.h"
#endif
#include HTHRD_FREERTOS_TASK_HEADER

#if !defined(configSUPPORT_STATIC_ALLOCATION) || (configSUPPORT_STATIC_ALLOCATION)<(1)
#undef HTHRD_USING_FREERTOS
#endif

#if !defined(INCLUDE_xTaskGetCurrentTaskHandle) || (INCLUDE_xTaskGetCurrentTaskHandle)<(1)
#undef HTHRD_USING_FREERTOS
#endif

#if !defined(configUSE_TIMERS) || (configUSE_TIMERS)<(1)
#undef HTHRD_USING_FREERTOS
#endif

#if defined(HTHRD_USING_FREERTOS)
#include "hthrd_rtos_freertos.c"
#endif

#endif

#if defined(HDEFAULTS_OS_WINDOWS)
#include "hthrd_os_windows.c"
#endif

#if defined(HDEFAULTS_OS_UNIX) || defined(HTHRD_USING_PTHREAD)
#include "hthrd_pthread.c"
#endif

#if defined(HTHRD_CREATE)
extern int HTHRD_CREATE(hthrd_t *thr,hthrd_start_t func,void *arg );
#endif

int hthrd_create(hthrd_t *thr,hthrd_start_t func,void *arg )
{
    int ret=hthrd_error;
    hthrd_t temp_thr;
    bool need_detach=false;
    if(thr==NULL)
    {
        thr=&temp_thr;
        need_detach=true;
    }
#if defined(HTHRD_CREATE)
    ret=HTHRD_CREATE(thr,func,arg);
#elif defined(HDEFAULTS_LIBC_HAVE_THREADS)
    if(sizeof(hthrd_t) < sizeof(thrd_t))
    {
        return ret;
    }
    ret=hthrd_code_wrapper(thrd_create((thrd_t *)thr,func,arg));
#elif defined(HDEFAULTS_OS_WINDOWS)
    ret=hthrd_windows_create(thr,func,arg);
#elif defined(HDEFAULTS_OS_UNIX) || defined(HTHRD_USING_PTHREAD)
    ret=hthrd_pthread_create(thr,func,arg);
#elif defined(HTHRD_USING_FREERTOS)
    ret=hthrd_freertos_create(thr,func,arg);
#endif
    if(ret==hthrd_success && need_detach)
    {
        hthrd_detach(*thr);
    }
    return ret;
}

#if defined(HTHRD_EQUAL)
extern int HTHRD_EQUAL(hthrd_t lhs,hthrd_t rhs );
#endif

int hthrd_equal(hthrd_t lhs,hthrd_t rhs )
{
    int ret=-1;
#if defined(HTHRD_EQUAL)
    ret=HTHRD_EQUAL(lhs,rhs);
#elif defined(HDEFAULTS_LIBC_HAVE_THREADS)
    ret=thrd_equal(*(thrd_t *)&lhs,*(thrd_t *)&rhs);
#elif defined(HDEFAULTS_OS_WINDOWS)
    ret=hthrd_windows_equal(lhs,rhs);
#elif defined(HDEFAULTS_OS_UNIX) || defined(HTHRD_USING_PTHREAD)
    ret=hthrd_pthread_equal(lhs,rhs);
#elif defined(HTHRD_USING_FREERTOS)
    ret=hthrd_freertos_equal(lhs,rhs);
#endif
    return ret;
}

#if defined(HTHRD_CURRENT)
extern hthrd_t HTHRD_CURRENT(void);
#endif

hthrd_t hthrd_current(void)
{
    hthrd_t ret= {0};
#if defined(HTHRD_CURRENT)
    ret=HTHRD_CURRENT();
#elif defined(HDEFAULTS_LIBC_HAVE_THREADS)
    (*(thrd_t *)&ret)=thrd_current();
#elif defined(HDEFAULTS_OS_WINDOWS)
    ret=hthrd_windows_current();
#elif defined(HDEFAULTS_OS_UNIX) || defined(HTHRD_USING_PTHREAD)
    ret=hthrd_pthread_current();
#elif defined(HTHRD_USING_FREERTOS)
    ret=hthrd_freertos_current();
#endif
    return ret;
}

#if defined(HTHRD_SLEEP)
extern int HTHRD_SLEEP(const htimespec_t* duration,htimespec_t * remaining);
#endif

int hthrd_sleep(const htimespec_t* duration,htimespec_t * remaining)
{
    int ret=-1;
#if defined(HTHRD_SLEEP)
    ret=HTHRD_SLEEP(duration,remaining);
#elif defined(HDEFAULTS_LIBC_HAVE_THREADS)
    {
        struct timespec dur= {0};
        if(duration!=NULL)
        {
            dur.tv_sec=duration->tv_sec;
            dur.tv_nsec=duration->tv_nsec;
        }
        struct timespec rem= {0};;
        ret=thrd_sleep(&dur,&rem);
        if(remaining!=NULL)
        {
            remaining->tv_sec=rem.tv_sec;
            remaining->tv_nsec=rem.tv_nsec;
        }
    }
#elif defined(HDEFAULTS_OS_WINDOWS)
    ret=hthrd_windows_sleep(duration,remaining);
#elif defined(HDEFAULTS_OS_UNIX) || defined(HTHRD_USING_PTHREAD)
    ret=hthrd_pthread_sleep(duration,remaining);
#elif defined(HTHRD_USING_FREERTOS)
    ret=hthrd_freertos_sleep(duration,remaining);
#endif
    return ret;
}

#if defined(HTHRD_YIELD)
extern void HTHRD_YIELD(void);
#endif
void hthrd_yield(void)
{
#if defined(HTHRD_YIELD)
    HTHRD_YIELD();
#elif defined(HDEFAULTS_LIBC_HAVE_THREADS)
    thrd_yield();
#elif defined(HDEFAULTS_OS_WINDOWS)
    hthrd_windows_yield();
#elif defined(HDEFAULTS_OS_UNIX) || defined(HTHRD_USING_PTHREAD)
    hthrd_pthread_yield();
#elif defined(HTHRD_USING_FREERTOS)
    hthrd_freertos_yield();
#endif
}

#if defined(HTHRD_EXIT)
extern void HTHRD_EXIT(int res);
#endif

void hthrd_exit(int res)
{
#if defined(HTHRD_EXIT)
    HTHRD_EXIT(res);
#elif defined(HDEFAULTS_LIBC_HAVE_THREADS)
    thrd_exit(res);
#elif defined(HDEFAULTS_OS_WINDOWS)
    hthrd_windows_exit(res);
#elif defined(HDEFAULTS_OS_UNIX) || defined(HTHRD_USING_PTHREAD)
    hthrd_pthread_exit(res);
#elif defined(HTHRD_USING_FREERTOS)
    hthrd_freertos_exit(res);
#endif
}

#if defined(HTHRD_DETACH)
extern int hthrd_detach(hthrd_t thr);
#endif
int hthrd_detach(hthrd_t thr)
{
    int ret=hthrd_error;
#if defined(HTHRD_DETACH)
    ret=HTHRD_DETACH(thr);
#elif defined(HDEFAULTS_LIBC_HAVE_THREADS)
    ret=hthrd_code_wrapper(thrd_detach(*(thrd_t *)&thr));
#elif defined(HDEFAULTS_OS_WINDOWS)
    ret=hthrd_windows_detach(thr);
#elif defined(HDEFAULTS_OS_UNIX) || defined(HTHRD_USING_PTHREAD)
    ret=hthrd_pthread_detach(thr);
#elif defined(HTHRD_USING_FREERTOS)
    ret=hthrd_freertos_detach(thr);
#endif
    return ret;
}

#if defined(HTHRD_JOIN)
extern int HTHRD_JOIN(hthrd_t thr, int *res);
#endif
int hthrd_join(hthrd_t thr, int *res)
{
    int ret=hthrd_error;
#if defined(HTHRD_JOIN)
    ret=HTHRD_JOIN(thr,res);
#elif defined(HDEFAULTS_LIBC_HAVE_THREADS)
    ret=hthrd_code_wrapper(thrd_join(*(thrd_t *)&thr,res));
#elif defined(HDEFAULTS_OS_WINDOWS)
    ret=hthrd_windows_join(thr,res);
#elif defined(HDEFAULTS_OS_UNIX) || defined(HTHRD_USING_PTHREAD)
    ret=hthrd_pthread_join(thr,res);
#elif defined(HTHRD_USING_FREERTOS)
    ret=hthrd_freertos_join(thr,res);
#endif
    return ret;
}

