#include "hbox.h"
#include <string>
#include <fstream>
#include <hrc.h>



static int cmd_help(int argc,const char *argv[]);
static int cmd_dll(int argc,const char *argv[]);
static int cmd_relocatable(int argc,const char *argv[]);
static struct
{
    const char * cmd;
    const char * cmd_short;
    int (*cmd_entry)(int argc,const char *argv[]);
    const char * usage;
    const char * help;
} cmd_list[]=
{
    {
        "--help",
        "-h",
        cmd_help,
        "--help  / -h ",
        "get  help"
    },
    {
        "--dll",
        "-d",
        cmd_dll,
        "--dll / -d",
        "dll path"
    },
    {
        "--relocatable",
        "-r",
        cmd_relocatable,
        "--relocatable / -r",
        "relocatable path"
    }
};

static void print_help(int argc,const char *argv[])
{
    const size_t cmd_max_len=8;
    const size_t usage_max_len=32;
    {
        //打印banner
        const char *banner=(const char *)RCGetHandle("banner");
        if(banner!=NULL)
        {
            printf("%s",banner);
        }
    }
    {
        printf("\r\n%s [options] \r\n",argv[0]);
    }
    {
        //打印标题栏
        for(size_t i=0; i<cmd_max_len; i++)
        {
            printf("%s","-");
        }
        printf("\t");
        for(size_t i=0; i<usage_max_len; i++)
        {
            printf("%s","-");
        }
        printf("\r\n");
    }
    {
        for(size_t i=0; i<sizeof(cmd_list)/sizeof(cmd_list[0]); i++)
        {
            {
                //打印cmd
                printf("%s",cmd_list[i].cmd);
                int cmd_pad_len=(int)cmd_max_len-strlen(cmd_list[i].cmd);
                if(cmd_pad_len > 0)
                {
                    for(size_t i=0; i<cmd_pad_len; i++)
                    {
                        printf(" ");
                    }
                }
            }
            printf("\t");
            {
                //打印usage
                printf("%s",cmd_list[i].usage);
                int usage_pad_len=(int)usage_max_len-strlen(cmd_list[i].usage);
                if(usage_pad_len > 0)
                {
                    for(size_t i=0; i<usage_pad_len; i++)
                    {
                        printf(" ");
                    }
                }
            }
            printf("\t");
            {
                printf("%s",cmd_list[i].help);
            }
            printf("\r\n");
        }
    }
}

static int cmd_help(int argc,const char *argv[])
{

    print_help(argc,argv);

    //退出程序
    exit(0);
    return 0;
}

static std::string dll_path;
static int cmd_dll(int argc,const char *argv[])
{
    for(int i=0; i<argc; i++)
    {
        {
            char temp[4096]= {0};
            const char *para=NULL;
            strcat(temp,argv[i]);
            for(size_t k=0; k<strlen(temp); k++)
            {
                if(temp[k]=='=')
                {
                    temp[k]='\0';
                    para=&temp[k+1];
                    break;
                }
            }
            if(strcmp("--dll",temp)==0)
            {
                if(para!=NULL)
                {
                    dll_path=para;
                    break;
                }
            }
            if(strcmp("-d",argv[i])==0)
            {
                if((i+1)<argc)
                {
                    dll_path=argv[i+1];
                    break;
                }
            }
        }
    }

    return 0;
}

static std::string relocatable_path;
static int cmd_relocatable(int argc,const char *argv[])
{
    for(int i=0; i<argc; i++)
    {
        {
            char temp[4096]= {0};
            const char *para=NULL;
            strcat(temp,argv[i]);
            for(size_t k=0; k<strlen(temp); k++)
            {
                if(temp[k]=='=')
                {
                    temp[k]='\0';
                    para=&temp[k+1];
                    break;
                }
            }
            if(strcmp("--relocatable",temp)==0)
            {
                if(para!=NULL)
                {
                    relocatable_path=para;
                    break;
                }
            }
            if(strcmp("-r",argv[i])==0)
            {
                if((i+1)<argc)
                {
                    relocatable_path=argv[i+1];
                    break;
                }
            }
        }
    }

    return 0;
}

