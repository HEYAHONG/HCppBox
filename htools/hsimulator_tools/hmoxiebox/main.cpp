#include "hbox.h"
#include "HCPPBox.h"
#include "hrc.h"
#include "hruntime.h"
#include "vector"
#include "string"
#include "fstream"
#include "algorithm"
#include "cstring"
#include "cstdlib"
#include "cstdint"
#include "cctype"
#include <cstdio>
#include "inttypes.h"
#include H3RDPARTY_ARGTABLE3_HEADER
#include "hsimulator/moxie/moxie_moxiebox.h"
#include "hsimulator/moxie/moxie_moxiebox_el.h"

static std::string g_output_filename;

static void show_banner()
{
    const uint8_t * banner=RCGetHandle("banner");
    FILE *stream=(g_output_filename=="-")?stderr:stdout;
    if(banner!=NULL)
    {
        hfprintf(stream,"%s",(const char *)banner);
    }
    fflush(stream);
}

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

/*
 * 内存中的ELF映像输入。
 */
typedef struct
{
    const uint8_t *data;
    size_t length;
} elf_memory_input_t;

static size_t elf_memory_input_read(helf_file_input_t *input,uintptr_t address,void *buffer,size_t buffer_length)
{
    elf_memory_input_t *ctx=(elf_memory_input_t *)input->usr;
    if(ctx==NULL || ctx->data==NULL || buffer==NULL)
    {
        return 0;
    }
    if(address>=ctx->length)
    {
        return 0;
    }
    size_t available=ctx->length-(size_t)address;
    if(buffer_length>available)
    {
        buffer_length=available;
    }
    memcpy(buffer,ctx->data+address,buffer_length);
    return buffer_length;
}

/*
 * 机器模型操作
 */
template<typename M>
struct machine_ops;

template<>
struct machine_ops<hs_moxie_moxiebox_t>
{
    using machine_t=hs_moxie_moxiebox_t;
    static size_t size(void)
    {
        return hs_moxie_moxiebox_size();
    }
    static machine_t *init(void *mem)
    {
        return hs_moxie_moxiebox_init(mem);
    }
    static void deinit(machine_t *machine)
    {
        hs_moxie_moxiebox_deinit(machine);
    }
    static bool add_memory(machine_t *machine,uint32_t address,void *data,size_t length,bool readonly,const char *name)
    {
        return hs_moxie_moxiebox_add_memory(machine,address,data,length,readonly,name);
    }
    static bool load_elf_with_buffer(machine_t *machine,helf_file_input_t *input_file,void *buffer,size_t length)
    {
        return hs_moxie_moxiebox_load_elf_with_buffer(machine,input_file,buffer,length);
    }
    static void set_entry(machine_t *machine,uint32_t entry)
    {
        hs_moxie_moxiebox_set_entry(machine,entry);
    }
    static size_t prepare_size(machine_t *machine)
    {
        return hs_moxie_moxiebox_prepare_size(machine);
    }
    static bool prepare(machine_t *machine,void *mem,size_t length)
    {
        return hs_moxie_moxiebox_prepare(machine,mem,length);
    }
    static int step(machine_t *machine,size_t count)
    {
        return hs_moxie_moxiebox_step(machine,count);
    }
    static bool stopped(machine_t *machine)
    {
        return hs_moxie_moxiebox_stopped(machine);
    }
    static bool halted(machine_t *machine)
    {
        return hs_moxie_moxiebox_halted(machine);
    }
    static int exit_code(machine_t *machine)
    {
        return hs_moxie_moxiebox_exit_code(machine);
    }
    static bool memory_read(machine_t *machine,uint32_t address,void *data,size_t length)
    {
        return hs_moxie_moxiebox_memory_read(machine,address,data,length);
    }
    static bool return_buffer(machine_t *machine,uint32_t *address,uint32_t *length)
    {
        return hs_moxie_moxiebox_return_buffer(machine,address,length);
    }
};

template<>
struct machine_ops<hs_moxie_moxiebox_el_t>
{
    using machine_t=hs_moxie_moxiebox_el_t;
    static size_t size(void)
    {
        return hs_moxie_moxiebox_el_size();
    }
    static machine_t *init(void *mem)
    {
        return hs_moxie_moxiebox_el_init(mem);
    }
    static void deinit(machine_t *machine)
    {
        hs_moxie_moxiebox_el_deinit(machine);
    }
    static bool add_memory(machine_t *machine,uint32_t address,void *data,size_t length,bool readonly,const char *name)
    {
        return hs_moxie_moxiebox_el_add_memory(machine,address,data,length,readonly,name);
    }
    static bool load_elf_with_buffer(machine_t *machine,helf_file_input_t *input_file,void *buffer,size_t length)
    {
        return hs_moxie_moxiebox_el_load_elf_with_buffer(machine,input_file,buffer,length);
    }
    static void set_entry(machine_t *machine,uint32_t entry)
    {
        hs_moxie_moxiebox_el_set_entry(machine,entry);
    }
    static size_t prepare_size(machine_t *machine)
    {
        return hs_moxie_moxiebox_el_prepare_size(machine);
    }
    static bool prepare(machine_t *machine,void *mem,size_t length)
    {
        return hs_moxie_moxiebox_el_prepare(machine,mem,length);
    }
    static int step(machine_t *machine,size_t count)
    {
        return hs_moxie_moxiebox_el_step(machine,count);
    }
    static bool stopped(machine_t *machine)
    {
        return hs_moxie_moxiebox_el_stopped(machine);
    }
    static bool halted(machine_t *machine)
    {
        return hs_moxie_moxiebox_el_halted(machine);
    }
    static int exit_code(machine_t *machine)
    {
        return hs_moxie_moxiebox_el_exit_code(machine);
    }
    static bool memory_read(machine_t *machine,uint32_t address,void *data,size_t length)
    {
        return hs_moxie_moxiebox_el_memory_read(machine,address,data,length);
    }
    static bool return_buffer(machine_t *machine,uint32_t *address,uint32_t *length)
    {
        return hs_moxie_moxiebox_el_return_buffer(machine,address,length);
    }
};

