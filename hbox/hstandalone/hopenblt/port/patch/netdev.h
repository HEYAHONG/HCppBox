#ifndef __PATCH_NETDEV_H__
#define __PATCH_NETDEV_H__

#include "hdefaults.h"

#ifdef __cplusplus
extern "C"
{
#endif

#if !defined(HOPENBLT_NET_NO_IMPLEMENTATION) || defined(HOPENBLT_NET_PATCH_HEADER)

void         netdev_init(void);
void         netdev_init_mac(void);
void         netdev_get_mac(unsigned char * mac_addr);
unsigned int netdev_read(void);
void         netdev_send(void);

void dhcpc_init(unsigned char * mac,int len);
void dhcpc_request(void);

#else

#include "netdev.h"

#endif

#ifdef __cplusplus
}
#endif


#endif

