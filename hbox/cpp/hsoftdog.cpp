/***************************************************************
 * Name:      hsoftdog.cpp
 * Purpose:   hsoftdog
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-02-02
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hsoftdog.h"
#include "hstacklesscoroutine.h"

HSTACKLESSCOROUTINE_BLOCK_START(hsoftdog)
HWATCHDOG_FEED();
HSTACKLESSCOROUTINE_BLOCK_END(hsoftdog)
