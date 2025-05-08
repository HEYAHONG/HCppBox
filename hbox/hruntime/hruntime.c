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
#include "hevent.h"
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
        for(size_t j=0; j<array_size; j++)
        {
            if(array_base[j].priority==i)
            {
                if(array_base[j].entry!=NULL)
                {
                    array_base[j].entry(&array_base[j]);
                }
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

#ifdef HRUNTIME_SYMBOL_SCAN
#undef HRUNTIME_USING_SYMBOL_TABLE
#endif // HRUNTIME_SYMBOL_SCAN

#ifdef HRUNTIME_USING_SYMBOL_TABLE

#include "symbol/hbox_symbol.c"
#include "symbol/hbox_h3rdparty_symbol.c"

const struct
{
    const hruntime_symbol_t *   array_base;
    size_t                      array_size;
}
hruntime_symbol_array_list[]=
{
    {
        hruntime_hbox_symbol,
        sizeof(hruntime_hbox_symbol)/sizeof(hruntime_hbox_symbol[0])
    },
    {
        hruntime_hbox_h3rdparty_symbol,
        sizeof(hruntime_hbox_h3rdparty_symbol)/sizeof(hruntime_hbox_h3rdparty_symbol[0])
    },
    {
        //结束
        NULL,
        0
    }
};

#endif

const hruntime_symbol_t *hruntime_symbol_find(const char *name)
{
    const hruntime_symbol_t *ret=NULL;
    if(name==NULL || strlen(name) == 0)
    {
        return ret;
    }

#ifdef HRUNTIME_USING_SYMBOL_TABLE
    for(size_t i=0; i<sizeof(hruntime_symbol_array_list)/sizeof(hruntime_symbol_array_list[0]); i++)
    {
        const hruntime_symbol_t *   array_base=hruntime_symbol_array_list[i].array_base;
        size_t                      array_size=hruntime_symbol_array_list[i].array_size;
        if(array_base!=NULL && array_size!=0)
        {
            for(size_t i=0; i<array_size; i++)
            {
                if(array_base[i].symbol_name!=NULL && strcmp(array_base[i].symbol_name,name)==0)
                {
                    ret=&array_base[i];
                    break;
                }
            }
        }
        if(ret!=NULL)
        {
            break;
        }
    }
#endif

    if(ret==NULL)
    {
        const hruntime_symbol_t *   array_base=NULL;
        size_t                      array_size=0;
#ifdef HRUNTIME_USING_SYMBOL_SECTION
#if defined(HCOMPILER_ARMCC) || defined(HCOMPILER_ARMCLANG)
        {
            array_base=(hruntime_symbol_t *)&HRuntimeLoop$$Base;
            array_size=(((uintptr_t)(hruntime_symbol_t *)&HRuntimeLoop$$Limit)-((uintptr_t)(hruntime_symbol_t *)&HRuntimeLoop$$Base))/sizeof(hruntime_symbol_t);
        }
#elif  defined(HCOMPILER_GCC) || defined(HCOMPILER_CLANG)
        {
            array_base=__hruntime_symbol_start;
            array_size=(((uintptr_t)__hruntime_symbol_end)-((uintptr_t)__hruntime_symbol_start))/sizeof(hruntime_symbol_t);
        }
#endif
#endif // HRUNTIME_USING_SYMBOL_SECTION
        if(array_base!=NULL && array_size!=0)
        {
            for(size_t i=0; i<array_size; i++)
            {
                if(array_base[i].symbol_name!=NULL && strcmp(array_base[i].symbol_name,name)==0)
                {
                    ret=&array_base[i];
                    break;
                }
            }
        }

    }
    return ret;
}

