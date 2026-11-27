/***************************************************************
 * Name:      in.h
 * Purpose:   声明IPv4/IPv6兼容接口
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

#ifndef __HALIBC_POSIX_SOCKET_NETINET_IN_H__
#define __HALIBC_POSIX_SOCKET_NETINET_IN_H__

#ifndef HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS
#define HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS 0
#endif

#ifndef _NETINET_IN_H
#define _NETINET_IN_H 1
#endif

#include "stdint.h"
#include "endian.h"
#include "sys/socket.h"

#ifndef __BEGIN_DECLS
#ifdef __cplusplus
#define __BEGIN_DECLS extern "C" {
#define __END_DECLS }
#else
#define __BEGIN_DECLS
#define __END_DECLS
#endif
#endif

__BEGIN_DECLS

/* Internet address.  */
#ifndef _IN_ADDR_T_DECLARED
typedef uint32_t in_addr_t;
#define _IN_ADDR_T_DECLARED
#endif

struct in_addr
{
    in_addr_t s_addr;
};

/* Type to represent a port.  */
#ifndef _IN_PORT_T_DECLARED
typedef uint16_t in_port_t;
#define _IN_PORT_T_DECLARED
#endif

/* Standard well-defined IP protocols.  */
enum
{
    IPPROTO_IP = 0,
#define IPPROTO_IP		IPPROTO_IP
    IPPROTO_ICMP = 1,
#define IPPROTO_ICMP		IPPROTO_ICMP
    IPPROTO_IGMP = 2,
#define IPPROTO_IGMP		IPPROTO_IGMP
    IPPROTO_IPIP = 4,
#define IPPROTO_IPIP		IPPROTO_IPIP
    IPPROTO_TCP = 6,
#define IPPROTO_TCP		IPPROTO_TCP
    IPPROTO_EGP = 8,
#define IPPROTO_EGP		IPPROTO_EGP
    IPPROTO_PUP = 12,
#define IPPROTO_PUP		IPPROTO_PUP
    IPPROTO_UDP = 17,
#define IPPROTO_UDP		IPPROTO_UDP
    IPPROTO_IDP = 22,
#define IPPROTO_IDP		IPPROTO_IDP
    IPPROTO_TP = 29,
#define IPPROTO_TP		IPPROTO_TP
    IPPROTO_DCCP = 33,
#define IPPROTO_DCCP		IPPROTO_DCCP
    IPPROTO_IPV6 = 41,
#define IPPROTO_IPV6		IPPROTO_IPV6
    IPPROTO_RSVP = 46,
#define IPPROTO_RSVP		IPPROTO_RSVP
    IPPROTO_GRE = 47,
#define IPPROTO_GRE		IPPROTO_GRE
    IPPROTO_ESP = 50,
#define IPPROTO_ESP		IPPROTO_ESP
    IPPROTO_AH = 51,
#define IPPROTO_AH		IPPROTO_AH
    IPPROTO_MTP = 92,
#define IPPROTO_MTP		IPPROTO_MTP
    IPPROTO_BEETPH = 94,
#define IPPROTO_BEETPH		IPPROTO_BEETPH
    IPPROTO_ENCAP = 98,
#define IPPROTO_ENCAP		IPPROTO_ENCAP
    IPPROTO_PIM = 103,
#define IPPROTO_PIM		IPPROTO_PIM
    IPPROTO_COMP = 108,
#define IPPROTO_COMP		IPPROTO_COMP
    IPPROTO_L2TP = 115,
#define IPPROTO_L2TP		IPPROTO_L2TP
    IPPROTO_SCTP = 132,
#define IPPROTO_SCTP		IPPROTO_SCTP
    IPPROTO_UDPLITE = 136,
#define IPPROTO_UDPLITE		IPPROTO_UDPLITE
    IPPROTO_MPLS = 137,
#define IPPROTO_MPLS		IPPROTO_MPLS
    IPPROTO_ETHERNET = 143,
#define IPPROTO_ETHERNET	IPPROTO_ETHERNET
    IPPROTO_RAW = 255,
#define IPPROTO_RAW		IPPROTO_RAW
    IPPROTO_SMC = 256,
#define IPPROTO_SMC		IPPROTO_SMC
    IPPROTO_MPTCP = 262,
#define IPPROTO_MPTCP		IPPROTO_MPTCP
    IPPROTO_MAX
};

