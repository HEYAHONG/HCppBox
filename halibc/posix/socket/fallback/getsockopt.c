/***************************************************************
 * Name:      getsockopt.c
 * Purpose:   实现getsockopt接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "sys/socket.h"
#include "errno.h"
#include "halibc_posix_socket_fallback.h"

HALIBC_POSIX_SOCKET_FALLBACK_WEAK
int getsockopt(int __fd, int __level, int __optname, void *__optval, socklen_t *__optlen)
{
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__fd);
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__level);
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__optname);
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__optval);
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__optlen);
    errno = ENOSYS;
    return -1;
}
