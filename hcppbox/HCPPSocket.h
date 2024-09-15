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
#ifndef WIN32
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>
#include "sys/stat.h"
#include "sys/un.h"
#else
#include <winsock2.h>
#include <ws2tcpip.h>
#endif
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#ifdef __QNX__
# include <net/netbyte.h>
#endif
#endif

#endif // __HCPPSOCKET_H__


