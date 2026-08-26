#ifndef __PATCH_UIP_ARP_H__
#define __PATCH_UIP_ARP_H__

#include "hdefaults.h"
#include "uip.h"

#ifdef __cplusplus
extern "C"
{
#endif

#if !defined(HOPENBLT_NET_NO_IMPLEMENTATION)


struct uip_eth_hdr
{
    struct uip_eth_addr dest;
    struct uip_eth_addr src;
    uint16_t type;
};

#ifndef UIP_ETHTYPE_ARP
#define UIP_ETHTYPE_ARP 0x0806
#endif
#ifndef UIP_ETHTYPE_IP
#define UIP_ETHTYPE_IP  0x0800
#endif // UIP_ETHTYPE_IP
#ifndef UIP_ETHTYPE_IP6
#define UIP_ETHTYPE_IP6 0x86dd
#endif


void uip_arp_init(void);
#define uip_arp_ipin()
void uip_arp_arpin(void);
void uip_arp_out(void);
void uip_arp_timer(void);

#else

#include "uip_arp.h"

#endif

#ifdef __cplusplus
}
#endif


#endif

