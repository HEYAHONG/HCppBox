/***************************************************************
 * Name:      hsunriseandsunset.h
 * Purpose:   声明hsunriseandsunset
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-04-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HSUNRISEANDSUNSET_H_INCLUDED__
#define __HSUNRISEANDSUNSET_H_INCLUDED__

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

/*
 * 天顶角定义
 */
#define HSUNRISEANDSUNSET_ZENITH_STANDARD     90.833   /**< 标准日出日落 */
#define HSUNRISEANDSUNSET_ZENITH_CIVIL         96.0     /**< 民用晨昏蒙影 */
#define HSUNRISEANDSUNSET_ZENITH_NAUTICAL     102.0     /**< 航海晨昏蒙影 */
#define HSUNRISEANDSUNSET_ZENITH_ASTRONOMICAL 108.0     /**< 天文晨昏蒙影 */

/*
 * 天文常数
 */
#define HSUNRISEANDSUNSET_J2000               2451545.0  /**< J2000.0 儒略日 */
#define HSUNRISEANDSUNSET_DAYS_PER_CENTURY    36525.0    /**< 每世纪天数 */

/*
 * 状态码
 */
#define HSUNRISEANDSUNSET_OK                  0         /**< 成功 */
#define HSUNRISEANDSUNSET_POLAR_NIGHT         (-1)       /**< 极夜 */
#define HSUNRISEANDSUNSET_POLAR_DAY            1         /**< 极昼 */
#define HSUNRISEANDSUNSET_ERR_PARAM          (-2)       /**< 参数错误 */

/*
 * 输入：年月日结构体
 */
typedef struct hsunriseandsunset_date
{
    int year;   /**< 年 (如 2026) */
    int month;  /**< 月 (1~12) */
    int day;    /**< 日 (1~31) */
} hsunriseandsunset_date_t;

/*
 * 输出：时分秒结构体 (允许负数)
 */
typedef struct hsunriseandsunset_time
{
    int h;      /**< 时 (可为负，如 -1 表示前一天 23:00) */
    int m;      /**< 分 (0~59 或 -59~0) */
    int s;      /**< 秒 (0~59 或 -59~0) */
} hsunriseandsunset_time_t;

/*
 * 结果结构体：日出时间 + 日落时间 (均为UTC)
 */
typedef struct hsunriseandsunset_result
{
    hsunriseandsunset_time_t sunrise;   /**< 日出时间 (UTC) */
    hsunriseandsunset_time_t sunset;    /**< 日落时间 (UTC) */
} hsunriseandsunset_result_t;


/** \brief 计算指定年/月/日的日出日落 (输出UTC时间)
 *
 * \param date   hsunriseandsunset_date_t  年月日输入
 * \param lat    double  纬度 (度, 北正南负)
 * \param lon    double  经度 (度, 东正西负)
 * \param result hsunriseandsunset_result_t* 结果输出 (UTC时间)
 * \param zenith double  天顶角 (度), <=0 时使用标准天顶角 (90.833)
 * \return int   状态码
 *
 */
int hsunriseandsunset_calculate_ymd(hsunriseandsunset_date_t date,double lat,double lon,hsunriseandsunset_result_t *result,double zenith);


/** \brief UTC时分秒 → 本地时分秒
 *
 * \param utc            hsunriseandsunset_time_t  UTC时间
 * \param tz_offset_hours int  时区偏移 (小时, 如 +8 表示东八区)
 * \return hsunriseandsunset_time_t  本地时间
 *
 * 例：UTC 22:00 + 8h = 本地 06:00 (次日)
 *    UTC -01:00 + 8h = 本地 07:00 (当天，-01:00表示前一天23:00)
 *
 */
hsunriseandsunset_time_t hsunriseandsunset_utc_to_local(hsunriseandsunset_time_t utc,int tz_offset_hours);


/** \brief 时分秒 → 当日秒数
 *
 * \param t   hsunriseandsunset_time_t
 * \return int 秒数 (可为负)
 *
 */
int hsunriseandsunset_time_to_sec(hsunriseandsunset_time_t t);


/** \brief 当日秒数 → 时分秒
 *
 * \param sec int 秒数 (可为负)
 * \return hsunriseandsunset_time_t
 *
 */
hsunriseandsunset_time_t hsunriseandsunset_sec_to_time(int sec);


#ifdef __cplusplus
}
#endif

#endif /* __HSUNRISEANDSUNSET_H_INCLUDED__ */
