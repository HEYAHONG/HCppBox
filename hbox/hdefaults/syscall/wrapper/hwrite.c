/***************************************************************
 * Name:      hwrite.c
 * Purpose:   实现hwrite接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-06-12
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"

hwrite_ssize_t hwrite(int fd,const void *buff,size_t buff_count)
{
    hwrite_ssize_t ret_value=-1;
#if    defined(HDEFAULTS_OS_LINUX_SYSCALL32_write)
    hdefaults_usercall(HDEFAULTS_OS_LINUX_SYSCALL32_write,&ret_value,fd,buff,buff_count);
#elif  defined(HDEFAULTS_OS_LINUX_SYSCALL64_write)
    hdefaults_usercall(HDEFAULTS_OS_LINUX_SYSCALL64_write,&ret_value,fd,buff,buff_count);
#elif  defined( HDEFAULTS_OS_FREEBSD_SYSCALL_write)
    hdefaults_usercall(HDEFAULTS_OS_FREEBSD_SYSCALL_write,&ret_value,fd,buff,buff_count);
#endif
    return ret_value;
}

