/***************************************************************
 * Name:      fstatvfs.c
 * Purpose:   实现moxiebox fstatvfs接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-24
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#include <errno.h>
#include <sys/statvfs.h>

int
fstatvfs(int fd, struct statvfs *buf)
{
    (void)fd;
    (void)buf;
    errno = ENOSYS;
    return -1;
}
