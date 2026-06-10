#include <time.h>
#if !defined(LIBMONO_RUNTIME_LIB_TIME_H)
#define LIBMONO_RUNTIME_LIB_TIME_H

/** \brief 获取当前时间
 *
 * \param timeptr time_t* 当前时间的指针
 * \return time_t 当前时间
 *
 */
time_t htime(time_t *timeptr);

#if !defined(TIME) && !defined(LIBMONO_RUNTIME_LIBC_NO_TIME)

void libmono_runtime_libc_time_set(time_t curtime) LIBMONO_REENTRANT_FUNCTION_ATTRIBUTE;

#endif

void libmono_runtime_libc_time_timezone_offset_set(int tz_offset) LIBMONO_REENTRANT_FUNCTION_ATTRIBUTE;

typedef struct tm htm_t;

/** \brief 获取UTC时间
 *
 * \param timep time_t* 时间戳指针，不可为NULL
 * \param result htm_t* 时间结构体指针，不可为NULL
 * \return htm_t* 返回的时间结构体指针
 *
 */
htm_t *hgmtime_r(time_t * timep,htm_t * result) LIBMONO_REENTRANT_FUNCTION_ATTRIBUTE;
/** \brief 获取本地时间
 *
 * \param timep time_t* 时间戳指针，不可为NULL
 * \param result htm_t* 时间结构体指针，不可为NULL
 * \return htm_t* 返回的时间结构体指针
 *
 */
htm_t *hlocaltime_r(time_t * timep,htm_t * result) LIBMONO_REENTRANT_FUNCTION_ATTRIBUTE;

/** \brief 通过本地时间获取时间戳
 *
 * \param timeptr htm_t* 时间结构体指针
 * \return time_t 时间戳
 *
 */
time_t hmktime(htm_t * timeptr) LIBMONO_REENTRANT_FUNCTION_ATTRIBUTE;

#endif

