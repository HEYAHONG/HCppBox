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
#ifdef __CYGWIN__
void HCPPSocketCygwinHelperStartup();
void HCPPSocketCygwinHelperCleanup();
#endif // __CYGWIN__
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
        for(size_t i=0; i<sizeof(VersionList)/sizeof(VersionList[0]); i++)
        {
            if(WSAStartup(VersionList[i],&wsaData)==0)
            {
                break;
            }
        }
#endif
#ifdef __CYGWIN__
        HCPPSocketCygwinHelperStartup();
#endif // __CYGWIN__
    }
    ~socket_manager()
    {
#ifdef WIN32
        WSACleanup();
#endif
#ifdef __CYGWIN__
        HCPPSocketCygwinHelperCleanup();
#endif // __CYGWIN__
    }
} g_socket_manager;

void *HCPPSocketInit()
{
    return &g_socket_manager;
}

#ifndef WIN32
int closesocket(SOCKET s)
{
    return close(s);
}
#endif

#ifdef HCPPSOCKET_HAVE_SOCKET_IPV4

/*
 * 通过主机名查询ip(IPV4)地址,通常用于connect
 */
bool HCPPSocketNslookup(const char* hostname, void (*reslut)(const char* hostname, const char* addr_string, HCPPSocketAddressIPV4* addr, void* usr), void* usr)
{
    return HCPPSocketNslookup(hostname, std::function<void(const char*, const char*, HCPPSocketAddressIPV4*, void*)>(reslut), usr);
}
bool HCPPSocketNslookup(const char* hostname, std::function<void(const char*, const char*, HCPPSocketAddressIPV4*, void*)> result, void* usr)
{
    bool ret = true;
    struct addrinfo hints = { 0 };
    struct addrinfo *ai_result = NULL;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_CANONNAME;
    hints.ai_protocol = IPPROTO_TCP;
    if (getaddrinfo(hostname, NULL, &hints, &ai_result) == 0)
    {
        for (struct addrinfo* info = ai_result; info != NULL; info = info->ai_next)
        {
            if (info->ai_family == AF_INET)
            {
                HCPPSocketAddressIPV4 addr = *(HCPPSocketAddressIPV4*)info->ai_addr;
                char addr_string[32] = { 0 };
                inet_ntop(AF_INET, &addr.sin_addr, addr_string, sizeof(addr_string));
                if (result != NULL)
                {
                    result(hostname, addr_string, &addr, usr);
                }
            }
        }
        freeaddrinfo(ai_result);
    }
    else
    {
        ret = false;
    }

    return ret;

}
#endif

#ifdef HCPPSOCKET_HAVE_SOCKET_IPV6

/*
 * 通过主机名查询ip(IPV6)地址,通常用于connect
 */
bool HCPPSocketNslookup6(const char* hostname, void (*reslut)(const char* hostname, const char* addr_string, HCPPSocketAddressIPV6* addr, void* usr), void* usr)
{
    return HCPPSocketNslookup6(hostname, std::function<void(const char*, const char*, HCPPSocketAddressIPV6*, void*)>(reslut), usr);
}
bool HCPPSocketNslookup6(const char* hostname, std::function<void(const char*, const char*, HCPPSocketAddressIPV6*, void*)> result, void* usr)
{
    bool ret = true;
    struct addrinfo hints = { 0 };
    struct addrinfo* ai_result = NULL;
    hints.ai_family = AF_INET6;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_CANONNAME;
    hints.ai_protocol = IPPROTO_TCP;
    if (getaddrinfo(hostname, NULL, &hints, &ai_result) == 0)
    {
        for (struct addrinfo* info = ai_result; info != NULL; info = info->ai_next)
        {
            if (info->ai_family == AF_INET6)
            {
                HCPPSocketAddressIPV6 addr = *(HCPPSocketAddressIPV6*)info->ai_addr;
                char addr_string[128] = { 0 };
                inet_ntop(AF_INET6, &addr.sin6_addr, addr_string, sizeof(addr_string));
                if (result != NULL)
                {
                    result(hostname, addr_string, &addr, usr);
                }
            }
        }
        freeaddrinfo(ai_result);
    }
    else
    {
        ret = false;
    }

    return ret;
}

#endif


#endif // HCPPSOCKET_HAVE_SOCKET

