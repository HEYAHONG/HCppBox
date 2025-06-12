/***************************************************************
 * Name:      hopen.c
 * Purpose:   实现hopen接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-06-12
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"


int hopen(const char * filename,int oflag,...)
{
    int ret_value=-1;
    unsigned int mode=0;
    {
        //读取mode值
        va_list va;
        va_start(va,oflag);
        mode=va_arg(va,unsigned int);
        va_end(va);
    }
#if    defined(HDEFAULTS_OS_LINUX_SYSCALL32_open)
    hdefaults_usercall(HDEFAULTS_OS_LINUX_SYSCALL32_open,&ret_value,filename,oflag,mode);
#elif  defined(HDEFAULTS_OS_LINUX_SYSCALL64_open)
    hdefaults_usercall(HDEFAULTS_OS_LINUX_SYSCALL64_open,&ret_value,filename,oflag,mode);
#elif  defined( HDEFAULTS_OS_FREEBSD_SYSCALL_open)
    hdefaults_usercall(HDEFAULTS_OS_FREEBSD_SYSCALL_open,&ret_value,filename,oflag,mode);
#endif
    return ret_value;
}

