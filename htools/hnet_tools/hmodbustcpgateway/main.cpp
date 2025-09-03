#include "HCPPBox.h"
#include "hrc.h"
#include "libserialport.h"
#include <event2/event.h>
#include <event2/listener.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/util.h>
#ifdef HDEFAULTS_OS_UNIX
#include <signal.h>
#endif // HDEFAULTS_OS_UNIX
#include "inttypes.h"
#include H3RDPARTY_ARGTABLE3_HEADER



static std::string service_port="502";
static std::string service_addr="0.0.0.0";
static std::string service_com_port;
static struct sp_port_config * service_com_config=NULL;
static int service_com_block_time_ms=100;
static class main_cleanup
{
public:
    main_cleanup()
    {

    }
    ~ main_cleanup()
    {
        if(service_com_config!=NULL)
        {
            sp_free_config(service_com_config);
        }
    }

} g_main_cleanup;

enum
{
    SERVICE_LOG_LEVEL_INFO=0,
    SERVICE_LOG_LEVEL_ERROR=1
};

int service_log(int level,const char *format,...)
{
    int ret=0;
    va_list va;
    va_start(va,format);
    if(SERVICE_LOG_LEVEL_INFO==level)
    {
        hvfprintf(stdout,format,va);
    }
    if(SERVICE_LOG_LEVEL_ERROR==level)
    {
        hvfprintf(stderr,format,va);
    }
    va_end(va);
    return ret;
}

#define SERVICE_LOG(level,format,...)  service_log(level,"[%" PRIu64 "]\t" format,(uint64_t)time(NULL),##__VA_ARGS__)
#define SERVICE_LOG_INFO(format,...) SERVICE_LOG(SERVICE_LOG_LEVEL_INFO,format,##__VA_ARGS__)
#define SERVICE_LOG_ERROR(format,...) SERVICE_LOG(SERVICE_LOG_LEVEL_ERROR,format,##__VA_ARGS__)

static void init_serialport()
{
    struct sp_port ** port_list=NULL;
    sp_list_ports(&port_list);
    if(port_list!=NULL)
    {
        for(struct sp_port **port=port_list; (*port)!=NULL; port++)
        {
            const char *port_name=sp_get_port_name(*port);
            if(port_name!=NULL)
            {
                service_com_port=port_name;
            }
        }
        sp_free_port_list(port_list);
    }

    sp_new_config(&service_com_config);

    if(service_com_config==NULL)
    {
        SERVICE_LOG_ERROR("sp_new_config() failed!\r\n");
        hexit(-1);
    }

    sp_set_config_baudrate(service_com_config,115200);
    sp_set_config_bits(service_com_config,8);
    sp_set_config_parity(service_com_config,SP_PARITY_NONE);
    sp_set_config_stopbits(service_com_config,1);
    sp_set_config_flowcontrol(service_com_config,SP_FLOWCONTROL_NONE);
}

static void show_banner()
{
    const char * banner=(const char *)RCGetHandle((const char *)"banner");
    if(banner!=NULL)
    {
        SERVICE_LOG_INFO("\r\n%s\r\n",banner);
    }
}

