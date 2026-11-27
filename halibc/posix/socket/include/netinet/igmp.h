/***************************************************************
 * Name:      igmp.h
 * Purpose:   声明IGMP兼容接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT AND LGPL-2.1-or-later AND BSD-3-Clause
 **************************************************************/
/* Copyright (C) 1997-2026 Free Software Foundation, Inc.
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

/* Portions Copyright (c) 1988 Stephen Deering.
   Portions Copyright (c) 1992, 1993
   The Regents of the University of California.  All rights reserved.
   Distributed under the BSD-3-Clause license.  */

#ifndef __HALIBC_POSIX_SOCKET_NETINET_IGMP_H__
#define __HALIBC_POSIX_SOCKET_NETINET_IGMP_H__

#include "stdint.h"
#include "netinet/in.h"

#ifndef HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS
#define HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS 0
#endif

#if HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS

struct igmp
{
    uint8_t igmp_type;
    uint8_t igmp_code;
    uint16_t igmp_cksum;
    struct in_addr igmp_group;
};

#define IGMP_MINLEN			8
#define IGMP_MEMBERSHIP_QUERY		0x11
#define IGMP_V1_MEMBERSHIP_REPORT	0x12
#define IGMP_V2_MEMBERSHIP_REPORT	0x16
#define IGMP_V2_LEAVE_GROUP		0x17
#define IGMP_DVMRP			0x13
#define IGMP_PIM			0x14
#define IGMP_TRACE			0x15
#define IGMP_MTRACE_RESP		0x1e
#define IGMP_MTRACE			0x1f
#define IGMP_MRDISC_ADV			0x30
#define IGMP_MAX_HOST_REPORT_DELAY	10
#define IGMP_TIMER_SCALE		10
#define IGMP_DELAYING_MEMBER		1
#define IGMP_IDLE_MEMBER		2
#define IGMP_LAZY_MEMBER		3
#define IGMP_SLEEPING_MEMBER		4
#define IGMP_AWAKENING_MEMBER		5
#define IGMP_v1_ROUTER			1
#define IGMP_v2_ROUTER			2

#define IGMP_HOST_MEMBERSHIP_QUERY	IGMP_MEMBERSHIP_QUERY
#define IGMP_HOST_MEMBERSHIP_REPORT	IGMP_V1_MEMBERSHIP_REPORT
#define IGMP_HOST_NEW_MEMBERSHIP_REPORT	IGMP_V2_MEMBERSHIP_REPORT
#define IGMP_HOST_LEAVE_MESSAGE		IGMP_V2_LEAVE_GROUP

#endif /* HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS */

#endif /* __HALIBC_POSIX_SOCKET_NETINET_IGMP_H__ */
