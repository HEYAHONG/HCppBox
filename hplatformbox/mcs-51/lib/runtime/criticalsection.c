#include "criticalsection.h"

#if defined(LIBMONO_BASE_CORE_DISABLE_INTERRUPT_EA) && defined(LIBMONO_BASE_CORE_ENABLE_INTERRUPT_EA)
static  LIBMONO_DATA_ATTRIBUTE int libmono_runtime_criticalsection_count=0;
#else
static  LIBMONO_DATA_ATTRIBUTE uint8_t libmono_runtime_criticalsection_count=0;
#endif

int libmono_runtime_criticalsection_enter(void) LIBMONO_REENTRANT_FUNCTION_ATTRIBUTE
{
#if !defined(LIBMONO_BASE_CORE_DISABLE_INTERRUPT_EA)
    if(libmono_runtime_criticalsection_count==0)
#endif
    {
        libmono_base_core_disable_interrupt();
    }
    libmono_runtime_criticalsection_count++;
    return libmono_runtime_criticalsection_count;
}

int libmono_runtime_criticalsection_leave(void) LIBMONO_REENTRANT_FUNCTION_ATTRIBUTE
{
    libmono_runtime_criticalsection_count--;
    int ret=libmono_runtime_criticalsection_count;
    if(libmono_runtime_criticalsection_count==0)
    {
        libmono_base_core_enable_interrupt();
    }
    return ret;
}

