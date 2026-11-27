/***************************************************************
 * Name:      if_ether.h
 * Purpose:   声明以太网ARP兼容接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT AND LGPL-2.1-or-later AND BSD-3-Clause
 **************************************************************/
/* Copyright (C) 1996-2026 Free Software Foundation, Inc.
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

/* Portions Copyright (c) 1982, 1986, 1993
   The Regents of the University of California.  All rights reserved.
   Distributed under the BSD-3-Clause license.  */

#ifndef __HALIBC_POSIX_SOCKET_NETINET_IF_ETHER_H__
#define __HALIBC_POSIX_SOCKET_NETINET_IF_ETHER_H__

#include "stdint.h"
#include "net/ethernet.h"
#include "net/if_arp.h"

#ifndef HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS
#define HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS 0
#endif

#if HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS

struct ether_arp
{
    struct arphdr ea_hdr;
    uint8_t arp_sha[ETH_ALEN];
    uint8_t arp_spa[4];
    uint8_t arp_tha[ETH_ALEN];
    uint8_t arp_tpa[4];
};

#define arp_hrd	ea_hdr.ar_hrd
#define arp_pro	ea_hdr.ar_pro
#define arp_hln	ea_hdr.ar_hln
#define arp_pln	ea_hdr.ar_pln
#define arp_op	ea_hdr.ar_op

#define ETHER_MAP_IP_MULTICAST(ipaddr, enaddr) \
    { \
	(enaddr)[0] = 0x01; \
	(enaddr)[1] = 0x00; \
	(enaddr)[2] = 0x5e; \
	(enaddr)[3] = ((uint8_t *)(ipaddr))[1] & 0x7f; \
	(enaddr)[4] = ((uint8_t *)(ipaddr))[2]; \
	(enaddr)[5] = ((uint8_t *)(ipaddr))[3]; \
    }

#endif /* HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS */

#endif /* __HALIBC_POSIX_SOCKET_NETINET_IF_ETHER_H__ */
