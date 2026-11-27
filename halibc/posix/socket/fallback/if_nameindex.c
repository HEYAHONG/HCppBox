/***************************************************************
 * Name:      if_nameindex.c
 * Purpose:   实现if_nameindex接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "net/if.h"
#include "errno.h"
#include "halibc_posix_socket_fallback.h"

HALIBC_POSIX_SOCKET_FALLBACK_WEAK
struct if_nameindex *if_nameindex(void)
{
    errno = ENOSYS;
    return NULL;
}
