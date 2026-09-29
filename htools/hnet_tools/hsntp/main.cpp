#include "HCPPBox.h"
#include "hrc.h"
#ifdef HDEFAULTS_OS_UNIX
#include <signal.h>
#endif // HDEFAULTS_OS_UNIX
#include "inttypes.h"
#include H3RDPARTY_ARGTABLE3_HEADER
#ifdef HAVE_SYSLOG_H
#include "syslog.h"
#endif // HAVE_SYSLOG_H



static std::string ntp_port="123";
static std::string ntp_addr="0.0.0.0";
static std::string server_addr="pool.ntp.org";

static void show_banner()
{
    const char * banner=(const char *)RCGetHandle((const char *)"banner");
    if(banner!=NULL)
    {
        hprintf("\r\n%s\r\n",banner);
    }
}


static void check_args(int argc,char *argv[])
{
    struct arg_lit  * help=NULL;
    struct arg_str  * addr=NULL;
    struct arg_str  * port=NULL;
    struct arg_int  * era=NULL;
    struct arg_str  * server;
    void *argtable[]=
    {
        addr=arg_str0("B","bind","0.0.0.0","ntp bind address"),
        port=arg_str0("P","port","123","ntp bind port"),
        era=arg_int0("E","era","0","ntp era"),
        server=arg_str0("S","server","pool.ntp.org","ntp server"),
        help=arg_lit0("H","help","print this help and exit"),
        arg_end(20)
    };
    if(arg_nullcheck(argtable)!=0)
    {
        hfprintf(stderr,"arg_nullcheck error!\r\n");
        hexit(-1);
    }

    if(arg_parse(argc,argv,argtable)>0)
    {
        hfputs("arg_parse error!\r\n",stderr);
        hfputs("Usage:\r\n",stderr);
        arg_print_glossary(stderr,argtable,"  %-30s %s\n");
        hexit(-1);
    }

    if(help->count > 0)
    {
        hfputs("Usage:\r\n",stdout);
        arg_print_glossary(stdout,argtable,"  %-30s %s\n");
        hexit(-1);
    }

    if(addr->count > 0)
    {
        ntp_addr=addr->sval[0];
    }

    if(port->count > 0)
    {
        ntp_port=port->sval[0];
    }

    if(era->count > 0)
    {
        hsntp_era_set(era->ival[0]);
    }

    if(server->count > 0)
    {
        server_addr=server->sval[0];
    }


    arg_freetable(argtable,sizeof(argtable)/sizeof(argtable[0]));
}

HCPPSocketAddressIPV4 server_socket_addr;
static bool ntp_server_addr_check(void)
{

    HCPPSocketInit();

    bool ret=false;

    if(!ret)
    {
        ret=HCPPSocketNslookup(server_addr.c_str(),
                               [=](const char* hostname, const char*addr_string, HCPPSocketAddressIPV4* sock_addr,void *usr)
        {
            server_socket_addr=(*sock_addr);
            server_socket_addr.sin_port=htons(123);
            hprintf("[dns] %s:%s\r\n",hostname,addr_string);
        },NULL);
    }

    if(!ret)
    {
        hfprintf(stderr,"[dns] error!\r\n");
    }

    return ret;

}

static int ntp_main(void)
{
    if(!ntp_server_addr_check())
    {
        return -1;
    }

    SOCKET client_fd=socket(AF_INET,SOCK_DGRAM,0);
    if(client_fd == INVALID_SOCKET)
    {
        hfprintf(stderr,"[ntp] new socket error!\r\n");
        return -1;
    }

    {
        HCPPSocketAddressIPV4 addr= {0};
        {
            addr.sin_family=AF_INET;
            addr.sin_port=htons(std::stoul(ntp_port));
        }
        inet_pton(AF_INET,ntp_addr.c_str(),&addr.sin_addr);
        if(bind(client_fd,(HCPPSocketAddress *)&addr,sizeof(addr))!=0)
        {
            closesocket(client_fd);
            hfprintf(stderr,"[ntp] bind socket error!\r\n");
            return -1;
        }
    }

    hsntp_ntp_client_t ntp;

    hsntp_ntp_client_init(&ntp,
                          [](const hsntp_ntp_client_t *ntp,const uint8_t *packet,size_t packet_size)
    {
        if(ntp==NULL)
        {
            return;
        }
        sendto((SOCKET)ntp->usr,(const char *)packet,packet_size,0,(HCPPSocketAddress *)&server_socket_addr,sizeof(server_socket_addr));
    },
    [](const hsntp_ntp_client_t *ntp,uint8_t *packet,size_t packet_size) ->size_t
    {
        HCPPSocketAddressIPV4 peer_addr;
        memset(&peer_addr,0,sizeof(peer_addr));
#if defined(__socklen_t_defined) || defined(HDEFAULTS_OS_CYGWIN)
        socklen_t socklen=sizeof(peer_addr);
#else
#if defined(HDEFAULTS_OS_WINDOWS)
        int socklen=sizeof(peer_addr);
#else
        unsigned int socklen=sizeof(peer_addr);
#endif
#endif
        int ret=recvfrom((SOCKET)ntp->usr,(char *)packet,packet_size,0,(HCPPSocketAddress *)&peer_addr,&socklen);
        if(ret < 0)
        {
            return 0;
        }
        return ret;
    },
    [](const hsntp_ntp_client_t *ntp,const htimeval_t *tv) -> int
    {
        if(tv==NULL)
        {
            return -1;
        }
        hprintf("[ntp] time=%lu.%06lu\r\n",(unsigned long)tv->tv_sec,(unsigned long)tv->tv_usec);
        return 0;
    },
    (void *)(intptr_t)client_fd
                         );

    /*
     * 发送请求
     */
    {
        hsntp_ntp_client_sendpacket(&ntp,NULL,0);
    }

    /*
     * 接收数据包
     */
    {
        hsntp_ntp_client_receivepacket(&ntp,NULL,0);
    }


    closesocket(client_fd);

    return 0;
}

int main(int argc,char *argv[])
{

    check_args(argc,argv);

    show_banner();

    return ntp_main();
}
