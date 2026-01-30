/***************************************************************
 * Name:      hmtx_stdatomic.c
 * Purpose:   实现hmtx_stdatomic接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-01-30
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"
#include "hmtx.h"

struct hmtx_stdatomic
{
    hthrd_t         owner;
    hatomic_int_t   lock;
    int             type;
};

typedef struct hmtx_stdatomic  hmtx_stdatomic_t;


int hmtx_stdatomic_init(hmtx_t* __mutex, int type)
{
    int ret=hthrd_error;
    if(__mutex==NULL)
    {
        return ret;
    }
    if(sizeof(hmtx_t) < sizeof(hmtx_stdatomic_t))
    {
        return ret;
    }
    hmtx_stdatomic_t *mtx=((hmtx_stdatomic_t *)__mutex);

    hmemset(mtx,0,sizeof(hmtx_stdatomic_t));


    hatomic_int_init(&mtx->lock,0);

    mtx->type=type;

    ret=hthrd_success;

    return ret;
}

int hmtx_stdatomic_trylock(hmtx_t *__mutex);

int hmtx_stdatomic_lock( hmtx_t* __mutex)
{
    int ret=hthrd_error;
    if(__mutex==NULL)
    {
        return ret;
    }
    if(sizeof(hmtx_t) < sizeof(hmtx_stdatomic_t))
    {
        return ret;
    }

    hmtx_stdatomic_t *mtx=((hmtx_stdatomic_t *)__mutex);

    bool wait_for_lock=true;
    while(wait_for_lock)
    {
        switch(hmtx_stdatomic_trylock(__mutex))
        {
        case hthrd_success:
        {
            ret=hthrd_success;
            wait_for_lock=false;
        }
        break;
        case hthrd_busy:
        {
            hthrd_yield();
        }
        break;
        default:
        {
            ret=hthrd_error;
            wait_for_lock=false;
        }
        break;
        }
    }

    return ret;
}


int hmtx_stdatomic_timedlock(hmtx_t * __mutex,const htimespec_t * time_point)
{
    int ret=hthrd_error;
    if(__mutex==NULL)
    {
        return ret;
    }
    if(sizeof(hmtx_t) < sizeof(hmtx_stdatomic_t))
    {
        return ret;
    }

    hmtx_stdatomic_t *mtx=((hmtx_stdatomic_t *)__mutex);

    bool wait_for_lock=true;
    while(wait_for_lock)
    {
        switch(hmtx_stdatomic_trylock(__mutex))
        {
        case hthrd_success:
        {
            ret=hthrd_success;
            wait_for_lock=false;
        }
        break;
        case hthrd_busy:
        {
            hthrd_yield();
            if(time_point!=NULL)
            {
                htimespec_t cur= {0};
                hclock_gettime(HCLOCK_REALTIME,&cur);
                if(cur.tv_sec >= time_point->tv_sec && cur.tv_nsec >= time_point->tv_nsec)
                {
                    ret=hthrd_timedout;
                    wait_for_lock=false;
                    break;
                }
            }
            else
            {
                /*
                 * 直接超时
                 */
                ret=hthrd_timedout;
                wait_for_lock=false;
                break;
            }
        }
        break;
        default:
        {
            ret=hthrd_error;
            wait_for_lock=false;
        }
        break;
        }
    }

    return ret;
}


int hmtx_stdatomic_trylock(hmtx_t *__mutex)
{
    int ret=hthrd_error;
    if(__mutex==NULL)
    {
        return ret;
    }
    if(sizeof(hmtx_t) < sizeof(hmtx_stdatomic_t))
    {
        return ret;
    }

    hmtx_stdatomic_t *mtx=((hmtx_stdatomic_t *)__mutex);

    int value=0;
    if(!hatomic_int_compare_exchange_strong(&mtx->lock,&value,1))
    {
        if((mtx->type&hmtx_recursive)!=0)
        {
            if(hthrd_equal(mtx->owner,hthrd_current()))
            {
                hatomic_int_fetch_add(&mtx->lock,1);
                ret=hthrd_success;
            }
            else
            {
                ret=hthrd_busy;
            }
        }
        else
        {
            ret=hthrd_busy;
        }
    }
    else
    {
        if((mtx->type&hmtx_recursive)!=0)
        {
            /*
             * 递归锁要设置所有者
             */
            mtx->owner=hthrd_current();
        }
        ret=hthrd_success;
    }





    return ret;
}


int hmtx_stdatomic_unlock( hmtx_t *__mutex)
{
    int ret=hthrd_error;
    if(__mutex==NULL)
    {
        return ret;
    }
    if(sizeof(hmtx_t) < sizeof(hmtx_stdatomic_t))
    {
        return ret;
    }
    hmtx_stdatomic_t *mtx=((hmtx_stdatomic_t *)__mutex);

    int value=0;
    if(hatomic_int_compare_exchange_strong(&mtx->lock,&value,value))
    {
        /*
         * 未加锁,直接报错
         */
        return ret;
    }

    value=1;
    if(hatomic_int_compare_exchange_strong(&mtx->lock,&value,value))
    {
        /*
         * 解锁
         */
        if((mtx->type&hmtx_recursive)!=0)
        {
            if(hthrd_equal(mtx->owner,hthrd_current()))
            {
                hatomic_int_fetch_sub(&mtx->lock,1);
                ret=hthrd_success;
            }
            else
            {
                /*
                 * 递归锁中只能当前线程解锁
                 */
                return ret;
            }
        }
        else
        {
            hatomic_int_fetch_sub(&mtx->lock,1);
            ret=hthrd_success;
        }
    }
    else
    {
        if((mtx->type&hmtx_recursive)!=0)
        {
            if(hthrd_equal(mtx->owner,hthrd_current()))
            {
                hatomic_int_fetch_sub(&mtx->lock,1);
                ret=hthrd_success;
            }
            else
            {
                /*
                 * 递归锁中只能当前线程解锁
                 */
                return ret;
            }
        }
        else
        {
            /*
             * 非递归锁只应当有0与1两个值
             */
            return ret;
        }
    }

    return ret;
}



void hmtx_stdatomic_destroy(hmtx_t *__mutex )
{
    if(__mutex==NULL)
    {
        return;
    }
    if(sizeof(hmtx_t) < sizeof(hmtx_stdatomic_t))
    {
        return;
    }
}
