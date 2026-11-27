/***************************************************************
 * Name:      ether_ntohost.c
 * Purpose:   实现ether_ntohost接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "netinet/ether.h"
#include "errno.h"
#include "halibc_posix_socket_fallback.h"

HALIBC_POSIX_SOCKET_FALLBACK_WEAK
int ether_ntohost(char *hostname, const struct ether_addr *addr)
{
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(hostname);
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(addr);
    errno = ENOSYS;
    return -1;
}
