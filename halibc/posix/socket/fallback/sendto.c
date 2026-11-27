/***************************************************************
 * Name:      sendto.c
 * Purpose:   实现sendto接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "sys/socket.h"
#include "errno.h"
#include "halibc_posix_socket_fallback.h"

HALIBC_POSIX_SOCKET_FALLBACK_WEAK
ssize_t sendto(int __fd, const void *__buf, size_t __n, int __flags, const struct sockaddr *__addr, socklen_t __addr_len)
{
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__fd);
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__buf);
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__n);
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__flags);
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__addr);
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__addr_len);
    errno = ENOSYS;
    return -1;
}
