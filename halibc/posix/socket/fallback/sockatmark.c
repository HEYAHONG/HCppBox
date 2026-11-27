/***************************************************************
 * Name:      sockatmark.c
 * Purpose:   实现sockatmark接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "sys/socket.h"
#include "errno.h"
#include "halibc_posix_socket_fallback.h"

HALIBC_POSIX_SOCKET_FALLBACK_WEAK
int sockatmark(int __fd)
{
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__fd);
    errno = ENOSYS;
    return -1;
}
