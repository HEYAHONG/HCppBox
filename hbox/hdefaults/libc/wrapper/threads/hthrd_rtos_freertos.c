/***************************************************************
 * Name:      hthrd_freertos.c
 * Purpose:   实现hthrd_freertos接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-01-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"
#include "stdlib.h"
#include "time.h"

#if defined(FREERTOS_KERNEL)
/*
 * 由h3rdparty组件引入FreeRTOS代码
 */
#include "h3rdparty.h"
#include FREERTOS_KERNEL_FREERTOS_HEADER
#include FREERTOS_KERNEL_TASK_HEADER
#include FREERTOS_KERNEL_TIMERS_HEADER
#include FREERTOS_KERNEL_QUEUE_HEADER
#include FREERTOS_KERNEL_SEMPHR_HEADER
#include FREERTOS_KERNEL_CROUTINE_HEADER
#include FREERTOS_KERNEL_LIST_HEADER
#include FREERTOS_KERNEL_EVENT_GROUPS_HEADER
#else
#include “FreeRTOS.h”
#include “task.h”
#endif

#ifndef configSUPPORT_STATIC_ALLOCATION
#define configSUPPORT_STATIC_ALLOCATION 0
#endif // configSUPPORT_STATIC_ALLOCATION
#if (configSUPPORT_STATIC_ALLOCATION) < 1
#error configSUPPORT_STATIC_ALLOCATION must be 1
#endif

#ifndef INCLUDE_xTaskGetCurrentTaskHandle
#define INCLUDE_xTaskGetCurrentTaskHandle 0
#endif // INCLUDE_xTaskGetCurrentTaskHandle
#if (INCLUDE_xTaskGetCurrentTaskHandle) < 1
#error INCLUDE_xTaskGetCurrentTaskHandle must be 1
#endif

#ifndef configUSE_TIMERS
#define configUSE_TIMERS  0
#endif // configUSE_TIMERS
#if (configUSE_TIMERS) < 1
#error configUSE_TIMERS must be 1
#endif

#ifndef HTHRD_FREERTOS_STACK_SIZE
#define HTHRD_FREERTOS_STACK_SIZE  (2048)
#endif // HTHRD_FREERTOS_STACK_SIZE

#ifndef HTHRD_FREERTOS_PRIORITY
#define HTHRD_FREERTOS_PRIORITY    (1)
#endif // HTHRD_FREERTOS_PRIORITY

#ifndef HTHRD_FREERTOS_NAME
#define HTHRD_FREERTOS_NAME        "thrd"
#endif // HTHRD_FREERTOS_NAME

struct hthrd_freertos_tcb
{
    StaticTask_t tcb;
    StackType_t  stack[HTHRD_FREERTOS_STACK_SIZE/sizeof(StackType_t)];
    int exit_code;                  /**< 退出代码 */
    struct
    {
        unsigned thread_detach:1;   /**< detach标志 */
        unsigned thread_exit:1;     /**< exit标志 */
    } flags;
};

typedef struct hthrd_freertos_tcb hthrd_freertos_tcb_t;


typedef TaskHandle_t hthrd_freertos_t;

struct hthrd_freertos_create_context
{
    hthrd_start_t func;
    void *arg;
};
typedef struct hthrd_freertos_create_context hthrd_freertos_create_context_t;

void hthrd_freertos_exit(int res);
static void hthrd_freertos_thread_proc(void *th_arg)
{
    hthrd_freertos_create_context_t *ctx=(hthrd_freertos_create_context_t*)th_arg;
    if(ctx==NULL)
    {
        hthrd_freertos_exit(-1);
    }

    hthrd_start_t   func=ctx->func;
    void *          arg=ctx->arg;

    hfree(ctx);

    if(func!=NULL)
    {
        hthrd_freertos_exit(func(arg));
    }

    hthrd_freertos_exit(-1);

}

int hthrd_freertos_detach(hthrd_t thr);
int hthrd_freertos_create(hthrd_t *thr,hthrd_start_t func,void *arg )
{
    int ret=hthrd_error;
    if(sizeof(hthrd_t) < sizeof(hthrd_freertos_t))
    {
        return ret;
    }
    if(func==NULL)
    {
        return ret;
    }
    hthrd_freertos_create_context_t *ctx=(hthrd_freertos_create_context_t *)hmalloc(sizeof(hthrd_freertos_create_context_t));
    if(ctx==NULL)
    {
        return ret;
    }

    ctx->func=func;
    ctx->arg=arg;

    hthrd_freertos_tcb_t *tcb=(hthrd_freertos_tcb_t *)hmalloc(sizeof(hthrd_freertos_tcb_t));

    if(tcb==NULL)
    {
        hfree(ctx);
        ret=hthrd_nomem;
        return ret;
    }

    hmemset(tcb,0,sizeof(hthrd_freertos_tcb_t));

    if(NULL!=xTaskCreateStatic(hthrd_freertos_thread_proc,HTHRD_FREERTOS_NAME,sizeof(tcb->stack)/sizeof(tcb->stack[0]),ctx,HTHRD_FREERTOS_PRIORITY,tcb->stack,&tcb->tcb))
    {
        ret=hthrd_success;
        if(thr==NULL)
        {
            /*
             * 若未使用，默认为分离线程
             */
            tcb->flags.thread_detach=1;
        }
    }
    else
    {
        hfree(ctx);
        hfree(tcb);
    }

    return ret;
}


