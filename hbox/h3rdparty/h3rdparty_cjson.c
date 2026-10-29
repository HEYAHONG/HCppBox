/***************************************************************
 * Name:      h3rdparty_cjson.c
 * Purpose:   引入第三方源代码文件
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-09-27
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#define H3RDPARTY_CJSON_IMPLEMENTATION  1

#include "hdefaults.h"
#include "h3rdparty.h"

#include "h3rdparty/patch/heap.c"

#include "h3rdparty/patch/disable_armcc_gnuc_micro.c"

#include "h3rdparty/patch/armclang_diagnostic_ignored.c"

#ifndef  H3RDPARTY_USING_SYSTEM_CJSON

/*
 * 隐藏cJSON符号
 */
#ifndef  CJSON_HIDE_SYMBOLS
#define  CJSON_HIDE_SYMBOLS 1
#endif

#include "3rdparty/cJSON/cJSON.c"

hcjson_hooks_t hcjson_hooks_get(void)
{
    hcjson_hooks_t ret;
    ret.malloc_fn=global_hooks.allocate;
    ret.free_fn=global_hooks.deallocate;
    return ret;
}

#else

hcjson_hooks_t hcjson_hooks_get(void)
{
    hcjson_hooks_t ret;
    ret.malloc_fn=malloc;
    ret.free_fn=free;
    return ret;
}

#endif // H3RDPARTY_USING_SYSTEM_CJSON


