/***************************************************************
 * Name:      hstacklesscoroutine.c
 * Purpose:   实现hstacklesscoroutine接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-08-24
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hstacklesscoroutine.h"
#include "stdlib.h"

bool hstacklesscoroutine_is_finished(hstacklesscoroutine_control_block_t *ccb)
{
    if(ccb!=NULL)
    {
        return ccb->corevalue==-1;
    }
    return true;
}

void hstacklesscoroutine_coroutine_restart(hstacklesscoroutine_control_block_t *ccb)
{
    if(ccb!=NULL)
    {
        if(hstacklesscoroutine_is_finished(ccb))
        {
            ccb->corevalue=0;
        }
    }
}

void hstacklesscoroutine_coroutine_force_restart(hstacklesscoroutine_control_block_t *ccb)
{
    if(ccb!=NULL)
    {
        ccb->corevalue=0;
    }
}


