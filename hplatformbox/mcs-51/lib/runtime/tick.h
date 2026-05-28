#include "criticalsection.h"
#include "stdint.h"

#if !defined(LIBMONO_RUNTIME_TICK_H)
#define LIBMONO_RUNTIME_TICK_H

typedef uint64_t libmono_runtime_tick_t;

/** \brief 节拍增加
 *
 * 注意：一般只在一处调用此函数，多处调用可能将导致节拍混乱
 *
 * \param time_us uint8_t 待增加的节拍（单位:豪秒）
 *
 */
void libmono_runtime_tick_inc(uint8_t time_ms) LIBMONO_FUNCTION_ATTRIBUTE;

/** \brief 获取节拍
 *
 * \return libmono_runtime_tick_t 当前节拍（单位：毫秒）
 *
 */
libmono_runtime_tick_t libmono_runtime_tick_get(void) LIBMONO_REENTRANT_FUNCTION_ATTRIBUTE;



#endif
