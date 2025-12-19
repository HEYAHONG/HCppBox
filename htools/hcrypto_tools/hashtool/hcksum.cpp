#include "HCPPBox.h"
#include "hrc.h"
#include "string"
#include "fstream"
#include H3RDPARTY_ARGTABLE3_HEADER

static void check_args(int argc,char *argv[])
{
    struct arg_file *files=NULL;
    struct arg_lit * check=NULL;
    struct arg_lit * help=NULL;
    struct arg_end * e=NULL;
    void *argtable[]=
    {
        check=arg_lit0("c","check",                         "check"),
        help=arg_lit0("h","help",                           "print this help and exit"),
        files=arg_filen(NULL, NULL, "<file>", 0, 100,       "Input files"),
        e=arg_end(20)
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
        hfprintf(stdout,"Usage: %s [options]\r\n",argv[0]);
        arg_print_glossary(stdout,argtable,"  %-25s %s\n");
        hexit(0);
    }

    if(check->count > 0)
    {
        for(size_t i=0; i<files->count; i++)
        {
            if(strcmp("-",files->basename[i])==0)
            {
                while(!feof(stdin))
                {
                    char fileline[8192]= {0};
                    if(NULL!=fgets(fileline,sizeof(fileline)-1,stdin))
                    {
                        char filename_string[sizeof(fileline)]= {0};
                        unsigned long cksum_hash= {0};
                        unsigned long long cksum_length=0;
                        sscanf(fileline,"%lu %llu %s",&cksum_hash,&cksum_length,filename_string);
                        uint32_t cksum_file= {0};
                        {
                            hcrc_cksum_context_t ctx=hcrc_cksum_starts();
                            std::fstream file;
                            file.open(filename_string,std::ios::in|std::ios::binary);
                            if(!file.is_open())
                            {
                                hfprintf(stderr,"open %s failed!\r\n",filename_string);
                                hexit(-1);
                            }
                            while(!file.eof())
                            {
                                uint8_t buffer[sizeof(hsm3_message_block_t)];
                                file.read((char *)buffer,sizeof(buffer));
                                ctx=hcrc_cksum_update(ctx,buffer,file.gcount());
                            }
                            file.close();
                            cksum_file=hcrc_cksum_finish(ctx);
                        }

                        if(cksum_hash==cksum_file)
                        {
                            hfprintf(stdout,"%s : ok\r\n",filename_string);
                        }
                        else
                        {
                            hfprintf(stderr,"%s : failed!\r\n",filename_string);
                            hexit(-1);
                        }
                    }
                }
            }
            else
            {
                std::fstream listfile;
                listfile.open(files->filename[i],std::ios::in|std::ios::binary);
                if(!listfile.is_open())
                {
                    hfprintf(stderr,"open %s failed!\r\n",files->filename[i]);
                    hexit(-1);
                }
                while(!listfile.eof())
                {
                    char fileline[8192]= {0};
                    listfile.getline(fileline,sizeof(fileline)-1);
                    if(listfile.gcount() > 0)
                    {
                        char filename_string[sizeof(fileline)]= {0};
                        unsigned long cksum_hash= {0};
                        unsigned long long cksum_length=0;
                        sscanf(fileline,"%lu %llu %s",&cksum_hash,&cksum_length,filename_string);
                        uint32_t cksum_file= {0};
                        {
                            hcrc_cksum_context_t ctx=hcrc_cksum_starts();
                            std::fstream file;
                            file.open(filename_string,std::ios::in|std::ios::binary);
                            if(!file.is_open())
                            {
                                hfprintf(stderr,"open %s failed!\r\n",filename_string);
                                hexit(-1);
                            }
                            while(!file.eof())
                            {
                                uint8_t buffer[sizeof(hsm3_message_block_t)];
                                file.read((char *)buffer,sizeof(buffer));
                                ctx=hcrc_cksum_update(ctx,buffer,file.gcount());
                            }
                            file.close();
                            cksum_file=hcrc_cksum_finish(ctx);
                        }

                        if(cksum_hash==cksum_file)
                        {
                            hfprintf(stdout,"%s : ok\r\n",filename_string);
                        }
                        else
                        {
                            hfprintf(stderr,"%s : failed!\r\n",filename_string);
                            hexit(-1);
                        }
                    }
                }
            }
        }
        if(files->count == 0)
        {
            while(!feof(stdin))
            {
                char fileline[8192]= {0};
                if(NULL!=fgets(fileline,sizeof(fileline)-1,stdin))
                {
                    char filename_string[sizeof(fileline)]= {0};
                    unsigned long cksum_hash= {0};
                    unsigned long long cksum_length=0;
                    sscanf(fileline,"%lu %llu %s",&cksum_hash,&cksum_length,filename_string);
                    uint32_t cksum_file= {0};
                    {
                        hcrc_cksum_context_t ctx=hcrc_cksum_starts();
                        std::fstream file;
                        file.open(filename_string,std::ios::in|std::ios::binary);
                        if(!file.is_open())
                        {
                            hfprintf(stderr,"open %s failed!\r\n",filename_string);
                            hexit(-1);
                        }
                        while(!file.eof())
                        {
                            uint8_t buffer[sizeof(hsm3_message_block_t)];
                            file.read((char *)buffer,sizeof(buffer));
                            ctx=hcrc_cksum_update(ctx,buffer,file.gcount());
                        }
                        file.close();
                        cksum_file=hcrc_cksum_finish(ctx);
                    }

                    if(cksum_hash==cksum_file)
                    {
                        hfprintf(stdout,"%s : ok\r\n",filename_string);
                    }
                    else
                    {
                        hfprintf(stderr,"%s : failed!\r\n",filename_string);
                        hexit(-1);
                    }
                }
            }
        }
    }
    else
    {
        if(files->count > 0)
        {
            for(size_t i=0; i<files->count; i++)
            {
                if(strcmp("-",files->basename[i])==0)
                {
                    hcrc_cksum_context_t ctx=hcrc_cksum_starts();
                    int c=EOF;
                    while((c=getchar())!=EOF)
                    {
                        uint8_t ch=(uint8_t)c;
                        ctx=hcrc_cksum_update(ctx,&ch,sizeof(ch));
                    }
                    uint32_t cksum=hcrc_cksum_finish(ctx);
                    hfprintf(stdout,"%lu %llu\r\n",(unsigned long)cksum,(unsigned)ctx.byte_count);
                }
                else
                {
                    hcrc_cksum_context_t ctx=hcrc_cksum_starts();
                    std::fstream file;
                    file.open(files->filename[i],std::ios::in|std::ios::binary);
                    if(!file.is_open())
                    {
                        continue;
                    }
                    while(!file.eof())
                    {
                        uint8_t buffer[sizeof(hsm3_message_block_t)];
                        file.read((char *)buffer,sizeof(buffer));
                        ctx=hcrc_cksum_update(ctx,buffer,file.gcount());
                    }
                    file.close();
                    uint32_t cksum=hcrc_cksum_finish(ctx);
                    hfprintf(stdout,"%lu %llu %s\r\n",(unsigned long)cksum,(unsigned)ctx.byte_count,files->filename[i]);
                }
            }
        }
        else
        {
            hcrc_cksum_context_t ctx=hcrc_cksum_starts();
            int c=EOF;
            while((c=getchar())!=EOF)
            {
                uint8_t ch=(uint8_t)c;
                ctx=hcrc_cksum_update(ctx,&ch,sizeof(ch));
            }
            uint32_t cksum=hcrc_cksum_finish(ctx);
            hfprintf(stdout,"%lu %llu\r\n",(unsigned long)cksum,(unsigned)ctx.byte_count);
        }
    }

    arg_freetable(argtable,sizeof(argtable)/sizeof(argtable[0]));
}


int main(int argc,char *argv[])
{
    check_args(argc,argv);
    return 0;
}
