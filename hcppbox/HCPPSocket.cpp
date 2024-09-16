/***************************************************************
 * Name:      HCPPSocket.cpp
 * Purpose:   HCPPSocket实现，辅助套接字调用,套接字依赖操作系统实现，在windows下需要链接ws2_32库。。
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-09-15
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "HCPPSocket.h"
#ifdef HCPPBOX_HAVE_SOCKET
static class socket_manager
{
public:
    socket_manager()
    {
#ifdef WIN32
        static WSADATA wsaData;
        WSAStartup((1 << 8) | 1, &wsaData);
#endif
    }
    ~socket_manager()
    {
#ifdef WIN32
        WSACleanup();
#endif
    }
} g_socket_manager;

#ifndef WIN32
int closesocket(SOCKET s)
{
    return close(s);
}
#endif

#endif // HCPPBOX_HAVE_SOCKET

