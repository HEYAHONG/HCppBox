/***************************************************************
 * Name:      open.c
 * Purpose:   实现moxiebox open系统调用接口及打开标志转换
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-24
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>

#include "moxiebox_syscall.h"
#include "moxiebox_libos.h"

/*
 * Translate picolibc/newlib open flags into the moxiebox (Linux)
 * flag values used by the virtual machine.
 */
int
moxiebox_open_flags(int flags)
{
    int ret = flags & O_ACCMODE;

#ifdef O_CREAT
    if (flags & O_CREAT)
        ret |= MOXIEBOX_O_CREAT;
#endif
#ifdef O_EXCL
    if (flags & O_EXCL)
        ret |= MOXIEBOX_O_EXCL;
#endif
#ifdef O_NOCTTY
    if (flags & O_NOCTTY)
        ret |= MOXIEBOX_O_NOCTTY;
#endif
#ifdef O_TRUNC
    if (flags & O_TRUNC)
        ret |= MOXIEBOX_O_TRUNC;
#endif
#ifdef O_APPEND
    if (flags & O_APPEND)
        ret |= MOXIEBOX_O_APPEND;
#endif
#ifdef O_NONBLOCK
    if (flags & O_NONBLOCK)
        ret |= MOXIEBOX_O_NONBLOCK;
#endif
#ifdef O_DSYNC
    if (flags & O_DSYNC)
        ret |= MOXIEBOX_O_DSYNC;
#endif
#ifdef O_DIRECT
    if (flags & O_DIRECT)
        ret |= MOXIEBOX_O_DIRECT;
#endif
#ifdef O_LARGEFILE
    if (flags & O_LARGEFILE)
        ret |= MOXIEBOX_O_LARGEFILE;
#endif
#ifdef O_DIRECTORY
    if (flags & O_DIRECTORY)
        ret |= MOXIEBOX_O_DIRECTORY;
#endif
#ifdef O_NOFOLLOW
    if (flags & O_NOFOLLOW)
        ret |= MOXIEBOX_O_NOFOLLOW;
#endif
#ifdef O_NOATIME
    if (flags & O_NOATIME)
        ret |= MOXIEBOX_O_NOATIME;
#endif
#ifdef O_CLOEXEC
    if (flags & O_CLOEXEC)
        ret |= MOXIEBOX_O_CLOEXEC;
#endif
#ifdef O_SYNC
    if (flags & O_SYNC)
        ret |= MOXIEBOX_O_SYNC;
#endif
#ifdef O_PATH
    if (flags & O_PATH)
        ret |= MOXIEBOX_O_PATH;
#endif
#ifdef O_TMPFILE
    if (flags & O_TMPFILE)
        ret |= MOXIEBOX_O_TMPFILE;
#endif
    return ret;
}

/*
 * Translate moxiebox (Linux) open flags back into picolibc/newlib flags.
 */
int
moxiebox_open_flags_from_guest(int flags)
{
    int ret;

    switch (flags & MOXIEBOX_O_ACCMODE) {
    case MOXIEBOX_O_WRONLY:
        ret = O_WRONLY;
        break;
    case MOXIEBOX_O_RDWR:
        ret = O_RDWR;
        break;
    default:
        ret = O_RDONLY;
        break;
    }

#ifdef O_CREAT
    if (flags & MOXIEBOX_O_CREAT)
        ret |= O_CREAT;
#endif
#ifdef O_EXCL
    if (flags & MOXIEBOX_O_EXCL)
        ret |= O_EXCL;
#endif
#ifdef O_NOCTTY
    if (flags & MOXIEBOX_O_NOCTTY)
        ret |= O_NOCTTY;
#endif
#ifdef O_TRUNC
    if (flags & MOXIEBOX_O_TRUNC)
        ret |= O_TRUNC;
#endif
#ifdef O_APPEND
    if (flags & MOXIEBOX_O_APPEND)
        ret |= O_APPEND;
#endif
#ifdef O_NONBLOCK
    if (flags & MOXIEBOX_O_NONBLOCK)
        ret |= O_NONBLOCK;
#endif
#ifdef O_DSYNC
    if (flags & MOXIEBOX_O_DSYNC)
        ret |= O_DSYNC;
#endif
#ifdef O_DIRECT
    if (flags & MOXIEBOX_O_DIRECT)
        ret |= O_DIRECT;
#endif
#ifdef O_LARGEFILE
    if (flags & MOXIEBOX_O_LARGEFILE)
        ret |= O_LARGEFILE;
#endif
#ifdef O_DIRECTORY
    if (flags & MOXIEBOX_O_DIRECTORY)
        ret |= O_DIRECTORY;
#endif
#ifdef O_NOFOLLOW
    if (flags & MOXIEBOX_O_NOFOLLOW)
        ret |= O_NOFOLLOW;
#endif
#ifdef O_NOATIME
    if (flags & MOXIEBOX_O_NOATIME)
        ret |= O_NOATIME;
#endif
#ifdef O_CLOEXEC
    if (flags & MOXIEBOX_O_CLOEXEC)
        ret |= O_CLOEXEC;
#endif
#ifdef O_SYNC
    if (flags & MOXIEBOX_O_SYNC)
        ret |= O_SYNC;
#endif
#ifdef O_PATH
    if (flags & MOXIEBOX_O_PATH)
        ret |= O_PATH;
#endif
#ifdef O_TMPFILE
    if (flags & MOXIEBOX_O_TMPFILE)
        ret |= O_TMPFILE;
#endif
    return ret;
}

int
open(const char *path, int flags, ...)
{
    int    mode = 0;
    long   ret;
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

    ret = __moxiebox_open(path, moxiebox_open_flags(flags), mode);
    if (ret < 0) {
        errno = (int)-ret;
        return -1;
    }
    return (int)ret;
}
