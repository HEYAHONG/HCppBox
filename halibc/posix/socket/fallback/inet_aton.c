/***************************************************************
 * Name:      inet_aton.c
 * Purpose:   实现inet_aton接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "arpa/inet.h"
#include "ctype.h"
#include "errno.h"
#include "stdlib.h"
#include "halibc_posix_socket_fallback.h"

HALIBC_POSIX_SOCKET_FALLBACK_WEAK
int inet_aton(const char *cp, struct in_addr *inp)
{
    uint32_t parts[4];
    int nparts = 0;
    uint32_t addr = 0;
    const char *p = cp;

    if (cp == NULL)
    {
	return 0;
    }

    for (;;)
    {
	char *end = NULL;
	unsigned long val;

	if (nparts >= 4)
	{
	    return 0;
	}
	if (!(*p >= '0' && *p <= '9'))
	{
	    return 0;
	}

	errno = 0;
	val = strtoul(p, &end, 0);
	if (end == p || errno == ERANGE || val > 0xFFFFFFFFUL)
	{
	    return 0;
	}

	parts[nparts++] = (uint32_t)val;
	p = end;

	if (*p == '\0')
	{
	    break;
	}
	if (isspace((unsigned char)*p))
	{
	    while (isspace((unsigned char)*p))
	    {
		p++;
	    }
	    if (*p == '\0')
	    {
		break;
	    }
	    return 0;
	}
	if (*p != '.')
	{
	    return 0;
	}
	p++;
	if (*p == '\0')
	{
	    return 0;
	}
    }

    switch (nparts)
    {
    case 1:
	addr = parts[0];
	break;
    case 2:
	if (parts[0] > 0xFF || parts[1] > 0xFFFFFF)
	{
	    return 0;
	}
	addr = (parts[0] << 24) | parts[1];
	break;
    case 3:
	if (parts[0] > 0xFF || parts[1] > 0xFF || parts[2] > 0xFFFF)
	{
	    return 0;
	}
	addr = (parts[0] << 24) | (parts[1] << 16) | parts[2];
	break;
    case 4:
	if (parts[0] > 0xFF || parts[1] > 0xFF
	    || parts[2] > 0xFF || parts[3] > 0xFF)
	{
	    return 0;
	}
	addr = (parts[0] << 24) | (parts[1] << 16)
	       | (parts[2] << 8) | parts[3];
	break;
    default:
	return 0;
    }

    if (inp != NULL)
    {
	inp->s_addr = htonl(addr);
    }
    return 1;
}
