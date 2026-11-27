/***************************************************************
 * Name:      inet_pton.c
 * Purpose:   实现inet_pton接口
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
int inet_pton(int af, const char *cp, void *buf)
{
    if (cp == NULL || buf == NULL)
    {
	return 0;
    }

    switch (af)
    {
    case AF_INET:
	return __halibc_posix_socket_inet_pton4(cp, (unsigned char *)buf);
    case AF_INET6:
	return __halibc_posix_socket_inet_pton6(cp, (unsigned char *)buf);
    default:
	errno = EAFNOSUPPORT;
	return -1;
    }
}
