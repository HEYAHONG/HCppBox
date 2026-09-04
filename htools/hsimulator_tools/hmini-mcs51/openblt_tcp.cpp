
#include "HCPPBox.h"
#include "hbox.h"
extern "C"
{
#include "boot.h"
}

extern "C" bool openblt_process(uint8_t *rom,size_t romlen);
bool openblt_load_rom_ok=false;

/*
 * nvm
 */
uint8_t nvm_rom[BOOT_NVM_SIZE_KB*1024]= {0};


void     NvmInit(void)
{

}


blt_bool NvmWrite(blt_addr addr, blt_int32u len, blt_int8u *data)
{
    if(addr > sizeof(nvm_rom))
    {
        return BLT_FALSE;
    }
    if(addr+len > sizeof(nvm_rom))
    {
        len=sizeof(nvm_rom)-addr;
    }
    if(data!=NULL)
    {
        memcpy(&nvm_rom[addr],data,len);
    }
    return BLT_TRUE;
}

blt_bool NvmErase(blt_addr addr, blt_int32u len)
{
    (void)addr;
    (void)len;
    return BLT_TRUE;
}


blt_bool NvmVerifyChecksum(void)
{
    /*
     * 此函数返回FALSE时bootloader不会启动用户程序
     */
    return openblt_load_rom_ok?BLT_TRUE:BLT_FALSE;
}


blt_addr NvmGetUserProgBaseAddress(void)
{
    return 0;
}


blt_bool NvmDone(void)
{
    openblt_load_rom_ok=true;
    return BLT_TRUE;
}

/*
 * NET
 */
extern "C"
{
#include "patch/netdev.h"
#include "patch/uip.h"
#include "patch/uip_arp.h"
}

struct uip_conn *uip_conn=NULL;                         /**< 当前TCP连接 */
struct uip_conn uip_conns[UIP_CONNS]= {};               /**< TCP连接 */
struct uip_udp_conn *uip_udp_conn=NULL;                 /**< 当前UDP连接 */
struct uip_udp_conn uip_udp_conns[UIP_UDP_CONNS]= {};   /**< UDP连接 */

void *   uip_appdata=NULL;                              /**< 调用UIP_APPCALL接收用户数据时需要将此指针设置为接收数据的地址  */
uint8_t  uip_buf[(UIP_BUFSIZE)>sizeof(struct uip_eth_hdr)?(UIP_BUFSIZE):(sizeof(struct uip_eth_hdr))];           /**< uip缓冲区,用户自行定义 */
uint16_t uip_len=0;                                     /**< uip各种长度的传递均使用此全局变量 */



void         netdev_init(void)
{
    HCPPSocketInit();
}


void         netdev_init_mac(void)
{

}
void         netdev_get_mac(unsigned char * mac_addr)
{

}


unsigned int netdev_read(void)
{
    struct uip_eth_hdr *hdr=(struct uip_eth_hdr *)uip_buf;
    /*
     * 整个过程不会使用uip_buf,但是需要将类型字段设置为UIP_ETHTYPE_IP触发uip_input
     */
    hdr->type=hhtobe16(UIP_ETHTYPE_IP);
    return  sizeof(uip_buf);
}


void  netdev_send(void)
{

}

void dhcpc_init(unsigned char * mac,int len)
{

}


void dhcpc_request(void)
{

}

static SOCKET server_fd=INVALID_SOCKET;
static SOCKET client_fd=INVALID_SOCKET;

#if !defined(SOCK_NONBLOCK) || defined(__CYGWIN__)
extern "C" void openblt_socket_set_nonblock(int socket_fd);
extern "C" bool openblt_socket_is_egain(void);
#endif

void      uip_init(void)
{
#if defined(SOCK_NONBLOCK) && !defined(__CYGWIN__)
    server_fd=socket(AF_INET,SOCK_STREAM | SOCK_NONBLOCK,0);
#else
    server_fd=socket(AF_INET,SOCK_STREAM,0);
#endif
    if(server_fd==INVALID_SOCKET)
    {
        hprintf("openblt:new socket error!\r\n");
        throw -1;
    }
#if !defined(SOCK_NONBLOCK) || defined(__CYGWIN__)
    {
        openblt_socket_set_nonblock(server_fd);
    }
#endif
}


