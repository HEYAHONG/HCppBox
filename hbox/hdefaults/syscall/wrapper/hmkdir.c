/***************************************************************
 * Name:      hmkdir.c
 * Purpose:   实现hmkdir接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-08-16
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"


int hmkdir(const char * pathname,unsigned long mode)
{
    int ret_value=-1;
#if    defined(HDEFAULTS_OS_LINUX_SYSCALL32_mkdir)
    hdefaults_usercall(HDEFAULTS_OS_LINUX_SYSCALL32_mkdir,&ret_value,pathname,mode);
#elif  defined(HDEFAULTS_OS_LINUX_SYSCALL64_mkdir)
    hdefaults_usercall(HDEFAULTS_OS_LINUX_SYSCALL64_mkdir,&ret_value,pathname,mode);
#elif  defined( HDEFAULTS_OS_FREEBSD_SYSCALL_mkdir)
    hdefaults_usercall(HDEFAULTS_OS_FREEBSD_SYSCALL_mkdir,&ret_value,pathname,mode);
#endif
    return ret_value;
}

