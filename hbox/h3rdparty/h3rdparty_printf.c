/***************************************************************
 * Name:      h3rdparty_printf.c
 * Purpose:   引入第三方源代码文件
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-09-27
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "h3rdparty.h"
#include "hdefaults.h"
#if defined(HDEFAULTS_OS_WINDOWS) || defined(HDEFAULTS_OS_UNIX)
extern int hputchar( int ch );
#endif

#ifdef _putchar
#undef  _putchar
#endif
#define _putchar h3rdparty_printf_putchar
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
    else
    {
#if defined(HDEFAULTS_OS_WINDOWS) || defined(HDEFAULTS_OS_UNIX)
        hputchar(c);
#endif
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

#define _vsnprintf hbox_vsnprintf

#include "3rdparty/printf/printf.c"

int hvfctprintf(void (*out)(char character, void* arg), void* arg, const char* format, va_list va)
{
    const out_fct_wrap_type out_fct_wrap = { out, arg };
    return _vsnprintf(_out_fct, (char*)(uintptr_t)&out_fct_wrap, (size_t)-1, format, va);
}

