/***************************************************************
 * Name:      HCPPSocket.h
 * Purpose:   HCPPSocket接口，辅助套接字调用,套接字依赖操作系统实现，在windows下需要链接ws2_32库。
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-09-15
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HCPPSOCKET_H__
#define __HCPPSOCKET_H__

#if defined(__unix__) || defined(__linux__)
#ifndef HCPPSOCKET_HAVE_SOCKET
#define HCPPSOCKET_HAVE_SOCKET 1
#endif
#ifndef HCPPSOCKET_HAVE_SOCKET_IPV4
#define HCPPSOCKET_HAVE_SOCKET_IPV4 1
#endif
#ifndef HCPPSOCKET_HAVE_SOCKET_IPV6
#define HCPPSOCKET_HAVE_SOCKET_IPV6 1
#endif
#ifndef HCPPSOCKET_HAVE_SOCKET_UNIX
#define HCPPSOCKET_HAVE_SOCKET_UNIX 1
#endif
#endif


#ifdef WIN32
#ifndef HCPPSOCKET_HAVE_SOCKET
#define HCPPSOCKET_HAVE_SOCKET 1
#endif
#ifndef HCPPSOCKET_HAVE_SOCKET_IPV4
#define HCPPSOCKET_HAVE_SOCKET_IPV4 1
#endif
#ifndef HCPPSOCKET_HAVE_SOCKET_IPV6
#define HCPPSOCKET_HAVE_SOCKET_IPV6 1
#endif
#endif


#ifdef HCPPSOCKET_HAVE_SOCKET

/*
 *  unix/linux头文件
 */
#if defined(__unix__) || defined(__linux__)
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>
#if defined(__unix__)
#include "sys/stat.h"
#include "sys/un.h"
#endif
#endif

/*
 *  windows头文件
 */
#ifdef WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

/*
 *  qnx头文件
 */
#ifdef __QNX__
#include <net/netbyte.h>
#endif

/*
 *  POSIX头文件
 */
#ifndef  HCPPSOCKET_NO_POSIX
#include "fcntl.h"
#endif


/*
 *  公共头文件
 */
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>


/*
 *  套接字,通常使用此类型定义一个套接字
 */
#ifndef SOCKET
#define SOCKET int
#endif

/*
 * 无效套接字，当SOCKET类型的变量等于无效套接字时，表示套接字无效，可用初始化SOCKET类型的变量
 */
#ifndef INVALID_SOCKET
#define INVALID_SOCKET (-1)
#endif

/*
 *  套接字地址,此地址一般不直接使用，一般由具体的套接字地址(HCPPSocketAddress*)指针转化为本类型的指针（其作用类似void*）调用API（如bind）
 */
typedef struct sockaddr HCPPSocketAddress;
/*
 *  套接字地址长度
 */
typedef socklen_t HCPPSocketAddressLength;
/*
 *  地址大小默认为16字节
 */
#define HCPPSOCKETADDRESS_MAX_LENGTH sizeof(HCPPSocketAddress)

#ifdef HCPPSOCKET_HAVE_SOCKET_IPV4
/*
 *  IPV4套接字地址,可转化为HCPPSocketAddress*;
 */
typedef struct sockaddr_in HCPPSocketAddressIPV4;

/*
 *  地址大小默认为16字节
 */
#undef  HCPPSOCKETADDRESS_MAX_LENGTH
#define HCPPSOCKETADDRESS_MAX_LENGTH (sizeof(HCPPSocketAddressIPV4))

#endif

#ifdef HCPPSOCKET_HAVE_SOCKET_IPV6
/*
 *  IPV6套接字地址,可转化为HCPPSocketAddress*;
 */
typedef struct sockaddr_in6 HCPPSocketAddressIPV6;

/*
 *  地址大小默认为28字节
 */
#undef  HCPPSOCKETADDRESS_MAX_LENGTH
#define HCPPSOCKETADDRESS_MAX_LENGTH (sizeof(HCPPSocketAddressIPV6))

#endif

#ifdef HCPPSOCKET_HAVE_SOCKET_UNIX
/*
 *  UNIX套接字(文件)地址,可转化为HCPPSocketAddress*;
 */
typedef struct sockaddr_un HCPPSocketAddressUNIX;

/*
 *  地址大小默认为110字节
 */
#undef  HCPPSOCKETADDRESS_MAX_LENGTH
#define HCPPSOCKETADDRESS_MAX_LENGTH (sizeof(HCPPSocketAddressUNIX))

#endif

#ifdef __cplusplus

/*
 * 自动套接字地址，可用于通用API编写，但可能浪费部分内存空间（会使用占用空间最大的套接字地址长度）
 */
class HCPPSocketAddressAuto
{
    struct
    {
        int8_t pad[HCPPSOCKETADDRESS_MAX_LENGTH];
    }   addr;
public:
    HCPPSocketAddressAuto()
    {
        memset(&addr,0,sizeof(addr));
    }
    HCPPSocketAddressAuto(HCPPSocketAddressAuto &oths):addr(oths.addr)
    {

    }
    HCPPSocketAddressAuto(HCPPSocketAddressAuto &&oths):addr(oths.addr)
    {

    }
    virtual ~HCPPSocketAddressAuto()
    {

    }
    operator HCPPSocketAddress *()
    {
        return (HCPPSocketAddress *)(void *)addr.pad;
    }
#ifdef  HCPPSOCKET_HAVE_SOCKET_IPV4
    operator HCPPSocketAddressIPV4 *()
    {
        if(((HCPPSocketAddress *)(*this))->sa_family==AF_INET)
        {
            return (HCPPSocketAddressIPV4 *)(void *)addr.pad;
        }
        else
        {
            return NULL;
        }
    }
#endif

#ifdef  HCPPSOCKET_HAVE_SOCKET_IPV6
    operator HCPPSocketAddressIPV6 *()
    {
        if(((HCPPSocketAddress *)(*this))->sa_family==AF_INET6)
        {
            return (HCPPSocketAddressIPV6 *)(void *)addr.pad;
        }
        else
        {
            return NULL;
        }
    }
#endif

#ifdef  HCPPSOCKET_HAVE_SOCKET_UNIX
    operator HCPPSocketAddressUNIX *()
    {
        if(((HCPPSocketAddress *)(*this))->sa_family==AF_UNIX)
        {
            return (HCPPSocketAddressUNIX *)(void *)addr.pad;
        }
        else
        {
            return NULL;
        }
    }
#endif

    HCPPSocketAddressLength Length()
    {
        HCPPSocketAddressLength len=sizeof(addr);
        switch(((HCPPSocketAddress *)(*this))->sa_family)
        {
#ifdef  HCPPSOCKET_HAVE_SOCKET_IPV4
        case AF_INET:
        {
            len=sizeof(HCPPSocketAddressIPV4);
        }
        break;
#endif
#ifdef  HCPPSOCKET_HAVE_SOCKET_IPV6
        case AF_INET6:
        {
            len=sizeof(HCPPSocketAddressIPV6);
        }
        break;
#endif
#ifdef  HCPPSOCKET_HAVE_SOCKET_UNIX
        case AF_UNIX:
        {
            len=sizeof(HCPPSocketAddressUNIX);
        }
        break;
#endif
        default:
            break;
        }
        return len;
    }

    operator HCPPSocketAddressLength()
    {
        return Length();
    }

};

#endif

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef WIN32
/*
 *  关闭套接字，通常用于兼容windows下的代码
 */
int closesocket(SOCKET s);
#endif

#ifdef __cplusplus
}
#endif



#endif

#endif // __HCPPSOCKET_H__


