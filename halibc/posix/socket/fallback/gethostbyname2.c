/***************************************************************
 * Name:      gethostbyname2.c
 * Purpose:   实现gethostbyname2接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "netdb.h"
#include "errno.h"
#include "halibc_posix_socket_fallback.h"

HALIBC_POSIX_SOCKET_FALLBACK_WEAK
struct hostent * gethostbyname2(const char *__name, int __af)
{
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__name);
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__af);
    errno = ENOSYS;
    return NULL;
}