enum
{
    IPPROTO_HOPOPTS = 0,
#define IPPROTO_HOPOPTS		IPPROTO_HOPOPTS
    IPPROTO_ROUTING = 43,
#define IPPROTO_ROUTING		IPPROTO_ROUTING
    IPPROTO_FRAGMENT = 44,
#define IPPROTO_FRAGMENT	IPPROTO_FRAGMENT
    IPPROTO_ICMPV6 = 58,
#define IPPROTO_ICMPV6		IPPROTO_ICMPV6
    IPPROTO_NONE = 59,
#define IPPROTO_NONE		IPPROTO_NONE
    IPPROTO_DSTOPTS = 60,
#define IPPROTO_DSTOPTS		IPPROTO_DSTOPTS
    IPPROTO_MH = 135
#define IPPROTO_MH		IPPROTO_MH
};

/* Standard well-known ports.  */
enum
{
    IPPORT_ECHO = 7,
    IPPORT_DISCARD = 9,
    IPPORT_SYSTAT = 11,
    IPPORT_DAYTIME = 13,
    IPPORT_NETSTAT = 15,
    IPPORT_FTP = 21,
    IPPORT_TELNET = 23,
    IPPORT_SMTP = 25,
    IPPORT_TIMESERVER = 37,
    IPPORT_NAMESERVER = 42,
    IPPORT_WHOIS = 43,
    IPPORT_MTP = 57,

    IPPORT_TFTP = 69,
    IPPORT_RJE = 77,
    IPPORT_FINGER = 79,
    IPPORT_TTYLINK = 87,
    IPPORT_SUPDUP = 95,

    IPPORT_EXECSERVER = 512,
    IPPORT_LOGINSERVER = 513,
    IPPORT_CMDSERVER = 514,
    IPPORT_EFSSERVER = 520,

    IPPORT_BIFFUDP = 512,
    IPPORT_WHOSERVER = 513,
    IPPORT_ROUTESERVER = 520,

    IPPORT_RESERVED = 1024,
    IPPORT_USERRESERVED = 5000
};

/* Definitions of the bits in an Internet address integer.  */
#define	IN_CLASSA(a)		((((in_addr_t)(a)) & 0x80000000) == 0)
#define	IN_CLASSA_NET		0xff000000
#define	IN_CLASSA_NSHIFT	24
#define	IN_CLASSA_HOST		(0xffffffff & ~IN_CLASSA_NET)
#define	IN_CLASSA_MAX		128

#define	IN_CLASSB(a)		((((in_addr_t)(a)) & 0xc0000000) == 0x80000000)
#define	IN_CLASSB_NET		0xffff0000
#define	IN_CLASSB_NSHIFT	16
#define	IN_CLASSB_HOST		(0xffffffff & ~IN_CLASSB_NET)
#define	IN_CLASSB_MAX		65536

#define	IN_CLASSC(a)		((((in_addr_t)(a)) & 0xe0000000) == 0xc0000000)
#define	IN_CLASSC_NET		0xffffff00
#define	IN_CLASSC_NSHIFT	8
#define	IN_CLASSC_HOST		(0xffffffff & ~IN_CLASSC_NET)

#define	IN_CLASSD(a)		((((in_addr_t)(a)) & 0xf0000000) == 0xe0000000)
#define	IN_MULTICAST(a)		IN_CLASSD(a)

#define	IN_EXPERIMENTAL(a)	((((in_addr_t)(a)) & 0xe0000000) == 0xe0000000)
#define	IN_BADCLASS(a)		((((in_addr_t)(a)) & 0xf0000000) == 0xf0000000)

/* Address to accept any incoming messages.  */
#define	INADDR_ANY		((in_addr_t) 0x00000000)
/* Address to send to all hosts.  */
#define	INADDR_BROADCAST	((in_addr_t) 0xffffffff)
/* Address indicating an error return.  */
#define	INADDR_NONE		((in_addr_t) 0xffffffff)
/* Dummy address for source of ICMPv6 errors converted to IPv4.  */
#define	INADDR_DUMMY		((in_addr_t) 0xc0000008)

/* Network number for local host loopback.  */
#define	IN_LOOPBACKNET		127
/* Address to loopback in software to local host.  */
#ifndef INADDR_LOOPBACK
#define	INADDR_LOOPBACK		((in_addr_t) 0x7f000001)
#endif

