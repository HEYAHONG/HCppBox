/***************************************************************
 * Name:      netdb.h
 * Purpose:   声明网络数据库兼容接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT AND LGPL-2.1-or-later
 **************************************************************/
/* Copyright (C) 1996-2026 Free Software Foundation, Inc.
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

/* All data returned by the network data base library are supplied in
   host order and returned in network order (suitable for use in
   system calls).  */

#ifndef __HALIBC_POSIX_SOCKET_NETDB_H__
#define __HALIBC_POSIX_SOCKET_NETDB_H__

#ifndef HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS
#define HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS 0
#endif

#ifndef _NETDB_H
#define _NETDB_H 1
#endif

#include "stddef.h"
#include "stdint.h"
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

/* Absolute file name for network data base files.  */
#if HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS
#define	_PATH_HEQUIV		"/etc/hosts.equiv"
#define	_PATH_HOSTS		"/etc/hosts"
#define	_PATH_NETWORKS		"/etc/networks"
#define	_PATH_NSSWITCH_CONF	"/etc/nsswitch.conf"
#define	_PATH_PROTOCOLS		"/etc/protocols"
#define	_PATH_SERVICES		"/etc/services"
#endif

__BEGIN_DECLS

/*
 * h_errno、herror与hstrerror为BSD/glibc扩展，并非POSIX标准接口。
 */
#if HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS

/* Error status for non-reentrant lookup functions.
   We use a macro to access always the thread-specific `h_errno' variable.  */
#ifndef h_errno
#define h_errno (*__h_errno_location ())
#endif

/* Function to get address of global `h_errno' variable.  */
extern int *__h_errno_location (void);

/* Possible values left in `h_errno'.  */
#define HOST_NOT_FOUND	1	/* Authoritative Answer Host not found.  */
#define TRY_AGAIN	2	/* Non-Authoritative Host not found,
				   or SERVERFAIL.  */
#define NO_RECOVERY	3	/* Non recoverable errors, FORMERR, REFUSED,
				   NOTIMP.  */
#define NO_DATA	4	/* Valid name, no data record of requested
				   type.  */

#define NETDB_INTERNAL	-1	/* See errno.  */
#define NETDB_SUCCESS	0	/* No problem.  */
#define NO_ADDRESS	NO_DATA	/* No address, look for MX record.  */

/* Print error indicated by `h_errno' variable on standard error.  STR
   if non-null is printed before the error string.  */
extern void herror (const char *__str);

/* Return string associated with error ERR_NUM.  */
extern const char *hstrerror (int __err_num);

#endif /* HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS */

#ifndef IPPORT_RESERVED
#define IPPORT_RESERVED	1024
#endif

#if HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS
/* Scope delimiter for getaddrinfo(), getnameinfo().  */
#define SCOPE_DELIMITER	'%'
#endif

/* Description of data base entry for a single host.  */
struct hostent
{
    char *h_name;			/* Official name of host.  */
    char **h_aliases;		/* Alias list.  */
    int h_addrtype;		/* Host address type.  */
    int h_length;			/* Length of address.  */
    char **h_addr_list;		/* List of addresses from name server.  */
#define	h_addr	h_addr_list[0] /* Address, for backward compatibility.*/
};

/* Open host data base files and mark them as staying open even after
   a later search if STAY_OPEN is non-zero.  */
extern void sethostent (int __stay_open);

/* Close host data base files and clear `stay open' flag.  */
extern void endhostent (void);

/* Get next entry from host data base file.  Open data base if necessary.  */
extern struct hostent *gethostent (void);

/* Return entry from host data base which address match ADDR with
   length LEN and type TYPE.  */
extern struct hostent *gethostbyaddr (const void *__addr, socklen_t __len,
				      int __type);

/* Return entry from host data base for host with NAME.  */
extern struct hostent *gethostbyname (const char *__name);

#if HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS
/* Return entry from host data base for host with NAME and AF.  */
extern struct hostent *gethostbyname2 (const char *__name, int __af);

/* Reentrant versions of the functions above.  */
extern int gethostent_r (struct hostent *__result_buf,
			 char *__buf, size_t __buflen,
			 struct hostent **__result,
			 int *__h_errnop);

extern int gethostbyaddr_r (const void *__addr, socklen_t __len,
			    int __type,
			    struct hostent *__result_buf,
			    char *__buf, size_t __buflen,
			    struct hostent **__result,
			    int *__h_errnop);

extern int gethostbyname_r (const char *__name,
			    struct hostent *__result_buf,
			    char *__buf, size_t __buflen,
			    struct hostent **__result,
			    int *__h_errnop);

extern int gethostbyname2_r (const char *__name, int __af,
			     struct hostent *__result_buf,
			     char *__buf, size_t __buflen,
			     struct hostent **__result,
			     int *__h_errnop);
#endif

#if HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS
/* Description of data base entry for a single network.  */
struct netent
{
    char *n_name;			/* Official name of network.  */
    char **n_aliases;		/* Alias list.  */
    int n_addrtype;		/* Net address type.  */
    uint32_t n_net;		/* Network number.  */
};

extern void setnetent (int __stay_open);
extern void endnetent (void);
extern struct netent *getnetent (void);
extern struct netent *getnetbyaddr (uint32_t __net, int __type);
extern struct netent *getnetbyname (const char *__name);

extern int getnetent_r (struct netent *__result_buf,
			char *__buf, size_t __buflen,
			struct netent **__result,
			int *__h_errnop);

extern int getnetbyaddr_r (uint32_t __net, int __type,
			   struct netent *__result_buf,
			   char *__buf, size_t __buflen,
			   struct netent **__result,
			   int *__h_errnop);

