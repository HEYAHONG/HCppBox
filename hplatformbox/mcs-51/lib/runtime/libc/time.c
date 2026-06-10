#include "time.h"
#include "../tick.h"

#if !defined(TIME)
static LIBMONO_DATA_ATTRIBUTE int64_t libmono_runtime_libc_time_offset=0;
#endif

#if !defined(LIBMONO_RUNTIME_LIBC_NO_TIME)

time_t htime(time_t *timeptr)
{
    time_t ret=0;
#if defined(TIME)
    ret=TIME(timeptr);
#else
    libmono_runtime_criticalsection_enter();
    ret=(libmono_runtime_tick_get()+libmono_runtime_libc_time_offset)/1000;
    libmono_runtime_criticalsection_leave();
#endif
    if(timeptr!=NULL)
    {
        (*timeptr)=ret;
    }
    return ret;
}

#endif

#if !defined(TIME)

void libmono_runtime_libc_time_set(time_t curtime) LIBMONO_REENTRANT_FUNCTION_ATTRIBUTE
{
    libmono_runtime_criticalsection_enter();
    uint64_t curtick=curtime;
    curtick*=1000;
    libmono_runtime_libc_time_offset=curtick-libmono_runtime_tick_get();
    libmono_runtime_criticalsection_leave();
}

#endif

static void libmono_runtime_libc_time_check_tm(htm_t *timeptr) LIBMONO_REENTRANT_FUNCTION_ATTRIBUTE
{
    if(timeptr==NULL)
    {
        return;
    }

#if !__TIME_UNSIGNED
    if (timeptr->tm_sec<0) timeptr->tm_sec=0;
    if (timeptr->tm_min<0) timeptr->tm_min=0;
    if (timeptr->tm_hour<0) timeptr->tm_hour=0;
    if (timeptr->tm_wday<0) timeptr->tm_wday=0;
    if (timeptr->tm_mon<0) timeptr->tm_mon=0;
#endif

    if (timeptr->tm_sec>59) timeptr->tm_sec=59;
    if (timeptr->tm_min>59) timeptr->tm_min=59;
    if (timeptr->tm_hour>23) timeptr->tm_hour=23;
    if (timeptr->tm_wday>6) timeptr->tm_wday=6;
    if (timeptr->tm_mday<1) timeptr->tm_mday=1;
    else if (timeptr->tm_mday>31) timeptr->tm_mday=31;
    if (timeptr->tm_mon>11) timeptr->tm_mon=11;
    if (timeptr->tm_year<0) timeptr->tm_year=0;
}