/* Defines for Multicast INADDR.  */
#define INADDR_UNSPEC_GROUP	((in_addr_t) 0xe0000000)
#define INADDR_ALLHOSTS_GROUP	((in_addr_t) 0xe0000001)
#define INADDR_ALLRTRS_GROUP    ((in_addr_t) 0xe0000002)
#define INADDR_ALLSNOOPERS_GROUP ((in_addr_t) 0xe000006a)
#define INADDR_MAX_LOCAL_GROUP  ((in_addr_t) 0xe00000ff)

/* IPv6 address */
struct in6_addr
{
    union
    {
	uint8_t	__u6_addr8[16];
	uint16_t __u6_addr16[8];
	uint32_t __u6_addr32[4];
    } __in6_u;
#define s6_addr			__in6_u.__u6_addr8
#define s6_addr16		__in6_u.__u6_addr16
#define s6_addr32		__in6_u.__u6_addr32
};

extern const struct in6_addr in6addr_any;        /* :: */
extern const struct in6_addr in6addr_loopback;   /* ::1 */
#define IN6ADDR_ANY_INIT { { { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 } } }
#define IN6ADDR_LOOPBACK_INIT { { { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 } } }

#ifndef INET_ADDRSTRLEN
#define INET_ADDRSTRLEN 16
#endif
#ifndef INET6_ADDRSTRLEN
#define INET6_ADDRSTRLEN 46
#endif

/* Structure describing an Internet socket address.  */
struct sockaddr_in
{
    __SOCKADDR_COMMON (sin_);
    in_port_t sin_port;			/* Port number.  */
    struct in_addr sin_addr;		/* Internet address.  */

    /* Pad to size of `struct sockaddr'.  */
    unsigned char sin_zero[sizeof (struct sockaddr)
			   - __SOCKADDR_COMMON_SIZE
			   - sizeof (in_port_t)
			   - sizeof (struct in_addr)];
};

/* Ditto, for IPv6.  */
struct sockaddr_in6
{
    __SOCKADDR_COMMON (sin6_);
    in_port_t sin6_port;	/* Transport layer port # */
    uint32_t sin6_flowinfo;	/* IPv6 flow information */
    struct in6_addr sin6_addr;	/* IPv6 address */
    uint32_t sin6_scope_id;	/* IPv6 scope-id */
};

/* IPv4 multicast request.  */
struct ip_mreq
{
    /* IP multicast address of group.  */
    struct in_addr imr_multiaddr;

    /* Local IP address of interface.  */
    struct in_addr imr_interface;
};

/* IPv4 multicast request with interface index.  */
#if HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS
struct ip_mreqn
{
    /* IP multicast address of group.  */
    struct in_addr imr_multiaddr;

    /* Local IP address of interface.  */
    struct in_addr imr_address;

    /* Interface index.  */
    int imr_ifindex;
};

struct ip_mreq_source
{
    /* IP multicast address of group.  */
    struct in_addr imr_multiaddr;

    /* IP address of interface.  */
    struct in_addr imr_interface;

    /* IP address of source.  */
    struct in_addr imr_sourceaddr;
};
#endif

/* Likewise, for IPv6.  */
struct ipv6_mreq
{
    /* IPv6 multicast address of group */
    struct in6_addr ipv6mr_multiaddr;

    /* local interface */
    unsigned int ipv6mr_interface;
};

/* Multicast group request.  */
#if HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS
struct group_req
{
    /* Interface index.  */
    uint32_t gr_interface;

    /* Group address.  */
    struct sockaddr_storage gr_group;
};

struct group_source_req
{
    /* Interface index.  */
    uint32_t gsr_interface;

    /* Group address.  */
    struct sockaddr_storage gsr_group;

    /* Source address.  */
    struct sockaddr_storage gsr_source;
};

/* Full-state filter operations.  */
struct ip_msfilter
{
    /* IP multicast address of group.  */
    struct in_addr imsf_multiaddr;

    /* Local IP address of interface.  */
    struct in_addr imsf_interface;

    /* Filter mode.  */
    uint32_t imsf_fmode;

    /* Number of source addresses.  */
    uint32_t imsf_numsrc;
    /* Source addresses.  */
    struct in_addr imsf_slist[1];
};

