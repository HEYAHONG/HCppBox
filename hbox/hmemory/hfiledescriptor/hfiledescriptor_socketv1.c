/***************************************************************
 * Name:      hfiledescriptor_socketv1.c
 * Purpose:   实现hfiledescriptor_socketv1接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-02-17
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hfiledescriptor_socketv1.h"

hfiledescriptor_socketv1_t *hfiledescriptor_socketv1_get(hfiledescriptor_fd_t fd)
{
    hfiledescriptor_common_table_t *obj=hfiledescriptor_common_table_get(fd);
    if(sizeof(hfiledescriptor_common_table_t) < sizeof(hfiledescriptor_socketv1_t))
    {
        return NULL;
    }

    if(obj!=NULL && hfiledescriptor_common_table_type(fd)==HFILEDESCRIPTOR_TYPE_SOCKETV1)
    {
        return (hfiledescriptor_socketv1_t *)obj;
    }

    return NULL;
}

hfiledescriptor_fd_t hfiledescriptor_socketv1_alloc(hfiledescriptor_fd_t reuse_fd,const hfiledescriptor_socketv1_t *socketv1)
{
    if(sizeof(hfiledescriptor_common_table_t) < sizeof(hfiledescriptor_socketv1_t) || socketv1==NULL)
    {
        return -1;
    }
    hfiledescriptor_fd_t ret=hfiledescriptor_common_table_alloc(reuse_fd,HFILEDESCRIPTOR_TYPE_SOCKETV1);
    if(ret >= 0)
    {
        (*hfiledescriptor_socketv1_get(ret))=(*socketv1);
    }
    return ret;
}

hfiledescriptor_fd_t hfiledescriptor_socketv1_accept(hfiledescriptor_fd_t fd, hfiledescriptor_sockaddr_t *addr, hfiledescriptor_socklen_t *addrlen)
{
    hfiledescriptor_socketv1_t *socketv1=hfiledescriptor_socketv1_get(fd);
    if(socketv1==NULL)
    {
        return -1;
    }
    int ret=-1;
    if(socketv1->socketv1_opts!=NULL && socketv1->socketv1_opts->socketv1_accept!=NULL)
    {
        ret=socketv1->socketv1_opts->socketv1_accept(fd,addr,addrlen);
    }
    return ret;
}

int hfiledescriptor_socketv1_bind(hfiledescriptor_fd_t fd, const hfiledescriptor_sockaddr_t *name, hfiledescriptor_socklen_t namelen)
{
    hfiledescriptor_socketv1_t *socketv1=hfiledescriptor_socketv1_get(fd);
    if(socketv1==NULL)
    {
        return -1;
    }
    int ret=-1;
    if(socketv1->socketv1_opts!=NULL && socketv1->socketv1_opts->socketv1_bind!=NULL)
    {
        ret=socketv1->socketv1_opts->socketv1_bind(fd,name,namelen);
    }
    return ret;
}

int hfiledescriptor_socketv1_shutdown(hfiledescriptor_fd_t fd, int how)
{
    hfiledescriptor_socketv1_t *socketv1=hfiledescriptor_socketv1_get(fd);
    if(socketv1==NULL)
    {
        return -1;
    }
    int ret=-1;
    if(socketv1->socketv1_opts!=NULL && socketv1->socketv1_opts->socketv1_shutdown!=NULL)
    {
        ret=socketv1->socketv1_opts->socketv1_shutdown(fd,how);
    }
    return ret;
}

int hfiledescriptor_socketv1_getpeername (hfiledescriptor_fd_t fd, hfiledescriptor_sockaddr_t *name, hfiledescriptor_socklen_t *namelen)
{
    hfiledescriptor_socketv1_t *socketv1=hfiledescriptor_socketv1_get(fd);
    if(socketv1==NULL)
    {
        return -1;
    }
    int ret=-1;
    if(socketv1->socketv1_opts!=NULL && socketv1->socketv1_opts->socketv1_getpeername!=NULL)
    {
        ret=socketv1->socketv1_opts->socketv1_getpeername(fd,name,namelen);
    }
    return ret;
}

int hfiledescriptor_socketv1_getsockname (hfiledescriptor_fd_t fd, hfiledescriptor_sockaddr_t *name, hfiledescriptor_socklen_t *namelen)
{
    hfiledescriptor_socketv1_t *socketv1=hfiledescriptor_socketv1_get(fd);
    if(socketv1==NULL)
    {
        return -1;
    }
    int ret=-1;
    if(socketv1->socketv1_opts!=NULL && socketv1->socketv1_opts->socketv1_getsockname!=NULL)
    {
        ret=socketv1->socketv1_opts->socketv1_getsockname(fd,name,namelen);
    }
    return ret;
}

int hfiledescriptor_socketv1_getsockopt (hfiledescriptor_fd_t fd, int level, int optname, void *optval, hfiledescriptor_socklen_t *optlen)
{
    hfiledescriptor_socketv1_t *socketv1=hfiledescriptor_socketv1_get(fd);
    if(socketv1==NULL)
    {
        return -1;
    }
    int ret=-1;
    if(socketv1->socketv1_opts!=NULL && socketv1->socketv1_opts->socketv1_read!=NULL)
    {
        ret=socketv1->socketv1_opts->socketv1_getsockopt(fd,level,optname,optval,optlen);
    }
    return ret;
}

int hfiledescriptor_socketv1_setsockopt (hfiledescriptor_fd_t fd, int level, int optname, const void *optval, hfiledescriptor_socklen_t optlen)
{
    hfiledescriptor_socketv1_t *socketv1=hfiledescriptor_socketv1_get(fd);
    if(socketv1==NULL)
    {
        return -1;
    }
    int ret=-1;
    if(socketv1->socketv1_opts!=NULL && socketv1->socketv1_opts->socketv1_setsockopt!=NULL)
    {
        ret=socketv1->socketv1_opts->socketv1_setsockopt(fd,level,optname,optval,optlen);
    }
    return ret;
}

int hfiledescriptor_socketv1_connect(hfiledescriptor_fd_t fd, const hfiledescriptor_sockaddr_t *name, hfiledescriptor_socklen_t namelen)
{
    hfiledescriptor_socketv1_t *socketv1=hfiledescriptor_socketv1_get(fd);
    if(socketv1==NULL)
    {
        return -1;
    }
    int ret=-1;
    if(socketv1->socketv1_opts!=NULL && socketv1->socketv1_opts->socketv1_connect!=NULL)
    {
        ret=socketv1->socketv1_opts->socketv1_connect(fd,name,namelen);
    }
    return ret;
}

int hfiledescriptor_socketv1_listen(hfiledescriptor_fd_t fd, int backlog)
{
    hfiledescriptor_socketv1_t *socketv1=hfiledescriptor_socketv1_get(fd);
    if(socketv1==NULL)
    {
        return -1;
    }
    int ret=-1;
    if(socketv1->socketv1_opts!=NULL && socketv1->socketv1_opts->socketv1_listen!=NULL)
    {
        ret=socketv1->socketv1_opts->socketv1_listen(fd,backlog);
    }
    return ret;
}

hfiledescriptor_ssize_t hfiledescriptor_socketv1_recv(hfiledescriptor_fd_t fd, void *mem, hfiledescriptor_size_t len, int flags)
{
    hfiledescriptor_socketv1_t *socketv1=hfiledescriptor_socketv1_get(fd);
    if(socketv1==NULL)
    {
        return -1;
    }
    int ret=-1;
    if(socketv1->socketv1_opts!=NULL && socketv1->socketv1_opts->socketv1_recv!=NULL)
    {
        ret=socketv1->socketv1_opts->socketv1_recv(fd,mem,len,flags);
    }
    return ret;
}

hfiledescriptor_ssize_t hfiledescriptor_socketv1_readv(hfiledescriptor_fd_t fd, const hfiledescriptor_iovec_t *iov, int iovcnt)
{
    hfiledescriptor_socketv1_t *socketv1=hfiledescriptor_socketv1_get(fd);
    if(socketv1==NULL)
    {
        return -1;
    }
    int ret=-1;
    if(socketv1->socketv1_opts!=NULL && socketv1->socketv1_opts->socketv1_readv!=NULL)
    {
        ret=socketv1->socketv1_opts->socketv1_readv(fd,iov,iovcnt);
    }
    return ret;
}

hfiledescriptor_ssize_t hfiledescriptor_socketv1_recvfrom(hfiledescriptor_fd_t fd, void *mem, hfiledescriptor_size_t len, int flags,hfiledescriptor_sockaddr_t *from, hfiledescriptor_socklen_t *fromlen)
{
    hfiledescriptor_socketv1_t *socketv1=hfiledescriptor_socketv1_get(fd);
    if(socketv1==NULL)
    {
        return -1;
    }
    int ret=-1;
    if(socketv1->socketv1_opts!=NULL && socketv1->socketv1_opts->socketv1_recvfrom!=NULL)
    {
        ret=socketv1->socketv1_opts->socketv1_recvfrom(fd,mem,len,flags,from,fromlen);
    }
    return ret;
}

hfiledescriptor_ssize_t hfiledescriptor_socketv1_recvmsg(hfiledescriptor_fd_t fd, hfiledescriptor_msghdr_t *message, int flags)
{
    hfiledescriptor_socketv1_t *socketv1=hfiledescriptor_socketv1_get(fd);
    if(socketv1==NULL)
    {
        return -1;
    }
    int ret=-1;
    if(socketv1->socketv1_opts!=NULL && socketv1->socketv1_opts->socketv1_recvmsg!=NULL)
    {
        ret=socketv1->socketv1_opts->socketv1_recvmsg(fd,message,flags);
    }
    return ret;
}

hfiledescriptor_ssize_t hfiledescriptor_socketv1_send(hfiledescriptor_fd_t fd, const void *dataptr, hfiledescriptor_size_t len, int flags)
{
    hfiledescriptor_socketv1_t *socketv1=hfiledescriptor_socketv1_get(fd);
    if(socketv1==NULL)
    {
        return -1;
    }
    int ret=-1;
    if(socketv1->socketv1_opts!=NULL && socketv1->socketv1_opts->socketv1_send!=NULL)
    {
        ret=socketv1->socketv1_opts->socketv1_send(fd,dataptr,len,flags);
    }
    return ret;
}

hfiledescriptor_ssize_t hfiledescriptor_socketv1_sendmsg(hfiledescriptor_fd_t fd, const hfiledescriptor_msghdr_t *message, int flags)
{
    hfiledescriptor_socketv1_t *socketv1=hfiledescriptor_socketv1_get(fd);
    if(socketv1==NULL)
    {
        return -1;
    }
    int ret=-1;
    if(socketv1->socketv1_opts!=NULL && socketv1->socketv1_opts->socketv1_sendmsg!=NULL)
    {
        ret=socketv1->socketv1_opts->socketv1_sendmsg(fd,message,flags);
    }
    return ret;
}

hfiledescriptor_ssize_t hfiledescriptor_socketv1_sendto(hfiledescriptor_fd_t fd, const void *dataptr, hfiledescriptor_size_t len, int flags,const hfiledescriptor_sockaddr_t *to, hfiledescriptor_socklen_t tolen)
{
    hfiledescriptor_socketv1_t *socketv1=hfiledescriptor_socketv1_get(fd);
    if(socketv1==NULL)
    {
        return -1;
    }
    int ret=-1;
    if(socketv1->socketv1_opts!=NULL && socketv1->socketv1_opts->socketv1_sendto!=NULL)
    {
        ret=socketv1->socketv1_opts->socketv1_sendto(fd,dataptr,len,flags,to,tolen);
    }
    return ret;
}

hfiledescriptor_ssize_t hfiledescriptor_socketv1_writev(hfiledescriptor_fd_t fd, const hfiledescriptor_iovec_t *iov, int iovcnt)
{
    hfiledescriptor_socketv1_t *socketv1=hfiledescriptor_socketv1_get(fd);
    if(socketv1==NULL)
    {
        return -1;
    }
    int ret=-1;
    if(socketv1->socketv1_opts!=NULL && socketv1->socketv1_opts->socketv1_writev!=NULL)
    {
        ret=socketv1->socketv1_opts->socketv1_writev(fd,iov,iovcnt);
    }
    return ret;
}

int  hfiledescriptor_socketv1_read(hfiledescriptor_fd_t fd, void *buff, hfiledescriptor_size_t buff_len)
{
    hfiledescriptor_socketv1_t *socketv1=hfiledescriptor_socketv1_get(fd);
    if(socketv1==NULL)
    {
        return -1;
    }
    int ret=-1;
    if(socketv1->socketv1_opts!=NULL && socketv1->socketv1_opts->socketv1_read!=NULL)
    {
        ret=socketv1->socketv1_opts->socketv1_read(fd,buff,buff_len);
    }
    return ret;
}

int  hfiledescriptor_socketv1_write(hfiledescriptor_fd_t fd,const void *buff,hfiledescriptor_size_t  buff_len)
{
    hfiledescriptor_socketv1_t *socketv1=hfiledescriptor_socketv1_get(fd);
    if(socketv1==NULL)
    {
        return -1;
    }
    int ret=-1;
    if(socketv1->socketv1_opts!=NULL && socketv1->socketv1_opts->socketv1_write!=NULL)
    {
        ret=socketv1->socketv1_opts->socketv1_write(fd,buff,buff_len);
    }
    return ret;
}

int  hfiledescriptor_socketv1_close(hfiledescriptor_fd_t fd)
{
    hfiledescriptor_socketv1_t *socketv1=hfiledescriptor_socketv1_get(fd);
    if(socketv1==NULL)
    {
        return -1;
    }
    int ret=-1;
    if(socketv1->socketv1_opts!=NULL && socketv1->socketv1_opts->socketv1_close!=NULL)
    {
        ret=socketv1->socketv1_opts->socketv1_close(fd);
    }
    /*
     * 释放文件描述符,用户需要释放除文件描述符的其它资源
     */
    hfiledescriptor_common_table_free(fd);
    return ret;
}


int  hfiledescriptor_socketv1_ioctl(hfiledescriptor_fd_t fd, unsigned long op, va_list va)
{
    hfiledescriptor_socketv1_t *socketv1=hfiledescriptor_socketv1_get(fd);
    if(socketv1==NULL)
    {
        return -1;
    }
    int ret=-1;
    if(socketv1->socketv1_opts!=NULL && socketv1->socketv1_opts->socketv1_ioctl!=NULL)
    {
        ret=socketv1->socketv1_opts->socketv1_ioctl(fd,op,va);
    }
    return ret;
}

int  hfiledescriptor_socketv1_fcntl(hfiledescriptor_fd_t fd, int op,va_list va)
{
    hfiledescriptor_socketv1_t *socketv1=hfiledescriptor_socketv1_get(fd);
    if(socketv1==NULL)
    {
        return -1;
    }
    int ret=-1;
    if(socketv1->socketv1_opts!=NULL && socketv1->socketv1_opts->socketv1_fcntl!=NULL)
    {
        ret=socketv1->socketv1_opts->socketv1_fcntl(fd,op,va);
    }
    return ret;
}

