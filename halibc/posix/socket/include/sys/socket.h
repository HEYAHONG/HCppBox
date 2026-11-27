/***************************************************************
 * Name:      socket.h
 * Purpose:   声明socket兼容接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT AND LGPL-2.1-or-later
 **************************************************************/
/* Declarations of socket constants, types, and functions.
   Copyright (C) 1991-2026 Free Software Foundation, Inc.
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

#ifndef __HALIBC_POSIX_SOCKET_SYS_SOCKET_H__
#define __HALIBC_POSIX_SOCKET_SYS_SOCKET_H__

#ifndef HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS
#define HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS 0
#endif

#ifndef _SYS_SOCKET_H
#define _SYS_SOCKET_H 1
#endif

#include "stddef.h"
#include "stdint.h"
#include "sys/types.h"

#ifndef __BEGIN_DECLS
#ifdef __cplusplus
#define __BEGIN_DECLS extern "C" {
#define __END_DECLS }
#else
#define __BEGIN_DECLS
#define __END_DECLS
#endif
#endif

#define	__SOCKADDR_COMMON(sa_prefix) \
    sa_family_t sa_prefix##family

#define	__SOCKADDR_COMMON_SIZE	(sizeof(unsigned short int))

/* Size of struct sockaddr_storage.  */
#ifndef _SS_SIZE
#define _SS_SIZE 128
#endif

#ifndef _SA_FAMILY_T_DECLARED
typedef unsigned short int sa_family_t;
#define _SA_FAMILY_T_DECLARED
#endif

/* Type for length arguments in socket calls.  */
#ifndef _SOCKLEN_T_DECLARED
typedef __socklen_t socklen_t;
#define _SOCKLEN_T_DECLARED
#endif

/* Structure for scatter/gather I/O.  */
#ifndef __iovec_defined
#define __iovec_defined 1
struct iovec
{
    void *iov_base;	/* Pointer to data.  */
    size_t iov_len;	/* Length of data.  */
};
#endif

/* Protocol families.  */
#define PF_UNSPEC	0
#define PF_LOCAL	1
#define PF_UNIX		PF_LOCAL
#define PF_FILE		PF_LOCAL
#define PF_INET		2
#define PF_AX25		3
#define PF_IPX		4
#define PF_APPLETALK	5
#define PF_NETROM	6
#define PF_BRIDGE	7
#define PF_ATMPVC	8
#define PF_X25		9
#define PF_INET6	10
#define PF_ROSE		11
#define PF_DECnet	12
#define PF_NETBEUI	13
#define PF_SECURITY	14
#define PF_KEY		15
#define PF_NETLINK	16
#define PF_ROUTE	PF_NETLINK
#define PF_PACKET	17
#define PF_ASH		18
#define PF_ECONET	19
#define PF_ATMSVC	20
#define PF_RDS		21
#define PF_SNA		22
#define PF_IRDA		23
#define PF_PPPOX	24
#define PF_WANPIPE	25
#define PF_LLC		26
#define PF_IB		27
#define PF_MPLS		28
#define PF_CAN		29
#define PF_TIPC		30
#define PF_BLUETOOTH	31
#define PF_IUCV		32
#define PF_RXRPC	33
#define PF_ISDN		34
#define PF_PHONET	35
#define PF_IEEE802154	36
#define PF_CAIF		37
#define PF_ALG		38
#define PF_NFC		39
#define PF_VSOCK	40
#define PF_KCM		41
#define PF_QIPCRTR	42
#define PF_SMC		43
#define PF_XDP		44
#define PF_MCTP		45
#define PF_MAX		46

