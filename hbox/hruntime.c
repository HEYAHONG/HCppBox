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



void hruntime_init_lowlevel()
{
    /*
     * 初始化第三方库
     */
    h3rdparty_init();
}


void hruntime_init()
{
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

