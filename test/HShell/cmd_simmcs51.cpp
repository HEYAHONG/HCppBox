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
    switch(argc)
    {
    case 1:
    {
        //一个参数，打印帮助
        hshell_printf(hshell_ctx,"simmcs51:\r\n");
        hshell_printf(hshell_ctx,"reset\r\n\t\treset system\r\n");
        hshell_printf(hshell_ctx,"tick [number]\r\n\t\tclock tick\r\n");
        hshell_printf(hshell_ctx,"uart [string]\r\n\t\tuart input\r\n");
    }
    break;
    case 2:
    {
        if(strcmp(argv[1],"reset")==0)
        {
            s_mcs51.reset();
        }

        if(strcmp(argv[1],"tick")==0)
        {
            s_mcs51.tick();
        }

    }
    break;
    case 3:
    {
        if(strcmp(argv[1],"tick")==0)
        {
            try
            {
                size_t cycles=std::stoull(argv[2]);
                s_mcs51.tick(cycles);
            }
            catch(...)
            {

            }
        }

        if(strcmp(argv[1],"uart")==0)
        {
            if(strlen(argv[2])>0)
            {
                uart_buffer+=std::string(argv[2]);
            }
        }
    }
    break;
    default:
    {

    }
    break;
    }
    return ret;
}


