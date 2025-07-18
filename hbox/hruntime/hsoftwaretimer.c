/***************************************************************
 * Name:      hsoftwaretimer.c
 * Purpose:   实现hsoftwaretimer接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-07-18
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hsoftwaretimer.h"
#include "hmemory.h"

static const hsoftwaretimer_lock_t *hsoftwaretimer_global_lock=NULL;

void hsoftwaretimer_lock_set(const hsoftwaretimer_lock_t *lock)
{
    hsoftwaretimer_global_lock=lock;
}

static void hsoftwaretimer_lock(void)
{
    const hsoftwaretimer_lock_t *lock=hsoftwaretimer_global_lock;
    if(lock!=NULL)
    {
        lock->mutex_lock(lock->usr);
    }
    else
    {
        hdefaults_mutex_lock(NULL);
    }
}

static void hsoftwaretimer_unlock(void)
{
    const hsoftwaretimer_lock_t *lock=hsoftwaretimer_global_lock;
    if(lock!=NULL)
    {
        lock->mutex_unlock(lock->usr);
    }
    else
    {
        hdefaults_mutex_unlock(NULL);
    }
}

void hsoftwaretimer_loop_hruntime(void)
{
#ifndef HSOFTWARETIMER_CUSTOM_LOOP
    hsoftwaretimer_mainloop();
#endif // HSOFTWARETIMER_CUSTOM_LOOP
}


static hsoftwaretimer_timer_t hsoftwaretimer_default_timer= {0};
static void hsoftwaretimer_default_timer_init(void)
{
    if(hsoftwaretimer_timer_check_flags(&hsoftwaretimer_default_timer,HSOFTWARETIMER_FLAG_INIT))
    {
        return;
    }
    hsoftwaretimer_lock();
    hdoublylinkedlist_init(&hsoftwaretimer_default_timer.node);
    hsoftwaretimer_default_timer.flags|=HSOFTWARETIMER_FLAG_INIT;
    hsoftwaretimer_unlock();
}

void hsoftwaretimer_mainloop(void)
{
    hsoftwaretimer_default_timer_init();
    hdefaults_tick_t current_tick=hdefaults_tick_get();
    hsoftwaretimer_lock();
    {
        //删除已被删除的定时器
        bool all_delete_timer_gc=false;
        while(!all_delete_timer_gc)
        {
            all_delete_timer_gc=true;
            hsoftwaretimer_timer_t *timer=NULL;
            HDOUBLYLINKEDLIST_FOREACH(&hsoftwaretimer_default_timer.node,timer_node)
            {
               timer=(hsoftwaretimer_timer_t *)GET_STRUCT_PTR_BY_MEMBER_PTR(timer_node,hsoftwaretimer_timer_t,node);
               if(hsoftwaretimer_timer_check_flags(timer,HSOFTWARETIMER_FLAG_DELETE))
               {
                   all_delete_timer_gc=false;
                   break;
               }
            }
            if(!all_delete_timer_gc)
            {
                if(timer!=NULL && timer!=&hsoftwaretimer_default_timer)
                {
                    /*
                     * 从定时器链表中移除
                     */
                    hdoublylinkedlist_remove(&timer->node);
                    hfree(timer);
                }
            }
        }
    }
    HDOUBLYLINKEDLIST_FOREACH(&hsoftwaretimer_default_timer.node,timer_node)
    {
        hsoftwaretimer_timer_t *timer=(hsoftwaretimer_timer_t *)GET_STRUCT_PTR_BY_MEMBER_PTR(timer_node,hsoftwaretimer_timer_t,node);
        if(hsoftwaretimer_timer_is_running(timer))
        {
            if(current_tick-timer->timeout < (1ULL << (sizeof(hdefaults_tick_t)*8-1)))
            {
                if(timer->timeout_cb!=NULL)
                {
                    timer->timeout_cb(timer);
                }
                if(hsoftwaretimer_timer_is_periodic(timer))
                {
                    timer->timeout+=timer->interval;
                }
                else
                {
                    hsoftwaretimer_timer_stop(timer);
                }
            }
        }
    }
    hsoftwaretimer_unlock();
}

bool hsoftwaretimer_timer_check_flags(const hsoftwaretimer_timer_t * timer,int flags)
{
    bool ret=false;
    if(timer!=NULL)
    {
        hsoftwaretimer_lock();
        ret=(flags==(timer->flags&flags));
        hsoftwaretimer_unlock();
    }
    return ret;
}

