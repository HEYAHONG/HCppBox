/***************************************************************
 * Name:      hshell.c
 * Purpose:   实现hshell接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-03-15
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#include "hshell.h"
#include "h3rdparty.h"
#include "hcompiler.h"
#include "stdarg.h"


extern hshell_context_t *__hshell_context_check_context(hshell_context_t *ctx);

static int hshell_process_execute_arg_parse(hshell_context_t *ctx,char *cmdline);
int hshell_execute(hshell_context_t *ctx,char *cmdline)
{
    return hshell_process_execute_arg_parse(ctx,cmdline);
}

static void hshell_show_banner(hshell_context_t *ctx)
{
    hshell_context_t *context=__hshell_context_check_context(ctx);
    if(context->flags.show_banner==0)
    {
        return;
    }
    hshell_context_printf(context,"\r\n");
    hshell_context_printf(context," \\ | /\r\n");
    hshell_context_printf(context," | H |   build %04d/%02d/%02d %02d:%02d:%02d\r\n",hcompiler_get_date_year(),hcompiler_get_date_month(),hcompiler_get_date_day(),hcompiler_get_time_hour(),hcompiler_get_time_minute(),hcompiler_get_time_second());
    hshell_context_printf(context," / | \\\r\n");
}

static int hshell_login(hshell_context_t *ctx)
{
    hshell_context_t *context=__hshell_context_check_context(ctx);
    if(context->flags.login==0)
    {
        context->flags.login=1;
        //处理登录操作

        hshell_show_banner(context);
        context->flags.prompt=0;        //显示提示符
    }
    return 0;
}

static bool hshell_is_login(hshell_context_t *ctx)
{
    hshell_context_t *context=__hshell_context_check_context(ctx);
    return context->flags.login!=0;
}

static int hshell_internal_command_exit_entry(hshell_context_t *ctx,int *ret_code,int argc,const char *argv[])
{
    int ret=0;
    hshell_context_t *context=__hshell_context_check_context(ctx);
    if(ret_code!=NULL)
    {
        //内部返回码为EOF
        (*ret_code)=EOF;
    }

    {
        //若在子上下文中执行，退出子上下文并返回正常
        if(context->sub_context.prev!=NULL)
        {
            (*ret_code)=0;//返回0,而不退出
            hshell_subcontext_exit_from_sub(context);
        }
    }

    //退出登录
    context->flags.login=0;
    return ret;
}


static int hshell_internal_command_help_entry(hshell_context_t *ctx,int *ret_code,int argc,const char *argv[]);
static struct
{
    int (*entry)(hshell_context_t *ctx,int *ret_code,int argc,const char *argv[]);
    const char *name;
    const char *help;
} hshell_internal_command[] =
{
    {
        hshell_internal_command_exit_entry,
        "exit",
        "exit shell or exit subcontext"
    },
    {
        hshell_internal_command_help_entry,
        "help",
        "show help. \nhelp [command]"
    }
};


static int hshell_internal_command_help_entry(hshell_context_t *ctx,int *ret_code,int argc,const char *argv[])
{
    int ret=0;
    hshell_context_t *context=__hshell_context_check_context(ctx);
    if(argv[1]==NULL || strlen(argv[1])==0)
    {
        {
            size_t max_name_len=12;
            size_t max_help_len=32;
            for(size_t i=0; i<sizeof(hshell_internal_command)/sizeof(hshell_internal_command[0]); i++)
            {
                if(hshell_internal_command[i].entry!=NULL)
                {
                    if(hshell_internal_command[i].name!=NULL)
                    {
                        size_t name_len=strlen(hshell_internal_command[i].name);
                        if(name_len>max_name_len)
                        {
                            max_name_len=name_len;
                        }
                    }

                    if(hshell_internal_command[i].help!=NULL)
                    {
                        size_t help_len=0;
                        {
                            size_t len=strlen(hshell_internal_command[i].help);
                            for(size_t index=0;index<len;index++)
                            {
                                if(hshell_internal_command[i].help[index]=='\0')
                                {
                                    break;
                                }

                                if(hshell_internal_command[i].help[index]=='\n')
                                {
                                    help_len=0;
                                    continue;
                                }

                                help_len++;
                            }
                        }
                        if(help_len>max_help_len)
                        {
                            max_help_len=help_len;
                        }
                    }
                }
            }

            {
                //打印标题
                hshell_context_printf(context,"\r\ninternal commands:\r\n");
                for(size_t i=0; i<max_name_len; i++)
                {
                    hshell_context_printf(context,"-");
                }
                hshell_context_printf(context,"\t");
                for(size_t i=0; i<max_help_len; i++)
                {
                    hshell_context_printf(context,"-");
                }
                hshell_context_printf(context,"\r\n");
            }

            for(size_t i=0; i<sizeof(hshell_internal_command)/sizeof(hshell_internal_command[0]); i++)
            {
                if(hshell_internal_command[i].entry!=NULL)
                {
                    if(hshell_internal_command[i].name!=NULL)
                    {
                        hshell_context_printf(context,"%s",hshell_internal_command[i].name);
                        size_t name_len=strlen(hshell_internal_command[i].name);
                        for(size_t i=0; i<(max_name_len-name_len); i++)
                        {
                            hshell_context_printf(context," ");
                        }
                        hshell_context_printf(context,"\t");
                        if(hshell_internal_command[i].help!=NULL)
                        {
                            size_t len=strlen(hshell_internal_command[i].help);
                            for(size_t index=0;index<len;index++)
                            {
                                uint8_t ch=hshell_internal_command[i].help[index];
                                if(ch=='\n')
                                {
                                    if(index == 0 || hshell_internal_command[i].help[index-1]!='\r')
                                    {
                                        hshell_context_printf(context,"\r");
                                    }
                                    hshell_context_printf(context,"\n");
                                    for(size_t j=0;j<max_name_len;j++)
                                    {
                                        hshell_context_printf(context," ");
                                    }
                                    hshell_context_printf(context,"\t");
                                    continue;
                                }
                                hshell_context_printf(context,"%c",(char)ch);

                            }
                        }
                        hshell_context_printf(context,"\r\n");
                    }
                }
            }
        }

        if(context->command.array_base!=NULL && context->command.array_count!=0)
        {
            size_t max_name_len=12;
            size_t max_help_len=32;
            for(size_t i=0; i<context->command.array_count; i++)
            {
                if(context->command.array_base[i].entry!=NULL)
                {
                    if(context->command.array_base[i].name!=NULL)
                    {
                        size_t name_len=strlen(context->command.array_base[i].name);
                        if(name_len>max_name_len)
                        {
                            max_name_len=name_len;
                        }
                    }

                    if(context->command.array_base[i].help!=NULL)
                    {
                        size_t help_len=0;
                        {
                            size_t len=strlen(context->command.array_base[i].help);
                            for(size_t index=0;index<len;index++)
                            {
                                if(context->command.array_base[i].help[index]=='\0')
                                {
                                    break;
                                }

                                if(context->command.array_base[i].help[index]=='\n')
                                {
                                    help_len=0;
                                    continue;
                                }

                                help_len++;
                            }
                        }
                        if(help_len>max_help_len)
                        {
                            max_help_len=help_len;
                        }
                    }
                }
            }

            {
                //打印标题
                hshell_context_printf(context,"\r\ncommands:\r\n");
                for(size_t i=0; i<max_name_len; i++)
                {
                    hshell_context_printf(context,"-");
                }
                hshell_context_printf(context,"\t");
                for(size_t i=0; i<max_help_len; i++)
                {
                    hshell_context_printf(context,"-");
                }
                hshell_context_printf(context,"\r\n");
            }

            for(size_t i=0; i<context->command.array_count; i++)
            {
                if(context->command.array_base[i].entry!=NULL)
                {
                    if(context->command.array_base[i].name!=NULL)
                    {
                        hshell_context_printf(context,"%s",context->command.array_base[i].name);
                        size_t name_len=strlen(context->command.array_base[i].name);
                        for(size_t i=0; i<(max_name_len-name_len); i++)
                        {
                            hshell_context_printf(context," ");
                        }
                        hshell_context_printf(context,"\t");
                        if(context->command.array_base[i].help!=NULL)
                        {
                            size_t len=strlen(context->command.array_base[i].help);
                            for(size_t index=0;index<len;index++)
                            {
                                uint8_t ch=context->command.array_base[i].help[index];
                                if(ch=='\n')
                                {
                                    if(index == 0 || context->command.array_base[i].help[index-1]!='\r')
                                    {
                                        hshell_context_printf(context,"\r");
                                    }
                                    hshell_context_printf(context,"\n");
                                    for(size_t j=0;j<max_name_len;j++)
                                    {
                                        hshell_context_printf(context," ");
                                    }
                                    hshell_context_printf(context,"\t");
                                    continue;
                                }
                                hshell_context_printf(context,"%c",(char)ch);

                            }
                        }
                        hshell_context_printf(context,"\r\n");
                    }
                }
            }

        }
    }
    else
    {
        bool help_shown=false;
        if(context->command.array_base!=NULL && context->command.array_count!=0)
        {
            for(size_t i=0; i<context->command.array_count; i++)
            {
                if(context->command.array_base[i].name!=NULL && strcmp(argv[1],context->command.array_base[i].name)==0)
                {
                    hshell_context_printf(context,"%s:\r\n%s\r\n",context->command.array_base[i].name,context->command.array_base[i].help!=NULL?context->command.array_base[i].help:"");
                    help_shown=true;
                }
            }
        }

        if(!help_shown)
        {
            for(size_t i=0; i<sizeof(hshell_internal_command)/sizeof(hshell_internal_command[0]); i++)
            {
                if(hshell_internal_command[i].name!=NULL && strcmp(argv[1],hshell_internal_command[i].name)==0)
                {
                    hshell_context_printf(context,"%s:\r\n%s\r\n",hshell_internal_command[i].name,hshell_internal_command[i].help!=NULL?hshell_internal_command[i].help:"");
                    help_shown=true;
                }
            }
        }

        if(!help_shown)
        {
            //未找到命令，返回-1
            ret=-1;
            hshell_context_printf(context,"%s not found!\r\n",argv[1]);
        }
    }
    return ret;
}

static int hshell_process_execute_command_shortcut_strcmp(const char *str_short,const char *str_long)
{
    int ret=-1;
    size_t str_short_len=0;
    if(str_short == NULL || (str_short_len=strlen(str_short))==0)
    {
        return ret;
    }

    size_t str_long_len=0;
    if(str_long == NULL || (str_long_len=strlen(str_long))==0 )
    {
        return ret;
    }

    if(str_long_len < str_short_len)
    {
        str_short_len=str_long_len;
    }


    for(size_t i=0; i<str_short_len; i++)
    {
        ret=(str_short[i]-str_long[i]);
        if(ret!=0)
        {
            break;
        }
    }

    return ret;
}

static void hshell_backspace(hshell_context_t *ctx,size_t count)
{
    hshell_context_t *context=__hshell_context_check_context(ctx);
    for(size_t i=0; i<count; i++)
    {
        hshell_context_printf(context,"\b");
    }
}

static int hshell_process_execute_command(hshell_context_t *ctx,int argc,const char *argv[])
{
    int ret=0;
    hshell_context_t *context=__hshell_context_check_context(ctx);
    hshell_context_printf(context,"\r\n");
    bool command_processed=false;


    if(!command_processed)
    {
        if(context->command.array_base!=NULL && context->command.array_count!=0)
        {
            for(size_t i=0; i<context->command.array_count; i++)
            {
                if(context->command.array_base[i].name!=NULL && strcmp(argv[0],context->command.array_base[i].name)==0)
                {
                    if(context->command.array_base[i].entry!=NULL)
                    {
                        ret=0;
                        context->command_exit_code=context->command.array_base[i].entry(argc,argv);
                    }
                    command_processed=true;
                    break;
                }
            }
        }
    }

    if(!command_processed)
    {
        //处理内部命令
        for(size_t i=0; i<sizeof(hshell_internal_command)/sizeof(hshell_internal_command[0]); i++)
        {
            if(hshell_internal_command[i].name!=NULL && strcmp(argv[0],hshell_internal_command[i].name)==0)
            {
                if(hshell_internal_command[i].entry!=NULL)
                {
                    ret=0;
                    context->command_exit_code=hshell_internal_command[i].entry(context,&ret,argc,argv);
                }
                command_processed=true;
                break;
            }
        }
    }

    if(!command_processed)
    {
        if(context->flags.command_name_shortcut!=0)
        {
            int index_matched=-1; //匹配的引索。-1=初始化，-2=匹配的命令个数不唯一

            if(context->command.array_base!=NULL && context->command.array_count!=0)
            {
                for(size_t i=0; i<context->command.array_count; i++)
                {
                    if(context->command.array_base[i].name!=NULL && hshell_process_execute_command_shortcut_strcmp(argv[0],context->command.array_base[i].name)==0)
                    {
                        if(context->command.array_base[i].entry!=NULL)
                        {
                            if(index_matched>=0)
                            {
                                index_matched=-2;
                            }
                            if(index_matched==-1)
                            {
                                index_matched=i;
                            }
                        }
                    }
                }


                if(index_matched >= 0)
                {
                    context->command_exit_code=context->command.array_base[index_matched].entry(argc,argv);
                    command_processed=true;
                }
            }
        }
    }

    if(!command_processed)
    {
        if(context->api.invoke_command!=NULL)
        {
            ret=0;
            context->command_exit_code=context->api.invoke_command(argc,argv);
            command_processed=true;
        }
    }

    if(!command_processed)
    {
        hshell_context_printf(context,"%s not found!\r\n",argv[0]);
        for(int i=0; i<argc; i++)
        {
            hshell_context_printf(context,"\targv%d=%s\r\n",(int)i,argv[i]);
        }
    }

    return ret;
}

static void hshell_process_execute_arg_parse_strip_quotation_remove_one_char(char *str,size_t i)
{
    if(str==NULL)
    {
        return ;
    }
    size_t str_len=strlen(str);
    for(; i<str_len; i++)
    {
        str[i]=str[i+1];
    }
}

static void hshell_process_execute_arg_parse_strip_quotation(char *str)
{
    if(str==NULL)
    {
        return ;
    }
    size_t str_len=strlen(str);
    char quotation_char='\0';
    for(size_t i=0; i<str_len;)
    {
        bool need_index_inc=true;
        if(str[i]==(char)'\'' || str[i]==(char)'\"')
        {
            if(quotation_char=='\0')
            {
                bool enter_quotation=true;
                if(i>0)
                {
                    if(str[i-1]=='\\')
                    {
                        enter_quotation=false;
                    }
                }
                if(enter_quotation)
                {
                    quotation_char=str[i];
                    hshell_process_execute_arg_parse_strip_quotation_remove_one_char(str,i);
                    str_len=strlen(str);
                    need_index_inc=false;
                }
            }
            else
            {
                //引号结束
                bool exit_quotation=true;
                if(i>0)
                {
                    if(str[i-1]=='\\')
                    {
                        exit_quotation=false;
                    }
                }
                if(exit_quotation)
                {
                    quotation_char='\0';
                    hshell_process_execute_arg_parse_strip_quotation_remove_one_char(str,i);
                    str_len=strlen(str);
                    need_index_inc=false;
                }
            }
        }

        if(need_index_inc)
        {
            i++;
        }
    }
}

static void hshell_util_strip_c_escape_sequences_remove_chars(char *str_to_strip,size_t index,size_t count)
{
    if(str_to_strip==NULL)
    {
        return;
    }
    size_t len=strlen(str_to_strip);
    if((len < (index+count)) && (index < len))
    {
        str_to_strip[index]='\0';
    }
    while(index + count <= len)
    {
        str_to_strip[index]=str_to_strip[index+count];
        index++;
    }
}

static void hshell_util_strip_c_escape_sequences(char *str_to_strip)
{
    size_t len=strlen(str_to_strip);
    for(size_t i=0; i<len; i++)
    {
        if(str_to_strip[i]=='\\')
        {
            //默认为下一个字符
            uint8_t new_char=str_to_strip[i+1];
            int remove_chars_length=1;
            switch(new_char)
            {
            case '\'':
            {
                new_char=0x27;
            }
            break;
            case '\"':
            {
                new_char=0x22;
            }
            break;
            case '?':
            {
                new_char=0x3f;
            }
            break;
            case '\\':
            {
                new_char=0x5c;
            }
            break;
            case 'a':
            {
                new_char=0x07;
            }
            break;
            case 'b':
            {
                new_char=0x08;
            }
            break;
            case 'f':
            {
                new_char=0x0C;
            }
            break;
            case 'n':
            {
                new_char=0x0A;
            }
            break;
            case 'r':
            {
                new_char=0x0D;
            }
            break;
            case 't':
            {
                new_char=0x09;
            }
            break;
            case 'v':
            {
                new_char=0x0B;
            }
            break;
            default:
            {

            }
            break;
            }
            hshell_util_strip_c_escape_sequences_remove_chars(str_to_strip,i+1,remove_chars_length);
            if(str_to_strip[i]!='\0')
            {
                str_to_strip[i]=new_char;
            }
            len=strlen(str_to_strip);
        }
    }
}

static int hshell_process_execute_arg_parse(hshell_context_t *ctx,char *cmdline)
{
    int ret=0;
    hshell_context_t *context=__hshell_context_check_context(ctx);
    if(cmdline==NULL)
    {
        ret=-1;
        return ret;
    }
    int argc=0;
    /*
     * argv 前两项记录hshell上下文(幻数+指针)，最后一项为NULL
     */
    const char * argv[HSHELL_MAX_ARGC+3]= {0};
    argv[0]=(const char *)(uintptr_t)0x6873686C; // 幻数 hshl
    argv[1]=(const char *)context;
    {
        size_t current_ptr=0;
        size_t cmdline_len=strlen(cmdline);
        argv[2]=(const char *)&cmdline[current_ptr];
        char quotation_char='\0';//当前的引号值，为\0时表示当前未在引号范围内
        while(current_ptr < cmdline_len)
        {
            if(cmdline[current_ptr]=='\0')
            {
                //到达字符串末尾
                break;
            }

            if((cmdline[current_ptr]=='\'' || cmdline[current_ptr]=='\"')&&(quotation_char=='\0'))
            {
                //未进入引号时字符为引号
                bool enter_quotation=true;
                if(current_ptr>0)
                {
                    if(cmdline[current_ptr-1]=='\\')
                    {
                        enter_quotation=false;
                    }
                }
                if(enter_quotation)
                {
                    quotation_char=cmdline[current_ptr];
                }
            }

            if(cmdline[current_ptr]==' ' && quotation_char=='\0')
            {
                current_ptr++;
                if(argc >= (HSHELL_MAX_ARGC))
                {
                    //超过允许的参数
                    break;
                }
                if(cmdline[current_ptr]!=' ' && cmdline[current_ptr]!='\0')
                {
                    argv[2+argc]=(const char *)&cmdline[current_ptr];
                }
                continue;
            }

            current_ptr++;

            if(quotation_char!='\0' && cmdline[current_ptr]==quotation_char)
            {
                //引号结束
                bool exit_quotation=true;
                if(current_ptr>0)
                {
                    if(cmdline[current_ptr-1]=='\\')
                    {
                        exit_quotation=false;
                    }
                }
                if(exit_quotation)
                {
                    quotation_char='\0';
                    current_ptr++;
                }
            }

            if(cmdline[current_ptr]==' ' && quotation_char=='\0')
            {
                bool end_argv=true;
                if(current_ptr>0)
                {
                    if(cmdline[current_ptr-1]=='\\')
                    {
                        end_argv=false;
                    }
                }

                if(!end_argv)
                {
                    current_ptr++;
                    if(cmdline[current_ptr]==' ')
                    {
                        end_argv=true;
                    }
                }

                if(end_argv)
                {
                    cmdline[current_ptr]='\0';
                    current_ptr++;
                    argc++;
                    if(argc >= (HSHELL_MAX_ARGC))
                    {
                        //超过允许的参数
                        break;
                    }
                    if(cmdline[current_ptr]!=' ' && cmdline[current_ptr]!='\0')
                    {
                        argv[2+argc]=(const char *)&cmdline[current_ptr];
                    }
                }
            }


            if(cmdline[current_ptr]=='\0')
            {
                argc++;
                break;
            }

        }
    }

    {
        //修复 末尾为空格导致崩溃的BUG
        while (argc > 1)
        {
            if (argv[2 + argc - 1] != NULL && strlen(argv[2 + argc - 1]) > 0)
            {
                break;
            }
            argv[2 + argc - 1]=NULL;
            argc--;
        }
    }

    {
        //去除引号
        for(size_t i=0; i<argc; i++)
        {
            hshell_process_execute_arg_parse_strip_quotation((char *)argv[2 + i]);
        }
    }

    {
        //去除C语言转义序列
        for(size_t i=0; i<argc; i++)
        {
            hshell_util_strip_c_escape_sequences((char *)argv[2 + i]);
        }
    }

    if(argc > 0)
    {
        // 执行命令
        ret=hshell_process_execute_command(context,argc,&argv[2]);
    }

    return ret;
}

