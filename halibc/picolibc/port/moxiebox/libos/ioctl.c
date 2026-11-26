/***************************************************************
 * Name:      ioctl.c
 * Purpose:   实现moxiebox ioctl系统调用接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-26
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#include <errno.h>
#include <sys/ioctl.h>

#include "moxiebox_syscall.h"

int
ioctl(int fd, unsigned long op, void *param)
{
    long ret = __moxiebox_ioctl(fd, op, param);

    if (ret < 0) {
        errno = (int)-ret;
        return -1;
    }
    return (int)ret;
}
