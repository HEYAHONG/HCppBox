#include "HCPPBox.h"
#include "hbox.h"
#include <mutex>
#include "ModbusSocketIo.h"
#ifdef HAVE_READLINE
#include <readline/readline.h>
#include <readline/history.h>
#endif

static ModbusSocketIo Io;

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
static int cmd_con(int argc,const char *argv[]);
static int cmd_discon(int argc,const char *argv[]);
static int cmd_rc(int argc,const char *argv[]);
static int cmd_rdi(int argc,const char *argv[]);
static int cmd_rhr(int argc,const char *argv[]);
static int cmd_rir(int argc,const char *argv[]);
static int cmd_wsc(int argc,const char *argv[]);
static int cmd_wsr(int argc,const char *argv[]);
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
        "con",
        cmd_con,
        "con [host] [port]",
        "connect to ModbusTCPServer"
    }
    ,
    {
        "discon",
        cmd_discon,
        "discon",
        "disconnect"
    }
    ,
    {
        "rc",
        cmd_rc,
        "rc [addr(hex)] [len[dec]]",
        "read coils"
    }
    ,
    {
        "rdi",
        cmd_rdi,
        "rdi [addr(hex)] [len[dec]]",
        "read discrete inputs"
    }
    ,
    {
        "rhr",
        cmd_rhr,
        "rhr [addr(hex)] [len[dec]]",
        "read holding registers"
    }
    ,
    {
        "rir",
        cmd_rir,
        "rir [addr(hex)] [len[dec]]",
        "read input registers"
    }
    ,
    {
        "wsc",
        cmd_wsc,
        "wsc [addr(hex)] [value[hex]]",
        "write single coil"
    }
    ,
    {
        "wsr",
        cmd_wsr,
        "wsr [addr(hex)] [value[hex]]",
        "write single register"
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
    const size_t usage_max_len=32;
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
        hprintf("\r\ncmd %s is not found!\r\n",argv[0]);
    }

}

static int cmd_con(int argc,const char *argv[])
{
    bool ret=false;
    if(argc > 2)
    {
        ret=Io.connect_ipv4(argv[1],argv[2]);
    }
    else if(argc > 1)
    {
        ret=Io.connect_ipv4(argv[1],"502");
    }
    else
    {
        ret=Io.connect_ipv4("localhost","502");
    }
    if(!ret)
    {
        hprintf("connect failed\r\n");
    }
    return 0;
}

static int cmd_discon(int argc,const char *argv[])
{
    bool ret=Io.disconnect();
    if(!ret)
    {
        hprintf("disconnect failed\r\n");
    }
    return 0;
}

static int cmd_rc(int argc,const char *argv[])
{
    if(!Io.IsConnected())
    {
        hprintf("please connect first!\r\n");
        return 0;
    }
    modbus_tcp_client_io_interface_t io=Io.GetIoInterface();
    modbus_io_interface_context_read_coils_t ctx=modbus_io_interface_context_read_coils_default();
    if(argc > 1)
    {
        ctx.starting_address=strtoll(argv[1],NULL,16);
    }
    if(argc > 2)
    {
        ctx.quantity_of_coils=strtoll(argv[2],NULL,10);
    }
    ctx.base.on_exception=[](modbus_io_interface_context_base_t *ctx,uint8_t function_code,uint8_t exception_code)
    {
        (void)ctx;
        hprintf("exception:func=%d,code=%d\r\n",(int)function_code,(int)exception_code);
    };
    ctx.on_read_coils=[](modbus_io_interface_context_read_coils_t *ctx,modbus_data_address_t addr,bool value)
    {
        (void)ctx;
        hprintf("%04x=%d\r\n",(int)addr,(int)(value?0x0001:0x0000));
    };
    if(!modbus_tcp_client_request_gateway(&io,MODBUS_FC_READ_COILS,(modbus_io_interface_context_base_t *)&ctx,sizeof(ctx)))
    {
        hprintf("failed\r\n");
    }
    return 0;
}

static int cmd_rdi(int argc,const char *argv[])
{
    if(!Io.IsConnected())
    {
        hprintf("please connect first!\r\n");
        return 0;
    }
    modbus_tcp_client_io_interface_t io=Io.GetIoInterface();
    modbus_io_interface_context_read_discrete_inputs_t ctx=modbus_io_interface_context_read_discrete_inputs_default();
    if(argc > 1)
    {
        ctx.starting_address=strtoll(argv[1],NULL,16);
    }
    if(argc > 2)
    {
        ctx.quantity_of_inputs=strtoll(argv[2],NULL,10);
    }
    ctx.base.on_exception=[](modbus_io_interface_context_base_t *ctx,uint8_t function_code,uint8_t exception_code)
    {
        (void)ctx;
        hprintf("exception:func=%d,code=%d\r\n",(int)function_code,(int)exception_code);
    };
    ctx.on_read_discrete_inputs=[](modbus_io_interface_context_read_discrete_inputs_t *ctx,modbus_data_address_t addr,bool value)
    {
        (void)ctx;
        hprintf("%04x=%d\r\n",(int)addr,(int)(value?0x0001:0x0000));
    };
    if(!modbus_tcp_client_request_gateway(&io,MODBUS_FC_READ_DISCRETE_INPUTS,(modbus_io_interface_context_base_t *)&ctx,sizeof(ctx)))
    {
        hprintf("failed\r\n");
    }
    return 0;
}

