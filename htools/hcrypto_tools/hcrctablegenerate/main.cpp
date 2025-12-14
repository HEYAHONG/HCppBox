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

static std::string destination_dir_name=".";
static bool        is_output_internal_crctable=true;
static size_t      custom_crc_bits=32;
static std::string custom_crc_name="custom_crc";
static uint64_t    custom_crc_init=0;
static uint64_t    custom_crc_poly=0;
static uint64_t    custom_crc_xorout=0;
static bool        custom_crc_refin=false;
static bool        custom_crc_refout=false;
static void check_args(int argc,char *argv[])
{
    struct arg_file * destdir=NULL;
    struct arg_lit  * nointernalcrc=NULL;
    struct arg_int  * crcbits=NULL;
    struct arg_str  * crcname=NULL;
    struct arg_str  * crcinit=NULL;
    struct arg_str  * crcpoly=NULL;
    struct arg_str  * crcxorout=NULL;
    struct arg_lit  * crcrefin=NULL;
    struct arg_lit  * crcrefout=NULL;
    struct arg_lit  * help=NULL;
    void *argtable[]=
    {
        destdir=arg_file0("D","dest","dir",                 "destination dir"),
        nointernalcrc=arg_lit0("N","no-internal-crc",       "don't generate internal crc"),
        crcbits=arg_int0("b","bits","8/16/32/64",           "custom crc bits(8/16/32/64)"),
        crcname=arg_str0("n","name","[str]",                "custom crc name"),
        crcinit=arg_str0("I","init","[hex]",                "custom crc init(hex)"),
        crcpoly=arg_str0("P","poly","[hex]",                "custom crc poly(hex)"),
        crcxorout=arg_str0("X","xorout","[hex]",            "custom crc xorout(hex)"),
        crcrefin=arg_lit0("i","refin",                      "custom crc refin"),
        crcrefout=arg_lit0("o","refout",                    "custom crc refout"),
        help=arg_lit0("H","help",                           "print this help and exit"),
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
        hfputs("Usage:\r\n",stderr);
        arg_print_glossary(stderr,argtable,"  %-25s %s\n");
        hexit(-1);
    }

    if(help->count > 0)
    {
        hfputs("Usage:\r\n",stdout);
        arg_print_glossary(stdout,argtable,"  %-25s %s\n");
        hexit(-1);
    }

    if(destdir->count > 0)
    {
        destination_dir_name=destdir->filename[0];
    }

    {
        /*
         * 检查输出目录
         */
        std::filesystem::directory_entry dir(destination_dir_name);
        if(!dir.is_directory())
        {
            hfputs("destination dir is not found!\r\n",stderr);
            hexit(-1);
        }
        else
        {
            hfprintf(stderr,"destination dir is %s !\r\n",destination_dir_name.c_str());
        }
    }

    if(nointernalcrc->count > 0)
    {
        is_output_internal_crctable=false;
    }

    if(crcbits->count >0)
    {
        custom_crc_bits=crcbits->ival[0];
    }

    if(custom_crc_bits!=8 && custom_crc_bits!=16 && custom_crc_bits!=32 && custom_crc_bits!=64)
    {
        hfprintf(stderr,"only support 8/16/32/64,current is %d!\r\n",custom_crc_bits);
        hexit(-1);
    }

    if(crcname->count > 0)
    {
        custom_crc_name=crcname->sval[0];
    }

    {
        /*
         * 处理名称字符串，只允许小写字母、数字、下划线,开头不得为数字。
         */
        if(custom_crc_name.length() == 0)
        {
            custom_crc_name="custom_crc";
        }
        for(auto it=custom_crc_name.begin(); it!=custom_crc_name.end(); it++)
        {
            if(!(('0' <= (*it) && '9' >= (*it)) || ('a' <= (*it) && 'z' >= (*it))))
            {
                if('A' <= (*it) && 'Z' >= (*it))
                {
                    (*it)=std::tolower(*it);
                }
                else
                {
                    (*it)='_';
                }
            }
        }
        if(custom_crc_name.c_str()[0] >= '0' && custom_crc_name.c_str()[0] <= '9')
        {
            custom_crc_name=std::string("crc_")+custom_crc_name;
        }
    }

    if(crcinit->count > 0)
    {
        custom_crc_init=std::strtoull(crcinit->sval[0],NULL,16);
    }

    if(crcpoly->count > 0)
    {
        custom_crc_poly=std::strtoull(crcpoly->sval[0],NULL,16);
    }

    if(crcxorout->count > 0)
    {
        custom_crc_xorout=std::strtoull(crcxorout->sval[0],NULL,16);
    }

    if(crcrefin->count > 0)
    {
        custom_crc_refin=true;
    }

    if(crcrefout->count > 0)
    {
        custom_crc_refout=true;
    }

    arg_freetable(argtable,sizeof(argtable)/sizeof(argtable[0]));
}

static void writefile(std::string filename,std::string data)
{
    std::fstream outfile;
    outfile.open((destination_dir_name+"/"+filename).c_str(),std::ios::out);
    if(outfile.is_open())
    {
        outfile << data;
        outfile.close();
    }
    else
    {
        hfprintf(stderr,"can't write %s !\r\n",filename.c_str());
    }
}

static void c_header_begin(std::string basename,std::string headername,std::stringstream &ss)
{
    ss<<"/* DO NOT EDIT,AUTO GENERATED BY hcrctablegenerate */"<<std::endl;
    std::string file_gaurd=std::string("__")+basename+std::string("_H__");
    std::transform(file_gaurd.begin(), file_gaurd.end(), file_gaurd.begin(),[](unsigned char c)
    {
        return std::toupper(c);
    });
    ss<<"#ifndef " << file_gaurd <<std::endl;
    ss<<"#define " << file_gaurd <<std::endl;
    ss<<"#include \"stdint.h\"" << std::endl;
    ss<<"#include \"stdbool.h\"" << std::endl;
    ss<<"#include \"stdlib.h\"" << std::endl;
    ss<<"#ifdef __cplusplus" <<std::endl;
    ss<<"extern \"C\"" <<std::endl;
    ss<<"{" <<std::endl;
    ss<<"#endif" <<std::endl;
    std::string file_micro=basename;
    std::transform(file_micro.begin(), file_micro.end(), file_micro.begin(),[](unsigned char c)
    {
        return std::toupper(c);
    });
    ss<<"#define "<<file_micro<<" 1"<<std::endl;

}

