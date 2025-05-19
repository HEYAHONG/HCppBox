/***************************************************************
 * Name:      hmsync.c
 * Purpose:   实现hmsync接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-19
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"

int hmsync(void *addr, size_t len, int flags)
{
    int ret_value=-1;
#if    defined(HDEFAULTS_OS_LINUX_SYSCALL32_msync)
    hdefaults_usercall(HDEFAULTS_OS_LINUX_SYSCALL32_msync,&ret_value,addr,len,flags);
#elif  defined(HDEFAULTS_OS_LINUX_SYSCALL64_msync)
    hdefaults_usercall(HDEFAULTS_OS_LINUX_SYSCALL64_msync,&ret_value,addr,len,flags);
#elif  defined( HDEFAULTS_OS_FREEBSD_SYSCALL_msync)
    hdefaults_usercall(HDEFAULTS_OS_FREEBSD_SYSCALL_msync,&ret_value,addr,len,flags);
#endif
    return ret_value;
}

