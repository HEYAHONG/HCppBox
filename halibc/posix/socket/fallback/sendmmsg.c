/***************************************************************
 * Name:      sendmmsg.c
 * Purpose:   实现sendmmsg接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "sys/socket.h"
#include "errno.h"
#include "halibc_posix_socket_fallback.h"

HALIBC_POSIX_SOCKET_FALLBACK_WEAK
int sendmmsg(int __fd, struct mmsghdr *__vmessages, unsigned int __vlen, int __flags)
{
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__fd);
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__vmessages);
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__vlen);
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__flags);
    errno = ENOSYS;
    return -1;
}
