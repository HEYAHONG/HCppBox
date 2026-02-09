/***************************************************************
 * Name:      hclock_gettime.c
 * Purpose:   实现hclock_gettime接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-01-21
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"

#if    defined(HDEFAULTS_OS_LINUX_SYSCALL32_clock_gettime)
#define HDEFAULTS_SYSCALL_HCLOCK_GETTIME  HDEFAULTS_OS_LINUX_SYSCALL32_clock_gettime
#elif  defined(HDEFAULTS_OS_LINUX_SYSCALL64_clock_gettime)
#define HDEFAULTS_SYSCALL_HCLOCK_GETTIME  HDEFAULTS_OS_LINUX_SYSCALL64_clock_gettime
#elif  defined( HDEFAULTS_OS_FREEBSD_SYSCALL_clock_gettime)
#define HDEFAULTS_SYSCALL_HCLOCK_GETTIME  HDEFAULTS_OS_FREEBSD_SYSCALL_clock_gettime
#endif


#ifdef HDEFAULTS_SYSCALL_HCLOCK_GETTIME

#if defined(HDEFAULTS_OS_UNIX)
#include <sys/time.h>
#elif defined(HDEFAULTS_OS_WINDOWS)
#include <windows.h>
#include <time.h>
#endif

#if defined(HCLOCK_GETTIME)
extern int HCLOCK_GETTIME(hclockid_t clockid, htimespec_t * tp);
#endif

HDEFAULTS_USERCALL_DEFINE2(hclock_gettime,HDEFAULTS_SYSCALL_HCLOCK_GETTIME,int,hclockid_t,clockid, htimespec_t *,tp)
{
    int ret=-1;
#if defined(HCLOCK_GETTIME)
    ret=HCLOCK_GETTIME(clockid,tp);
#elif (defined(HDEFAULTS_OS_UNIX))  && (!defined(HDEFAULTS_OS_EMSCRIPTEN))
    {
        struct timespec _tp= {0};
        ret=clock_gettime(clockid,&_tp);
        if(tp!=NULL)
        {
            tp->tv_sec=_tp.tv_sec;
            tp->tv_nsec=_tp.tv_nsec;
        }
    }
#elif defined(HDEFAULTS_OS_WINDOWS)
    {
        switch(clockid)
        {
        case HCLOCK_REALTIME:
        {
            htimeval_t tv= {0};
            ret=hgettimeofday(&tv,NULL);
            if(tp!=NULL)
            {
                tp->tv_sec=tv.tv_sec;
                tp->tv_nsec=tv.tv_usec*1000;
            }
        }
        break;
        case HCLOCK_MONOTONIC:
        {
            static htimespec_t current_monotonic_clock= {0};
            static uint32_t current_monotonic_clock_last_tick=0;
            ret=0;
            hdefaults_mutex_lock(NULL);
            uint32_t current_tick=GetTickCount();
            current_monotonic_clock.tv_nsec+=(current_tick-current_monotonic_clock_last_tick)*1000000;
            current_monotonic_clock_last_tick=current_tick;
            current_monotonic_clock.tv_sec+=current_monotonic_clock.tv_nsec/1000000000;
            current_monotonic_clock.tv_nsec=current_monotonic_clock.tv_nsec%1000000000;
            if(tp!=NULL)
            {
                tp->tv_sec=current_monotonic_clock.tv_sec;
                tp->tv_nsec=current_monotonic_clock.tv_nsec;
            }
            hdefaults_mutex_unlock(NULL);
        }
        break;
        default:
        {

        }
        break;
        }
    }
#elif  !defined(HSYSCALL_NO_IMPLEMENTATION) && !defined(HSYSCALL_NO_TIME)
    {
        ret=hsyscall_clock_gettime(clockid,tp);
    }
#else

#endif
    return ret;
}
#endif // HDEFAULTS_SYSCALL_HCLOCK_GETTIME
