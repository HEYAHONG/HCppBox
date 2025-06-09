/***************************************************************
 * Name:      hstacklesscoroutine_v2.c
 * Purpose:   实现hstacklesscoroutine_v2接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-04-04
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hstacklesscoroutine_v2.h"
#include "hdefaults.h"
#include "hcompiler.h"
#include "stdlib.h"
#include "setjmp.h"
#include "string.h"




struct hstacklesscoroutine2_ccb
{
    hstacklesscoroutine2_ccb_t *next;
    hstacklesscoroutine2_ccb_t *prev;
    hstacklesscoroutine2_task_t task;           /**< 任务信息 */
    jmp_buf scheduler_point;                    /**< 调度器保存点 */
    int scheduler_point_status;                 /**< setjmp返回值 */
#ifdef HSTACKLESSCOROUTINE2_BARE_MACHINE
    jmp_buf coroutine_point;
    int coroutine_point_status;
#endif
    struct
    {
        uint32_t running_state:3;               /**< 运行状态，见hstacklesscoroutine2_running_state_t */
        uint32_t suspend:1;                     /**< 是否被暂停，0=未被暂停，1=被暂停 */
        uint32_t delay:1;                       /**< 正在被延时，0=未被延时，1=被延时 */
        uint32_t await:1;                       /**< 正在等待，0=未等待，1=等待 */
    } state;
    struct
    {
        hdefaults_tick_t next_tick;             /**< 下次运行的节拍，当协程处于阻塞状态时，当前时钟节拍大于此值时才会唤醒 */
        hstacklesscoroutine2_awaiter_t awaiter; /**< 挂起点 */
    } block;
    uintptr_t stack_top;                        /**< 栈顶，由调度器启动协程时设定 */
#ifdef HSTACKLESSCOROUTINE2_BARE_MACHINE
    uintptr_t stack_bottom;                     /**< 栈底，由协程让出控制权时设定 */
    size_t  stack_length;
    uint8_t stack[128];                         /**< 预先分配128字节的栈空间.注意：此空间不是运行时的栈空间，而是用于在让出控制权时备份相应栈到此空间，在回复控制权时从此空间还原栈。 */
#endif
};

size_t hstacklesscoroutine2_ccb_size(void)
{
    return sizeof(hstacklesscoroutine2_ccb_t);
}

hstacklesscoroutine2_ccb_t *hstacklesscoroutine2_ccb_init(void *mem,size_t length)
{
    if(mem==NULL || length < hstacklesscoroutine2_ccb_size())
    {
        return NULL;
    }
    hstacklesscoroutine2_ccb_t *ret=(hstacklesscoroutine2_ccb_t *)mem;
    memset(mem,0,hstacklesscoroutine2_ccb_size());
    ret->state.running_state=HSTACKLESSCOROUTINE2_RUNNING_STATE_CREATE;
#ifdef HSTACKLESSCOROUTINE2_BARE_MACHINE
    ret->stack_length=length-hstacklesscoroutine2_ccb_size()+sizeof(ret->stack);
#endif
    return ret;
}

hstacklesscoroutine2_task_t hstacklesscoroutine2_task_init(hstacklesscoroutine2_task_entry_t entry,void *usr)
{
    hstacklesscoroutine2_task_t task= {0};
    task.entry=entry;
    task.usr=usr;
#ifndef HSTACKLESSCOROUTINE2_BARE_MACHINE
    task.core_value=0;
#endif // HSTACKLESSCOROUTINE2_BARE_MACHINE
    return task;
}

bool hstacklesscoroutine2_ccb_set(hstacklesscoroutine2_ccb_t *ccb,hstacklesscoroutine2_task_t task)
{
    if(ccb==NULL || task.entry == NULL)
    {
        return false;
    }
    if(hstacklesscoroutine2_ccb_running_state_get(ccb) == HSTACKLESSCOROUTINE2_RUNNING_STATE_CREATE || hstacklesscoroutine2_ccb_running_state_get(ccb) == HSTACKLESSCOROUTINE2_RUNNING_STATE_STOPPED)
    {
        ccb->task=task;
        ccb->state.running_state=HSTACKLESSCOROUTINE2_RUNNING_STATE_CREATE;
    }
    return false;
}

