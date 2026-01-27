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

#if defined(HDEFAULTS_OS_WINDOWS)
#include "hthrd_os_windows.c"
#endif

#if defined(HTHRD_CREATE)
extern int HTHRD_CREATE(hthrd_t *thr,hthrd_start_t func,void *arg )
#endif

int hthrd_create(hthrd_t *thr,hthrd_start_t func,void *arg )
{
    int ret=hthrd_error;
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
#endif
    return ret;
}

#if defined(HTHRD_EQUAL)
extern int HTHRD_EQUAL(hthrd_t lhs,hthrd_t rhs )
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
#endif
    return ret;
}

