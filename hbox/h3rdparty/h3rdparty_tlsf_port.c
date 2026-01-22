/***************************************************************
 * Name:      h3rdparty_tlsf_port.c
 * Purpose:   引入第三方源代码文件
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-12-24
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

/*
 * 此宏定义用于辅助移植TLSF，尤其是某些SDK中的魔改版本
 */
#define H3RDPARTY_TLSF_PORT 1
#include "hdefaults.h"
#include "h3rdparty.h"

#include "port/tlsf/htlsf.c"