#define IP_MSFILTER_SIZE(numsrc) (sizeof (struct ip_msfilter) \
				  - sizeof (struct in_addr)		      \
				  + (numsrc) * sizeof (struct in_addr))

struct group_filter
{
    /* Interface index.  */
    uint32_t gf_interface;

    /* Group address.  */
    struct sockaddr_storage gf_group;

    /* Filter mode.  */
    uint32_t gf_fmode;

    /* Number of source addresses.  */
    uint32_t gf_numsrc;
    /* Source addresses.  */
    struct sockaddr_storage gf_slist[1];
};

#define GROUP_FILTER_SIZE(numsrc) (sizeof (struct group_filter) \
				   - sizeof (struct sockaddr_storage)	      \
				   + ((numsrc)				      \
				      * sizeof (struct sockaddr_storage)))
#endif

/* Options for use with `getsockopt' and `setsockopt' at the IP level.  */
#define IP_OPTIONS		4
#define IP_HDRINCL		3
#define IP_TOS			1
#define IP_TTL			2
#define IP_RECVOPTS		6
#define IP_RECVRETOPTS		IP_RETOPTS
#define IP_RETOPTS		7
#define IP_MULTICAST_IF		32
#define IP_MULTICAST_TTL	33
#define IP_MULTICAST_LOOP	34
#define IP_ADD_MEMBERSHIP	35
#define IP_DROP_MEMBERSHIP	36
#define IP_UNBLOCK_SOURCE	37
#define IP_BLOCK_SOURCE		38
#define IP_ADD_SOURCE_MEMBERSHIP	39
#define IP_DROP_SOURCE_MEMBERSHIP	40
#define IP_MSFILTER		41
#define MCAST_JOIN_GROUP	42
#define MCAST_BLOCK_SOURCE	43
#define MCAST_UNBLOCK_SOURCE	44
#define MCAST_LEAVE_GROUP	45
#define MCAST_JOIN_SOURCE_GROUP	46
#define MCAST_LEAVE_SOURCE_GROUP	47
#define MCAST_MSFILTER		48
#define IP_MULTICAST_ALL	49
#define IP_UNICAST_IF		50

#define MCAST_EXCLUDE   0
#define MCAST_INCLUDE   1

#define IP_ROUTER_ALERT	5
#define IP_PKTINFO	8
#define IP_PKTOPTIONS	9
#define IP_PMTUDISC	10
#define IP_MTU_DISCOVER	10
#define IP_RECVERR	11
#define IP_RECVTTL	12
#define IP_RECVTOS	13
#define IP_MTU		14
#define IP_FREEBIND	15
#define IP_IPSEC_POLICY 16
#define IP_XFRM_POLICY	17
#define IP_PASSSEC	18
#define IP_TRANSPARENT	19
#define IP_ORIGDSTADDR       20
#define IP_RECVORIGDSTADDR   IP_ORIGDSTADDR
#define IP_MINTTL       21
#define IP_NODEFRAG     22
#define IP_CHECKSUM     23
#define IP_BIND_ADDRESS_NO_PORT 24
#define IP_RECVFRAGSIZE 25
#define IP_RECVERR_RFC4884 26

/* IP_MTU_DISCOVER arguments.  */
#define IP_PMTUDISC_DONT   0
#define IP_PMTUDISC_WANT   1
#define IP_PMTUDISC_DO     2
#define IP_PMTUDISC_PROBE  3
#define IP_PMTUDISC_INTERFACE           4
#define IP_PMTUDISC_OMIT		5

/* To select the IP level.  */
#ifndef SOL_IP
#define SOL_IP	0
#endif

#define IP_DEFAULT_MULTICAST_TTL        1
#define IP_DEFAULT_MULTICAST_LOOP       1
#define IP_MAX_MEMBERSHIPS              20

/* Structure used to describe IP options for IP_OPTIONS and IP_RETOPTS.  */
#if HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS
struct ip_opts
{
    struct in_addr ip_dst;	/* First hop; zero without source route.  */
    char ip_opts[40];		/* Actually variable in size.  */
};

/* Structure used for IP_PKTINFO.  */
struct in_pktinfo
{
    int ipi_ifindex;			/* Interface index  */
    struct in_addr ipi_spec_dst;	/* Routing destination address  */
    struct in_addr ipi_addr;		/* Header destination address  */
};
#endif