static void c_header_end(std::string basename,std::string headername,std::stringstream &ss)
{
    ss<<"#ifdef __cplusplus" <<std::endl;
    ss<<"}" <<std::endl;
    ss<<"#endif"     << std::endl;
    ss<<"#endif"     << std::endl;
}

static void c_source_begin(std::string basename,std::string headername,std::stringstream &ss)
{
    ss<<"/* DO NOT EDIT,AUTO GENERATED BY hcrctablegenerate */"<<std::endl;
    ss<<"#include \"" << headername << "\"" << std::endl;
}

static void c_source_end(std::string basename,std::string headername,std::stringstream &ss)
{

}

static void output_crc8_table(std::string basename,hcrc_crc8_t config)
{
    std::string c_header_name=basename+".h";
    std::string c_source_name=basename+".c";
    {
        std::stringstream ss;
        c_header_begin(basename,c_header_name,ss);
        {
            /*
             * 声明接口
             */
            ss <<"uint8_t " << basename << "_table" << "_starts(void);"<<std::endl;
            ss <<"uint8_t " << basename << "_table" << "_update(uint8_t ctx,const uint8_t *data,size_t datalen);"<<std::endl;
            ss <<"uint8_t " << basename << "_table" << "_finish(uint8_t ctx);"<<std::endl;
            ss <<"uint8_t " << basename << "_table" << "_calculate(const uint8_t *data,size_t datalen);"<<std::endl;
            ss <<"bool    " << basename << "_table" << "_check(const uint8_t *data,size_t datalen,uint8_t check);"<<std::endl;
        }
        c_header_end(basename,c_header_name,ss);
        std::string outputdata;
        {
            astyle::ASFormatter formatter;
            formatter.setFormattingStyle(astyle::STYLE_ALLMAN);
            formatter.setCStyle();
            formatter.setModeManuallySet(true);
            astyle::ASStreamIterator<std::stringstream> streamIterator(&ss);
            formatter.init(&streamIterator);
            while (formatter.hasMoreLines())
            {
                outputdata+=formatter.nextLine();
                if(formatter.hasMoreLines())
                {
                    outputdata+="\r\n";
                }
                else
                {
                    if (formatter.getIsLineReady())
                    {
                        outputdata+="\r\n";
                        outputdata+=formatter.nextLine();
                    }
                }
            }
        }
        writefile(c_header_name,outputdata);
    }

    {
        std::stringstream ss;
        c_source_begin(basename,c_header_name,ss);
        {
            {
                ss<<"static uint8_t " <<  "__"<< basename << "_table" << "_data[]=" <<std::endl;
                ss<<"{"<<std::endl;
                for(size_t i=0; i<256; i++)
                {
                    if(i!=0 && ((i%8)==0))
                    {
                        ss<<std::endl;
                    }
                    ss<<std::showbase<<std::hex<<(int)hcrc_crc8_table(&config,i)<<",";
                }
                ss<<std::endl;
                ss<<"0" <<std::endl;
                ss<<"};"<<std::endl;
            }
            {
                ss<<"static uint8_t " << "__"<< basename << "_table(uint8_t index)" << std::endl;
                ss<<"{"<<std::endl;
                ss<<"return "<< "__"<< basename << "_table" << "_data[index];" <<std::endl;
                ss<<"};"<<std::endl;
            }
            {
                ss <<"uint8_t " << basename << "_table" << "_starts(void)"<<std::endl;
                ss<<"{"<<std::endl;
                ss<<"return "<< std::showbase<<std::hex<<(int)config.init <<";" <<std::endl;
                ss<<"};"<<std::endl;
            }
            {
                ss <<"uint8_t " << basename << "_table" << "_update(uint8_t ctx,const uint8_t *data,size_t datalen)"<<std::endl;
                ss<<"{"<<std::endl;
                ss<<"uint8_t crc=ctx;";
                if(config.refin&&config.refout)
                {
                    ss<<"\
if(data!=NULL && datalen > 0)\
{\
    for(size_t i=0; i<datalen; i++)\
    {\
        uint8_t current_data=data[i];\
        crc = "<<  "__"<<basename << "_table"<<"(current_data^crc);\
    }\
}"                  <<std::endl;

                }
                else
                {
                    ss<<"\
if(data!=NULL && datalen > 0)\
{\
    for(size_t i=0; i<datalen; i++)\
    {\
        uint8_t current_data=data[i];";

                    if(config.refin)
                    {
                        ss<<"\
        {\
            uint8_t temp=0;\
            for(size_t i=0; i < sizeof(current_data)*8 ; i++)\
            {\
                if(current_data & (1U << (i)))\
                {\
                        temp |= (1U << (sizeof(current_data)*8-1-i));\
                }\
            }\
            current_data=temp;\
        }"              <<std::endl;
                    }
                    ss<<"\
        crc = "<<  "__"<<basename << "_table"<<"(current_data^crc);\
    }\
}"                  <<std::endl;
                }
                ss<<"return crc;" <<std::endl;
                ss<<"};"<<std::endl;
            }
            {
                ss <<"uint8_t " << basename << "_table" << "_finish(uint8_t ctx)"<<std::endl;
                ss<<"{"<<std::endl;
                if(!config.refin&&config.refout)
                {
                    ss<<"\
{\
    uint8_t temp=0;\
    for(size_t i=0; i < sizeof(ctx)*8 ; i++)\
    {\
        if(ctx & (1U << (i)))\
        {\
            temp |= (1U << (sizeof(ctx)*8-1-i));\
        }\
    }\
    ctx=temp;\
}\
"                   <<std::endl;
                }
                ss<<"return "<<"ctx^"<< std::showbase<<std::hex<<(int)config.xorout <<";" <<std::endl;
                ss<<"};"<<std::endl;
            }
            {
                ss <<"uint8_t " << basename << "_table" << "_calculate(const uint8_t *data,size_t datalen)"<<std::endl;
                ss<<"{"<<std::endl;
                ss<<"uint8_t ctx="<<basename << "_table" << "_starts();"<<std::endl;
                ss<<"ctx="<<basename << "_table" << "_update(ctx,data,datalen);"<<std::endl;
                ss<<"return "<<basename << "_table" << "_finish(ctx);"<<std::endl;
                ss<<"};"<<std::endl;
            }
            {
                ss <<"bool    " << basename << "_table" << "_check(const uint8_t *data,size_t datalen,uint8_t check)"<<std::endl;
                ss<<"{"<<std::endl;
                ss<<"return check=="<<basename << "_table" << "_calculate(data,datalen);"<<std::endl;
                ss<<"};"<<std::endl;
            }
        }

        c_source_end(basename,c_header_name,ss);
        std::string outputdata;
        {
            astyle::ASFormatter formatter;
            formatter.setFormattingStyle(astyle::STYLE_ALLMAN);
            formatter.setCStyle();
            formatter.setModeManuallySet(true);
            astyle::ASStreamIterator<std::stringstream> streamIterator(&ss);
            formatter.init(&streamIterator);
            while (formatter.hasMoreLines())
            {
                outputdata+=formatter.nextLine();
                if(formatter.hasMoreLines())
                {
                    outputdata+="\r\n";
                }
                else
                {
                    if (formatter.getIsLineReady())
                    {
                        outputdata+="\r\n";
                        outputdata+=formatter.nextLine();
                    }
                }
            }
        }
        writefile(c_source_name,outputdata);
    }
}

