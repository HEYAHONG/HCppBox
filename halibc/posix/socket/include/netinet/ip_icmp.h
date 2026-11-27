/***************************************************************
 * Name:      ip_icmp.h
 * Purpose:   声明ICMP兼容接口
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

#ifndef __HALIBC_POSIX_SOCKET_NETINET_IP_ICMP_H__
#define __HALIBC_POSIX_SOCKET_NETINET_IP_ICMP_H__

#include "stdint.h"
#include "netinet/in.h"
#include "netinet/ip.h"

#ifndef HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS
#define HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS 0
#endif

#if HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS

struct icmphdr
{
    uint8_t type;
    uint8_t code;
    uint16_t checksum;
    union
    {
	struct
	{
	    uint16_t id;
	    uint16_t sequence;
	} echo;
	uint32_t gateway;
	struct
	{
	    uint16_t __reserved;
	    uint16_t mtu;
	} frag;
    } un;
};

#define ICMP_ECHOREPLY		0
#define ICMP_DEST_UNREACH	3
#define ICMP_SOURCE_QUENCH	4
#define ICMP_REDIRECT		5
#define ICMP_ECHO		8
#define ICMP_TIME_EXCEEDED	11
#define ICMP_PARAMETERPROB	12
#define ICMP_TIMESTAMP		13
#define ICMP_TIMESTAMPREPLY	14
#define ICMP_INFO_REQUEST	15
#define ICMP_INFO_REPLY		16
#define ICMP_ADDRESS		17
#define ICMP_ADDRESSREPLY	18
#define NR_ICMP_TYPES		18

#define ICMP_NET_UNREACH	0
#define ICMP_HOST_UNREACH	1
#define ICMP_PROT_UNREACH	2
#define ICMP_PORT_UNREACH	3
#define ICMP_FRAG_NEEDED	4
#define ICMP_SR_FAILED		5
#define ICMP_NET_UNKNOWN	6
#define ICMP_HOST_UNKNOWN	7
#define ICMP_HOST_ISOLATED	8
#define ICMP_NET_ANO		9
#define ICMP_HOST_ANO		10
#define ICMP_NET_UNR_TOS	11
#define ICMP_HOST_UNR_TOS	12
#define ICMP_PKT_FILTERED	13
#define ICMP_PREC_VIOLATION	14
#define ICMP_PREC_CUTOFF	15
#define NR_ICMP_UNREACH		15

#define ICMP_REDIR_NET		0
#define ICMP_REDIR_HOST		1
#define ICMP_REDIR_NETTOS	2
#define ICMP_REDIR_HOSTTOS	3

#define ICMP_EXC_TTL		0
#define ICMP_EXC_FRAGTIME	1

#define ICMP_EXT_ECHO		42
#define ICMP_EXT_ECHOREPLY	43

struct icmp
{
    uint8_t icmp_type;
    uint8_t icmp_code;
    uint16_t icmp_cksum;
    union
    {
	unsigned char ih_pptr;
	struct in_addr ih_gwaddr;
	struct
	{
	    uint16_t icd_id;
	    uint16_t icd_seq;
	} ih_idseq;
	uint32_t ih_void;
	struct
	{
	    uint16_t ipm_void;
	    uint16_t ipm_nextmtu;
	} ih_pmtu;
    } icmp_hun;
#define icmp_pptr	icmp_hun.ih_pptr
#define icmp_gwaddr	icmp_hun.ih_gwaddr
#define icmp_id		icmp_hun.ih_idseq.icd_id
#define icmp_seq	icmp_hun.ih_idseq.icd_seq
#define icmp_void	icmp_hun.ih_void
#define icmp_pmvoid	icmp_hun.ih_pmtu.ipm_void
#define icmp_nextmtu	icmp_hun.ih_pmtu.ipm_nextmtu
    union
    {
	struct
	{
	    uint32_t its_otime;
	    uint32_t its_rtime;
	    uint32_t its_ttime;
	} id_ts;
	struct
	{
	    struct ip idi_ip;
	} id_ip;
	uint32_t id_mask;
	uint8_t id_data[1];
    } icmp_dun;
#define icmp_otime	icmp_dun.id_ts.its_otime
#define icmp_rtime	icmp_dun.id_ts.its_rtime
#define icmp_ttime	icmp_dun.id_ts.its_ttime
#define icmp_ip		icmp_dun.id_ip.idi_ip
#define icmp_mask	icmp_dun.id_mask
#define icmp_data	icmp_dun.id_data
};

#endif /* HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS */

#endif /* __HALIBC_POSIX_SOCKET_NETINET_IP_ICMP_H__ */
