/***************************************************************
 * Name:      HCPPSocket.cpp
 * Purpose:   HCPPSocket实现，辅助套接字调用,套接字依赖操作系统实现，在windows下需要链接ws2_32库。。
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-09-15
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "HCPPSocket.h"
#ifdef HCPPSOCKET_HAVE_SOCKET
static class socket_manager
{
public:
    socket_manager()
    {
#ifdef WIN32
        WSADATA wsaData;
        const WORD VersionList[]=
        {
            (2 << 8) | 2,
            (1 << 8) | 1,
        };
        for(size_t i=0;i<sizeof(VersionList)/sizeof(VersionList[0]);i++)
        {
            if(WSAStartup(VersionList[i],&wsaData)==0)
            {
                break;
            }
        }
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



#endif // HCPPSOCKET_HAVE_SOCKET

