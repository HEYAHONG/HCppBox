/***************************************************************
 * Name:      getnetbyaddr.c
 * Purpose:   实现getnetbyaddr接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "netdb.h"
#include "errno.h"
#include "halibc_posix_socket_fallback.h"

HALIBC_POSIX_SOCKET_FALLBACK_WEAK
struct netent * getnetbyaddr(uint32_t __net, int __type)
{
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__net);
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__type);
    errno = ENOSYS;
    return NULL;
}
