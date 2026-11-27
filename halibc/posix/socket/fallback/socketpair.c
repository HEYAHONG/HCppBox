/***************************************************************
 * Name:      socketpair.c
 * Purpose:   实现socketpair接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "sys/socket.h"
#include "errno.h"
#include "halibc_posix_socket_fallback.h"

HALIBC_POSIX_SOCKET_FALLBACK_WEAK
int socketpair(int __domain, int __type, int __protocol, int __fds[2])
{
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__domain);
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__type);
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__protocol);
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__fds);
    errno = ENOSYS;
    return -1;
}