static void hshell_input_buffer_clear(hshell_context_t *ctx)
{
    hshell_context_t *context=__hshell_context_check_context(ctx);
    {
        //复位buffer
        memset(context->buffer,0,sizeof(context->buffer));
        context->buffer_ptr=0;
    }
}

static void hshell_history_store(hshell_context_t *ctx)
{
    hshell_context_t *context=__hshell_context_check_context(ctx);
    if(context->flags.echo==0)
    {
        return;
    }
#if HSHELL_MAX_HISTORY_COUNT > 0
    size_t current_index=(context->history.store_ptr++)%(sizeof(context->history.history)/sizeof(context->history.history[0]));
    memcpy(&context->history.history[current_index],context->buffer,sizeof(context->history.history[0]));
    context->history.load_ptr=current_index;
#endif // HSHELL_MAX_HISTORY_COUNT
}

static void hshell_history_load_next(hshell_context_t *ctx)
{
    hshell_context_t *context=__hshell_context_check_context(ctx);
    if(context->flags.echo==0)
    {
        return;
    }
    if(context->history.store_ptr == 0)
    {
        return;
    }
    {
        hshell_backspace(context,context->buffer_ptr);
        {
            size_t len=strlen((char *)context->buffer);
            for(size_t i=0; i<len; i++)
            {
                hshell_context_printf(context," ");
            }
            hshell_backspace(context,len);
        }
        hshell_input_buffer_clear(context);
    }
#if HSHELL_MAX_HISTORY_COUNT > 0
    size_t current_index=(context->history.load_ptr);
    if(((current_index > (context->history.store_ptr-1)) && ((context->history.store_ptr-1) < sizeof(context->history.history)/sizeof(context->history.history[0]))) || (current_index >= (sizeof(context->history.history)/sizeof(context->history.history[0]))))
    {
        current_index=0;
        context->history.load_ptr=current_index;
    }
    memcpy(context->buffer,&context->history.history[current_index],sizeof(context->history.history[0]));
    context->history.load_ptr++;
#endif // HSHELL_MAX_HISTORY_COUNT
    {
        size_t len=strlen((char *)context->buffer);
        for(size_t i=0; i<len; i++)
        {
            hshell_context_printf(context,"%c",context->buffer[i]);
            context->buffer_ptr++;
        }
    }
}