static int cmd_rhr(int argc,const char *argv[])
{
    if(!Io.IsConnected())
    {
        hprintf("please connect first!\r\n");
        return 0;
    }
    modbus_tcp_client_io_interface_t io=Io.GetIoInterface();
    modbus_io_interface_context_read_holding_registers_t ctx=modbus_io_interface_context_read_holding_registers_default();
    if(argc > 1)
    {
        ctx.starting_address=strtoll(argv[1],NULL,16);
    }
    if(argc > 2)
    {
        ctx.quantity_of_registers=strtoll(argv[2],NULL,10);
    }
    ctx.base.on_exception=[](modbus_io_interface_context_base_t *ctx,uint8_t function_code,uint8_t exception_code)
    {
        (void)ctx;
        hprintf("exception:func=%d,code=%d\r\n",(int)function_code,(int)exception_code);
    };
    ctx.on_read_holding_registers=[](modbus_io_interface_context_read_holding_registers_t *ctx,modbus_data_address_t addr,modbus_data_register_t value)
    {
        (void)ctx;
        hprintf("%04x=%04x\r\n",(int)addr,(int)(value));
    };
    if(!modbus_tcp_client_request_gateway(&io,MODBUS_FC_READ_HOLDING_REGISTERS,(modbus_io_interface_context_base_t *)&ctx,sizeof(ctx)))
    {
        hprintf("failed\r\n");
    }
    return 0;
}

static int cmd_rir(int argc,const char *argv[])
{
    if(!Io.IsConnected())
    {
        hprintf("please connect first!\r\n");
        return 0;
    }
    modbus_tcp_client_io_interface_t io=Io.GetIoInterface();
    modbus_io_interface_context_read_input_registers_t ctx=modbus_io_interface_context_read_input_registers_default();
    if(argc > 1)
    {
        ctx.starting_address=strtoll(argv[1],NULL,16);
    }
    if(argc > 2)
    {
        ctx.quantity_of_registers=strtoll(argv[2],NULL,10);
    }
    ctx.base.on_exception=[](modbus_io_interface_context_base_t *ctx,uint8_t function_code,uint8_t exception_code)
    {
        (void)ctx;
        hprintf("exception:func=%d,code=%d\r\n",(int)function_code,(int)exception_code);
    };
    ctx.on_read_input_registers=[](modbus_io_interface_context_read_input_registers_t *ctx,modbus_data_address_t addr,modbus_data_register_t value)
    {
        (void)ctx;
        hprintf("%04x=%04x\r\n",(int)addr,(int)(value));
    };
    if(!modbus_tcp_client_request_gateway(&io,MODBUS_FC_READ_INPUT_REGISTERS,(modbus_io_interface_context_base_t *)&ctx,sizeof(ctx)))
    {
        hprintf("failed\r\n");
    }
    return 0;
}

static int cmd_wsc(int argc,const char *argv[])
{
    if(!Io.IsConnected())
    {
        hprintf("please connect first!\r\n");
        return 0;
    }
    modbus_tcp_client_io_interface_t io=Io.GetIoInterface();
    modbus_io_interface_context_write_single_coil_t ctx=modbus_io_interface_context_write_single_coil_default();
    if(argc > 1)
    {
        ctx.output_address=strtoll(argv[1],NULL,16);
    }
    if(argc > 2)
    {
        ctx.output_value=(strtoll(argv[2],NULL,16)!=0);
    }
    ctx.base.on_exception=[](modbus_io_interface_context_base_t *ctx,uint8_t function_code,uint8_t exception_code)
    {
        (void)ctx;
        hprintf("exception:func=%d,code=%d\r\n",(int)function_code,(int)exception_code);
    };
    ctx.on_write_single_coil=[](modbus_io_interface_context_write_single_coil_t *ctx,modbus_data_address_t addr,modbus_data_register_t value)
    {
        (void)ctx;
        hprintf("%04x=%04x\r\n",(int)addr,(int)(value));
    };
    if(!modbus_tcp_client_request_gateway(&io,MODBUS_FC_WRITE_SINGLE_COIL,(modbus_io_interface_context_base_t *)&ctx,sizeof(ctx)))
    {
        hprintf("failed\r\n");
    }
    return 0;
}

static int cmd_wsr(int argc,const char *argv[])
{
    if(!Io.IsConnected())
    {
        hprintf("please connect first!\r\n");
        return 0;
    }
    modbus_tcp_client_io_interface_t io=Io.GetIoInterface();
    modbus_io_interface_context_write_single_register_t ctx=modbus_io_interface_context_write_single_register_default();
    if(argc > 1)
    {
        ctx.output_address=strtoll(argv[1],NULL,16);
    }
    if(argc > 2)
    {
        ctx.output_value=strtoll(argv[2],NULL,16);
    }
    ctx.base.on_exception=[](modbus_io_interface_context_base_t *ctx,uint8_t function_code,uint8_t exception_code)
    {
        (void)ctx;
        hprintf("exception:func=%d,code=%d\r\n",(int)function_code,(int)exception_code);
    };
    ctx.on_write_single_register=[](modbus_io_interface_context_write_single_register_t *ctx,modbus_data_address_t addr,modbus_data_register_t value)
    {
        (void)ctx;
        hprintf("%04x=%04x\r\n",(int)addr,(int)(value));
    };
    if(!modbus_tcp_client_request_gateway(&io,MODBUS_FC_WRITE_SINGLE_REGISTER,(modbus_io_interface_context_base_t *)&ctx,sizeof(ctx)))
    {
        hprintf("failed\r\n");
    }
    return 0;
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
        //打印信息
        std::lock_guard<std::recursive_mutex> lock(printf_lock);
        banner();
    }
    while(true)
    {
        char *s=readline(Io.IsConnected()?"modbus>":"modbus(not connected)>");
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


