#include "getchar.h"

int getchar(void)
{
    int ret=-1;
#if defined(GETCHAR)
    ret=GETCHAR();
#endif
    return ret;
}
