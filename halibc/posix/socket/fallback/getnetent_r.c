/***************************************************************
 * Name:      getnetent_r.c
 * Purpose:   实现getnetent_r接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "netdb.h"
#include "errno.h"
#include "halibc_posix_socket_fallback.h"

HALIBC_POSIX_SOCKET_FALLBACK_WEAK
int getnetent_r(struct netent *__result_buf, char *__buf, size_t __buflen, struct netent **__result, int *__h_errnop)
{
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
