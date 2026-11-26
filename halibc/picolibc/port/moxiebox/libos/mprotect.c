/***************************************************************
 * Name:      mprotect.c
 * Purpose:   实现moxiebox mprotect系统调用接口及标志转换
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-26
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#include <errno.h>
#include <sys/mman.h>

#include "moxiebox_syscall.h"
#include "moxiebox_libos.h"

int
mprotect(void *addr, size_t len, int prot)
{
    long ret = __moxiebox_mprotect(addr, len, moxiebox_mmap_prot(prot));

    if (ret < 0) {
        errno = (int)-ret;
        return -1;
    }
    return 0;
}
