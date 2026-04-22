#include "hbox.h"
#include "HCPPBox.h"
#include "hrc.h"
#include "vector"
#include "string.h"
#include "fstream"
#include "algorithm"
#include "thread"
#include "chrono"
#include H3RDPARTY_ARGTABLE3_HEADER


static void show_banner()
{
    const uint8_t * banner=RCGetHandle("banner");
    if(banner!=NULL)
    {
        hprintf("%s",(const char *)banner);
    }
}


static std::string filename="Image";
static void check_args(int argc,char *argv[])
{
    struct arg_lit  * help=NULL;
    struct arg_lit  * decompress=NULL;
    struct arg_file *file=NULL;
    void *argtable[]=
    {
        help=arg_lit0("h","help",                                      "print this help and exit"),
        file=arg_file0(NULL,NULL,"<file>",                              "image file name(default:Image)"),
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
void console_init(void)
{
    system("");
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
    write(fileno(stdout), (char*)&txchar, sizeof(txchar));
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

hminirv32ima_machine_default64mb_t machine;

static void run_vm(int argc,char *argv[])
{
    console_init();
    bool vm_running=true;

    hminirv32ima_machine_default64mb_init(&machine);

    machine.console_has_data=[](const struct hminirv32ima_machine_default64mb *machine)->bool
    {
        return console_iskbhit();
    };

    machine.console_put_data=[](const struct hminirv32ima_machine_default64mb *machine,uint8_t data)->void
    {
        console_output(data);
    };

    machine.console_get_data=[](const struct hminirv32ima_machine_default64mb *machine)->uint8_t
    {
        return console_keyboard_input();
    };

    while(vm_running)
    {
        hminirv32ima_machine_default64mb_reset(&machine);

        {
            std::fstream file;
            file.open(filename.c_str(),std::ios::in|std::ios::binary);
            if(!file.is_open())
            {
                hfprintf(stderr,"open %s failed!\r\n",filename.c_str());
                hexit(-1);
            }

            file.read((char *)machine.ram,sizeof(machine.ram));

            file.close();
        }

        bool cpu_cycle=true;
        uint64_t time_us=0;
        {
            htimespec_t ts= {0};
            hclock_gettime(HCLOCK_MONOTONIC,&ts);
            time_us=ts.tv_sec*1000000+ts.tv_nsec/1000;
        }
        while(cpu_cycle)
        {
            uint64_t time_us_now=0;
            {
                htimespec_t ts= {0};
                hclock_gettime(HCLOCK_MONOTONIC,&ts);
                time_us_now=ts.tv_sec*1000000+ts.tv_nsec/1000;
            }
            int code=hminirv32ima_machine_default64mb_step(&machine,time_us_now-time_us,1000);
            time_us=time_us_now;
            switch(code)
            {
            case 0:
            {
                //正常执行代码
            }
            break;
            case 1:
            {
                //WFI
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
            break;
            case 0x5555:
            {
                //关机
                cpu_cycle=false;
                vm_running=false;
            }
            break;
            case 0x7777:
            {
                //重启
                cpu_cycle=false;
                vm_running=true;
            }
            break;
            default:
            {

            }
            break;
            }
        }

    }
}

int main(int argc,char *argv[])
{
    check_args(argc,argv);

    run_vm(argc,argv);

    return 0;
}
