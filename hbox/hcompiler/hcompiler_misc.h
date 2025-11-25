/***************************************************************
 * Name:      hcompiler_misc.h
 * Purpose:   定义杂项
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-11-25
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HCOMPILER_MISC_H__
#define __HCOMPILER_MISC_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/*
 * 在C++代码中，可能不能获取到C编译器C语言版本(编译库时)，定义一个函数获取__STDC_VERSION__
 */

/** \brief  获取C语言版本
 *
 * \return long C语言版本
 *
 */
long hcompiler_get_stdc_version(void);

/** \brief 获取编译日期
 *
 * \return const char* 日期
 *
 */
const char * hcompiler_get_date(void);

/** \brief 获取日期年
 *
 * \return int 年
 *
 */
int hcompiler_get_date_year(void);

/** \brief 获取日期月
 *
 * \return int 月 1-12
 *
 */
int hcompiler_get_date_month(void);

/** \brief  获取日期日
 *
 * \return int 日
 *
 */
int hcompiler_get_date_day(void);

/** \brief 获取编译时间
 *
 * \return const char* 时间
 *
 */
const char * hcompiler_get_time(void);

/** \brief 获取编译时间时
 *
 * \return int 时
 *
 */
int hcompiler_get_time_hour(void);

/** \brief 获取编译时间分
 *
 * \return int 分
 *
 */
int hcompiler_get_time_minute(void);

/** \brief 获取编译时间秒
 *
 * \return int 秒
 *
 */
int hcompiler_get_time_second(void);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif
