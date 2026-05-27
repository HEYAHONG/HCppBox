#include "putchar.h"

int putchar(int ch)
{

#if defined(PUTCHAR)
    return PUTCHAR(ch);
#endif
    return ch;
}

