/***************************************************************
 * Name:      hmtx.c
 * Purpose:   实现hmtx接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-01-29
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"
#include "hmtx.h"

#if defined(HMTX_INIT)
extern int HMTX_INIT(hmtx_t* __mutex, int type);
#endif

int hmtx_init(hmtx_t* __mutex, int type)
{
    int ret=hthrd_error;
    if(__mutex==NULL)
    {
        return ret;
    }

#if defined(HMTX_INIT)
    ret=HMTX_INIT(__mutex,type);
#elif defined(HDEFAULTS_LIBC_HAVE_THREADS)
    if(sizeof(hmtx_t) < sizeof(mtx_t))
    {
        return ret;
    }
    ret=hthrd_code_wrapper(mtx_init((mtx_t *)__mutex,type));
#endif

    return ret;
}

#if defined(HMTX_LOCK)
extern int HMTX_LOCK( hmtx_t* __mutex);
#endif

int hmtx_lock( hmtx_t* __mutex)
{
    int ret=hthrd_error;
    if(__mutex==NULL)
    {
        return ret;
    }

#if defined(HMTX_LOCK)
    ret=HMTX_LOCK(__mutex);
#elif defined(HDEFAULTS_LIBC_HAVE_THREADS)
    if(sizeof(hmtx_t) < sizeof(mtx_t))
    {
        return ret;
    }
    ret=hthrd_code_wrapper(mtx_lock((mtx_t *)__mutex));
#endif

    return ret;
}

#if defined(HMTX_TIMEDLOCK)
extern int HMTX_TIMEDLOCK(hmtx_t * __mutex,const htimespec_t * time_point);
#endif

int hmtx_timedlock(hmtx_t * __mutex,const htimespec_t * time_point)
{
    int ret=hthrd_error;
    if(__mutex==NULL)
    {
        return ret;
    }

#if defined(HMTX_TIMEDLOCK)
    ret=HMTX_TIMEDLOCK(__mutex,time_point);
#elif defined(HDEFAULTS_LIBC_HAVE_THREADS)
    if(sizeof(hmtx_t) < sizeof(mtx_t))
    {
        return ret;
    }
    struct timespec tp= {0};
    if(time_point!=NULL)
    {
        tp.tv_sec=time_point->tv_sec;
        tp.tv_nsec=time_point->tv_nsec;
    }
    ret=hthrd_code_wrapper(mtx_timedlock((mtx_t *)__mutex,&tp));
#endif

    return ret;
}

#if defined(HMTX_TRYLOCK)
extern int HMTX_TRYLOCK(hmtx_t *__mutex);
#endif

int hmtx_trylock(hmtx_t *__mutex)
{
    int ret=hthrd_error;
    if(__mutex==NULL)
    {
        return ret;
    }

#if defined(HMTX_TRYLOCK)
    ret=HMTX_TRYLOCK(__mutex);
#elif defined(HDEFAULTS_LIBC_HAVE_THREADS)
    if(sizeof(hmtx_t) < sizeof(mtx_t))
    {
        return ret;
    }
    ret=hthrd_code_wrapper(mtx_trylock((mtx_t *)__mutex));
#endif

    return ret;
}


#if defined(HMTX_UNLOCK)
extern int HMTX_UNLOCK( hmtx_t *__mutex);
#endif

int hmtx_unlock( hmtx_t *__mutex)
{
    int ret=hthrd_error;
    if(__mutex==NULL)
    {
        return ret;
    }

#if defined(HMTX_UNLOCK)
    ret=HMTX_UNLOCK(__mutex);
#elif defined(HDEFAULTS_LIBC_HAVE_THREADS)
    if(sizeof(hmtx_t) < sizeof(mtx_t))
    {
        return ret;
    }
    ret=hthrd_code_wrapper(mtx_unlock((mtx_t *)__mutex));
#endif


    return ret;
}

#if defined(HMTX_DESTROY)
extern void HMTX_DESTROY(hmtx_t *__mutex );
#endif

void hmtx_destroy(hmtx_t *__mutex )
{
    if(__mutex==NULL)
    {
        return;
    }

#if defined(HMTX_DESTROY)
    HMTX_DESTROY(__mutex);
#elif defined(HDEFAULTS_LIBC_HAVE_THREADS)
    if(sizeof(hmtx_t) < sizeof(mtx_t))
    {
        return;
    }
    mtx_destroy((mtx_t *)__mutex);
#endif

}
