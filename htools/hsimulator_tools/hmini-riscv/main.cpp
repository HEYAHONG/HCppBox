#include "hbox.h"
#include "HCPPBox.h"
#include "hrc.h"
#include "vector"
#include "string"
#include "fstream"
#include "algorithm"
#include "thread"
#include "chrono"
#include "cstring"
#include "cstdlib"
#include "cstdint"
#include "cctype"
#include <cstdio>
#include "inttypes.h"
#include H3RDPARTY_ARGTABLE3_HEADER
#include "hsimulator/risc-v/risc-v_machine_rv32.h"
#include "hsimulator/risc-v/risc-v_machine_rv64.h"

static void show_banner()
{
    const uint8_t * banner=RCGetHandle("banner");
    if(banner!=NULL)
    {
        hprintf("%s",(const char *)banner);
    }
    fflush(stdout);
}

/*
 * 控制台
 */
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
            Mode |= (ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
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


static uint64_t now_us(void)
{
    htimespec_t ts= {0};
    hclock_gettime(HCLOCK_MONOTONIC,&ts);
    return (uint64_t)ts.tv_sec*1000000ULL+(uint64_t)ts.tv_nsec/1000ULL;
}

/*
 * 镜像文件
 */
static std::vector<uint8_t> g_rom;
static std::vector<uint8_t> g_flash;
static std::vector<uint8_t> g_ram;

static const uint32_t RAM_BASE=0x20000000UL;
static const uint32_t ROM_BASE=0xD0000000UL;
static const uint32_t REGION_SIZE=2UL*1024*1024;

template<typename MEM>
static size_t rom_read(const MEM *mem,uint32_t address,void *data,size_t len)
{
    (void)mem;
    size_t offset=0;
    if(address<0x10000000UL && address<g_rom.size())
    {
        offset=address;
    }
    else if(address>=ROM_BASE && (uint64_t)(address-ROM_BASE)<g_rom.size())
    {
        offset=(size_t)(address-ROM_BASE);
    }
    else
    {
        return 0;
    }
    if((uint64_t)offset+len>g_rom.size())
    {
        return 0;
    }
    memcpy(data,g_rom.data()+offset,len);
    return len;
}

template<typename MEM>
static size_t rom_write(const MEM *mem,uint32_t address,const void *data,size_t len)
{
    (void)mem;
    (void)address;
    (void)data;
    (void)len;
    return 0;
}

template<typename MEM>
static size_t flash_read(const MEM *mem,uint32_t address,void *data,size_t len)
{
    (void)mem;
    size_t offset=0;
    if(address<0x10000000UL && address<g_flash.size())
    {
        offset=address;
    }
    else if(address>=0x10000000UL && (uint64_t)(address-0x10000000UL)<g_flash.size())
    {
        offset=(size_t)(address-0x10000000UL);
    }
    else
    {
        return 0;
    }
    if((uint64_t)offset+len>g_flash.size())
    {
        return 0;
    }
    memcpy(data,g_flash.data()+offset,len);
    return len;
}

template<typename MEM>
static size_t flash_write(const MEM *mem,uint32_t address,const void *data,size_t len)
{
    (void)mem;
    (void)address;
    (void)data;
    (void)len;
    return 0;
}

template<typename MEM>
static size_t ram_read(const MEM *mem,uint32_t address,void *data,size_t len)
{
    (void)mem;
    if(address<RAM_BASE || (uint64_t)address+len>(uint64_t)RAM_BASE+g_ram.size())
    {
        return 0;
    }
    memcpy(data,g_ram.data()+(address-RAM_BASE),len);
    return len;
}

template<typename MEM>
static size_t ram_write(const MEM *mem,uint32_t address,const void *data,size_t len)
{
    (void)mem;
    if(address<RAM_BASE || (uint64_t)address+len>(uint64_t)RAM_BASE+g_ram.size())
    {
        return 0;
    }
    memcpy(g_ram.data()+(address-RAM_BASE),data,len);
    return len;
}

template<typename M>
static bool console_put_cb(M *machine,void *usr,uint8_t data)
{
    (void)machine;
    (void)usr;
    console_output(data);
    return true;
}

template<typename M>
static bool console_get_cb(M *machine,void *usr,uint8_t *data)
{
    (void)machine;
    (void)usr;
    if(data==NULL || !console_iskbhit())
    {
        return false;
    }
    *data=console_keyboard_input();
    return true;
}

template<typename M>
static bool console_has_data_cb(M *machine,void *usr)
{
    (void)machine;
    (void)usr;
    return console_iskbhit();
}

/*
 * 机器模型操作
 */
template<typename M>
struct machine_ops;

template<>
struct machine_ops<hs_risc_v_machine_rv32_t>
{
    using machine_t=hs_risc_v_machine_rv32_t;
    using memory_t=hs_risc_v_machine_rv32_memory_t;
    static size_t size(void)
    {
        return hs_risc_v_machine_rv32_size();
    }
    static machine_t *init(void *mem)
    {
        return hs_risc_v_machine_rv32_init(mem);
    }
    static bool check(machine_t *machine)
    {
        return hs_risc_v_machine_rv32_check(machine);
    }
    static int step(machine_t *machine,uint32_t elapsed_us,size_t count)
    {
        return hs_risc_v_machine_rv32_step(machine,elapsed_us,count);
    }
    static void wfi_clear(machine_t *machine)
    {
        hs_risc_v_core_rv32_wfi_clear(hs_risc_v_machine_rv32_core(machine));
    }
};

template<>
struct machine_ops<hs_risc_v_machine_rv64_t>
{
    using machine_t=hs_risc_v_machine_rv64_t;
    using memory_t=hs_risc_v_machine_rv64_memory_t;
    static size_t size(void)
    {
        return hs_risc_v_machine_rv64_size();
    }
    static machine_t *init(void *mem)
    {
        return hs_risc_v_machine_rv64_init(mem);
    }
    static bool check(machine_t *machine)
    {
        return hs_risc_v_machine_rv64_check(machine);
    }
    static int step(machine_t *machine,uint32_t elapsed_us,size_t count)
    {
        return hs_risc_v_machine_rv64_step(machine,elapsed_us,count);
    }
    static void wfi_clear(machine_t *machine)
    {
        hs_risc_v_core_rv64_wfi_clear(hs_risc_v_machine_rv64_core(machine));
    }
};

static bool load_file(const std::string &path,std::vector<uint8_t> &data)
{
    std::ifstream file;
    file.open(path.c_str(),std::ios::in|std::ios::binary);
    if(!file.is_open())
    {
        return false;
    }
    file.seekg(0,std::ios::end);
    std::streamoff length=file.tellg();
    if(length<0)
    {
        return false;
    }
    file.seekg(0,std::ios::beg);
    data.resize((size_t)length);
    if(length>0)
    {
        file.read((char *)data.data(),length);
    }
    file.close();
    return true;
}

static bool parse_size(const std::string &text,size_t *size)
{
    if(text.empty() || size==NULL)
    {
        return false;
    }
    char *end=NULL;
    unsigned long long value=strtoull(text.c_str(),&end,0);
    if(end==text.c_str())
    {
        return false;
    }
    while(*end==' ')
    {
        end++;
    }
    if(*end=='k' || *end=='K')
    {
        value*=1024ULL;
        end++;
    }
    else if(*end=='m' || *end=='M')
    {
        value*=1024ULL*1024ULL;
        end++;
    }
    else if(*end=='g' || *end=='G')
    {
        value*=1024ULL*1024ULL*1024ULL;
        end++;
    }
    if(*end!='\0')
    {
        return false;
    }
    *size=(size_t)value;
    return true;
}

static std::string g_arch="rv32";
static std::string g_rom_filename;
static std::string g_flash_filename;
static size_t g_ram_size=1024*1024;
static uint32_t g_entry=0;

static void check_args(int argc,char *argv[])
{
    struct arg_lit  * help=NULL;
    struct arg_str  * arch=NULL;
    struct arg_file * rom=NULL;
    struct arg_file * flash=NULL;
    struct arg_str  * ram_size=NULL;
    struct arg_str  * entry=NULL;
    void *argtable[]=
    {
        help=arg_lit0("h","help",                                      "print this help and exit"),
        arch=arg_str0("a","arch","<rv32|rv64>",                        "architecture(default:rv32)"),
        rom=arg_file0("r","rom","<file>",                              "rom image file(required)"),
        flash=arg_file0("f","flash","<file>",                          "flash image file(optional)"),
        ram_size=arg_str0("m","ram-size","<size>",                     "ram size, min 16KB(default:1M)"),
        entry=arg_str0("e","entry","<addr>",                           "entry address, 0=reset vector(default:0)"),
        arg_end(20)
    };
    if(arg_nullcheck(argtable)!=0)
    {
        hfputs("arg_nullcheck error!\r\n",stderr);
        hexit(-1);
    }

    auto print_help=[&](FILE *stream)
    {
        hfprintf(stream,"Usage:%s [options]\r\n",argv[0]);
        arg_print_glossary(stream,argtable,"  %-25s %s\n");
    };

    if(arg_parse(argc,argv,argtable)>0)
    {
        hfputs("arg_parse error!\r\n",stderr);
        print_help(stderr);
        hexit(-1);
    }

    if(help->count>0)
    {
        print_help(stdout);
        hexit(0);
    }

    if(arch->count>0)
    {
        g_arch=std::string(arch->sval[0]);
        std::transform(g_arch.begin(),g_arch.end(),g_arch.begin(),::tolower);
        if(g_arch!="rv32" && g_arch!="rv64")
        {
            hfprintf(stderr,"invalid arch: %s\r\n",arch->sval[0]);
            print_help(stderr);
            hexit(-1);
        }
    }

    if(rom->count>0)
    {
        g_rom_filename=std::string(rom->filename[0]);
    }
    if(flash->count>0)
    {
        g_flash_filename=std::string(flash->filename[0]);
    }

    if(ram_size->count>0)
    {
        if(!parse_size(ram_size->sval[0],&g_ram_size))
        {
            hfprintf(stderr,"invalid ram size: %s\r\n",ram_size->sval[0]);
            print_help(stderr);
            hexit(-1);
        }
        if(g_ram_size<16UL*1024)
        {
            hfprintf(stderr,"ram size must be at least 16KB\r\n");
            print_help(stderr);
            hexit(-1);
        }
        if(g_ram_size>(size_t)256UL*1024*1024)
        {
            hfprintf(stderr,"ram size must not exceed 256MB\r\n");
            print_help(stderr);
            hexit(-1);
        }
    }

    if(entry->count>0)
    {
        char *end=NULL;
        unsigned long long value=strtoull(entry->sval[0],&end,0);
        if(end==entry->sval[0] || *end!='\0' || value>0xFFFFFFFFULL)
        {
            hfprintf(stderr,"invalid entry address: %s\r\n",entry->sval[0]);
            print_help(stderr);
            hexit(-1);
        }
        g_entry=(uint32_t)value;
    }

    if(g_rom_filename.empty())
    {
        hfprintf(stderr,"must specify rom file!\r\n");
        print_help(stderr);
        hexit(-1);
    }

    arg_freetable(argtable,sizeof(argtable)/sizeof(argtable[0]));
}

template<typename M>
static int run_vm(void)
{
    using ops=machine_ops<M>;
    std::vector<uint8_t> machine_mem(ops::size());
    M *machine=ops::init(machine_mem.data());
    if(machine==NULL)
    {
        hfprintf(stderr,"machine init failed!\r\n");
        return -1;
    }

    machine->ram.base=RAM_BASE;
    machine->ram.size=(uint32_t)g_ram.size();
    machine->ram.read=ram_read<typename ops::memory_t>;
    machine->ram.write=ram_write<typename ops::memory_t>;

    machine->rom.base=ROM_BASE;
    machine->rom.size=REGION_SIZE;
    machine->rom.read=rom_read<typename ops::memory_t>;
    machine->rom.write=rom_write<typename ops::memory_t>;

    if(!g_flash.empty())
    {
        machine->flash.base=0x00000000UL;
        machine->flash.size=REGION_SIZE;
        machine->flash.read=flash_read<typename ops::memory_t>;
        machine->flash.write=flash_write<typename ops::memory_t>;
    }
    else
    {
        machine->flash.base=0;
        machine->flash.size=0;
        machine->flash.read=NULL;
        machine->flash.write=NULL;
    }

    machine->entry=g_entry;
    machine->hart_id=0;
    machine->dtb_address=0;
    machine->console_put=console_put_cb<M>;
    machine->console_get=console_get_cb<M>;
    machine->console_has_data=console_has_data_cb<M>;

    if(!ops::check(machine))
    {
        hfprintf(stderr,"machine check failed!\r\n");
        return -1;
    }

    console_init();

    uint64_t last_us=now_us();
    while(true)
    {
        uint64_t current_us=now_us();
        uint64_t elapsed_us=current_us-last_us;
        last_us=current_us;
        if(elapsed_us>UINT32_MAX)
        {
            elapsed_us=UINT32_MAX;
        }

        int ret=ops::step(machine,(uint32_t)elapsed_us,10000);
        if(ret==1)
        {
            ops::wfi_clear(machine);
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        else if(ret<0)
        {
            hfprintf(stderr,"machine step failed!\r\n");
            return -1;
        }
    }

    return 0;
}

int main(int argc,char *argv[])
{
    show_banner();
    check_args(argc,argv);

    if(!load_file(g_rom_filename,g_rom))
    {
        hfprintf(stderr,"open %s failed!\r\n",g_rom_filename.c_str());
        return -1;
    }
    if(g_rom.empty())
    {
        hfprintf(stderr,"rom file %s is empty!\r\n",g_rom_filename.c_str());
        return -1;
    }

    if(!g_flash_filename.empty())
    {
        if(!load_file(g_flash_filename,g_flash))
        {
            hfprintf(stderr,"open %s failed!\r\n",g_flash_filename.c_str());
            return -1;
        }
        if(g_flash.empty())
        {
            hfprintf(stderr,"flash file %s is empty!\r\n",g_flash_filename.c_str());
            return -1;
        }
    }

    g_ram.resize(g_ram_size,0);

    if(g_arch=="rv64")
    {
        return run_vm<hs_risc_v_machine_rv64_t>();
    }
    return run_vm<hs_risc_v_machine_rv32_t>();
}
