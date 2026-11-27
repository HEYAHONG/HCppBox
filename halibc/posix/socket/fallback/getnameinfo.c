/***************************************************************
 * Name:      getnameinfo.c
 * Purpose:   实现getnameinfo接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "netdb.h"
#include "errno.h"
#include "halibc_posix_socket_fallback.h"

HALIBC_POSIX_SOCKET_FALLBACK_WEAK
int getnameinfo(const struct sockaddr *__sa, socklen_t __salen, char *__host, socklen_t __hostlen, char *__serv, socklen_t __servlen, int __flags)
{
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__sa);
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__salen);
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__host);
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__hostlen);
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__serv);
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__servlen);
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__flags);
    errno = ENOSYS;
    return EAI_FAIL;
}
