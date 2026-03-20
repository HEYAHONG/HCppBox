/***************************************************************
 * Name:      h3rdparty_lz4.c
 * Purpose:   引入第三方源代码文件
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-06-06
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#define H3RDPARTY_LZ4_IMPLEMENTATION  1

#include "hdefaults.h"
#include "h3rdparty.h"

#include "h3rdparty/patch/heap.c"

#include "h3rdparty/patch/cfile.c"

#include "h3rdparty/patch/disable_armcc_gnuc_micro.c"


#include "stdarg.h"

/*
 * 修复armcc下的警告
 */
#ifdef __ARMCC_VERSION
#define LZ4_DISABLE_DEPRECATE_WARNINGS 1
#endif // __ARMCC_VERSION

#ifndef  H3RDPARTY_USING_SYSTEM_LZ4

#include "3rdparty/lz4/lz4hc.c"

#endif // H3RDPARTY_USING_SYSTEM_LZ4