static void output_crc16_table(std::string basename,hcrc_crc16_t config)
{
    std::string c_header_name=basename+".h";
    std::string c_source_name=basename+".c";
    {
        std::stringstream ss;
        c_header_begin(basename,c_header_name,ss);
        {
            /*
             * 声明接口
             */
            ss <<"uint16_t " << basename << "_table" << "_starts(void);"<<std::endl;
            ss <<"uint16_t " << basename << "_table" << "_update(uint16_t ctx,const uint8_t *data,size_t datalen);"<<std::endl;
            ss <<"uint16_t " << basename << "_table" << "_finish(uint16_t ctx);"<<std::endl;
            ss <<"uint16_t " << basename << "_table" << "_calculate(const uint8_t *data,size_t datalen);"<<std::endl;
            ss <<"bool     " << basename << "_table" << "_check(const uint8_t *data,size_t datalen,uint16_t check);"<<std::endl;
        }
        c_header_end(basename,c_header_name,ss);
        std::string outputdata;
        {
            astyle::ASFormatter formatter;
            formatter.setFormattingStyle(astyle::STYLE_ALLMAN);
            formatter.setCStyle();
            formatter.setModeManuallySet(true);
            astyle::ASStreamIterator<std::stringstream> streamIterator(&ss);
            formatter.init(&streamIterator);
            while (formatter.hasMoreLines())
            {
                outputdata+=formatter.nextLine();
                if(formatter.hasMoreLines())
                {
                    outputdata+="\r\n";
                }
                else
                {
                    if (formatter.getIsLineReady())
                    {
                        outputdata+="\r\n";
                        outputdata+=formatter.nextLine();
                    }
                }
            }
        }
        writefile(c_header_name,outputdata);
    }

    {
        std::stringstream ss;
        c_source_begin(basename,c_header_name,ss);
        {
            {
                ss<<"static uint16_t " <<  "__"<<basename << "_table" << "_data[]=" <<std::endl;
                ss<<"{"<<std::endl;
                for(size_t i=0; i<256; i++)
                {
                    if(i!=0 && ((i%8)==0))
                    {
                        ss<<std::endl;
                    }
                    ss<<std::showbase<<std::hex<<(int)hcrc_crc16_table(&config,i)<<",";
                }
                ss<<std::endl;
                ss<<"0" <<std::endl;
                ss<<"};"<<std::endl;
            }
            {
                ss<<"static uint16_t " << "__"<<basename << "_table(uint8_t index)" << std::endl;
                ss<<"{"<<std::endl;
                ss<<"return "<< "__"<<basename << "_table" << "_data[index];" <<std::endl;
                ss<<"};"<<std::endl;
            }
            {
                ss <<"uint16_t " << basename << "_table" << "_starts(void)"<<std::endl;
                ss<<"{"<<std::endl;
                ss<<"return "<< std::showbase<<std::hex<<(int)config.init <<";" <<std::endl;
                ss<<"};"<<std::endl;
            }
            {
                ss <<"uint16_t " << basename << "_table" << "_update(uint16_t ctx,const uint8_t *data,size_t datalen)"<<std::endl;
                ss<<"{"<<std::endl;
                ss<<"uint16_t crc=ctx;";
                if(config.refin&&config.refout)
                {
                    ss<<"\
if(data!=NULL && datalen > 0)\
{\
    for(size_t i=0; i<datalen; i++)\
    {\
        uint8_t current_data=data[i];\
        crc = (crc >> sizeof(current_data)*8) ^"<<  "__"<< basename << "_table"<<"((crc ^ ((uint16_t)current_data)));\
    }\
}"                  <<std::endl;

                }
                else
                {
                    ss<<"\
if(data!=NULL && datalen > 0)\
{\
    for(size_t i=0; i<datalen; i++)\
    {\
        uint8_t current_data=data[i];";

                    if(config.refin)
                    {
                        ss<<"\
        {\
            uint8_t temp=0;\
            for(size_t i=0; i < sizeof(current_data)*8 ; i++)\
            {\
                if(current_data & (1U << (i)))\
                {\
                        temp |= (1U << (sizeof(current_data)*8-1-i));\
                }\
            }\
            current_data=temp;\
        }"              <<std::endl;
                    }
                    ss<<"\
        crc = (crc << sizeof(current_data)*8) ^ "<<  "__"<< basename << "_table"<<"((crc>>((sizeof(crc)-sizeof(current_data))*8) ^ ((uint16_t)current_data)));\
    }\
}"                  <<std::endl;
                }
                ss<<"return crc;" <<std::endl;
                ss<<"};"<<std::endl;
            }
            {
                ss <<"uint16_t " << basename << "_table" << "_finish(uint16_t ctx)"<<std::endl;
                ss<<"{"<<std::endl;
                if(!config.refin&&config.refout)
                {
                    ss<<"\
{\
    uint16_t temp=0;\
    for(size_t i=0; i < sizeof(ctx)*8 ; i++)\
    {\
        if(ctx & (1U << (i)))\
        {\
            temp |= (1U << (sizeof(ctx)*8-1-i));\
        }\
    }\
    ctx=temp;\
}\
"                   <<std::endl;
                }
                ss<<"return "<<"ctx^"<< std::showbase<<std::hex<<(int)config.xorout <<";" <<std::endl;
                ss<<"};"<<std::endl;
            }
            {
                ss <<"uint16_t " << basename << "_table" << "_calculate(const uint8_t *data,size_t datalen)"<<std::endl;
                ss<<"{"<<std::endl;
                ss<<"uint16_t ctx="<<basename << "_table" << "_starts();"<<std::endl;
                ss<<"ctx="<<basename << "_table" << "_update(ctx,data,datalen);"<<std::endl;
                ss<<"return "<<basename << "_table" << "_finish(ctx);"<<std::endl;
                ss<<"};"<<std::endl;
            }
            {
                ss <<"bool    " << basename << "_table" << "_check(const uint8_t *data,size_t datalen,uint16_t check)"<<std::endl;
                ss<<"{"<<std::endl;
                ss<<"return check=="<<basename << "_table" << "_calculate(data,datalen);"<<std::endl;
                ss<<"};"<<std::endl;
            }
        }
        c_source_end(basename,c_header_name,ss);
        std::string outputdata;
        {
            astyle::ASFormatter formatter;
            formatter.setFormattingStyle(astyle::STYLE_ALLMAN);
            formatter.setCStyle();
            formatter.setModeManuallySet(true);
            astyle::ASStreamIterator<std::stringstream> streamIterator(&ss);
            formatter.init(&streamIterator);
            while (formatter.hasMoreLines())
            {
                outputdata+=formatter.nextLine();
                if(formatter.hasMoreLines())
                {
                    outputdata+="\r\n";
                }
                else
                {
                    if (formatter.getIsLineReady())
                    {
                        outputdata+="\r\n";
                        outputdata+=formatter.nextLine();
                    }
                }
            }
        }
        writefile(c_source_name,outputdata);
    }
}

