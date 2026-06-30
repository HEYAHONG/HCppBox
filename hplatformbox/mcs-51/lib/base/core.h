#include "registers.h"
#include "model.h"
#include "globalconfig.h"
#if !defined(LIBMONO_BASE_CORE_H)
#define LIBMONO_BASE_CORE_H

#ifndef LIBMONO_BASE_CORE_DISABLE_INTERRUPT
#define LIBMONO_BASE_CORE_DISABLE_INTERRUPT()   EA=0
#define LIBMONO_BASE_CORE_DISABLE_INTERRUPT_EA  1
#endif // LIBMONO_BASE_CORE_DISABLE_INTERRUPT

/** \brief 关闭中断
 *
 *
 */
static inline void libmono_base_core_disable_interrupt(void) LIBMONO_REENTRANT_FUNCTION_ATTRIBUTE
{
    LIBMONO_BASE_CORE_DISABLE_INTERRUPT();
}

#ifndef LIBMONO_BASE_CORE_ENABLE_INTERRUPT
#define LIBMONO_BASE_CORE_ENABLE_INTERRUPT()   EA=1
#define LIBMONO_BASE_CORE_ENABLE_INTERRUPT_EA  1
#endif // LIBMONO_BASE_CORE_ENABLE_INTERRUPT

/** \brief 打开中断
 *
 *
 */
static inline void libmono_base_core_enable_interrupt(void) LIBMONO_REENTRANT_FUNCTION_ATTRIBUTE
{
    LIBMONO_BASE_CORE_ENABLE_INTERRUPT();
}

/** \brief 串口读取
 *
 * 注意：此函数会影响RI位
 *
 * \return int 失败返回-1，成功返回相应数据
 *
 */
int libmono_base_core_serial_read(void) LIBMONO_REENTRANT_FUNCTION_ATTRIBUTE;


/** \brief 串口写入
 *
 * 注意：此函数会影响TI位
 *
 * \param ch uint8_t 数据
 * \return int 失败返回-1
 *
 */
int libmono_base_core_serial_write(int ch) LIBMONO_REENTRANT_FUNCTION_ATTRIBUTE;

#endif

