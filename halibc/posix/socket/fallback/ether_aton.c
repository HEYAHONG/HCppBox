/***************************************************************
 * Name:      ether_aton.c
 * Purpose:   实现ether_aton接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "netinet/ether.h"
#include "halibc_posix_socket_fallback.h"

HALIBC_POSIX_SOCKET_FALLBACK_WEAK
struct ether_addr *ether_aton(const char *asc)
{
    static struct ether_addr addr;

    return ether_aton_r(asc, &addr);
}
