/***************************************************************
 * Name:      fcntl.c
 * Purpose:   实现moxiebox fcntl系统调用接口及标志转换
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
fcntl(int fd, int cmd, ...)
{
    long    arg = 0;
    int     moxiebox_cmd;
    long    ret;
    va_list ap;

    switch (cmd) {
    case F_DUPFD:
        moxiebox_cmd = MOXIEBOX_F_DUPFD;
        break;
    case F_GETFD:
        moxiebox_cmd = MOXIEBOX_F_GETFD;
        break;
    case F_SETFD:
        moxiebox_cmd = MOXIEBOX_F_SETFD;
        break;
    case F_GETFL:
        moxiebox_cmd = MOXIEBOX_F_GETFL;
        break;
    case F_SETFL:
        moxiebox_cmd = MOXIEBOX_F_SETFL;
        break;
#ifdef F_DUPFD_CLOEXEC
    case F_DUPFD_CLOEXEC:
        moxiebox_cmd = MOXIEBOX_F_DUPFD_CLOEXEC;
        break;
#endif
    default:
        errno = ENOSYS;
        return -1;
    }

    if (cmd != F_GETFD && cmd != F_GETFL) {
        va_start(ap, cmd);
        if (cmd == F_SETFD) {
            int flags = va_arg(ap, int);

            arg = 0;
#ifdef FD_CLOEXEC
            if (flags & FD_CLOEXEC)
                arg |= MOXIEBOX_FD_CLOEXEC;
#endif
        } else if (cmd == F_SETFL) {
            arg = moxiebox_open_flags(va_arg(ap, int));
        } else {
            arg = (long)va_arg(ap, int);
        }
        va_end(ap);
    }

    ret = __moxiebox_fcntl(fd, moxiebox_cmd, arg);
    if (ret < 0) {
        errno = (int)-ret;
        return -1;
    }

    if (cmd == F_GETFL)
        return moxiebox_open_flags_from_guest((int)ret);
    if (cmd == F_GETFD) {
        int flags = 0;

#ifdef FD_CLOEXEC
        if (ret & MOXIEBOX_FD_CLOEXEC)
            flags |= FD_CLOEXEC;
#endif
        return flags;
    }
    return (int)ret;
}
