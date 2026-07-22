/***************************************************************
 * Name:      timer.c
 * Purpose:   实现timer接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-07-22
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#define HOPENBLT_TIMER_IMPLEMENTATION  1

#include "boot.h"
#include "hbox.h"

#if !defined(HOPENBLT_TIMER_FUNCTION_ATTRIBUTE)
#define HOPENBLT_TIMER_FUNCTION_ATTRIBUTE __WEAK
#endif

#if !defined(HOPENBLT_TIMER_NO_IMPLEMENTATION)

HOPENBLT_TIMER_FUNCTION_ATTRIBUTE
void       TimerInit(void)
{

}

HOPENBLT_TIMER_FUNCTION_ATTRIBUTE
void       TimerUpdate(void)
{

}

HOPENBLT_TIMER_FUNCTION_ATTRIBUTE
blt_int32u TimerGet(void)
{
    return hdefaults_tick_get();
}

HOPENBLT_TIMER_FUNCTION_ATTRIBUTE
void       TimerReset(void)
{
    /*
     * TODO:关闭定时器
     */

}

#endif