/* Address families.  */
#define AF_UNSPEC	PF_UNSPEC
#define AF_LOCAL	PF_LOCAL
#define AF_UNIX		PF_UNIX
#define AF_FILE		PF_FILE
#define AF_INET		PF_INET
#define AF_AX25		PF_AX25
#define AF_IPX		PF_IPX
#define AF_APPLETALK	PF_APPLETALK
#define AF_NETROM	PF_NETROM
#define AF_BRIDGE	PF_BRIDGE
#define AF_ATMPVC	PF_ATMPVC
#define AF_X25		PF_X25
#define AF_INET6	PF_INET6
#define AF_ROSE		PF_ROSE
#define AF_DECnet	PF_DECnet
#define AF_NETBEUI	PF_NETBEUI
#define AF_SECURITY	PF_SECURITY
#define AF_KEY		PF_KEY
#define AF_NETLINK	PF_NETLINK
#define AF_ROUTE	PF_ROUTE
#define AF_PACKET	PF_PACKET
#define AF_ASH		PF_ASH
#define AF_ECONET	PF_ECONET
#define AF_ATMSVC	PF_ATMSVC
#define AF_RDS		PF_RDS
#define AF_SNA		PF_SNA
#define AF_IRDA		PF_IRDA
#define AF_PPPOX	PF_PPPOX
#define AF_WANPIPE	PF_WANPIPE
#define AF_LLC		PF_LLC
#define AF_IB		PF_IB
#define AF_MPLS		PF_MPLS
#define AF_CAN		PF_CAN
#define AF_TIPC		PF_TIPC
#define AF_BLUETOOTH	PF_BLUETOOTH
#define AF_IUCV		PF_IUCV
#define AF_RXRPC	PF_RXRPC
#define AF_ISDN		PF_ISDN
#define AF_PHONET	PF_PHONET
#define AF_IEEE802154	PF_IEEE802154
#define AF_CAIF		PF_CAIF
#define AF_ALG		PF_ALG
#define AF_NFC		PF_NFC
#define AF_VSOCK	PF_VSOCK
#define AF_KCM		PF_KCM
#define AF_QIPCRTR	PF_QIPCRTR
#define AF_SMC		PF_SMC
#define AF_XDP		PF_XDP
#define AF_MCTP		PF_MCTP
#define AF_MAX		PF_MAX

/* Socket level values.  */
#define SOL_RAW		255
#define SOL_DECNET	261
#define SOL_X25		262
#define SOL_PACKET	263
#define SOL_ATM		264
#define SOL_AAL		265
#define SOL_IRDA	266
#define SOL_NETBEUI	267
#define SOL_LLC		268
#define SOL_DCCP	269
#define SOL_NETLINK	270
#define SOL_TIPC	271
#define SOL_RXRPC	272
#define SOL_PPPOL2TP	273
#define SOL_BLUETOOTH	274
#define SOL_PNPIPE	275
#define SOL_RDS		276
#define SOL_IUCV	277
#define SOL_CAIF	278
#define SOL_ALG		279
#define SOL_NFC		280
#define SOL_KCM		281
#define SOL_TLS		282
#define SOL_XDP		283
#define SOL_MPTCP	284
#define SOL_MCTP	285
#define SOL_SMC		286
#define SOL_VSOCK	287

/* Maximum queue length specifiable by listen.  */
#ifndef SOMAXCONN
#define SOMAXCONN	4096
#endif

/* Types of sockets.  */
enum __socket_type
{
    SOCK_STREAM = 1,
#define SOCK_STREAM SOCK_STREAM
    SOCK_DGRAM = 2,
#define SOCK_DGRAM SOCK_DGRAM
    SOCK_RAW = 3,
#define SOCK_RAW SOCK_RAW
    SOCK_RDM = 4,
#define SOCK_RDM SOCK_RDM
    SOCK_SEQPACKET = 5,
#define SOCK_SEQPACKET SOCK_SEQPACKET
    SOCK_DCCP = 6,
#define SOCK_DCCP SOCK_DCCP
    SOCK_PACKET = 10,
#define SOCK_PACKET SOCK_PACKET

    SOCK_CLOEXEC = 02000000,
#define SOCK_CLOEXEC SOCK_CLOEXEC
    SOCK_NONBLOCK = 00004000
#define SOCK_NONBLOCK SOCK_NONBLOCK
};

/* Structure describing a generic socket address.  */
struct sockaddr
{
    __SOCKADDR_COMMON (sa_);	/* Common data: address family and length.  */
    char sa_data[14];		/* Address data.  */
};

/* Structure large enough to hold any socket address (with the historical
   exception of AF_UNIX).  */
#define __ss_aligntype	unsigned long int
#define _SS_PADSIZE \
    (_SS_SIZE - __SOCKADDR_COMMON_SIZE - sizeof (__ss_aligntype))

struct sockaddr_storage
{
    __SOCKADDR_COMMON (ss_);	/* Address family, etc.  */
    char __ss_padding[_SS_PADSIZE];
    __ss_aligntype __ss_align;	/* Force desired alignment.  */
};

