#include "hbox.h"
#include <string>
#include <hrc.h>



static int cmd_help(int argc,const char *argv[]);
static int cmd_dll(int argc,const char *argv[]);
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
    if(dll_path.empty())
    {
        fprintf(stderr,"must specify dll path!\r\n");
        print_help(argc,argv);
        exit(-1);
    }

    return 0;
}

static int dumpplcdll(void)
{
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

int main(int argc,const char *argv[])
{
    arg_parse(argc,argv);

    check_arg(argc,argv);

    dumpplcdll();

    return 0;
}

