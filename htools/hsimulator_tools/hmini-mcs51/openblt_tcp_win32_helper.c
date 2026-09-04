
#if defined(WIN32) || defined(_WIN32) || defined(__CYGWIN__)

#include "winsock2.h"
#include "windows.h"
#include "stdbool.h"
#if defined(__CYGWIN__)
#include "fcntl.h"
#endif

void openblt_socket_set_nonblock(int socket_fd)
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

bool openblt_socket_is_egain(void)
{
    return WSAGetLastError()== WSAEWOULDBLOCK;
}

#endif

