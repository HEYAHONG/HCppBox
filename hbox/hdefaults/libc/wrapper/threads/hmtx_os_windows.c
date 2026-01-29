/***************************************************************
 * Name:      hmtx_os_windows.c
 * Purpose:   实现hmtx_os_windows接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-01-29
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"
#include "hmtx.h"

struct hmtx_windows
{
    HANDLE hMutex;
    int    type;
};
typedef struct hmtx_windows hmtx_windows_t;


int hmtx_windows_init(hmtx_t* __mutex, int type)
{
    int ret=hthrd_error;
    if(__mutex==NULL)
    {
        return ret;
    }
    if(sizeof(hmtx_t) < sizeof(hmtx_windows_t))
    {
        return ret;
    }
    hmtx_windows_t *mtx=((hmtx_windows_t *)__mutex);

    hmemset(mtx,0,sizeof(hmtx_windows_t));

    mtx->hMutex=CreateMutex(NULL,FALSE,NULL);
    /*
     * 记录类型，此处并不使用此参数
     */
    mtx->type=type;

    if(mtx->hMutex!=NULL)
    {
        ret=hthrd_success;
    }

    return ret;
}


int hmtx_windows_lock( hmtx_t* __mutex)
{
    int ret=hthrd_error;
    if(__mutex==NULL)
    {
        return ret;
    }
    if(sizeof(hmtx_t) < sizeof(hmtx_windows_t))
    {
        return ret;
    }
    hmtx_windows_t *mtx=((hmtx_windows_t *)__mutex);
    if(mtx->hMutex==NULL)
    {
        return ret;
    }

    /*
     * 加锁
     */
    ret=(WaitForSingleObject(mtx->hMutex,INFINITE)==WAIT_OBJECT_0)?hthrd_success:hthrd_error;

    return ret;
}


int hmtx_windows_timedlock(hmtx_t * __mutex,const htimespec_t * time_point)
{
    int ret=hthrd_error;
    if(__mutex==NULL)
    {
        return ret;
    }
    if(sizeof(hmtx_t) < sizeof(hmtx_windows_t))
    {
        return ret;
    }

    hmtx_windows_t *mtx=((hmtx_windows_t *)__mutex);
    if(mtx->hMutex==NULL)
    {
        return ret;
    }

    do
    {
        /*
         * 加锁
         */
        ret=(WaitForSingleObject(mtx->hMutex,0)==WAIT_OBJECT_0)?hthrd_success:hthrd_error;

        if(ret==hthrd_success)
        {
            break;
        }

        if(time_point!=NULL)
        {
            htimespec_t cur= {0};
            hclock_gettime(HCLOCK_REALTIME,&cur);
            if(cur.tv_sec >= time_point->tv_sec && cur.tv_nsec >= time_point->tv_nsec)
            {
                ret=hthrd_timedout;
                break;
            }
            else
            {
                Sleep(1);
            }
        }
        else
        {
            if(ret==hthrd_error)
            {
                ret=hthrd_timedout;
                break;
            }
        }

    } while(ret!=hthrd_success);


    return ret;
}


int hmtx_windows_trylock(hmtx_t *__mutex)
{
    int ret=hthrd_error;
    if(__mutex==NULL)
    {
        return ret;
    }
    if(sizeof(hmtx_t) < sizeof(hmtx_windows_t))
    {
        return ret;
    }

    hmtx_windows_t *mtx=((hmtx_windows_t *)__mutex);
    if(mtx->hMutex==NULL)
    {
        return ret;
    }

    /*
     * 加锁
     */
    switch(WaitForSingleObject(mtx->hMutex,0))
    {
    case WAIT_OBJECT_0:
    {
        ret=hthrd_success;
    }
    break;
    case WAIT_TIMEOUT:
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


int hmtx_windows_unlock( hmtx_t *__mutex)
{
    int ret=hthrd_error;
    if(__mutex==NULL)
    {
        return ret;
    }
    if(sizeof(hmtx_t) < sizeof(hmtx_windows_t))
    {
        return ret;
    }
    hmtx_windows_t *mtx=((hmtx_windows_t *)__mutex);

    if(mtx->hMutex!=NULL && ReleaseMutex(mtx->hMutex))
    {
        ret=hthrd_success;
    }

    return ret;
}



void hmtx_windows_destroy(hmtx_t *__mutex )
{
    if(__mutex==NULL)
    {
        return;
    }
    if(sizeof(hmtx_t) < sizeof(hmtx_windows_t))
    {
        return;
    }

    hmtx_windows_t *mtx=((hmtx_windows_t *)__mutex);

    if(mtx->hMutex!=NULL)
    {
        CloseHandle(mtx->hMutex);
    }
}
