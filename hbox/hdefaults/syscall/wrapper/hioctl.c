/***************************************************************
 * Name:      hioctl.c
 * Purpose:   实现hioctl接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-02-09
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"
#include "hioctl.h"

int hioctl(int fd, unsigned long op, ...)
{
    int ret_value=-1;
    va_list va;
    va_start(va,op);
#if    defined(HDEFAULTS_OS_LINUX_SYSCALL32_ioctl)
    hdefaults_usercall(HDEFAULTS_OS_LINUX_SYSCALL32_ioctl,&ret_value,fd,op,&va);
#elif  defined(HDEFAULTS_OS_LINUX_SYSCALL64_ioctl)
    hdefaults_usercall(HDEFAULTS_OS_LINUX_SYSCALL64_ioctl,&ret_value,fd,op,&va);
#elif  defined( HDEFAULTS_OS_FREEBSD_SYSCALL_ioctl)
    hdefaults_usercall(HDEFAULTS_OS_FREEBSD_SYSCALL_ioctl,&ret_value,fd,op,&va);
#endif
    va_end(va);
    return ret_value;
}