/* Bits in the FLAGS argument to `send', `recv', et al.  */
enum
{
    MSG_OOB		= 0x01,
#define MSG_OOB		MSG_OOB
    MSG_PEEK		= 0x02,
#define MSG_PEEK	MSG_PEEK
    MSG_DONTROUTE	= 0x04,
#define MSG_DONTROUTE	MSG_DONTROUTE
    MSG_TRYHARD		= MSG_DONTROUTE,
#define MSG_TRYHARD	MSG_DONTROUTE
    MSG_CTRUNC		= 0x08,
#define MSG_CTRUNC	MSG_CTRUNC
    MSG_PROXY		= 0x10,
#define MSG_PROXY	MSG_PROXY
    MSG_TRUNC		= 0x20,
#define MSG_TRUNC	MSG_TRUNC
    MSG_DONTWAIT	= 0x40,
#define MSG_DONTWAIT	MSG_DONTWAIT
    MSG_EOR		= 0x80,
#define MSG_EOR		MSG_EOR
    MSG_WAITALL		= 0x100,
#define MSG_WAITALL	MSG_WAITALL
    MSG_FIN		= 0x200,
#define MSG_FIN		MSG_FIN
    MSG_SYN		= 0x400,
#define MSG_SYN		MSG_SYN
    MSG_CONFIRM		= 0x800,
#define MSG_CONFIRM	MSG_CONFIRM
    MSG_RST		= 0x1000,
#define MSG_RST		MSG_RST
    MSG_ERRQUEUE	= 0x2000,
#define MSG_ERRQUEUE	MSG_ERRQUEUE
    MSG_NOSIGNAL	= 0x4000,
#define MSG_NOSIGNAL	MSG_NOSIGNAL
    MSG_MORE		= 0x8000,
#define MSG_MORE	MSG_MORE
    MSG_WAITFORONE	= 0x10000,
#define MSG_WAITFORONE	MSG_WAITFORONE
    MSG_BATCH		= 0x40000,
#define MSG_BATCH	MSG_BATCH
    MSG_SOCK_DEVMEM	= 0x2000000,
#define MSG_SOCK_DEVMEM	MSG_SOCK_DEVMEM
    MSG_ZEROCOPY	= 0x4000000,
#define MSG_ZEROCOPY	MSG_ZEROCOPY
    MSG_FASTOPEN	= 0x20000000,
#define MSG_FASTOPEN	MSG_FASTOPEN
    MSG_CMSG_CLOEXEC	= 0x40000000
#define MSG_CMSG_CLOEXEC MSG_CMSG_CLOEXEC
};

/* Structure describing messages sent by
   `sendmsg' and received by `recvmsg'.  */
struct msghdr
{
    void *msg_name;		/* Address to send to/receive from.  */
    socklen_t msg_namelen;	/* Length of address data.  */

    struct iovec *msg_iov;	/* Vector of data to send/receive into.  */
    size_t msg_iovlen;		/* Number of elements in the vector.  */

    void *msg_control;		/* Ancillary data (eg BSD filedesc passing). */
    size_t msg_controllen;	/* Ancillary data buffer length.  */

    int msg_flags;		/* Flags on received message.  */
};

/* Structure used for storage of ancillary data object information.  */
struct cmsghdr
{
    size_t cmsg_len;		/* Length of data in cmsg_data plus length
				   of cmsghdr structure.  */
    int cmsg_level;		/* Originating protocol.  */
    int cmsg_type;		/* Protocol specific type.  */
};

/* Ancillary data object manipulation macros.  */
#define CMSG_DATA(cmsg) ((unsigned char *) ((struct cmsghdr *) (cmsg) + 1))
#define CMSG_ALIGN(len) (((len) + sizeof (size_t) - 1) \
			 & (size_t) ~(sizeof (size_t) - 1))
#define CMSG_SPACE(len) (CMSG_ALIGN (len) \
			 + CMSG_ALIGN (sizeof (struct cmsghdr)))
#define CMSG_LEN(len)   (CMSG_ALIGN (sizeof (struct cmsghdr)) + (len))
#define CMSG_FIRSTHDR(mhdr) \
    ((size_t) (mhdr)->msg_controllen >= sizeof (struct cmsghdr)		      \
     ? (struct cmsghdr *) (mhdr)->msg_control : (struct cmsghdr *) 0)

#define __CMSG_PADDING(len) ((sizeof (size_t) \
			      - ((len) & (sizeof (size_t) - 1))) \
			     & (sizeof (size_t) - 1))

static __inline__ struct cmsghdr *
__halibc_posix_socket_cmsg_nxthdr (struct msghdr *__mhdr,
				   struct cmsghdr *__cmsg)
{
    unsigned char *__msg_control_ptr = (unsigned char *) __mhdr->msg_control;
    unsigned char *__cmsg_ptr = (unsigned char *) __cmsg;
    size_t __size_needed = sizeof (struct cmsghdr)
			   + __CMSG_PADDING (__cmsg->cmsg_len);

