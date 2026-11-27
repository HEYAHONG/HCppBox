/***************************************************************
 * Name:      inet_ntop.c
 * Purpose:   实现inet_ntop接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "arpa/inet.h"
#include "errno.h"
#include "stddef.h"
#include "halibc_posix_socket_fallback.h"
#include "halibc_posix_socket_inet_private.h"

HALIBC_POSIX_SOCKET_FALLBACK_WEAK
const char *inet_ntop(int af, const void *cp, char *buf, socklen_t len)
{
    if (cp == NULL || buf == NULL)
    {
	errno = EINVAL;
	return NULL;
    }

    switch (af)
    {
    case AF_INET:
	if (__halibc_posix_socket_inet_ntop4((const unsigned char *)cp,
					     buf, len))
	{
	    return buf;
	}
	return NULL;
    case AF_INET6:
	if (__halibc_posix_socket_inet_ntop6((const unsigned char *)cp,
					     buf, len))
	{
	    return buf;
	}
	return NULL;
    default:
	errno = EAFNOSUPPORT;
	return NULL;
    }
}
