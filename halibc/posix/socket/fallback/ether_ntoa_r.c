/***************************************************************
 * Name:      ether_ntoa_r.c
 * Purpose:   实现ether_ntoa_r接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "netinet/ether.h"
#include "halibc_posix_socket_fallback.h"

HALIBC_POSIX_SOCKET_FALLBACK_WEAK
char *ether_ntoa_r(const struct ether_addr *addr, char *buf)
{
    static const char hexdig[] = "0123456789abcdef";
    char *start = buf;
    int i;

    for (i = 0; i < ETH_ALEN; i++)
    {
	if (i != 0)
	{
	    *buf++ = ':';
	}
	*buf++ = hexdig[(addr->ether_addr_octet[i] >> 4) & 0x0F];
	*buf++ = hexdig[addr->ether_addr_octet[i] & 0x0F];
    }
    *buf = '\0';

    return start;
}