extern int getnetbyname_r (const char *__name,
			   struct netent *__result_buf,
			   char *__buf, size_t __buflen,
			   struct netent **__result,
			   int *__h_errnop);

/* Description of data base entry for a single service.  */
struct servent
{
    char *s_name;			/* Official service name.  */
    char **s_aliases;		/* Alias list.  */
    int s_port;			/* Port number.  */
    char *s_proto;		/* Protocol to use.  */
};

extern void setservent (int __stay_open);
extern void endservent (void);
extern struct servent *getservent (void);
extern struct servent *getservbyname (const char *__name, const char *__proto);
extern struct servent *getservbyport (int __port, const char *__proto);

extern int getservent_r (struct servent *__result_buf,
			 char *__buf, size_t __buflen,
			 struct servent **__result);

extern int getservbyname_r (const char *__name,
			    const char *__proto,
			    struct servent *__result_buf,
			    char *__buf, size_t __buflen,
			    struct servent **__result);

extern int getservbyport_r (int __port, const char *__proto,
			    struct servent *__result_buf,
			    char *__buf, size_t __buflen,
			    struct servent **__result);

/* Description of data base entry for a single protocol.  */
struct protoent
{
    char *p_name;			/* Official protocol name.  */
    char **p_aliases;		/* Alias list.  */
    int p_proto;			/* Protocol number.  */
};

extern void setprotoent (int __stay_open);
extern void endprotoent (void);
extern struct protoent *getprotoent (void);
extern struct protoent *getprotobyname (const char *__name);
extern struct protoent *getprotobynumber (int __proto);

extern int getprotoent_r (struct protoent *__result_buf,
			  char *__buf, size_t __buflen,
			  struct protoent **__result);

extern int getprotobyname_r (const char *__name,
			     struct protoent *__result_buf,
			     char *__buf, size_t __buflen,
			     struct protoent **__result);

extern int getprotobynumber_r (int __proto,
			       struct protoent *__result_buf,
			       char *__buf, size_t __buflen,
			       struct protoent **__result);
#endif

/* Structure to contain information about address of a service provider.  */
struct addrinfo
{
    int ai_flags;			/* Input flags.  */
    int ai_family;		/* Protocol family for socket.  */
    int ai_socktype;		/* Socket type.  */
    int ai_protocol;		/* Protocol for socket.  */
    socklen_t ai_addrlen;		/* Length of socket address.  */
    struct sockaddr *ai_addr;	/* Socket address for socket.  */
    char *ai_canonname;		/* Canonical name for service location.  */
    struct addrinfo *ai_next;	/* Pointer to next in list.  */
};

/* Possible values for `ai_flags' field in `addrinfo' structure.  */
#define AI_PASSIVE	0x0001	/* Socket address is intended for `bind'.  */
#define AI_CANONNAME	0x0002	/* Request for canonical name.  */
#define AI_NUMERICHOST	0x0004	/* Don't use name resolution.  */
#define AI_V4MAPPED	0x0008	/* IPv4 mapped addresses are acceptable.  */
#define AI_ALL		0x0010	/* Return IPv4 mapped and IPv6 addresses.  */
#define AI_ADDRCONFIG	0x0020	/* Use configuration of this host to choose
				   returned address type..  */
#define AI_NUMERICSERV	0x0400	/* Don't use name resolution.  */

/* Error values for `getaddrinfo' function.  */
#define EAI_BADFLAGS	  -1	/* Invalid value for `ai_flags' field.  */
#define EAI_NONAME	  -2	/* NAME or SERVICE is unknown.  */
#define EAI_AGAIN	  -3	/* Temporary failure in name resolution.  */
#define EAI_FAIL	  -4	/* Non-recoverable failure in name res.  */
#define EAI_FAMILY	  -6	/* `ai_family' not supported.  */
#define EAI_SOCKTYPE	  -7	/* `ai_socktype' not supported.  */
#define EAI_SERVICE	  -8	/* SERVICE not supported for `ai_socktype'.  */
#define EAI_MEMORY	  -10	/* Memory allocation failure.  */
#define EAI_SYSTEM	  -11	/* System error returned in `errno'.  */
#define EAI_OVERFLOW	  -12	/* Argument buffer overflow.  */
#if HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS
#define EAI_NODATA	  -5	/* No address associated with NAME.  */
#define EAI_ADDRFAMILY  -9	/* Address family for NAME not supported.  */
#endif

#define NI_MAXHOST      1025
#define NI_MAXSERV      32

#define NI_NUMERICHOST	1	/* Don't try to look up hostname.  */
#define NI_NUMERICSERV 2	/* Don't convert port number to name.  */
#define NI_NOFQDN	4	/* Only return nodename portion.  */
#define NI_NAMEREQD	8	/* Don't return numeric addresses.  */
#define NI_DGRAM	16	/* Look up UDP service rather than TCP.  */

/* Translate name of a service location and/or a service name to set of
   socket addresses.  */
extern int getaddrinfo (const char *__name,
			const char *__service,
			const struct addrinfo *__req,
			struct addrinfo **__pai);

/* Free `addrinfo' structure AI including associated storage.  */
extern void freeaddrinfo (struct addrinfo *__ai);

/* Convert error return from getaddrinfo() to a string.  */
extern const char *gai_strerror (int __ecode);

/* Translate a socket address to a location and service name.  */
extern int getnameinfo (const struct sockaddr *__sa,
			socklen_t __salen, char *__host,
			socklen_t __hostlen, char *__serv,
			socklen_t __servlen, int __flags);

__END_DECLS

#endif /* __HALIBC_POSIX_SOCKET_NETDB_H__ */
