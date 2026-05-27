#include "../base/core.h"

#if !defined(LIBMONO_RUNTIME_CRITICALSECTION_H)
#define LIBMONO_RUNTIME_CRITICALSECTION_H

/** \brief 进入临界区
 *
 * \return int 当前临界区加锁次数
 *
 */
int libmono_runtime_criticalsection_enter(void) LIBMONO_REENTRANT_FUNCTION_ATTRIBUTE;


/** \brief 退出临界区
 *
 * \return int 当前临界区加锁次数
 *
 */
int libmono_runtime_criticalsection_leave(void) LIBMONO_REENTRANT_FUNCTION_ATTRIBUTE;

#endif