int hthrd_freertos_equal(hthrd_t lhs,hthrd_t rhs )
{
    int ret=-1;

    if((*(hthrd_freertos_t *)&lhs)==(*(hthrd_freertos_t *)&rhs))
    {
        ret=0;
    }

    return ret;
}


hthrd_t hthrd_freertos_current(void)
{
    hthrd_t ret= {0};

    if(sizeof(hthrd_t) < sizeof(hthrd_freertos_t))
    {
        return ret;
    }

    (*(hthrd_freertos_t *)&ret)=xTaskGetCurrentTaskHandle();

    return ret;
}


int hthrd_freertos_sleep(const htimespec_t* duration,htimespec_t * remaining)
{
    int ret=0;

    htimespec_t cur= {0};
    if(duration!=NULL)
    {
        cur=(*duration);
    }

    while(cur.tv_sec--)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    cur.tv_sec++;

    while(cur.tv_nsec > (portTICK_PERIOD_MS*1000000))
    {
        vTaskDelay(1);
        cur.tv_nsec-=(portTICK_PERIOD_MS*1000000);
    }

    if(remaining!=NULL)
    {
        (*remaining)=cur;
    }

    return ret;
}


void hthrd_freertos_yield(void)
{
    taskYIELD();
}

static void hthrd_freertos_clean_timer_callback(TimerHandle_t xTimer)
{
    void *tcb=(void *)pvTimerGetTimerID(xTimer);
    if(tcb!=NULL)
    {
        hfree(tcb);
    }
    xTimerDelete(xTimer,0);
}

void hthrd_freertos_exit(int res)
{
    if(sizeof(hthrd_t) < sizeof(hthrd_freertos_t))
    {
        return ;
    }
    if((*(hthrd_freertos_t *)&res)!=NULL)
    {
        hthrd_freertos_tcb_t *tcb=(hthrd_freertos_tcb_t *)(*(hthrd_freertos_t *)&res);
        /*
         * 设置退出信息
         */
        tcb->exit_code=res;
        tcb->flags.thread_exit=1;
        /*
         * 等待线程分离
         */
        while(tcb->flags.thread_detach==0)
        {
            hthrd_freertos_yield();
        }
        /*
         * 删除TCB
         */
        TimerHandle_t hthrd_freertos_clean_timer=NULL;
        while(hthrd_freertos_clean_timer==NULL)
        {
            hthrd_freertos_clean_timer=xTimerCreate("thrd_clr",pdMS_TO_TICKS(1),pdFALSE,tcb,hthrd_freertos_clean_timer_callback);
            if(hthrd_freertos_clean_timer!=NULL)
            {
                xTimerStart(hthrd_freertos_clean_timer,100);
                /*
                 * 删除当前任务
                 *                  */
                vTaskDelete(NULL);
            }
            hthrd_freertos_yield();
        }
    }
}


int hthrd_freertos_detach(hthrd_t thr)
{
    int ret=hthrd_error;
    if(sizeof(hthrd_t) < sizeof(hthrd_freertos_t))
    {
        return ret;
    }

    if((*(hthrd_freertos_t *)&thr)!=NULL)
    {
        hthrd_freertos_tcb_t *tcb=(hthrd_freertos_tcb_t *)(*(hthrd_freertos_t *)&thr);
        /*
         * 设置分离标志
         */
        tcb->flags.thread_detach=1;
        /*
         * 让出时间片让线程执行
         */
        hthrd_freertos_yield();

        ret=hthrd_success;
    }


    return ret;
}


int hthrd_freertos_join(hthrd_t thr, int *res)
{
    int ret=hthrd_error;
    if(sizeof(hthrd_t) < sizeof(hthrd_freertos_t))
    {
        return ret;
    }

    if((*(hthrd_freertos_t *)&thr)!=NULL)
    {
        hthrd_freertos_tcb_t *tcb=(hthrd_freertos_tcb_t *)(*(hthrd_freertos_t *)&thr);
        if(tcb->flags.thread_detach==1)
        {
            /*
             * 已分离的线程无法join
             */
            return ret;
        }
        ret=hthrd_success;
        while(tcb->flags.thread_exit==0)
        {
            hthrd_freertos_yield();
        }
        if(res!=NULL)
        {
            (*res)=tcb->exit_code;
        }
        hthrd_freertos_detach(thr);
    }


    return ret;
}

