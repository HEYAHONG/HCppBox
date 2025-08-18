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
                    char fileline[sizeof(hmd4_md4_t)+4096]= {0};
                    if(NULL!=fgets(fileline,sizeof(fileline)-1,stdin))
                    {
                        const char *hash_string=NULL;
                        const char *filename_string=NULL;
                        size_t fileline_length=strlen(fileline);
                        while((fileline[fileline_length-1]=='\n') || (fileline[fileline_length-1]=='\r'))
                        {
                            fileline[fileline_length-1]='\0';
                            fileline_length--;
                        }
                        for(size_t i=0; i<fileline_length; i++)
                        {
                            if(fileline[i]!=' ')
                            {
                                if(hash_string==NULL)
                                {
                                    hash_string=&fileline[i];
                                }
                                else
                                {
                                    if(filename_string==NULL && fileline[i-1]=='\0')
                                    {
                                        filename_string=&fileline[i];
                                    }
                                }
                            }
                            else
                            {
                                if(hash_string==NULL || filename_string==NULL)
                                {
                                    fileline[i]='\0';
                                }
                            }
                            if(hash_string!=NULL && filename_string!=NULL)
                            {
                                break;
                            }
                        }

                        if(hash_string==NULL || filename_string==NULL)
                        {
                            hfprintf(stderr,"line error!\r\n");
                            hexit(-1);
                        }

                        hmd4_md4_t md4_hash= {0};
                        hbase16_decode(md4_hash,sizeof(md4_hash),hash_string,strlen(hash_string));
                        hmd4_md4_t md4_file= {0};
                        {
                            hmd4_context_t ctx;
                            hmd4_starts(&ctx);
                            std::fstream file;
                            file.open(filename_string,std::ios::in|std::ios::binary);
                            if(!file.is_open())
                            {
                                hfprintf(stderr,"open %s failed!\r\n",filename_string);
                                hexit(-1);
                            }
                            while(!file.eof())
                            {
                                uint8_t buffer[sizeof(hmd4_message_block_t)];
                                file.read((char *)buffer,sizeof(buffer));
                                hmd4_update(&ctx,buffer,file.gcount());
                            }
                            file.close();
                            hmd4_finish(&ctx,md4_file);
                        }

                        if(memcmp(md4_file,md4_hash,sizeof(hmd4_md4_t))==0)
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
                    char fileline[sizeof(hmd4_md4_t)+4096]= {0};
                    listfile.getline(fileline,sizeof(fileline)-1);
                    if(listfile.gcount() > 0)
                    {
                        const char *hash_string=NULL;
                        const char *filename_string=NULL;
                        size_t fileline_length=strlen(fileline);
                        while((fileline[fileline_length-1]=='\n') || (fileline[fileline_length-1]=='\r'))
                        {
                            fileline[fileline_length-1]='\0';
                            fileline_length--;
                        }
                        for(size_t i=0; i<fileline_length; i++)
                        {
                            if(fileline[i]!=' ')
                            {
                                if(hash_string==NULL)
                                {
                                    hash_string=&fileline[i];
                                }
                                else
                                {
                                    if(filename_string==NULL && fileline[i-1]=='\0')
                                    {
                                        filename_string=&fileline[i];
                                    }
                                }
                            }
                            else
                            {
                                if(hash_string==NULL || filename_string==NULL)
                                {
                                    fileline[i]='\0';
                                }
                            }
                            if(hash_string!=NULL && filename_string!=NULL)
                            {
                                break;
                            }
                        }

                        if(hash_string==NULL || filename_string==NULL)
                        {
                            hfprintf(stderr,"line error!\r\n");
                            hexit(-1);
                        }

                        hmd4_md4_t md4_hash= {0};
                        hbase16_decode(md4_hash,sizeof(md4_hash),hash_string,strlen(hash_string));
                        hmd4_md4_t md4_file= {0};
                        {
                            hmd4_context_t ctx;
                            hmd4_starts(&ctx);
                            std::fstream file;
                            file.open(filename_string,std::ios::in|std::ios::binary);
                            if(!file.is_open())
                            {
                                hfprintf(stderr,"open %s failed!\r\n",filename_string);
                                hexit(-1);
                            }
                            while(!file.eof())
                            {
                                uint8_t buffer[sizeof(hmd4_message_block_t)];
                                file.read((char *)buffer,sizeof(buffer));
                                hmd4_update(&ctx,buffer,file.gcount());
                            }
                            file.close();
                            hmd4_finish(&ctx,md4_file);
                        }

                        if(memcmp(md4_file,md4_hash,sizeof(hmd4_md4_t))==0)
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
                char fileline[sizeof(hmd4_md4_t)+4096]= {0};
                if(NULL!=fgets(fileline,sizeof(fileline)-1,stdin))
                {
                    const char *hash_string=NULL;
                    const char *filename_string=NULL;
                    size_t fileline_length=strlen(fileline);
                    while((fileline[fileline_length-1]=='\n') || (fileline[fileline_length-1]=='\r'))
                    {
                        fileline[fileline_length-1]='\0';
                        fileline_length--;
                    }
                    for(size_t i=0; i<fileline_length; i++)
                    {
                        if(fileline[i]!=' ')
                        {
                            if(hash_string==NULL)
                            {
                                hash_string=&fileline[i];
                            }
                            else
                            {
                                if(filename_string==NULL && fileline[i-1]=='\0')
                                {
                                    filename_string=&fileline[i];
                                }
                            }
                        }
                        else
                        {
                            if(hash_string==NULL || filename_string==NULL)
                            {
                                fileline[i]='\0';
                            }
                        }
                        if(hash_string!=NULL && filename_string!=NULL)
                        {
                            break;
                        }
                    }

                    if(hash_string==NULL || filename_string==NULL)
                    {
                        hfprintf(stderr,"line error!\r\n");
                        hexit(-1);
                    }

                    hmd4_md4_t md4_hash= {0};
                    hbase16_decode(md4_hash,sizeof(md4_hash),hash_string,strlen(hash_string));
                    hmd4_md4_t md4_file= {0};
                    {
                        hmd4_context_t ctx;
                        hmd4_starts(&ctx);
                        std::fstream file;
                        file.open(filename_string,std::ios::in|std::ios::binary);
                        if(!file.is_open())
                        {
                            hfprintf(stderr,"open %s failed!\r\n",filename_string);
                            hexit(-1);
                        }
                        while(!file.eof())
                        {
                            uint8_t buffer[sizeof(hmd4_message_block_t)];
                            file.read((char *)buffer,sizeof(buffer));
                            hmd4_update(&ctx,buffer,file.gcount());
                        }
                        file.close();
                        hmd4_finish(&ctx,md4_file);
                    }

                    if(memcmp(md4_file,md4_hash,sizeof(hmd4_md4_t))==0)
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
                    hmd4_context_t ctx;
                    hmd4_starts(&ctx);
                    int c=EOF;
                    while((c=getchar())!=EOF)
                    {
                        uint8_t ch=(uint8_t)c;
                        hmd4_update(&ctx,&ch,sizeof(ch));
                    }
                    hmd4_md4_t md4= {0};
                    hmd4_finish(&ctx,md4);
                    char output_string[sizeof(md4)*2+1]= {0};
                    hbase16_encode_with_null_terminator(output_string,sizeof(output_string),md4,sizeof(md4));
                    hfprintf(stdout,"%s -\r\n",output_string);
                }
                else
                {
                    hmd4_context_t ctx;
                    hmd4_starts(&ctx);
                    std::fstream file;
                    file.open(files->filename[i],std::ios::in|std::ios::binary);
                    if(!file.is_open())
                    {
                        continue;
                    }
                    while(!file.eof())
                    {
                        uint8_t buffer[sizeof(hmd4_message_block_t)];
                        file.read((char *)buffer,sizeof(buffer));
                        hmd4_update(&ctx,buffer,file.gcount());
                    }
                    file.close();
                    hmd4_md4_t md4= {0};
                    hmd4_finish(&ctx,md4);
                    char output_string[sizeof(md4)*2+1]= {0};
                    hbase16_encode_with_null_terminator(output_string,sizeof(output_string),md4,sizeof(md4));
                    hfprintf(stdout,"%s %s\r\n",output_string,files->filename[i]);
                }
            }
        }
        else
        {
            hmd4_context_t ctx;
            hmd4_starts(&ctx);
            int c=EOF;
            while((c=getchar())!=EOF)
            {
                uint8_t ch=(uint8_t)c;
                hmd4_update(&ctx,&ch,sizeof(ch));
            }
            hmd4_md4_t md4= {0};
            hmd4_finish(&ctx,md4);
            char output_string[sizeof(md4)*2+1]= {0};
            hbase16_encode_with_null_terminator(output_string,sizeof(output_string),md4,sizeof(md4));
            hfprintf(stdout,"%s -\r\n",output_string);
        }
    }

    arg_freetable(argtable,sizeof(argtable)/sizeof(argtable[0]));
}


int main(int argc,char *argv[])
{
    check_args(argc,argv);
    return 0;
}
