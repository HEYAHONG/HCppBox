/***************************************************************
 * Name:      getentropy.c
 * Purpose:   实现moxiebox getrandom系统调用接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-24
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#define _GNU_SOURCE
#include <errno.h>
#include <unistd.h>

#include "moxiebox_syscall.h"

int
getentropy(void *buffer, size_t length)
{
    long ret;

    if (length > 256) {
        errno = EIO;
        return -1;
    }

    ret = __moxiebox_getrandom(buffer, length, 0);
    if (ret < 0) {
        errno = (int)-ret;
        return -1;
    }
    return 0;
}
