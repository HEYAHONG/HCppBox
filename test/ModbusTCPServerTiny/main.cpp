#include "HCPPBox.h"
#include "hbox.h"
#include <thread>
#include <map>
#include <mutex>
#ifdef HAVE_READLINE
#include <readline/readline.h>
#include <readline/history.h>
#endif

/*
 * tcp_server上下文
 */
modbus_tcp_gateway_server_context_with_modbus_rtu_tiny_t tcp_server_tiny=modbus_tcp_gateway_server_context_with_modbus_rtu_tiny_context_default();


typedef struct
{
    std::map<modbus_data_address_t,bool> coils;
    std::map<modbus_data_address_t,modbus_data_register_t> registers;
} modbus_data_t;
static std::map<modbus_rtu_slave_tiny_context_t*,modbus_data_t> mb_data;
static bool    read_coil(modbus_rtu_slave_tiny_context_t* ctx,modbus_data_address_t addr)
{
    if(mb_data.find(ctx)!=mb_data.end())
    {
        if(mb_data[ctx].coils.find(addr)!=mb_data[ctx].coils.end())
        {
            return mb_data[ctx].coils[addr];
        }
    }
    return false;
}
static bool    read_discrete_input(modbus_rtu_slave_tiny_context_t* ctx,modbus_data_address_t addr)
{
    return !read_coil(ctx,addr);
}
static modbus_data_register_t  read_holding_register(modbus_rtu_slave_tiny_context_t* ctx,modbus_data_address_t addr)
{
    if(mb_data.find(ctx)!=mb_data.end())
    {
        if(mb_data[ctx].registers.find(addr)!=mb_data[ctx].registers.end())
        {
            return mb_data[ctx].registers[addr];
        }
    }
    return 0xDEAD;
}
static modbus_data_register_t  read_input_register(modbus_rtu_slave_tiny_context_t* ctx,modbus_data_address_t addr)
{
    return ~ read_holding_register(ctx, addr);
}
static void    write_coil(modbus_rtu_slave_tiny_context_t* ctx,modbus_data_address_t addr,bool value)
{
    if(mb_data.find(ctx)!=mb_data.end())
    {
        mb_data[ctx].coils[addr]=value;
    }
}
static void    write_holding_register(modbus_rtu_slave_tiny_context_t* ctx,modbus_data_address_t addr,modbus_data_register_t value)
{
    if(mb_data.find(ctx)!=mb_data.end())
    {
        mb_data[ctx].registers[addr]=value;
    }
}
static void modbus_init_ctx(modbus_rtu_slave_tiny_context_t* ctx)
{
    if(ctx==NULL)
    {
        return;
    }
    mb_data[ctx]=modbus_data_t();
    ctx->read_coil=read_coil;
    ctx->read_discrete_input=read_discrete_input;
    ctx->read_holding_register=read_holding_register;
    ctx->read_input_register=read_input_register;
    ctx->write_coil=write_coil;
    ctx->write_holding_register=write_holding_register;
}

