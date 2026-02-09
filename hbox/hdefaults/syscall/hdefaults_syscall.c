/***************************************************************
 * Name:      hdefaults_syscall.c
 * Purpose:   实现hdefaults_syscall接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-04-30
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults_syscall.h"

/*
 * hsyscall
 */
#if !defined(HSYSCALL_NO_IMPLEMENTATION)
#if !defined(HSYSCALL_NO_TIME)
#include "hsyscall/time/hsyscall_time.c"
#endif
#if !defined(HSYSCALL_NO_RANDOM)
#include "hsyscall/random/hsyscall_random.c"
#endif
#endif

/*
 * 包装
 */
#include "wrapper/hgettimeofday.c"
#include "wrapper/hsettimeofday.c"
#include "wrapper/hgetrandom.c"
#include "wrapper/hmmap.c"
#include "wrapper/hmsync.c"
#include "wrapper/hmunmap.c"
#include "wrapper/hmprotect.c"
#include "wrapper/hclose.c"
#include "wrapper/hread.c"
#include "wrapper/hwrite.c"
#include "wrapper/hlseek.c"
#include "wrapper/hopen.c"
#include "wrapper/hopenat.c"
#include "wrapper/hioctl.c"
#include "wrapper/hclock_getres.c"
#include "wrapper/hclock_gettime.c"
#include "wrapper/hclock_settime.c"


#ifndef HDEFAULTS_SYSCALL_NO_IMPLEMENTATION

#include "implementation/hgettimeofday.c"
#include "implementation/hsettimeofday.c"
#include "implementation/hgetrandom.c"
#include "implementation/hmmap.c"
#include "implementation/hmsync.c"
#include "implementation/hmunmap.c"
#include "implementation/hmprotect.c"
#include "implementation/hclose.c"
#include "implementation/hread.c"
#include "implementation/hwrite.c"
#include "implementation/hlseek.c"
#include "implementation/hopen.c"
#include "implementation/hopenat.c"
#include "implementation/hioctl.c"
#include "implementation/hclock_getres.c"
#include "implementation/hclock_gettime.c"
#include "implementation/hclock_settime.c"

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

#ifdef HDEFAULTS_SYSCALL_NO_HMMAP
#undef HDEFAULTS_SYSCALL_HMMAP
#endif // HDEFAULTS_SYSCALL_NO_HMMAP

#ifdef HDEFAULTS_SYSCALL_NO_HMSYNC
#undef HDEFAULTS_SYSCALL_HMSYNC
#endif // HDEFAULTS_SYSCALL_NO_HMSYNC

#ifdef HDEFAULTS_SYSCALL_NO_HMUNMAP
#undef HDEFAULTS_SYSCALL_HMUNMAP
#endif // HDEFAULTS_SYSCALL_NO_HMUNMAP

#ifdef HDEFAULTS_SYSCALL_NO_HMPROTECT
#undef HDEFAULTS_SYSCALL_HMPROTECT
#endif // HDEFAULTS_SYSCALL_NO_HMPROTECT

#ifdef HDEFAULTS_SYSCALL_NO_HCLOSE
#undef HDEFAULTS_SYSCALL_HCLOSE
#endif // HDEFAULTS_SYSCALL_NO_HCLOSE

#ifdef HDEFAULTS_SYSCALL_NO_HREAD
#undef HDEFAULTS_SYSCALL_HREAD
#endif // HDEFAULTS_SYSCALL_NO_HREAD

#ifdef HDEFAULTS_SYSCALL_NO_HWRITE
#undef HDEFAULTS_SYSCALL_HWRITE
#endif // HDEFAULTS_SYSCALL_NO_HWRITE

#ifdef HDEFAULTS_SYSCALL_NO_HLSEEK
#undef HDEFAULTS_SYSCALL_HLSEEK
#endif // HDEFAULTS_SYSCALL_NO_HLSEEK

#ifdef HDEFAULTS_SYSCALL_NO_HOPEN
#undef HDEFAULTS_SYSCALL_HOPEN
#endif // HDEFAULTS_SYSCALL_NO_HOPEN

#ifdef HDEFAULTS_SYSCALL_NO_HOPENAT
#undef HDEFAULTS_SYSCALL_HOPENAT
#endif // HDEFAULTS_SYSCALL_NO_HOPENAT

#ifdef HDEFAULTS_SYSCALL_NO_HIOCTL
#undef HDEFAULTS_SYSCALL_HIOCTL
#endif // HDEFAULTS_SYSCALL_NO_HIOCTL

#ifdef HDEFAULTS_SYSCALL_NO_HCLOCK_GETRES
#undef HDEFAULTS_SYSCALL_HCLOCK_GETRES
#endif // HDEFAULTS_SYSCALL_NO_HCLOCK_GETRES

#ifdef HDEFAULTS_SYSCALL_NO_HCLOCK_GETTIME
#undef HDEFAULTS_SYSCALL_HCLOCK_GETTIME
#endif // HDEFAULTS_SYSCALL_NO_HCLOCK_GETTIME

#ifdef HDEFAULTS_SYSCALL_NO_HCLOCK_SETTIME
#undef HDEFAULTS_SYSCALL_HCLOCK_SETTIME
#endif // HDEFAULTS_SYSCALL_NO_HCLOCK_SETTIME

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
#ifdef HDEFAULTS_SYSCALL_HMMAP
    case HDEFAULTS_SYSCALL_HMMAP:
    {
        ret=__hdefaults_usercall_hmmap;
    }
    break;
