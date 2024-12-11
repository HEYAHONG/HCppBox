#include "HCPPBox.h"
#include "hbox.h"
#include "hrc.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include "stdlib.h"
#include "stdio.h"
#include <string>


static class ft_lib
{
    FT_Library library;
public:
    operator FT_Library()
    {
        return library;
    }
    ft_lib():library(NULL)
    {
        FT_Init_FreeType(&library);
    }
    ~ft_lib()
    {
        FT_Done_FreeType(library);
    }
} g_ft_lib;


static int cmd_help(int argc,const char *argv[]);
static int cmd_font(int argc,const char *argv[]);
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
        "--font",
        "-f",
        cmd_help,
        "--font=[font_file_path]  / -f [font_file_path] ",
        "fontfile path"
    }
};

static int cmd_help(int argc,const char *argv[])
{
    const size_t cmd_max_len=16;
    const size_t usage_max_len=50;
    {
        //打印banner
        const char *banner=(const char *)RCGetHandle("banner");
        if(banner!=NULL)
        {
            printf("%s",banner);
        }
    }
    {
        printf("\r\n%s [options]\r\n",argv[0]);
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
    //退出程序
    exit(0);
    return 0;
}

#ifdef WIN32
std::string font_file_path("C:/Windows/Fonts/simsun.ttc");
#else
//默认使用 文泉驿 字体,debian系统(最新版)使用sudo apt-get install fonts-wqy-zenhei安装后即可使用
std::string font_file_path("/usr/share/fonts/truetype/wqy/wqy-zenhei.ttc");
#endif
static int cmd_font(int argc,const char *argv[])
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
            if(strcmp("--font",temp)==0)
            {
                if(para!=NULL)
                {
                    font_file_path=para;
                    break;
                }
            }
            if(strcmp("-f",argv[i])==0)
            {
                if((i+1)<argc)
                {
                    font_file_path=argv[i+1];
                    break;
                }
            }
        }
    }

    return 0;
}

static void arg_parse(int argc,const char *argv[])
{
    if(argc==1)
    {
        cmd_help(argc,argv);
    }
    else
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


int main(int argc,const char *argv[])
{
    arg_parse(argc,argv);

    FT_Face face=NULL;

    if(FT_New_Face(g_ft_lib, font_file_path.c_str(), 0, &face))
    {
        printf("FT_New_Face (%s) error!\r\n",font_file_path.c_str());
        return -1;
    }

    //设置为unicode
    FT_Select_Charmap(face, FT_ENCODING_UNICODE);



    FT_Done_Face(face);
    return 0;
}