void check_env()
{
    {
        const char *value=hgetenv("MBTCP_ADDR");
        if(value!=NULL && strlen(value) > 0)
        {
            service_addr=value;
        }
    }
    {
        const char *value=hgetenv("MBTCP_PORT");
        if(value!=NULL && strlen(value) > 0)
        {
            service_port=value;
        }
    }
    {
        const char *value=hgetenv("MBTCP_SERIALPORT");
        if(value!=NULL && strlen(value) > 0)
        {
            service_com_port=value;
        }
    }
    {
        const char *value=hgetenv("MBTCP_SERIALPORT_BAUDRATE");
        if(value!=NULL && strlen(value) > 0)
        {
            int baudrate=atoi(value);
            if(baudrate > 0)
            {
                sp_set_config_baudrate(service_com_config,baudrate);
            }
        }
    }
    {
        const char *value=hgetenv("MBTCP_SERIALPORT_DATABITS");
        if(value!=NULL && strlen(value) > 0)
        {
            int databits=atoi(value);
            if(databits > 0)
            {
                sp_set_config_baudrate(service_com_config,databits);
            }
        }
    }
    {
        const char *value=hgetenv("MBTCP_SERIALPORT_PARITY");
        if(value!=NULL && strlen(value) > 0)
        {
            enum sp_parity parity=SP_PARITY_NONE;
            switch(value[0])
            {
            case 'N':
            {
                parity=SP_PARITY_NONE;
            }
            break;
            case 'O':
            {
                parity=SP_PARITY_ODD;
            }
            break;
            case 'E':
            {
                parity=SP_PARITY_EVEN;
            }
            break;
            case 'S':
            {
                parity=SP_PARITY_SPACE;
            }
            break;
            case 'M':
            {
                parity=SP_PARITY_MARK;
            }
            break;
            default:
            {
            }
            break;
            }
            sp_set_config_parity(service_com_config,parity);
        }
    }
    {
        const char *value=hgetenv("MBTCP_SERIALPORT_STOPBITS");
        if(value!=NULL && strlen(value) > 0)
        {
            int stopbits=atoi(value);
            if(stopbits > 0)
            {
                sp_set_config_baudrate(service_com_config,stopbits);
            }
        }
    }
    {
        const char *value=hgetenv("MBTCP_SERIALPORT_FLOWCONTROL");
        if(value!=NULL && strlen(value) > 0)
        {
            enum sp_flowcontrol flowcontrol=SP_FLOWCONTROL_NONE;
            switch(value[0])
            {
            case 'N':
            {
                flowcontrol=SP_FLOWCONTROL_NONE;
            }
            break;
            case 'R':
            {
                flowcontrol=SP_FLOWCONTROL_RTSCTS;
            }
            break;
            case 'D':
            {
                flowcontrol=SP_FLOWCONTROL_DTRDSR;
            }
            break;
            case 'X':
            {
                flowcontrol=SP_FLOWCONTROL_XONXOFF;
            }
            break;
            default:
            {

            }
            break;
            }
            sp_set_config_flowcontrol(service_com_config,flowcontrol);
        }
    }

}

static void check_args(int argc,char *argv[])
{
    struct arg_lit  * help=NULL;
    struct arg_str  * addr=NULL;
    struct arg_str  * port=NULL;
    struct arg_str  * com_port=NULL;
    struct arg_int  * com_port_baudrate=NULL;
    struct arg_int  * com_port_databits=NULL;
    struct arg_str  * com_port_parity=NULL;
    struct arg_int  * com_port_stopbits=NULL;
    struct arg_str  * com_port_flowcontrol=NULL;
    struct arg_int  * com_block_time=NULL;
    void *argtable[]=
    {
        addr=arg_str0("B","bind","0.0.0.0","bind address"),
        port=arg_str0("P","port","502","bind port"),
        com_port=arg_str0("S","serial",service_com_port.c_str(),"serialport name"),
        com_port_baudrate=arg_int0("R","baudrate","115200","serialport baudrate"),
        com_port_databits=arg_int0("D","databits","8","serialport databits"),
        com_port_parity=arg_str0("P","parity","N","serialport parity(N/E/O/M/S)"),
        com_port_stopbits=arg_int0("T","stopbits","1","serialport stopbits"),
        com_port_flowcontrol=arg_str0("F","flowcontrol","N","serialport flowcontrol(N/R/D/X)"),
        com_block_time=arg_int0("B","block","100","serialport block time(ms)"),
        help=arg_lit0("H","help","print this help and exit"),
        arg_end(20)
    };
    if(arg_nullcheck(argtable)!=0)
    {
        SERVICE_LOG_ERROR("arg_nullcheck error!\r\n");
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
        service_addr=addr->sval[0];
    }

    if(port->count > 0)
    {
        service_port=port->sval[0];
    }

    if(com_port->count>0)
    {
        service_com_port=com_port->sval[0];
    }


    if(com_port_baudrate->count>0)
    {
        sp_set_config_baudrate(service_com_config,com_port_baudrate->ival[0]);
    }

    if(com_port_databits->count>0)
    {
        sp_set_config_bits(service_com_config,com_port_databits->ival[0]);
    }

    if(com_port_parity->count>0)
    {
        enum sp_parity parity=SP_PARITY_NONE;
        switch(com_port_parity->sval[0][0])
        {
        case 'N':
        {
            parity=SP_PARITY_NONE;
        }
        break;
        case 'O':
        {
            parity=SP_PARITY_ODD;
        }
        break;
        case 'E':
        {
            parity=SP_PARITY_EVEN;
        }
        break;
        case 'S':
        {
            parity=SP_PARITY_SPACE;
        }
        break;
        case 'M':
        {
            parity=SP_PARITY_MARK;
        }
        break;
        default:
        {
        }
        break;
        }
        sp_set_config_parity(service_com_config,parity);
    }

    if(com_port_stopbits->count>0)
    {
        sp_set_config_stopbits(service_com_config,com_port_stopbits->ival[0]);
    }

    if(com_port_flowcontrol->count>0)
    {
        enum sp_flowcontrol flowcontrol=SP_FLOWCONTROL_NONE;
        switch(com_port_flowcontrol->sval[0][0])
        {
        case 'N':
        {
            flowcontrol=SP_FLOWCONTROL_NONE;
        }
        break;
        case 'R':
        {
            flowcontrol=SP_FLOWCONTROL_RTSCTS;
        }
        break;
        case 'D':
        {
            flowcontrol=SP_FLOWCONTROL_DTRDSR;
        }
        break;
        case 'X':
        {
            flowcontrol=SP_FLOWCONTROL_XONXOFF;
        }
        break;
        default:
        {

        }
        break;
        }
        sp_set_config_flowcontrol(service_com_config,flowcontrol);
    }

    if(com_block_time->count>0)
    {
        service_com_block_time_ms=com_block_time->ival[0];
    }

    arg_freetable(argtable,sizeof(argtable)/sizeof(argtable[0]));
}


