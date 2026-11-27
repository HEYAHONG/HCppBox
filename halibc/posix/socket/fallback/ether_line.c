/***************************************************************
 * Name:      ether_line.c
 * Purpose:   实现ether_line接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "netinet/ether.h"
#include "errno.h"
#include "halibc_posix_socket_fallback.h"

HALIBC_POSIX_SOCKET_FALLBACK_WEAK
int ether_line(const char *line, struct ether_addr *addr, char *hostname)
{
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(line);
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(addr);
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(hostname);
    errno = ENOSYS;
    return -1;
}
