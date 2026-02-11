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
static int  level=9;
static std::string filename;
static void check_args(int argc,char *argv[])
{
    struct arg_lit  * help=NULL;
    struct arg_lit  * decompress=NULL;
    struct arg_lit  * level_1=NULL;
    struct arg_lit  * level_9=NULL;
    struct arg_file *file=NULL;
    void *argtable[]=
    {
        help=arg_lit0("h","help",                                      "print this help and exit"),
        decompress=arg_lit0("d","decompress",                          "decompress"),
        level_1=arg_lit0("1","level-1",                                "level 1"),
        level_9=arg_lit0("9","level-9",                                "level 9"),
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

    if(level_1->count > 0)
    {
        level=1;
    }

    if(level_9->count > 0)
    {
        level=9;
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
    if(input_filename.length() > 3)
    {
        output_filename=input_filename.substr(0,input_filename.length()-3);
    }
    else
    {
        output_filename=input_filename+".data";
    }
    int input_fd=hopen(input_filename.c_str(),O_RDONLY);
    if(input_fd < 0)
    {
        hfprintf(stderr,"open %s failed!\r\n",input_filename.c_str());
        hexit(-1);
    }
    int output_fd=hopen(output_filename.c_str(),O_RDWR | O_CREAT,00777);
    if(output_fd < 0)
    {
        hfprintf(stderr,"open %s failed!\r\n",output_filename.c_str());
        hclose(input_fd);
        hexit(-1);
    }

    hgzfile_t * file=hgzdopen(input_fd,"r");
    if(file==NULL)
    {
        hfprintf(stderr,"gzopen  failed!\r\n");
        hclose(output_fd);
        hclose(input_fd);
        hexit(-1);
    }

    hgzbuffer(file,8192);

    hgzsetparams(file, level, 0);

    {
        uint8_t buffer[4096];
        int     buffer_bytes_read=0;
        while((buffer_bytes_read=hgzread(file,buffer,sizeof(buffer))) > 0)
        {
            if(buffer_bytes_read!=hwrite(output_fd,buffer,buffer_bytes_read))
            {
                hgzclose(file);
                hclose(output_fd);
                hclose(input_fd);
                hfprintf(stderr,"write  failed!\r\n");
                hexit(-1);
            }
        }
    }

    hgzclose(file);
    hclose(output_fd);
    hclose(input_fd);
    return 0;
}

static int do_compress_entry(int argc,char *argv[])
{
    std::string input_filename=filename;
    std::string output_filename=(filename+".gz");
    int input_fd=hopen(input_filename.c_str(),O_RDONLY);
    if(input_fd < 0)
    {
        hfprintf(stderr,"open %s failed!\r\n",input_filename.c_str());
        hexit(-1);
    }
    int output_fd=hopen(output_filename.c_str(),O_RDWR | O_CREAT,00777);
    if(output_fd < 0)
    {
        hfprintf(stderr,"open %s failed!\r\n",output_filename.c_str());
        hclose(input_fd);
        hexit(-1);
    }

    hgzfile_t * file=hgzdopen(output_fd,"w");
    if(file==NULL)
    {
        hfprintf(stderr,"gzopen  failed!\r\n");
        hclose(output_fd);
        hclose(input_fd);
        hexit(-1);
    }

    hgzbuffer(file,8192);

    hgzsetparams(file, level, 0);

    {
        uint8_t buffer[4096];
        int     buffer_bytes_read=0;
        while((buffer_bytes_read=hread(input_fd,buffer,sizeof(buffer))) > 0)
        {
            if(buffer_bytes_read!=hgzwrite(file,buffer,buffer_bytes_read))
            {
                hgzclose(file);
                hclose(output_fd);
                hclose(input_fd);
                hfprintf(stderr,"gzwrite  failed!\r\n");
                hexit(-1);
            }
        }
    }

    hgzclose(file);
    hclose(output_fd);
    hclose(input_fd);

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

