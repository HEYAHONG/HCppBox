#include "HCPPBox.h"
#include "hbox.h"
#include <thread>
#include <map>
#include <mutex>
#include <exception>
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
    std::map<modbus_data_address_t,bool> discrete_inputs;
    std::map<modbus_data_address_t,modbus_data_register_t> registers;
    std::map<modbus_data_address_t,modbus_data_register_t> input_registers;
} modbus_data_t;
static std::map<modbus_rtu_slave_tiny_context_t*,modbus_data_t> mb_data;
static std::recursive_mutex mb_data_lock;
static bool    read_coil(modbus_rtu_slave_tiny_context_t* ctx,modbus_data_address_t addr)
{
    std::lock_guard<std::recursive_mutex> lock(mb_data_lock);
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
    std::lock_guard<std::recursive_mutex> lock(mb_data_lock);
    if(mb_data.find(ctx)!=mb_data.end())
    {
        if(mb_data[ctx].discrete_inputs.find(addr)!=mb_data[ctx].discrete_inputs.end())
        {
            return mb_data[ctx].discrete_inputs[addr];
        }
    }
    return !read_coil(ctx,addr);
}
static modbus_data_register_t  read_holding_register(modbus_rtu_slave_tiny_context_t* ctx,modbus_data_address_t addr)
{
    std::lock_guard<std::recursive_mutex> lock(mb_data_lock);
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
    std::lock_guard<std::recursive_mutex> lock(mb_data_lock);
    if(mb_data.find(ctx)!=mb_data.end())
    {
        if(mb_data[ctx].input_registers.find(addr)!=mb_data[ctx].input_registers.end())
        {
            return mb_data[ctx].input_registers[addr];
        }
    }
    return ~ read_holding_register(ctx, addr);
}
static void    write_coil(modbus_rtu_slave_tiny_context_t* ctx,modbus_data_address_t addr,bool value)
{
    std::lock_guard<std::recursive_mutex> lock(mb_data_lock);
    if(mb_data.find(ctx)!=mb_data.end())
    {
        mb_data[ctx].coils[addr]=value;
    }
}
static void    write_holding_register(modbus_rtu_slave_tiny_context_t* ctx,modbus_data_address_t addr,modbus_data_register_t value)
{
    std::lock_guard<std::recursive_mutex> lock(mb_data_lock);
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

static void exit_program(int code)
{
    std::terminate();
}

#ifndef MODBUS_TCP_PORT
#define MODBUS_TCP_PORT 502
#endif // MODBUS_TCP_PORT

static void server_thread()
{
    HCPPSocketAddressIPV4 addr= {0};
    {
        //默认addr为0.0.0.0：502
        addr.sin_family=AF_INET;
        addr.sin_port=htons(MODBUS_TCP_PORT);
    }
    SOCKET server_fd=socket(AF_INET,SOCK_STREAM,0);
    if(server_fd==INVALID_SOCKET)
    {
        hprintf("new socket error!\r\n");
        exit_program(-1);
    }

    if(bind(server_fd,(HCPPSocketAddress *)&addr,sizeof(addr))!=0)
    {
        hprintf("bind socket error!\r\n");
        closesocket(server_fd);
        exit_program(-1);
    }
    else
    {
        hprintf("socket on 0.0.0.0:%d bind success!\r\n",(int)MODBUS_TCP_PORT);
    }

    //默认队列中只有一个客户端
    if(listen(server_fd,1)!=0)
    {
        hprintf("listen socket error!\r\n");
        closesocket(server_fd);
        exit_program(-1);
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
    exit_program(-1);
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
static int cmd_getc(int argc,const char *argv[]);
static int cmd_getdi(int argc,const char *argv[]);
static int cmd_gethr(int argc,const char *argv[]);
static int cmd_getir(int argc,const char *argv[]);
static int cmd_setc(int argc,const char *argv[]);
static int cmd_setdi(int argc,const char *argv[]);
static int cmd_sethr(int argc,const char *argv[]);
static int cmd_setir(int argc,const char *argv[]);
static struct
{
    const char * cmd;
    int (*cmd_entry)(int argc,const char *argv[]);
    const char * usage;
    const char * help;
} cmd_list[]=
{
    {
        "help",
        cmd_help,
        "help [cmd]",
        "get cmd help"
    }
    ,
    {
        "getc",
        cmd_getc,
        "getc  [addr(hex)]",
        "get coil"
    },
    {
        "getdi",
        cmd_getdi,
        "getdi [addr(hex)]",
        "get discrete input"
    }
    ,
    {
        "gethr",
        cmd_gethr,
        "gethr [addr(hex)]",
        "get holding register"
    },
    {
        "getir",
        cmd_getir,
        "getir [addr(hex)]",
        "get input register"
    },
    {
        "setc",
        cmd_setc,
        "setc  [addr(hex)] [value(0 or 1)]",
        "set coil"
    },
    {
        "setdi",
        cmd_setdi,
        "setdi [addr(hex)] [value(0 or 1)]",
        "set discrete input"
    }
    ,
    {
        "sethr",
        cmd_sethr,
        "sethr [addr(hex)] [value(hex)]",
        "set holding register"
    },
    {
        "setir",
        cmd_setir,
        "setir [addr(hex)] [value(hex)]",
        "set input register"
    }
    ,
    {
        "exit",
        cmd_exit,
        "exit",
        "exit the program"
    }

};

static int cmd_exit(int argc,const char *argv[])
{
    //啥也不做,在主循环中处理退出命令
    return 0;
}

static int cmd_help(int argc,const char *argv[])
{
    const size_t cmd_max_len=8;
    const size_t usage_max_len=36;
    {
        //打印标题栏
        for(size_t i=0; i<cmd_max_len; i++)
        {
            hprintf("%s","-");
        }
        hprintf("\t");
        for(size_t i=0; i<usage_max_len; i++)
        {
            hprintf("%s","-");
        }
        hprintf("\r\n");
    }
    if(argc == 1)
    {
        for(size_t i=0; i<sizeof(cmd_list)/sizeof(cmd_list[0]); i++)
        {
            {
                //打印cmd
                hprintf("%s",cmd_list[i].cmd);
                int cmd_pad_len=(int)cmd_max_len-strlen(cmd_list[i].cmd);
                if(cmd_pad_len > 0)
                {
                    for(size_t i=0; i<cmd_pad_len; i++)
                    {
                        hprintf(" ");
                    }
                }
            }
            hprintf("\t");
            {
                //打印usage
                hprintf("%s",cmd_list[i].usage);
                int usage_pad_len=(int)usage_max_len-strlen(cmd_list[i].usage);
                if(usage_pad_len > 0)
                {
                    for(size_t i=0; i<usage_pad_len; i++)
                    {
                        hprintf(" ");
                    }
                }
            }
            hprintf("\t");
            {
                hprintf("%s",cmd_list[i].help);
            }
            hprintf("\r\n");
        }
    }
    if(argc > 1)
    {
        for(size_t i=0; i<sizeof(cmd_list)/sizeof(cmd_list[0]); i++)
        {
            if(strcmp(cmd_list[i].cmd,argv[1])==0)
            {
                {
                    //打印cmd
                    hprintf("%s",cmd_list[i].cmd);
                    int cmd_pad_len=(int)cmd_max_len-strlen(cmd_list[i].cmd);
                    if(cmd_pad_len > 0)
                    {
                        for(size_t i=0; i<cmd_pad_len; i++)
                        {
                            hprintf(" ");
                        }
                    }
                }
                hprintf("\t");
                {
                    //打印usage
                    hprintf("%s",cmd_list[i].usage);
                    int usage_pad_len=(int)usage_max_len-strlen(cmd_list[i].usage);
                    if(usage_pad_len > 0)
                    {
                        for(size_t i=0; i<usage_pad_len; i++)
                        {
                            hprintf(" ");
                        }
                    }
                }
                hprintf("\t");
                {
                    hprintf("%s",cmd_list[i].help);
                }
                hprintf("\r\n");
                return 0;
            }
        }
        hprintf("\r\ncmd %s is not found!\r\n",argv[1]);
    }
    return 0;
}


static int cmd_getc(int argc,const char *argv[])
{
    std::lock_guard<std::recursive_mutex> lock(mb_data_lock);
    {
        modbus_rtu_slave_tiny_context_t*ctx=&tcp_server_tiny.slave;
        if(argc == 1)
        {
            if(mb_data.find(ctx)!=mb_data.end())
            {
                auto data=mb_data[ctx].coils;
                for(auto it=data.begin(); it!=data.end(); it++)
                {
                    hprintf("%04X=%04X\r\n",(int)it->first,it->second?1:0);
                }
            }
        }
        if(argc >= 2)
        {
            modbus_data_address_t addr=strtoll(argv[1],NULL,16);
            if(mb_data.find(ctx)!=mb_data.end())
            {
                auto data=mb_data[ctx].coils;
                if(data.find(addr)!=data.end())
                {
                    hprintf("%04X=%04X\r\n",(int)addr,data[addr]?1:0);
                }
            }

        }
    }
    return 0;
}
static int cmd_getdi(int argc,const char *argv[])
{
    std::lock_guard<std::recursive_mutex> lock(mb_data_lock);
    {
        modbus_rtu_slave_tiny_context_t*ctx=&tcp_server_tiny.slave;
        if(argc == 1)
        {
            if(mb_data.find(ctx)!=mb_data.end())
            {
                auto data=mb_data[ctx].discrete_inputs;
                for(auto it=data.begin(); it!=data.end(); it++)
                {
                    hprintf("%04X=%04X\r\n",(int)it->first,it->second?1:0);
                }
            }
        }
        if(argc >= 2)
        {
            modbus_data_address_t addr=strtoll(argv[1],NULL,16);
            if(mb_data.find(ctx)!=mb_data.end())
            {
                auto data=mb_data[ctx].discrete_inputs;
                if(data.find(addr)!=data.end())
                {
                    hprintf("%04X=%04X\r\n",(int)addr,data[addr]?1:0);
                }
            }

        }
    }
    return 0;
}
static int cmd_gethr(int argc,const char *argv[])
{
    std::lock_guard<std::recursive_mutex> lock(mb_data_lock);
    {
        modbus_rtu_slave_tiny_context_t*ctx=&tcp_server_tiny.slave;
        if(argc == 1)
        {
            if(mb_data.find(ctx)!=mb_data.end())
            {
                auto data=mb_data[ctx].registers;
                for(auto it=data.begin(); it!=data.end(); it++)
                {
                    hprintf("%04X=%04X\r\n",(int)it->first,(int)it->second);
                }
            }
        }
        if(argc >= 2)
        {
            modbus_data_address_t addr=strtoll(argv[1],NULL,16);
            if(mb_data.find(ctx)!=mb_data.end())
            {
                auto data=mb_data[ctx].registers;
                if(data.find(addr)!=data.end())
                {
                    hprintf("%04X=%04X\r\n",(int)addr,(int)data[addr]);
                }
            }

        }
    }
    return 0;
}
static int cmd_getir(int argc,const char *argv[])
{
    std::lock_guard<std::recursive_mutex> lock(mb_data_lock);
    {
        modbus_rtu_slave_tiny_context_t*ctx=&tcp_server_tiny.slave;
        if(argc == 1)
        {
            if(mb_data.find(ctx)!=mb_data.end())
            {
                auto data=mb_data[ctx].input_registers;
                for(auto it=data.begin(); it!=data.end(); it++)
                {
                    hprintf("%04X=%04X\r\n",(int)it->first,(int)it->second);
                }
            }
        }
        if(argc >= 2)
        {
            modbus_data_address_t addr=strtoll(argv[1],NULL,16);
            if(mb_data.find(ctx)!=mb_data.end())
            {
                auto data=mb_data[ctx].input_registers;
                if(data.find(addr)!=data.end())
                {
                    hprintf("%04X=%04X\r\n",(int)addr,data[addr]);
                }
            }

        }
    }
    return 0;
}
static int cmd_setc(int argc,const char *argv[])
{
    std::lock_guard<std::recursive_mutex> lock(mb_data_lock);
    {
        modbus_rtu_slave_tiny_context_t*ctx=&tcp_server_tiny.slave;
        if(argc >= 2)
        {
            modbus_data_address_t addr=strtoll(argv[1],NULL,16);
            modbus_data_register_t value=0;
            if(argc >= 3)
            {
                value=strtoll(argv[2],NULL,16);
            }
            if(mb_data.find(ctx)!=mb_data.end())
            {
                mb_data[ctx].coils[addr]=value!=0;
                auto data=mb_data[ctx].coils;
                {
                    hprintf("%04X=%04X\r\n",(int)addr,data[addr]?1:0);
                }
            }

        }
    }
    return 0;
}
static int cmd_setdi(int argc,const char *argv[])
{
    std::lock_guard<std::recursive_mutex> lock(mb_data_lock);
    {
        modbus_rtu_slave_tiny_context_t*ctx=&tcp_server_tiny.slave;
        if(argc >= 2)
        {
            modbus_data_address_t addr=strtoll(argv[1],NULL,16);
            modbus_data_register_t value=0;
            if(argc >= 3)
            {
                value=strtoll(argv[2],NULL,16);
            }
            if(mb_data.find(ctx)!=mb_data.end())
            {
                mb_data[ctx].discrete_inputs[addr]=value!=0;
                auto data=mb_data[ctx].discrete_inputs;
                {
                    hprintf("%04X=%04X\r\n",(int)addr,data[addr]?1:0);
                }
            }

        }
    }
    return 0;
}
static int cmd_sethr(int argc,const char *argv[])
{
    std::lock_guard<std::recursive_mutex> lock(mb_data_lock);
    {
        modbus_rtu_slave_tiny_context_t*ctx=&tcp_server_tiny.slave;
        if(argc >= 2)
        {
            modbus_data_address_t addr=strtoll(argv[1],NULL,16);
            modbus_data_register_t value=0;
            if(argc >= 3)
            {
                value=strtoll(argv[2],NULL,16);
            }
            if(mb_data.find(ctx)!=mb_data.end())
            {
                mb_data[ctx].registers[addr]=value;
                auto data=mb_data[ctx].registers;
                {
                    hprintf("%04X=%04X\r\n",(int)addr,data[addr]);
                }
            }

        }
    }
    return 0;
}
static int cmd_setir(int argc,const char *argv[])
{
    std::lock_guard<std::recursive_mutex> lock(mb_data_lock);
    {
        modbus_rtu_slave_tiny_context_t*ctx=&tcp_server_tiny.slave;
        if(argc >= 2)
        {
            modbus_data_address_t addr=strtoll(argv[1],NULL,16);
            modbus_data_register_t value=0;
            if(argc >= 3)
            {
                value=strtoll(argv[2],NULL,16);
            }
            if(mb_data.find(ctx)!=mb_data.end())
            {
                mb_data[ctx].input_registers[addr]=value;
                auto data=mb_data[ctx].input_registers;
                {
                    hprintf("%04X=%04X\r\n",(int)addr,data[addr]);
                }
            }

        }
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
        while((*line) !='\0' && argc < max_argc);
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
                    return;
                }
            }
        }
        hprintf("cmd %s is not found!\r\n",argv[0]);
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
#ifdef HAVE_READLINE
            add_history(s);
#endif // HAVE_READLINE
            std::lock_guard<std::recursive_mutex> lock(printf_lock);
            //处理命令
            {
                //输入exit退出
                if(strcmp(s,"exit")==0)
                {
                    free(s);
                    break;
                }
                //执行行，将修改字符串s
                execute_line(s);
            }
            free(s);
        }
    }
    return 0;
}