static void arg_parse(int argc,const char *argv[])
{
    {
        for(int i=0; i<argc; i++)
        {
            const char *cmd=argv[i];
            for(size_t j=0; j<sizeof(cmd_list)/sizeof(cmd_list[0]); j++)
            {
                if(cmd_list[j].cmd_short==NULL)
                {
                    cmd_list[j].cmd_short="";
                }
                if(cmd_list[j].cmd==NULL)
                {
                    cmd_list[j].cmd="";
                }
                if(strcmp(cmd,cmd_list[j].cmd_short)==0)
                {
                    if(cmd_list[j].cmd_entry!=NULL)
                    {
                        cmd_list[j].cmd_entry(argc,argv);
                    }
                }
                {
                    char temp[256]= {0};
                    strcat(temp,cmd);
                    for(size_t k=0; k<strlen(temp); k++)
                    {
                        if(temp[k]=='=')
                        {
                            temp[k]='\0';
                        }
                    }
                    if(strcmp(temp,cmd_list[j].cmd)==0)
                    {
                        if(cmd_list[j].cmd_entry!=NULL)
                        {
                            cmd_list[j].cmd_entry(argc,argv);
                        }
                    }
                }
            }
        }
    }
}

static int check_arg(int argc,const char *argv[])
{
    if(dll_path.empty() && relocatable_path.empty())
    {
        fprintf(stderr,"must specify dll path or relocatable path!\r\n");
        print_help(argc,argv);
        exit(-1);
    }

    return 0;
}

static int dumpplcdll(void)
{

    if(dll_path.empty())
    {
        return 0;
    }

    hsoftplc_dll_import_handle_t handle=hsoftplc_dll_load(dll_path.c_str());
    if(handle.hsoftplc_interface == NULL)
    {
        fprintf(stderr,"get interface failed!\r\n");
        exit(-1);
    }

    if(handle.hsoftplc_interface->interface_get_located_all_variables == NULL)
    {
        fprintf(stderr,"check interface failed!\r\n");
        exit(-1);
    }

    handle.hsoftplc_interface->interface_get_located_all_variables([](const char *name,void *var,void *usr)
    {
        hsoftplc_variable_name_t iec_addr;
        hsoftplc_variable_name_t variable_name;
        hsoftplc_get_iec_addr_from_variable_name(iec_addr,name);
        hsoftplc_get_variable_name_from_iec_addr(variable_name,iec_addr);
        hsoftplc_variable_symbol_t variable_symbol;
        hsoftplc_parse_variable_symbol(&variable_symbol,name);
        for(size_t i=0; i<sizeof(variable_symbol.variable_address)/sizeof(variable_symbol.variable_address[0]); i++)
        {
            if(variable_symbol.variable_address[i]==NULL)
            {
                variable_symbol.variable_address[i]="";
            }
        }
        printf("%s %c %c %s %s %s\r\n",iec_addr,variable_symbol.variable_location,variable_symbol.variable_size,variable_symbol.variable_address[0],variable_symbol.variable_address[1],variable_symbol.variable_address[2]);
    }
    ,NULL);

    hsoftplc_dll_unload(&handle);
    return 0;
}

