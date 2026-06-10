#include "mainloop.h"


void libmono_runtime_mainloop_task_event_set(struct libmono_runtime_mainloop_task_context *ctx,uint16_t events) LIBMONO_REENTRANT_FUNCTION_ATTRIBUTE
{
    libmono_runtime_criticalsection_enter();
    if(ctx!=NULL)
    {
        ctx->event_group |= (events);
    }
    libmono_runtime_criticalsection_leave();
}


void libmono_runtime_mainloop_task_event_clear(struct libmono_runtime_mainloop_task_context *ctx,uint16_t events) LIBMONO_REENTRANT_FUNCTION_ATTRIBUTE
{
    libmono_runtime_criticalsection_enter();
    if(ctx!=NULL)
    {
        ctx->event_group &= (~events);
    }
    libmono_runtime_criticalsection_leave();
}

uint16_t libmono_runtime_mainloop_task_event(struct libmono_runtime_mainloop_task_context *ctx) LIBMONO_REENTRANT_FUNCTION_ATTRIBUTE
{
    uint16_t ret=0;
    libmono_runtime_criticalsection_enter();
    if(ctx!=NULL)
    {
        ret=ctx->event_group;
    }
    libmono_runtime_criticalsection_leave();
    return ret;
}

static LIBMONO_DATA_ATTRIBUTE struct
{
    /** \brief  标志位：
     *              位0表示是否第一次进入。
     *              位1表示正在执行任务
     */
    uint8_t flags;

    /** \brief 当前任务
     *
     *
     */
    libmono_runtime_mainloop_task_context_t **curtask;

} libmono_runtime_mainloop_context;

enum
{
    LIBMONO_RUNTIME_MAINLOOP_INIT=(1UL<< 0),
    LIBMONO_RUNTIME_MAINLOOP_TASK_RUNNING=(1UL << 1)
};

static inline void libmono_runtime_mainloop_flags_set(uint8_t flags) LIBMONO_FUNCTION_ATTRIBUTE
{
    libmono_runtime_criticalsection_enter();
    libmono_runtime_mainloop_context.flags |= flags;
    libmono_runtime_criticalsection_leave();
}

static inline void libmono_runtime_mainloop_flags_clear(uint8_t flags) LIBMONO_FUNCTION_ATTRIBUTE
{
    libmono_runtime_criticalsection_enter();
    libmono_runtime_mainloop_context.flags &= (~flags);
    libmono_runtime_criticalsection_leave();
}

static inline uint8_t libmono_runtime_mainloop_flags(void) LIBMONO_FUNCTION_ATTRIBUTE
{
    uint8_t ret=0;
    libmono_runtime_criticalsection_enter();
    ret=libmono_runtime_mainloop_context.flags;
    libmono_runtime_criticalsection_leave();
    return ret;
}



#if defined(LIBMONO_RUNTIME_MAINLOOP_TASK_ARRAY_NAME)

extern libmono_runtime_mainloop_task_context_t * const LIBMONO_RUNTIME_MAINLOOP_TASK_ARRAY_NAME[];

#endif

void libmono_runtime_mainloop_process(void) LIBMONO_FUNCTION_ATTRIBUTE
{
    if((libmono_runtime_mainloop_flags()&(LIBMONO_RUNTIME_MAINLOOP_INIT))==0)
    {
        /*
         * 第一次进入
         */
        libmono_runtime_mainloop_flags_set(LIBMONO_RUNTIME_MAINLOOP_INIT);

#if defined(LIBMONO_RUNTIME_MAINLOOP_TASK_ARRAY_NAME)

        if(LIBMONO_RUNTIME_MAINLOOP_TASK_ARRAY_NAME!=NULL)
        {
            libmono_runtime_mainloop_context.curtask=(libmono_runtime_mainloop_task_context_t **)&LIBMONO_RUNTIME_MAINLOOP_TASK_ARRAY_NAME[0];
            while(libmono_runtime_mainloop_context.curtask!=NULL && (*libmono_runtime_mainloop_context.curtask)!=NULL)
            {
                libmono_runtime_mainloop_task_event_set(*libmono_runtime_mainloop_context.curtask,LIBMONO_RUNTIME_MAINLOOP_TASK_EVENT_INIT);
                libmono_runtime_mainloop_context.curtask++;
            }
        }

#endif

    }

#if defined(LIBMONO_RUNTIME_MAINLOOP_TASK_ARRAY_NAME)

    if(LIBMONO_RUNTIME_MAINLOOP_TASK_ARRAY_NAME!=NULL)
    {
        libmono_runtime_mainloop_context.curtask=(libmono_runtime_mainloop_task_context_t **)&LIBMONO_RUNTIME_MAINLOOP_TASK_ARRAY_NAME[0];
        while(libmono_runtime_mainloop_context.curtask!=NULL && (*libmono_runtime_mainloop_context.curtask)!=NULL)
        {
            if(libmono_runtime_mainloop_task_event(*libmono_runtime_mainloop_context.curtask)!=0)
            {
                if((*libmono_runtime_mainloop_context.curtask)->entry!=NULL)
                {
                    libmono_runtime_mainloop_flags_set(LIBMONO_RUNTIME_MAINLOOP_TASK_RUNNING);
                    (*libmono_runtime_mainloop_context.curtask)->entry(*(libmono_runtime_mainloop_context.curtask));
                    libmono_runtime_mainloop_flags_clear(LIBMONO_RUNTIME_MAINLOOP_TASK_RUNNING);
                }
            }
            libmono_runtime_mainloop_context.curtask++;
        }
    }

#endif

}

bool libmono_runtime_mainloop_is_idle(void) LIBMONO_REENTRANT_FUNCTION_ATTRIBUTE
{
    bool ret=true;
    libmono_runtime_criticalsection_enter();
#if defined(LIBMONO_RUNTIME_MAINLOOP_TASK_ARRAY_NAME)

    for(size_t i=0; LIBMONO_RUNTIME_MAINLOOP_TASK_ARRAY_NAME[i]!=NULL; i++)
    {
        if(libmono_runtime_mainloop_task_event((struct libmono_runtime_mainloop_task_context *)LIBMONO_RUNTIME_MAINLOOP_TASK_ARRAY_NAME[i])!=0)
        {
            /*
             * 当任务有事件未处理时判定为非空闲
             */
            ret=false;
            break;
        }
    }

#endif
    libmono_runtime_criticalsection_leave();
    return ret;
}

libmono_runtime_mainloop_task_context_t * libmono_runtime_mainloop_current_task(void) LIBMONO_REENTRANT_FUNCTION_ATTRIBUTE
{
    libmono_runtime_mainloop_task_context_t *ret=NULL;
    libmono_runtime_criticalsection_enter();
    if((libmono_runtime_mainloop_flags()&LIBMONO_RUNTIME_MAINLOOP_TASK_RUNNING)!=0)
    {
        ret=(*libmono_runtime_mainloop_context.curtask);
    }
    libmono_runtime_criticalsection_leave();
    return ret;
}
