/***************************************************************
 * Name:      hgettimeofday.c
 * Purpose:   实现hgettimeofday接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-04-30
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"

int hgettimeofday (hgettimeofday_timeval_t *tv, hgettimeofday_timezone_t * tz)
{
    int ret_value=0;
    if(tv!=NULL || tz!=NULL)
    {
        ret_value=-1;
#if    defined(HDEFAULTS_OS_LINUX_SYSCALL32_gettimeofday)
        hdefaults_usercall(HDEFAULTS_OS_LINUX_SYSCALL32_gettimeofday,&ret_value,tv,tz);
#elif  defined(HDEFAULTS_OS_LINUX_SYSCALL64_gettimeofday)
        hdefaults_usercall(HDEFAULTS_OS_LINUX_SYSCALL64_gettimeofday,&ret_value,tv,tz);
#elif  defined( HDEFAULTS_OS_FREEBSD_SYSCALL_gettimeofday)
        hdefaults_usercall(HDEFAULTS_OS_FREEBSD_SYSCALL_gettimeofday,&ret_value,tv,tz);
#endif
    }
    return ret_value;
}

