/***************************************************************
 * Name:      sendmsg.c
 * Purpose:   实现sendmsg接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "sys/socket.h"
#include "errno.h"
#include "halibc_posix_socket_fallback.h"

HALIBC_POSIX_SOCKET_FALLBACK_WEAK
ssize_t sendmsg(int __fd, const struct msghdr *__message, int __flags)
{
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__fd);
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__message);
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__flags);
    errno = ENOSYS;
    return -1;
}
