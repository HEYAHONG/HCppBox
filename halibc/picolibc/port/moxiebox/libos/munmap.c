/***************************************************************
 * Name:      munmap.c
 * Purpose:   实现moxiebox munmap系统调用接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-24
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#include <errno.h>
#include <sys/mman.h>

#include "moxiebox_syscall.h"

int
munmap(void *addr, size_t len)
{
    long ret = __moxiebox_munmap(addr, len);

    if (ret < 0) {
        errno = (int)-ret;
        return -1;
    }
    return 0;
}
