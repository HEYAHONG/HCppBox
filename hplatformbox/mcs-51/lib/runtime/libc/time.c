#include "time.h"
#include "../tick.h"

#if !defined(TIME)
static LIBMONO_DATA_ATTRIBUTE int64_t libmono_runtime_libc_time_offset=0;
#endif

time_t time(time_t *timeptr)
{
    time_t ret=0;
#if defined(TIME)
    ret=TIME(timeptr);
#else
    libmono_runtime_criticalsection_enter();
    ret=(libmono_runtime_tick_get()+libmono_runtime_libc_time_offset)/1000;
    libmono_runtime_criticalsection_leave();
#endif
    if(timeptr!=NULL)
    {
        (*timeptr)=ret;
    }
    return ret;
}

#if !defined(TIME)

void libmono_runtime_libc_time_set(time_t curtime) LIBMONO_REENTRANT_FUNCTION_ATTRIBUTE
{
    libmono_runtime_criticalsection_enter();
    uint64_t curtick=curtime;
    curtick*=1000;
    libmono_runtime_libc_time_offset=curtick-libmono_runtime_tick_get();
    libmono_runtime_criticalsection_leave();
}

#endif

