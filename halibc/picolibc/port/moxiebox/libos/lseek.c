/***************************************************************
 * Name:      lseek.c
 * Purpose:   实现moxiebox lseek系统调用接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-24
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#include <errno.h>
#include <unistd.h>

#include "moxiebox_syscall.h"

off_t
lseek(int fd, off_t offset, int whence)
{
    long ret = __moxiebox_lseek(fd, (long)offset, whence);

    if (ret < 0) {
        errno = (int)-ret;
        return (off_t)-1;
    }
    return (off_t)ret;
}
