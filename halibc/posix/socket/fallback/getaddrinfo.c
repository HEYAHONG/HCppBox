/***************************************************************
 * Name:      getaddrinfo.c
 * Purpose:   实现getaddrinfo接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "netdb.h"
#include "errno.h"
#include "halibc_posix_socket_fallback.h"

HALIBC_POSIX_SOCKET_FALLBACK_WEAK
int getaddrinfo(const char *__name, const char *__service, const struct addrinfo *__req, struct addrinfo **__pai)
{
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__name);
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__service);
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__req);
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__pai);
    if(__pai != NULL)
    {
        *__pai = NULL;
    }
    errno = ENOSYS;
    return EAI_FAIL;
}
