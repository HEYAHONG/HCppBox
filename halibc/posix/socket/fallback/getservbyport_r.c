/***************************************************************
 * Name:      getservbyport_r.c
 * Purpose:   实现getservbyport_r接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "netdb.h"
#include "errno.h"
#include "halibc_posix_socket_fallback.h"

HALIBC_POSIX_SOCKET_FALLBACK_WEAK
int getservbyport_r(int __port, const char *__proto, struct servent *__result_buf, char *__buf, size_t __buflen, struct servent **__result)
{
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__port);
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__proto);
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__result_buf);
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__buf);
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__buflen);
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__result);
    if(__result != NULL)
    {
        *__result = NULL;
    }
    errno = ENOSYS;
    return -1;
}