static std::string g_arch="auto";
static std::string g_elf_filename;
static std::vector<std::string> g_data_filenames;
static bool g_entry_set=false;
static uint32_t g_entry=0;

static void check_args(int argc,char *argv[])
{
    struct arg_lit  * help=NULL;
    struct arg_str  * arch=NULL;
    struct arg_file * elf=NULL;
    struct arg_file * data=NULL;
    struct arg_file * output=NULL;
    struct arg_str  * entry=NULL;
    void *argtable[]=
    {
        help=arg_lit0("h","help",                                      "print this help and exit"),
        arch=arg_str0("a","arch","<auto|be|le>",                       "architecture(default:auto)"),
        elf=arg_file0("f","file","<file>",                             "moxie ELF executable(required)"),
        data=arg_filen("d","data","<file>",0,16,                       "data file(s) loaded after ELF(optional)"),
        output=arg_file0("o","output","<file>",                        "output return buffer to <file>, \"-\" for stdout"),
        entry=arg_str0("e","entry","<addr>",                           "entry address override(default:ELF entry)"),
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
        show_banner();
        print_help(stdout);
        hexit(0);
    }

    if(arch->count>0)
    {
        g_arch=std::string(arch->sval[0]);
        std::transform(g_arch.begin(),g_arch.end(),g_arch.begin(),::tolower);
        if(g_arch!="auto" && g_arch!="be" && g_arch!="le")
        {
            hfprintf(stderr,"invalid arch: %s\r\n",arch->sval[0]);
            print_help(stderr);
            hexit(-1);
        }
    }

    if(elf->count>0)
    {
        g_elf_filename=std::string(elf->filename[0]);
    }
    if(g_elf_filename.empty())
    {
        hfprintf(stderr,"must specify ELF file!\r\n");
        print_help(stderr);
        hexit(-1);
    }

    if(data->count>0)
    {
        for(int i=0;i<data->count;i++)
        {
            g_data_filenames.push_back(std::string(data->filename[i]));
        }
    }

    if(output->count>0)
    {
        g_output_filename=std::string(output->filename[0]);
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
        g_entry_set=true;
    }

    arg_freetable(argtable,sizeof(argtable)/sizeof(argtable[0]));
}

/*
 * 输出虚拟机return缓冲区(特殊寄存器6/7)。
 */
template<typename M>
static int write_return_buffer(M *machine)
{
    if(g_output_filename.empty())
    {
        return 0;
    }
    using ops=machine_ops<M>;
    uint32_t address=0;
    uint32_t length=0;
    if(!ops::return_buffer(machine,&address,&length) || length==0)
    {
        return 0;
    }

    FILE *file=stdout;
    bool close_file=false;
    if(g_output_filename!="-")
    {
        file=fopen(g_output_filename.c_str(),"wb");
        if(file==NULL)
        {
            hfprintf(stderr,"open %s failed!\r\n",g_output_filename.c_str());
            return -1;
        }
        close_file=true;
    }

    uint8_t buffer[256];
    uint32_t written=0;
    while(written<length)
    {
        size_t chunk=length-written;
        if(chunk>sizeof(buffer))
        {
            chunk=sizeof(buffer);
        }
        if(!ops::memory_read(machine,address+written,buffer,chunk))
        {
            hfprintf(stderr,"read return buffer failed!\r\n");
            if(close_file)
            {
                fclose(file);
            }
            return -1;
        }
        if(fwrite(buffer,1,chunk,file)!=chunk)
        {
            hfprintf(stderr,"write return buffer failed!\r\n");
            if(close_file)
            {
                fclose(file);
            }
            return -1;
        }
        written+=(uint32_t)chunk;
    }
    fflush(file);
    if(close_file)
    {
        fclose(file);
    }
    return 0;
}

