/***************************************************************
 * Name:      ip6.h
 * Purpose:   声明IPv6兼容接口
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

#ifndef __HALIBC_POSIX_SOCKET_NETINET_IP6_H__
#define __HALIBC_POSIX_SOCKET_NETINET_IP6_H__

#include "stdint.h"
#include "endian.h"
#include "netinet/in.h"

#ifndef HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS
#define HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS 0
#endif

#if HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS

struct ip6_hdr
{
    union
    {
	struct ip6_hdrctl
	{
	    uint32_t ip6_un1_flow;
	    uint16_t ip6_un1_plen;
	    uint8_t ip6_un1_nxt;
	    uint8_t ip6_un1_hlim;
	} ip6_un1;
	uint8_t ip6_un2_vfc;
    } ip6_ctlun;
    struct in6_addr ip6_src;
    struct in6_addr ip6_dst;
};

#define ip6_vfc   ip6_ctlun.ip6_un2_vfc
#define ip6_flow  ip6_ctlun.ip6_un1.ip6_un1_flow
#define ip6_plen  ip6_ctlun.ip6_un1.ip6_un1_plen
#define ip6_nxt   ip6_ctlun.ip6_un1.ip6_un1_nxt
#define ip6_hlim  ip6_ctlun.ip6_un1.ip6_un1_hlim
#define ip6_hops  ip6_ctlun.ip6_un1.ip6_un1_hlim

#define IPV6_VERSION       0x60
#define IPV6_VERSION_MASK  0xf0

#if _BYTE_ORDER == _BIG_ENDIAN
#define IPV6_FLOWINFO_MASK   0x0fffffff
#define IPV6_FLOWLABEL_MASK  0x000fffff
#else
#define IPV6_FLOWINFO_MASK   0xffffff0f
#define IPV6_FLOWLABEL_MASK  0xffff0f00
#endif

struct ip6_ext
{
    uint8_t ip6e_nxt;
    uint8_t ip6e_len;
};

struct ip6_hbh
{
    uint8_t ip6h_nxt;
    uint8_t ip6h_len;
};

struct ip6_dest
{
    uint8_t ip6d_nxt;
    uint8_t ip6d_len;
};

struct ip6_rthdr
{
    uint8_t ip6r_nxt;
    uint8_t ip6r_len;
    uint8_t ip6r_type;
    uint8_t ip6r_segleft;
};

struct ip6_rthdr0
{
    uint8_t ip6r0_nxt;
    uint8_t ip6r0_len;
    uint8_t ip6r0_type;
    uint8_t ip6r0_segleft;
    uint8_t ip6r0_reserved;
    uint8_t ip6r0_slmap[3];
    struct in6_addr ip6r0_addr[1];
};

struct ip6_frag
{
    uint8_t ip6f_nxt;
    uint8_t ip6f_reserved;
    uint16_t ip6f_offlg;
    uint32_t ip6f_ident;
};

#if _BYTE_ORDER == _BIG_ENDIAN
#define IP6F_OFF_MASK       0xfff8
#define IP6F_RESERVED_MASK  0x0006
#define IP6F_MORE_FRAG      0x0001
#else
#define IP6F_OFF_MASK       0xf8ff
#define IP6F_RESERVED_MASK  0x0600
#define IP6F_MORE_FRAG      0x0100
#endif

struct ip6_opt
{
    uint8_t ip6o_type;
    uint8_t ip6o_len;
};

#define IP6OPT_TYPE(o)		((o) & 0xc0)
#define IP6OPT_TYPE_SKIP	0x00
#define IP6OPT_TYPE_DISCARD	0x40
#define IP6OPT_TYPE_FORCEICMP	0x80
#define IP6OPT_TYPE_ICMP	0xc0
#define IP6OPT_TYPE_MUTABLE	0x20

#define IP6OPT_PAD1	0
#define IP6OPT_PADN	1
#define IP6OPT_JUMBO	0xc2
#define IP6OPT_NSAP_ADDR 0xc3
#define IP6OPT_TUNNEL_LIMIT 0x04
#define IP6OPT_ROUTER_ALERT 0x05

struct ip6_opt_jumbo
{
    uint8_t ip6oj_type;
    uint8_t ip6oj_len;
    uint8_t ip6oj_jumbo_len[4];
};

#define IP6OPT_JUMBO_LEN 6

struct ip6_opt_nsap
{
    uint8_t ip6on_type;
    uint8_t ip6on_len;
    uint8_t ip6on_src_nsap_len;
    uint8_t ip6on_dst_nsap_len;
};

struct ip6_opt_tunnel
{
    uint8_t ip6ot_type;
    uint8_t ip6ot_len;
    uint8_t ip6ot_encap_limit;
};

struct ip6_opt_router
{
    uint8_t ip6or_type;
    uint8_t ip6or_len;
    uint8_t ip6or_value[2];
};

#endif /* HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS */

#endif /* __HALIBC_POSIX_SOCKET_NETINET_IP6_H__ */
