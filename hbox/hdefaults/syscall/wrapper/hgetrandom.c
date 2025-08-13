/***************************************************************
 * Name:      hgetrandom.c
 * Purpose:   实现hgetrandom接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-01
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"

hgetrandom_ssize_t hgetrandom (void *buffer, size_t length,unsigned int flags)
{
    hgetrandom_ssize_t ret_value=0;
    if(buffer!=NULL && length!=0)
    {
        ret_value=-1;
#if    defined(HDEFAULTS_OS_LINUX_SYSCALL32_getrandom)
        hdefaults_usercall(HDEFAULTS_OS_LINUX_SYSCALL32_getrandom,&ret_value,buffer,length,flags);
#elif  defined(HDEFAULTS_OS_LINUX_SYSCALL64_getrandom)
        hdefaults_usercall(HDEFAULTS_OS_LINUX_SYSCALL64_getrandom,&ret_value,buffer,length,flags);
#elif  defined( HDEFAULTS_OS_FREEBSD_SYSCALL_getrandom)
        hdefaults_usercall(HDEFAULTS_OS_FREEBSD_SYSCALL_getrandom,&ret_value,buffer,length,flags);
#endif
    }
    return ret_value;
}


