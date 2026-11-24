/***************************************************************
 * Name:      read.c
 * Purpose:   实现moxiebox read系统调用接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-24
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#include <errno.h>
#include <unistd.h>

#include "moxiebox_syscall.h"

ssize_t
read(int fd, void *buf, size_t count)
{
    long ret = __moxiebox_read(fd, buf, count);

    if (ret < 0) {
        errno = (int)-ret;
        return -1;
    }
    return (ssize_t)ret;
}
