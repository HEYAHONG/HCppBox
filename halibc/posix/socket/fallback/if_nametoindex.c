/***************************************************************
 * Name:      if_nametoindex.c
 * Purpose:   实现if_nametoindex接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "net/if.h"
#include "errno.h"
#include "halibc_posix_socket_fallback.h"

HALIBC_POSIX_SOCKET_FALLBACK_WEAK
unsigned int if_nametoindex(const char *ifname)
{
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(ifname);
    errno = ENOSYS;
    return 0;
}
