/***************************************************************
 * Name:      hclock_settime.c
 * Purpose:   实现hclock_settime接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-01-21
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"

int hclock_settime(hclockid_t clockid,const htimespec_t * tp)
{
    int ret_value=0;
    if(tp!=NULL)
    {
        ret_value=-1;
#if    defined(HDEFAULTS_OS_LINUX_SYSCALL32_clock_settime)
        hdefaults_usercall(HDEFAULTS_OS_LINUX_SYSCALL32_clock_settime,&ret_value,clockid,tp);
#elif  defined(HDEFAULTS_OS_LINUX_SYSCALL64_clock_settime)
        hdefaults_usercall(HDEFAULTS_OS_LINUX_SYSCALL64_clock_settime,&ret_value,clockid,tp);
#elif  defined( HDEFAULTS_OS_FREEBSD_SYSCALL_clock_settime)
        hdefaults_usercall(HDEFAULTS_OS_FREEBSD_SYSCALL_clock_settime,&ret_value,clockid,tp);
#endif
    }
    return ret_value;
}

