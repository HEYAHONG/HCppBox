/***************************************************************
 * Name:      unlink.c
 * Purpose:   实现moxiebox unlink接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-24
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#include <errno.h>
#include <unistd.h>

int
unlink(const char *path)
{
    (void)path;
    errno = ENOENT;
    return -1;
}