template<typename M>
static int run_vm(helf_file_input_t *input_file,uint32_t base,size_t span,
                  std::vector<std::vector<uint8_t> > &data_files)
{
    using ops=machine_ops<M>;

    std::vector<uint8_t> machine_mem(ops::size());
    M *machine=ops::init(machine_mem.data());
    if(machine==NULL)
    {
        hfprintf(stderr,"machine init failed!\r\n");
        return -1;
    }

    /*
     * ELF可加载段直接使用用户提供的缓冲区。
     * image必须在machine使用期间保持有效。
     */
    std::vector<uint8_t> image(span);
    if(!ops::load_elf_with_buffer(machine,input_file,image.data(),image.size()))
    {
        hfprintf(stderr,"ELF load failed!\r\n");
        ops::deinit(machine);
        return -1;
    }

    if(g_entry_set)
    {
        ops::set_entry(machine,g_entry);
    }

    /*
     * 数据文件加载在ELF之后的地址。
     */
    const uint64_t page_size=4096ULL;
    uint64_t next_address=(uint64_t)base+span;
    for(size_t i=0;i<data_files.size();i++)
    {
        if(data_files[i].empty())
        {
            continue;
        }
        next_address=(next_address+page_size-1)&(~(page_size-1));
        next_address+=page_size;
        if(next_address+data_files[i].size()>0x100000000ULL)
        {
            hfprintf(stderr,"data file %zu address overflow!\r\n",i);
            ops::deinit(machine);
            return -1;
        }
        char name[32];
        snprintf(name,sizeof(name),"data%zu",i);
        if(!ops::add_memory(machine,(uint32_t)next_address,data_files[i].data(),data_files[i].size(),true,name))
        {
            hfprintf(stderr,"data file %zu load failed!\r\n",i);
            ops::deinit(machine);
            return -1;
        }
        next_address+=data_files[i].size();
    }

    size_t prepare_size=ops::prepare_size(machine);
    if(prepare_size==0)
    {
        hfprintf(stderr,"prepare size is zero!\r\n");
        ops::deinit(machine);
        return -1;
    }
    std::vector<uint8_t> prepare_mem(prepare_size);
    if(!ops::prepare(machine,prepare_mem.data(),prepare_mem.size()))
    {
        hfprintf(stderr,"prepare failed!\r\n");
        ops::deinit(machine);
        return -1;
    }

    while(!ops::stopped(machine))
    {
        int ret=ops::step(machine,100000);
        if(ret<0)
        {
            hfprintf(stderr,"machine step failed!\r\n");
            ops::deinit(machine);
            return -1;
        }
        if(ret==1)
        {
            break;
        }
    }

    int output_ret=write_return_buffer(machine);
    int exit_code=ops::exit_code(machine);
    bool halted=ops::halted(machine);
    if(halted)
    {
        hfprintf(stderr,"machine halted: cause=%u fault_pc=%08x pc=%08x\r\n",
                 machine->sregs[2],machine->sregs[5],machine->pc);
    }

    ops::deinit(machine);
    if(output_ret!=0)
    {
        return -1;
    }
    return halted?-1:(exit_code&0xFF);
}

int main(int argc,char *argv[])
{
    check_args(argc,argv);
    show_banner();

    std::vector<uint8_t> elf_data;
    if(!load_file(g_elf_filename,elf_data))
    {
        hfprintf(stderr,"open %s failed!\r\n",g_elf_filename.c_str());
        return -1;
    }
    if(elf_data.empty())
    {
        hfprintf(stderr,"ELF file %s is empty!\r\n",g_elf_filename.c_str());
        return -1;
    }

    elf_memory_input_t input_ctx;
    input_ctx.data=elf_data.data();
    input_ctx.length=elf_data.size();
    helf_file_input_t input_file;
    helf_file_input_init(&input_file,elf_memory_input_read,&input_ctx);

    if(!helf_moxie_is_moxie(&input_file) || !helf_moxie_is_executable(&input_file))
    {
        hfprintf(stderr,"%s is not a moxie executable ELF!\r\n",g_elf_filename.c_str());
        return -1;
    }

    bool big_endian=helf_moxie_is_big_endian(&input_file);
    if(g_arch=="be" && !big_endian)
    {
        hfprintf(stderr,"ELF endianness mismatch: expected big endian\r\n");
        return -1;
    }
    if(g_arch=="le" && big_endian)
    {
        hfprintf(stderr,"ELF endianness mismatch: expected little endian\r\n");
        return -1;
    }

    uint32_t base=0;
    size_t span=0;
    if(!hs_moxie_moxiebox_elf_memory_range_get(&input_file,&base,&span) || span==0)
    {
        hfprintf(stderr,"ELF load range invalid!\r\n");
        return -1;
    }

    std::vector<std::vector<uint8_t> > data_files(g_data_filenames.size());
    for(size_t i=0;i<g_data_filenames.size();i++)
    {
        if(!load_file(g_data_filenames[i],data_files[i]))
        {
            hfprintf(stderr,"open %s failed!\r\n",g_data_filenames[i].c_str());
            return -1;
        }
    }

    if(big_endian)
    {
        return run_vm<hs_moxie_moxiebox_t>(&input_file,base,span,data_files);
    }
    return run_vm<hs_moxie_moxiebox_el_t>(&input_file,base,span,data_files);
}
