/***************************************************************
 * Name:      herror.c
 * Purpose:   实现herror接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "netdb.h"
#include "errno.h"
#include "halibc_posix_socket_fallback.h"

HALIBC_POSIX_SOCKET_FALLBACK_WEAK
void herror(const char *__str)
{
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__str);
}
