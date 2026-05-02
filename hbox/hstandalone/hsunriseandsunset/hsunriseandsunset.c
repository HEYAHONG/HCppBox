/***************************************************************
 * Name:      hsunriseandsunset.h
 * Purpose:   实现hsunriseandsunset
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-04-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hsunriseandsunset.h"
#include <math.h>
#include <stdlib.h>

#ifndef HSUNRISEANDSUNSET_M_PI
#define HSUNRISEANDSUNSET_M_PI  3.14159265358979323846
#endif

#define HSUNRISEANDSUNSET_RAD  (HSUNRISEANDSUNSET_M_PI / 180.0)
#define HSUNRISEANDSUNSET_DEG  (180.0 / HSUNRISEANDSUNSET_M_PI)

static double hsunriseandsunset_fmod360(double x)
{
    double r = fmod(x, 360.0);
    return (r < 0) ? r + 360.0 : r;
}

static double hsunriseandsunset_jd_to_jcent(double jd)
{
    return (jd - HSUNRISEANDSUNSET_J2000) / HSUNRISEANDSUNSET_DAYS_PER_CENTURY;
}

static int hsunriseandsunset_day_of_year(int year, int month, int day)
{
    static const int md[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int doy = day;
    for (int i = 1; i < month; i++)
        doy += md[i];
    if (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0))
        if (month > 2)
            doy++;
    return doy;
}

/* Solar declination [degrees]: complete formula with center equation C */
static double hsunriseandsunset_solar_declination(double T)
{
    double L0 = hsunriseandsunset_fmod360(280.46646 + 36000.76983 * T);
    double M  = hsunriseandsunset_fmod360(357.52911 + 35999.05029 * T);
    double Mr = M * HSUNRISEANDSUNSET_RAD;

    double C = (1.914602 - 0.004817 * T - 0.000014 * T * T) * sin(Mr)
               + (0.019993 - 0.000101 * T) * sin(2.0 * Mr)
               + 0.000290 * sin(3.0 * Mr);

    double lambda = L0 + C;

    double eps = (23.439291 - 0.013004167 * T - 0.000000164 * T * T
                  + 0.000000504 * T * T * T) * HSUNRISEANDSUNSET_RAD;

    return asin(sin(eps) * sin(lambda * HSUNRISEANDSUNSET_RAD)) * HSUNRISEANDSUNSET_DEG;
}

/* Equation of Time [arcminutes]: NOAA formula */
static double hsunriseandsunset_equation_of_time(double N)
{
    double B  = 360.0 * (N - 81.0) / 365.0;
    double Br = B * HSUNRISEANDSUNSET_RAD;
    return 9.87 * sin(2.0 * Br) - 7.53 * cos(Br) - 1.5 * sin(Br);
}

/* Calendar date -> Astronomical JDN (noon JD) */
static double date_to_jd(int year, int month, int day)
{
    int a, b;
    if (month <= 2) {
        year -= 1;
        month += 12;
    }
    a = year / 100;
    b = 2 - a + a / 4;
    return floor(365.25 * (year + 4716)) + floor(30.6001 * (month + 1)) + day + b - 1524.0;
}

/* seconds -> {h, m, s} (允许负数) */
hsunriseandsunset_time_t hsunriseandsunset_sec_to_time(int sec)
{
    hsunriseandsunset_time_t r;
    int sign = (sec < 0) ? -1 : 1;
    int abs_sec = (sec < 0) ? -sec : sec;
    r.h = sign * (abs_sec / 3600);
    abs_sec %= 3600;
    r.m = sign * (abs_sec / 60);
    r.s = sign * (abs_sec % 60);
    return r;
}

/* {h, m, s} -> seconds (允许负数，各分量直接相加) */
int hsunriseandsunset_time_to_sec(hsunriseandsunset_time_t t)
{
    return t.h * 3600 + t.m * 60 + t.s;
}

/* UTC时分秒 → 本地时分秒 */
hsunriseandsunset_time_t hsunriseandsunset_utc_to_local(hsunriseandsunset_time_t utc,int tz_offset_hours)
{
    int sec = hsunriseandsunset_time_to_sec(utc);
    sec += tz_offset_hours * 3600;
    return hsunriseandsunset_sec_to_time(sec);
}

/* Core calculation: 输出UTC时间 */
int hsunriseandsunset_calculate_ymd(hsunriseandsunset_date_t date,double lat,double lon,hsunriseandsunset_result_t *result,double zenith)
{
    if (result == NULL)
        return HSUNRISEANDSUNSET_ERR_PARAM;

    double jd   = date_to_jd(date.year, date.month, date.day);
    double T    = hsunriseandsunset_jd_to_jcent(jd);
    double N    = (double)hsunriseandsunset_day_of_year(date.year, date.month, date.day);
    double eot  = hsunriseandsunset_equation_of_time(N);          /* arcminutes */
    double decl = hsunriseandsunset_solar_declination(T);         /* degrees */

    if (zenith <= 0.0)
        zenith = HSUNRISEANDSUNSET_ZENITH_STANDARD;

    double lr = lat * HSUNRISEANDSUNSET_RAD;
    double dr = decl * HSUNRISEANDSUNSET_RAD;
    double zr = zenith * HSUNRISEANDSUNSET_RAD;

    /* Hour angle [degrees] */
    double cos_omega = (cos(zr) - sin(lr) * sin(dr)) / (cos(lr) * cos(dr));
    double omega;
    if (cos_omega < -1.0)
        omega = 180.0;                          /* polar night */
    else if (cos_omega > 1.0)
        omega = 0.0;                            /* polar day  */
    else
        omega = acos(cos_omega) * HSUNRISEANDSUNSET_DEG;

    /* Solar noon [UTC hours] = 12 - lon/15 - EoT/60 */
    double transit = 12.0 - lon / 15.0 - eot / 60.0;

    /* Sunrise / Sunset [UTC hours] (不做时区转换) */
    double rise_h = transit - omega / 15.0;
    double set_h  = transit + omega / 15.0;

    /* 转换为秒 (允许负数) */
    int rise_sec = (int)floor(rise_h * 3600.0 + 0.5);
    int set_sec  = (int)floor(set_h  * 3600.0 + 0.5);

    result->sunrise = hsunriseandsunset_sec_to_time(rise_sec);
    result->sunset  = hsunriseandsunset_sec_to_time(set_sec);
    return HSUNRISEANDSUNSET_OK;
}
