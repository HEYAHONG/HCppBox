/***************************************************************
 * Name:      halibc_posix_socket_inet_private.h
 * Purpose:   声明inet地址转换内部辅助函数
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HALIBC_POSIX_SOCKET_INET_PRIVATE_H__
#define __HALIBC_POSIX_SOCKET_INET_PRIVATE_H__

#include "stdint.h"
#include "string.h"
#include "errno.h"
#include "netinet/in.h"

/*
 * 严格解析IPv4点分十进制地址。
 * 成功返回1，失败返回0。
 */
static __inline__ int __halibc_posix_socket_inet_pton4(const char *src,
						       unsigned char *dst)
{
    unsigned char tmp[4];
    int octets = 0;
    const char *p = src;

    while (octets < 4)
    {
	int val = 0;
	int digits = 0;

	if (*p == '0')
	{
	    p++;
	    digits = 1;
	    if (*p >= '0' && *p <= '9')
	    {
		return 0;
	    }
	}
	else
	{
	    while (*p >= '0' && *p <= '9')
	    {
		val = val * 10 + (*p - '0');
		if (val > 255)
		{
		    return 0;
		}
		p++;
		digits++;
		if (digits > 3)
		{
		    return 0;
		}
	    }
	    if (digits == 0)
	    {
		return 0;
	    }
	}

	tmp[octets++] = (unsigned char)val;
	if (octets == 4)
	{
	    break;
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

    if (*p != '\0')
    {
	return 0;
    }

    memcpy(dst, tmp, 4);
    return 1;
}

/*
 * 解析IPv6地址，支持"::"压缩与内嵌IPv4地址。
 * 成功返回1，失败返回0。
 */
static __inline__ int __halibc_posix_socket_inet_pton6(const char *src,
						       unsigned char *dst)
{
    unsigned char tmp[16];
    unsigned char *tp = tmp;
    unsigned char *end = tmp + 16;
    unsigned char *colonp = NULL;
    const char *p = src;

    if (*p == ':')
    {
	if (p[1] != ':')
	{
	    return 0;
	}
	p += 2;
	colonp = tp;
    }
    else if (*p == '\0')
    {
	return 0;
    }

    while (*p)
    {
	const char *group_start = p;
	unsigned int val = 0;
	int digits = 0;

	while (digits < 4)
	{
	    int c = (unsigned char)*p;
	    int d;

	    if (c >= '0' && c <= '9')
	    {
		d = c - '0';
	    }
	    else if (c >= 'a' && c <= 'f')
	    {
		d = c - 'a' + 10;
	    }
	    else if (c >= 'A' && c <= 'F')
	    {
		d = c - 'A' + 10;
	    }
	    else
	    {
		break;
	    }

	    val = (val << 4) | (unsigned int)d;
	    p++;
	    digits++;
	}

	if (*p == '.')
	{
	    if (digits == 0 || tp + 4 > end)
	    {
		return 0;
	    }
	    if (!__halibc_posix_socket_inet_pton4(group_start, tp))
	    {
		return 0;
	    }
	    tp += 4;
	    p += strlen(p);
	    break;
	}

	if (digits == 0 || tp + 2 > end)
	{
	    return 0;
	}

	*tp++ = (unsigned char)((val >> 8) & 0xFF);
	*tp++ = (unsigned char)(val & 0xFF);

	if (*p == ':')
	{
	    if (p[1] == ':')
	    {
		if (colonp != NULL)
		{
		    return 0;
		}
		colonp = tp;
		p += 2;
		if (*p == '\0')
		{
		    break;
		}
	    }
	    else
	    {
		p++;
		if (*p == '\0')
		{
		    return 0;
		}
	    }
	}
	else if (*p == '\0')
	{
	    break;
	}
	else
	{
	    return 0;
	}
    }

    if (colonp != NULL)
    {
	size_t n = (size_t)(tp - colonp);

	memmove(end - n, colonp, n);
	memset(colonp, 0, (size_t)(end - colonp - n));
	tp = end;
    }

    if (tp != end)
    {
	return 0;
    }

    memcpy(dst, tmp, 16);
    return 1;
}

/*
 * 将IPv4地址格式化为点分十进制。
 * 成功返回1，缓冲区不足返回0并设置errno为ENOSPC。
 */
static __inline__ int __halibc_posix_socket_inet_ntop4(const unsigned char *src,
						       char *dst, socklen_t size)
{
    char tmp[INET_ADDRSTRLEN];
    int n = 0;
    int i;

    for (i = 0; i < 4; i++)
    {
	unsigned int v = src[i];

	if (v >= 100)
	{
	    tmp[n++] = (char)('0' + (v / 100));
	    v %= 100;
	    tmp[n++] = (char)('0' + (v / 10));
	    tmp[n++] = (char)('0' + (v % 10));
	}
	else if (v >= 10)
	{
	    tmp[n++] = (char)('0' + (v / 10));
	    tmp[n++] = (char)('0' + (v % 10));
	}
	else
	{
	    tmp[n++] = (char)('0' + v);
	}

	if (i != 3)
	{
	    tmp[n++] = '.';
	}
    }

    tmp[n] = '\0';
    if ((socklen_t)(n + 1) > size)
    {
	errno = ENOSPC;
	return 0;
    }

    memcpy(dst, tmp, (size_t)(n + 1));
    return 1;
}

static __inline__ int __halibc_posix_socket_inet_ntop6_hex(char *dst,
							   unsigned int val)
{
    static const char hexdig[] = "0123456789abcdef";
    int n = 0;

    if (val >= 0x1000)
    {
	dst[n++] = hexdig[(val >> 12) & 0xF];
    }
    if (val >= 0x100)
    {
	dst[n++] = hexdig[(val >> 8) & 0xF];
    }
    if (val >= 0x10)
    {
	dst[n++] = hexdig[(val >> 4) & 0xF];
    }
    dst[n++] = hexdig[val & 0xF];

    return n;
}

/*
 * 将IPv6地址格式化。
 * 成功返回1，缓冲区不足返回0并设置errno为ENOSPC。
 */
static __inline__ int __halibc_posix_socket_inet_ntop6(const unsigned char *src,
						       char *dst, socklen_t size)
{
    char tmp[INET6_ADDRSTRLEN];
    unsigned int words[8];
    int best_base = -1;
    int best_len = 0;
    int cur_base = -1;
    int cur_len = 0;
    int n = 0;
    int i;

    for (i = 0; i < 8; i++)
    {
	words[i] = ((unsigned int)src[2 * i] << 8) | src[2 * i + 1];
    }

    /* 查找最长的连续零分组，用于"::"压缩 */
    for (i = 0; i < 8; i++)
    {
	if (words[i] == 0)
	{
	    if (cur_base == -1)
	    {
		cur_base = i;
		cur_len = 1;
	    }
	    else
	    {
		cur_len++;
	    }
	}
	else if (cur_base != -1)
	{
	    if (best_base == -1 || cur_len > best_len)
	    {
		best_base = cur_base;
		best_len = cur_len;
	    }
	    cur_base = -1;
	}
    }
    if (cur_base != -1)
    {
	if (best_base == -1 || cur_len > best_len)
	{
	    best_base = cur_base;
	    best_len = cur_len;
	}
    }
    if (best_base != -1 && best_len < 2)
    {
	best_base = -1;
	best_len = 0;
    }

    for (i = 0; i < 8; i++)
    {
	if (best_base != -1 && i >= best_base && i < best_base + best_len)
	{
	    if (i == best_base)
	    {
		tmp[n++] = ':';
	    }
	    continue;
	}

	if (i != 0)
	{
	    tmp[n++] = ':';
	}

	/* 内嵌IPv4地址：v4映射地址与v4兼容地址 */
	if (i == 6 && best_base == 0
	    && (best_len == 6 || (best_len == 5 && words[5] == 0xFFFF)))
	{
	    if (!__halibc_posix_socket_inet_ntop4(src + 12, tmp + n,
						 (socklen_t)(sizeof(tmp) - (size_t)n)))
	    {
		return 0;
	    }
	    n += (int)strlen(tmp + n);
	    break;
	}

	n += __halibc_posix_socket_inet_ntop6_hex(tmp + n, words[i]);
    }

    if (best_base != -1 && best_base + best_len == 8)
    {
	tmp[n++] = ':';
    }

    tmp[n] = '\0';
    if ((socklen_t)(n + 1) > size)
    {
	errno = ENOSPC;
	return 0;
    }

    memcpy(dst, tmp, (size_t)(n + 1));
    return 1;
}

#endif /* __HALIBC_POSIX_SOCKET_INET_PRIVATE_H__ */
