/***************************************************************
 * Name:      hfsync.c
 * Purpose:   实现hfsync接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-08-14
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"

int hfsync(int fd)
{
    int ret_value=-1;
#if    defined(HDEFAULTS_OS_LINUX_SYSCALL32_fsync)
    hdefaults_usercall(HDEFAULTS_OS_LINUX_SYSCALL32_fsync,&ret_value,fd);
#elif  defined(HDEFAULTS_OS_LINUX_SYSCALL64_fsync)
    hdefaults_usercall(HDEFAULTS_OS_LINUX_SYSCALL64_fsync,&ret_value,fd);
#elif  defined( HDEFAULTS_OS_FREEBSD_SYSCALL_fsync)
    hdefaults_usercall(HDEFAULTS_OS_FREEBSD_SYSCALL_fsync,&ret_value,fd);
#endif
    return ret_value;
}

