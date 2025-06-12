/***************************************************************
 * Name:      hread.c
 * Purpose:   实现hread接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-06-12
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"

hread_ssize_t hread(int fd,void *buff,size_t buff_count)
{
    hread_ssize_t ret_value=-1;
#if    defined(HDEFAULTS_OS_LINUX_SYSCALL32_read)
    hdefaults_usercall(HDEFAULTS_OS_LINUX_SYSCALL32_read,&ret_value,fd,buff,buff_count);
#elif  defined(HDEFAULTS_OS_LINUX_SYSCALL64_read)
    hdefaults_usercall(HDEFAULTS_OS_LINUX_SYSCALL64_read,&ret_value,fd,buff,buff_count);
#elif  defined( HDEFAULTS_OS_FREEBSD_SYSCALL_read)
    hdefaults_usercall(HDEFAULTS_OS_FREEBSD_SYSCALL_read,&ret_value,fd,buff,buff_count);
#endif
    return ret_value;
}

