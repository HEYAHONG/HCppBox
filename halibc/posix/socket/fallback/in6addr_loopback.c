/***************************************************************
 * Name:      in6addr_loopback.c
 * Purpose:   实现IPv6环回地址常量
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "netinet/in.h"
#include "halibc_posix_socket_fallback.h"

HALIBC_POSIX_SOCKET_FALLBACK_WEAK
const struct in6_addr in6addr_loopback = IN6ADDR_LOOPBACK_INIT;