static void hshell_history_load_prev(hshell_context_t *ctx)
{
    hshell_context_t *context=__hshell_context_check_context(ctx);
    if(context->flags.echo==0)
    {
        return;
    }
    if(context->history.store_ptr == 0)
    {
        return;
    }
    {
        hshell_backspace(context,context->buffer_ptr);
        {
            size_t len=strlen((char *)context->buffer);
            for(size_t i=0; i<len; i++)
            {
                hshell_context_printf(context," ");
            }
            hshell_backspace(context,len);
        }
        hshell_input_buffer_clear(context);
    }
#if HSHELL_MAX_HISTORY_COUNT > 0
    {
        size_t current_index=(context->history.load_ptr);
        if(((current_index > (context->history.store_ptr-1)) && ((context->history.store_ptr-1) < sizeof(context->history.history)/sizeof(context->history.history[0]))) || (current_index >= (sizeof(context->history.history)/sizeof(context->history.history[0]))))
        {
            if(((context->history.store_ptr-1) < sizeof(context->history.history)/sizeof(context->history.history[0])))
            {
                current_index=(((context->history.store_ptr-1))%(sizeof(context->history.history)/sizeof(context->history.history[0])));
            }
            else
            {
                current_index=((sizeof(context->history.history)/sizeof(context->history.history[0]))-1);
            }
            context->history.load_ptr=current_index;
        }
        memcpy(context->buffer,&context->history.history[current_index],sizeof(context->history.history[0]));
        context->history.load_ptr--;
    }
#endif // HSHELL_MAX_HISTORY_COUNT
    {
        size_t len=strlen((char *)context->buffer);
        for(size_t i=0; i<len; i++)
        {
            hshell_context_printf(context,"%c",context->buffer[i]);
            context->buffer_ptr++;
        }
    }
}

