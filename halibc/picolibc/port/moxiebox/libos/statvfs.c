/***************************************************************
 * Name:      statvfs.c
 * Purpose:   实现moxiebox statvfs接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-24
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#include <errno.h>
#include <sys/statvfs.h>

int
statvfs(const char *path, struct statvfs *buf)
{
    (void)path;
    (void)buf;
    errno = ENOSYS;
    return -1;
}
