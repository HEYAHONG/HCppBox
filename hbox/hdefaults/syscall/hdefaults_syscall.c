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
#include "wrapper/hsettimeofday.c"
#include "wrapper/hgetrandmon.c"

#ifndef HDEFAULTS_SYSCALL_NO_IMPLEMENTATION

#include "implementation/hgettimeofday.c"
#include "implementation/hsettimeofday.c"
#include "implementation/hgetrandom.c"

#endif // HDEFAULTS_SYSCALL_NO_IMPLEMENTATION


#ifdef HDEFAULTS_SYSCALL_NO_HGETTIMEOFDAY
#undef HDEFAULTS_SYSCALL_HGETTIMEOFDAY
#endif // HDEFAULTS_SYSCALL_NO_HGETTIMEOFDAY

#ifdef HDEFAULTS_SYSCALL_NO_HSETTIMEOFDAY
#undef HDEFAULTS_SYSCALL_HSETTIMEOFDAY
#endif // HDEFAULTS_SYSCALL_NO_HSETTIMEOFDAY

#ifdef HDEFAULTS_SYSCALL_NO_HGETRANDOM
#undef HDEFAULTS_SYSCALL_HGETRANDOM
#endif // HDEFAULTS_SYSCALL_NO_HGETRANDOM

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
#ifdef HDEFAULTS_SYSCALL_HSETTIMEOFDAY
    case HDEFAULTS_SYSCALL_HSETTIMEOFDAY:
    {
        ret=__hdefaults_usercall_hsettimeofday;
    }
    break;
#endif
#ifdef HDEFAULTS_SYSCALL_HGETRANDOM
    case HDEFAULTS_SYSCALL_HGETRANDOM:
    {
        ret=__hdefaults_usercall_hgetrandom;
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



