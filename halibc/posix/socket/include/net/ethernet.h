/***************************************************************
 * Name:      ethernet.h
 * Purpose:   声明以太网兼容接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT AND LGPL-2.1-or-later
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

#ifndef __HALIBC_POSIX_SOCKET_NET_ETHERNET_H__
#define __HALIBC_POSIX_SOCKET_NET_ETHERNET_H__

#include "stdint.h"
#include "sys/socket.h"

#ifndef HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS
#define HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS 0
#endif

#if HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS

#define ETH_ALEN 6
#define ETH_HLEN 14
#define ETH_ZLEN 60
#define ETH_DATA_LEN 1500
#define ETH_FRAME_LEN 1514
#define ETH_FCS_LEN 4

struct ether_addr
{
    uint8_t ether_addr_octet[ETH_ALEN];
} __attribute__ ((__packed__));

struct ether_header
{
    uint8_t ether_dhost[ETH_ALEN];
    uint8_t ether_shost[ETH_ALEN];
    uint16_t ether_type;
} __attribute__ ((__packed__));

#define ETHERTYPE_PUP		0x0200
#define ETHERTYPE_SPRITE	0x0500
#define ETHERTYPE_IP		0x0800
#define ETHERTYPE_ARP		0x0806
#define ETHERTYPE_REVARP	0x8035
#define ETHERTYPE_AT		0x809B
#define ETHERTYPE_AARP		0x80F3
#define ETHERTYPE_VLAN		0x8100
#define ETHERTYPE_IPX		0x8137
#define ETHERTYPE_IPV6		0x86dd
#define ETHERTYPE_LOOPBACK	0x9000
#define ETHERTYPE_TRAIL		0x1000
#define ETHERTYPE_NTRAILER	16

#define ETHER_ADDR_LEN	ETH_ALEN
#define ETHER_TYPE_LEN	2
#define ETHER_CRC_LEN	ETH_FCS_LEN
#define ETHER_HDR_LEN	ETH_HLEN
#define ETHER_MIN_LEN	(ETH_ZLEN + ETHER_CRC_LEN)
#define ETHER_MAX_LEN	(ETH_FRAME_LEN + ETHER_CRC_LEN)
#define ETHER_IS_VALID_LEN(foo) \
    ((foo) >= ETHER_MIN_LEN && (foo) <= ETHER_MAX_LEN)
#define ETHERMTU	ETH_DATA_LEN
#define ETHERMIN	(ETHER_MIN_LEN - ETHER_HDR_LEN - ETHER_CRC_LEN)

#endif /* HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS */

#endif /* __HALIBC_POSIX_SOCKET_NET_ETHERNET_H__ */
