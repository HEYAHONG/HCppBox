/***************************************************************
 * Name:      h3rdparty_printf.c
 * Purpose:   引入第三方源代码文件
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-09-27
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "h3rdparty.h"

#ifdef _putchar
#undef  _putchar
#endif
#define _putchar hputchar
static void (*putchar_cb)(char c)=NULL;
void hprintf_set_callback(void (*cb)(char c))
{
    putchar_cb=cb;
}
void _putchar(char c)
{
    if(putchar_cb!=NULL)
    {
        putchar_cb(c);
    }
}

#ifdef printf_
#undef printf_
#endif // printf_
#define printf_ hprintf

#ifdef sprintf_
#undef sprintf_
#endif // sprintf_
#define sprintf_ hsprintf

#ifdef snprintf_
#undef snprintf_
#endif // snprintf_
#define snprintf_ hsnprintf

#ifdef vsnprintf_
#undef vsnprintf_
#endif // vsnprintf_
#define vsnprintf_ hvsnprintf

#ifdef vprintf_
#undef vprintf_
#endif // vprintf_
#define vprintf_ hvprintf

#ifdef fctprintf
#undef fctprintf
#endif // fctprintf
#define fctprintf hfctprintf


#include "3rdparty/printf/printf.h"
#include "3rdparty/printf/printf.c"



