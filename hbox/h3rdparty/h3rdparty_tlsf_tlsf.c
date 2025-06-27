/***************************************************************
 * Name:      h3rdparty_tlsf.c
 * Purpose:   引入第三方源代码文件
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-06-27
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"
#include "h3rdparty.h"

#ifdef printf
#undef printf
#endif
#define printf hprintf

/*
 * 修复armcc下的警告
 */
#ifdef __ARMCC_VERSION
#ifndef __clang__
#pragma diag_suppress 68
#endif
#endif // __ARMCC_VERSION



#ifndef  H3RDPARTY_USING_SYSTEM_TLSF

#include "3rdparty/tlsf/tlsf.c"

#endif // H3RDPARTY_USING_SYSTEM_TLSF


