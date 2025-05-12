/***************************************************************
 * Name:      hcalloc.c
 * Purpose:   实现hcalloc接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-12
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hcalloc.h"
#include "hmalloc.h"
#include "hdefaults.h"

void* hcalloc( size_t num, size_t nBytes)
{
    void *ptr=hmalloc(num*nBytes);
    if(ptr!=NULL)
    {
        memset(ptr,0,num*nBytes);
    }
    return ptr;
}