#if !defined(LIBMONO_RUNTIME_TIMEZONE_OFFSET)
#define LIBMONO_RUNTIME_TIMEZONE_OFFSET (-8*3600)
#endif
static LIBMONO_CONST_DATA_ATTRIBUTE const char libmono_runtime_libc_monthdays[]= {31,28,31,30,31,30,31,31,30,31,30,31};
static LIBMONO_CONST_DATA_ATTRIBUTE const char *const libmono_runtime_libc_asctime_month[]= {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
static LIBMONO_CONST_DATA_ATTRIBUTE const char *const libmono_runtime_libc_asctime_day[]= {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
static LIBMONO_DATA_ATTRIBUTE time_t libmono_runtime_libc_epoch;
static LIBMONO_DATA_ATTRIBUTE uint16_t libmono_runtime_libc_year;
static LIBMONO_DATA_ATTRIBUTE uint8_t libmono_runtime_libc_month, libmono_runtime_libc_monthlength;
static LIBMONO_DATA_ATTRIBUTE uint32_t libmono_runtime_libc_days;
static LIBMONO_DATA_ATTRIBUTE htm_t  libmono_runtime_libc_tm_internal;
static LIBMONO_CONST_DATA_ATTRIBUTE int   libmono_runtime_libc_timezone_offset=LIBMONO_RUNTIME_TIMEZONE_OFFSET;

void libmono_runtime_libc_time_timezone_offset_set(int tz_offset) LIBMONO_REENTRANT_FUNCTION_ATTRIBUTE
{
    libmono_runtime_criticalsection_enter();
    libmono_runtime_libc_timezone_offset=tz_offset;
    libmono_runtime_criticalsection_leave();
}

#define LIBMONO_RUNTIME_LIBC_LEAP_YEAR(year) ((((year%4)==0) && ((year%100)!=0)) || ((year%400)==0))

htm_t *hgmtime_r(time_t * timep,htm_t * result) LIBMONO_REENTRANT_FUNCTION_ATTRIBUTE
{
    if(timep==NULL)
    {
        return NULL;
    }

    if(result==NULL)
    {
        result=&libmono_runtime_libc_tm_internal;
    }

    libmono_runtime_criticalsection_enter();

    libmono_runtime_libc_epoch=*timep;
    result->tm_sec=libmono_runtime_libc_epoch%60;
    libmono_runtime_libc_epoch/=60; // now it is minutes
    result->tm_min=libmono_runtime_libc_epoch%60;
    libmono_runtime_libc_epoch/=60; // now it is hours
    result->tm_hour=libmono_runtime_libc_epoch%24;
    libmono_runtime_libc_epoch/=24; // now it is days
    result->tm_wday=(libmono_runtime_libc_epoch+4)%7;

    libmono_runtime_libc_year=1970;
    libmono_runtime_libc_days=0;
    while((libmono_runtime_libc_days += (LIBMONO_RUNTIME_LIBC_LEAP_YEAR(libmono_runtime_libc_year) ? 366 : 365)) <= libmono_runtime_libc_epoch)
    {
        libmono_runtime_libc_year++;
    }
    result->tm_year=libmono_runtime_libc_year-1900;

    libmono_runtime_libc_days -= LIBMONO_RUNTIME_LIBC_LEAP_YEAR(libmono_runtime_libc_year) ? 366 : 365;
    libmono_runtime_libc_epoch -= libmono_runtime_libc_days; // now it is days in this year, starting at 0
    result->tm_yday=libmono_runtime_libc_epoch;

    libmono_runtime_libc_days=0;
    libmono_runtime_libc_month=0;
    libmono_runtime_libc_monthlength=0;
    for (libmono_runtime_libc_month=0; libmono_runtime_libc_month<12; libmono_runtime_libc_month++)
    {
        if (libmono_runtime_libc_month==1)
        {
            // februari
            if (LIBMONO_RUNTIME_LIBC_LEAP_YEAR(libmono_runtime_libc_year))
            {
                libmono_runtime_libc_monthlength=29;
            }
            else
            {
                libmono_runtime_libc_monthlength=28;
            }
        }
        else
        {
            libmono_runtime_libc_monthlength = libmono_runtime_libc_monthdays[libmono_runtime_libc_month];
        }

        if (libmono_runtime_libc_epoch>=libmono_runtime_libc_monthlength)
        {
            libmono_runtime_libc_epoch-=libmono_runtime_libc_monthlength;
        }
        else
        {
            break;
        }
    }
    result->tm_mon=libmono_runtime_libc_month;
    result->tm_mday=libmono_runtime_libc_epoch+1;
    result->tm_isdst=0;

    libmono_runtime_criticalsection_leave();

    return result;
}

htm_t *hlocaltime_r(time_t * timep,htm_t * result) LIBMONO_REENTRANT_FUNCTION_ATTRIBUTE
{
    if(timep==NULL)
    {
        return NULL;
    }
    libmono_runtime_criticalsection_enter();
    (*timep)-=libmono_runtime_libc_timezone_offset;
    htm_t *ret=hgmtime_r(timep,result);
    (*timep)+=libmono_runtime_libc_timezone_offset;
    libmono_runtime_criticalsection_leave();
    return ret;
}

time_t hmktime(htm_t * timeptr) LIBMONO_REENTRANT_FUNCTION_ATTRIBUTE
{
    if(timeptr==NULL)
    {
        return (time_t)-1;
    }

    libmono_runtime_criticalsection_enter();

    libmono_runtime_libc_year=timeptr->tm_year+1900;
    libmono_runtime_libc_month=timeptr->tm_mon;
    libmono_runtime_libc_time_check_tm(timeptr);

    // seconds from 1970 till 1 jan 00:00:00 this year
    libmono_runtime_libc_epoch= (libmono_runtime_libc_year-1970)*(60*60*24L*365);

    // add extra days for leap years
    for (int i=1970; i<libmono_runtime_libc_year; i++)
    {
        if (LIBMONO_RUNTIME_LIBC_LEAP_YEAR(i))
        {
            libmono_runtime_libc_epoch+= 60*60*24L;
        }
    }

    // add days for this year
    for (int i=0; i<libmono_runtime_libc_month; i++)
    {
        if (i==1 && LIBMONO_RUNTIME_LIBC_LEAP_YEAR(libmono_runtime_libc_year))
        {
            libmono_runtime_libc_epoch+= 60*60*24L*29;
        }
        else
        {
            libmono_runtime_libc_epoch+= 60*60*24L*libmono_runtime_libc_monthdays[i];
        }
    }

    libmono_runtime_libc_epoch+= (timeptr->tm_mday-1)*60*60*24L;
    libmono_runtime_libc_epoch+= timeptr->tm_hour*60*60L;
    libmono_runtime_libc_epoch+= timeptr->tm_min*60;
    libmono_runtime_libc_epoch+= timeptr->tm_sec;
    time_t ret=libmono_runtime_libc_epoch;
    /*
     * 修正时区
     */
    ret+=libmono_runtime_libc_timezone_offset;
    libmono_runtime_criticalsection_leave();
    return ret;
}