static void output_crc32_table(std::string basename,hcrc_crc32_t config)
{
    std::string c_header_name=basename+".h";
    std::string c_source_name=basename+".c";
    {
        std::stringstream ss;
        c_header_begin(basename,c_header_name,ss);
        {
            /*
             * 声明接口
             */
            ss <<"uint32_t " << basename << "_table" << "_starts(void);"<<std::endl;
            ss <<"uint32_t " << basename << "_table" << "_update(uint32_t ctx,const uint8_t *data,size_t datalen);"<<std::endl;
            ss <<"uint32_t " << basename << "_table" << "_finish(uint32_t ctx);"<<std::endl;
            ss <<"uint32_t " << basename << "_table" << "_calculate(const uint8_t *data,size_t datalen);"<<std::endl;
            ss <<"bool     " << basename << "_table" << "_check(const uint8_t *data,size_t datalen,uint32_t check);"<<std::endl;
        }
        c_header_end(basename,c_header_name,ss);
        std::string outputdata;
        {
            astyle::ASFormatter formatter;
            formatter.setFormattingStyle(astyle::STYLE_ALLMAN);
            formatter.setCStyle();
            formatter.setModeManuallySet(true);
            astyle::ASStreamIterator<std::stringstream> streamIterator(&ss);
            formatter.init(&streamIterator);
            while (formatter.hasMoreLines())
            {
                outputdata+=formatter.nextLine();
                if(formatter.hasMoreLines())
                {
                    outputdata+="\r\n";
                }
                else
                {
                    if (formatter.getIsLineReady())
                    {
                        outputdata+="\r\n";
                        outputdata+=formatter.nextLine();
                    }
                }
            }
        }
        writefile(c_header_name,outputdata);
    }

    {
        std::stringstream ss;
        c_source_begin(basename,c_header_name,ss);
        {
            {
                ss<<"static uint32_t " <<  "__"<<basename << "_table" << "_data[]=" <<std::endl;
                ss<<"{"<<std::endl;
                for(size_t i=0; i<256; i++)
                {
                    if(i!=0 && ((i%8)==0))
                    {
                        ss<<std::endl;
                    }
                    ss<<std::showbase<<std::hex<<hcrc_crc32_table(&config,i)<<",";
                }
                ss<<std::endl;
                ss<<"0" <<std::endl;
                ss<<"};"<<std::endl;
            }
            {
                ss<<"static uint32_t " << "__"<<basename << "_table(uint8_t index)" << std::endl;
                ss<<"{"<<std::endl;
                ss<<"return "<< "__"<<basename << "_table" << "_data[index];" <<std::endl;
                ss<<"};"<<std::endl;
            }
            {
                ss <<"uint32_t " << basename << "_table" << "_starts(void)"<<std::endl;
                ss<<"{"<<std::endl;
                ss<<"return "<< std::showbase<<std::hex<<config.init <<";" <<std::endl;
                ss<<"};"<<std::endl;
            }
            {
                ss <<"uint32_t " << basename << "_table" << "_update(uint32_t ctx,const uint8_t *data,size_t datalen)"<<std::endl;
                ss<<"{"<<std::endl;
                ss<<"uint32_t crc=ctx;";
                if(config.refin&&config.refout)
                {
                    ss<<"\
if(data!=NULL && datalen > 0)\
{\
    for(size_t i=0; i<datalen; i++)\
    {\
        uint8_t current_data=data[i];\
        crc = (crc >> sizeof(current_data)*8) ^"<<  "__"<< basename << "_table"<<"((crc ^ ((uint32_t)current_data)));\
    }\
}"                  <<std::endl;

                }
                else
                {
                    ss<<"\
if(data!=NULL && datalen > 0)\
{\
    for(size_t i=0; i<datalen; i++)\
    {\
        uint8_t current_data=data[i];";

                    if(config.refin)
                    {
                        ss<<"\
        {\
            uint8_t temp=0;\
            for(size_t i=0; i < sizeof(current_data)*8 ; i++)\
            {\
                if(current_data & (1U << (i)))\
                {\
                        temp |= (1U << (sizeof(current_data)*8-1-i));\
                }\
            }\
            current_data=temp;\
        }"              <<std::endl;
                    }
                    ss<<"\
        crc = (crc << sizeof(current_data)*8) ^ "<<  "__"<< basename << "_table"<<"((crc>>((sizeof(crc)-sizeof(current_data))*8) ^ ((uint32_t)current_data)));\
    }\
}"                  <<std::endl;
                }
                ss<<"return crc;" <<std::endl;
                ss<<"};"<<std::endl;
            }
            {
                ss <<"uint32_t " << basename << "_table" << "_finish(uint32_t ctx)"<<std::endl;
                ss<<"{"<<std::endl;
                if(!config.refin&&config.refout)
                {
                    ss<<"\
{\
    uint32_t temp=0;\
    for(size_t i=0; i < sizeof(ctx)*8 ; i++)\
    {\
        if(ctx & (1UL << (i)))\
        {\
            temp |= (1UL << (sizeof(ctx)*8-1-i));\
        }\
    }\
    ctx=temp;\
}\
"                   <<std::endl;
                }
                ss<<"return "<<"ctx^"<< std::showbase<<std::hex<<config.xorout <<";" <<std::endl;
                ss<<"};"<<std::endl;
            }
            {
                ss <<"uint32_t " << basename << "_table" << "_calculate(const uint8_t *data,size_t datalen)"<<std::endl;
                ss<<"{"<<std::endl;
                ss<<"uint32_t ctx="<<basename << "_table" << "_starts();"<<std::endl;
                ss<<"ctx="<<basename << "_table" << "_update(ctx,data,datalen);"<<std::endl;
                ss<<"return "<<basename << "_table" << "_finish(ctx);"<<std::endl;
                ss<<"};"<<std::endl;
            }
            {
                ss <<"bool    " << basename << "_table" << "_check(const uint8_t *data,size_t datalen,uint32_t check)"<<std::endl;
                ss<<"{"<<std::endl;
                ss<<"return check=="<<basename << "_table" << "_calculate(data,datalen);"<<std::endl;
                ss<<"};"<<std::endl;
            }
        }
        c_source_end(basename,c_header_name,ss);
        std::string outputdata;
        {
            astyle::ASFormatter formatter;
            formatter.setFormattingStyle(astyle::STYLE_ALLMAN);
            formatter.setCStyle();
            formatter.setModeManuallySet(true);
            astyle::ASStreamIterator<std::stringstream> streamIterator(&ss);
            formatter.init(&streamIterator);
            while (formatter.hasMoreLines())
            {
                outputdata+=formatter.nextLine();
                if(formatter.hasMoreLines())
                {
                    outputdata+="\r\n";
                }
                else
                {
                    if (formatter.getIsLineReady())
                    {
                        outputdata+="\r\n";
                        outputdata+=formatter.nextLine();
                    }
                }
            }
        }
        writefile(c_source_name,outputdata);
    }
}

