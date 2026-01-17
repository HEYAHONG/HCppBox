/***************************************************************
 * Name:      hfree.c
 * Purpose:   实现hfree接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-12
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hfree.h"
#include "hdefaults.h"

void  hfree(void *ptr)
{
    if(ptr !=NULL)
    {
        hdefaults_free(ptr,NULL);
    }
}


