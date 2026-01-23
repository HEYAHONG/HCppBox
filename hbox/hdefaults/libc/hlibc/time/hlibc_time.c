/***************************************************************
 * Name:      hlibc_time.c
 * Purpose:   实现hlibc_time接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-01-22
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"
#include "hlibc_time.h"


#define HLIBC_TIME_SECSPERMIN   60ULL
#define HLIBC_TIME_MINSPERHOUR  60ULL
#define HLIBC_TIME_HOURSPERDAY  24ULL
#define HLIBC_TIME_SECSPERHOUR  (HLIBC_TIME_SECSPERMIN * HLIBC_TIME_MINSPERHOUR)
#define HLIBC_TIME_SECSPERDAY   (HLIBC_TIME_SECSPERHOUR * HLIBC_TIME_HOURSPERDAY)
#define HLIBC_TIME_DAYSPERWEEK  7ULL
#define HLIBC_TIME_MONSPERYEAR  12ULL

#define HLIBC_TIME_YEAR_BASE                        1900ULL
#define HLIBC_TIME_EPOCH_YEAR                       1970ULL
#define HLIBC_TIME_EPOCH_WDAY                       4ULL
#define HLIBC_TIME_EPOCH_YEARS_SINCE_LEAP           2ULL
#define HLIBC_TIME_EPOCH_YEARS_SINCE_CENTURY        70ULL
#define HLIBC_TIME_EPOCH_YEARS_SINCE_LEAP_CENTURY   370ULL

#define hlibc_time_isleap(y) ((((y) % 4) == 0 && ((y) % 100) != 0) || ((y) % 400) == 0)

/* Move epoch from 01.01.1970 to 01.03.0000 (yes, Year 0) - this is the first
 * day of a 400-year long "era", right after additional day of leap year.
 * This adjustment is required only for date calculation, so instead of
 * modifying time_t value (which would require 64-bit operations to work
 * correctly) it's enough to adjust the calculated number of days since epoch.
 */
#define HLIBC_TIME_EPOCH_ADJUSTMENT_DAYS        719468ULL
/* year to which the adjustment was made */
#define HLIBC_TIME_ADJUSTED_EPOCH_YEAR          0ULL
/* 1st March of year 0 is Wednesday */
#define HLIBC_TIME_ADJUSTED_EPOCH_WDAY          3ULL
/* there are 97 leap years in 400-year periods. ((400 - 97) * 365 + 97 * 366) */
#define HLIBC_TIME_DAYS_PER_ERA                 146097ULL
/* there are 24 leap years in 100-year periods. ((100 - 24) * 365 + 24 * 366) */
#define HLIBC_TIME_DAYS_PER_CENTURY             36524ULL
/* there is one leap year every 4 years */
#define HLIBC_TIME_DAYS_PER_4_YEARS             (3 * 365 + 366ULL)
/* number of days in a non-leap year */
#define HLIBC_TIME_DAYS_PER_YEAR                365ULL
/* number of days in January */
#define HLIBC_TIME_DAYS_IN_JANUARY              31ULL
/* number of days in non-leap February */
#define HLIBC_TIME_DAYS_IN_FEBRUARY             28ULL
/* number of years per era */
#define HLIBC_TIME_YEARS_PER_ERA                400ULL

