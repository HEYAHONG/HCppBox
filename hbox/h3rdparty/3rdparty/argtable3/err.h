#include "hcompiler.h"
#include "hdefaults.h"
#include "stdarg.h"
#include "stdio.h"

__USED
static void warnx(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr,fmt,ap);
    va_end(ap);
}