    if ((size_t) __cmsg->cmsg_len < sizeof (struct cmsghdr))
	return (struct cmsghdr *) 0;

    if (((size_t)
	 (__msg_control_ptr + __mhdr->msg_controllen - __cmsg_ptr)
	 < __size_needed)
	|| ((size_t)
	    (__msg_control_ptr + __mhdr->msg_controllen - __cmsg_ptr
	     - __size_needed)
	    < __cmsg->cmsg_len))
	return (struct cmsghdr *) 0;

    return (struct cmsghdr *) ((unsigned char *) __cmsg
			       + CMSG_ALIGN (__cmsg->cmsg_len));
}

#define CMSG_NXTHDR(mhdr, cmsg) \
    __halibc_posix_socket_cmsg_nxthdr ((mhdr), (cmsg))

/* Socket level message types.  This must match the definitions in
   <linux/socket.h>.  */
enum
{
    SCM_RIGHTS = 0x01,
#define SCM_RIGHTS SCM_RIGHTS
#if HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS
    SCM_CREDENTIALS = 0x02,
#define SCM_CREDENTIALS SCM_CREDENTIALS
    SCM_SECURITY = 0x03,
#define SCM_SECURITY SCM_SECURITY
    SCM_PIDFD = 0x04
#define SCM_PIDFD SCM_PIDFD
#endif
};

#if HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS
/* User visible structure for SCM_CREDENTIALS message */
struct ucred
{
    pid_t pid;			/* PID of sending process.  */
    uid_t uid;			/* UID of sending process.  */
    gid_t gid;			/* GID of sending process.  */
};
#endif

/* Socket constants which vary among Linux architectures.  */
#ifndef SOL_SOCKET
#define SOL_SOCKET 1
#endif
#ifndef SO_ACCEPTCONN
#define SO_ACCEPTCONN 30
#endif
#ifndef SO_BROADCAST
#define SO_BROADCAST 6
#endif
#ifndef SO_DONTROUTE
#define SO_DONTROUTE 5
#endif
#ifndef SO_ERROR
#define SO_ERROR 4
#endif
#ifndef SO_KEEPALIVE
#define SO_KEEPALIVE 9
#endif
#ifndef SO_LINGER
#define SO_LINGER 13
#endif
#ifndef SO_OOBINLINE
#define SO_OOBINLINE 10
#endif
#ifndef SO_RCVBUF
#define SO_RCVBUF 8
#endif
#ifndef SO_RCVLOWAT
#define SO_RCVLOWAT 18
#endif
#ifndef SO_REUSEADDR
#define SO_REUSEADDR 2
#endif
#ifndef SO_SNDBUF
#define SO_SNDBUF 7
#endif
#ifndef SO_SNDLOWAT
#define SO_SNDLOWAT 19
#endif
#ifndef SO_TYPE
#define SO_TYPE 3
#endif
#ifndef SO_RCVTIMEO
#define SO_RCVTIMEO 20
#endif
#ifndef SO_SNDTIMEO
#define SO_SNDTIMEO 21
#endif
#ifndef SO_TIMESTAMP
#define SO_TIMESTAMP 29
#endif
#ifndef SO_TIMESTAMPNS
#define SO_TIMESTAMPNS 35
#endif
#ifndef SO_TIMESTAMPING
#define SO_TIMESTAMPING 37
#endif

struct timespec;

/* Structure used to manipulate the SO_LINGER option.  */
struct linger
{
    int l_onoff;		/* Nonzero to linger on close.  */
    int l_linger;		/* Time to linger.  */
};

/* For `recvmmsg' and `sendmmsg'.  */
#if HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS
struct mmsghdr
{
    struct msghdr msg_hdr;	/* Actual message header.  */
    unsigned int msg_len;	/* Number of received or sent bytes for the
				   entry.  */
};
#endif

/* The following constants should be used for the second parameter of
   `shutdown'.  */
enum
{
    SHUT_RD = 0,
#define SHUT_RD		SHUT_RD
    SHUT_WR,
#define SHUT_WR		SHUT_WR
    SHUT_RDWR
#define SHUT_RDWR	SHUT_RDWR
};

__BEGIN_DECLS

/* Create a new socket of type TYPE in domain DOMAIN, using
   protocol PROTOCOL.  If PROTOCOL is zero, one is chosen automatically.
   Returns a file descriptor for the new socket, or -1 for errors.  */
extern int socket (int __domain, int __type, int __protocol);

