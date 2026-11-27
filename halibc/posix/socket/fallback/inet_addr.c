/***************************************************************
 * Name:      inet_addr.c
 * Purpose:   实现inet_addr接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "arpa/inet.h"
#include "halibc_posix_socket_fallback.h"

HALIBC_POSIX_SOCKET_FALLBACK_WEAK
in_addr_t inet_addr(const char *cp)
{
    struct in_addr addr;

    if (inet_aton(cp, &addr))
    {
	return addr.s_addr;
    }

    return (in_addr_t)INADDR_NONE;
}
