/***************************************************************
 * Name:      hmtx_pthread.c
 * Purpose:   实现hmtx_pthread接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-01-29
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"
#include "hmtx.h"
#include "pthread.h"
#include "errno.h"

typedef pthread_mutex_t hmtx_pthread_t;


int hmtx_pthread_init(hmtx_t* __mutex, int type)
{
    int ret=hthrd_error;
    if(__mutex==NULL)
    {
        return ret;
    }
    if(sizeof(hmtx_t) < sizeof(hmtx_pthread_t))
    {
        return ret;
    }
    hmtx_pthread_t *mtx=((hmtx_pthread_t *)__mutex);

    hmemset(mtx,0,sizeof(hmtx_pthread_t));

    int mt=PTHREAD_MUTEX_NORMAL;
    if((type&hmtx_recursive)!=0)
    {
        mt=PTHREAD_MUTEX_RECURSIVE;
    }

    pthread_mutexattr_t attr;
    if(pthread_mutexattr_init(&attr)!=0)
    {
        return ret;
    }

    if(pthread_mutexattr_settype(&attr, mt) != 0)
    {
        return ret;
    }

    if (pthread_mutex_init(mtx, &attr) != 0)
    {
        return ret;
    }

    ret=hthrd_success;

    return ret;
}


int hmtx_pthread_lock( hmtx_t* __mutex)
{
    int ret=hthrd_error;
    if(__mutex==NULL)
    {
        return ret;
    }
    if(sizeof(hmtx_t) < sizeof(hmtx_pthread_t))
    {
        return ret;
    }
    hmtx_pthread_t *mtx=((hmtx_pthread_t *)__mutex);

    if(pthread_mutex_lock(mtx) == 0)
    {
        ret=hthrd_success;
    }

    return ret;
}


int hmtx_pthread_timedlock(hmtx_t * __mutex,const htimespec_t * time_point)
{
    int ret=hthrd_error;
    if(__mutex==NULL)
    {
        return ret;
    }
    if(sizeof(hmtx_t) < sizeof(hmtx_pthread_t))
    {
        return ret;
    }

    hmtx_pthread_t *mtx=((hmtx_pthread_t *)__mutex);


    struct timespec tp= {0};
    if(time_point!=NULL)
    {
        tp.tv_sec=time_point->tv_sec;
        tp.tv_nsec=time_point->tv_nsec;
    }
    switch(pthread_mutex_timedlock(mtx,&tp))
    {
    case 0:
    {
        ret=hthrd_success;
    }
    break;
    case ETIMEDOUT:
    {
        ret=hthrd_timedout;
    }
    break;
    default:
    {

    }
    break;
    }

    return ret;
}


int hmtx_pthread_trylock(hmtx_t *__mutex)
{
    int ret=hthrd_error;
    if(__mutex==NULL)
    {
        return ret;
    }
    if(sizeof(hmtx_t) < sizeof(hmtx_pthread_t))
    {
        return ret;
    }

    hmtx_pthread_t *mtx=((hmtx_pthread_t *)__mutex);

    switch(pthread_mutex_trylock(mtx))
    {
    case 0:
    {
        ret=hthrd_success;
    }
    break;
    case EBUSY:
    {
        ret=hthrd_busy;
    }
    break;
    default:
    {

    }
    break;
    }


    return ret;
}


int hmtx_pthread_unlock( hmtx_t *__mutex)
{
    int ret=hthrd_error;
    if(__mutex==NULL)
    {
        return ret;
    }
    if(sizeof(hmtx_t) < sizeof(hmtx_pthread_t))
    {
        return ret;
    }
    hmtx_pthread_t *mtx=((hmtx_pthread_t *)__mutex);

    if(pthread_mutex_unlock(mtx) == 0)
    {
        ret=hthrd_success;
    }

    return ret;
}



void hmtx_pthread_destroy(hmtx_t *__mutex )
{
    if(__mutex==NULL)
    {
        return;
    }
    if(sizeof(hmtx_t) < sizeof(hmtx_pthread_t))
    {
        return;
    }

    hmtx_pthread_t *mtx=((hmtx_pthread_t *)__mutex);

    pthread_mutex_destroy(mtx);

}
