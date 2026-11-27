/***************************************************************
 * Name:      inet_netof.c
 * Purpose:   实现inet_netof接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "arpa/inet.h"
#include "halibc_posix_socket_fallback.h"

HALIBC_POSIX_SOCKET_FALLBACK_WEAK
in_addr_t inet_netof(struct in_addr in)
{
    in_addr_t addr = ntohl(in.s_addr);

    if (IN_CLASSA(addr))
    {
	return (addr & IN_CLASSA_NET) >> IN_CLASSA_NSHIFT;
    }
    if (IN_CLASSB(addr))
    {
	return (addr & IN_CLASSB_NET) >> IN_CLASSB_NSHIFT;
    }

    return (addr & IN_CLASSC_NET) >> IN_CLASSC_NSHIFT;
}