static void output_crc64_table(std::string basename,hcrc_crc64_t config)
{
    std::string c_header_name=basename+".h";
    std::string c_source_name=basename+".c";
    {
        std::stringstream ss;
        c_header_begin(basename,c_header_name,ss);
        {
            /*
             * 声明接口
             */
            ss <<"uint64_t " << basename << "_table" << "_starts(void);"<<std::endl;
            ss <<"uint64_t " << basename << "_table" << "_update(uint64_t ctx,const uint8_t *data,size_t datalen);"<<std::endl;
            ss <<"uint64_t " << basename << "_table" << "_finish(uint64_t ctx);"<<std::endl;
            ss <<"uint64_t " << basename << "_table" << "_calculate(const uint8_t *data,size_t datalen);"<<std::endl;
            ss <<"bool     " << basename << "_table" << "_check(const uint8_t *data,size_t datalen,uint64_t check);"<<std::endl;
        }
        c_header_end(basename,c_header_name,ss);
        std::string outputdata;
        {
            astyle::ASFormatter formatter;
            formatter.setFormattingStyle(astyle::STYLE_ALLMAN);
            formatter.setCStyle();
            formatter.setModeManuallySet(true);
            astyle::ASStreamIterator<std::stringstream> streamIterator(&ss);
            formatter.init(&streamIterator);
            while (formatter.hasMoreLines())
            {
                outputdata+=formatter.nextLine();
                if(formatter.hasMoreLines())
                {
                    outputdata+="\r\n";
                }
                else
                {
                    if (formatter.getIsLineReady())
                    {
                        outputdata+="\r\n";
                        outputdata+=formatter.nextLine();
                    }
                }
            }
        }
        writefile(c_header_name,outputdata);
    }

    {
        std::stringstream ss;
        c_source_begin(basename,c_header_name,ss);
        {
            {
                ss<<"static uint64_t " <<  "__"<<basename << "_table" << "_data[]=" <<std::endl;
                ss<<"{"<<std::endl;
                for(size_t i=0; i<256; i++)
                {
                    if(i!=0 && ((i%8)==0))
                    {
                        ss<<std::endl;
                    }
                    ss<<std::showbase<<std::hex<<hcrc_crc64_table(&config,i)<<",";
                }
                ss<<std::endl;
                ss<<"0" <<std::endl;
                ss<<"};"<<std::endl;
            }
            {
                ss<<"static uint64_t " << "__"<<basename << "_table(uint8_t index)" << std::endl;
                ss<<"{"<<std::endl;
                ss<<"return "<< "__"<<basename << "_table" << "_data[index];" <<std::endl;
                ss<<"};"<<std::endl;
            }
            {
                ss <<"uint64_t " << basename << "_table" << "_starts(void)"<<std::endl;
                ss<<"{"<<std::endl;
                ss<<"return "<< std::showbase<<std::hex<<config.init <<";" <<std::endl;
                ss<<"};"<<std::endl;
            }
            {
                ss <<"uint64_t " << basename << "_table" << "_update(uint64_t ctx,const uint8_t *data,size_t datalen)"<<std::endl;
                ss<<"{"<<std::endl;
                ss<<"uint64_t crc=ctx;";
                if(config.refin&&config.refout)
                {
                    ss<<"\
if(data!=NULL && datalen > 0)\
{\
    for(size_t i=0; i<datalen; i++)\
    {\
        uint8_t current_data=data[i];\
        crc = (crc >> sizeof(current_data)*8) ^"<<  "__"<< basename << "_table"<<"((crc ^ ((uint64_t)current_data)));\
    }\
}"                  <<std::endl;

                }
                else
                {
                    ss<<"\
if(data!=NULL && datalen > 0)\
{\
    for(size_t i=0; i<datalen; i++)\
    {\
        uint8_t current_data=data[i];";

                    if(config.refin)
                    {
                        ss<<"\
        {\
            uint8_t temp=0;\
            for(size_t i=0; i < sizeof(current_data)*8 ; i++)\
            {\
                if(current_data & (1U << (i)))\
                {\
                        temp |= (1U << (sizeof(current_data)*8-1-i));\
                }\
            }\
            current_data=temp;\
        }"              <<std::endl;
                    }
                    ss<<"\
        crc = (crc << sizeof(current_data)*8) ^ "<<  "__"<< basename << "_table"<<"((crc>>((sizeof(crc)-sizeof(current_data))*8) ^ ((uint64_t)current_data)));\
    }\
}"                  <<std::endl;
                }
                ss<<"return crc;" <<std::endl;
                ss<<"};"<<std::endl;
            }
            {
                ss <<"uint64_t " << basename << "_table" << "_finish(uint64_t ctx)"<<std::endl;
                ss<<"{"<<std::endl;
                if(!config.refin&&config.refout)
                {
                    ss<<"\
{\
    uint64_t temp=0;\
    for(size_t i=0; i < sizeof(ctx)*8 ; i++)\
    {\
        if(ctx & (1ULL << (i)))\
        {\
            temp |= (1ULL << (sizeof(ctx)*8-1-i));\
        }\
    }\
    ctx=temp;\
}\
"                   <<std::endl;
                }
                ss<<"return "<<"ctx^"<< std::showbase<<std::hex<<config.xorout <<";" <<std::endl;
                ss<<"};"<<std::endl;
            }
            {
                ss <<"uint64_t " << basename << "_table" << "_calculate(const uint8_t *data,size_t datalen)"<<std::endl;
                ss<<"{"<<std::endl;
                ss<<"uint64_t ctx="<<basename << "_table" << "_starts();"<<std::endl;
                ss<<"ctx="<<basename << "_table" << "_update(ctx,data,datalen);"<<std::endl;
                ss<<"return "<<basename << "_table" << "_finish(ctx);"<<std::endl;
                ss<<"};"<<std::endl;
            }
            {
                ss <<"bool    " << basename << "_table" << "_check(const uint8_t *data,size_t datalen,uint64_t check)"<<std::endl;
                ss<<"{"<<std::endl;
                ss<<"return check=="<<basename << "_table" << "_calculate(data,datalen);"<<std::endl;
                ss<<"};"<<std::endl;
            }
        }
        c_source_end(basename,c_header_name,ss);
        std::string outputdata;
        {
            astyle::ASFormatter formatter;
            formatter.setFormattingStyle(astyle::STYLE_ALLMAN);
            formatter.setCStyle();
            formatter.setModeManuallySet(true);
            astyle::ASStreamIterator<std::stringstream> streamIterator(&ss);
            formatter.init(&streamIterator);
            while (formatter.hasMoreLines())
            {
                outputdata+=formatter.nextLine();
                if(formatter.hasMoreLines())
                {
                    outputdata+="\r\n";
                }
                else
                {
                    if (formatter.getIsLineReady())
                    {
                        outputdata+="\r\n";
                        outputdata+=formatter.nextLine();
                    }
                }
            }
        }
        writefile(c_source_name,outputdata);
    }
}

