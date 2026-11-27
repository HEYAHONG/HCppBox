/***************************************************************
 * Name:      shutdown.c
 * Purpose:   实现shutdown接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "sys/socket.h"
#include "errno.h"
#include "halibc_posix_socket_fallback.h"

HALIBC_POSIX_SOCKET_FALLBACK_WEAK
int shutdown(int __fd, int __how)
{
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__fd);
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__how);
    errno = ENOSYS;
    return -1;
}
