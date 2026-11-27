/***************************************************************
 * Name:      ether_ntoa.c
 * Purpose:   实现ether_ntoa接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "netinet/ether.h"
#include "halibc_posix_socket_fallback.h"

HALIBC_POSIX_SOCKET_FALLBACK_WEAK
char *ether_ntoa(const struct ether_addr *addr)
{
    static char buf[18];

    return ether_ntoa_r(addr, buf);
}