static void  output_internal_crctable(int argc,char *argv[])
{
    if(!is_output_internal_crctable)
    {
        return;
    }

    std::vector<std::string> config_name;
    {
        hruntime_symbol_enum(HRUNTIME_SYMBOL_ENUM_TYPE_TABLE_ALL,[](hruntime_symbol_enum_type_t type,const hruntime_symbol_t * symbol,void *usr)
        {
            if(usr==NULL || symbol==NULL || symbol->symbol_name==NULL)
            {
                return;
            }
            std::vector<std::string> &config_name=*(std::vector<std::string> *)usr;
            if(strstr(symbol->symbol_name,"hcrc_crc")==symbol->symbol_name)
            {
                /*
                 * 包含以下字符串的符号将会被排除
                 */
                const std::vector<std::string> exception_name_table=
                {
                    "starts",
                    "table",
                    "update",
                    "finish",
                    "calculate",
                    "check"
                };
                for(auto it=exception_name_table.begin(); it!=exception_name_table.end(); it++)
                {
                    if(strstr(symbol->symbol_name,(*it).c_str())!=NULL)
                    {
                        return;
                    }
                }
                config_name.push_back(symbol->symbol_name);
            }
        },&config_name);
    }
    {
        std::stringstream hcrc_table_header;
        std::stringstream hcrc_table_source;
        for(auto it=config_name.begin(); it!=config_name.end(); it++)
        {

            std::cout << "process " << (*it) << std::endl;
            const char *name=(*it).c_str();
            if(strstr(name,"hcrc_crc8")==name)
            {
                std::cout << "type is crc8" << std::endl;
                hcrc_crc8_t config= {0};
                {
                    const hruntime_symbol_t *handle=hruntime_symbol_find(name);
                    if(handle!=NULL && handle->symbol_addr!=0)
                    {
                        config=*(const hcrc_crc8_t *)handle->symbol_addr;
                    }
                }
                std::cout << "init=" << std::showbase << std::hex << (int)config.init<<std::endl;
                std::cout << "poly=" << std::showbase << std::hex << (int)config.poly<<std::endl;
                std::cout << "xorout=" << std::showbase << std::hex << (int)config.xorout<<std::endl;
                std::cout << "refin=" <<  (const char *)(config.refin?"true":"false") <<std::endl;
                std::cout << "refout=" << (const char *)(config.refin?"true":"false") <<std::endl;
                output_crc8_table(name,config);
                hcrc_table_header<<"#include \"" << std::string(name)+".h" << "\"" << std::endl;
                hcrc_table_source<<"#include \"" << std::string(name)+".c" << "\"" << std::endl;
            }
            else if(strstr(name,"hcrc_crc16")==name)
            {
                std::cout << "type is crc16" << std::endl;
                hcrc_crc16_t config= {0};
                {
                    const hruntime_symbol_t *handle=hruntime_symbol_find(name);
                    if(handle!=NULL && handle->symbol_addr!=0)
                    {
                        config=*(const hcrc_crc16_t *)handle->symbol_addr;
                    }
                }
                std::cout << "init=" << std::showbase << std::hex << config.init<<std::endl;
                std::cout << "poly=" << std::showbase << std::hex << config.poly<<std::endl;
                std::cout << "xorout=" << std::showbase << std::hex << config.xorout<<std::endl;
                std::cout << "refin=" <<  (const char *)(config.refin?"true":"false") <<std::endl;
                std::cout << "refout=" << (const char *)(config.refin?"true":"false") <<std::endl;
                output_crc16_table(name,config);
                hcrc_table_header<<"#include \"" << std::string(name)+".h" << "\"" << std::endl;
                hcrc_table_source<<"#include \"" << std::string(name)+".c" << "\"" << std::endl;
            }
            else if(strstr(name,"hcrc_crc32")==name)
            {
                std::cout << "type is crc32" << std::endl;
                hcrc_crc32_t config= {0};
                {
                    const hruntime_symbol_t *handle=hruntime_symbol_find(name);
                    if(handle!=NULL && handle->symbol_addr!=0)
                    {
                        config=*(const hcrc_crc32_t *)handle->symbol_addr;
                    }
                }
                std::cout << "init=" << std::showbase << std::hex << config.init<<std::endl;
                std::cout << "poly=" << std::showbase << std::hex << config.poly<<std::endl;
                std::cout << "xorout=" << std::showbase << std::hex << config.xorout<<std::endl;
                std::cout << "refin=" <<  (const char *)(config.refin?"true":"false") <<std::endl;
                std::cout << "refout=" << (const char *)(config.refin?"true":"false") <<std::endl;
                output_crc32_table(name,config);
                hcrc_table_header<<"#include \"" << std::string(name)+".h" << "\"" << std::endl;
                hcrc_table_source<<"#include \"" << std::string(name)+".c" << "\"" << std::endl;
            }
            else if(strstr(name,"hcrc_crc64")==name)
            {
                std::cout << "type is crc64" << std::endl;
                hcrc_crc64_t config= {0};
                {
                    const hruntime_symbol_t *handle=hruntime_symbol_find(name);
                    if(handle!=NULL && handle->symbol_addr!=0)
                    {
                        config=*(const hcrc_crc64_t *)handle->symbol_addr;
                    }
                }
                std::cout << "init=" << std::showbase << std::hex << config.init<<std::endl;
                std::cout << "poly=" << std::showbase << std::hex << config.poly<<std::endl;
                std::cout << "xorout=" << std::showbase << std::hex << config.xorout<<std::endl;
                std::cout << "refin=" <<  (const char *)(config.refin?"true":"false") <<std::endl;
                std::cout << "refout=" << (const char *)(config.refin?"true":"false") <<std::endl;
                output_crc64_table(name,config);
                hcrc_table_header<<"#include \"" << std::string(name)+".h" << "\"" << std::endl;
                hcrc_table_source<<"#include \"" << std::string(name)+".c" << "\"" << std::endl;
            }
            std::cout << std::endl;
        }

        {
            std::string outputdata;
            {
                astyle::ASFormatter formatter;
                formatter.setFormattingStyle(astyle::STYLE_ALLMAN);
                formatter.setCStyle();
                formatter.setModeManuallySet(true);
                astyle::ASStreamIterator<std::stringstream> streamIterator(&hcrc_table_header);
                formatter.init(&streamIterator);
                while (formatter.hasMoreLines())
                {
                    outputdata+=formatter.nextLine();
                    if(formatter.hasMoreLines())
                    {
                        outputdata+="\r\n";
                    }
                    else
                    {
                        if (formatter.getIsLineReady())
                        {
                            outputdata+="\r\n";
                            outputdata+=formatter.nextLine();
                        }
                    }
                }
            }
            writefile("hcrc_table.h",outputdata);
        }

        {
            std::string outputdata;
            {
                astyle::ASFormatter formatter;
                formatter.setFormattingStyle(astyle::STYLE_ALLMAN);
                formatter.setCStyle();
                formatter.setModeManuallySet(true);
                astyle::ASStreamIterator<std::stringstream> streamIterator(&hcrc_table_source);
                formatter.init(&streamIterator);
                while (formatter.hasMoreLines())
                {
                    outputdata+=formatter.nextLine();
                    if(formatter.hasMoreLines())
                    {
                        outputdata+="\r\n";
                    }
                    else
                    {
                        if (formatter.getIsLineReady())
                        {
                            outputdata+="\r\n";
                            outputdata+=formatter.nextLine();
                        }
                    }
                }
            }
            writefile("hcrc_table.c",outputdata);
        }
    }
}

