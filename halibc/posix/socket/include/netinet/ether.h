/***************************************************************
 * Name:      ether.h
 * Purpose:   声明以太网地址转换兼容接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT AND LGPL-2.1-or-later
 **************************************************************/
/* Functions for storing Ethernet addresses in ASCII and mapping to hostnames.
   Copyright (C) 1996-2026 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */

#ifndef __HALIBC_POSIX_SOCKET_NETINET_ETHER_H__
#define __HALIBC_POSIX_SOCKET_NETINET_ETHER_H__

#include "netinet/if_ether.h"

#ifndef HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS
#define HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS 0
#endif

#if HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS

__BEGIN_DECLS

extern char *ether_ntoa (const struct ether_addr *__addr);
extern char *ether_ntoa_r (const struct ether_addr *__addr, char *__buf);
extern struct ether_addr *ether_aton (const char *__asc);
extern struct ether_addr *ether_aton_r (const char *__asc,
					struct ether_addr *__addr);
extern int ether_ntohost (char *__hostname, const struct ether_addr *__addr);
extern int ether_hostton (const char *__hostname, struct ether_addr *__addr);
extern int ether_line (const char *__line, struct ether_addr *__addr,
		       char *__hostname);

__END_DECLS

#endif /* HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS */

#endif /* __HALIBC_POSIX_SOCKET_NETINET_ETHER_H__ */
