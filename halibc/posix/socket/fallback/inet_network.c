/***************************************************************
 * Name:      inet_network.c
 * Purpose:   实现inet_network接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "arpa/inet.h"
#include "ctype.h"
#include "stddef.h"
#include "halibc_posix_socket_fallback.h"

HALIBC_POSIX_SOCKET_FALLBACK_WEAK
in_addr_t inet_network(const char *cp)
{
    uint32_t parts[4];
    uint32_t *pp = parts;
    const char *p = cp;

    if (p == NULL)
    {
	return (in_addr_t)INADDR_NONE;
    }

again:
    {
	uint32_t val = 0;
	uint32_t base = 10;
	int digit = 0;
	char c;

	if (*p == '0')
	{
	    digit = 1;
	    base = 8;
	    p++;
	}
	if (*p == 'x' || *p == 'X')
	{
	    digit = 0;
	    base = 16;
	    p++;
	}

	while ((c = *p) != '\0')
	{
	    if (isdigit((unsigned char)c))
	    {
		if (base == 8 && (c == '8' || c == '9'))
		{
		    return (in_addr_t)INADDR_NONE;
		}
		val = (val * base) + (uint32_t)(c - '0');
		if (val > 0xFF)
		{
		    return (in_addr_t)INADDR_NONE;
		}
		p++;
		digit = 1;
		continue;
	    }
	    if (base == 16 && isxdigit((unsigned char)c))
	    {
		val = (val << 4) + (uint32_t)(tolower((unsigned char)c) + 10 - 'a');
		if (val > 0xFF)
		{
		    return (in_addr_t)INADDR_NONE;
		}
		p++;
		digit = 1;
		continue;
	    }
	    break;
	}

	if (!digit)
	{
	    return (in_addr_t)INADDR_NONE;
	}
	if (pp >= parts + 4 || val > 0xFF)
	{
	    return (in_addr_t)INADDR_NONE;
	}

	if (*p == '.')
	{
	    *pp++ = val;
	    p++;
	    goto again;
	}

	while (isspace((unsigned char)*p))
	{
	    p++;
	}
	if (*p != '\0')
	{
	    return (in_addr_t)INADDR_NONE;
	}
	if (pp >= parts + 4 || val > 0xFF)
	{
	    return (in_addr_t)INADDR_NONE;
	}
	*pp++ = val;
    }

    {
	uint32_t val = 0;
	uint32_t n = (uint32_t)(pp - parts);
	uint32_t i;

	for (i = 0; i < n; i++)
	{
	    val <<= 8;
	    val |= parts[i] & 0xFF;
	}
	return (in_addr_t)val;
    }
}
