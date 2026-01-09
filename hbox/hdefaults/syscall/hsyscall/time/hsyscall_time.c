/***************************************************************
 * Name:      hsyscall_time.c
 * Purpose:   实现hsyscall_time接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-01-09
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hsyscall_time.h"


static hgettimeofday_timeval_t   hsyscall_gettimeofday_current_timeval= {0};
static hgettimeofday_timezone_t  hsyscall_gettimeofday_current_timezone= {0};
static void hsyscall_gettimeofday_update_timeval(void)
{
    hdefaults_mutex_lock(NULL);
    hgettimeofday_timeval_t old_val=hsyscall_gettimeofday_current_timeval;
    hdefaults_mutex_unlock(NULL);
    static hdefaults_tick_t hgettimeofday_last_tick=0;
    hdefaults_tick_t current_tick=hdefaults_tick_get();
    if(current_tick!=hgettimeofday_last_tick)
    {
        old_val.tv_usec+=((current_tick-hgettimeofday_last_tick)*1000);
        old_val.tv_sec+=(old_val.tv_usec/1000000);
        old_val.tv_usec=(old_val.tv_usec%1000000);
        hdefaults_mutex_lock(NULL);
        hgettimeofday_last_tick=current_tick;
        hsyscall_gettimeofday_current_timeval=old_val;
        hdefaults_mutex_unlock(NULL);
    }
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
    return 0;
}




