/***************************************************************
 * Name:      socket.c
 * Purpose:   实现socket接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "sys/socket.h"
#include "errno.h"
#include "halibc_posix_socket_fallback.h"

HALIBC_POSIX_SOCKET_FALLBACK_WEAK
int socket(int __domain, int __type, int __protocol)
{
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__domain);
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__type);
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__protocol);
    errno = ENOSYS;
    return -1;
}
