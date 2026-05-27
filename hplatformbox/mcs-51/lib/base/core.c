
#include "core.h"

int libmono_base_core_serial_read(void) LIBMONO_REENTRANT_FUNCTION_ATTRIBUTE
{
    int ret=-1;
    if(RI==0)
    {
        return ret;
    }
    else
    {
        ret=(uint8_t)SBUF;
        RI=0;
        return ret;
    }
}

int libmono_base_core_serial_write(int ch) LIBMONO_REENTRANT_FUNCTION_ATTRIBUTE
{
    TI=0;
    SBUF=(uint8_t)ch;
    while(TI==0)
    {

    }
    return ch;
}

