/***************************************************************
 * Name:      inet.h
 * Purpose:   补充arpa/inet.h地址转换接口声明
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

/*
 * picolibc已经提供arpa/inet.h（字节序转换宏）。
 * 此处通过include_next复用picolibc的实现，不复制其内容，
 * 仅补充fallback中实现的地址转换接口声明。
 *
 * 注意：include_next是GCC/Clang扩展，armcc(ARM Compiler 5)不支持。
 * 因此仅在编译器提供__has_include_next且能找到下一级头文件时使用，
 * 其余情况由netinet/in.h提供字节序转换宏。
 */

#ifndef __HALIBC_POSIX_SOCKET_ARPA_INET_H__
#define __HALIBC_POSIX_SOCKET_ARPA_INET_H__

#ifndef HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS
#define HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS 0
#endif

#if defined(__has_include_next)
#if __has_include_next("arpa/inet.h")
#include_next "arpa/inet.h"
#endif
#endif

#include "netinet/in.h"

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

/* Convert Internet host address from numbers-and-dots notation in CP
   into binary data in network byte order.  */
extern in_addr_t inet_addr (const char *__cp);

#if HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS
/* Return the local host address part of the Internet address in IN.  */
extern in_addr_t inet_lnaof (struct in_addr __in);

/* Make Internet host address in network byte order by combining the
   network number NET with the local address HOST.  */
extern struct in_addr inet_makeaddr (in_addr_t __net, in_addr_t __host);

/* Return network number part of the Internet address in IN.  */
extern in_addr_t inet_netof (struct in_addr __in);

/* Extract the network number in network byte order from the address
   in numbers-and-dots notation starting at CP.  */
extern in_addr_t inet_network (const char *__cp);
#endif

/* Convert Internet number in IN to ASCII representation.  The return value
   is a pointer to an internal array containing the string.  */
extern char *inet_ntoa (struct in_addr __in);

/* Convert from presentation format of an Internet number in buffer
   starting at CP to the binary network format and store result for
   interface type AF in buffer starting at BUF.  */
extern int inet_pton (int __af, const char *__cp, void *__buf);

/* Convert a Internet address in binary network format for interface
   type AF in buffer starting at CP to presentation form and place
   result in buffer of length LEN starting at BUF.  */
extern const char *inet_ntop (int __af, const void *__cp,
			      char *__buf, socklen_t __len);

#if HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS
/* Convert Internet host address from numbers-and-dots notation in CP
   into binary data and store the result in the structure INP.  */
extern int inet_aton (const char *__cp, struct in_addr *__inp);
#endif

__END_DECLS

#endif /* __HALIBC_POSIX_SOCKET_ARPA_INET_H__ */
