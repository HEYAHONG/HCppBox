/***************************************************************
 * Name:      hsettimeofday.c
 * Purpose:   实现hsettimeofday接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-01
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"

#if    defined(HDEFAULTS_OS_LINUX_SYSCALL32_settimeofday)
#define HDEFAULTS_SYSCALL_HSETTIMEOFDAY  HDEFAULTS_OS_LINUX_SYSCALL32_settimeofday
#elif  defined(HDEFAULTS_OS_LINUX_SYSCALL64_settimeofday)
#define HDEFAULTS_SYSCALL_HSETTIMEOFDAY  HDEFAULTS_OS_LINUX_SYSCALL64_settimeofday
#elif  defined( HDEFAULTS_OS_FREEBSD_SYSCALL_settimeofday)
#define HDEFAULTS_SYSCALL_HSETTIMEOFDAY  HDEFAULTS_OS_FREEBSD_SYSCALL_settimeofday
#endif

#if defined(HDEFAULTS_OS_EMSCRIPTEN) && !defined(HSETTIMEOFDAY)
/*
 * Emscripten 默认不支持链接此系统调用
 */
#undef HDEFAULTS_SYSCALL_HSETTIMEOFDAY
#endif // HDEFAULTS_OS_EMSCRIPTEN


/*
 * 此函数在hgettimeofday.c定义
 */
extern void __hgettimeofday_tick_set(uint64_t tick);


#ifdef HDEFAULTS_SYSCALL_HSETTIMEOFDAY

#if defined(HDEFAULTS_OS_UNIX)
#include <sys/time.h>
#elif defined(HDEFAULTS_OS_WINDOWS)
#include <windows.h>
#include <time.h>
#endif

#if defined(HSETTIMEOFDAY)
extern int HSETTIMEOFDAY(const hsettimeofday_timeval_t *tv,const  hsettimeofday_timezone_t * tz);
#endif // defined

HDEFAULTS_USERCALL_DEFINE2(hsettimeofday,HDEFAULTS_SYSCALL_HSETTIMEOFDAY,int,const hsettimeofday_timeval_t *,tv,const hsettimeofday_timezone_t *,tz)
{
    int ret=-1;
#if defined(HSETTIMEOFDAY)
    ret=HSETTIMEOFDAY(tv,tz);
#elif defined(HDEFAULTS_OS_UNIX)
    {
        struct timeval _tv= {0};
        struct timezone _tz= {0};
        if(tv!=NULL)
        {
            _tv.tv_sec=tv->tv_sec;
            _tv.tv_usec=tv->tv_usec;
        }
        if(tz!=NULL)
        {
            _tz.tz_minuteswest=tz->tz_minuteswest;
            _tz.tz_dsttime=tz->tz_dsttime;
        }
        ret=settimeofday(tv!=NULL?&_tv:NULL,tz!=NULL?&_tz:NULL);
    }
#elif defined(HDEFAULTS_OS_WINDOWS)
    {
        SYSTEMTIME curtime;
        if(tv!=NULL)
        {
            curtime.wMilliseconds=tv->tv_usec/1000;
            time_t current_time=time(NULL);
            struct tm current_tm=*localtime(&current_time);
            current_time=tv->tv_sec;
            current_tm=*localtime(&current_time);
            curtime.wYear =current_tm.tm_year+  1900;
            curtime.wMonth=current_tm.tm_mon+1;
            curtime.wDay=current_tm.tm_mday;;
            curtime.wHour=current_tm.tm_hour;
            curtime.wMinute=current_tm.tm_min;
            curtime.wSecond=current_tm.tm_sec;
            if(SetLocalTime(&curtime))
            {
                ret=0;
            }
        }
        if(tz!=NULL)
        {
            //不支持时区
        }
    }
#else
    {
        uint64_t tick=0;
        if(tv!=NULL)
        {
            tick=tv->tv_sec*1000+tv->tv_usec/1000;
            __hgettimeofday_tick_set(tick);
        }
        if(tz!=NULL)
        {
            //不支持时区
        }
        ret=0;
    }
#endif
    return ret;
}
#endif // HDEFAULTS_SYSCALL_HSETTIMEOFDAY
