#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "hbox.h"


int c_defaults_test(int argc,const char *argv[])
{
#if defined(HAVE_ATOMIC)
    {
        hatomic(int) a;
        a=0;
        a++;
        a&=0;
        a|=1;
        printf("hdefaults(c):hatomic addr=%p,value=%d\r\n",&a,(int)a);
    }
#endif
    return 0;
}
