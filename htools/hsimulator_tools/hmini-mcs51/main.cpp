#include "hbox.h"
#include "HCPPBox.h"
#include "hrc.h"
#include "vector"
#include "string.h"
#include "fstream"
#include "algorithm"
#include "cctype"
#include "thread"
#include "chrono"
#include H3RDPARTY_ARGTABLE3_HEADER
#include "inttypes.h"


static void show_banner()
{
    const uint8_t * banner=RCGetHandle("banner");
    if(banner!=NULL)
    {
        hprintf("%s",(const char *)banner);
    }
}


static std::string filename="rom.hex";
static void check_args(int argc,char *argv[])
{
    struct arg_lit  * help=NULL;
    struct arg_lit  * display_dtb=NULL;
    struct arg_file *file=NULL;
    struct arg_file *dtbfile=NULL;
    void *argtable[]=
    {
        help=arg_lit0("h","help",                                      "print this help and exit"),
        file=arg_file0(NULL,NULL,"<file>",                             "program name(default:rom.hex)"),
        arg_end(20)
    };
    if(arg_nullcheck(argtable)!=0)
    {
        hfputs("arg_nullcheck error!\r\n",stderr);
        hexit(-1);
    }

    if(arg_parse(argc,argv,argtable)>0)
    {
        hfputs("arg_parse error!\r\n",stderr);
        hfprintf(stderr,"Usage:%s [options] <file>\r\n",argv[0]);
        arg_print_glossary(stderr,argtable,"  %-25s %s\n");
        hexit(-1);
    }

    if(help->count > 0)
    {
        hfprintf(stderr,"Usage:%s [options] <file>\r\n",argv[0]);
        arg_print_glossary(stdout,argtable,"  %-25s %s\n");
        show_banner();
        hexit(-1);
    }

    if(file->count > 0)
    {
        filename=std::string(file->filename[0]);
    }

    arg_freetable(argtable,sizeof(argtable)/sizeof(argtable[0]));

    if(filename.empty())
    {
        hfprintf(stderr,"must specify file!\r\n");
        hexit(-1);
    }
    else
    {
        {
            std::fstream file;
            file.open(filename.c_str(),std::ios::in|std::ios::binary);
            if(!file.is_open())
            {
                hfprintf(stderr,"open %s failed!\r\n",filename.c_str());
                hexit(-1);
            }
            file.close();
        }
    }

}