static void output_custom_crc(int argc,char *argv[])
{
    if(custom_crc_poly == 0)
    {
        //多项式为0,不生成crc
        return;
    }

    std::string name=custom_crc_name;
    std::cout << "generate " << name <<std::endl;
    if(custom_crc_bits==8)
    {
        std::cout << "type is crc8" << std::endl;
        hcrc_crc8_t config= {0};
        {
            config.init=custom_crc_init;
            config.poly=custom_crc_poly;
            config.xorout=custom_crc_xorout;
            config.refin=custom_crc_refin;
            config.refout=custom_crc_refout;
        }
        std::cout << "init=" << std::showbase << std::hex << (int)config.init<<std::endl;
        std::cout << "poly=" << std::showbase << std::hex << (int)config.poly<<std::endl;
        std::cout << "xorout=" << std::showbase << std::hex << (int)config.xorout<<std::endl;
        std::cout << "refin=" <<  (const char *)(config.refin?"true":"false") <<std::endl;
        std::cout << "refout=" << (const char *)(config.refin?"true":"false") <<std::endl;
        output_crc8_table(name,config);
    }
    else if(custom_crc_bits==16)
    {
        std::cout << "type is crc16" << std::endl;
        hcrc_crc16_t config= {0};
        {
            config.init=custom_crc_init;
            config.poly=custom_crc_poly;
            config.xorout=custom_crc_xorout;
            config.refin=custom_crc_refin;
            config.refout=custom_crc_refout;
        }
        std::cout << "init=" << std::showbase << std::hex << config.init<<std::endl;
        std::cout << "poly=" << std::showbase << std::hex << config.poly<<std::endl;
        std::cout << "xorout=" << std::showbase << std::hex << config.xorout<<std::endl;
        std::cout << "refin=" <<  (const char *)(config.refin?"true":"false") <<std::endl;
        std::cout << "refout=" << (const char *)(config.refin?"true":"false") <<std::endl;
        output_crc16_table(name,config);
    }
    else if(custom_crc_bits==32)
    {
        std::cout << "type is crc32" << std::endl;
        hcrc_crc32_t config= {0};
        {
            config.init=custom_crc_init;
            config.poly=custom_crc_poly;
            config.xorout=custom_crc_xorout;
            config.refin=custom_crc_refin;
            config.refout=custom_crc_refout;
        }
        std::cout << "init=" << std::showbase << std::hex << config.init<<std::endl;
        std::cout << "poly=" << std::showbase << std::hex << config.poly<<std::endl;
        std::cout << "xorout=" << std::showbase << std::hex << config.xorout<<std::endl;
        std::cout << "refin=" <<  (const char *)(config.refin?"true":"false") <<std::endl;
        std::cout << "refout=" << (const char *)(config.refin?"true":"false") <<std::endl;
        output_crc32_table(name,config);
    }
    else if(custom_crc_bits==64)
    {
        std::cout << "type is crc64" << std::endl;
        hcrc_crc64_t config= {0};
        {
            config.init=custom_crc_init;
            config.poly=custom_crc_poly;
            config.xorout=custom_crc_xorout;
            config.refin=custom_crc_refin;
            config.refout=custom_crc_refout;
        }
        std::cout << "init=" << std::showbase << std::hex << config.init<<std::endl;
        std::cout << "poly=" << std::showbase << std::hex << config.poly<<std::endl;
        std::cout << "xorout=" << std::showbase << std::hex << config.xorout<<std::endl;
        std::cout << "refin=" <<  (const char *)(config.refin?"true":"false") <<std::endl;
        std::cout << "refout=" << (const char *)(config.refin?"true":"false") <<std::endl;
        output_crc64_table(name,config);
    }

}

int main(int argc,char *argv[])
{
    show_banner();

    check_args(argc,argv);

    output_internal_crctable(argc,argv);

    output_custom_crc(argc,argv);

    return 0;
}

/*
 * 导入Astyle主程序
 */
#ifdef main
#undef main
#endif // main
#define main astyle_main
#ifdef _
#undef _
#endif
#define _(X) (X)
#ifdef _WIN32
#ifdef UNICODE
#undef UNICODE
#endif // UNICODE
#ifdef GetFileAttributes
#undef GetFileAttributes
#endif // GetFileAttributes
#define GetFileAttributes GetFileAttributesA
#ifdef  FormatMessage
#undef  FormatMessage
#endif //  FormatMessage
#define  FormatMessage  FormatMessageA
#ifdef SHELLEXECUTEINFO
#undef SHELLEXECUTEINFO
#endif // SHELLEXECUTEINFO
#define SHELLEXECUTEINFO SHELLEXECUTEINFOA
#ifdef ShellExecuteEx
#undef ShellExecuteEx
#endif // ShellExecuteEx
#define ShellExecuteEx ShellExecuteExA
#endif
#include "astyle_main.cpp"
