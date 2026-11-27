/***************************************************************
 * Name:      recv.c
 * Purpose:   实现recv接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "sys/socket.h"
#include "errno.h"
#include "halibc_posix_socket_fallback.h"

HALIBC_POSIX_SOCKET_FALLBACK_WEAK
ssize_t recv(int __fd, void *__buf, size_t __n, int __flags)
{
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__fd);
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__buf);
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__n);
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__flags);
    errno = ENOSYS;
    return -1;
}