#if defined(WINDOWS) || defined(WIN32) || defined(_WIN32)
#include <conio.h>
#include <windows.h>
void console_init(void)
{
    /*
     * 启用虚拟终端处理 
     */
#if defined(ENABLE_VIRTUAL_TERMINAL_INPUT)
    {
        HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
        DWORD  Mode = 0;
        if (GetConsoleMode(hStdin, &Mode))
        {
            Mode |= ENABLE_VIRTUAL_TERMINAL_INPUT;
            SetConsoleMode(hStdin, Mode);
        }
    }
#endif
#if defined(ENABLE_VIRTUAL_TERMINAL_PROCESSING)
    {
        HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD  Mode = 0;
        if (GetConsoleMode(hStdout, &Mode))
        {
            Mode |=(ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
            SetConsoleMode(hStdout, Mode);
        }
    }
#endif
}

static bool console_iskbhit(void)
{
    return _kbhit()!=0;
}

static void console_output(uint8_t data)
{
    _putch(data);
}

static uint8_t console_keyboard_input(void)
{
    if(!console_iskbhit())
    {
        return -1;
    }
    // This code is kind of tricky, but used to convert windows arrow keys
    // to VT100 arrow keys.
    static int is_escape_sequence = 0;
    int r;
    if( is_escape_sequence == 1 )
    {
        is_escape_sequence++;
        return '[';
    }

    r = _getch();

    if( is_escape_sequence )
    {
        is_escape_sequence = 0;
        switch( r )
        {
        case 'H':
            return 'A'; // Up
        case 'P':
            return 'B'; // Down
        case 'K':
            return 'D'; // Left
        case 'M':
            return 'C'; // Right
        case 'G':
            return 'H'; // Home
        case 'O':
            return 'F'; // End
        default:
            return r; // Unknown code.
        }
    }
    else
    {
        switch( r )
        {
        case 13:
            return 10; //cr->lf
        case 224:
            is_escape_sequence = 1;
            return 27; // Escape arrow keys
        default:
            return r;
        }
    }
}

#else
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/time.h>

static void reset_console_keyboard(void)
{
    // Re-enable echo, etc. on keyboard.
    struct termios term;
    tcgetattr(0, &term);
    term.c_lflag |= ICANON | ECHO;
    tcsetattr(0, TCSANOW, &term);
}

static void set_console_keyboard(void)
{
    atexit(reset_console_keyboard);
    struct termios term;
    tcgetattr(0, &term);
    term.c_lflag &= ~(ICANON | ECHO); // Disable echo as well
    tcsetattr(0, TCSANOW, &term);
    int flags = fcntl(0, F_GETFL, 0);
    flags |= O_NONBLOCK;
    fcntl(0, F_SETFL, flags);
}

static bool is_eofd=false;
static bool console_iskbhit()
{
    if( is_eofd )
    {
        return false;
    }
    int byteswaiting=0;
    ioctl(fileno(stdin), FIONREAD, &byteswaiting);
    if( !byteswaiting && write( fileno(stdin), 0, 0 ) != 0 )
    {
        is_eofd = true;    // Is end-of-file for
        return false;
    }
    return byteswaiting!=0;
}

static void console_output(uint8_t data)
{
    char txchar = (char)data;
    int txlen=write(fileno(stdout), (char*)&txchar, sizeof(txchar));
    (void)txlen;
}

static uint8_t console_keyboard_input(void)
{
    if( is_eofd )
    {
        return -1;
    }
    char rxchar = 0;
    int rread = read(fileno(stdin), (char*)&rxchar, 1);
    if( rread > 0 )
        return rxchar;
    else
        return -1;

}

static void console_init(void)
{
    setbuf(stdin,NULL);
    setbuf(stdout,NULL);
    set_console_keyboard();
}

#endif

class vm:public hsimmcs51huge
{
    uint64_t timer_escaped_time_ns;
    void init_timer()
    {
        hs_mcs_51_timer_clk_freq_set(timer_get(),1000);
        hs_mcs_51_timer_timer2_enable(timer_get(),true);
    }
    void update_timer(uint64_t escaped_time_ns)
    {
        timer_escaped_time_ns+=escaped_time_ns;
        uint64_t timer_tick_time_ns=1000000000/ hs_mcs_51_timer_clk_freq_get(timer_get());
        while(timer_escaped_time_ns > timer_tick_time_ns)
        {
            timer_escaped_time_ns-=timer_tick_time_ns;
            hs_mcs_51_timer_external_clk_tick(core_get(),timer_get(),true);
        }
    }
    virtual bool on_uart(hs_mcs_51_serial_t *serial,hs_mcs_51_serial_io_t io_type,uint16_t *data)
    {
        if(serial==NULL || serial->usr==NULL)
        {
            return false;
        }
        hsimmcs51huge &parent=*(hsimmcs51huge *)serial->usr;
        switch(io_type)
        {
        case HS_MCS_51_SERIAL_IO_TRANSMIT:       /**< 发送数据,写SBUF时触发*/
        {
            if(data!=NULL)
            {
                console_output((*data));
            }
        }
        break;
        case HS_MCS_51_SERIAL_IO_RECEIVE:
        {
            if(data!=NULL)
            {
                if(console_iskbhit())
                {
                    (*data)=console_keyboard_input();
                }
            }
        }
        break;
        default:
        {
            if(console_iskbhit())
            {
                /*
                 * 自动设置标志
                 */
                hs_mcs_51_serial_status_dataready_set(parent.core_get(),parent.serial_get(),0);
            }
        }
        break;
        }
        return true;
    }
    uint8_t rom[256*1024];
public:
    vm():timer_escaped_time_ns{0},rom{{0}}
    {

    }
    ~vm()
    {

    }
    void vm_init()
    {
        init_timer();
        hs_mcs_51_core_set_1t(core_get(),true);
    }

    void vm_execute(uint64_t escaped_ns,int tick_count)
    {
        update_timer(escaped_ns);

        hs_mcs_51_core_tick(core_get(),tick_count);
    }

    void load_rom(const uint8_t *rom_data,size_t rom_length)
    {
        hs_mcs_51_core_reset(core_get());
        if(rom_data!=NULL && rom_length > 0)
        {
            memcpy(rom,rom_data,rom_length>sizeof(rom)?sizeof(rom):rom_length);
        }
        hs_mcs_51_rom_t rom_mcs51=HS_MCS_51_ROM_INITIALIZER;
        rom_mcs51.code=rom;
        rom_mcs51.len=sizeof(rom);
        rom_set(rom_mcs51);
    }

} g_vm;

static void run_vm(int argc,char *argv[])
{
    /*
     * 初始化控制台
     */
    console_init();
    bool vm_running=true;

    /*
     * 初始化虚拟机
     */
    g_vm.vm_init();

    while(vm_running)
    {
        {
            //加载程序
            std::fstream file;
            bool is_hex=false;
            {
                std::string filename_suffix=filename.substr(filename.length()-3);
                if(filename_suffix.length()>=3)
                {
                    std::transform(filename_suffix.begin(), filename_suffix.end(), filename_suffix.begin(), ::toupper);
                    if(filename_suffix=="HEX" || filename_suffix=="IHX")
                    {
                        is_hex=true;
                    }
                }
            }
            if(is_hex)
            {
                file.open(filename.c_str(),std::ios::in);
            }
            else
            {
                file.open(filename.c_str(),std::ios::in|std::ios::binary);
            }
            if(!file.is_open())
            {
                hfprintf(stderr,"open %s failed!\r\n",filename.c_str());
                hexit(-1);
            }

            std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            uint8_t rom[256*1024]= {0};
            if(is_hex)
            {
                hintelhex_reader reader;
                hintelhex_reader_init(&reader,[](hintelhex_reader_t *reader,hintelhex_reader_operate_t op,uint32_t address,const uint8_t *data,size_t data_len)
                {
                    uint8_t *code=(uint8_t *)reader->usr;
                    if(op==HINTELHEX_READER_OPERATE_DATA)
                    {
                        if(address+data_len < 256*1024)
                        {
                            memcpy(&code[address],data,data_len);
                        }
                    }
                },rom);
                hintelhex_reader_input(&reader,content.c_str(),content.length());
            }
            else
            {
                memcpy(rom,content.c_str(),content.length()>sizeof(rom)?sizeof(rom):content.length());
            }

            g_vm.load_rom(rom,sizeof(rom));

            file.close();
        }

        bool cpu_cycle=true;
        uint64_t time_ns=0;
        {
            htimespec_t ts= {0};
            hclock_gettime(HCLOCK_MONOTONIC,&ts);
            time_ns=ts.tv_sec*1000000000+ts.tv_nsec;
        }
        while(cpu_cycle)
        {
            uint64_t time_ns_now=0;
            {
                htimespec_t ts= {0};
                hclock_gettime(HCLOCK_MONOTONIC,&ts);
                time_ns_now=ts.tv_sec*1000000000+ts.tv_nsec;
            }
            {
                //执行虚拟机
                g_vm.vm_execute(time_ns_now-time_ns,32);
            }
            time_ns=time_ns_now;
        }

    }
}

int main(int argc,char *argv[])
{
    check_args(argc,argv);

    run_vm(argc,argv);

    return 0;
}
