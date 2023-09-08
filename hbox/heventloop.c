/***************************************************************
 * Name:      heventloop.c
 * Purpose:   实现heventloop接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2023-07-14
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#include "heventloop.h"
#include "hdefaults.h"

typedef struct heventloop_event
{
    //用户参数
    void *usr;
    //事件处理,参数为用户参数
    void(*event_process)(void *);
    //事件释放,参数为用户参数
    void(*event_onfree)(void *);
    //下一个事件指针
    struct heventloop_event *next;
} heventloop_event_t;

struct heventloop
{
    //用户参数
    void *usr;
    //获取内存,第一个参数为待获取的内存大小,第二个参数为用户参数。
    void *(*mem_alloc)(size_t,void *);
    //释放内存,第一个参数为待释放的内存指针,第二个参数为用户参数。
    void (*mem_free)(void *,void *);
    //锁,参数为用户参数。
    void (*mutex_lock)(void *);
    //解锁,参数为用户参数。
    void (*mutex_unlock)(void *);
    //事件单向链表起始指针
    heventloop_event_t *event_start;
    //事件数量
    uint32_t event_number;
    //最大事件数量,0表示无限制
    uint32_t max_event_number;
};

heventloop_t * heventloop_new_with_memmang_and_lock(void *usr,void *(*mem_alloc)(size_t,void *),void (*mem_free)(void *,void *),void (*mutex_lock)(void *),void (*mutex_unlock)(void *))
{
    struct heventloop *loop=NULL;
    if(mem_alloc!=NULL)
    {
        loop=(struct heventloop *)mem_alloc(sizeof(struct heventloop),usr);
    }
    else
    {
        loop=(struct heventloop *)malloc(sizeof(struct heventloop));
    }

    if(loop==NULL)
    {
        return NULL;
    }

    memset(loop,0,sizeof(struct heventloop));

    loop->usr=usr;
    loop->mem_alloc=mem_alloc;
    loop->mem_free=mem_free;
    loop->mutex_lock=mutex_lock;
    loop->mutex_unlock=mutex_unlock;
    loop->event_start=NULL;
    loop->event_number=0;
    loop->max_event_number=0;

    return loop;
}
heventloop_t * heventloop_new_with_memmang(void *usr,void *(*mem_alloc)(size_t,void *),void (*mem_free)(void *,void *))
{
    return heventloop_new_with_memmang_and_lock(usr,mem_alloc,mem_free,hdefaults_mutex_lock,hdefaults_mutex_unlock);
}
heventloop_t * heventloop_new_with_lock(void *usr,void (*mutex_lock)(void *),void (*mutex_unlock)(void *))
{
    return heventloop_new_with_memmang_and_lock(usr,hdefaults_malloc,hdefaults_free,mutex_lock,mutex_unlock);
}
heventloop_t * heventloop_new(void *usr)
{
    return heventloop_new_with_memmang_and_lock(usr,hdefaults_malloc,hdefaults_free,hdefaults_mutex_lock,hdefaults_mutex_unlock);

}

void * heventloop_get_usr_ptr(heventloop_t *loop)
{
    if(loop==NULL)
    {
        return NULL;
    }
    return loop->usr;
}

void heventloop_free(heventloop_t *loop)
{
    if(loop==NULL)
    {
        return;
    }

    //处理完所有事件
    while(heventloop_has_events(loop))
    {
        heventloop_process_event(loop);
    }

    if(loop->mem_free!=NULL)
    {
        loop->mem_free(loop,loop->usr);
    }
    else
    {
        free(loop);
    }
}

bool heventloop_has_events(heventloop_t *loop)
{
    if(loop!=NULL &&loop->event_start!=NULL)
    {
        return true;
    }
    return false;
}

void heventloop_process_event(heventloop_t *loop)
{
    if(loop==NULL)
    {
        return;
    }


    //处理事件
    while(heventloop_has_events(loop))
    {
        //加锁
        if(loop->mutex_lock!=NULL)
        {
            loop->mutex_lock(loop->usr);
        }

        heventloop_event_t *current=loop->event_start;
        loop->event_start=current->next;

        if(loop->event_number!=0)
        {
            loop->event_number--;
        }

        //释放锁
        if(loop->mutex_unlock!=NULL)
        {
            loop->mutex_unlock(loop->usr);
        }

        if(current!=NULL)
        {
            if(current->event_process!=NULL)
            {
                current->event_process(current->usr);
            }
            if(current->event_onfree!=NULL)
            {
                current->event_onfree(current->usr);
            }
            if(loop->mem_free!=NULL)
            {
                loop->mem_free(current,loop->usr);
            }
            else
            {
                free(current);
            }
        }
    }

}

bool heventloop_add_event(heventloop_t *loop,void *event_usr,void(*event_process)(void *),void(*event_onfree)(void *))
{
    if(loop==NULL)
    {
        return false;
    }

    if(loop->max_event_number!=0)
    {
        if(loop->max_event_number < loop->event_number)
        {
            return false;
        }
    }

    heventloop_event_t *event=NULL;
    if(loop->mem_alloc!=NULL)
    {
        event=(heventloop_event_t *)loop->mem_alloc(sizeof(heventloop_event_t),loop->usr);
    }
    else
    {
        event=(heventloop_event_t *)malloc(sizeof(heventloop_event_t));
    }

    if(event==NULL)
    {
        return false;
    }

    event->usr=event_usr;
    event->event_process=event_process;
    event->event_onfree=event_onfree;
    event->next=NULL;

    //加锁
    if(loop->mutex_lock!=NULL)
    {
        loop->mutex_lock(loop->usr);
    }

    heventloop_event_t *loop_event=loop->event_start;

    while(loop_event!=NULL)
    {
        if(loop_event->next==NULL)
        {
            loop_event->next=event;
            break;
        }
        loop_event=loop_event->next;
    }

    loop->event_number++;

    if(loop_event==NULL)
    {
        loop->event_start=event;
        loop->event_number=1;
    }


    //释放锁
    if(loop->mutex_unlock!=NULL)
    {
        loop->mutex_unlock(loop->usr);
    }

    return true;
}

typedef struct
{
    void * event_usr;
    void(*event_process)(void *,heventloop_t *);
    void(*event_onfree)(void *,heventloop_t *);
    heventloop_t *loop;

} heventloop_event_ex1_t;

static void heventloop_event_ex1_process(void * usr)
{
    if(usr==NULL)
    {
        return;
    }
    heventloop_event_ex1_t *event_ex1=(heventloop_event_ex1_t *)usr;
    if(event_ex1->event_process!=NULL)
    {
        event_ex1->event_process(event_ex1->event_usr,event_ex1->loop);
    }
}

static void heventloop_event_ex1_onfree(void * usr)
{
    if(usr==NULL)
    {
        return;
    }
    heventloop_event_ex1_t *event_ex1=(heventloop_event_ex1_t *)usr;
    if(event_ex1->event_onfree!=NULL)
    {
        event_ex1->event_onfree(event_ex1->event_usr,event_ex1->loop);
    }

    if(event_ex1->loop->mem_free!=NULL)
    {
        event_ex1->loop->mem_free(event_ex1,event_ex1->loop->usr);
    }
    else
    {
        free(event_ex1);
    }

}

bool heventloop_add_event_ex1(heventloop_t *loop,void *event_usr,void(*event_process)(void *,heventloop_t *),void(*event_onfree)(void *,heventloop_t *))
{
    if(loop==NULL)
    {
        return false;
    }

    heventloop_event_ex1_t *event_ex1=NULL;
    if(loop->mem_alloc!=NULL)
    {
        event_ex1=(heventloop_event_ex1_t *)loop->mem_alloc(sizeof(heventloop_event_ex1_t),loop->usr);
    }
    else
    {
        event_ex1=(heventloop_event_ex1_t *)malloc(sizeof(heventloop_event_ex1_t));
    }

    if(event_ex1==NULL)
    {
        return false;
    }

    event_ex1->event_usr=event_usr;
    event_ex1->event_process=event_process;
    event_ex1->event_onfree=event_onfree;
    event_ex1->loop=loop;

    if(!heventloop_add_event(loop,event_ex1,heventloop_event_ex1_process,heventloop_event_ex1_onfree))
    {
        if(loop->mem_free!=NULL)
        {
            loop->mem_free(event_ex1,loop->usr);
        }
        else
        {
            free(event_ex1);
        }

        return false;
    }

    return true;
}

uint32_t heventloop_get_events_number(heventloop_t *loop)
{
    if(loop==NULL)
    {
        return 0;
    }

    return loop->event_number;
}

uint32_t heventloop_get_max_events_number(heventloop_t *loop)
{
    if(loop==NULL)
    {
        return 0;
    }

    return loop->max_event_number;
}

void  heventloop_set_max_events_number(heventloop_t *loop,uint32_t max_event_number)
{
    if(loop==NULL)
    {
        return;
    }
    loop->max_event_number=max_event_number;
}