htm_t * hlibc_gmtime_r (const htime_t *tim_p,htm_t * res)
{
    if(tim_p==NULL || res==NULL)
    {
        return NULL;
    }
    int64_t days=0, rem=0;
    const htime_t lcltime = *tim_p;
    int64_t era=0, weekday=0, year=0;
    uint64_t erayear=0, yearday=0, month=0, day=0;
    uint64_t eraday=0;

    days = lcltime / HLIBC_TIME_SECSPERDAY + HLIBC_TIME_EPOCH_ADJUSTMENT_DAYS;
    rem = lcltime % HLIBC_TIME_SECSPERDAY;
    if (rem < 0)
    {
        rem += HLIBC_TIME_SECSPERDAY;
        --days;
    }

    /* compute hour, min, and sec */
    res->tm_hour = (int) (rem / HLIBC_TIME_SECSPERHOUR);
    rem %= HLIBC_TIME_SECSPERHOUR;
    res->tm_min = (int) (rem / HLIBC_TIME_SECSPERMIN);
    res->tm_sec = (int) (rem % HLIBC_TIME_SECSPERMIN);

    /* compute day of week */
    if ((weekday = ((HLIBC_TIME_ADJUSTED_EPOCH_WDAY + days) % HLIBC_TIME_DAYSPERWEEK)) < 0)
    {
        weekday += HLIBC_TIME_DAYSPERWEEK;
    }
    res->tm_wday = weekday;

    /* compute year, month, day & day of year */
    /* for description of this algorithm see
     * http://howardhinnant.github.io/date_algorithms.html#civil_from_days */
    era = (days >= 0 ? days : days - (HLIBC_TIME_DAYS_PER_ERA - 1)) / HLIBC_TIME_DAYS_PER_ERA;
    eraday = days - era * HLIBC_TIME_DAYS_PER_ERA;   /* [0, 146096] */
    erayear = (eraday - eraday / (HLIBC_TIME_DAYS_PER_4_YEARS - 1) + eraday / HLIBC_TIME_DAYS_PER_CENTURY - eraday / (HLIBC_TIME_DAYS_PER_ERA - 1)) / 365;   /* [0, 399] */
    yearday = eraday - (HLIBC_TIME_DAYS_PER_YEAR * erayear + erayear / 4 - erayear / 100);   /* [0, 365] */
    month = (5 * yearday + 2) / 153;  /* [0, 11] */
    day = yearday - (153 * month + 2) / 5 + 1;    /* [1, 31] */
    month += month < 10 ? 2 : -10;
    year = HLIBC_TIME_ADJUSTED_EPOCH_YEAR + erayear + era * HLIBC_TIME_YEARS_PER_ERA + (month <= 1);
    res->tm_yday = yearday >= HLIBC_TIME_DAYS_PER_YEAR - HLIBC_TIME_DAYS_IN_JANUARY - HLIBC_TIME_DAYS_IN_FEBRUARY ?yearday - (HLIBC_TIME_DAYS_PER_YEAR - HLIBC_TIME_DAYS_IN_JANUARY - HLIBC_TIME_DAYS_IN_FEBRUARY) : yearday + HLIBC_TIME_DAYS_IN_JANUARY + HLIBC_TIME_DAYS_IN_FEBRUARY + hlibc_time_isleap(erayear);
    res->tm_year = year - HLIBC_TIME_YEAR_BASE;
    res->tm_mon = month;
    res->tm_mday = day;

    res->tm_isdst = 0;

    return (res);
}

htm_t * hlibc_localtime_r(const htime_t *tim_p,htm_t * res)
{
    if(tim_p==NULL || res ==NULL)
    {
        return NULL;
    }

    htime_t local_time=(*tim_p);

    /*
     * 时区偏移
     */
    bool is_dst=false;
    hlibc_time_timezone_info_t * timezone=hlibc_timezone_info_current();
    if(timezone!=NULL)
    {
        if(local_time >= timezone->dst_start && local_time < timezone->dst_end)
        {
            local_time-=timezone->dst_offset;
            is_dst=true;
        }
        else
        {
            local_time-=timezone->std_offset;
        }
    }

    //转换日历
    hlibc_gmtime_r(&local_time,res);

    /*
     * 添加夏令时标志
     */
    if(is_dst)
    {
        res->tm_isdst=1;
    }


    //返回日历
    return res;
}

static bool hmktime_time_compare_eq(const htm_t *res1,const htm_t *res2)
{
    if(res1!=NULL && res2!=NULL)
    {
        if(res1->tm_sec!=res2->tm_sec)
        {
            return false;
        }
        if(res1->tm_min!=res2->tm_min)
        {
            return false;
        }
        if(res1->tm_hour!=res2->tm_hour)
        {
            return false;
        }
        if(res1->tm_mday!=res2->tm_mday)
        {
            return false;
        }
        if(res1->tm_mon!=res2->tm_mon)
        {
            return false;
        }
        if(res1->tm_year!=res2->tm_year)
        {
            return false;
        }
        if(res1->tm_yday!=res2->tm_yday)
        {
            return false;
        }
        if(res1->tm_wday!=res2->tm_wday)
        {
            return false;
        }

        /*
        if(res1->tm_isdst!=res2->tm_isdst)
        {
            return false;
        }
        */

        return true;
    }
    return false;
}