static void server_thread()
{
    HCPPSocketAddressIPV4 addr= {0};
    {
        //默认addr为0.0.0.0：502
        addr.sin_family=AF_INET;
        addr.sin_port=htons(502);
    }
    SOCKET server_fd=socket(AF_INET,SOCK_STREAM,0);
    if(server_fd==INVALID_SOCKET)
    {
        hprintf("new socket error!\r\n");
        exit(-1);
    }

    if(bind(server_fd,(HCPPSocketAddress *)&addr,sizeof(addr))!=0)
    {
        hprintf("bind socket error!\r\n");
        closesocket(server_fd);
        exit(-1);
    }
    else
    {
        hprintf("socket on 0.0.0.0:502 bind success!\r\n");
    }

    //默认队列中只有一个客户端
    if(listen(server_fd,1)!=0)
    {
        hprintf("listen socket error!\r\n");
        closesocket(server_fd);
        exit(-1);
    }

    {
        SOCKET connection_fd=INVALID_SOCKET;
        while((connection_fd=accept(server_fd,NULL,0))!=-1)
        {
            hprintf("new connection %d!\r\n",connection_fd);
            {
                //设定接收超时5ms
                struct timeval  tv;
                tv.tv_sec = 0;
                tv.tv_usec = 5*1000;
                setsockopt(connection_fd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof(tv));
            }
            bool is_running=true;
            while(is_running)
            {
                uint8_t packet[MODBUS_TCP_MAX_ADU_LENGTH*2];//缓冲稍大于实际需要
                int recvlen=recv(connection_fd,(char *)packet,sizeof(packet),0);
                if(recvlen>0)
                {
                    //接收到正确的数据
                    auto reply=[](modbus_tcp_gateway_server_context_with_modbus_rtu_tiny_t* ctx,const uint8_t *adu,size_t adu_length)
                    {
                        if(ctx==NULL|| adu==NULL ||adu_length==0)
                        {
                            return;
                        }
                        {
                            hprintf("server send:");
                            for(size_t i=0; i<adu_length; i++)
                            {
                                printf("%02X ",(uint8_t)adu[i]);
                            }
                            hprintf("\r\n");
                        }
                        SOCKET connection_fd=(SOCKET)(intptr_t)ctx->usr;
                        size_t offset=0;
                        do
                        {
                            int sendlen=send(connection_fd,(const char *)(adu+offset),adu_length-offset,0);
                            if(sendlen>0)
                            {
                                offset+=sendlen;
                            }
                            if(sendlen==0)
                            {
                                //连接已断开
                                offset=adu_length;
                            }

                        }
                        while(offset<adu_length);
                    };
                    tcp_server_tiny.reply=reply;
                    tcp_server_tiny.usr=(void *)(intptr_t)connection_fd;
                    {
                        hprintf("server recv:");
                        for(size_t i=0; i<recvlen; i++)
                        {
                            hprintf("%02X ",(uint8_t)packet[i]);
                        }
                        hprintf("\r\n");
                    }
                    modbus_tcp_gateway_server_context_with_modbus_rtu_tiny_parse_input(&tcp_server_tiny,packet,recvlen);
                }
                if(recvlen==0)
                {
                    is_running=false;
                }
                if(recvlen<0)
                {
                    int socket_errno=errno;
#ifdef WIN32
                    if(socket_errno!=EWOULDBLOCK)
#else
                    if(socket_errno!=EAGAIN)
#endif // WIN32
                    {
                        is_running=false;
                    }
                }
            }
            closesocket(connection_fd);
        }
    }

    closesocket(server_fd);
    hprintf("socket exit!\r\n");
    exit(-1);

}


void cmd_help()
{
    hprintf("--------\r\n");
    hprintf("help:\r\n");
    hprintf("\texit\texit the program\r\n");
    hprintf("--------\r\n");
}

#ifndef HAVE_READLINE
static char *readline(const char *prompt)
{
    if(prompt!=NULL)
    {
        hprintf("%s",prompt);
    }
    const size_t max_len=4096;
    char *str=(char *)malloc(max_len);
    return fgets(str,max_len,stdin);
}
#endif // HAVE_READLINE

static std::recursive_mutex printf_lock;
int main()
{
    //初始化套接字
    HCPPSocketInit();

    hprintf_set_callback([](char c)
    {
        std::lock_guard<std::recursive_mutex> lock(printf_lock);
        putchar(c);
    });

    {
        //TODO:初始化modbus上下文
        modbus_init_ctx(&tcp_server_tiny.slave);
    }

    //启动服务线程
    std::thread server(server_thread);
    server.detach();
    {
        //打印帮助
        std::lock_guard<std::recursive_mutex> lock(printf_lock);
        cmd_help();
    }
    while(true)
    {
        char *s=readline("modbus>");
        if(s!=NULL)
        {
            std::lock_guard<std::recursive_mutex> lock(printf_lock);
            //处理命令
            {
                //输入exit退出
                if(strcmp(s,"exit")==0)
                {
                    break;
                }
                //输入help打印帮助
                if(strcmp(s,"help")==0)
                {
                    cmd_help();
                }
            }
#ifdef HAVE_READLINE
            add_history(s);
#endif // HAVE_READLINE
            free(s);
        }
    }
    return 0;
}


