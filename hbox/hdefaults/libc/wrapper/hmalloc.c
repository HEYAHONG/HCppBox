/***************************************************************
 * Name:      hmalloc.c
 * Purpose:   实现hmalloc接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-12
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hmalloc.h"
#include "hdefaults.h"

void * hmalloc(size_t nBytes)
{
    if(nBytes==0)
    {
        /*
         * 至少申请1字节
         */
        nBytes=1;
    }
    return hdefaults_malloc(nBytes,NULL);
}


