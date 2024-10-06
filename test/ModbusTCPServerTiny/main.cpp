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


static void banner()
{
    hprintf("--------\r\n");
    hprintf("type help to get cmd help!\r\n");
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
    memset(str,0,max_len);
    char *ret=fgets(str,max_len,stdin);
    {
        //去除末尾的空格回车换行
        size_t len=strlen(ret);
        while(ret[len-1]==' ' || ret[len-1]=='\r' ||ret[len-1]=='\n')
        {
            ret[len-1]='\0';
            len=strlen(ret);
        }
    }
    return ret;
}
#endif // HAVE_READLINE

static int cmd_exit(int argc,const char *argv[]);
static int cmd_help(int argc,const char *argv[]);
static struct
{
    const char * cmd;
    int (*cmd_entry)(int argc,const char *argv[]);
    const char * help;
} cmd_list[]=
{
    {
        "help",
        cmd_help,
        "help [cmd]\tget cmd help"
    }
    ,
    {
        "exit",
        cmd_exit,
        "exit\texit the program!"
    }
};

static int cmd_exit(int argc,const char *argv[])
{
    //啥也不做,在主循环中处理退出命令
    return 0;
}

static int cmd_help(int argc,const char *argv[])
{
    if(argc == 1)
    {
        for(size_t i=0; i<sizeof(cmd_list)/sizeof(cmd_list[0]); i++)
        {
            hprintf("%s\t%s\r\n",cmd_list[i].cmd,cmd_list[i].help);
        }
    }
    if(argc > 1)
    {
        for(size_t i=0; i<sizeof(cmd_list)/sizeof(cmd_list[0]); i++)
        {
            if(strcmp(cmd_list[i].cmd,argv[1])==0)
            {
                hprintf("%s\t%s\r\n",cmd_list[i].cmd,cmd_list[i].help);
                return 0;
            }
        }
        hprintf("cmd %s is not found!\r\n",argv[1]);
    }
    return 0;
}
static void execute_line(char *line)
{
    if(line == NULL || line[0] == '\0')
    {
        return;
    }
    const size_t max_argc=32;
    const char *argv[max_argc]= {0};
    size_t argc=0;
    {
        //处理参数
        do
        {
            {
                //去除开头的空格
                while((*line)==' ')
                {
                    line++;
                }
            }
            argv[argc++]=line;
            while((*line)!='\0')
            {
                line++;
                if((*line)==' ')
                {
                    (*line)='\0';
                    line++;
                    break;
                }
            }

        }
        while((*line) !='\0' || argc > max_argc);
        if((*argv[argc-1])=='\0')
        {
            argc--;
        }
    }

    {
        for(size_t i=0; i<sizeof(cmd_list)/sizeof(cmd_list[0]); i++)
        {
            if(strcmp(argv[0],cmd_list[i].cmd)==0)
            {
                if(cmd_list[i].cmd_entry!=NULL)
                {
                    cmd_list[i].cmd_entry(argc,argv);
                }
            }
        }
    }

}

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

    {
        //打印信息
        std::lock_guard<std::recursive_mutex> lock(printf_lock);
        banner();
    }

    {
        //启动服务线程
        std::thread server(server_thread);
        server.detach();
    }

    getchar();
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
                //执行行，将修改字符串s
                execute_line(s);
            }
#ifdef HAVE_READLINE
            add_history(s);
#endif // HAVE_READLINE
            free(s);
        }
    }
    return 0;
}


