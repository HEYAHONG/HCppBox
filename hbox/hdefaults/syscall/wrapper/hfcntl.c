/***************************************************************
 * Name:      hfcntl.c
 * Purpose:   实现hfcntl接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-03-09
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"


int hfcntl(int fd, int op, ... /* arg */ )
{
    int ret_value=-1;
    va_list va;
    va_start(va,op);
#if    defined(HDEFAULTS_OS_LINUX_SYSCALL32_fcntl)
    hdefaults_usercall(HDEFAULTS_OS_LINUX_SYSCALL32_fcntl,&ret_value,fd,op,&va);
#elif  defined(HDEFAULTS_OS_LINUX_SYSCALL64_fcntl)
    hdefaults_usercall(HDEFAULTS_OS_LINUX_SYSCALL64_fcntl,&ret_value,fd,op,&va);
#elif  defined( HDEFAULTS_OS_FREEBSD_SYSCALL_fcntl)
    hdefaults_usercall(HDEFAULTS_OS_FREEBSD_SYSCALL_fcntl,&ret_value,fd,op,&va);
#endif
    va_end(va);
    return ret_value;
}

