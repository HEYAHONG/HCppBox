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
#ifndef HCPPBOX_HAVE_SOCKET
#define HCPPBOX_HAVE_SOCKET 1
#endif
#endif
#ifdef WIN32
#ifndef HCPPBOX_HAVE_SOCKET
#define HCPPBOX_HAVE_SOCKET 1
#endif
#endif
#ifdef HCPPBOX_HAVE_SOCKET

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


