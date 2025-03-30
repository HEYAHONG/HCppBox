#include "hbox.h"
#include <functional>
#include <string>
#include <fstream>

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
static int cmd_rom(int argc,const char *argv[]);
static int cmd_dump_core(int argc,const char *argv[]);
static int cmd_dump_ram(int argc,const char *argv[]);
static int cmd_dump_xram(int argc,const char *argv[]);
static int cmd_dump_rom(int argc,const char *argv[]);
static int cmd_dump_pin(int argc,const char *argv[]);
static int cmd_disassembly(int argc,const char *argv[]);
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
        "clock tick. \ntick [number]"
    },
    {
        cmd_uart,
        "uart",
        "uart input. \nuart [string]"
    },
    {
        cmd_rom,
        "rom",
        "show rom list or set rom.\nrom [name]"
    },
    {
        cmd_dump_core,
        "dump_core",
        "show PC and SFR"
    },
    {
        cmd_dump_ram,
        "dump_ram",
        "show ram. \ndump_ram [address(hex)] [length]"
    },
    {
        cmd_dump_xram,
        "dump_xram",
        "show xram. \ndump_xram [address(hex)] [length]"
    },
    {
        cmd_dump_rom,
        "dump_rom",
        "show rom. \ndump_rom [address(hex)] [length]"
    },
    {
        cmd_disassembly,
        "disassembly",
        "show rom(disassembly). \ndisassembly [address(hex)] [length]"
    },
    {
        cmd_dump_pin,
        "dump_pin",
        "show pin. "
    },
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
            hshell_printf(hshell_ctx,"%c",(uint8_t)(*data));
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
        hshell_command_name_shortcut_set(&simmcs51_ctx,true);
        hshell_echo_set(&simmcs51_ctx, hshell_echo_get(hshell_ctx));
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

static struct
{
    const char *name;
    const hs_mcs_51_rom_t *rom;
} rom_list[]=
{
    {
        "helloworld",
        &hs_mcs_51_rom_helloworld
    },
    {
        "helloworld_stdio",
        &hs_mcs_51_rom_helloworld_stdio
    }
};
static int cmd_rom(int argc,const char *argv[])
{
    hshell_context_t * hshell_ctx=hshell_context_get_from_main_argv(argc,argv);
    if(argc==1)
    {
        for(size_t i=0; i<sizeof(rom_list)/sizeof(rom_list[0]); i++)
        {
            if(rom_list[i].name!=NULL)
            {
                hshell_printf(hshell_ctx,"%d\t%s\r\n",(int)i,rom_list[i].name);
            }
        }
    }
    if(argc>=2)
    {
        bool is_rom_set=false;
        if(!is_rom_set)
        {
            for(size_t i=0; i<sizeof(rom_list)/sizeof(rom_list[0]); i++)
            {
                if(strcmp(argv[1],rom_list[i].name)==0)
                {
                    s_mcs51.rom_set(*rom_list[i].rom);
                    is_rom_set=true;
                    hshell_printf(hshell_ctx,"%s loaded\r\n",rom_list[i].name!=NULL?rom_list[i].name:"");
                    s_mcs51.reset();
                    break;
                }
            }
        }
        if(!is_rom_set)
        {
            try
            {
                size_t pos=0;
                size_t index=std::stoull(argv[1],&pos);
                if(pos!=0 && index < (sizeof(rom_list)/sizeof(rom_list[0])))
                {
                    s_mcs51.rom_set(*rom_list[index].rom);
                    is_rom_set=true;
                    hshell_printf(hshell_ctx,"%s loaded\r\n",rom_list[index].name!=NULL?rom_list[index].name:"");
                    s_mcs51.reset();
                }
            }
            catch(...)
            {

            }
        }
        if(!is_rom_set)
        {
            std::string filename(argv[1]);
            if(filename.find(".bin")==(filename.length()-strlen(".bin")))
            {
                std::ifstream file;
                static std::string file_bin;
                file.open(filename);
                if(file.is_open())
                {
                    file_bin.clear();
                    file >> file_bin;
                    hs_mcs_51_rom_t rom= {0};
                    rom.psbank_addr=HS_MCS_51_ROM_PSBANK_C8051F120_SFR_ADDRESS;
                    rom.code=(uint8_t *)file_bin.c_str();
                    rom.len=file_bin.length();
                    s_mcs51.rom_set(rom);
                    is_rom_set=true;
                    hshell_printf(hshell_ctx,"%s loaded\r\n",argv[1]);
                    s_mcs51.reset();
                    file.close();
                }
            }
        }
        if(!is_rom_set)
        {
            hshell_printf(hshell_ctx,"%s not found\r\n",argv[1]);
        }
    }
    return 0;
}