#endif
#ifdef HDEFAULTS_SYSCALL_HMSYNC
    case HDEFAULTS_SYSCALL_HMSYNC:
    {
        ret=__hdefaults_usercall_hmsync;
    }
    break;
#endif
#ifdef HDEFAULTS_SYSCALL_HMUNMAP
    case HDEFAULTS_SYSCALL_HMUNMAP:
    {
        ret=__hdefaults_usercall_hmunmap;
    }
    break;
#endif
#ifdef HDEFAULTS_SYSCALL_HMPROTECT
    case HDEFAULTS_SYSCALL_HMPROTECT:
    {
        ret=__hdefaults_usercall_hmprotect;
    }
    break;
#endif
#ifdef HDEFAULTS_SYSCALL_HCLOSE
    case HDEFAULTS_SYSCALL_HCLOSE:
    {
        ret=__hdefaults_usercall_hclose;
    }
    break;
#endif
#ifdef HDEFAULTS_SYSCALL_HREAD
    case HDEFAULTS_SYSCALL_HREAD:
    {
        ret=__hdefaults_usercall_hread;
    }
    break;
#endif
#ifdef HDEFAULTS_SYSCALL_HWRITE
    case HDEFAULTS_SYSCALL_HWRITE:
    {
        ret=__hdefaults_usercall_hwrite;
    }
    break;
#endif
#ifdef HDEFAULTS_SYSCALL_HLSEEK
    case HDEFAULTS_SYSCALL_HLSEEK:
    {
        ret=__hdefaults_usercall_hlseek;
    }
    break;
#endif
#ifdef HDEFAULTS_SYSCALL_HOPEN
    case HDEFAULTS_SYSCALL_HOPEN:
    {
        ret=__hdefaults_usercall_hopen;
    }
    break;
#endif
#ifdef HDEFAULTS_SYSCALL_HOPENAT
    case HDEFAULTS_SYSCALL_HOPENAT:
    {
        ret=__hdefaults_usercall_hopenat;
    }
    break;
#endif
#ifdef HDEFAULTS_SYSCALL_HIOCTL
    case HDEFAULTS_SYSCALL_HIOCTL:
    {
        ret=__hdefaults_usercall_hioctl;
    }
    break;
#endif
#ifdef HDEFAULTS_SYSCALL_HCLOCK_GETRES
    case HDEFAULTS_SYSCALL_HCLOCK_GETRES:
    {
        ret=__hdefaults_usercall_hclock_getres;
    }
    break;
#endif
#ifdef HDEFAULTS_SYSCALL_HCLOCK_GETTIME
    case HDEFAULTS_SYSCALL_HCLOCK_GETTIME:
    {
        ret=__hdefaults_usercall_hclock_gettime;
    }
    break;
#endif
#ifdef HDEFAULTS_SYSCALL_HCLOCK_SETTIME
    case HDEFAULTS_SYSCALL_HCLOCK_SETTIME:
    {
        ret=__hdefaults_usercall_hclock_settime;
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

void hdefaults_syscall_init(void)
{

#if  defined(HDEFAULTS_OS_EMSCRIPTEN)
    {
        hsettimeofday_timeval_t tv= {0};
        struct timeval old_tv={0};
        gettimeofday(&old_tv,NULL);
        tv.tv_sec=old_tv.tv_sec;
        tv.tv_usec=old_tv.tv_usec;
        hsettimeofday(&tv,NULL);
    }
#endif

#if !defined(HDEFAULTS_SYSCALL_NO_IMPLEMENTATION) && !defined(HDEFAULTS_SYSCALL_NO_HGETRANDOM) && !defined(HGETRANDOM)
    {
        //使用当前时间作为随机数种子
        hgettimeofday_timeval_t tv= {0};
        hgettimeofday(&tv,NULL);
        {
#if defined(HSYSCALL_GETRANDOM_USING_MT)
            hrng_mt_srand(NULL,tv.tv_sec*1000000+tv.tv_usec);
#else
            hrng_linearcongruential_rand48_srand(tv.tv_sec*1000000+tv.tv_usec);
#endif
        }
    }
#endif

}

void hdefaults_syscall_loop(void)
{

#if !defined(HDEFAULTS_SYSCALL_NO_IMPLEMENTATION) && !defined(HDEFAULTS_SYSCALL_NO_HGETTIMEOFDAY) && !defined(HGETTIMEOFDAY) && !defined(HSYSCALL_GETTIMEOFDAY_UPDATE)
    /*
     * 调用一次hgettimeofday更新内部时间
     */
    {
        hgettimeofday_timeval_t tv;
        hgettimeofday_timezone_t tz;
        hgettimeofday(&tv,&tz);
    }
#endif

#if !defined(HDEFAULTS_SYSCALL_NO_IMPLEMENTATION) && !defined(HDEFAULTS_SYSCALL_NO_HCLOCK_GETTIME) && !defined(HCLOCK_GETTIME)
    /*
     * 更新内部单调时钟时间
     */
    {
        htimespec_t tp={0};
        hclock_gettime(HCLOCK_MONOTONIC,&tp);
    }
#endif

}

