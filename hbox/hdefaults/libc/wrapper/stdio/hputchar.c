/***************************************************************
 * Name:      hputchar.c
 * Purpose:   实现hputchar接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-02
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hputchar.h"
#include "hdefaults.h"

#ifdef HPUTCHAR
extern int HPUTCHAR(int ch);
#endif // HPUTCHAR


int hputchar(int ch)
{
#if defined(HPUTCHAR)
    return HPUTCHAR(ch);
#elif ((HDEFAULTS_LIBC_OPTIMIZE_LEVEL) > 0) && !defined(HDEFAULTS_LIBC_TINY) && !defined(HLIBC_NO_STDIO)
    return hlibc_putchar(ch);
#else
    return putchar(ch);
#endif
}