static int dumpplcrelocatable(void)
{
    if(relocatable_path.empty())
    {
        return 0;
    }

    std::string filedata;

    {
        std::fstream file;
        file.open(relocatable_path.c_str(),std::ios::in | std::ios::binary);
        if(file.is_open())
        {
            filedata=std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            file.close();
        }
    }

    {
        //尝试coff
        auto cofffileread=[](hcoff_file_input_t *input,uintptr_t address,void *buffer,size_t buffer_length)->size_t
        {
            if(input==NULL || input->usr==NULL || buffer==NULL)
            {
                return 0;
            }
            std::string &filedata=*(std::string *)input->usr;
            if(filedata.length() > address)
            {
                size_t byte_to_read=buffer_length;
                if(byte_to_read > filedata.length()-address)
                {
                    byte_to_read=filedata.length()-address;
                }
                memcpy(buffer,&filedata.c_str()[address],byte_to_read);
                return byte_to_read;
            }
            return 0;
        };
        hcoff_file_input_t cofffile;
        hcoff_file_input_init(&cofffile,cofffileread,&filedata);
        hcoff_fileheader_t fileheader;
        if(hcoff_fileheader_read(&fileheader,(uint8_t *)filedata.c_str(),filedata.length()) && hcoff_fileheader_is_relocatable_object_file(&fileheader))
        {
            for(size_t i=0; i<fileheader.f_nsyms; i++)
            {
                if(hcoff_symbol_is_symbol(i,&cofffile))
                {
                    hcoff_symbol_entry_t symbol_entry;
                    if(hcoff_symbol_entry_read(&symbol_entry,i,&cofffile))
                    {
                        if(symbol_entry.e_sclass==HCOFF_SYMBOL_ENTRY_E_SCLASS_EXT && ((int16_t)symbol_entry.e_scnum)==0)
                        {
                            /*
                             * 外部符号
                             */
                            char namebuf[4096];
                            hcoff_symbol_entry_name_read(&symbol_entry,&cofffile,namebuf,sizeof(namebuf)-1);
                            {
                                /*
                                 * 编译器有时会添加一个下划线，需要去除.
                                 */
                                if(namebuf[0]=='_' && namebuf[1]=='_' &&  namebuf[2]=='_')
                                {
                                    for(size_t i=0; i < (sizeof(namebuf)-1-1); i++)
                                    {
                                        namebuf[i]=namebuf[i+1];
                                        if(namebuf[i]=='\0')
                                        {
                                            break;
                                        }
                                    }
                                }
                            }
                            if(namebuf[0]!='_' || namebuf[1]!='_')
                            {
                                /*
                                 * 下划线前缀
                                 */
                                continue;
                            }
                            if(!(namebuf[2]=='Q' || namebuf[2]=='I' || namebuf[2]=='M'))
                            {
                                /*
                                 * 位置
                                 */
                                continue;
                            }
                            if(!(namebuf[3]=='X' || namebuf[3]=='B' || namebuf[3]=='W' ||  namebuf[3]=='D' || namebuf[3]=='L' || ('0' <= namebuf[3] && '9' >= namebuf[3])))
                            {
                                /*
                                 * 位宽
                                 */
                                continue;
                            }

                            {
                                /*
                                 * 打印符号信息
                                 */
                                hsoftplc_variable_name_t iec_addr;
                                hsoftplc_variable_name_t variable_name;
                                hsoftplc_get_iec_addr_from_variable_name(iec_addr,namebuf);
                                hsoftplc_get_variable_name_from_iec_addr(variable_name,iec_addr);
                                hsoftplc_variable_symbol_t variable_symbol;
                                hsoftplc_parse_variable_symbol(&variable_symbol,namebuf);
                                for(size_t i=0; i<sizeof(variable_symbol.variable_address)/sizeof(variable_symbol.variable_address[0]); i++)
                                {
                                    if(variable_symbol.variable_address[i]==NULL)
                                    {
                                        variable_symbol.variable_address[i]="";
                                    }
                                }
                                printf("%s %c %c %s %s %s\r\n",iec_addr,variable_symbol.variable_location,variable_symbol.variable_size,variable_symbol.variable_address[0],variable_symbol.variable_address[1],variable_symbol.variable_address[2]);
                            }

                        }
                    }
                }
            }
            /*
             * 已作为coff读取，直接返回
             */
            return 0;
        }
    }

    {
        //尝试elf
        auto elffileread=[](helf_file_input_t *input,uintptr_t address,void *buffer,size_t buffer_length)->size_t
        {
            if(input==NULL || input->usr==NULL || buffer==NULL)
            {
                return 0;
            }
            std::string &filedata=*(std::string *)input->usr;
            if(filedata.length() > address)
            {
                size_t byte_to_read=buffer_length;
                if(byte_to_read > filedata.length()-address)
                {
                    byte_to_read=filedata.length()-address;
                }
                memcpy(buffer,&filedata.c_str()[address],byte_to_read);
                return byte_to_read;
            }
            return 0;
        };
        helf_file_input_t elffile;
        helf_file_input_init(&elffile,elffileread,&filedata);

        /*
         * 32位elf
         */
        {
            helf_elf32_file_header_t fileheader;
            if(helf_file_input_32_bits_header_get(&elffile,&fileheader))
            {
                size_t symbol_count=helf_file_input_32_bits_symbol_count_get(&elffile);
                for(size_t i=0; i<symbol_count; i++)
                {
                    helf_elf32_symbol_t symbol;
                    if(!helf_file_input_32_bits_symbol_get(&elffile,i,&symbol))
                    {
                        continue;
                    }

                    if(symbol.st_shndx==0 && HELF_ELF32_ST_BIND(symbol.st_info)==HELF_STB_GLOBAL && HELF_ELF32_ST_TYPE(symbol.st_info)==HELF_STT_NOTYPE)
                    {
                        /*
                         * 全局未定义符号
                         */
                        char namebuf[4096]= {0};
                        if(!helf_file_input_32_bits_symbol_name_get(&elffile,&symbol,namebuf,sizeof(namebuf)-1))
                        {
                            continue;
                        }

                        if(namebuf[0]!='_' || namebuf[1]!='_')
                        {
                            /*
                             * 下划线前缀
                             */
                            continue;
                        }
                        if(!(namebuf[2]=='Q' || namebuf[2]=='I' || namebuf[2]=='M'))
                        {
                            /*
                             * 位置
                             */
                            continue;
                        }
                        if(!(namebuf[3]=='X' || namebuf[3]=='B' || namebuf[3]=='W' ||  namebuf[3]=='D' || namebuf[3]=='L' || ('0' <= namebuf[3] && '9' >= namebuf[3])))
                        {
                            /*
                             * 位宽
                             */
                            continue;
                        }

                        {
                            /*
                             * 打印符号信息
                             */
                            hsoftplc_variable_name_t iec_addr;
                            hsoftplc_variable_name_t variable_name;
                            hsoftplc_get_iec_addr_from_variable_name(iec_addr,namebuf);
                            hsoftplc_get_variable_name_from_iec_addr(variable_name,iec_addr);
                            hsoftplc_variable_symbol_t variable_symbol;
                            hsoftplc_parse_variable_symbol(&variable_symbol,namebuf);
                            for(size_t i=0; i<sizeof(variable_symbol.variable_address)/sizeof(variable_symbol.variable_address[0]); i++)
                            {
                                if(variable_symbol.variable_address[i]==NULL)
                                {
                                    variable_symbol.variable_address[i]="";
                                }
                            }
                            printf("%s %c %c %s %s %s\r\n",iec_addr,variable_symbol.variable_location,variable_symbol.variable_size,variable_symbol.variable_address[0],variable_symbol.variable_address[1],variable_symbol.variable_address[2]);
                        }
                    }

                }
                return 0;
            }
        }

        /*
         * 64位elf
         */
        {
            helf_elf64_file_header_t fileheader;
            if(helf_file_input_64_bits_header_get(&elffile,&fileheader))
            {
                size_t symbol_count=helf_file_input_64_bits_symbol_count_get(&elffile);
                for(size_t i=0; i<symbol_count; i++)
                {
                    helf_elf64_symbol_t symbol;
                    if(!helf_file_input_64_bits_symbol_get(&elffile,i,&symbol))
                    {
                        continue;
                    }

                    if(symbol.st_shndx==0 && HELF_ELF64_ST_BIND(symbol.st_info)==HELF_STB_GLOBAL && HELF_ELF64_ST_TYPE(symbol.st_info)==HELF_STT_NOTYPE)
                    {
                        /*
                         * 全局未定义符号
                         */
                        char namebuf[4096]= {0};
                        if(!helf_file_input_64_bits_symbol_name_get(&elffile,&symbol,namebuf,sizeof(namebuf)-1))
                        {
                            continue;
                        }

                        if(namebuf[0]!='_' || namebuf[1]!='_')
                        {
                            /*
                             * 下划线前缀
                             */
                            continue;
                        }
                        if(!(namebuf[2]=='Q' || namebuf[2]=='I' || namebuf[2]=='M'))
                        {
                            /*
                             * 位置
                             */
                            continue;
                        }
                        if(!(namebuf[3]=='X' || namebuf[3]=='B' || namebuf[3]=='W' ||  namebuf[3]=='D' || namebuf[3]=='L' || ('0' <= namebuf[3] && '9' >= namebuf[3])))
                        {
                            /*
                             * 位宽
                             */
                            continue;
                        }

                        {
                            /*
                             * 打印符号信息
                             */
                            hsoftplc_variable_name_t iec_addr;
                            hsoftplc_variable_name_t variable_name;
                            hsoftplc_get_iec_addr_from_variable_name(iec_addr,namebuf);
                            hsoftplc_get_variable_name_from_iec_addr(variable_name,iec_addr);
                            hsoftplc_variable_symbol_t variable_symbol;
                            hsoftplc_parse_variable_symbol(&variable_symbol,namebuf);
                            for(size_t i=0; i<sizeof(variable_symbol.variable_address)/sizeof(variable_symbol.variable_address[0]); i++)
                            {
                                if(variable_symbol.variable_address[i]==NULL)
                                {
                                    variable_symbol.variable_address[i]="";
                                }
                            }
                            printf("%s %c %c %s %s %s\r\n",iec_addr,variable_symbol.variable_location,variable_symbol.variable_size,variable_symbol.variable_address[0],variable_symbol.variable_address[1],variable_symbol.variable_address[2]);
                        }
                    }
                }
                return 0;
            }
        }


    }

    return 0;
}

int main(int argc,const char *argv[])
{
    arg_parse(argc,argv);

    check_arg(argc,argv);

    dumpplcdll();

    dumpplcrelocatable();

    return 0;
}

