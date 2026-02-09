/***************************************************************
 * Name:      hclock_settime.c
 * Purpose:   实现hclock_settime接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-01-21
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"

#if    defined(HDEFAULTS_OS_LINUX_SYSCALL32_clock_settime)
#define HDEFAULTS_SYSCALL_HCLOCK_SETTIME  HDEFAULTS_OS_LINUX_SYSCALL32_clock_settime
#elif  defined(HDEFAULTS_OS_LINUX_SYSCALL64_clock_settime)
#define HDEFAULTS_SYSCALL_HCLOCK_SETTIME  HDEFAULTS_OS_LINUX_SYSCALL64_clock_settime
#elif  defined( HDEFAULTS_OS_FREEBSD_SYSCALL_clock_settime)
#define HDEFAULTS_SYSCALL_HCLOCK_SETTIME  HDEFAULTS_OS_FREEBSD_SYSCALL_clock_settime
#endif


#ifdef HDEFAULTS_SYSCALL_HCLOCK_SETTIME

#if defined(HDEFAULTS_OS_UNIX)
#include <sys/time.h>
#elif defined(HDEFAULTS_OS_WINDOWS)
#include <windows.h>
#include <time.h>
#endif

#if defined(HCLOCK_SETTIME)
extern int HCLOCK_SETTIME(hclockid_t clockid,const htimespec_t * tp);
#endif

HDEFAULTS_USERCALL_DEFINE2(hclock_settime,HDEFAULTS_SYSCALL_HCLOCK_SETTIME,int,hclockid_t,clockid,const htimespec_t *,tp)
{
    int ret=-1;
#if defined(HCLOCK_SETTIME)
    ret=HCLOCK_SETTIME(clockid,tp);
#elif (defined(HDEFAULTS_OS_UNIX))  && (!defined(HDEFAULTS_OS_EMSCRIPTEN))
    {
        struct timespec _tp= {0};
        _tp.tv_sec=tp->tv_sec;
        _tp.tv_nsec=tp->tv_nsec;
        ret=clock_settime(clockid,&_tp);
    }
#elif defined(HDEFAULTS_OS_WINDOWS)
    {
        switch(clockid)
        {
        case HCLOCK_REALTIME:
        {
            htimeval_t tv= {0};
            tv.tv_sec=tp->tv_sec;
            tv.tv_usec=tp->tv_nsec/1000;
            ret=hsettimeofday(&tv,NULL);
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
        ret=hsyscall_clock_settime(clockid,tp);
    }
#else

#endif
    return ret;
}
#endif // HDEFAULTS_SYSCALL_HCLOCK_SETTIME
