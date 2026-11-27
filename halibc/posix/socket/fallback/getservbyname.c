/***************************************************************
 * Name:      getservbyname.c
 * Purpose:   实现getservbyname接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "netdb.h"
#include "errno.h"
#include "halibc_posix_socket_fallback.h"

HALIBC_POSIX_SOCKET_FALLBACK_WEAK
struct servent * getservbyname(const char *__name, const char *__proto)
{
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__name);
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__proto);
    errno = ENOSYS;
    return NULL;
}
