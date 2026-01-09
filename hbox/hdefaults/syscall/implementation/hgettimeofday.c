/***************************************************************
 * Name:      hgettimeofday.c
 * Purpose:   实现hgettimeofday接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-04-30
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"

#if    defined(HDEFAULTS_OS_LINUX_SYSCALL32_gettimeofday)
#define HDEFAULTS_SYSCALL_HGETTIMEOFDAY  HDEFAULTS_OS_LINUX_SYSCALL32_gettimeofday
#elif  defined(HDEFAULTS_OS_LINUX_SYSCALL64_gettimeofday)
#define HDEFAULTS_SYSCALL_HGETTIMEOFDAY  HDEFAULTS_OS_LINUX_SYSCALL64_gettimeofday
#elif  defined( HDEFAULTS_OS_FREEBSD_SYSCALL_gettimeofday)
#define HDEFAULTS_SYSCALL_HGETTIMEOFDAY  HDEFAULTS_OS_FREEBSD_SYSCALL_gettimeofday
#endif


#ifdef HDEFAULTS_SYSCALL_HGETTIMEOFDAY

#if defined(HDEFAULTS_OS_UNIX) || ( defined(HDEFAULTS_PLATFORM_ESP) && defined(IDF_VER) )
#include <sys/time.h>
#elif defined(HDEFAULTS_OS_WINDOWS)
#include <windows.h>
#include <time.h>
#endif

#if defined(HGETTIMEOFDAY)
extern int HGETTIMEOFDAY(hgettimeofday_timeval_t *tv, hgettimeofday_timezone_t * tz);
#endif // defined

HDEFAULTS_USERCALL_DEFINE2(hgettimeofday,HDEFAULTS_SYSCALL_HGETTIMEOFDAY,int,hgettimeofday_timeval_t *,tv, hgettimeofday_timezone_t *,tz)
{
    int ret=-1;
#if defined(HGETTIMEOFDAY)
    ret=HGETTIMEOFDAY(tv,tz);
#elif defined(HDEFAULTS_OS_UNIX)  || ( defined(HDEFAULTS_PLATFORM_ESP) && defined(IDF_VER) )
    {
        struct timeval _tv= {0};
        struct timezone _tz= {0};
        ret=gettimeofday(&_tv, &_tz);
        if(tv!=NULL)
        {
            tv->tv_sec=_tv.tv_sec;
            tv->tv_usec=_tv.tv_usec;
        }
        if(tz!=NULL)
        {
            tz->tz_minuteswest=_tz.tz_minuteswest;
            tz->tz_dsttime=_tz.tz_dsttime;
        }
    }
#elif defined(HDEFAULTS_OS_WINDOWS)
    {
        time_t clock;
        struct tm tm;
        SYSTEMTIME wtm;
        GetLocalTime(&wtm);
        tm.tm_year   = wtm.wYear  - 1900;
        tm.tm_mon    = wtm.wMonth - 1;
        tm.tm_mday   = wtm.wDay;
        tm.tm_hour   = wtm.wHour;
        tm.tm_min    = wtm.wMinute;
        tm.tm_sec    = wtm.wSecond;
        tm.tm_isdst  = -1;

        clock = mktime(&tm);
        if(tv!=NULL)
        {
            tv->tv_sec   = clock;
            tv->tv_usec  = wtm.wMilliseconds * 1000;
        }
        if(tz!=NULL)
        {
            //不支持时区
            tz->tz_minuteswest=0;
            tz->tz_dsttime=-1;
        }
        ret=0;
    }
#else
    {
        ret=hsyscall_gettimeofday(tv,tz);
    }
#endif
    return ret;
}
#endif // HDEFAULTS_SYSCALL_HGETTIMEOFDAY
