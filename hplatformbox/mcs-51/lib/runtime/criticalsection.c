#include "criticalsection.h"

static  LIBMONO_DATA_ATTRIBUTE int libmono_runtime_criticalsection_count=0;

int libmono_runtime_criticalsection_enter(void) LIBMONO_REENTRANT_FUNCTION_ATTRIBUTE
{
    if(libmono_runtime_criticalsection_count==0)
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