static void bufferevent_read_callback(struct bufferevent *bev, void *ctx)
{
    if(bev==NULL)
    {
        return;
    }
    uint8_t buffer[MODBUS_TCP_MAX_ADU_LENGTH];
    size_t buffer_length=bufferevent_read(bev,buffer,sizeof(buffer));
    if(buffer_length > 0)
    {
        modbus_tcp_gateway_server_context_t mb_ctx=modbus_tcp_gateway_server_context_default();
        mb_ctx.usr=bev;
        mb_ctx.reply=[](modbus_tcp_gateway_server_context_t* ctx,const uint8_t *adu,size_t adu_length)
        {
            if(ctx==NULL || ctx->usr==NULL)
            {
                return;
            }
            struct bufferevent *bev=(struct bufferevent *)ctx->usr;
            bufferevent_write(bev,adu,adu_length);
        };
        mb_ctx.rtu_request=[](modbus_tcp_gateway_server_context_t *ctx,const uint8_t *adu,size_t adu_length,bool (*rtu_reply)(modbus_tcp_gateway_server_context_t* ctx,const uint8_t *adu,size_t adu_length))->bool
        {
            if(ctx==NULL || ctx->usr==NULL || adu==NULL || adu_length==0)
            {
                return false;
            }

            struct sp_port *port=NULL;
            if(sp_get_port_by_name(service_com_port.c_str(),&port)!=SP_OK)
            {
                SERVICE_LOG_ERROR("sp_get_port_by_name() failed!\r\n");
                return false;
            }

            if(port==NULL)
            {
                return false;
            }

            if(sp_open(port,SP_MODE_READ_WRITE)!=SP_OK)
            {
                SERVICE_LOG_ERROR("sp_open() failed!\r\n");
                sp_free_port(port);
                return false;
            }

            if(sp_set_config(port,service_com_config)!=SP_OK)
            {
                SERVICE_LOG_ERROR("sp_ser_config() failed!\r\n");
                sp_close(port);
                sp_free_port(port);
                return false;
            }

            sp_flush(port,SP_BUF_BOTH);

            size_t byte_time_us=0;
            {
                int baudrate=115200;
                sp_get_config_baudrate(service_com_config,&baudrate);
                int bits=8;
                sp_get_config_bits(service_com_config,&bits);
                int stop_bits=1;
                sp_get_config_stopbits(service_com_config,&stop_bits);
                byte_time_us=1000000/(baudrate/(1+bits+stop_bits));
                if(byte_time_us < 1000)
                {
                    /*
                     * 至少1ms
                     */
                    byte_time_us=1000;
                }
            }

            if(adu_length!=sp_blocking_write(port,adu,adu_length,service_com_block_time_ms+adu_length*byte_time_us/1000))
            {
                sp_close(port);
                sp_free_port(port);
                return false;
            }

            {
                char str[MODBUS_RTU_MAX_ADU_LENGTH*2+1]={0};
                hbase16_encode(str,sizeof(str),adu,adu_length);
                SERVICE_LOG_INFO("ModbusRTU TX:%s\r\n",str);
            }

            uint8_t buffer[MODBUS_RTU_MAX_ADU_LENGTH]= {0};
            int bytes_read=0;
            if((bytes_read=sp_blocking_read(port,buffer,sizeof(buffer),service_com_block_time_ms+sizeof(buffer)*byte_time_us/1000)) <=0)
            {
                sp_close(port);
                sp_free_port(port);
                return false;
            }

            {
                char str[MODBUS_RTU_MAX_ADU_LENGTH*2+1]={0};
                hbase16_encode(str,sizeof(str),buffer,bytes_read);
                SERVICE_LOG_INFO("ModbusRTU RX:%s\r\n",str);
            }

            sp_close(port);
            sp_free_port(port);

            if(rtu_reply!=NULL)
            {
                return rtu_reply(ctx,buffer,bytes_read);
            }
            return false;
        };
        modbus_tcp_gateway_server_parse_input(&mb_ctx,buffer,buffer_length);
    }
}

