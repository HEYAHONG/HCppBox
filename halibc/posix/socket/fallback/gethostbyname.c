/***************************************************************
 * Name:      gethostbyname.c
 * Purpose:   实现gethostbyname接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "netdb.h"
#include "errno.h"
#include "halibc_posix_socket_fallback.h"

HALIBC_POSIX_SOCKET_FALLBACK_WEAK
struct hostent * gethostbyname(const char *__name)
{
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__name);
    errno = ENOSYS;
    return NULL;
}