hstacklesscoroutine2_task_t hstacklesscoroutine2_ccb_get(hstacklesscoroutine2_ccb_t *ccb)
{
    hstacklesscoroutine2_task_t ret= {0};
    if(ccb==NULL)
    {
        return ret;
    }
    return ccb->task;
}

bool hstacklesscoroutine2_ccb_is_suspend(hstacklesscoroutine2_ccb_t *ccb)
{
    bool ret=false;
    if(ccb!=NULL)
    {
        ret=ccb->state.suspend!=0;
    }
    return ret;
}

void hstacklesscoroutine2_ccb_suspend(hstacklesscoroutine2_ccb_t *ccb)
{
    if(ccb!=NULL)
    {
        ccb->state.suspend=1;
    }
}

void hstacklesscoroutine2_ccb_resume(hstacklesscoroutine2_ccb_t *ccb)
{
    if(ccb!=NULL)
    {
        ccb->state.suspend=0;
    }
}

hstacklesscoroutine2_running_state_t hstacklesscoroutine2_ccb_running_state_get(hstacklesscoroutine2_ccb_t *ccb)
{
    if(ccb==NULL)
    {
        return HSTACKLESSCOROUTINE2_RUNNING_STATE_UNKOWN;
    }
    return (hstacklesscoroutine2_running_state_t)ccb->state.running_state;
}

static hstacklesscoroutine2_scheduler_t hstacklesscoroutine2_default_scheduler= {0};
static hstacklesscoroutine2_scheduler_t *hstacklesscoroutine2_scheduler_check(hstacklesscoroutine2_scheduler_t * scheduler)
{
    if(scheduler==NULL)
    {
        scheduler=&hstacklesscoroutine2_default_scheduler;
    }
    return scheduler;
}

