/***************************************************************
 * Name:      hfiledescriptor_socketv1.h
 * Purpose:   声明hfiledescriptor_socketv1接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-02-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HSOCKETDESCRIPTOR_SOCKETV1_H__
#define __HSOCKETDESCRIPTOR_SOCKETV1_H__
#include "hfiledescriptor_common.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus


struct hfiledescriptor_socketv1_opt
{
    int                     (*socketv1_accept)(hfiledescriptor_fd_t fd, hfiledescriptor_sockaddr_t *addr, hfiledescriptor_socklen_t *addrlen);
    int                     (*socketv1_bind)(hfiledescriptor_fd_t fd, const hfiledescriptor_sockaddr_t *name, hfiledescriptor_socklen_t namelen);
    int                     (*socketv1_shutdown)(hfiledescriptor_fd_t fd, int how);
    int                     (*socketv1_getpeername)(hfiledescriptor_fd_t fd, hfiledescriptor_sockaddr_t *name, hfiledescriptor_socklen_t *namelen);
    int                     (*socketv1_getsockname)(hfiledescriptor_fd_t fd, hfiledescriptor_sockaddr_t *name, hfiledescriptor_socklen_t *namelen);
    int                     (*socketv1_getsockopt)(hfiledescriptor_fd_t fd, int level, int optname, void *optval, hfiledescriptor_socklen_t *optlen);
    int                     (*socketv1_setsockopt)(hfiledescriptor_fd_t fd, int level, int optname, const void *optval, hfiledescriptor_socklen_t optlen);
    int                     (*socketv1_connect)(hfiledescriptor_fd_t fd, const hfiledescriptor_sockaddr_t *name, hfiledescriptor_socklen_t namelen);
    int                     (*socketv1_listen)(hfiledescriptor_fd_t fd, int backlog);
    hfiledescriptor_ssize_t (*socketv1_recv)(hfiledescriptor_fd_t fd, void *mem, hfiledescriptor_size_t len, int flags);
    hfiledescriptor_ssize_t (*socketv1_readv)(hfiledescriptor_fd_t fd, const hfiledescriptor_iovec_t *iov, int iovcnt);
    hfiledescriptor_ssize_t (*socketv1_recvfrom)(hfiledescriptor_fd_t fd, void *mem, hfiledescriptor_size_t len, int flags,hfiledescriptor_sockaddr_t *from, hfiledescriptor_socklen_t *fromlen);
    hfiledescriptor_ssize_t (*socketv1_recvmsg)(hfiledescriptor_fd_t fd, hfiledescriptor_msghdr_t *message, int flags);
    hfiledescriptor_ssize_t (*socketv1_send)(hfiledescriptor_fd_t fd, const void *dataptr, hfiledescriptor_size_t len, int flags);
    hfiledescriptor_ssize_t (*socketv1_sendmsg)(hfiledescriptor_fd_t fd, const hfiledescriptor_msghdr_t *message, int flags);
    hfiledescriptor_ssize_t (*socketv1_sendto)(hfiledescriptor_fd_t fd, const void *dataptr, hfiledescriptor_size_t len, int flags,const hfiledescriptor_sockaddr_t *to, hfiledescriptor_socklen_t tolen);
    hfiledescriptor_ssize_t (*socketv1_writev)(hfiledescriptor_fd_t fd, const hfiledescriptor_iovec_t *iov, int iovcnt);
    hfiledescriptor_ssize_t (*socketv1_read)(hfiledescriptor_fd_t fd, void *mem, hfiledescriptor_size_t len);
    hfiledescriptor_ssize_t (*socketv1_write)(hfiledescriptor_fd_t fd, const void *dataptr, hfiledescriptor_size_t len);
    int                     (*socketv1_close)(hfiledescriptor_fd_t fd);
    int                     (*socketv1_ioctl)(hfiledescriptor_fd_t fd, long cmd, va_list va);
    int                     (*socketv1_fcntl)(hfiledescriptor_fd_t fd, int cmd, va_list va);
};
typedef struct hfiledescriptor_socketv1_opt hfiledescriptor_socketv1_opt_t;

struct hfiledescriptor_socketv1
{
    void *usr;
    hfiledescriptor_socketv1_opt_t *socketv1_opts;

};

typedef struct hfiledescriptor_socketv1 hfiledescriptor_socketv1_t;

hfiledescriptor_socketv1_t *hfiledescriptor_socketv1_get(hfiledescriptor_fd_t fd);


hfiledescriptor_fd_t hfiledescriptor_socketv1_alloc(hfiledescriptor_fd_t reuse_fd,const hfiledescriptor_socketv1_t *socketv1);

hfiledescriptor_fd_t    hfiledescriptor_socketv1_accept(hfiledescriptor_fd_t fd, hfiledescriptor_sockaddr_t *addr, hfiledescriptor_socklen_t *addrlen);
int                     hfiledescriptor_socketv1_bind(hfiledescriptor_fd_t fd, const hfiledescriptor_sockaddr_t *name, hfiledescriptor_socklen_t namelen);
int                     hfiledescriptor_socketv1_shutdown(hfiledescriptor_fd_t fd, int how);
int                     hfiledescriptor_socketv1_getpeername (hfiledescriptor_fd_t fd, hfiledescriptor_sockaddr_t *name, hfiledescriptor_socklen_t *namelen);
int                     hfiledescriptor_socketv1_getsockname (hfiledescriptor_fd_t fd, hfiledescriptor_sockaddr_t *name, hfiledescriptor_socklen_t *namelen);
int                     hfiledescriptor_socketv1_getsockopt (hfiledescriptor_fd_t fd, int level, int optname, void *optval, hfiledescriptor_socklen_t *optlen);
int                     hfiledescriptor_socketv1_setsockopt (hfiledescriptor_fd_t fd, int level, int optname, const void *optval, hfiledescriptor_socklen_t optlen);
int                     hfiledescriptor_socketv1_connect(hfiledescriptor_fd_t fd, const hfiledescriptor_sockaddr_t *name, hfiledescriptor_socklen_t namelen);
int                     hfiledescriptor_socketv1_listen(hfiledescriptor_fd_t fd, int backlog);
hfiledescriptor_ssize_t hfiledescriptor_socketv1_recv(hfiledescriptor_fd_t fd, void *mem, hfiledescriptor_size_t len, int flags);
hfiledescriptor_ssize_t hfiledescriptor_socketv1_readv(hfiledescriptor_fd_t fd, const hfiledescriptor_iovec_t *iov, int iovcnt);
hfiledescriptor_ssize_t hfiledescriptor_socketv1_recvfrom(hfiledescriptor_fd_t fd, void *mem, hfiledescriptor_size_t len, int flags,hfiledescriptor_sockaddr_t *from, hfiledescriptor_socklen_t *fromlen);
hfiledescriptor_ssize_t hfiledescriptor_socketv1_recvmsg(hfiledescriptor_fd_t fd, hfiledescriptor_msghdr_t *message, int flags);
hfiledescriptor_ssize_t hfiledescriptor_socketv1_send(hfiledescriptor_fd_t fd, const void *dataptr, hfiledescriptor_size_t len, int flags);
hfiledescriptor_ssize_t hfiledescriptor_socketv1_sendmsg(hfiledescriptor_fd_t fd, const hfiledescriptor_msghdr_t *message, int flags);
hfiledescriptor_ssize_t hfiledescriptor_socketv1_sendto(hfiledescriptor_fd_t fd, const void *dataptr, hfiledescriptor_size_t len, int flags,const hfiledescriptor_sockaddr_t *to, hfiledescriptor_socklen_t tolen);
hfiledescriptor_ssize_t hfiledescriptor_socketv1_writev(hfiledescriptor_fd_t fd, const hfiledescriptor_iovec_t *iov, int iovcnt);
int                     hfiledescriptor_socketv1_read(hfiledescriptor_fd_t fd, void *buff, hfiledescriptor_size_t buff_len);
int                     hfiledescriptor_socketv1_write(hfiledescriptor_fd_t fd,const void *buff,hfiledescriptor_size_t  buff_len);
int                     hfiledescriptor_socketv1_close(hfiledescriptor_fd_t fd);
int                     hfiledescriptor_socketv1_ioctl(hfiledescriptor_fd_t fd, unsigned long op, va_list va);
int                     hfiledescriptor_socketv1_fcntl(hfiledescriptor_fd_t fd, int op,va_list va);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HSOCKETDESCRIPTOR_SOCKETV1_H__
