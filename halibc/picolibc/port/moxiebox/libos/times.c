/***************************************************************
 * Name:      times.c
 * Purpose:   实现moxiebox times接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-24
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#define _DEFAULT_SOURCE
#include <errno.h>
#include <sys/times.h>

clock_t
times(struct tms *buf)
{
    (void)buf;
    errno = ENOSYS;
    return (clock_t)-1;
}
