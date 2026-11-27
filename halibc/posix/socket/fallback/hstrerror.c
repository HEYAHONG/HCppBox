/***************************************************************
 * Name:      hstrerror.c
 * Purpose:   实现hstrerror接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "netdb.h"
#include "errno.h"
#include "halibc_posix_socket_fallback.h"

HALIBC_POSIX_SOCKET_FALLBACK_WEAK
const char * hstrerror(int __err_num)
{
    HALIBC_POSIX_SOCKET_FALLBACK_UNUSED(__err_num);
    return "unknown error";
}
