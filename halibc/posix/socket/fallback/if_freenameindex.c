/***************************************************************
 * Name:      if_freenameindex.c
 * Purpose:   实现if_freenameindex接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "net/if.h"
#include "halibc_posix_socket_fallback.h"

HALIBC_POSIX_SOCKET_FALLBACK_WEAK
void if_freenameindex(struct if_nameindex *ptr)
{
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(ptr);
}
