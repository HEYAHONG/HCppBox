/***************************************************************
 * Name:      gethostbyaddr.c
 * Purpose:   实现gethostbyaddr接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "netdb.h"
#include "errno.h"
#include "halibc_posix_socket_fallback.h"

HALIBC_POSIX_SOCKET_FALLBACK_WEAK
struct hostent * gethostbyaddr(const void *__addr, socklen_t __len, int __type)
{
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__addr);
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__len);
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__type);
    errno = ENOSYS;
    return NULL;
}
