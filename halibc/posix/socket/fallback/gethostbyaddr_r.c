/***************************************************************
 * Name:      gethostbyaddr_r.c
 * Purpose:   实现gethostbyaddr_r接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "netdb.h"
#include "errno.h"
#include "halibc_posix_socket_fallback.h"

HALIBC_POSIX_SOCKET_FALLBACK_WEAK
int gethostbyaddr_r(const void *__addr, socklen_t __len, int __type, struct hostent *__result_buf, char *__buf, size_t __buflen, struct hostent **__result, int *__h_errnop)
{
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__addr);
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__len);
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__type);
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__result_buf);
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__buf);
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__buflen);
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__result);
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__h_errnop);
    if(__result != NULL)
    {
        *__result = NULL;
    }
    errno = ENOSYS;
    return -1;
}
