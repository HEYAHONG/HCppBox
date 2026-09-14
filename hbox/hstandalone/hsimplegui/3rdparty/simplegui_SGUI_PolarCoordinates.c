/***************************************************************
 * Name:      hsimplegui.c
 * Purpose:   引入第三方源代码文件
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-09-14
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#define HSIMPLEGUI_IMPLEMENTATION  1

#include "hdefaults.h"

#include "h3rdparty/patch/heap.c"

#include "h3rdparty/patch/cfile.c"

#include "h3rdparty/patch/posix_file.c"

#include "h3rdparty/patch/disable_armcc_gnuc_micro.c"


#ifndef  HSIMPLEGUI_USING_SYSTEM_SIMPLEGUI

#include "source/SGUI_PolarCoordinates.c"

#endif // HSIMPLEGUI_USING_SYSTEM_SIMPLEGUI


