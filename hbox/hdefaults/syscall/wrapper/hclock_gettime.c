/***************************************************************
 * Name:      hclock_gettime.c
 * Purpose:   实现hclock_gettime接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-01-21
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"

int hclock_gettime(hclockid_t clockid, htimespec_t * tp)
{
    int ret_value=0;
    if(tp!=NULL)
    {
        ret_value=-1;
#if    defined(HDEFAULTS_OS_LINUX_SYSCALL32_clock_gettime)
        hdefaults_usercall(HDEFAULTS_OS_LINUX_SYSCALL32_clock_gettime,&ret_value,clockid,tp);
#elif  defined(HDEFAULTS_OS_LINUX_SYSCALL64_clock_gettime)
        hdefaults_usercall(HDEFAULTS_OS_LINUX_SYSCALL64_clock_gettime,&ret_value,clockid,tp);
#elif  defined( HDEFAULTS_OS_FREEBSD_SYSCALL_clock_gettime)
        hdefaults_usercall(HDEFAULTS_OS_FREEBSD_SYSCALL_clock_gettime,&ret_value,clockid,tp);
#endif
    }
    return ret_value;
}

