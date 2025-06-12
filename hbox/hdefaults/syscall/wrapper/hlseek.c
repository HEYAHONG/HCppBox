/***************************************************************
 * Name:      hlseek.c
 * Purpose:   实现hlseek接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-06-12
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"

hlseek_off_t hlseek(int fd,hlseek_off_t offset,int whence)
{
    hlseek_off_t ret_value=-1;
#if    defined(HDEFAULTS_OS_LINUX_SYSCALL32_lseek)
    hdefaults_usercall(HDEFAULTS_OS_LINUX_SYSCALL32_lseek,&ret_value,fd,offset,whence);
#elif  defined(HDEFAULTS_OS_LINUX_SYSCALL64_lseek)
    hdefaults_usercall(HDEFAULTS_OS_LINUX_SYSCALL64_lseek,&ret_value,fd,offset,whence);
#elif  defined( HDEFAULTS_OS_FREEBSD_SYSCALL_lseek)
    hdefaults_usercall(HDEFAULTS_OS_FREEBSD_SYSCALL_lseek,&ret_value,fd,offset,whence);
#endif
    return ret_value;
}

