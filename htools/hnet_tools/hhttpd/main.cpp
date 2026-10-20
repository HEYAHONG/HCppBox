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
#include "signal_helper.h"
#include "httpd.h"
#include <chrono>
#include <atomic>
#include <chrono>
#include <thread>


static std::string server_addr="0.0.0.0";
static std::string server_port="80";
static int io_idle_timeout=5;
static int max_thread_count=100;

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
    struct arg_str  * indexfilename=NULL;
    struct arg_str  * rootpath=NULL;
    void *argtable[]=
    {
        server=arg_str0("S","server","pool.http.org","http bind server"),
        serverport=arg_str0("p","server port","123","http bind server port"),
        ioidletimeout=arg_int0("I","io-idle-timeout","5","io idle timeout"),
        indexfilename=arg_str0("i","index-filename","index.html","index filename"),
        rootpath=arg_str0("r","root-path",".","root path"),
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

    if(ioidletimeout->count > 0)
    {
        io_idle_timeout=ioidletimeout->ival[0];
    }

    if(indexfilename->count > 0)
    {
        httpd_set_index_file_name(indexfilename->sval[0]);
    }

    if(rootpath->count > 0)
    {
        httpd_set_root_path(rootpath->sval[0]);
    }

    arg_freetable(argtable,sizeof(argtable)/sizeof(argtable[0]));
}

static void check_env(int argc,char *argv[])
{
    {
        const char *hhttpd_port=hgetenv("HTTPD_PORT");
        if(hhttpd_port!=NULL)
        {
            if(strlen(hhttpd_port) > 0)
            {
                server_port=std::string(hhttpd_port);
            }
        }
    }

    {
        const char *hhttpd_idle_timeout=hgetenv("HTTPD_IDLE_TIMEOUT");
        if(hhttpd_idle_timeout!=NULL)
        {
            if(strlen(hhttpd_idle_timeout) > 0)
            {
                io_idle_timeout=atoi(hhttpd_idle_timeout);
                if(io_idle_timeout < 1)
                {
                    io_idle_timeout=1;
                }
            }
        }
    }


    {
        const char *hhttpd_index_filename=hgetenv("HTTPD_INDEX_FILENAME");
        if(hhttpd_index_filename!=NULL)
        {
            if(strlen(hhttpd_index_filename) > 0)
            {
                httpd_set_index_file_name(hhttpd_index_filename);
            }
        }
    }

    {
        const char *hhttpd_root_path=hgetenv("HTTPD_ROOT_PATH");
        if(hhttpd_root_path!=NULL)
        {
            if(strlen(hhttpd_root_path) > 0)
            {
                httpd_set_root_path(hhttpd_root_path);
            }
        }
    }
}

static thread_local std::chrono::time_point<std::chrono::steady_clock> io_active_timepoint;
static std::atomic_int current_thread_count=0;
static bool httpd_running=true;
SOCKET server_fd=INVALID_SOCKET;
static void httpd_stop(void)
{
    httpd_running=false;
    if(server_fd!=INVALID_SOCKET)
    {
        closesocket(server_fd);
        server_fd=INVALID_SOCKET;
    }
}
static int httpd_main(void)
{
    HCPPSocketInit();
    sigint_init();
    sigint_reg(httpd_stop);

    server_fd=socket(AF_INET,SOCK_STREAM,0);
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
            if(server_fd!=INVALID_SOCKET)
            {
                closesocket(server_fd);
                server_fd=INVALID_SOCKET;
            }
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
            while(current_thread_count > max_thread_count)
            {
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
            current_thread_count++;
            std::thread connection_thread([=]()
            {
                int m_connection_fd=connection_fd;
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
                &m_connection_fd);
                while(m_connection_fd!=INVALID_SOCKET)
                {
                    uint8_t buffer[4096];
                    int readlen=recv(m_connection_fd,(char *)buffer,sizeof(buffer),0);
                    if(readlen > 0)
                    {
                        io_active_timepoint=std::chrono::steady_clock::now();
                        hhttp_tinyserver_connection_input(&httpd_con,buffer,readlen);
                    }
                    if(readlen == 0 || (readlen < 0 && !tcp_socket_is_egain()))
                    {
                        hhttp_tinyserver_connection_cleanup(&httpd_con);
                        closesocket(m_connection_fd);
                        m_connection_fd=INVALID_SOCKET;
                    }
                    if(std::chrono::steady_clock::now()-io_active_timepoint > std::chrono::seconds(io_idle_timeout))
                    {
                        hfprintf(stderr,"[httpd] connection %d io idle timeout!\r\n",m_connection_fd);
                        hhttp_tinyserver_connection_cleanup(&httpd_con);
                        closesocket(m_connection_fd);
                        m_connection_fd=INVALID_SOCKET;
                    }
                    if(!httpd_running)
                    {
                        hfprintf(stderr,"[httpd] connection %d stop!\r\n",m_connection_fd);
                        hhttp_tinyserver_connection_cleanup(&httpd_con);
                        closesocket(m_connection_fd);
                        m_connection_fd=INVALID_SOCKET;
                    }
                }
                hfprintf(stderr,"[httpd] connection %d closed!\r\n",connection_fd);
                current_thread_count--;
            });
            connection_thread.detach();
        }
    }


    while(current_thread_count!=0)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}

int main(int argc,char *argv[])
{

    check_env(argc,argv);

    check_args(argc,argv);

    show_banner();

    return httpd_main();
}
