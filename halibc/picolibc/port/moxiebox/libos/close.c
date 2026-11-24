/***************************************************************
 * Name:      close.c
 * Purpose:   实现moxiebox close系统调用接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-24
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#include <errno.h>
#include <unistd.h>

#include "moxiebox_syscall.h"

int
close(int fd)
{
    long ret = __moxiebox_close(fd);

    if (ret < 0) {
        errno = (int)-ret;
        return -1;
    }
    return 0;
}
