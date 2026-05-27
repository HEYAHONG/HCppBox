#include "getchar.h"

int getchar(void)
{
    int ret=-1;
#if defined(GETCHAR)
    ret=GETCHAR();
#elif defined(LIBMONO_RUNTIME_LIBC_STDIO_SERIAL)
    libmono_runtime_criticalsection_enter();
    ret=libmono_base_core_serial_read();
    libmono_runtime_criticalsection_leave();
#endif
    return ret;
}
