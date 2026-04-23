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
#include "inttypes.h"


static void show_banner()
{
    const uint8_t * banner=RCGetHandle("banner");
    if(banner!=NULL)
    {
        hprintf("%s",(const char *)banner);
    }
}


static std::string filename="Image";
static std::string dtbfilename;
static bool displaydtb=false;
static void check_args(int argc,char *argv[])
{
    struct arg_lit  * help=NULL;
    struct arg_lit  * display_dtb=NULL;
    struct arg_file *file=NULL;
    struct arg_file *dtbfile=NULL;
    void *argtable[]=
    {
        help=arg_lit0("h","help",                                      "print this help and exit"),
        display_dtb=arg_lit0("D","displaydtb",                          "display dtb before vm start"),
        file=arg_file0(NULL,NULL,"<file>",                              "image file name(default:Image)"),
        dtbfile=arg_file0("d","dtb","<file>",                           "dtb file name"),
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

    if(display_dtb->count>0)
    {
        displaydtb=true;
    }

    if(file->count > 0)
    {
        filename=std::string(file->filename[0]);
    }

    if(dtbfile->count > 0)
    {
        dtbfilename=std::string(dtbfile->filename[0]);
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


hminirv32ima_machine_default64mb_t machine;

static void dtb_display(void)
{
    hprintf("dtb:\r\n");
    /*
     * 遍历节点
     */

    /*
     * node_ctx[0]: depth
     * node_ctx[1]: #address-cells
     * node_ctx[2]: #size-cells
     * node_ctx[3]: blank_space
     */
    intptr_t node_ctx[8]= {0};
    node_ctx[1]=2;
    node_ctx[2]=1;
    hlibfdt_traverse_node(machine.dtb,[](const void *fdt,int offset,const char *name,int depth,void *usr)
    {
        intptr_t *node_ctx=(intptr_t *)usr;
        std::string blank_space;
        for(size_t i=0; i<(depth-1); i++)
        {
            blank_space+="\t";
        }
        if(name==NULL || strlen(name)==0)
        {
            name="/";
        }
        hprintf("%s%s\r\n",blank_space.c_str(),name);
        node_ctx[0]=depth;
        node_ctx[3]=(intptr_t)blank_space.c_str();
        hlibfdt_traverse_node_property(fdt,offset,[](const void *fdt,int offset,const char *name,const uint8_t *value,size_t value_length,void *usr)
        {
            intptr_t *node_ctx=(intptr_t *)usr;
            const char *blank_space=(const char *)node_ctx[3];
            if(strcmp("#address-cells",name)==0)
            {
                uint32_t val=0;
                for(size_t i=0; i<value_length; i++)
                {
                    val <<= 8;
                    val +=  value[i];
                }
                node_ctx[1]=val;
                hprintf( "%s\t%-20s:%" PRIu32 "\r\n",blank_space,name,val);
            }
            else if(strcmp("#size-cells",name)==0)
            {
                uint32_t val=0;
                for(size_t i=0; i<value_length; i++)
                {
                    val <<= 8;
                    val +=  value[i];
                }
                node_ctx[2]=val;
                hprintf( "%s\t%-20s:%" PRIu32 "\r\n",blank_space,name,val);
            }
            else if(strcmp("reg",name)==0)
            {
                hprintf( "%s\t%-20s:",blank_space,name);
                for(size_t i=0; i<value_length; i++)
                {
                    if(i==node_ctx[1]*4)
                    {
                        hprintf("\t");
                    }
                    if(i==(node_ctx[1]*4+node_ctx[2]*4))
                    {
                        hprintf("\t");
                    }
                    hprintf("%02X",(int)value[i]);
                }
                hprintf("\r\n");
            }
            else
            {
                const char *string_data_name_list[]
                {
                    "model",
                    "compatible",
                    "status",
                    "device_type",
                    "mmu-type",
                    "riscv,isa",
                    "bootargs"
                };
                bool is_string_data=false;
                for(size_t i=0; i< sizeof(string_data_name_list)/sizeof(string_data_name_list[0]); i++)
                {
                    if(strcmp(string_data_name_list[i],name)==0)
                    {
                        is_string_data=true;
                        break;
                    }
                }
                hprintf( "%s\t%-20s:",blank_space,name);
                if(is_string_data)
                {
                    size_t string_offset=0;
                    while(string_offset<value_length)
                    {
                        hprintf(" %s",(char *)&value[string_offset]);
                        string_offset+=strlen((char *)&value[string_offset])+1;
                    }
                }
                else
                {
                    for(size_t i=0; i<value_length; i++)
                    {
                        hprintf("%02X",(int)value[i]);
                    }
                }
                hprintf("\r\n");
            }

        },node_ctx);
    },node_ctx);
    hprintf("\r\n");
}


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
            //加载Image
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

        if(!dtbfilename.empty())
        {
            //加载dtb
            std::fstream file;
            file.open(dtbfilename.c_str(),std::ios::in|std::ios::binary);
            if(!file.is_open())
            {
                hfprintf(stderr,"open %s failed!\r\n",dtbfilename.c_str());
                hexit(-1);
            }

            file.read((char *)machine.dtb,sizeof(machine.dtb));

            file.close();
        }

        if(displaydtb)
        {
            dtb_display();
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