int hstacklesscoroutine2_scheduler_start(hstacklesscoroutine2_scheduler_t * scheduler)
{
    hstacklesscoroutine2_scheduler_t * sch=hstacklesscoroutine2_scheduler_check(scheduler);
    if(sch->ccb_list_head==NULL)
    {
        return -1;
    }
    bool all_task_finished=false;
    hstacklesscoroutine2_ccb_t *  current_ccb=NULL;
    size_t ccb_unfinished=1;//默认至少有一个任务未完成
    /*
     * 栈顶后的变量可能会被覆盖
     */
    int8_t stack_top=0;
    while(!all_task_finished)
    {
        //遍历下一个协程控制块
        if(current_ccb!=NULL)
        {
            current_ccb=current_ccb->next;
        }
        if(current_ccb==NULL)
        {
            current_ccb=sch->ccb_list_head;
            if(ccb_unfinished==0)
            {
                current_ccb=NULL;
                all_task_finished=true;
            }
            ccb_unfinished=0;
        }


        if(current_ccb==NULL)
        {
            break;
        }
        if(hstacklesscoroutine2_ccb_running_state_get(current_ccb)!=HSTACKLESSCOROUTINE2_RUNNING_STATE_STOPPED)
        {
            ccb_unfinished++;
        }

        if(hstacklesscoroutine2_ccb_running_state_get(current_ccb)==HSTACKLESSCOROUTINE2_RUNNING_STATE_BLOCKED)
        {
            //处理阻塞
            bool be_ready=true;
            if(current_ccb->state.delay!=0)
            {
                hdefaults_tick_t current_tick=hdefaults_get_api_table()->tick_get();
                if(current_ccb->block.next_tick > current_tick)
                {
                    be_ready=false;
                }
                if((current_ccb->block.next_tick < current_tick) && ((current_tick - current_ccb->block.next_tick) > (1ULL << (sizeof(hdefaults_tick_t)*8-1)))) //默认超时不能大过hdefaults_tick_t大小的1/2
                {
                    be_ready=false;
                }

                if(be_ready)
                {
                    current_ccb->state.delay=0;
                }
            }

            if(current_ccb->state.suspend!=0)
            {
                be_ready=false;
            }

            if(be_ready && current_ccb->state.await!=0)
            {
                if(current_ccb->block.awaiter.wait_for_ready!=NULL)
                {
                    be_ready=current_ccb->block.awaiter.wait_for_ready(sch,current_ccb,&current_ccb->block.awaiter);
                }

                if(be_ready)
                {
                    current_ccb->state.await=0;
                }
            }

            if(be_ready)
            {
                current_ccb->state.running_state=HSTACKLESSCOROUTINE2_RUNNING_STATE_READY;
            }
        }

        if(hstacklesscoroutine2_ccb_running_state_get(current_ccb)==HSTACKLESSCOROUTINE2_RUNNING_STATE_READY)
        {
            if(current_ccb->state.suspend!=0)
            {
                current_ccb->state.running_state=HSTACKLESSCOROUTINE2_RUNNING_STATE_BLOCKED;
            }
        }

        if(hstacklesscoroutine2_ccb_running_state_get(current_ccb)==HSTACKLESSCOROUTINE2_RUNNING_STATE_READY)
        {
            /*
             * 为保持栈顶一致，stack_top变量后不应有任何局部变量
             */
            stack_top=0;//此变量置1表示从 协程中返回,-1表示出错
            current_ccb->stack_top=(uintptr_t)&stack_top;
            current_ccb->scheduler_point_status=setjmp(current_ccb->scheduler_point);
#ifdef HSTACKLESSCOROUTINE2_BARE_MACHINE
            if(stack_top==0)
            {
                current_ccb->state.running_state=HSTACKLESSCOROUTINE2_RUNNING_STATE_RUNNING;
                if(current_ccb->stack_bottom!=0)
                {
                    //栈底置CCB指针,提示已从保存的点恢复
                    (*(uintptr_t*)current_ccb->stack_bottom)=(uintptr_t)current_ccb;
                }
                longjmp(current_ccb->coroutine_point,current_ccb->coroutine_point_status);//从保存的点恢复
            }
            if(stack_top==1)
            {
                if(hstacklesscoroutine2_ccb_running_state_get(current_ccb)==HSTACKLESSCOROUTINE2_RUNNING_STATE_RUNNING)
                {
                    current_ccb->state.running_state=HSTACKLESSCOROUTINE2_RUNNING_STATE_READY;
                }
            }
            if(stack_top==-1)
            {
                return -1;
            }
#else
            if(stack_top==0)
            {
                if(current_ccb->task.entry!=NULL)
                {
                    current_ccb->state.running_state=HSTACKLESSCOROUTINE2_RUNNING_STATE_RUNNING;
                    current_ccb->task.entry(sch,current_ccb,current_ccb->task.usr);
                    current_ccb->state.running_state=HSTACKLESSCOROUTINE2_RUNNING_STATE_STOPPED;
                }
            }
            if(stack_top==1)
            {
                if(hstacklesscoroutine2_ccb_running_state_get(current_ccb)==HSTACKLESSCOROUTINE2_RUNNING_STATE_RUNNING)
                {
                    current_ccb->state.running_state=HSTACKLESSCOROUTINE2_RUNNING_STATE_READY;
                }
            }
            if(stack_top==-1)
            {
                return -1;
            }
#endif
        }


        if(hstacklesscoroutine2_ccb_running_state_get(current_ccb)==HSTACKLESSCOROUTINE2_RUNNING_STATE_CREATE)
        {
            /*
             * 为保持栈顶一致，stack_top变量后不应有任何局部变量
             */
            stack_top=0;//此变量置1表示从 协程中返回,-1表示出错
            current_ccb->stack_top=(uintptr_t)&stack_top;
            current_ccb->scheduler_point_status=setjmp(current_ccb->scheduler_point);
            if(stack_top==0)
            {
                if(current_ccb->task.entry!=NULL && current_ccb->state.suspend == 0)
                {
                    current_ccb->state.running_state=HSTACKLESSCOROUTINE2_RUNNING_STATE_RUNNING;
                    current_ccb->task.entry(sch,current_ccb,current_ccb->task.usr);
                    current_ccb->state.running_state=HSTACKLESSCOROUTINE2_RUNNING_STATE_STOPPED;
                }
            }
            if(stack_top==1)
            {
                if(hstacklesscoroutine2_ccb_running_state_get(current_ccb)==HSTACKLESSCOROUTINE2_RUNNING_STATE_RUNNING)
                {
                    current_ccb->state.running_state=HSTACKLESSCOROUTINE2_RUNNING_STATE_READY;
                }
            }
            if(stack_top==-1)
            {
                return -1;
            }
        }


    }
    return 0;
}