bool hsoftwaretimer_timer_is_running(const hsoftwaretimer_timer_t *timer)
{
    return hsoftwaretimer_timer_check_flags(timer,HSOFTWARETIMER_FLAG_START);
}

void hsoftwaretimer_timer_start(hsoftwaretimer_timer_t *timer,hdefaults_tick_t interval)
{
    hsoftwaretimer_default_timer_init();
    if(!hsoftwaretimer_timer_check_flags(timer,HSOFTWARETIMER_FLAG_INIT))
    {
        /*
         * 未初始化的定时器不进行任何操作
         */
        return;
    }
    hsoftwaretimer_lock();
    timer->interval=interval;
    timer->start=hdefaults_tick_get();
    timer->timeout=timer->start;
    timer->timeout+=timer->interval;
    timer->flags|=HSOFTWARETIMER_FLAG_START;
    hsoftwaretimer_unlock();
}

void hsoftwaretimer_timer_stop(hsoftwaretimer_timer_t *timer)
{
    if(!hsoftwaretimer_timer_is_running(timer))
    {
        /*
         * 未运行定时器不进行任何操作
         */
        return;
    }
    hsoftwaretimer_lock();
    timer->flags&= (~((uint32_t)HSOFTWARETIMER_FLAG_START));
    hsoftwaretimer_unlock();
}

bool hsoftwaretimer_timer_is_periodic(const hsoftwaretimer_timer_t *timer)
{
    return hsoftwaretimer_timer_check_flags(timer,HSOFTWARETIMER_FLAG_PERIODIC);
}

void hsoftwaretimer_timer_set_periodic(hsoftwaretimer_timer_t *timer,bool is_periodic)
{
    hsoftwaretimer_default_timer_init();
    if(is_periodic)
    {
        if(!hsoftwaretimer_timer_is_periodic(timer))
        {
            hsoftwaretimer_lock();
            timer->flags|=HSOFTWARETIMER_FLAG_PERIODIC;
            hsoftwaretimer_unlock();
        }
    }
    else
    {
        if(hsoftwaretimer_timer_is_periodic(timer))
        {
            hsoftwaretimer_lock();
            timer->flags&=(~((uint32_t)HSOFTWARETIMER_FLAG_PERIODIC));
            hsoftwaretimer_unlock();
        }
    }
}

void hsoftwaretimer_timer_set_timeout_callback(hsoftwaretimer_timer_t *timer,hsoftwaretimer_timer_callback_t callback,void *usr)
{
    hsoftwaretimer_default_timer_init();
    if(!hsoftwaretimer_timer_check_flags(timer,HSOFTWARETIMER_FLAG_INIT))
    {
        /*
         * 未初始化的定时器不进行任何操作
         */
        return;
    }
    hsoftwaretimer_lock();
    timer->timeout_cb=callback;
    timer->usr=usr;
    hsoftwaretimer_unlock();
}

hsoftwaretimer_timer_t *hsoftwaretimer_timer_get_default_timer(void)
{
    return &hsoftwaretimer_default_timer;
}

hsoftwaretimer_timer_t *hsoftwaretimer_timer_new(void)
{

    hsoftwaretimer_default_timer_init();
    hsoftwaretimer_timer_t *timer=hmalloc(sizeof(hsoftwaretimer_timer_t));
    if(timer!=NULL)
    {
        memset(timer,0,sizeof(*timer));
        hsoftwaretimer_lock();
        hdoublylinkedlist_init(&timer->node);
        timer->flags|=HSOFTWARETIMER_FLAG_INIT;
        /*
         * 添加至定时器链表（双向循环）
         */
        hdoublylinkedlist_add_front(&hsoftwaretimer_default_timer.node,&timer->node);
        hsoftwaretimer_unlock();
    }
    return timer;
}

void hsoftwaretimer_timer_delete(hsoftwaretimer_timer_t * timer)
{
    if(!hsoftwaretimer_timer_check_flags(timer,HSOFTWARETIMER_FLAG_INIT))
    {
        /*
         * 未初始化的定时器不进行任何操作
         */
        return;
    }

    if(timer==&hsoftwaretimer_default_timer)
    {
        /*
         * 默认定时器无法删除,以停止代替删除
         */
        hsoftwaretimer_timer_stop(timer);
        return;
    }

    hsoftwaretimer_timer_stop(timer);
    hsoftwaretimer_lock();
    timer->flags|=HSOFTWARETIMER_FLAG_DELETE;
    hsoftwaretimer_unlock();

}

