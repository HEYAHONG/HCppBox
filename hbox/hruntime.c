/***************************************************************
 * Name:      hruntime.c
 * Purpose:   实现hruntime接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-04-19
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hruntime.h"
#include "h3rdparty.h"
#include "heventslots.h"
#include "heventloop.h"
#include "stdbool.h"
#include "stdint.h"
#include "stdlib.h"


void hruntime_init_lowlevel()
{
    /*
     * 初始化第三方库
     */
    h3rdparty_init();
}


void hruntime_init()
{

#ifdef HRUNTIME_USING_INIT_SECTION
    HRUNTIME_INIT_INVOKE();
#endif // HRUNTIME_USING_INIT_SECTION

    /*
     * 系统初始化完成
     */
    {
        heventslots_t *slots_init=heventslots_get_slots_from_table(HEVENTSLOTS_SYSTEM_SLOTS_INIT);
        if(slots_init!=NULL)
        {
            heventslots_emit_signal(slots_init,NULL);
        }
    }

}

void hruntime_loop()
{

#ifdef HRUNTIME_USING_LOOP_SECTION
    HRUNTIME_LOOP_INVOKE();
#endif // HRUNTIME_USING_LOOP_SECTION

    /*
     * 系统循环
     */
    {
        heventslots_t *slots_loop=heventslots_get_slots_from_table(HEVENTSLOTS_SYSTEM_SLOTS_LOOP);
        if(slots_loop!=NULL)
        {
            heventslots_emit_signal(slots_loop,NULL);
        }
    }

    /*
     * 工作队列
     */
    {
        heventloop_t *loop_workqueue=heventloop_get_loop_from_table(HEVENTLOOP_SYSTEM_LOOP_WORKQUEUE);
        if(loop_workqueue!=NULL)
        {
            heventloop_process_event(loop_workqueue);
        }
    }

}

void hruntime_function_array_invoke(const hruntime_function_t *array_base,size_t array_size)
{
    if(array_base==NULL || array_size == 0)
    {
        return;
    }

    size_t priority_min=0;
    size_t priority_max=0;
    for(size_t i=0; i<array_size; i++)
    {
        if(array_base[i].priority < priority_min)
        {
            priority_min=array_base[i].priority;
        }
        if(array_base[i].priority > priority_max)
        {
            priority_max=array_base[i].priority;
        }
    }

    for(size_t i=priority_min; priority_min<=priority_max;)
    {
        if(array_base[i].priority==i)
        {
            if(array_base[i].entry!=NULL)
            {
                array_base[i].entry(&array_base[i]);
            }
        }

        if(i==priority_max)
        {
            break;
        }

        {
            size_t priority=priority_max;
            for(size_t j=0; j<array_size; j++)
            {
                if(array_base[j].priority > i && array_base[j].priority < priority )
                {
                    priority=array_base[j].priority;
                }
            }
            i=priority;
        }
    }

}