static bool hmktime_time_compare_ge(const htm_t *res1,const htm_t *res2)
{
    if(res1!=NULL && res2!=NULL)
    {
        if(res1->tm_year<res2->tm_year)
        {
            return false;
        }
        if(res1->tm_year>res2->tm_year)
        {
            return true;
        }

        if(res1->tm_mon<res2->tm_mon)
        {
            return false;
        }
        if(res1->tm_mon>res2->tm_mon)
        {
            return true;
        }

        if(res1->tm_mday<res2->tm_mday)
        {
            return false;
        }
        if(res1->tm_mday>res2->tm_mday)
        {
            return true;
        }

        if(res1->tm_hour<res2->tm_hour)
        {
            return false;
        }
        if(res1->tm_hour>res2->tm_hour)
        {
            return true;
        }

        if(res1->tm_min<res2->tm_min)
        {
            return false;
        }
        if(res1->tm_min>res2->tm_min)
        {
            return true;
        }

        if(res1->tm_sec<res2->tm_sec)
        {
            return false;
        }
        if(res1->tm_sec>res2->tm_sec)
        {
            return true;
        }

        /*
         * 相等也返回true
         */
        return true;
    }
    return false;
}

htime_t hlibc_mktime(const htm_t *res)
{
    uint64_t ret=(htime_t)-1;
    if(res==NULL)
    {
        return ret;
    }

    /*
     * TODO:检查并修正res中的数据
     */


/*
 * 受限于tm_year,日历并不能处理64位时间
 */
#define HMKTIME_MAX_BITS ((sizeof(htime_t)*8 >(32))?48:((((htime_t)-1) < 0)?31:32))
    ret=0;
    for(size_t i=0; i<HMKTIME_MAX_BITS; i++)
    {
        ret|=(1ULL << (HMKTIME_MAX_BITS-1-i));
        htm_t temp_tm= {0};
        htime_t temp=ret;
        if(temp <= 0)
        {
            /*
             * 不支持小于0的时间
             */
            ret&=(~(1ULL <<  (HMKTIME_MAX_BITS-1-i)));
        }
        hlibc_localtime_r(&temp,&temp_tm);
        if(hmktime_time_compare_eq(&temp_tm,res))
        {
            break;
        }
        else
        {
            if(hmktime_time_compare_ge(&temp_tm,res))
            {
                ret&=(~(1ULL <<  (HMKTIME_MAX_BITS-1-i)));
            }
            if(i==(HMKTIME_MAX_BITS-1))
            {
                ret=(uint64_t)-1;
            }
        }
    }

    return (htime_t)ret;
}

htime_t hlibc_time(htime_t* arg)
{
    htime_t ret=(htime_t)-1;
    {
        htimespec_t tp={0};
        hclock_gettime(HCLOCK_REALTIME,&tp);
        ret=tp.tv_sec;
    }
    if(arg==NULL)
    {
        (*arg)=ret;
    }
    return ret;
}

hclock_t hlibc_clock(void)
{
    hclock_t ret=(hclock_t)-1;
    {
        htimespec_t tp={0};
        hclock_gettime(HCLOCK_MONOTONIC,&tp);
        ret=tp.tv_sec*HCLOCKS_PER_SEC+tp.tv_nsec*HCLOCKS_PER_SEC/1000000000;
    }
    return ret;
}

extern int hsprintf(char* buffer, const char* format, ...);
char * hlibc_asctime_r (const htm_t *tim_p,char * result)
{
    if(tim_p==NULL || result==NULL)
    {
        return NULL;
    }
    static const char day_name[7][3] =
    {
        "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
    };
    static const char mon_name[12][3] =
    {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun",
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };
    hsprintf(result, "%.3s %.3s%3d %.2d:%.2d:%.2d %d\n",day_name[tim_p->tm_wday],mon_name[tim_p->tm_mon],tim_p->tm_mday, tim_p->tm_hour, tim_p->tm_min,tim_p->tm_sec, 1900 + tim_p->tm_year);
    return result;
}

char * hlibc_ctime_r(const time_t * tim_p,char * result)
{
    if(tim_p==NULL || result==NULL)
    {
        return NULL;
    }
    htm_t temp={0};
    return hlibc_asctime_r (hlibc_localtime_r (tim_p, &temp), result);
}

/*
 * 全局时区信息，默认东8区，无夏令时
 */
static hlibc_time_timezone_info_t hlibc_time_timezone=
{
    (int32_t)(-8*HLIBC_TIME_SECSPERHOUR),
    (int32_t)(-8*HLIBC_TIME_SECSPERHOUR),
    0,
    0
};
hlibc_time_timezone_info_t *hlibc_timezone_info_global(void)
{
    return &hlibc_time_timezone;
}

hlibc_time_timezone_info_t *hlibc_timezone_info_current(void)
{
    hlibc_time_timezone_info_t *ret=hlibc_timezone_info_global();

#if defined(HLIBC_TIME_TIMEZONE_INFO_HOOK)
    ret=HLIBC_TIME_TIMEZONE_INFO_HOOK(ret);
#endif

    return ret;
}
