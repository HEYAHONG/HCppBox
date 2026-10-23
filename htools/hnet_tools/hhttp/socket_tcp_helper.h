#ifndef __SOCKET_TCP_HELPER_H__
#define __SOCKET_TCP_HELPER_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus
#include "stdbool.h"

void tcp_socket_set_nonblock(int socket_fd);
bool tcp_socket_is_egain(void);

#ifdef __cplusplus
}
#endif

#endif // __SOCKET_TCP_HELPER_H__

