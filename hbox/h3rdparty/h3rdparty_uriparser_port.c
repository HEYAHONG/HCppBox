/***************************************************************
 * Name:      h3rdparty_uriparser.c
 * Purpose:   引入第三方源代码文件
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-10-20
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#define H3RDPARTY_URIPARSER_IMPLEMENTATION  1

#include "hdefaults.h"
#include "h3rdparty.h"

#ifdef printf
#undef printf
#endif
#define printf hprintf

#include "h3rdparty/patch/heap.c"

#include "h3rdparty/patch/armcc_diag_suppress.c"





#include "port/uriparser/huriparser.c"