/* Options for use with `getsockopt' and `setsockopt' at the IPv6 level.  */
#define IPV6_ADDRFORM		1
#define IPV6_2292PKTINFO	2
#define IPV6_2292HOPOPTS	3
#define IPV6_2292DSTOPTS	4
#define IPV6_2292RTHDR		5
#define IPV6_2292PKTOPTIONS	6
#define IPV6_CHECKSUM		7
#define IPV6_2292HOPLIMIT	8

#define IPV6_NEXTHOP		9
#define IPV6_AUTHHDR		10
#define IPV6_UNICAST_HOPS	16
#define IPV6_MULTICAST_IF	17
#define IPV6_MULTICAST_HOPS	18
#define IPV6_MULTICAST_LOOP	19
#define IPV6_JOIN_GROUP		20
#define IPV6_LEAVE_GROUP	21
#define IPV6_ROUTER_ALERT	22
#define IPV6_MTU_DISCOVER	23
#define IPV6_MTU		24
#define IPV6_RECVERR		25
#define IPV6_V6ONLY		26
#define IPV6_JOIN_ANYCAST	27
#define IPV6_LEAVE_ANYCAST	28
#define IPV6_MULTICAST_ALL	29
#define IPV6_ROUTER_ALERT_ISOLATE 30
#define IPV6_RECVERR_RFC4884	31
#define IPV6_IPSEC_POLICY	34
#define IPV6_XFRM_POLICY	35
#define IPV6_HDRINCL		36

/* Advanced API (RFC3542) (1).  */
#define IPV6_RECVPKTINFO	49
#define IPV6_PKTINFO		50
#define IPV6_RECVHOPLIMIT	51
#define IPV6_HOPLIMIT		52
#define IPV6_RECVHOPOPTS	53
#define IPV6_HOPOPTS		54
#define IPV6_RTHDRDSTOPTS	55
#define IPV6_RECVRTHDR		56
#define IPV6_RTHDR		57
#define IPV6_RECVDSTOPTS	58
#define IPV6_DSTOPTS		59
#define IPV6_RECVPATHMTU	60
#define IPV6_PATHMTU		61
#define IPV6_DONTFRAG		62

/* Advanced API (RFC3542) (2).  */
#define IPV6_RECVTCLASS		66
#define IPV6_TCLASS		67

#define IPV6_AUTOFLOWLABEL	70

/* RFC5014.  */
#define IPV6_ADDR_PREFERENCES	72

/* RFC5082.  */
#define IPV6_MINHOPCOUNT	73

#define IPV6_ORIGDSTADDR	74
#define IPV6_RECVORIGDSTADDR	IPV6_ORIGDSTADDR
#define IPV6_TRANSPARENT	75
#define IPV6_UNICAST_IF		76
#define IPV6_RECVFRAGSIZE	77
#define IPV6_FREEBIND		78

/* Obsolete synonyms for the above.  */
#define IPV6_ADD_MEMBERSHIP	IPV6_JOIN_GROUP
#define IPV6_DROP_MEMBERSHIP	IPV6_LEAVE_GROUP
#define IPV6_RXHOPOPTS		IPV6_HOPOPTS
#define IPV6_RXDSTOPTS		IPV6_DSTOPTS

/* IPV6_MTU_DISCOVER values.  */
#define IPV6_PMTUDISC_DONT	0
#define IPV6_PMTUDISC_WANT	1
#define IPV6_PMTUDISC_DO	2
#define IPV6_PMTUDISC_PROBE	3
#define IPV6_PMTUDISC_INTERFACE	4
#define IPV6_PMTUDISC_OMIT	5

/* Socket level values for IPv6.  */
#ifndef SOL_IPV6
#define SOL_IPV6        41
#endif
#ifndef SOL_ICMPV6
#define SOL_ICMPV6      58
#endif

/* Routing header options for IPv6.  */
#define IPV6_RTHDR_LOOSE	0
#define IPV6_RTHDR_STRICT	1

#define IPV6_RTHDR_TYPE_0	0

/* 复用picolibc的<arpa/inet.h>提供的字节序转换宏。  */
#include "arpa/inet.h"

