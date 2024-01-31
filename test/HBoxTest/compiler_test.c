#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "hbox.h"

void c_compiler_test()
{
#ifdef __HAS_C11
    printf("hcompiler:C11\r\n");
#endif // __HAS_C11

#ifdef __HAS_C17
    printf("hcompiler:C17\r\n");
#endif // __HAS_C17

#ifdef __HAS_C23
    printf("hcompiler:C23\r\n");
#endif // __HAS_C23

#ifdef __STDC_VERSION__
    printf("hcompiler: C %d\r\n",(int)__STDC_VERSION__);
#endif // __STDC_VERSION__

}
