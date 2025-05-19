/***************************************************************
 * Name:      hmmap.c
 * Purpose:   实现hmmap接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-19
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"

void *hmmap(void *addr, size_t len, int prot, int flags, int fildes, uint64_t off)
{
    void * ret_value=HMMAN_MAP_FAILED;
#if    defined(HDEFAULTS_OS_LINUX_SYSCALL32_mmap)
    hdefaults_usercall(HDEFAULTS_OS_LINUX_SYSCALL32_mmap,&ret_value,addr,len,prot,flags,fildes,off);
#elif  defined(HDEFAULTS_OS_LINUX_SYSCALL64_mmap)
    hdefaults_usercall(HDEFAULTS_OS_LINUX_SYSCALL64_mmap,&ret_value,addr,len,prot,flags,fildes,off);
#elif  defined( HDEFAULTS_OS_FREEBSD_SYSCALL_mmap)
    hdefaults_usercall(HDEFAULTS_OS_FREEBSD_SYSCALL_mmap,&ret_value,addr,len,prot,flags,fildes,off);
#endif
    return ret_value;
}