static int cmd_dump_core(int argc,const char *argv[])
{
    hshell_context_t * hshell_ctx=hshell_context_get_from_main_argv(argc,argv);
    {
        hshell_printf(hshell_ctx,"PC=%04X\r\n",(int)hs_mcs_51_pc_get(s_mcs51.core_get()));
        hshell_printf(hshell_ctx,"\r\nSFR:\r\n");
        for(size_t i=0x80; i<0x100; i++)
        {
            if((i%16)==0)
            {
                hshell_printf(hshell_ctx,"%02X:\t",i);
            }
            uint8_t data=0;
            hs_mcs_51_sfr_read(s_mcs51.core_get(),(hs_mcs_51_sfr_addr_t)i,&data);
            hshell_printf(hshell_ctx,"%02X ",data);
            if((i%16)==15)
            {
                hshell_printf(hshell_ctx,"\r\n");
            }
        }
    }
    return 0;
}

static int cmd_dump_ram(int argc,const char *argv[])
{
    hshell_context_t * hshell_ctx=hshell_context_get_from_main_argv(argc,argv);
    size_t start=0;
    if(argc > 1)
    {
        try
        {
            start=std::stoull(argv[1],NULL,16);
        }
        catch(...)
        {

        }
    }
    size_t length=128;
    if(argc > 2)
    {
        try
        {
            length=std::stoull(argv[2]);
        }
        catch(...)
        {

        }
    }
    if(start >= 0x100)
    {
        start=0xFF;
    }
    if(start+length > 0x100)
    {
        length=0x100-start;
    }
    if(length==0)
    {
        length=1;
    }
    {
        hshell_printf(hshell_ctx,"\r\nRAM:\r\n");
        for(size_t i=start; i<start+length; i++)
        {
            if((i%16)==0)
            {
                hshell_printf(hshell_ctx,"%02X:\t",i);
            }
            uint8_t data=0;
            hs_mcs_51_ram_model_huge_ram_read(s_mcs51.ram_get(),i,&data);
            hshell_printf(hshell_ctx,"%02X ",data);
            if((i%16)==15)
            {
                hshell_printf(hshell_ctx,"\r\n");
            }
        }
    }
    return 0;
}

static int cmd_dump_xram(int argc,const char *argv[])
{
    hshell_context_t * hshell_ctx=hshell_context_get_from_main_argv(argc,argv);
    size_t start=0;
    if(argc > 1)
    {
        try
        {
            start=std::stoull(argv[1],NULL,16);
        }
        catch(...)
        {

        }
    }
    size_t length=256;
    if(argc > 2)
    {
        try
        {
            length=std::stoull(argv[2]);
        }
        catch(...)
        {

        }
    }
    if(start >= 0x10000)
    {
        start=0xFFFF;
    }
    if(start+length > 0x10000)
    {
        length=0x10000-start;
    }
    if(length==0)
    {
        length=1;
    }
    {
        hshell_printf(hshell_ctx,"\r\nXRAM:\r\n");
        for(size_t i=start; i<start+length; i++)
        {
            if((i%16)==0)
            {
                hshell_printf(hshell_ctx,"%04X:\t",i);
            }
            uint8_t data=0;
            hs_mcs_51_ram_model_huge_xram_read(s_mcs51.ram_get(),i,&data);
            hshell_printf(hshell_ctx,"%02X ",data);
            if((i%16)==15)
            {
                hshell_printf(hshell_ctx,"\r\n");
            }
        }
    }
    return 0;
}