static void bufferevent_event_callback(struct bufferevent *bev, short what, void *ctx)
{
    if(bev==NULL)
    {
        return;
    }
    if((what & BEV_EVENT_ERROR)!=0 || (what &BEV_EVENT_EOF)!=0)
    {
        SERVICE_LOG_INFO("connection %p close!\r\n",bev);
        bufferevent_free(bev);
    }
}

static void evconnlistener_callback(struct evconnlistener *lev, evutil_socket_t fd, struct sockaddr *sock, int socklen, void *ptr)
{
    struct event_base *base=evconnlistener_get_base(lev);
    struct bufferevent *bufferevt=bufferevent_socket_new(base,fd,BEV_OPT_CLOSE_ON_FREE);
    SERVICE_LOG_INFO("connection %p(fd=%d) open!\r\n",bufferevt,fd);
    if(bufferevt!=NULL)
    {
        bufferevent_setcb(bufferevt,bufferevent_read_callback,NULL,bufferevent_event_callback,NULL);
        bufferevent_enable(bufferevt,EV_READ);
    }
};


#ifdef HDEFAULTS_OS_UNIX
static void signal_callback(evutil_socket_t sig, short events, void *user_data)
{
    struct event_base *base = (struct event_base *)user_data;
    struct timeval delay = { 1, 0 };//设置延迟时间 2s
    event_base_loopexit(base, &delay);//延时 2s 退出
}
#endif

int service_main()
{
    HCPPSocketInit();
    SERVICE_LOG_INFO("service_main started!\r\n");
    struct event_base *base = event_base_new();
    if(base==NULL)
    {
        SERVICE_LOG_ERROR("event_base_new() failed!\r\n");
        return -1;
    }

    struct evconnlistener *lev=NULL;

    if(lev==NULL)
    {
        struct addrinfo hints = { 0 };
        struct addrinfo* ai_result = NULL;
        hints.ai_family = AF_INET6;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE;
        hints.ai_protocol = IPPROTO_TCP;
        if(getaddrinfo(service_addr.c_str(), service_port.c_str(), &hints, &ai_result)==0)
        {
            if(ai_result!=NULL)
            {
                lev=evconnlistener_new_bind(base,evconnlistener_callback,NULL,LEV_OPT_CLOSE_ON_FREE,100,(struct sockaddr *)ai_result->ai_addr,ai_result->ai_addrlen);
                freeaddrinfo(ai_result);
            }
        }
    }

    if(lev==NULL)
    {
        struct addrinfo hints = { 0 };
        struct addrinfo* ai_result = NULL;
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE;
        hints.ai_protocol = IPPROTO_TCP;
        if(getaddrinfo(service_addr.c_str(), service_port.c_str(), &hints, &ai_result)==0)
        {
            if(ai_result!=NULL)
            {
                lev=evconnlistener_new_bind(base,evconnlistener_callback,NULL,LEV_OPT_CLOSE_ON_FREE,100,(struct sockaddr *)ai_result->ai_addr,ai_result->ai_addrlen);
                freeaddrinfo(ai_result);
            }
        }
    }

    if(lev==NULL)
    {
        SERVICE_LOG_ERROR("evconnlistener_new_bind() failed!\r\n");
        event_base_free(base);
        return -1;
    }

    evconnlistener_enable(lev);

#ifdef HDEFAULTS_OS_UNIX
    struct event *signal_event=evsignal_new(base, SIGINT, signal_callback, (void *)base);
    if(signal_event!=NULL)
    {
        event_add(signal_event, NULL);
    }
#endif

    event_base_dispatch(base);

#ifdef HDEFAULTS_OS_UNIX
    if(signal_event!=NULL)
    {
        evsignal_del(signal_event);
        signal_event=NULL;
    }
#endif

    if(lev!=NULL)
    {
        evconnlistener_free(lev);
        lev=NULL;
    }

    if(base!=NULL)
    {
        event_base_free(base);
        base=NULL;
    }

    SERVICE_LOG_INFO("service_main exited normally!\r\n");

    return 0;
}

int main(int argc,char *argv[])
{
    init_serialport();

    check_env();

    check_args(argc,argv);

    show_banner();

    return service_main();
}
