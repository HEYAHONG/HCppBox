#ifndef __PATCH_UIP_H__
#define __PATCH_UIP_H__

#include "hdefaults.h"

#include "boot.h"
#include "net.h"
#ifdef __cplusplus
extern "C"
{
#endif

#if !defined(HOPENBLT_NET_NO_IMPLEMENTATION)

typedef uint8_t uip_ip4addr_t[4];
typedef uip_ip4addr_t uip_ipaddr_t;
#ifndef uip_ipaddr
#define uip_ipaddr(addr, addr0,addr1,addr2,addr3)  do {addr[0]=addr0;addr[1]=addr1;addr[2]=addr2;addr[3]=addr3;} while(0)
#endif

void      uip_init(void);
void      uip_input(void);
void      uip_sethostaddr(uip_ipaddr_t addr);
void      uip_setnetmask(uip_ipaddr_t addr);
void      uip_setdraddr(uip_ipaddr_t addr);
void      uip_listen(uint16_t port);
int       uip_connected(void);
int       uip_acked(void);
int       uip_rexmit(void);
int       uip_poll(void);
int       uip_newdata(void);
void      uip_send(const void *data, int len);
uint16_t  uip_datalen(void);
void      uip_periodic(size_t index);
void      uip_udp_periodic(size_t index);
void      uip_poll_conn(void *conn);


#ifndef htons
#define htons hhtobe16
#endif


#ifndef HTONS
#define HTONS htons
#endif

struct uip_eth_addr
{
    uint8_t addr[6];
};


struct uip_conn
{
    uip_ipaddr_t ripaddr;   /**< The IP address of the remote host. */

    uint16_t lport;        /**< The local TCP port, in network byte order. */
    uint16_t rport;        /**< The local remote TCP port, in network byte
             order. */

    uint8_t rcv_nxt[4];    /**< The sequence number that we expect to
             receive next. */
    uint8_t snd_nxt[4];    /**< The sequence number that was last sent by
                         us. */
    uint16_t len;          /**< Length of the data that was previously sent. */
    uint16_t mss;          /**< Current maximum segment size for the
             connection. */
    uint16_t initialmss;   /**< Initial maximum segment size for the
             connection. */
    uint16_t sa;            /**< Retransmission time-out calculation state
             variable. */
    uint16_t sv;            /**< Retransmission time-out calculation state
             variable. */
    uint8_t rto;           /**< Retransmission time-out. */
    uint8_t tcpstateflags; /**< TCP state and flags. */
    uint8_t timer;         /**< The retransmission timer. */
    uint8_t nrtx;          /**< The number of retransmissions for the last
             segment sent. */

    /** The application state. */
    uip_tcp_appstate_t appstate;
};


#if !defined(UIP_CONNS)
#define UIP_CONNS 1
#endif

extern struct uip_conn *uip_conn;
/* The array containing all uIP connections. */
extern struct uip_conn uip_conns[UIP_CONNS];

#ifndef uip_udp_appstate_t
#define uip_udp_appstate_t uip_tcp_appstate_t
#endif // uip_udp_appstate_t

struct uip_udp_conn
{
    uip_ipaddr_t ripaddr;   /**< The IP address of the remote peer. */
    uint16_t lport;        /**< The local port number in network byte order. */
    uint16_t rport;        /**< The remote port number in network byte order. */
    uint8_t  ttl;          /**< Default time-to-live. */

    /** The application state. */
    uip_udp_appstate_t appstate;
};

#ifndef UIP_UDP_CONNS
#define UIP_UDP_CONNS 1
#endif
extern struct uip_udp_conn *uip_udp_conn;
extern struct uip_udp_conn uip_udp_conns[UIP_UDP_CONNS];

extern void *   uip_appdata;

#ifndef UIP_CONF_BUFFER_SIZE
#define UIP_BUFSIZE     400
#else /* UIP_CONF_BUFFER_SIZE */
#define UIP_BUFSIZE UIP_CONF_BUFFER_SIZE
#endif /* UIP_CONF_BUFFER_SIZE */

extern uint8_t  uip_buf[];
extern uint16_t uip_len;

#else

#include "uip.h"

#endif

#ifdef __cplusplus
}
#endif


#endif

