/***************************************************************
 * Name:      hsyscall_time.c
 * Purpose:   实现hsyscall_time接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-01-09
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"
#include "hsyscall_time.h"


static hgettimeofday_timeval_t   hsyscall_gettimeofday_current_timeval= {0};
/*
 * 此处变量仅用于存储系统时区,不起实际作用，实际系统用户时区由libc（通常可通过环境变量或者配置文件设置）决定
 */
static hgettimeofday_timezone_t  hsyscall_gettimeofday_current_timezone= {0};
static void hsyscall_gettimeofday_update_timeval(void)
{
#if defined(HSYSCALL_GETTIMEOFDAY_UPDATE)
    hdefaults_mutex_lock(NULL);
    HSYSCALL_GETTIMEOFDAY_UPDATE(&hsyscall_gettimeofday_current_timeval,&hsyscall_gettimeofday_current_timezone);
    hdefaults_mutex_unlock(NULL);
#else
    hdefaults_mutex_lock(NULL);
    hgettimeofday_timeval_t old_val=hsyscall_gettimeofday_current_timeval;
    static hdefaults_tick_t hgettimeofday_last_tick=0;
    hdefaults_tick_t current_tick=hdefaults_tick_get();
    if(current_tick!=hgettimeofday_last_tick)
    {
        old_val.tv_usec+=((current_tick-hgettimeofday_last_tick)*1000);
        old_val.tv_sec+=(old_val.tv_usec/1000000);
        old_val.tv_usec=(old_val.tv_usec%1000000);
        hgettimeofday_last_tick=current_tick;
        hsyscall_gettimeofday_current_timeval=old_val;
    }
    hdefaults_mutex_unlock(NULL);
#endif
}
int hsyscall_gettimeofday (hgettimeofday_timeval_t *tv, hgettimeofday_timezone_t * tz)
{
    hsyscall_gettimeofday_update_timeval();
    if(tv!=NULL)
    {
        hdefaults_mutex_lock(NULL);
        (*tv)=hsyscall_gettimeofday_current_timeval;
        hdefaults_mutex_unlock(NULL);
    }
    if(tz!=NULL)
    {
        hdefaults_mutex_lock(NULL);
        (*tz)=hsyscall_gettimeofday_current_timezone;
        hdefaults_mutex_unlock(NULL);
    }
    return 0;
}

int hsyscall_settimeofday (const hsettimeofday_timeval_t *tv,const  hsettimeofday_timezone_t * tz)
{
    if(tv!=NULL)
    {
        hdefaults_mutex_lock(NULL);
        hsyscall_gettimeofday_current_timeval=(*tv);
        hdefaults_mutex_unlock(NULL);
    }
    if(tz!=NULL)
    {
        hdefaults_mutex_lock(NULL);
        hsyscall_gettimeofday_current_timezone=(*tz);
        hdefaults_mutex_unlock(NULL);
    }
#if defined(HSYSCALL_SETTIMEOFDAY_UPDATE)
    hdefaults_mutex_lock(NULL);
    HSYSCALL_SETTIMEOFDAY_UPDATE(&hsyscall_gettimeofday_current_timeval,&hsyscall_gettimeofday_current_timezone);
    hdefaults_mutex_unlock(NULL);
#endif
    return 0;
}

/*
 * 获取时钟精度
 */
int hsyscall_clock_getres(hclockid_t clockid, htimespec_t * res)
{
    int ret=-1;
    if(res==NULL)
    {
        return ret;
    }
    switch(clockid)
    {
    case HCLOCK_REALTIME:
    {
        /*
         * 默认精度为1ms(1000000ns)
         */
        res->tv_sec=0;
        res->tv_nsec=1000000;
        ret=0;
    }
    break;
    case HCLOCK_MONOTONIC:
    {
        /*
         * 默认精度为1ms(1000000ns)
         */
        res->tv_sec=0;
        res->tv_nsec=1000000;
        ret=0;
    }
    break;
    default:
    {

    }
    break;
    }

#if defined(HSYSCALL_CLOCK_GETRES_HOOK)
    ret=HSYSCALL_CLOCK_GETRES_HOOK(clockid,res);
#endif

    return ret;
}
/*
 * 获取时钟
 */
int hsyscall_clock_gettime(hclockid_t clockid,htimespec_t *tp)
{
    int ret=-1;
    if(tp==NULL)
    {
        return ret;
    }
    switch(clockid)
    {
    case HCLOCK_REALTIME:
    {
        htimeval_t tv= {0};
        ret=hsyscall_gettimeofday(&tv,NULL);
        tp->tv_sec=tv.tv_sec;
        tp->tv_nsec=tv.tv_usec*1000;
    }
    break;
    case HCLOCK_MONOTONIC:
    {
        static htimespec_t hsyscall_clock_gettime_current_monotonic_clock= {0};
        static hdefaults_tick_t hsyscall_clock_gettime_current_monotonic_clock_last_tick=0;
        ret=0;
        hdefaults_mutex_lock(NULL);
        hdefaults_tick_t current_tick=hdefaults_tick_get();
        hsyscall_clock_gettime_current_monotonic_clock.tv_nsec+=(current_tick-hsyscall_clock_gettime_current_monotonic_clock_last_tick)*1000000;
        hsyscall_clock_gettime_current_monotonic_clock_last_tick=current_tick;
        hsyscall_clock_gettime_current_monotonic_clock.tv_sec+=hsyscall_clock_gettime_current_monotonic_clock.tv_nsec/1000000000;
        hsyscall_clock_gettime_current_monotonic_clock.tv_nsec=hsyscall_clock_gettime_current_monotonic_clock.tv_nsec%1000000000;
        (*tp)=hsyscall_clock_gettime_current_monotonic_clock;
        hdefaults_mutex_unlock(NULL);
    }
    break;
    default:
    {

    }
    break;
    }

#if defined(HSYSCALL_CLOCK_GETTIME_HOOK)
    ret=HSYSCALL_CLOCK_GETTIME_HOOK(clockid,tp);
#endif

    return ret;
}
/*
 * 设置时钟
 */
int hsyscall_clock_settime(hclockid_t clockid, const htimespec_t *tp)
{
    int ret=-1;
    if(tp==NULL)
    {
        return ret;
    }
    switch(clockid)
    {
    case HCLOCK_REALTIME:
    {
        htimeval_t tv= {0};
        tv.tv_sec=tp->tv_sec;
        tv.tv_usec=tp->tv_nsec/1000;
        ret=hsyscall_settimeofday(&tv,NULL);
    }
    break;
    default:
    {

    }
    break;
    }

#if defined(HSYSCALL_CLOCK_SETTIME_HOOK)
    ret=HSYSCALL_CLOCK_SETTIME_HOOK(clockid,tp);
#endif

    return ret;
}



