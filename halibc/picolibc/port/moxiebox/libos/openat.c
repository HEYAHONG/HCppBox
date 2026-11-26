/***************************************************************
 * Name:      openat.c
 * Purpose:   实现moxiebox openat系统调用接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-26
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>

#include "moxiebox_syscall.h"
#include "moxiebox_libos.h"

int
openat(int dirfd, const char *path, int flags, ...)
{
    int     mode = 0;
    long    ret;
    va_list ap;

    if (flags & (O_CREAT
#ifdef O_TMPFILE
                 | O_TMPFILE
#endif
                 )) {
        va_start(ap, flags);
        mode = va_arg(ap, int);
        va_end(ap);
    }

#ifdef AT_FDCWD
    if (dirfd == AT_FDCWD)
        dirfd = MOXIEBOX_AT_FDCWD;
#endif

    ret = __moxiebox_openat(dirfd, path, moxiebox_open_flags(flags), mode);
    if (ret < 0) {
        errno = (int)-ret;
        return -1;
    }
    return (int)ret;
}