int hstacklesscoroutine2_scheduler_ccb_register(hstacklesscoroutine2_scheduler_t * scheduler,hstacklesscoroutine2_ccb_t *ccb)
{
    hstacklesscoroutine2_scheduler_t * sch=hstacklesscoroutine2_scheduler_check(scheduler);
    if(ccb==NULL || ccb->next!=NULL || ccb->prev!=NULL)
    {
        //协程控制块无效或者已被使用
        return -1;
    }
    if(sch->ccb_list_head==NULL)
    {
        sch->ccb_list_head=ccb;
        ccb->prev=ccb;
    }
    else
    {
        hstacklesscoroutine2_ccb_t *current_ccb=sch->ccb_list_head;
        while(current_ccb->next!=NULL)
        {
            current_ccb=current_ccb->next;
        }
        current_ccb->next=ccb;
        ccb->prev=current_ccb;
    }
    return 0;
}

void hstacklesscoroutine2_yield(hstacklesscoroutine2_scheduler_t * sch,hstacklesscoroutine2_ccb_t *ccb)
{
    if(sch == NULL || ccb==NULL || hstacklesscoroutine2_ccb_running_state_get(ccb)==HSTACKLESSCOROUTINE2_RUNNING_STATE_CREATE)
    {
        return;
    }
#ifdef HSTACKLESSCOROUTINE2_BARE_MACHINE
    uintptr_t stack_bottom=0;//栈底非0表示从调度器返回(此时表示现在CCB指针)
    ccb->stack_bottom=(uintptr_t)&stack_bottom;
    int point_status=setjmp(ccb->coroutine_point);
    if(stack_bottom==0)
    {
        ccb->coroutine_point_status=point_status;
        if(hstacklesscoroutine2_ccb_running_state_get(ccb)==HSTACKLESSCOROUTINE2_RUNNING_STATE_RUNNING)
        {
            ccb->state.running_state=HSTACKLESSCOROUTINE2_RUNNING_STATE_READY;
        }
        uintptr_t stack_base=ccb->stack_bottom;
        if(ccb->stack_top < stack_base)
        {
            stack_base=ccb->stack_top;
        }
        uintptr_t stack_limit=ccb->stack_top;
        if(ccb->stack_bottom > stack_limit)
        {
            stack_limit=ccb->stack_bottom;
        }
        if(ccb->stack_length < (stack_limit-stack_base))
        {
            //备份栈的空间不够
            if(ccb->stack_top!=0)
            {
                //栈顶置-1
                (*(int8_t*)ccb->stack_top)=-1;
            }
            longjmp(ccb->scheduler_point,ccb->scheduler_point_status); //返回调度器
        }
        //备份栈
        memcpy((void *)ccb->stack,(void *)stack_base,stack_limit-stack_base);
        if(ccb->stack_top!=0)
        {
            //栈顶置1
            (*(int8_t*)ccb->stack_top)=1;
        }
        longjmp(ccb->scheduler_point,ccb->scheduler_point_status); //返回调度器
    }
    else
    {
        ccb=(hstacklesscoroutine2_ccb_t *)stack_bottom;
        uintptr_t stack_base=ccb->stack_bottom;
        if(ccb->stack_top < stack_base)
        {
            stack_base=ccb->stack_top;
        }
        uintptr_t stack_limit=ccb->stack_top;
        if(ccb->stack_bottom > stack_limit)
        {
            stack_limit=ccb->stack_bottom;
        }
        //恢复栈
        memcpy((void *)stack_base,(void *)ccb->stack,stack_limit-stack_base);
    }
#else
    if(hstacklesscoroutine2_ccb_running_state_get(ccb)==HSTACKLESSCOROUTINE2_RUNNING_STATE_RUNNING)
    {
        ccb->state.running_state=HSTACKLESSCOROUTINE2_RUNNING_STATE_READY;
    }
    if(ccb->stack_top!=0)
    {
        //栈顶置1
        (*(int8_t*)ccb->stack_top)=1;
    }
    longjmp(ccb->scheduler_point,ccb->scheduler_point_status); //返回调度器
#endif
}

