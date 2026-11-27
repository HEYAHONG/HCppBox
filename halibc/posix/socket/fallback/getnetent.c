/***************************************************************
 * Name:      getnetent.c
 * Purpose:   实现getnetent接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "netdb.h"
#include "errno.h"
#include "halibc_posix_socket_fallback.h"

HALIBC_POSIX_SOCKET_FALLBACK_WEAK
struct netent * getnetent(void)
{
    errno = ENOSYS;
    return NULL;
}
