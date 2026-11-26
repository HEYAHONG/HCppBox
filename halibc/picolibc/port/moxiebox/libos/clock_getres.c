/***************************************************************
 * Name:      clock_getres.c
 * Purpose:   实现moxiebox clock_getres系统调用接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-26
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#include <errno.h>
#include <time.h>

#include "moxiebox_syscall.h"
#include "moxiebox_libos.h"

int
clock_getres(clockid_t clock_id, struct timespec *res)
{
    long ret = __moxiebox_clock_getres((long)moxiebox_clockid(clock_id), res);

    if (ret < 0) {
        errno = (int)-ret;
        return -1;
    }
    return 0;
}