static int hshell_process_execute(hshell_context_t *ctx)
{
    int ret=0;
    hshell_context_t *context=__hshell_context_check_context(ctx);

    if(context->buffer[0]!='\0')
    {
        hshell_history_store(ctx);
        ret=hshell_process_execute_arg_parse(context,(char *)context->buffer);
    }

    if(context->flags.login!=0)
    {
        //清零提示符标志，显示提示符
        context->flags.prompt=0;
    }

    //清空输入
    hshell_input_buffer_clear(context);

    return ret;
}

static int hshell_process_control(hshell_context_t *ctx)
{
    int ret=0;
    hshell_context_t *context=__hshell_context_check_context(ctx);
    if(context->flags.escape!=0)
    {
        int ch=hshell_context_getchar(context);
        if(ch==EOF)
        {
            ret=EOF;
        }
        else
        {
            uint8_t ch_val=ch&0xFF;
            size_t seq_len=strlen((char *)context->escape_sequence);
            if(seq_len>=(sizeof(context->escape_sequence)-1))
            {
                //超过能存储的序列长度
                context->flags.escape=0;
                memset(context->escape_sequence,0,sizeof(context->escape_sequence));
            }
            else
            {
                context->escape_sequence[seq_len]=ch_val;
            }
        }

        if(context->escape_sequence[0]!='\0')
        {
            //处理转义序列
            bool escape_processed=false;

            hshell_ctlseq_control_set_t ctrl_char=HSHELL_CTLSEQ_CONTROL_SET_INVAILD;
            {
                uint8_t control_bytes[sizeof(context->escape_sequence)]= {0};
                for(size_t i=0; i<(sizeof(control_bytes)-1); i++)
                {
                    control_bytes[i+1]=context->escape_sequence[i];
                }
                control_bytes[0]=((uint8_t)HSHELL_CTLSEQ_CONTROL_SET_C0_ESC);
                ctrl_char=hshell_ctlseq_control_set_get_from_bytes(control_bytes,sizeof(control_bytes));
            }

            switch(ctrl_char)
            {
            case HSHELL_CTLSEQ_CONTROL_SET_C1_CSI:
            {
                if(!escape_processed && strcmp((char *)context->escape_sequence,"[A")==0)
                {
                    //上键(上一条历史记录)
                    escape_processed=true;
                    hshell_history_load_next(context);
                }

                if(!escape_processed && strcmp((char *)context->escape_sequence,"[B")==0)
                {
                    //下键（下一条历史记录）
                    escape_processed=true;
                    hshell_history_load_prev(context);
                }

                if(!escape_processed && strcmp((char *)context->escape_sequence,"[C")==0)
                {
                    //右键
                    escape_processed=true;
                    if(context->buffer[context->buffer_ptr]!='\0' && context->flags.echo != 0)
                    {
                        hshell_context_printf(context,"%c",(char)context->buffer[context->buffer_ptr]);
                        context->buffer_ptr++;
                    }

                }

                if(!escape_processed && strcmp((char *)context->escape_sequence,"[D")==0)
                {
                    //左键
                    escape_processed=true;
                    if(context->buffer_ptr>0 && context->flags.echo != 0)
                    {
                        hshell_backspace(context,1);
                        context->buffer_ptr--;
                    }
                }

                if(!escape_processed && strcmp((char *)context->escape_sequence,"[2~")==0)
                {
                    //insert键
                    escape_processed=true;
                    if(context->flags.insert_mode!=0)
                    {
                        context->flags.insert_mode=0;
                    }
                    else
                    {
                        context->flags.insert_mode=1;
                    }
                }

                if(!escape_processed && strcmp((char *)context->escape_sequence,"[3~")==0)
                {
                    //del键
                    escape_processed=true;
                    if(context->flags.echo != 0)
                    {
                        size_t char_count=0;
                        for(size_t i=context->buffer_ptr; i<(sizeof(context->buffer)-1); i++)
                        {
                            if(context->buffer[i+1]!='\0')
                            {
                                context->buffer[i]=context->buffer[i+1];
                                hshell_context_printf(context,"%c",(char)context->buffer[i]);
                                char_count++;
                            }
                            else
                            {
                                context->buffer[i]='\0';
                                hshell_context_printf(context," ");
                                char_count++;
                                break;
                            }
                        }
                        hshell_backspace(context,char_count);
                    }
                }

            }
            break;
            default:
            {
                //不支持的控制字符，直接退出
                escape_processed=true;
            }
            break;
            }

            if(escape_processed)
            {
                context->flags.escape=0;
                memset(context->escape_sequence,0,sizeof(context->escape_sequence));
            }
        }
    }
    return ret;
}


