#include "HCPPBox.h"
#include "hrc.h"
#include H3RDPARTY_ARGTABLE3_HEADER
#include "string"
#include "astyle.h"
#include "astyle_main.h"
#include "sstream"
#include "filesystem"
#include "inttypes.h"
#include "vector"
#include "string.h"
#include "fstream"
#include "algorithm"

static void show_banner()
{
    const uint8_t * banner=RCGetHandle("banner");
    if(banner!=NULL)
    {
        hprintf("%s",(const char *)banner);
    }
}

static bool do_decompress=false;
static std::string filename;
static void check_args(int argc,char *argv[])
{
    struct arg_lit  * help=NULL;
    struct arg_lit  * decompress=NULL;
    struct arg_file *file=NULL;
    void *argtable[]=
    {
        help=arg_lit0("h","help",                                      "print this help and exit"),
        decompress=arg_lit0("d","decompress",                          "decompress"),
        file=arg_file0(NULL,NULL,"<file>",                               "file name"),
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

    if(decompress->count > 0)
    {
        do_decompress=true;
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

static int do_decompress_entry(int argc,char *argv[])
{
    std::string input_filename=filename;
    std::string output_filename;
    if(input_filename.length() > 4)
    {
        output_filename=input_filename.substr(0,input_filename.length()-4);
    }
    else
    {
        output_filename=input_filename+".data";
    }
    FILE *input_file=hfopen(input_filename.c_str(),"rb");
    if(input_file==NULL)
    {
        hfprintf(stderr,"open %s failed!\r\n",input_filename.c_str());
        hexit(-1);
    }
    FILE *output_file=hfopen(output_filename.c_str(),"wb+");
    if(output_file==NULL)
    {
        hfclose(input_file);
        hfprintf(stderr,"open %s failed!\r\n",output_filename.c_str());
        hexit(-1);
    }

    hlz4_readfile_t *lz4f=NULL;
    if(hlz4f_readopen(&lz4f,input_file)!=0)
    {
        hfclose(input_file);
        hfclose(output_file);
        hfprintf(stderr,"readopen failed!\r\n");
        hexit(-1);
    }

    uint8_t buffer[4096];
    size_t buffer_bytes_read=0;
    while((buffer_bytes_read=hlz4f_read(lz4f,buffer,sizeof(buffer)))>0)
    {
        if(hfwrite(buffer,1,buffer_bytes_read,output_file)!=buffer_bytes_read)
        {
            hlz4f_readclose(lz4f);
            hfclose(input_file);
            hfclose(output_file);
            hfprintf(stderr,"write failed!\r\n");
            hexit(-1);
        }
    }

    hlz4f_readclose(lz4f);

    hfclose(input_file);
    hfclose(output_file);

    return 0;
}

static int do_compress_entry(int argc,char *argv[])
{
    std::string input_filename=filename;
    std::string output_filename=(filename+".lz4");
    FILE *input_file=hfopen(input_filename.c_str(),"rb");
    if(input_file==NULL)
    {
        hfprintf(stderr,"open %s failed!\r\n",input_filename.c_str());
        hexit(-1);
    }
    FILE *output_file=hfopen(output_filename.c_str(),"wb+");
    if(output_file==NULL)
    {
        hfclose(input_file);
        hfprintf(stderr,"open %s failed!\r\n",output_filename.c_str());
        hexit(-1);
    }

    hlz4_writefile_t *lz4f=NULL;
    if(hlz4f_writeopen(&lz4f,output_file,NULL)!=0)
    {
        hfclose(input_file);
        hfclose(output_file);
        hfprintf(stderr,"writeopen failed!\r\n");
        hexit(-1);
    }

    uint8_t buffer[64*1024];
    size_t buffer_bytes_read=0;
    while((buffer_bytes_read=hfread(buffer,1,sizeof(buffer),input_file))>0)
    {
        if(hlz4f_write(lz4f,buffer,buffer_bytes_read)!=buffer_bytes_read)
        {
            hlz4f_writeclose(lz4f);
            hfclose(input_file);
            hfclose(output_file);
            hfprintf(stderr,"write failed!\r\n");
            hexit(-1);
        }
    }

    hlz4f_writeclose(lz4f);

    hfclose(input_file);
    hfclose(output_file);

    return 0;
}


int main(int argc,char *argv[])
{
    int ret=0;

    check_args(argc,argv);

    if(do_decompress)
    {
        ret=do_decompress_entry(argc,argv);
    }
    else
    {
        ret=do_compress_entry(argc,argv);
    }

    return ret;
}

