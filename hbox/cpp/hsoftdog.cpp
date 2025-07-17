/***************************************************************
 * Name:      hsoftdog.cpp
 * Purpose:   hsoftdog
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-02-02
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hsoftdog.h"
#include "hruntime.h"

enum
{
    HSOFTDOG_START = 1,
    HSOFTDOG_END   = 2,
};

HSTACKLESSCOROUTINE_BLOCK_START(hsoftdog)
hstacklesscoroutine_yield_with_label(HSOFTDOG_START);
HWATCHDOG_FEED();
if(hwatchdog_is_valid())
{
    /*
     * 仅当hcpprt循环调用前初始化的看门狗在此处喂狗,应用过程中的喂狗操作交由用户自行实现。
     */
    hstacklesscoroutine_goto_label(HSOFTDOG_START);
}
hstacklesscoroutine_yield_with_label(HSOFTDOG_END);
HSTACKLESSCOROUTINE_BLOCK_END(hsoftdog)
