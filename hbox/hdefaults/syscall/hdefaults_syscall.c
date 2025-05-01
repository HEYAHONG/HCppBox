/***************************************************************
 * Name:      hdefaults_syscall.c
 * Purpose:   实现hdefaults_syscall接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-04-30
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults_syscall.h"


#include "wrapper/hgettimeofday.c"

#ifndef HDEFAULTS_SYSCALL_NO_IMPLEMENTATION

#include "implementation/hgettimeofday.c"

#endif // HDEFAULTS_SYSCALL_NO_IMPLEMENTATION

hdefaults_syscall_function_t hdefaults_syscall_function_find(uintptr_t number)
{
    hdefaults_syscall_function_t ret=NULL;
#ifndef HDEFAULTS_SYSCALL_NO_IMPLEMENTATION
    switch(number)
    {
#ifdef HDEFAULTS_SYSCALL_HGETTIMEOFDAY
    case HDEFAULTS_SYSCALL_HGETTIMEOFDAY:
    {
        ret=__hdefaults_usercall_hgettimeofday;
    }
    break;
#endif
    default:
    {
    }
    break;
    }
#endif // HDEFAULTS_SYSCALL_NO_IMPLEMENTATION
    return ret;
}



