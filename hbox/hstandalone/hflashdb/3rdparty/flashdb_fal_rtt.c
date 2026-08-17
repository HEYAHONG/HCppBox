/***************************************************************
 * Name:      hflashdb.c
 * Purpose:   引入第三方源代码文件
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-08-16
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#define HFLASHDB_IMPLEMENTATION  1

#include "hdefaults.h"

#include "h3rdparty/patch/heap.c"

#include "h3rdparty/patch/cfile.c"

#include "h3rdparty/patch/posix_file.c"

#include "h3rdparty/patch/disable_armcc_gnuc_micro.c"

#include "source/flashdb.h"

#if defined(HFLASHDB_NO_RTT)
#undef  RT_VER_NUM 
#endif

#ifndef  HFLASHDB_USING_SYSTEM_FLASHDB

#ifdef FDB_USING_FAL_MODE

#include "source/fal_rtt.c"

#endif

#endif // HFLASHDB_USING_SYSTEM_FLASHDB


