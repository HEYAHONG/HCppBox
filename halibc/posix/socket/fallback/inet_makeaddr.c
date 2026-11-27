/***************************************************************
 * Name:      inet_makeaddr.c
 * Purpose:   实现inet_makeaddr接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "arpa/inet.h"
#include "halibc_posix_socket_fallback.h"

HALIBC_POSIX_SOCKET_FALLBACK_WEAK
struct in_addr inet_makeaddr(in_addr_t net, in_addr_t host)
{
    in_addr_t addr;
    struct in_addr ret;

    if (net < 128)
    {
	addr = (net << IN_CLASSA_NSHIFT) | (host & IN_CLASSA_HOST);
    }
    else if (net < 65536)
    {
	addr = (net << IN_CLASSB_NSHIFT) | (host & IN_CLASSB_HOST);
    }
    else if (net < 16777216UL)
    {
	addr = (net << IN_CLASSC_NSHIFT) | (host & IN_CLASSC_HOST);
    }
    else
    {
	addr = net;
    }

    ret.s_addr = htonl(addr);
    return ret;
}
