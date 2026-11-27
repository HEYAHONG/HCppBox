/***************************************************************
 * Name:      un.h
 * Purpose:   声明Unix域socket兼容接口
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

#ifndef __HALIBC_POSIX_SOCKET_SYS_UN_H__
#define __HALIBC_POSIX_SOCKET_SYS_UN_H__

#ifndef HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS
#define HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS 0
#endif

#ifndef _SYS_UN_H
#define _SYS_UN_H 1
#endif

#include "stddef.h"
#include "string.h"
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

/* Structure describing the address of an AF_LOCAL (aka AF_UNIX) socket.  */
struct sockaddr_un
{
    __SOCKADDR_COMMON (sun_);
    char sun_path[108];		/* Path name.  */
};

/* Evaluate to actual length of the `sockaddr_un' structure.  */
#if HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS
#ifndef SUN_LEN
#define SUN_LEN(ptr) (offsetof (struct sockaddr_un, sun_path)		      \
		      + strlen ((ptr)->sun_path))
#endif
#endif

__END_DECLS

#endif /* __HALIBC_POSIX_SOCKET_SYS_UN_H__ */
