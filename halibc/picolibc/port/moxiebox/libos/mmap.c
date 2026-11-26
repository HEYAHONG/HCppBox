/***************************************************************
 * Name:      mmap.c
 * Purpose:   实现moxiebox mmap系统调用接口及标志转换
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-24
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#include <errno.h>
#include <stdint.h>
#include <sys/mman.h>

#include "moxiebox_syscall.h"
#include "moxiebox_libos.h"

int
moxiebox_mmap_prot(int prot)
{
    int ret = 0;

    if (prot & PROT_EXEC)
        ret |= MOXIEBOX_PROT_EXEC;
    if (prot & PROT_READ)
        ret |= MOXIEBOX_PROT_READ;
    if (prot & PROT_WRITE)
        ret |= MOXIEBOX_PROT_WRITE;
    return ret;
}

static int
moxiebox_mmap_flags(int flags)
{
    int ret = 0;

    if (flags & MAP_SHARED)
        ret |= MOXIEBOX_MAP_SHARED;
    if (flags & MAP_PRIVATE)
        ret |= MOXIEBOX_MAP_PRIVATE;
    if (flags & MAP_ANONYMOUS)
        ret |= MOXIEBOX_MAP_ANONYMOUS;
    return ret;
}

void *
mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset)
{
    long ret;

    ret = __moxiebox_mmap(addr, len, moxiebox_mmap_prot(prot),
                           moxiebox_mmap_flags(flags), fd, (long)offset);
    if (ret < 0) {
        errno = (int)-ret;
        return MAP_FAILED;
    }
    return (void *)(uintptr_t)ret;
}
