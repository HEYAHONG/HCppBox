/***************************************************************
 * Name:      hclock_getres.c
 * Purpose:   实现hclock_getres接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-01-21
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"

#if    defined(HDEFAULTS_OS_LINUX_SYSCALL32_clock_getres)
#define HDEFAULTS_SYSCALL_HCLOCK_GETRES  HDEFAULTS_OS_LINUX_SYSCALL32_clock_getres
#elif  defined(HDEFAULTS_OS_LINUX_SYSCALL64_clock_getres)
#define HDEFAULTS_SYSCALL_HCLOCK_GETRES  HDEFAULTS_OS_LINUX_SYSCALL64_clock_getres
#elif  defined( HDEFAULTS_OS_FREEBSD_SYSCALL_clock_getres)
#define HDEFAULTS_SYSCALL_HCLOCK_GETRES  HDEFAULTS_OS_FREEBSD_SYSCALL_clock_getres
#endif


#ifdef HDEFAULTS_SYSCALL_HCLOCK_GETRES

#if defined(HDEFAULTS_OS_UNIX)
#include <sys/time.h>
#elif defined(HDEFAULTS_OS_WINDOWS)
#include <windows.h>
#include <time.h>
#endif

#if defined(HCLOCK_GETRES)
extern int HCLOCK_GETRES(hclockid_t clockid, htimespec_t * res);
#endif

HDEFAULTS_USERCALL_DEFINE2(hclock_getres,HDEFAULTS_SYSCALL_HCLOCK_GETRES,int,hclockid_t,clockid, htimespec_t *,res)
{
    int ret=-1;
#if defined(HCLOCK_GETRES)
    ret=HCLOCK_GETRES(clockid,res);
#elif (defined(HDEFAULTS_OS_UNIX))  && (!defined(HDEFAULTS_OS_EMSCRIPTEN))
    {
        struct timespec _res= {0};
        ret=clock_getres(clockid,&_res);
        if(res!=NULL)
        {
            res->tv_sec=_res.tv_sec;
            res->tv_nsec=_res.tv_nsec;
        }
    }
#elif defined(HDEFAULTS_OS_WINDOWS)
    {
        switch(clockid)
        {
        case HCLOCK_REALTIME:
        {
            /*
             * 默认精度为1ms(1000000ns)
             */
            if(res!=NULL)
            {
                res->tv_sec=0;
                res->tv_nsec=1000000;
            }
            ret=0;
        }
        break;
        case HCLOCK_MONOTONIC:
        {
            /*
             * 默认精度为1ms(1000000ns)
             */
            if(res!=NULL)
            {
                res->tv_sec=0;
                res->tv_nsec=1000000;
            }
            ret=0;
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
        ret=hsyscall_clock_getres(clockid,res);
    }
#else

#endif
    return ret;
}
#endif // HDEFAULTS_SYSCALL_HCLOCK_GETRES