static int hshell_process_input_start_execute(hshell_context_t *ctx)
{
    int ret=0;
    hshell_context_t *context=__hshell_context_check_context(ctx);
    context->buffer_ptr=strlen((char *)context->buffer);//将指针放在末尾
    ret=hshell_process_execute(context);
    return ret;
}

static bool hshell_process_input_check_complete(hshell_context_t *ctx)
{
    hshell_context_t *context=__hshell_context_check_context(ctx);
    if(context->flags.input_complete!=0)
    {
        context->flags.input_complete=0;
        return true;
    }
    if((context->buffer_ptr == (sizeof(context->buffer)-1)) || (strlen((char *)context->buffer) >= (sizeof(context->buffer)-1)))
    {
        //缓冲区即将溢出
        return true;
    }
    return false;
}

static void hshell_process_input_strip_comments(char *line)
{
    if(line==NULL)
    {
        return;
    }
    size_t line_len=strlen(line);
    char quotation_char='\0';
    for(size_t i=0; i<line_len; i++)
    {
        if(line[i]==(char)'\'' || line[i]==(char)'\"')
        {
            if(quotation_char=='\0')
            {
                bool enter_quotation=true;
                if(i>0)
                {
                    if(line[i-1]=='\\')
                    {
                        enter_quotation=false;
                    }
                }
                if(enter_quotation)
                {
                    quotation_char=line[i];
                }
            }
            else
            {
                //引号结束
                bool exit_quotation=true;
                if(i>0)
                {
                    if(line[i-1]=='\\')
                    {
                        exit_quotation=false;
                    }
                }
                if(exit_quotation)
                {
                    quotation_char='\0';
                }
            }
        }
        if(line[i]=='#')
        {
            bool need_strip=true;
            if(i>0 && line[i-1]=='\\')
            {
                need_strip=false;
            }
            if(need_strip && quotation_char!='\0')
            {
                need_strip=false;
            }
            if(need_strip)
            {
                line[i]='\0';
                break;
            }
        }
    }
}

