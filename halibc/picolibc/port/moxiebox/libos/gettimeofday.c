/***************************************************************
 * Name:      gettimeofday.c
 * Purpose:   实现moxiebox gettimeofday系统调用接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-24
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#define _DEFAULT_SOURCE
#include <errno.h>
#include <sys/time.h>

#include "moxiebox_syscall.h"

int
gettimeofday(struct timeval *tv, void *tz)
{
    long ret;

    if (tv == NULL) {
        errno = EINVAL;
        return -1;
    }

    ret = __moxiebox_gettimeofday(tv, tz);
    if (ret < 0) {
        errno = (int)-ret;
        return -1;
    }
    return 0;
}
