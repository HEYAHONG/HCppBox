/***************************************************************
 * Name:      freeifaddrs.c
 * Purpose:   实现freeifaddrs接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "ifaddrs.h"
#include "errno.h"
#include "halibc_posix_socket_fallback.h"

HALIBC_POSIX_SOCKET_FALLBACK_WEAK
void freeifaddrs(struct ifaddrs *__ifa)
{
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__ifa);
}
