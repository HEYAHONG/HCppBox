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
};

HSTACKLESSCOROUTINE_BLOCK_START(hsoftdog)
hstacklesscoroutine_yield_with_label(HSOFTDOG_START);
HWATCHDOG_FEED();
hstacklesscoroutine_goto_label(HSOFTDOG_START);
HSTACKLESSCOROUTINE_BLOCK_END(hsoftdog)
