#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "hbox.h"
#ifdef HAVE_STDATOMIC_H
#include "stdatomic.h"
#endif // HAVE_STDATOMIC_H

void c_compiler_test()
{
#ifdef __HAS_C11
    printf("hcompiler:C11\r\n");
#endif // __HAS_C11

#ifdef HAVE_STDATOMIC_H
    printf("hcompiler:have stdatomic.h\r\n");
#endif // HAVE_STDATOMIC_H

#ifdef __HAS_C17
    printf("hcompiler:C17\r\n");
#endif // __HAS_C17

#ifdef __HAS_C23
    printf("hcompiler:C23\r\n");
#endif // __HAS_C23

#ifdef __STDC_VERSION__
    printf("hcompiler:CVer=%d\r\n",(int)__C_VERSION);
#endif // __STDC_VERSION__

}
