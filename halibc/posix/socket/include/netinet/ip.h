/***************************************************************
 * Name:      ip.h
 * Purpose:   声明IPv4兼容接口
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

/* Portions Copyright (c) 1982, 1986, 1993
   The Regents of the University of California.  All rights reserved.
   Distributed under the BSD-3-Clause license.  */

#ifndef __HALIBC_POSIX_SOCKET_NETINET_IP_H__
#define __HALIBC_POSIX_SOCKET_NETINET_IP_H__

#include "stdint.h"
#include "endian.h"
#include "netinet/in.h"

#ifndef HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS
#define HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS 0
#endif

#if HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS

struct timestamp
{
    uint8_t len;
    uint8_t ptr;
#if _BYTE_ORDER == _LITTLE_ENDIAN
    unsigned int flags:4;
    unsigned int overflow:4;
#else
    unsigned int overflow:4;
    unsigned int flags:4;
#endif
    uint32_t data[9];
};

struct iphdr
{
#if _BYTE_ORDER == _LITTLE_ENDIAN
    unsigned int ihl:4;
    unsigned int version:4;
#else
    unsigned int version:4;
    unsigned int ihl:4;
#endif
    uint8_t tos;
    uint16_t tot_len;
    uint16_t id;
    uint16_t frag_off;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t check;
    uint32_t saddr;
    uint32_t daddr;
};

struct ip
{
#if _BYTE_ORDER == _LITTLE_ENDIAN
    unsigned int ip_hl:4;
    unsigned int ip_v:4;
#else
    unsigned int ip_v:4;
    unsigned int ip_hl:4;
#endif
    uint8_t ip_tos;
    unsigned short ip_len;
    unsigned short ip_id;
    unsigned short ip_off;
#define IP_RF 0x8000
#define IP_DF 0x4000
#define IP_MF 0x2000
#define IP_OFFMASK 0x1fff
    uint8_t ip_ttl;
    uint8_t ip_p;
    unsigned short ip_sum;
    struct in_addr ip_src, ip_dst;
};

struct ip_timestamp
{
    uint8_t ipt_code;
    uint8_t ipt_len;
    uint8_t ipt_ptr;
#if _BYTE_ORDER == _LITTLE_ENDIAN
    unsigned int ipt_flg:4;
    unsigned int ipt_oflw:4;
#else
    unsigned int ipt_oflw:4;
    unsigned int ipt_flg:4;
#endif
    uint32_t data[9];
};

#define IPVERSION 4
#define IP_MAXPACKET 65535

#define IPTOS_ECN_MASK		0x03
#define IPTOS_ECN(x)		((x) & IPTOS_ECN_MASK)
#define IPTOS_ECN_NOT_ECT	0x00
#define IPTOS_ECN_ECT1		0x01
#define IPTOS_ECN_ECT0		0x02
#define IPTOS_ECN_CE		0x03

#define IPTOS_DSCP_MASK		0xfc
#define IPTOS_DSCP(x)		((x) & IPTOS_DSCP_MASK)
#define IPTOS_DSCP_AF11		0x28
#define IPTOS_DSCP_AF12		0x30
#define IPTOS_DSCP_AF13		0x38
#define IPTOS_DSCP_AF21		0x48
#define IPTOS_DSCP_AF22		0x50
#define IPTOS_DSCP_AF23		0x58
#define IPTOS_DSCP_AF31		0x68
#define IPTOS_DSCP_AF32		0x70
#define IPTOS_DSCP_AF33		0x78
#define IPTOS_DSCP_AF41		0x88
#define IPTOS_DSCP_AF42		0x90
#define IPTOS_DSCP_AF43		0x98
#define IPTOS_DSCP_EF		0xb8

#define IPTOS_CLASS_MASK	0xe0
#define IPTOS_CLASS(class)	((class) & IPTOS_CLASS_MASK)
#define IPTOS_CLASS_CS0		0x00
#define IPTOS_CLASS_CS1		0x20
#define IPTOS_CLASS_CS2		0x40
#define IPTOS_CLASS_CS3		0x60
#define IPTOS_CLASS_CS4		0x80
#define IPTOS_CLASS_CS5		0xa0
#define IPTOS_CLASS_CS6		0xc0
#define IPTOS_CLASS_CS7		0xe0

#define IPTOS_TOS_MASK		0x1E
#define IPTOS_TOS(tos)		((tos) & IPTOS_TOS_MASK)
#define IPTOS_LOWDELAY		0x10
#define IPTOS_THROUGHPUT	0x08
#define IPTOS_RELIABILITY	0x04
#define IPTOS_LOWCOST		0x02
#define IPTOS_MINCOST		IPTOS_LOWCOST

#define IPOPT_COPY		0x80
#define IPOPT_CLASS_MASK	0x60
#define IPOPT_NUMBER_MASK	0x1f
#define IPOPT_COPIED(o)		((o) & IPOPT_COPY)
#define IPOPT_CLASS(o)		((o) & IPOPT_CLASS_MASK)
#define IPOPT_NUMBER(o)		((o) & IPOPT_NUMBER_MASK)

#define IPOPT_EOL		0
#define IPOPT_NOP		1
#define IPOPT_RR		7
#define IPOPT_TS		68
#define IPOPT_SECURITY		130
#define IPOPT_LSRR		131
#define IPOPT_SATID		136
#define IPOPT_SSRR		137
#define IPOPT_RA		148

#define MAX_IPOPTLEN		40
#define MAXTTL			255
#define IPDEFTTL		64
#define IP_MSS			576

#endif /* HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS */

#endif /* __HALIBC_POSIX_SOCKET_NETINET_IP_H__ */
