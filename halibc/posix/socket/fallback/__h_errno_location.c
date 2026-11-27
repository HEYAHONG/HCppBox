/***************************************************************
 * Name:      __h_errno_location.c
 * Purpose:   实现__h_errno_location接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "netdb.h"
#include "errno.h"
#include "halibc_posix_socket_fallback.h"

HALIBC_POSIX_SOCKET_FALLBACK_WEAK
int * __h_errno_location(void)
{
    static int __h_errno_value = 0;
    return &__h_errno_value;
}
