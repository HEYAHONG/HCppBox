/***************************************************************
 * Name:      write.c
 * Purpose:   实现moxiebox write系统调用接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-24
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#include <errno.h>
#include <unistd.h>

#include "moxiebox_syscall.h"

ssize_t
write(int fd, const void *buf, size_t count)
{
    long ret = __moxiebox_write(fd, buf, count);

    if (ret < 0) {
        errno = (int)-ret;
        return -1;
    }
    return (ssize_t)ret;
}