static int cmd_dump_rom(int argc,const char *argv[])
{
    hshell_context_t * hshell_ctx=hshell_context_get_from_main_argv(argc,argv);
    size_t start=0;
    if(argc > 1)
    {
        try
        {
            start=std::stoull(argv[1],NULL,16);
        }
        catch(...)
        {

        }
    }
    size_t length=256;
    if(argc > 2)
    {
        try
        {
            length=std::stoull(argv[2]);
        }
        catch(...)
        {

        }
    }
    if(length==0)
    {
        length=1;
    }
    {
        hshell_printf(hshell_ctx,"\r\nROM:\r\n");
        for(size_t i=start; i<start+length; i++)
        {
            if((i%16)==0)
            {
                hshell_printf(hshell_ctx,"%05X:\t",i);
            }
            uint8_t data=0;
            hs_mcs_51_rom_read(s_mcs51.rom_get(),i,&data);
            hshell_printf(hshell_ctx,"%02X ",data);
            if((i%16)==15)
            {
                hshell_printf(hshell_ctx,"\r\n");
            }
        }
    }
    return 0;
}

static int cmd_dump_pin(int argc,const char *argv[])
{
    hshell_context_t * hshell_ctx=hshell_context_get_from_main_argv(argc,argv);
    for(size_t i=(size_t)HS_MCS_51_PIN_PORT_0; i <= ((size_t)HS_MCS_51_PIN_PORT_3); i++)
    {
        hshell_printf(hshell_ctx,"P%d=%02X\t",i,hs_mcs_51_pin_port_get(s_mcs51.core_get(),s_mcs51.pin_get(),(hs_mcs_51_pin_port_t)i));
        for(size_t j=0; j<8; j++)
        {
            hshell_printf(hshell_ctx,"%d ",hs_mcs_51_pin_port_pin_get(s_mcs51.core_get(),s_mcs51.pin_get(),(hs_mcs_51_pin_port_t)i,j)?0x01:0x00);
        }
        hshell_printf(hshell_ctx,"\r\n");
    }
    return 0;
}

static int cmd_disassembly(int argc,const char *argv[])
{
    hshell_context_t * hshell_ctx=hshell_context_get_from_main_argv(argc,argv);
    size_t start=0;
    size_t length=25;
    {
        //默认地址由PC决定
        start=hs_mcs_51_pc_get(s_mcs51.core_get());
        if(start >= length/2)
        {
            start-=length/2;
        }
        else
        {
            start=0;
        }
    }
    if(argc > 1)
    {
        try
        {
            start=std::stoull(argv[1],NULL,16);
        }
        catch(...)
        {

        }
    }
    if(argc > 2)
    {
        try
        {
            length=std::stoull(argv[2]);
        }
        catch(...)
        {

        }
    }
    if(length==0)
    {
        length=1;
    }
    {
        for(size_t i=start; i<start+length;)
        {
            uint8_t instruction=0;
            if(i==hs_mcs_51_pc_get(s_mcs51.core_get()))
            {
                hshell_printf(hshell_ctx,"> ");
            }
            else
            {
                hshell_printf(hshell_ctx,"  ");
            }
            hshell_printf(hshell_ctx,"%05X:\t",i);
            hs_mcs_51_rom_read(s_mcs51.rom_get(),i,&instruction);
            size_t len=hs_mcs_51_disassembly_instruction_length(&instruction);
            const hs_mcs_51_disassembly_instruction_t * disasm=hs_mcs_51_disassembly_instruction_table_get(&instruction);
            if(disasm!=NULL && disasm->mnemonic!=NULL)
            {
                std::string mnemonic;
                uint8_t instruction_array[3];
                for(size_t j=i; j<i+len; j++)
                {
                    hs_mcs_51_rom_read(s_mcs51.rom_get(),j,&instruction_array[j-i]);
                }
                hs_mcs_51_disassembly_print(instruction_array,len,[](const char ch,void *usr)
                {
                    if(usr!=NULL)
                    {
                        std::string &mnemonic=*(std::string *)usr;
                        char buff[2]= {ch,0};
                        mnemonic+=buff;
                    }
                },&mnemonic);
                hshell_printf(hshell_ctx,"%-16s ",mnemonic.c_str());
            }
            for(size_t j=i; j<(i+len); j++)
            {
                uint8_t data=0;
                hs_mcs_51_rom_read(s_mcs51.rom_get(),j,&data);
                hshell_printf(hshell_ctx,"%02X ",data);
            }
            i+=len;
            hshell_printf(hshell_ctx,"\r\n");
        }
    }
    return 0;
}
