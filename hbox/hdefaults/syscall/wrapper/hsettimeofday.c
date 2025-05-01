/***************************************************************
 * Name:      hsettimeofday.c
 * Purpose:   实现hsettimeofday接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-01
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"

int hsettimeofday (const hsettimeofday_timeval_t *tv,const  hsettimeofday_timezone_t * tz)
{
    int ret_value=0;
    if(tv!=NULL || tz!=NULL)
    {
        ret_value=-1;
#if    defined(HDEFAULTS_OS_LINUX_SYSCALL32_settimeofday)
        hdefaults_usercall(HDEFAULTS_OS_LINUX_SYSCALL32_settimeofday,&ret_value,tv,tz);
#elif  defined(HDEFAULTS_OS_LINUX_SYSCALL64_settimeofday)
        hdefaults_usercall(HDEFAULTS_OS_LINUX_SYSCALL64_settimeofday,&ret_value,tv,tz);
#elif  defined( HDEFAULTS_OS_FREEBSD_SYSCALL_settimeofday)
        hdefaults_usercall(HDEFAULTS_OS_FREEBSD_SYSCALL_settimeofday,&ret_value,tv,tz);
#endif
    }
    return ret_value;
}

