/***************************************************************
 * Name:      inet_ntoa.c
 * Purpose:   实现inet_ntoa接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "arpa/inet.h"
#include "stddef.h"
#include "halibc_posix_socket_fallback.h"

HALIBC_POSIX_SOCKET_FALLBACK_WEAK
char *inet_ntoa(struct in_addr in)
{
    static char buf[INET_ADDRSTRLEN];

    if (inet_ntop(AF_INET, &in, buf, (socklen_t)sizeof(buf)) == NULL)
    {
	return NULL;
    }

    return buf;
}
