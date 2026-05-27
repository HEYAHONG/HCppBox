#include "putchar.h"

int putchar(int ch)
{

#if defined(PUTCHAR)
    return PUTCHAR(ch);
#elif defined(LIBMONO_RUNTIME_LIBC_STDIO_SERIAL)
    libmono_runtime_criticalsection_enter();
    libmono_base_core_serial_write(ch);
    libmono_runtime_criticalsection_leave();
#endif
    return ch;
}

