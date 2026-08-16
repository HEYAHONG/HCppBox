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

#ifndef  HFLASHDB_USING_SYSTEM_FLASHDB

#include "source/fdb_kvdb.c"

#endif // HFLASHDB_USING_SYSTEM_FLASHDB


