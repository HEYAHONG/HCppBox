/***************************************************************
 * Name:      bind.c
 * Purpose:   实现bind接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "sys/socket.h"
#include "errno.h"
#include "halibc_posix_socket_fallback.h"

HALIBC_POSIX_SOCKET_FALLBACK_WEAK
int bind(int __fd, const struct sockaddr *__addr, socklen_t __len)
{
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__fd);
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__addr);
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__len);
    errno = ENOSYS;
    return -1;
}
