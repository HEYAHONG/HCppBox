#include "shell.h"
#include "stdint.h"
#include "stdlib.h"
#include "ctype.h"
#include "stdbool.h"

static LIBMONO_DATA_ATTRIBUTE uint8_t libmono_shell_buffer[LIBMONO_SHELL_BUFFER_LENGTH+1]= {0};
static LIBMONO_DATA_ATTRIBUTE size_t  libmono_shell_buffer_index=0;
static LIBMONO_DATA_ATTRIBUTE struct
{
    uint8_t         banner:1;                   /**<显示banner  */
    uint8_t         echo_enable:1;              /**<启用回显 */
} libmono_shell_flags;

static LIBMONO_CONST_DATA_ATTRIBUTE const char *const libmono_shell_prompt_string="shell>";

/*
 * 命令列表（具体格式见libmono_shell_command_t）
 */
#if !defined(LIBMONO_SHELL_COMMAND_LIST)
#define LIBMONO_SHELL_COMMAND_LIST
#endif

static void libmono_shell_command_internal_help_entry(char *cmd) LIBMONO_FUNCTION_ATTRIBUTE;
static void libmono_shell_command_internal_date_entry(char *cmd) LIBMONO_FUNCTION_ATTRIBUTE;
static LIBMONO_CONST_DATA_ATTRIBUTE const libmono_shell_command_t libmono_shell_command_internal[]=
{
    {
        libmono_shell_command_internal_help_entry,
        "help",
        "help [command],show command help"
    },
    {
        libmono_shell_command_internal_date_entry,
        "date",
        "show date"
    },
    LIBMONO_SHELL_COMMAND_LIST
};



static void libmono_shell_command_internal_help_entry(char *cmd) LIBMONO_FUNCTION_ATTRIBUTE
{
    size_t cmd_offset=4;

    /*
     * 去除参数1的空格
     */
    while(cmd[cmd_offset]==' ')
    {
        cmd_offset++;
    }

    if(cmd[cmd_offset]=='\0')
    {
        /*
         * 无参数
         */
        printf("command:\r\n");
        for(size_t i=0; i<sizeof(libmono_shell_command_internal)/sizeof(libmono_shell_command_internal[0]); i++)
        {
            if(libmono_shell_command_internal[i].name!=NULL && libmono_shell_command_internal[i].help!=NULL)
            {
                printf("\t%s\t%s\r\n",libmono_shell_command_internal[i].name,libmono_shell_command_internal[i].help);
            }
        }

    }
    else
    {
        /*
         * 参数1为命令名称
         */
        for(size_t i=0; i<sizeof(libmono_shell_command_internal)/sizeof(libmono_shell_command_internal[0]); i++)
        {
            if(libmono_shell_command_internal[i].name!=NULL && libmono_shell_command_internal[i].help!=NULL)
            {
                size_t cmd_para1_offset=cmd_offset;

                while(cmd[cmd_para1_offset]==libmono_shell_command_internal[i].name[cmd_para1_offset-cmd_offset])
                {
                    if(cmd[cmd_para1_offset]==' ' || cmd[cmd_para1_offset]=='\0')
                    {
                        break;
                    }
                    if(libmono_shell_command_internal[i].name[cmd_para1_offset-cmd_offset]=='\0')
                    {
                        break;
                    }
                    cmd_para1_offset++;
                }

                if((cmd[cmd_para1_offset]==' ' || cmd[cmd_para1_offset]=='\0') && libmono_shell_command_internal[i].name[cmd_para1_offset-cmd_offset]=='\0')
                {
                    printf("%s\t%s\r\n",libmono_shell_command_internal[i].name,libmono_shell_command_internal[i].help);
                }
            }
        }
    }
}

static void libmono_shell_command_internal_date_entry(char *cmd) LIBMONO_FUNCTION_ATTRIBUTE
{
    (void)cmd;
    time_t time_now=htime(NULL);
    htm_t *cur=hlocaltime_r(&time_now,NULL);
    printf("%04d-%02d-%02d %02d:%02d:%02d\r\n",cur->tm_year+1900,cur->tm_mon+1,cur->tm_mday,cur->tm_hour,cur->tm_min,cur->tm_sec);
}

void libmono_shell_process(int ch) LIBMONO_FUNCTION_ATTRIBUTE
{
    if(libmono_shell_flags.banner==0)
    {
        libmono_shell_flags.banner=1;
        printf("\r\n");
        printf(" \\ | /\r\n");
        printf(" | H |   build %s %s\r\n",__DATE__,__TIME__);
        printf(" / | \\\r\n");
        printf("%s",libmono_shell_prompt_string);
        /*
         * 默认启用回显
         */
        libmono_shell_flags.echo_enable=1;
    }

    if(ch < 0 || ch > 0xFF)
    {
        return;
    }

    bool start_process=false;

    if(isprint(ch))
    {
        libmono_shell_buffer[libmono_shell_buffer_index]=(uint8_t)ch;
        libmono_shell_buffer_index++;
        if(libmono_shell_flags.echo_enable!=0)
        {
            putchar(ch);
        }
        if(libmono_shell_buffer_index==LIBMONO_SHELL_BUFFER_LENGTH)
        {
            start_process=true;
        }
    }
    else
    {
        if(ch=='\n' || ch=='\r')
        {
            if(libmono_shell_flags.echo_enable!=0)
            {
                putchar(ch);
            }
            start_process=true;
        }
    }

    if(libmono_shell_buffer_index==0 && start_process)
    {
        printf("%s",libmono_shell_prompt_string);
        start_process=false;
    }

    if(start_process)
    {
        bool start_process=false;
        for(size_t i=0; i<sizeof(libmono_shell_command_internal)/sizeof(libmono_shell_command_internal[0]); i++)
        {
            if(libmono_shell_command_internal[i].entry!=NULL && libmono_shell_command_internal[i].name!=NULL)
            {
                size_t str_offset=0;
                /*
                 * 去除空格
                 */
                while(libmono_shell_buffer[str_offset]==' ')
                {
                    if(libmono_shell_buffer[str_offset]=='\0')
                    {
                        break;
                    }
                    str_offset++;
                }

                size_t name_offset=0;

                /*
                 * 去掉重复项目
                 */
                while(libmono_shell_buffer[str_offset]==libmono_shell_command_internal[i].name[name_offset])
                {
                    if(libmono_shell_buffer[str_offset]=='\0' || libmono_shell_buffer[str_offset]==' ')
                    {
                        break;
                    }
                    if(libmono_shell_command_internal[i].name[name_offset]=='\0')
                    {
                        break;
                    }
                    str_offset++;
                    name_offset++;
                }

                if(libmono_shell_command_internal[i].name[name_offset]=='\0' && (libmono_shell_buffer[str_offset]=='\0' || libmono_shell_buffer[str_offset]==' '))
                {
                    /*
                     * 成功匹配
                     */
                    start_process=true;
                    libmono_shell_command_internal[i].entry((char *)&libmono_shell_buffer[0]);
                }

            }
        }

        if(!start_process)
        {
            printf("command not found!\r\n");
        }

        /*
         * 处理结束，重置buffer，显示提示符
         */
        libmono_shell_buffer_index=0;
        printf("%s",libmono_shell_prompt_string);
    }
}
