/***************************************************************
 * Name:      getifaddrs.c
 * Purpose:   实现getifaddrs接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "ifaddrs.h"
#include "errno.h"
#include "halibc_posix_socket_fallback.h"

HALIBC_POSIX_SOCKET_FALLBACK_WEAK
int getifaddrs(struct ifaddrs **__ifap)
{
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__ifap);
    if(__ifap != NULL)
    {
        *__ifap = NULL;
    }
    errno = ENOSYS;
    return -1;
}
