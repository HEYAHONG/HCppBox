/***************************************************************
 * Name:      hclock_getres.c
 * Purpose:   实现hclock_getres接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-01-21
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"

int hclock_getres(hclockid_t clockid, htimespec_t * res)
{
    int ret_value=0;
    if(res!=NULL)
    {
        ret_value=-1;
#if    defined(HDEFAULTS_OS_LINUX_SYSCALL32_clock_getres)
        hdefaults_usercall(HDEFAULTS_OS_LINUX_SYSCALL32_clock_getres,&ret_value,clockid,res);
#elif  defined(HDEFAULTS_OS_LINUX_SYSCALL64_clock_getres)
        hdefaults_usercall(HDEFAULTS_OS_LINUX_SYSCALL64_clock_getres,&ret_value,clockid,res);
#elif  defined( HDEFAULTS_OS_FREEBSD_SYSCALL_clock_getres)
        hdefaults_usercall(HDEFAULTS_OS_FREEBSD_SYSCALL_clock_getres,&ret_value,clockid,res);
#endif
    }
    return ret_value;
}

