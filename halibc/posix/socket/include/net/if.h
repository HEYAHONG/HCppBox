/***************************************************************
 * Name:      if.h
 * Purpose:   声明网络接口兼容接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT AND LGPL-2.1-or-later
 **************************************************************/
/* net/if.h -- declarations for inquiring about network interfaces
   Copyright (C) 1997-2026 Free Software Foundation, Inc.
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

#ifndef __HALIBC_POSIX_SOCKET_NET_IF_H__
#define __HALIBC_POSIX_SOCKET_NET_IF_H__

#include "sys/socket.h"

#ifndef HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS
#define HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS 0
#endif

#define IF_NAMESIZE 16

struct if_nameindex
{
    unsigned int if_index;
    char *if_name;
};

__BEGIN_DECLS

extern unsigned int if_nametoindex (const char *__ifname);
extern char *if_indextoname (unsigned int __ifindex,
			     char __ifname[IF_NAMESIZE]);
extern struct if_nameindex *if_nameindex (void);
extern void if_freenameindex (struct if_nameindex *__ptr);

__END_DECLS

#if HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS

#define IFNAMSIZ IF_NAMESIZE
#define IFHWADDRLEN 6

enum
{
    IFF_UP = 0x1,
#define IFF_UP IFF_UP
    IFF_BROADCAST = 0x2,
#define IFF_BROADCAST IFF_BROADCAST
    IFF_DEBUG = 0x4,
#define IFF_DEBUG IFF_DEBUG
    IFF_LOOPBACK = 0x8,
#define IFF_LOOPBACK IFF_LOOPBACK
    IFF_POINTOPOINT = 0x10,
#define IFF_POINTOPOINT IFF_POINTOPOINT
    IFF_NOTRAILERS = 0x20,
#define IFF_NOTRAILERS IFF_NOTRAILERS
    IFF_RUNNING = 0x40,
#define IFF_RUNNING IFF_RUNNING
    IFF_NOARP = 0x80,
#define IFF_NOARP IFF_NOARP
    IFF_PROMISC = 0x100,
#define IFF_PROMISC IFF_PROMISC
    IFF_ALLMULTI = 0x200,
#define IFF_ALLMULTI IFF_ALLMULTI
    IFF_MASTER = 0x400,
#define IFF_MASTER IFF_MASTER
    IFF_SLAVE = 0x800,
#define IFF_SLAVE IFF_SLAVE
    IFF_MULTICAST = 0x1000,
#define IFF_MULTICAST IFF_MULTICAST
    IFF_PORTSEL = 0x2000,
#define IFF_PORTSEL IFF_PORTSEL
    IFF_AUTOMEDIA = 0x4000,
#define IFF_AUTOMEDIA IFF_AUTOMEDIA
    IFF_DYNAMIC = 0x8000
#define IFF_DYNAMIC IFF_DYNAMIC
};

struct ifmap
{
    unsigned long int mem_start;
    unsigned long int mem_end;
    unsigned short int base_addr;
    unsigned char irq;
    unsigned char dma;
    unsigned char port;
};

struct ifreq
{
    union
    {
	char ifrn_name[IFNAMSIZ];
    } ifr_ifrn;

    union
    {
	struct sockaddr ifru_addr;
	struct sockaddr ifru_dstaddr;
	struct sockaddr ifru_broadaddr;
	struct sockaddr ifru_netmask;
	struct sockaddr ifru_hwaddr;
	short int ifru_flags;
	int ifru_ivalue;
	int ifru_mtu;
	struct ifmap ifru_map;
	char ifru_slave[IFNAMSIZ];
	char ifru_newname[IFNAMSIZ];
	char *ifru_data;
    } ifr_ifru;
};

#define ifr_name	ifr_ifrn.ifrn_name
#define ifr_hwaddr	ifr_ifru.ifru_hwaddr
#define ifr_addr	ifr_ifru.ifru_addr
#define ifr_dstaddr	ifr_ifru.ifru_dstaddr
#define ifr_broadaddr	ifr_ifru.ifru_broadaddr
#define ifr_netmask	ifr_ifru.ifru_netmask
#define ifr_flags	ifr_ifru.ifru_flags
#define ifr_metric	ifr_ifru.ifru_ivalue
#define ifr_mtu		ifr_ifru.ifru_mtu
#define ifr_map		ifr_ifru.ifru_map
#define ifr_slave	ifr_ifru.ifru_slave
#define ifr_data	ifr_ifru.ifru_data
#define ifr_ifindex	ifr_ifru.ifru_ivalue
#define ifr_newname	ifr_ifru.ifru_newname

struct ifconf
{
    int ifc_len;
    union
    {
	char *ifcu_buf;
	struct ifreq *ifcu_req;
    } ifc_ifcu;
};

#define ifc_buf	ifc_ifcu.ifcu_buf
#define ifc_req	ifc_ifcu.ifcu_req

#endif /* HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS */

#endif /* __HALIBC_POSIX_SOCKET_NET_IF_H__ */
