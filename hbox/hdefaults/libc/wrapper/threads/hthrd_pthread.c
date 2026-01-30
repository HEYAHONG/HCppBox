/***************************************************************
 * Name:      hthrd_pthread.c
 * Purpose:   实现hthrd_pthread接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-01-27
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"
#include "stdlib.h"
#include "pthread.h"
#include "time.h"

typedef pthread_t hthrd_pthread_t;

struct hthrd_pthread_create_context
{
    hthrd_start_t func;
    void *arg;
};
typedef struct hthrd_pthread_create_context hthrd_pthread_create_context_t;

static void * hthrd_pthread_thread_proc(void *th_arg)
{
    hthrd_pthread_create_context_t *ctx=(hthrd_pthread_create_context_t*)th_arg;
    if(ctx==NULL)
    {
        return (void *)(intptr_t)-1;
    }

    hthrd_start_t   func=ctx->func;
    void *          arg=ctx->arg;

    hfree(ctx);

    if(func!=NULL)
    {
        return (void *)(intptr_t)func(arg);
    }


    return (void *)(intptr_t)-1;
}


int hthrd_pthread_create(hthrd_t *thr,hthrd_start_t func,void *arg )
{
    int ret=hthrd_error;
    if(sizeof(hthrd_t) < sizeof(hthrd_pthread_t))
    {
        return ret;
    }
    if(func==NULL)
    {
        return ret;
    }
    hthrd_pthread_create_context_t *ctx=(hthrd_pthread_create_context_t *)hmalloc(sizeof(hthrd_pthread_create_context_t));
    if(ctx==NULL)
    {
        return ret;
    }

    ctx->func=func;
    ctx->arg=arg;

    if (pthread_create((hthrd_pthread_t *)thr, NULL, hthrd_pthread_thread_proc, ctx) == 0)
    {
        ret=hthrd_success;
    }
    else
    {
        hfree(ctx);
    }

    return ret;
}


int hthrd_pthread_equal(hthrd_t lhs,hthrd_t rhs )
{
    int ret=0;

    if(sizeof(hthrd_t) < sizeof(hthrd_pthread_t))
    {
        return ret;
    }

    ret=pthread_equal(*(hthrd_pthread_t *)&lhs,*(hthrd_pthread_t *)&rhs);

    return ret;
}


hthrd_t hthrd_pthread_current(void)
{
    hthrd_t ret= {0};

    if(sizeof(hthrd_t) < sizeof(hthrd_pthread_t))
    {
        return ret;
    }

    (*(hthrd_pthread_t *)&ret)=pthread_self();

    return ret;
}


int hthrd_pthread_sleep(const htimespec_t* duration,htimespec_t * remaining)
{
    int ret=0;

    struct timespec req= {0},rem= {0};
    if(duration!=NULL)
    {
        req.tv_sec=duration->tv_sec;
        req.tv_nsec=duration->tv_nsec;
    }
#if defined(NANOSLEEP)
    ret=NANOSLEEP(&req,&rem);
#else
    ret=nanosleep(&req,&rem);
#endif

    if(remaining!=NULL)
    {
        remaining->tv_sec=rem.tv_sec;
        remaining->tv_nsec=rem.tv_nsec;
    }

    return ret;
}

#if defined(HDEFAULTS_OS_UNIX) && defined(_POSIX_PRIORITY_SCHEDULING)
#include "sched.h"
#endif
void hthrd_pthread_yield(void)
{
#if defined(HDEFAULTS_OS_UNIX) && defined(_POSIX_PRIORITY_SCHEDULING)
    sched_yield();
#else
    hthrd_pthread_sleep(NULL,NULL);
#endif
}


void hthrd_pthread_exit(int res)
{
    pthread_exit((void *)(intptr_t)res);
}


int hthrd_pthread_detach(hthrd_t thr)
{
    int ret=hthrd_error;
    if(sizeof(hthrd_t) < sizeof(hthrd_pthread_t))
    {
        return ret;
    }

    if((pthread_detach(*(hthrd_pthread_t *)&thr)==0))
    {
        ret=hthrd_success;
    }

    return ret;
}


int hthrd_pthread_join(hthrd_t thr, int *res)
{
    int ret=hthrd_error;
    if(sizeof(hthrd_t) < sizeof(hthrd_pthread_t))
    {
        return ret;
    }

    void *value_ptr=NULL;

    if (pthread_join(*(hthrd_pthread_t *)&thr, &value_ptr) != 0)
    {
        return ret;
    }
    if (res != NULL)
    {
        (*res) = (intptr_t)value_ptr;
    }
    ret=hthrd_success;

    return ret;
}

