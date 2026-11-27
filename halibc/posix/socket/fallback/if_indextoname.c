/***************************************************************
 * Name:      if_indextoname.c
 * Purpose:   实现if_indextoname接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "net/if.h"
#include "errno.h"
#include "halibc_posix_socket_fallback.h"

HALIBC_POSIX_SOCKET_FALLBACK_WEAK
char *if_indextoname(unsigned int ifindex, char ifname[IF_NAMESIZE])
{
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(ifindex);
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(ifname);
    errno = ENOSYS;
    return NULL;
}