void hstacklesscoroutine2_delay_util(hstacklesscoroutine2_scheduler_t * sch,hstacklesscoroutine2_ccb_t *ccb,hdefaults_tick_t tick)
{
    if(sch == NULL || ccb==NULL || hstacklesscoroutine2_ccb_running_state_get(ccb)==HSTACKLESSCOROUTINE2_RUNNING_STATE_CREATE)
    {
        return;
    }
    ccb->state.running_state=HSTACKLESSCOROUTINE2_RUNNING_STATE_BLOCKED;
    ccb->block.next_tick=tick;
    ccb->state.delay=1;
    hstacklesscoroutine2_yield(sch,ccb);
}

void hstacklesscoroutine2_delay(hstacklesscoroutine2_scheduler_t * sch,hstacklesscoroutine2_ccb_t *ccb,hdefaults_tick_t  tick)
{
    hstacklesscoroutine2_delay_util(sch,ccb,tick+hdefaults_get_api_table()->tick_get());
}

hstacklesscoroutine2_awaiter_t hstacklesscoroutine_awaiter2_init(hstacklesscoroutine2_awaiter_wait_for_ready_t wait_for_ready,void *usr)
{
    hstacklesscoroutine2_awaiter_t awaiter= {0};
    awaiter.wait_for_ready=wait_for_ready;
    awaiter.usr=usr;
    return awaiter;
}

void hstacklesscoroutine2_await(hstacklesscoroutine2_scheduler_t * sch,hstacklesscoroutine2_ccb_t *ccb,hstacklesscoroutine2_awaiter_t awaiter)
{
    if(sch == NULL || ccb==NULL || hstacklesscoroutine2_ccb_running_state_get(ccb)==HSTACKLESSCOROUTINE2_RUNNING_STATE_CREATE)
    {
        return;
    }
    ccb->state.running_state=HSTACKLESSCOROUTINE2_RUNNING_STATE_BLOCKED;
    ccb->state.await=1;
    ccb->block.awaiter=awaiter;
    hstacklesscoroutine2_yield(sch,ccb);
}

#ifndef HSTACKLESSCOROUTINE2_BARE_MACHINE

int __hstacklesscoroutine2_core_value_get(hstacklesscoroutine2_ccb_t *ccb)
{
    if(ccb!=NULL)
    {
        return ccb->task.core_value;
    }
    return 0;
}
void __hstacklesscoroutine2_core_value_set(hstacklesscoroutine2_ccb_t *ccb,int core_value)
{
    if(ccb!=NULL)
    {
        ccb->task.core_value=core_value;
    }
}

#else

int __hstacklesscoroutine2_core_value_get(hstacklesscoroutine2_ccb_t *ccb)
{
    return 0;
}

void __hstacklesscoroutine2_core_value_set(hstacklesscoroutine2_ccb_t *ccb,int core_value)
{

}

#endif // HSTACKLESSCOROUTINE2_BARE_MACHINE