/* 若底层<arpa/inet.h>未提供字节序转换宏（例如底层为glibc），则在此补全。 */
#ifndef htonl
#define __HALIBC_POSIX_SOCKET_BSWAP16(_x) \
    ((uint16_t)((((uint16_t)(_x) & 0x00FFU) << 8) | (((uint16_t)(_x) & 0xFF00U) >> 8)))
#define __HALIBC_POSIX_SOCKET_BSWAP32(_x) \
    ((uint32_t)((((uint32_t)(_x) & 0x000000FFUL) << 24) \
		| (((uint32_t)(_x) & 0x0000FF00UL) << 8) \
		| (((uint32_t)(_x) & 0x00FF0000UL) >> 8) \
		| (((uint32_t)(_x) & 0xFF000000UL) >> 24)))
#if defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__) \
    && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
#define __HALIBC_POSIX_SOCKET_LITTLE_ENDIAN 1
#elif defined(__BYTE_ORDER__) && defined(__ORDER_BIG_ENDIAN__) \
    && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
#define __HALIBC_POSIX_SOCKET_LITTLE_ENDIAN 0
#elif defined(_BYTE_ORDER) && defined(_LITTLE_ENDIAN) \
    && (_BYTE_ORDER == _LITTLE_ENDIAN)
#define __HALIBC_POSIX_SOCKET_LITTLE_ENDIAN 1
#elif defined(_BYTE_ORDER) && defined(_BIG_ENDIAN) \
    && (_BYTE_ORDER == _BIG_ENDIAN)
#define __HALIBC_POSIX_SOCKET_LITTLE_ENDIAN 0
#elif defined(__LITTLE_ENDIAN__) || defined(__ARMEL__) || defined(__THUMBEL__)
#define __HALIBC_POSIX_SOCKET_LITTLE_ENDIAN 1
#elif defined(__BIG_ENDIAN__) || defined(__ARMEB__) || defined(__THUMBEB__)
#define __HALIBC_POSIX_SOCKET_LITTLE_ENDIAN 0
#elif defined(__BIG_ENDIAN)
/* armcc大端模式 */
#define __HALIBC_POSIX_SOCKET_LITTLE_ENDIAN 0
#elif defined(__CC_ARM) || defined(__ARMCC_VERSION)
/* armcc默认小端模式 */
#define __HALIBC_POSIX_SOCKET_LITTLE_ENDIAN 1
#else
/* 默认小端序，与hbox hdefaults保持一致 */
#define __HALIBC_POSIX_SOCKET_LITTLE_ENDIAN 1
#endif

#if __HALIBC_POSIX_SOCKET_LITTLE_ENDIAN
#define htonl(_x) __HALIBC_POSIX_SOCKET_BSWAP32(_x)
#define ntohl(_x) __HALIBC_POSIX_SOCKET_BSWAP32(_x)
#define htons(_x) __HALIBC_POSIX_SOCKET_BSWAP16(_x)
#define ntohs(_x) __HALIBC_POSIX_SOCKET_BSWAP16(_x)
#else
#define htonl(_x) ((uint32_t)(_x))
#define ntohl(_x) ((uint32_t)(_x))
#define htons(_x) ((uint16_t)(_x))
#define ntohs(_x) ((uint16_t)(_x))
#endif
#endif

#ifdef __GNUC__
#define IN6_IS_ADDR_UNSPECIFIED(a) \
    (__extension__							      \
     ({ const struct in6_addr *__a = (const struct in6_addr *) (a);	      \
	__a->__in6_u.__u6_addr32[0] == 0				      \
	&& __a->__in6_u.__u6_addr32[1] == 0				      \
	&& __a->__in6_u.__u6_addr32[2] == 0				      \
	&& __a->__in6_u.__u6_addr32[3] == 0; }))

#define IN6_IS_ADDR_LOOPBACK(a) \
    (__extension__							      \
     ({ const struct in6_addr *__a = (const struct in6_addr *) (a);	      \
	__a->__in6_u.__u6_addr32[0] == 0				      \
	&& __a->__in6_u.__u6_addr32[1] == 0				      \
	&& __a->__in6_u.__u6_addr32[2] == 0				      \
	&& __a->__in6_u.__u6_addr32[3] == htonl (1); }))

