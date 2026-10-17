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
#include "socket_tcp_helper.h"
#include "httpd.h"
#include <chrono>


static std::string server_addr="0.0.0.0";
static std::string server_port="80";
static int io_idle_timeout=5;

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
    struct arg_int  * ioidletimeout=NULL;
    void *argtable[]=
    {
        server=arg_str0("S","server","pool.http.org","http bind server"),
        serverport=arg_str0("p","server port","123","http bind server port"),
        ioidletimeout=arg_int0("I","io-idle-timeout","5","io idle timeout"),
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


    if(server->count > 0)
    {
        server_addr=server->sval[0];
    }

    if(serverport->count > 0)
    {
        server_port=serverport->sval[0];
    }

    if(ioidletimeout->count)
    {
        io_idle_timeout=ioidletimeout->ival[0];
    }

    arg_freetable(argtable,sizeof(argtable)/sizeof(argtable[0]));
}

static std::chrono::time_point<std::chrono::steady_clock> io_active_timepoint;
static int httpd_main(void)
{
    HCPPSocketInit();

    SOCKET server_fd=socket(AF_INET,SOCK_STREAM,0);
    if(server_fd == INVALID_SOCKET)
    {
        hfprintf(stderr,"[httpd] new socket error!\r\n");
        return -1;
    }

    class server_fd_guard
    {
        int &server_fd;
    public:
        server_fd_guard(int &m_server_fd):server_fd(m_server_fd)
        {

        }
        ~server_fd_guard()
        {
            closesocket(server_fd);
            server_fd=INVALID_SOCKET;
        }

    } m_server_fd{server_fd};

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
            hfprintf(stderr,"[httpd] bind socket error!\r\n");
            return -1;
        }
    }

    {
        if(listen(server_fd,128))
        {
            closesocket(server_fd);
            hfprintf(stderr,"[httpd] listen socket error!\r\n");
            return -1;
        }
    }

    {
        int connection_fd=INVALID_SOCKET;
        HCPPSocketAddressIPV4 addr= {0};
#if defined(__socklen_t_defined) || defined(HDEFAULTS_OS_CYGWIN)
        socklen_t socklen=(sizeof(addr));
#else
#if defined(HDEFAULTS_OS_WINDOWS)
        int socklen=(sizeof(addr));
#else
        unsigned int socklen=(sizeof(addr));
#endif
#endif
        while((connection_fd=accept(server_fd,(HCPPSocketAddress *)&addr,&socklen))!=INVALID_SOCKET)
        {
            io_active_timepoint=std::chrono::steady_clock::now();
            {
                char addr_buffer[64]= {0};
                hfprintf(stderr,"[httpd] connection %d (%s:%d) opened!\r\n",connection_fd,inet_ntop(AF_INET,&addr.sin_addr,addr_buffer,sizeof(addr_buffer)),(int)hbe16toh(addr.sin_port));
            }
            tcp_socket_set_nonblock(connection_fd);
            hhttp_tinyserver_connection_t httpd_con;
            hhttp_tinyserver_connection_init(&httpd_con,
                                             httpd_service(),
                                             [](hhttp_tinyserver_connection_t *connection,uint64_t offset,hhttp_string_t part_data)
            {
                io_active_timepoint=std::chrono::steady_clock::now();
                if(connection->usr==NULL)
                {
                    return;
                }
                int &connection_fd=*(int *)connection->usr;
                const char *data=part_data.str;
                size_t len=part_data.str_len;
                if(data==NULL || len == 0)
                {
                    return;
                }
                if(connection_fd!=INVALID_SOCKET)
                {
                    size_t offset=0;
                    do
                    {
                        int sendlen=send(connection_fd,(const char *)(&((uint8_t *)data)[offset]),len-offset,0);
                        if(sendlen>0)
                        {
                            offset+=sendlen;
                        }
                        if(sendlen==0)
                        {
                            //连接已断开
                            offset=len;
                        }
                    }
                    while(offset < len);
                }

            },
            [](hhttp_tinyserver_connection_t *connection)
            {
                if(connection->usr==NULL)
                {
                    return;
                }
                int &connection_fd=*(int *)connection->usr;
                closesocket(connection_fd);
                connection_fd=INVALID_SOCKET;
            },
            &connection_fd);
            while(connection_fd!=INVALID_SOCKET)
            {
                uint8_t buffer[4096];
                int readlen=recv(connection_fd,(char *)buffer,sizeof(buffer),0);
                if(readlen > 0)
                {
                    io_active_timepoint=std::chrono::steady_clock::now();
                    hhttp_tinyserver_connection_input(&httpd_con,buffer,readlen);
                }
                if(readlen == 0 || (readlen < 0 && !tcp_socket_is_egain()))
                {
                    hhttp_tinyserver_connection_cleanup(&httpd_con);
                    closesocket(connection_fd);
                    connection_fd=INVALID_SOCKET;
                }
                if(std::chrono::steady_clock::now()-io_active_timepoint > std::chrono::seconds(io_idle_timeout))
                {
                    hfprintf(stderr,"[httpd] io idle timeout!\r\n");
                    hhttp_tinyserver_connection_cleanup(&httpd_con);
                    closesocket(connection_fd);
                    connection_fd=INVALID_SOCKET;
                }
            }
            hfprintf(stderr,"[httpd] connection closed!\r\n");
        }
    }


    return 0;
}

int main(int argc,char *argv[])
{

    check_args(argc,argv);

    show_banner();

    return httpd_main();
}
