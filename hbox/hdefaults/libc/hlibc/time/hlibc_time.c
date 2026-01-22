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


#define HLIBC_TIME_SECSPERMIN   60L
#define HLIBC_TIME_MINSPERHOUR  60L
#define HLIBC_TIME_HOURSPERDAY  24L
#define HLIBC_TIME_SECSPERHOUR  (HLIBC_TIME_SECSPERMIN * HLIBC_TIME_MINSPERHOUR)
#define HLIBC_TIME_SECSPERDAY   (HLIBC_TIME_SECSPERHOUR * HLIBC_TIME_HOURSPERDAY)
#define HLIBC_TIME_DAYSPERWEEK  7
#define HLIBC_TIME_MONSPERYEAR  12

#define HLIBC_TIME_YEAR_BASE                        1900
#define HLIBC_TIME_EPOCH_YEAR                       1970
#define HLIBC_TIME_EPOCH_WDAY                       4
#define HLIBC_TIME_EPOCH_YEARS_SINCE_LEAP           2
#define HLIBC_TIME_EPOCH_YEARS_SINCE_CENTURY        70
#define HLIBC_TIME_EPOCH_YEARS_SINCE_LEAP_CENTURY   370

#define hlibc_time_isleap(y) ((((y) % 4) == 0 && ((y) % 100) != 0) || ((y) % 400) == 0)

/* Move epoch from 01.01.1970 to 01.03.0000 (yes, Year 0) - this is the first
 * day of a 400-year long "era", right after additional day of leap year.
 * This adjustment is required only for date calculation, so instead of
 * modifying time_t value (which would require 64-bit operations to work
 * correctly) it's enough to adjust the calculated number of days since epoch.
 */
#define HLIBC_TIME_EPOCH_ADJUSTMENT_DAYS        719468L
/* year to which the adjustment was made */
#define HLIBC_TIME_ADJUSTED_EPOCH_YEAR          0
/* 1st March of year 0 is Wednesday */
#define HLIBC_TIME_ADJUSTED_EPOCH_WDAY          3
/* there are 97 leap years in 400-year periods. ((400 - 97) * 365 + 97 * 366) */
#define HLIBC_TIME_DAYS_PER_ERA                 146097L
/* there are 24 leap years in 100-year periods. ((100 - 24) * 365 + 24 * 366) */
#define HLIBC_TIME_DAYS_PER_CENTURY             36524L
/* there is one leap year every 4 years */
#define HLIBC_TIME_DAYS_PER_4_YEARS             (3 * 365 + 366)
/* number of days in a non-leap year */
#define HLIBC_TIME_DAYS_PER_YEAR                365
/* number of days in January */
#define HLIBC_TIME_DAYS_IN_JANUARY              31
/* number of days in non-leap February */
#define HLIBC_TIME_DAYS_IN_FEBRUARY             28
/* number of years per era */
#define HLIBC_TIME_YEARS_PER_ERA                400

htm_t * hgmtime_r (const htime_t *tim_p,htm_t * res)
{
    if(tim_p==NULL || res==NULL)
    {
        return NULL;
    }
    long days=0, rem=0;
    const htime_t lcltime = *tim_p;
    int era=0, weekday=0, year=0;
    unsigned erayear=0, yearday=0, month=0, day=0;
    unsigned long eraday=0;

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


