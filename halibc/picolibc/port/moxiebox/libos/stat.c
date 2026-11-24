/***************************************************************
 * Name:      stat.c
 * Purpose:   实现moxiebox stat接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-24
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#include <errno.h>
#include <sys/stat.h>

int
stat(const char *path, struct stat *statbuf)
{
    (void)path;
    (void)statbuf;
    errno = ENOENT;
    return -1;
}
