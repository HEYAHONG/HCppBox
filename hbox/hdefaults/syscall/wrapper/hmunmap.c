/***************************************************************
 * Name:      hmunmap.c
 * Purpose:   实现hmunmap接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-19
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"

int hmunmap(void *addr, size_t len)
{
    int ret_value=-1;
#if    defined(HDEFAULTS_OS_LINUX_SYSCALL32_munmap)
    hdefaults_usercall(HDEFAULTS_OS_LINUX_SYSCALL32_munmap,&ret_value,addr,len);
#elif  defined(HDEFAULTS_OS_LINUX_SYSCALL64_munmap)
    hdefaults_usercall(HDEFAULTS_OS_LINUX_SYSCALL64_munmap,&ret_value,addr,len);
#elif  defined( HDEFAULTS_OS_FREEBSD_SYSCALL_munmap)
    hdefaults_usercall(HDEFAULTS_OS_FREEBSD_SYSCALL_munmap,&ret_value,addr,len);
#endif
    return ret_value;
}

