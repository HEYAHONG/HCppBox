/***************************************************************
 * Name:      sbrk.c
 * Purpose:   实现基于mmap的sbrk接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-24
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#define _DEFAULT_SOURCE
#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>

#include "moxiebox_syscall.h"

#define MOXIEBOX_HEAP_CHUNK (64U * 1024U)

static char  *moxiebox_brk;
static size_t moxiebox_brk_left;

void *
sbrk(ptrdiff_t incr)
{
    void  *ret;
    size_t need;

    if (incr == 0)
        return moxiebox_brk;

    if (incr < 0) {
        errno = EINVAL;
        return (void *)-1;
    }

    need = (size_t)incr;
    if (need > moxiebox_brk_left) {
        size_t length = (need + 4095U) & ~(size_t)4095U;
        long   addr;

        if (length < MOXIEBOX_HEAP_CHUNK)
            length = MOXIEBOX_HEAP_CHUNK;

        addr = __moxiebox_mmap(NULL, length,
                               MOXIEBOX_PROT_READ | MOXIEBOX_PROT_WRITE,
                               MOXIEBOX_MAP_PRIVATE | MOXIEBOX_MAP_ANONYMOUS,
                               -1, 0);
        if (addr < 0) {
            errno = ENOMEM;
            return (void *)-1;
        }
        moxiebox_brk = (char *)(uintptr_t)addr;
        moxiebox_brk_left = length;
    }

    ret = moxiebox_brk;
    moxiebox_brk += need;
    moxiebox_brk_left -= need;
    return ret;
}