static int hshell_process_input(hshell_context_t *ctx)
{
    int ret=0;
    hshell_context_t *context=__hshell_context_check_context(ctx);
    if(context->flags.escape!=0)
    {
        //正在转义过程中，不接受数据
        return ret;
    }
    int ch=hshell_context_getchar(context);
    bool need_echo=true;
    switch(ch)
    {
    case HSHELL_CTLSEQ_CONTROL_SET_C0_EOT: //EOT,终端中可通过Ctrl-D触发。
    case EOF:
    {
        ret=EOF;
        need_echo=false;
    }
    break;
    case HSHELL_CTLSEQ_CONTROL_SET_C0_HT:  //HT,水平制表符，可通过Tab触发
    {
        need_echo=false;
    }
    break;
    case HSHELL_CTLSEQ_CONTROL_SET_C0_CR:
    {
        context->flags.return_newline_compatible=1;
        context->flags.input_complete=1;
        hshell_process_input_strip_comments((char *)context->buffer);
    }
    break;
    case HSHELL_CTLSEQ_CONTROL_SET_C0_LF:
    {
        //处理字符串
        if( strlen((const char *)context->buffer)>0)
        {
            context->flags.input_complete=1;
            hshell_process_input_strip_comments((char *)context->buffer);
        }
        else
        {
            if(context->flags.return_newline_compatible==0)
            {
                context->flags.input_complete=1;
            }
        }
        context->flags.return_newline_compatible=0;
    }
    break;
    case HSHELL_CTLSEQ_CONTROL_SET_C0_ESC:
    {
        //处理特殊转义序列
        need_echo=false;
        context->flags.escape=1;
    }
    break;
    case HSHELL_CTLSEQ_CONTROL_SET_C0_BS:
    case HSHELL_CTLSEQ_CONTROL_SET_DEL: //   删除字符
    {
        if(context->buffer_ptr>0 &&context->flags.echo != 0)
        {
            hshell_backspace(context,1);
            hshell_context_printf(context," ");
            hshell_backspace(context,1);
            size_t char_count=0;
            for(size_t i=(context->buffer_ptr-1); i<(sizeof(context->buffer)-1); i++)
            {
                if(context->buffer[i+1]!='\0')
                {
                    context->buffer[i]=context->buffer[i+1];
                    hshell_context_printf(context,"%c",(char)context->buffer[i]);
                    char_count++;
                }
                else
                {
                    context->buffer[i]='\0';
                    hshell_context_printf(context," ");
                    char_count++;
                    break;
                }
            }
            hshell_backspace(context,char_count);
            context->buffer_ptr--;
        }
        need_echo=false;
    }
    break;
    default:
    {
        if(context->buffer_ptr < (sizeof(context->buffer)-1))
        {
            uint8_t ch_val=(ch&0xFF);
            if(ch_val>=0x20)
            {
                if(context->flags.insert_mode==0 || context->flags.echo==0)
                {
                    context->buffer[context->buffer_ptr++]=ch_val;
                }
                else
                {
                    need_echo=false;
                    uint8_t old_ch_val=context->buffer[context->buffer_ptr];
                    context->buffer[context->buffer_ptr++]=ch_val;
                    hshell_context_printf(context,"%c",(char)ch_val);
                    size_t char_count=0;
                    for(size_t i=context->buffer_ptr; i<(sizeof(context->buffer)-1); i++)
                    {
                        if(old_ch_val=='\0')
                        {
                            context->buffer[i]='\0';
                            break;
                        }
                        uint8_t temp=context->buffer[i];
                        context->buffer[i]=old_ch_val;
                        old_ch_val=temp;
                        hshell_context_printf(context,"%c",(char)context->buffer[i]);
                        char_count++;
                    }
                    hshell_backspace(context,char_count);
                }
            }
        }
    }
    break;
    }

    //检查是否可执行
    if(hshell_process_input_check_complete(ctx))
    {
        hshell_process_input_strip_comments((char *)context->buffer);
        ret=hshell_process_input_start_execute(ctx);
    }

    //回显字符
    if(context->flags.echo!=0 && need_echo)
    {
        hshell_context_putchar(context,ch);
    }

    return ret;
}

