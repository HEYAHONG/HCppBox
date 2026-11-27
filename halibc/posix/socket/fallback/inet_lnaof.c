/***************************************************************
 * Name:      inet_lnaof.c
 * Purpose:   实现inet_lnaof接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "arpa/inet.h"
#include "halibc_posix_socket_fallback.h"

HALIBC_POSIX_SOCKET_FALLBACK_WEAK
in_addr_t inet_lnaof(struct in_addr in)
{
    in_addr_t addr = ntohl(in.s_addr);

    if (IN_CLASSA(addr))
    {
	return addr & IN_CLASSA_HOST;
    }
    if (IN_CLASSB(addr))
    {
	return addr & IN_CLASSB_HOST;
    }

    return addr & IN_CLASSC_HOST;
}
