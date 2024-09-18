#include "HCPPBox.h"
#include "stdint.h"
#include <thread>
#include <chrono>

void Socket_main()
{
#ifdef HCPPSOCKET_HAVE_SOCKET
    printf(__FILE__  " Start\r\n");
    {
        printf("-----Test IPV4 Nslookup-----\r\n");
        //IPV4
        HCPPSocketNslookup("hyhsystem.cn", [=](const char* hostname, const char* addr_string, HCPPSocketAddressIPV4* addr, void* usr)
        {
            printf("name:%s,ip:%s\r\n", hostname, addr_string);
        }, NULL);
        HCPPSocketNslookup("www.baidu.com", [=](const char* hostname, const char* addr_string, HCPPSocketAddressIPV4* addr, void* usr)
        {
            printf("name:%s,ip:%s\r\n", hostname, addr_string);
        }, NULL);

        printf("-----Test IPV6 Nslookup-----\r\n");
        //IPV6
        HCPPSocketNslookup6("hyhsystem.cn", [=](const char* hostname, const char* addr_string, HCPPSocketAddressIPV6* addr, void* usr)
        {
            printf("name:%s,ip:%s\r\n", hostname, addr_string);
        }, NULL);
        HCPPSocketNslookup6("www.baidu.com", [=](const char* hostname, const char* addr_string, HCPPSocketAddressIPV6* addr, void* usr)
        {
            printf("name:%s,ip:%s\r\n", hostname, addr_string);
        }, NULL);
    }
#endif
    printf(__FILE__  " END\r\n");
}
