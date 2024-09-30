/***************************************************************
 * Name:      h3rdparty.c
 * Purpose:   引入第三方源代码文件
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-09-27
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "h3rdparty.h"
#include "hdefaults.h"



static void *h3rdparty_malloc(size_t len)
{
    return hdefaults_get_api_table()->mem_alloc(len,NULL);
}

static void h3rdparty_free(void *ptr)
{
    hdefaults_get_api_table()->mem_free(ptr,NULL);
}

void h3rdparty_init(void)
{
    {
        //初始化cJSON
        cJSON_Hooks hook= {h3rdparty_malloc,h3rdparty_free};
        cJSON_InitHooks(&hook);
    }
}

