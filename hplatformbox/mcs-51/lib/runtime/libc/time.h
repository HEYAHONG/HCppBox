#include <time.h>
#if !defined(LIBMONO_RUNTIME_LIB_TIME_H)
#define LIBMONO_RUNTIME_LIB_TIME_H

/** \brief 获取当前时间
 *
 * \param timeptr time_t* 当前时间的指针
 * \return time_t 当前时间
 *
 */
time_t time(time_t *timeptr);

#if !defined(TIME) && !defined(LIBMONO_RUNTIME_LIBC_NO_TIME)

void libmono_runtime_libc_time_set(time_t curtime) LIBMONO_REENTRANT_FUNCTION_ATTRIBUTE;

#endif

#endif

