
#if defined(WIN32) || defined(_WIN32) || defined(__CYGWIN__)

#include "winsock2.h"
#include "windows.h"
#include "stdbool.h"
#if defined(__CYGWIN__)
#include "fcntl.h"
#endif

void tcp_socket_set_nonblock(int socket_fd)
{
#if defined(__CYGWIN__)
    int flags = fcntl(socket_fd, F_GETFL, 0);
    fcntl(socket_fd, F_SETFL, flags | O_NONBLOCK);
#endif
#if defined(__CYGWIN__)
    __ms_u_long mode=1;
#else
    u_long mode=1;
#endif
    ioctlsocket(socket_fd, FIONBIO, &mode);
}

bool tcp_socket_is_egain(void)
{
    return WSAGetLastError()== WSAEWOULDBLOCK;
}

#else

#if defined(__unix__) || defined(__linux__)
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>
#if defined(__unix__)
#include "sys/stat.h"
#include "sys/un.h"
#endif
#endif
#include "fcntl.h"
#include "errno.h"
#include "stdint.h"
#include "stdbool.h"
#include "socket_tcp_helper.h"

void tcp_socket_set_nonblock(int socket_fd)
{
    {
        int flags = fcntl(socket_fd, F_GETFL, 0);
        fcntl(socket_fd, F_SETFL, flags | O_NONBLOCK);
    }

    {
        //设定接收超时10ms
        struct timeval  tv;
        tv.tv_sec = 0;
        tv.tv_usec = 10*1000;
        setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof(tv));
    }

}



bool tcp_socket_is_egain(void)
{
    return (errno==EAGAIN) || (errno==EWOULDBLOCK);
}

#endif

