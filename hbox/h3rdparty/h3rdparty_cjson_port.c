/***************************************************************
 * Name:      h3rdparty_cjson_port.c
 * Purpose:   引入第三方源代码文件
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-12-22
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"
#include "h3rdparty.h"


/*
 * 修复armcc下的警告
 */
#ifdef __ARMCC_VERSION
#ifndef __clang__
#pragma diag_suppress 191
#endif
#endif // __ARMCC_VERSION


#include "port/cJSON/hcjson.c"


