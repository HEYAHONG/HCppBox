/***************************************************************
 * Name:      hlibc_threads.c
 * Purpose:   实现hlibc_threads接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-01-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"
#include "hlibc_threads.h"

void hlibc_call_once(hlibc_once_flag_t *flag,hlibc_call_once_func_t func)
{
    if(flag!=NULL)
    {
        if(!hatomic_flag_test_and_set(flag))
        {
            if(func!=NULL)
            {
                func();
            }
        }
    }
}

