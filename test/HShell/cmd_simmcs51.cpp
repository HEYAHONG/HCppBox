#include "hbox.h"
#include <functional>
#include <string>

class mcs51:public hsimmcs51huge
{
    std::function<bool(hs_mcs_51_serial_t *serial,hs_mcs_51_serial_io_t io_type,uint16_t *data)> on_uart_callback;
public:
    void set_on_uart_callback(std::function<bool(hs_mcs_51_serial_t *serial,hs_mcs_51_serial_io_t io_type,uint16_t *data)> _on_uart_callback)
    {
        on_uart_callback=_on_uart_callback;
    }
    mcs51()
    {
        //默认装载hellworld程序
        rom_set(hs_mcs_51_rom_helloworld_stdio);
    }
    virtual ~mcs51()
    {

    }
protected:
    virtual bool on_uart(hs_mcs_51_serial_t *serial,hs_mcs_51_serial_io_t io_type,uint16_t *data) override
    {
        if(on_uart_callback!=nullptr)
        {
            return on_uart_callback(serial,io_type,data);
        }
        return true;
    }
};

static mcs51 s_mcs51;
static std::string uart_buffer;
static hshell_context_t simmcs51_ctx;
static int cmd_reset(int argc,const char *argv[]);
static int cmd_tick(int argc,const char *argv[]);
static int cmd_uart(int argc,const char *argv[]);
static hshell_command_t commands[]=
{
    {
        cmd_reset,
        "reset",
        "reset machine"
    },
    {
        cmd_tick,
        "tick",
        "clock tick. tick [number]"
    },
    {
        cmd_uart,
        "uart",
        "uart input. uart [string]"
    }
};
extern "C" int command_simmcs51_main(int argc,const char *argv[]);
int command_simmcs51_main(int argc,const char *argv[])
{
    int ret=0;
    hshell_context_t * hshell_ctx=hshell_context_get_from_main_argv(argc,argv);
    s_mcs51.set_on_uart_callback([=](hs_mcs_51_serial_t *serial,hs_mcs_51_serial_io_t io_type,uint16_t *data) -> bool
    {
        if(io_type==HS_MCS_51_SERIAL_IO_TRANSMIT)
        {
            hshell_putchar(hshell_ctx,(uint8_t)(*data));
        }
        if(io_type==HS_MCS_51_SERIAL_IO_TICK)
        {
            if(uart_buffer.length()>0)
            {
                if(hs_mcs_51_serial_status_dataready_set(s_mcs51.core_get(),s_mcs51.serial_get(),uart_buffer.c_str()[0]))
                {
                    uart_buffer=uart_buffer.substr(1);
                }
            }
        }
        return true;
    });
    {
        //配置hshell,并进入子上下文
        hshell_prompt_string_set(&simmcs51_ctx,"simmcs51>");
        hshell_show_banner_set(&simmcs51_ctx,false);
        hshell_command_array_set(&simmcs51_ctx,commands,sizeof(commands)/sizeof(commands[0]));
        hshell_subcontext_enter(hshell_ctx,&simmcs51_ctx);
    }
    return ret;
}

static int cmd_reset(int argc,const char *argv[])
{
    hshell_context_t * hshell_ctx=hshell_context_get_from_main_argv(argc,argv);
    s_mcs51.reset();
    return 0;
}

static int cmd_tick(int argc,const char *argv[])
{
    hshell_context_t * hshell_ctx=hshell_context_get_from_main_argv(argc,argv);
    if(argc==1)
    {
        s_mcs51.tick();
    }
    if(argc>=2)
    {
        size_t cycles=std::stoull(argv[1]);
        s_mcs51.tick(cycles);
    }
    return 0;
}

static int cmd_uart(int argc,const char *argv[])
{
    hshell_context_t * hshell_ctx=hshell_context_get_from_main_argv(argc,argv);
    if(strlen(argv[1])>0)
    {
        uart_buffer+=std::string(argv[1]);
    }
    return 0;
}