void      uip_input(void)
{
    uip_len=0;

    if(uip_conn==NULL)
    {
        uip_conn=&uip_conns[0];
    }

    if(uip_udp_conn==NULL)
    {
        uip_udp_conn=&uip_udp_conns[0];
    }

    if(client_fd==INVALID_SOCKET)
    {
        client_fd=accept(server_fd,NULL,NULL);
        if(client_fd < 0)
        {
            client_fd=INVALID_SOCKET;
        }
        else
        {
            {
                //设定接收超时5ms
                struct timeval  tv;
                tv.tv_sec = 0;
                tv.tv_usec = 5*1000;
                setsockopt(client_fd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof(tv));
            }
#if !defined(SOCK_NONBLOCK) || defined(__CYGWIN__)
            {
                openblt_socket_set_nonblock(client_fd);
            }
#endif

        }
    }
    else if(((uip_tcp_appstate_t *)&(uip_conn->appstate))->dto_tx_req != BLT_TRUE)
    {
        uint8_t packet[4096]= {0};
        int recvlen=recv(client_fd,(char *)packet,sizeof(packet),0);
        if(recvlen>0)
        {
            uip_appdata=packet;
            uip_len=recvlen;
        }
#if !defined(SOCK_NONBLOCK) || defined(__CYGWIN__)
        if(recvlen==0 || (recvlen < 0 && !openblt_socket_is_egain()))
#else
        if(recvlen==0 || (recvlen < 0 && errno!=EAGAIN))
#endif
        {
            closesocket(client_fd);
            client_fd=INVALID_SOCKET;
        }
    }

    UIP_APPCALL();          /**< 调用APPCALL,调用APPCALL时应当设置好全局变量uip_conn、uip_appdata、uip_len */

}


void      uip_sethostaddr(uip_ipaddr_t addr)
{

}


void      uip_setnetmask(uip_ipaddr_t addr)
{

}


void      uip_setdraddr(uip_ipaddr_t addr)
{

}


void      uip_listen(uint16_t port)
{
    HCPPSocketAddressIPV4 addr= {0};
    {
        addr.sin_family=AF_INET;
        addr.sin_port=port;
    }

    if(bind(server_fd,(HCPPSocketAddress *)&addr,sizeof(addr))!=0)
    {
        hprintf("openblt:bind socket error!\r\n");
        closesocket(server_fd);
        throw -1;
    }

    //默认队列中只有一个客户端
    if(listen(server_fd,1)!=0)
    {
        hprintf("listen socket error!\r\n");
        closesocket(server_fd);
        throw -1;
    }

}


int       uip_connected(void)
{
    static bool once_flag=false;
    if(client_fd!=INVALID_SOCKET)
    {
        if(!once_flag)
        {
            once_flag=true;
            return 1;
        }
    }
    else
    {
        once_flag=false;
    }

    return 0;
}


int       uip_acked(void)
{
    return 1;
}


int       uip_rexmit(void)
{
    return 0;
}


int       uip_poll(void)
{
    return 1;
}


int       uip_newdata(void)
{
    return uip_len;
}


void      uip_send(const void *data, int len)
{
    if(client_fd!=INVALID_SOCKET)
    {
        size_t offset=0;
        do
        {
            int sendlen=send(client_fd,(const char *)(&((uint8_t *)data)[offset]),len-offset,0);
            if(sendlen>0)
            {
                offset+=sendlen;
            }
            if(sendlen==0)
            {
                //连接已断开
                offset=len;
                closesocket(client_fd);
            }
        }
        while(offset < len);
    }

    uip_len=0;
}


uint16_t  uip_datalen(void)
{
    return uip_len;
}


void      uip_periodic(size_t index)
{
    uip_conn=&uip_conns[index];
}


void      uip_udp_periodic(size_t index)
{
    uip_udp_conn=&uip_udp_conns[index];
}


void      uip_poll_conn(void *conn)
{
    uip_conn=(struct uip_conn *)conn;
}


void uip_arp_init(void)
{

}


void uip_arp_arpin(void)
{

}


void uip_arp_out(void)
{

}


void uip_arp_timer(void)
{

}



/*
 * 主处理过程
 */
bool openblt_process(uint8_t *rom,size_t romlen)
{
    try
    {
        hprintf("openblt:start on 0.0.0.0:%d\r\n",(int)BOOT_COM_NET_PORT);

        BootInit();

        while(!openblt_load_rom_ok)
        {
            BootTask();
        }

        if(server_fd!=INVALID_SOCKET)
        {
            closesocket(server_fd);
            server_fd=INVALID_SOCKET;
        }

        if(client_fd!=INVALID_SOCKET)
        {
            closesocket(client_fd);
            client_fd=INVALID_SOCKET;
        }

        if(romlen > sizeof(nvm_rom))
        {
            romlen=sizeof(nvm_rom);
        }

        if(rom!=NULL)
        {
            memcpy(rom,nvm_rom,romlen);
        }
        hprintf("openblt:load ok!\r\n");
    }
    catch(...)
    {
        hprintf("openblt:exited!\r\n");
        return false;
    }

    return true;
}

