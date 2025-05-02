/***************************************************************
 * Name:      hstacklesscoroutine_v1.c
 * Purpose:   实现hstacklesscoroutine_v1接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-08-24
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hstacklesscoroutine_v1.h"
#include "stdlib.h"

bool hstacklesscoroutine_is_finished(hstacklesscoroutine_control_block_t *ccb)
{
    if(ccb!=NULL)
    {
        return (ccb->flags&(0x1ULL<<1))!=0;
    }
    return true;
}

void hstacklesscoroutine_coroutine_restart(hstacklesscoroutine_control_block_t *ccb)
{
    if(ccb!=NULL)
    {
        if(hstacklesscoroutine_is_finished(ccb))
        {
            ccb->flags=0;
            ccb->corevalue=0;
        }
    }
}

void hstacklesscoroutine_coroutine_force_restart(hstacklesscoroutine_control_block_t *ccb)
{
    if(ccb!=NULL)
    {
        ccb->flags=0;
        ccb->corevalue=0;
    }
}

bool hstacklesscoroutine_is_suspend(hstacklesscoroutine_control_block_t *ccb)
{
    if(ccb!=NULL)
    {
        return (ccb->flags &(0x1ULL<<0))!=0;
    }
    return false;
}
void hstacklesscoroutine_coroutine_suspend(hstacklesscoroutine_control_block_t *ccb)
{
    if(ccb!=NULL)
    {
        ccb->flags|=(0x1ULL << 0);
    }
}
void hstacklesscoroutine_coroutine_resume(hstacklesscoroutine_control_block_t *ccb)
{
    if(ccb!=NULL)
    {
        ccb->flags&=~(0x1ULL << 0);
    }
}


bool hstacklesscoroutine_is_await(hstacklesscoroutine_control_block_t *ccb)
{
    if(ccb!=NULL)
    {
        return ccb->awaiter.wait_for_ready!=NULL;
    }
    return false;
}


int hstacklesscoroutine_coroutine_get_current_nested(hstacklesscoroutine_control_block_t *ccb)
{
    if(ccb!=NULL)
    {
        return ccb->nested;
    }
    return 0;
}


void hstacklesscoroutine_coroutine_set_max_nested(hstacklesscoroutine_control_block_t *ccb,int max_nested)
{
    if(ccb!=NULL &&max_nested > 0)
    {
        ccb->max_nested=max_nested;
    }
}

