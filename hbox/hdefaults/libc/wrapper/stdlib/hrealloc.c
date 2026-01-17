/***************************************************************
 * Name:      hrealloc.c
 * Purpose:   实现hrealloc接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-12
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hrealloc.h"
#include "hmalloc.h"
#include "hfree.h"
#include "hdefaults.h"

void *hrealloc( void *ptr, size_t new_size )
{
    void * new_ptr=hmalloc(new_size);
    if(new_ptr!=NULL && ptr!=NULL)
    {
        memcpy(new_ptr,ptr,new_size);
    }
    hfree(ptr);
    return new_ptr;
}

