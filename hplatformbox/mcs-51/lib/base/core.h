#include "registers.h"
#include "model.h"
#include "globalconfig.h"
#if !defined(LIBMONO_BASE_CORE_H)
#define LIBMONO_BASE_CORE_H

#ifndef LIBMONO_BASE_CORE_DISABLE_INTERRUPT
#define LIBMONO_BASE_CORE_DISABLE_INTERRUPT()   EA=0
#endif // LIBMONO_BASE_CORE_DISABLE_INTERRUPT

static inline void libmono_base_core_disable_interrupt(void) LIBMONO_REENTRANT_FUNCTION_ATTRIBUTE
{
    LIBMONO_BASE_CORE_DISABLE_INTERRUPT();
}

#ifndef LIBMONO_BASE_CORE_ENABLE_INTERRUPT
#define LIBMONO_BASE_CORE_ENABLE_INTERRUPT()   EA=1
#endif // LIBMONO_BASE_CORE_ENABLE_INTERRUPT

static inline void libmono_base_core_enable_interrupt(void) LIBMONO_REENTRANT_FUNCTION_ATTRIBUTE
{
    LIBMONO_BASE_CORE_ENABLE_INTERRUPT();
}

#endif

