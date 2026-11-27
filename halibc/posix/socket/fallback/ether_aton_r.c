/***************************************************************
 * Name:      ether_aton_r.c
 * Purpose:   实现ether_aton_r接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "netinet/ether.h"
#include "stddef.h"
#include "halibc_posix_socket_fallback.h"

static int __halibc_posix_socket_ether_hexval(char c)
{
    if (c >= '0' && c <= '9')
    {
	return c - '0';
    }
    if (c >= 'a' && c <= 'f')
    {
	return c - 'a' + 10;
    }
    if (c >= 'A' && c <= 'F')
    {
	return c - 'A' + 10;
    }
    return -1;
}

HALIBC_POSIX_SOCKET_FALLBACK_WEAK
struct ether_addr *ether_aton_r(const char *asc, struct ether_addr *addr)
{
    int i;

    if (asc == NULL || addr == NULL)
    {
	return NULL;
    }

    for (i = 0; i < ETH_ALEN; i++)
    {
	int hi;
	int lo;

	if (i != 0)
	{
	    if (*asc != ':')
	    {
		return NULL;
	    }
	    asc++;
	}

	hi = __halibc_posix_socket_ether_hexval(*asc++);
	lo = __halibc_posix_socket_ether_hexval(*asc++);
	if (hi < 0 || lo < 0)
	{
	    return NULL;
	}
	addr->ether_addr_octet[i] = (unsigned char)((hi << 4) | lo);
    }

    if (*asc != '\0')
    {
	return NULL;
    }

    return addr;
}
