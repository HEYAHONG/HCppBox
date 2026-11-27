/***************************************************************
 * Name:      udp.h
 * Purpose:   声明UDP兼容接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT AND LGPL-2.1-or-later AND BSD-3-Clause
 **************************************************************/
/* Copyright (C) 1991-2026 Free Software Foundation, Inc.
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

/* Portions Copyright (c) 1982, 1986 Regents of the University of California.
   All rights reserved.  Distributed under the BSD-3-Clause license.  */

#ifndef __HALIBC_POSIX_SOCKET_NETINET_UDP_H__
#define __HALIBC_POSIX_SOCKET_NETINET_UDP_H__

#include "stdint.h"
#include "netinet/in.h"

#ifndef HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS
#define HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS 0
#endif

#if HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS

struct udphdr
{
    __extension__ union
    {
	struct
	{
	    uint16_t uh_sport;
	    uint16_t uh_dport;
	    uint16_t uh_ulen;
	    uint16_t uh_sum;
	};
	struct
	{
	    uint16_t source;
	    uint16_t dest;
	    uint16_t len;
	    uint16_t check;
	};
    };
};

#define UDP_CORK	1
#define UDP_ENCAP	100
#define UDP_NO_CHECK6_TX 101
#define UDP_NO_CHECK6_RX 102
#define UDP_SEGMENT	103
#define UDP_GRO		104

#define UDP_ENCAP_ESPINUDP_NON_IKE 1
#define UDP_ENCAP_ESPINUDP	2
#define UDP_ENCAP_L2TPINUDP	3
#define UDP_ENCAP_GTP0		4
#define UDP_ENCAP_GTP1U		5

#define SOL_UDP 17

#endif /* HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS */

#endif /* __HALIBC_POSIX_SOCKET_NETINET_UDP_H__ */
