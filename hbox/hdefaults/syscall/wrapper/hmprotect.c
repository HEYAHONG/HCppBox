/***************************************************************
 * Name:      hmprotect.c
 * Purpose:   实现hmprotect接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-20
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"

int hmprotect(void *addr, size_t len, int prot)
{
    int ret_value=-1;
#if    defined(HDEFAULTS_OS_LINUX_SYSCALL32_mprotect)
    hdefaults_usercall(HDEFAULTS_OS_LINUX_SYSCALL32_mprotect,&ret_value,addr,len,prot);
#elif  defined(HDEFAULTS_OS_LINUX_SYSCALL64_mprotect)
    hdefaults_usercall(HDEFAULTS_OS_LINUX_SYSCALL64_mprotect,&ret_value,addr,len,prot);
#elif  defined( HDEFAULTS_OS_FREEBSD_SYSCALL_mprotect)
    hdefaults_usercall(HDEFAULTS_OS_FREEBSD_SYSCALL_mprotect,&ret_value,addr,len,prot);
#endif
    return ret_value;
}

