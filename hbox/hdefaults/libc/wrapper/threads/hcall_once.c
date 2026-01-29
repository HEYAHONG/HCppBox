/***************************************************************
 * Name:      hcall_once.c
 * Purpose:   实现hcall_once接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-01-29
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"
#include "hcall_once.h"

#if defined(HCALL_ONCE)
extern void HCALL_ONCE(honce_flag_t *flag,hcall_once_func_t func);
#endif

void hcall_once(honce_flag_t *flag,hcall_once_func_t func)
{
#if defined(HCALL_ONCE)
    HCALL_ONCE(flag,func);
#elif  defined(HDEFAULTS_LIBC_HAVE_THREADS)
    {
        if(sizeof(honce_flag_t) < sizeof(once_flag))
        {
            return;
        }
        call_once((once_flag*)flag,func);
    }
#else
    {
        if(sizeof(honce_flag_t) < sizeof(hatomic_flag_t) || flag==NULL)
        {
            return;
        }
        if(!hatomic_flag_test_and_set((hatomic_flag_t*)flag))
        {
            if(func!=NULL)
            {
                func();
            }
        }
    }
#endif
}

