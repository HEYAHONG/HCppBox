/***************************************************************
 * Name:      net.c
 * Purpose:   实现net接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-08-26
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#define HOPENBLT_NET_IMPLEMENTATION  1

#include "boot.h"
#include "hbox.h"

#if !defined(HOPENBLT_NET_FUNCTION_ATTRIBUTE)
#define HOPENBLT_NET_FUNCTION_ATTRIBUTE __WEAK
#endif

#if !defined(HOPENBLT_NET_NO_IMPLEMENTATION)

#include "patch/netdev.h"
#include "patch/uip.h"
#include "patch/uip_arp.h"

/*
 * 网络设备相关实现
 */
HOPENBLT_NET_FUNCTION_ATTRIBUTE
void         netdev_init(void)
{

}

HOPENBLT_NET_FUNCTION_ATTRIBUTE
void         netdev_init_mac(void)
{

}
void         netdev_get_mac(unsigned char * mac_addr)
{
    /*
     * 用户需要处理mac地址读取,此函数一般必须实现
     */
}

HOPENBLT_NET_FUNCTION_ATTRIBUTE
unsigned int netdev_read(void)
{
    /*
     * 用户实现时需要实现网络接收,返回数据包长度。具体内容通过全局变量uip_buf传递
     * 如果没有网络设备层(比如由其它协议栈管理)，用户可返回非0值，不过需要注意后续处理uip_len时其值可能并非实际长度
     */
    return 0;
}

HOPENBLT_NET_FUNCTION_ATTRIBUTE
void         netdev_send(void)
{
    /*
     * 用户实现时需要实现网络发送，具体内容通过全局变量uip_buf与uip_len传递。
     */
}

/*
 * DHCPC相关实现
 */
HOPENBLT_NET_FUNCTION_ATTRIBUTE
void dhcpc_init(unsigned char * mac,int len)
{

}

HOPENBLT_NET_FUNCTION_ATTRIBUTE
void dhcpc_request(void)
{

}

/*
 * UIP相关实现
 */
struct uip_conn *uip_conn=NULL;                         /**< 当前TCP连接 */
struct uip_conn uip_conns[UIP_CONNS]= {};               /**< TCP连接 */
struct uip_udp_conn *uip_udp_conn=NULL;                 /**< 当前UDP连接 */
struct uip_udp_conn uip_udp_conns[UIP_UDP_CONNS]= {};   /**< UDP连接 */

void *   uip_appdata=NULL;                              /**< 调用UIP_APPCALL接收用户数据时需要将此指针设置为接收数据的地址  */
//uint8_t  uip_buf[sizeof(struct uip_eth_hdr)];         /**< uip缓冲区,用户自行定义 */
uint16_t uip_len=0;                                     /**< uip各种长度的传递均使用此全局变量 */

HOPENBLT_NET_FUNCTION_ATTRIBUTE
void      uip_init(void)
{

}

HOPENBLT_NET_FUNCTION_ATTRIBUTE
void      uip_input(void)
{
    /*
     * 处理网络数据输入,具体内容通过全局变量uip_buf与uip_len传递
     */


    if(uip_conn==NULL)
    {
        uip_conn=&uip_conns[0];
    }

    if(uip_udp_conn==NULL)
    {
        uip_udp_conn=&uip_udp_conns[0];
    }

    UIP_APPCALL();          /**< 调用APPCALL,调用APPCALL时应当设置好全局变量uip_conn、uip_appdata、uip_len */

}

HOPENBLT_NET_FUNCTION_ATTRIBUTE
void      uip_sethostaddr(uip_ipaddr_t addr)
{
    /*
     * 设置ip地址
     */
}

HOPENBLT_NET_FUNCTION_ATTRIBUTE
void      uip_setnetmask(uip_ipaddr_t addr)
{
    /*
     * 设置掩码地址
     */
}

HOPENBLT_NET_FUNCTION_ATTRIBUTE
void      uip_setdraddr(uip_ipaddr_t addr)
{
    /*
     * 设置网关地址
     */
}

HOPENBLT_NET_FUNCTION_ATTRIBUTE
void      uip_listen(uint16_t port)
{
    /*
     * 监听端口
     */
}

HOPENBLT_NET_FUNCTION_ATTRIBUTE
int       uip_connected(void)
{
    /*
     * 返回非0用于标识已连接
     */
    return 1;
}

HOPENBLT_NET_FUNCTION_ATTRIBUTE
int       uip_acked(void)
{
    /*
     * 返回非0用于标识数据发送成功
     */
    return 1;
}

HOPENBLT_NET_FUNCTION_ATTRIBUTE
int       uip_rexmit(void)
{
    /*
     * 返回非0用于重新发送数据
     */
    return 0;
}

HOPENBLT_NET_FUNCTION_ATTRIBUTE
int       uip_poll(void)
{
    /*
     * 返回非0用于发送数据
     */
    return 1;
}

HOPENBLT_NET_FUNCTION_ATTRIBUTE
int       uip_newdata(void)
{
    /*
     * 返回非0标识具有新数据,具体内容通过uip_appdata与uip_datalen()获取。
     */
    return 0;
}

HOPENBLT_NET_FUNCTION_ATTRIBUTE
void      uip_send(const void *data, int len)
{
    /*
     * 发送uip数据,具体内容通过全局变量uip_buf与uip_len传递。
     */
}

HOPENBLT_NET_FUNCTION_ATTRIBUTE
uint16_t  uip_datalen(void)
{
    /*
     * 返回接收到的用户数据长度
     */
    return uip_len;
}

HOPENBLT_NET_FUNCTION_ATTRIBUTE
void      uip_periodic(size_t index)
{
    uip_conn=&uip_conns[index];
    /*
     * 更新连接状态
     */
}

HOPENBLT_NET_FUNCTION_ATTRIBUTE
void      uip_udp_periodic(size_t index)
{
    uip_udp_conn=&uip_udp_conns[index];
    /*
     * 更新连接状态
     */
}

HOPENBLT_NET_FUNCTION_ATTRIBUTE
void      uip_poll_conn(void *conn)
{
    uip_conn=(struct uip_conn *)conn;
    /*
     * 获取连接数据
     */
}


/*
 * UIP-ARP相关实现,使用其它协议栈时一般不用实现
 */
HOPENBLT_NET_FUNCTION_ATTRIBUTE
void uip_arp_init(void)
{

}

HOPENBLT_NET_FUNCTION_ATTRIBUTE
void uip_arp_arpin(void)
{
    /*
     * 具体内容通过全局变量uip_buf与uip_len传递。
     */
}

HOPENBLT_NET_FUNCTION_ATTRIBUTE
void uip_arp_out(void)
{
    /*
     * 具体内容通过全局变量uip_buf与uip_len传递。
     */
}

HOPENBLT_NET_FUNCTION_ATTRIBUTE
void uip_arp_timer(void)
{

}

#endif