#define IN6_IS_ADDR_LINKLOCAL(a) \
    (__extension__							      \
     ({ const struct in6_addr *__a = (const struct in6_addr *) (a);	      \
	(__a->__in6_u.__u6_addr32[0] & htonl (0xffc00000)) == htonl (0xfe800000); }))

#define IN6_IS_ADDR_SITELOCAL(a) \
    (__extension__							      \
     ({ const struct in6_addr *__a = (const struct in6_addr *) (a);	      \
	(__a->__in6_u.__u6_addr32[0] & htonl (0xffc00000)) == htonl (0xfec00000); }))

#define IN6_IS_ADDR_V4MAPPED(a) \
    (__extension__							      \
     ({ const struct in6_addr *__a = (const struct in6_addr *) (a);	      \
	__a->__in6_u.__u6_addr32[0] == 0				      \
	&& __a->__in6_u.__u6_addr32[1] == 0				      \
	&& __a->__in6_u.__u6_addr32[2] == htonl (0xffff); }))

#define IN6_IS_ADDR_V4COMPAT(a) \
    (__extension__							      \
     ({ const struct in6_addr *__a = (const struct in6_addr *) (a);	      \
	__a->__in6_u.__u6_addr32[0] == 0				      \
	&& __a->__in6_u.__u6_addr32[1] == 0				      \
	&& __a->__in6_u.__u6_addr32[2] == 0				      \
	&& ntohl (__a->__in6_u.__u6_addr32[3]) > 1; }))

#define IN6_ARE_ADDR_EQUAL(a,b) \
    (__extension__							      \
     ({ const struct in6_addr *__a = (const struct in6_addr *) (a);	      \
	const struct in6_addr *__b = (const struct in6_addr *) (b);	      \
	__a->__in6_u.__u6_addr32[0] == __b->__in6_u.__u6_addr32[0]	      \
	&& __a->__in6_u.__u6_addr32[1] == __b->__in6_u.__u6_addr32[1]	      \
	&& __a->__in6_u.__u6_addr32[2] == __b->__in6_u.__u6_addr32[2]	      \
	&& __a->__in6_u.__u6_addr32[3] == __b->__in6_u.__u6_addr32[3]; }))
#else
#define IN6_IS_ADDR_UNSPECIFIED(a) \
	(((const uint32_t *) (a))[0] == 0				      \
	 && ((const uint32_t *) (a))[1] == 0				      \
	 && ((const uint32_t *) (a))[2] == 0				      \
	 && ((const uint32_t *) (a))[3] == 0)

#define IN6_IS_ADDR_LOOPBACK(a) \
	(((const uint32_t *) (a))[0] == 0				      \
	 && ((const uint32_t *) (a))[1] == 0				      \
	 && ((const uint32_t *) (a))[2] == 0				      \
	 && ((const uint32_t *) (a))[3] == htonl (1))

#define IN6_IS_ADDR_LINKLOCAL(a) \
	((((const uint32_t *) (a))[0] & htonl (0xffc00000))		      \
	 == htonl (0xfe800000))

#define IN6_IS_ADDR_SITELOCAL(a) \
	((((const uint32_t *) (a))[0] & htonl (0xffc00000))		      \
	 == htonl (0xfec00000))

#define IN6_IS_ADDR_V4MAPPED(a) \
	((((const uint32_t *) (a))[0] == 0)				      \
	 && (((const uint32_t *) (a))[1] == 0)				      \
	 && (((const uint32_t *) (a))[2] == htonl (0xffff)))

#define IN6_IS_ADDR_V4COMPAT(a) \
	((((const uint32_t *) (a))[0] == 0)				      \
	 && (((const uint32_t *) (a))[1] == 0)				      \
	 && (((const uint32_t *) (a))[2] == 0)				      \
	 && (ntohl (((const uint32_t *) (a))[3]) > 1))

#define IN6_ARE_ADDR_EQUAL(a,b) \
	((((const uint32_t *) (a))[0] == ((const uint32_t *) (b))[0])	      \
	 && (((const uint32_t *) (a))[1] == ((const uint32_t *) (b))[1])      \
	 && (((const uint32_t *) (a))[2] == ((const uint32_t *) (b))[2])      \
	 && (((const uint32_t *) (a))[3] == ((const uint32_t *) (b))[3]))
#endif

__END_DECLS

#endif /* __HALIBC_POSIX_SOCKET_NETINET_IN_H__ */