static int hshell_show_prompt_string(hshell_context_t *ctx)
{
    int ret=0;
    hshell_context_t *context=__hshell_context_check_context(ctx);
    if(context->flags.login!=0 && context->flags.prompt==0)
    {
        context->flags.prompt=1;
        //打印提示符
        hshell_context_printf(context,"%s",(context->prompt!=NULL)?context->prompt:"");
    }
    return ret;
}

int hshell_loop(hshell_context_t *ctx)
{
    int ret=0;
    hshell_context_t *context=__hshell_context_check_context(ctx);

    if(context->sub_context.next!=NULL)
    {
        hshell_context_t *sub_context=__hshell_context_check_context(context->sub_context.next);
        sub_context->api=context->api;
        sub_context->sub_context.prev=context;
        return hshell_loop(context->sub_context.next);
    }

    if((ret=hshell_login(context))<0)
    {
        return ret;
    }

    if(!hshell_is_login(context))
    {
        return ret;
    }

    if((ret=hshell_show_prompt_string(context))<0)
    {
        return ret;
    }

    if((ret=hshell_process_control(context))<0)
    {
        return ret;
    }

    if((ret=hshell_process_input(context))<0)
    {
        return ret;
    }

    return ret;
}

static void hshell_printf_out(char character, void* arg)
{
    hshell_context_putchar((hshell_context_t *)arg,(uint8_t)character);
}

int hshell_printf(hshell_context_t *ctx,const char *fmt,...)
{
    hshell_context_t *context=__hshell_context_check_context(ctx);
    int ret=0;
    va_list va;
    va_start(va, fmt);
    ret=hvfctprintf(hshell_printf_out,context,fmt,va);
    va_end(va);
    return ret;
}

