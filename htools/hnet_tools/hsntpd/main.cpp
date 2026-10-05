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


static std::string server_addr="0.0.0.0";
static std::string server_port="123";

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
    struct arg_int  * era=NULL;
    struct arg_str  * server=NULL;
    struct arg_str  * serverport=NULL;
    void *argtable[]=
    {
        era=arg_int0("E","era","0","ntp era"),
        server=arg_str0("S","server","pool.ntp.org","ntp bind server"),
        serverport=arg_str0("p","server port","123","ntp bind server port"),
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

    if(era->count > 0)
    {
        hsntp_era_set(era->ival[0]);
    }

    if(server->count > 0)
    {
        server_addr=server->sval[0];
    }

    if(serverport->count > 0)
    {
        server_port=serverport->sval[0];
    }

    arg_freetable(argtable,sizeof(argtable)/sizeof(argtable[0]));
}


static int ntpd_main(void)
{
    HCPPSocketInit();

    SOCKET server_fd=socket(AF_INET,SOCK_DGRAM,0);
    if(server_fd == INVALID_SOCKET)
    {
        hfprintf(stderr,"[ntp] new socket error!\r\n");
        return -1;
    }

    {
        HCPPSocketAddressIPV4 addr= {0};
        {
            addr.sin_family=AF_INET;
            addr.sin_port=htons(std::stoul(server_port));
        }
        inet_pton(AF_INET,server_addr.c_str(),&addr.sin_addr);
        if(bind(server_fd,(HCPPSocketAddress *)&addr,sizeof(addr))!=0)
        {
            closesocket(server_fd);
            hfprintf(stderr,"[ntp] bind socket error!\r\n");
            return -1;
        }
    }

    hsntp_ntp_server_t ntp;

    hsntp_ntp_server_init(&ntp,
                          [](const hsntp_ntp_server_t *ntp,const uint8_t *packet,size_t packet_size,const void *addr,size_t addr_size)
    {
        if(ntp==NULL)
        {
            return;
        }
        sendto((SOCKET)ntp->usr,(const char *)packet,packet_size,0,(HCPPSocketAddress *)addr,addr_size);
    },
    [](const hsntp_ntp_server_t *ntp,uint8_t *packet,size_t packet_size,void *addr,size_t *addr_size) ->size_t
    {
        if(addr_size==NULL)
        {
            return 0;
        }
#if defined(__socklen_t_defined) || defined(HDEFAULTS_OS_CYGWIN)
        socklen_t socklen=(*addr_size);
#else
#if defined(HDEFAULTS_OS_WINDOWS)
        int socklen=(*addr_size);
#else
        unsigned int socklen=(*addr_size);
#endif
#endif
        int ret=recvfrom((SOCKET)ntp->usr,(char *)packet,packet_size,0,(HCPPSocketAddress *)addr,&socklen);
        if(ret < 0)
        {
            return 0;
        }

        (*addr_size)=socklen;

        return ret;
    },
    [](const hsntp_ntp_server_t *ntp,htimeval_t *tv) -> int
    {
        if(tv==NULL)
        {
            return -1;
        }
        hgettimeofday(tv,NULL);
        hprintf("[ntpd] time=%lu.%06lu\r\n",(unsigned long)tv->tv_sec,(unsigned long)tv->tv_usec);
        return 0;
    },
    (void *)(intptr_t)server_fd
                         );

    while(hsntp_ntp_server_loop(&ntp,NULL,0,NULL,0));

    closesocket(server_fd);

    return 0;
}

int main(int argc,char *argv[])
{

    check_args(argc,argv);

    show_banner();

    return ntpd_main();
}
