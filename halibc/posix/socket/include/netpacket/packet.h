/***************************************************************
 * Name:      packet.h
 * Purpose:   声明Linux AF_PACKET socket兼容接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT AND LGPL-2.1-or-later
 **************************************************************/
/* Definitions for use with Linux AF_PACKET sockets.
   Copyright (C) 1998-2026 Free Software Foundation, Inc.
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

#ifndef __HALIBC_POSIX_SOCKET_NETPACKET_PACKET_H__
#define __HALIBC_POSIX_SOCKET_NETPACKET_PACKET_H__

#include "sys/socket.h"

#ifndef HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS
#define HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS 0
#endif

#if HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS

struct sockaddr_ll
{
    unsigned short int sll_family;
    unsigned short int sll_protocol;
    int sll_ifindex;
    unsigned short int sll_hatype;
    unsigned char sll_pkttype;
    unsigned char sll_halen;
    unsigned char sll_addr[8];
};

#define PACKET_HOST		0
#define PACKET_BROADCAST	1
#define PACKET_MULTICAST	2
#define PACKET_OTHERHOST	3
#define PACKET_OUTGOING		4
#define PACKET_LOOPBACK		5
#define PACKET_FASTROUTE	6

#define PACKET_ADD_MEMBERSHIP		1
#define PACKET_DROP_MEMBERSHIP		2
#define PACKET_RECV_OUTPUT		3
#define PACKET_RX_RING			5
#define PACKET_STATISTICS		6
#define PACKET_COPY_THRESH		7
#define PACKET_AUXDATA			8
#define PACKET_ORIGDEV			9
#define PACKET_VERSION			10
#define PACKET_HDRLEN			11
#define PACKET_RESERVE			12
#define PACKET_TX_RING			13
#define PACKET_LOSS			14
#define PACKET_VNET_HDR			15
#define PACKET_TX_TIMESTAMP		16
#define PACKET_TIMESTAMP		17
#define PACKET_FANOUT			18
#define PACKET_TX_HAS_OFF		19
#define PACKET_QDISC_BYPASS		20
#define PACKET_ROLLOVER_STATS		21
#define PACKET_FANOUT_DATA		22
#define PACKET_IGNORE_OUTGOING		23
#define PACKET_VNET_HDR_SZ		24

struct packet_mreq
{
    int mr_ifindex;
    unsigned short int mr_type;
    unsigned short int mr_alen;
    unsigned char mr_address[8];
};

#define PACKET_MR_MULTICAST	0
#define PACKET_MR_PROMISC	1
#define PACKET_MR_ALLMULTI	2
#define PACKET_MR_UNICAST	3

#endif /* HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS */

#endif /* __HALIBC_POSIX_SOCKET_NETPACKET_PACKET_H__ */