/* Create two new sockets, of type TYPE in domain DOMAIN and using
   protocol PROTOCOL, which are connected to each other, and put file
   descriptors for them in FDS[0] and FDS[1].  If PROTOCOL is zero,
   one will be chosen automatically.  Returns 0 on success, -1 for errors.  */
extern int socketpair (int __domain, int __type, int __protocol,
		       int __fds[2]);

/* Give the socket FD the local address ADDR (which is LEN bytes long).  */
extern int bind (int __fd, const struct sockaddr *__addr, socklen_t __len);

/* Put the local address of FD into *ADDR and its length in *LEN.  */
extern int getsockname (int __fd, struct sockaddr *__addr,
			socklen_t *__len);

/* Open a connection on socket FD to peer at ADDR (which LEN bytes long).  */
extern int connect (int __fd, const struct sockaddr *__addr, socklen_t __len);

/* Put the address of the peer connected to socket FD into *ADDR
   (which is *LEN bytes long), and its actual length into *LEN.  */
extern int getpeername (int __fd, struct sockaddr *__addr,
			socklen_t *__len);

/* Send N bytes of BUF to socket FD.  Returns the number sent or -1.  */
extern ssize_t send (int __fd, const void *__buf, size_t __n, int __flags);

/* Read N bytes into BUF from socket FD.
   Returns the number read or -1 for errors.  */
extern ssize_t recv (int __fd, void *__buf, size_t __n, int __flags);

/* Send N bytes of BUF on socket FD to peer at address ADDR (which is
   ADDR_LEN bytes long).  Returns the number sent, or -1 for errors.  */
extern ssize_t sendto (int __fd, const void *__buf, size_t __n,
		       int __flags, const struct sockaddr *__addr,
		       socklen_t __addr_len);

/* Read N bytes into BUF through socket FD.  */
extern ssize_t recvfrom (int __fd, void *__buf, size_t __n,
			 int __flags, struct sockaddr *__addr,
			 socklen_t *__addr_len);

/* Send a message described MESSAGE on socket FD.
   Returns the number of bytes sent, or -1 for errors.  */
extern ssize_t sendmsg (int __fd, const struct msghdr *__message,
			int __flags);

/* Receive a message as described by MESSAGE from socket FD.
   Returns the number of bytes read or -1 for errors.  */
extern ssize_t recvmsg (int __fd, struct msghdr *__message, int __flags);

#if HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS
/* Send a VLEN messages as described by VMESSAGES to socket FD.  */
extern int sendmmsg (int __fd, struct mmsghdr *__vmessages,
		     unsigned int __vlen, int __flags);

/* Receive up to VLEN messages as described by VMESSAGES from socket FD.  */
extern int recvmmsg (int __fd, struct mmsghdr *__vmessages,
		     unsigned int __vlen, int __flags,
		     struct timespec *__tmo);
#endif

/* Put the current value for socket FD's option OPTNAME at protocol level LEVEL
   into OPTVAL (which is *OPTLEN bytes long), and set *OPTLEN to the value's
   actual length.  Returns 0 on success, -1 for errors.  */
extern int getsockopt (int __fd, int __level, int __optname,
		       void *__optval, socklen_t *__optlen);

/* Set socket FD's option OPTNAME at protocol level LEVEL
   to *OPTVAL (which is OPTLEN bytes long).
   Returns 0 on success, -1 for errors.  */
extern int setsockopt (int __fd, int __level, int __optname,
		       const void *__optval, socklen_t __optlen);

/* Prepare to accept connections on socket FD.  */
extern int listen (int __fd, int __n);

/* Await a connection on socket FD.  */
extern int accept (int __fd, struct sockaddr *__addr,
		   socklen_t *__addr_len);

#if HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS
/* Similar to 'accept' but takes an additional parameter to specify flags.  */
extern int accept4 (int __fd, struct sockaddr *__addr,
		    socklen_t *__addr_len, int __flags);
#endif

/* Shut down all or part of the connection open on socket FD.  */
extern int shutdown (int __fd, int __how);

/* Determine whether socket is at a out-of-band mark.  */
extern int sockatmark (int __fd);

#if HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS
/* FDTYPE is S_IFSOCK or another S_IF* macro defined in <sys/stat.h>;
   returns 1 if FD is open on an object of the indicated type, 0 if not,
   or -1 for errors (setting errno).  */
extern int isfdtype (int __fd, int __fdtype);
#endif

__END_DECLS

#endif /* __HALIBC_POSIX_SOCKET_SYS_SOCKET_H__ */
