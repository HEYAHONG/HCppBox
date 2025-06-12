/***************************************************************
 * Name:      hclose.c
 * Purpose:   实现hclose接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-06-12
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"

int hclose(int fd)
{
    int ret_value=-1;
#if    defined(HDEFAULTS_OS_LINUX_SYSCALL32_close)
    hdefaults_usercall(HDEFAULTS_OS_LINUX_SYSCALL32_close,&ret_value,fd);
#elif  defined(HDEFAULTS_OS_LINUX_SYSCALL64_close)
    hdefaults_usercall(HDEFAULTS_OS_LINUX_SYSCALL64_close,&ret_value,fd);
#elif  defined( HDEFAULTS_OS_FREEBSD_SYSCALL_close)
    hdefaults_usercall(HDEFAULTS_OS_FREEBSD_SYSCALL_close,&ret_value,fd);
#endif
    return ret_value;
}

