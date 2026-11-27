/***************************************************************
 * Name:      gai_strerror.c
 * Purpose:   实现gai_strerror接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "netdb.h"
#include "errno.h"
#include "halibc_posix_socket_fallback.h"

HALIBC_POSIX_SOCKET_FALLBACK_WEAK
const char * gai_strerror(int __ecode)
{
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__ecode);
    return "unknown error";
}
