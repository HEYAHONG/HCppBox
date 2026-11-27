/***************************************************************
 * Name:      icmp6.h
 * Purpose:   声明ICMPv6兼容接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT AND LGPL-2.1-or-later
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

#ifndef __HALIBC_POSIX_SOCKET_NETINET_ICMP6_H__
#define __HALIBC_POSIX_SOCKET_NETINET_ICMP6_H__

#include "stdint.h"
#include "string.h"
#include "endian.h"
#include "netinet/in.h"

#ifndef HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS
#define HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS 0
#endif

#if HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS

#define ICMP6_FILTER 1
#define ICMP6_FILTER_BLOCK		1
#define ICMP6_FILTER_PASS		2
#define ICMP6_FILTER_BLOCKOTHERS	3
#define ICMP6_FILTER_PASSONLY		4

struct icmp6_filter
{
    uint32_t icmp6_filt[8];
};

struct icmp6_hdr
{
    uint8_t icmp6_type;
    uint8_t icmp6_code;
    uint16_t icmp6_cksum;
    union
    {
	uint32_t icmp6_un_data32[1];
	uint16_t icmp6_un_data16[2];
	uint8_t icmp6_un_data8[4];
    } icmp6_dataun;
};

#define icmp6_data32	icmp6_dataun.icmp6_un_data32
#define icmp6_data16	icmp6_dataun.icmp6_un_data16
#define icmp6_data8	icmp6_dataun.icmp6_un_data8
#define icmp6_pptr	icmp6_data32[0]
#define icmp6_mtu	icmp6_data32[0]
#define icmp6_id	icmp6_data16[0]
#define icmp6_seq	icmp6_data16[1]
#define icmp6_maxdelay	icmp6_data16[0]

#define ICMP6_DST_UNREACH		1
#define ICMP6_PACKET_TOO_BIG		2
#define ICMP6_TIME_EXCEEDED		3
#define ICMP6_PARAM_PROB		4
#define ICMP6_ECHO_REQUEST		128
#define ICMP6_ECHO_REPLY		129
#define MLD_LISTENER_QUERY		130
#define MLD_LISTENER_REPORT		131
#define MLD_LISTENER_REDUCTION		132
#define ICMPV6_EXT_ECHO_REQUEST		160
#define ICMPV6_EXT_ECHO_REPLY		161

#define ICMP6_DST_UNREACH_NOROUTE	0
#define ICMP6_DST_UNREACH_ADMIN		1
#define ICMP6_DST_UNREACH_BEYONDSCOPE	2
#define ICMP6_DST_UNREACH_ADDR		3
#define ICMP6_DST_UNREACH_NOPORT	4
#define ICMP6_TIME_EXCEED_TRANSIT	0
#define ICMP6_TIME_EXCEED_REASSEMBLY	1
#define ICMP6_PARAMPROB_HEADER		0
#define ICMP6_PARAMPROB_NEXTHEADER	1
#define ICMP6_PARAMPROB_OPTION		2

#define ICMP6_FILTER_WILLPASS(type, filterp) \
    ((((filterp)->icmp6_filt[(type) >> 5]) & (1U << ((type) & 31))) == 0)
#define ICMP6_FILTER_WILLBLOCK(type, filterp) \
    ((((filterp)->icmp6_filt[(type) >> 5]) & (1U << ((type) & 31))) != 0)
#define ICMP6_FILTER_SETPASS(type, filterp) \
    (((filterp)->icmp6_filt[(type) >> 5] &= ~(1U << ((type) & 31))))
#define ICMP6_FILTER_SETBLOCK(type, filterp) \
    (((filterp)->icmp6_filt[(type) >> 5] |= (1U << ((type) & 31))))
#define ICMP6_FILTER_SETPASSALL(filterp) \
    memset (filterp, 0, sizeof (struct icmp6_filter))
#define ICMP6_FILTER_SETBLOCKALL(filterp) \
    memset (filterp, 0xFF, sizeof (struct icmp6_filter))

#define ND_ROUTER_SOLICIT		133
#define ND_ROUTER_ADVERT		134
#define ND_NEIGHBOR_SOLICIT		135
#define ND_NEIGHBOR_ADVERT		136
#define ND_REDIRECT			137

struct nd_router_solicit
{
    struct icmp6_hdr nd_rs_hdr;
};

struct nd_router_advert
{
    struct icmp6_hdr nd_ra_hdr;
    uint32_t nd_ra_reachable;
    uint32_t nd_ra_retransmit;
};

struct nd_neighbor_solicit
{
    struct icmp6_hdr nd_ns_hdr;
    struct in6_addr nd_ns_target;
};

struct nd_neighbor_advert
{
    struct icmp6_hdr nd_na_hdr;
    struct in6_addr nd_na_target;
};

struct nd_redirect
{
    struct icmp6_hdr nd_rd_hdr;
    struct in6_addr nd_rd_target;
    struct in6_addr nd_rd_dst;
};

struct nd_opt_hdr
{
    uint8_t nd_opt_type;
    uint8_t nd_opt_len;
};

#define ND_OPT_SOURCE_LINKADDR		1
#define ND_OPT_TARGET_LINKADDR		2
#define ND_OPT_PREFIX_INFORMATION	3
#define ND_OPT_REDIRECTED_HEADER	4
#define ND_OPT_MTU			5
#define ND_OPT_RTR_ADV_INTERVAL		7
#define ND_OPT_HOME_AGENT_INFO		8

struct nd_opt_prefix_info
{
    uint8_t nd_opt_pi_type;
    uint8_t nd_opt_pi_len;
    uint8_t nd_opt_pi_prefix_len;
    uint8_t nd_opt_pi_flags_reserved;
    uint32_t nd_opt_pi_valid_time;
    uint32_t nd_opt_pi_preferred_time;
    uint32_t nd_opt_pi_reserved2;
    struct in6_addr nd_opt_pi_prefix;
};

struct nd_opt_mtu
{
    uint8_t nd_opt_mtu_type;
    uint8_t nd_opt_mtu_len;
    uint16_t nd_opt_mtu_reserved;
    uint32_t nd_opt_mtu_mtu;
};

#endif /* HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS */

#endif /* __HALIBC_POSIX_SOCKET_NETINET_ICMP6_H__ */
