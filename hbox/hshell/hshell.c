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

extern int getchar(void);
extern int putchar(int ch);
hshell_context_external_api_t hshell_context_default_external_api(void)
{
    hshell_context_external_api_t api= {0};
    api.getchar=getchar;
    api.putchar=putchar;
    return api;
}

const char * hshell_context_default_prompt_string(void)
{
    return "hshell>";
}

static hshell_context_t default_context= {0};
static hshell_context_t *hshell_context_real_context_get(hshell_context_t *ctx)
{
    if(ctx==NULL)
    {
        return &default_context;
    }
    else
    {
        return ctx;
    }
}

hshell_context_t *hshell_context_default_context(void)
{
    return hshell_context_real_context_get(NULL);
}

void hshell_context_init(hshell_context_t *ctx)
{
    hshell_context_t *real_context=hshell_context_real_context_get(ctx);
    real_context->prompt=hshell_context_default_prompt_string();
    real_context->api=hshell_context_default_external_api();
    real_context->flags.init=1;
    real_context->flags.login=0;
    real_context->flags.prompt=0;
    real_context->flags.echo=1;         //默认打开回显
    memset(real_context->buffer,0,sizeof(real_context->buffer));
    real_context->buffer_ptr=0;
}

static hshell_context_t *hshell_context_check_context(hshell_context_t *ctx)
{
    hshell_context_t *real_context=hshell_context_real_context_get(ctx);
    if(real_context->flags.init!=1)
    {
        hshell_context_init(real_context);
    }
    return real_context;
}

const char *hshell_prompt_string_set(hshell_context_t *ctx,const char *prompt)
{
    hshell_context_t *context=hshell_context_check_context(ctx);
    const char *old_prompt_string=context->prompt;
    if(prompt!=NULL)
    {
        context->prompt=prompt;
    }
    return old_prompt_string;
}

const char *hshell_prompt_string_get(hshell_context_t *ctx)
{
    hshell_context_t *context=hshell_context_check_context(ctx);
    return context->prompt;
}

hshell_context_external_api_t hshell_external_api_set(hshell_context_t *ctx,hshell_context_external_api_t api)
{
    hshell_context_t *context=hshell_context_check_context(ctx);
    hshell_context_external_api_t old_api=context->api;
    context->api=api;
    return old_api;
}

hshell_context_external_api_t hshell_external_api_get(hshell_context_t *ctx)
{
    hshell_context_t *context=hshell_context_check_context(ctx);
    return context->api;
}

bool hshell_echo_set(hshell_context_t *ctx,bool echo)
{
    hshell_context_t *context=hshell_context_check_context(ctx);
    bool old_echo=(context->flags.echo!=0);
    context->flags.echo=(echo?0x1:0x0);
    return old_echo;
}

bool hshell_echo_get(hshell_context_t *ctx)
{
    hshell_context_t *context=hshell_context_check_context(ctx);
    return context->flags.echo!=0;
}

int hshell_getchar(hshell_context_t *ctx)
{
    hshell_context_t *context=hshell_context_check_context(ctx);
    if(context->api.getchar!=NULL)
    {
        return context->api.getchar();
    }
    else
    {
        return hshell_context_default_external_api().getchar();
    }
}

int hshell_putchar(hshell_context_t *ctx,int ch)
{
    hshell_context_t *context=hshell_context_check_context(ctx);
    if(context->api.putchar!=NULL)
    {
        return context->api.putchar(ch);
    }
    else
    {
        return hshell_context_default_external_api().putchar(ch);
    }
}

static void hshell_printf_out(char character, void* arg)
{
    hshell_putchar((hshell_context_t *)arg,(uint8_t)character);
}

int hshell_printf(hshell_context_t *ctx,const char *fmt,...)
{
    hshell_context_t *context=hshell_context_check_context(ctx);
    int ret=0;
    va_list va;
    va_start(va, fmt);
    ret=hvfctprintf(hshell_printf_out,context,fmt,va);
    va_end(va);
    return ret;
}

static void hshell_show_banner(hshell_context_t *ctx)
{
    hshell_context_t *context=hshell_context_check_context(ctx);
    hshell_printf(context," \\ | /\r\n");
    hshell_printf(context," | H |   build %04d/%02d/%02d %02d:%02d:%02d\r\n",hcompiler_get_date_year(),hcompiler_get_date_month(),hcompiler_get_date_day(),hcompiler_get_time_hour(),hcompiler_get_time_minute(),hcompiler_get_time_second());
    hshell_printf(context," / | \\\r\n");
}

static int hshell_login(hshell_context_t *ctx)
{
    hshell_context_t *context=hshell_context_check_context(ctx);
    if(context->flags.login==0)
    {
        context->flags.login=1;
        //处理登录操作

        hshell_show_banner(context);
    }
    return 0;
}

static int hshell_process_execute_command(hshell_context_t *ctx,int argc,const char *argv[])
{
    int ret=0;
    hshell_context_t *context=hshell_context_check_context(ctx);
    hshell_printf(context,"\r\n");
    bool command_processed=false;
    {
        //处理内部命令


        if(strcmp(argv[0],"exit")==0)
        {
            ret=-1;
            command_processed=true;
            //退出登录
            context->flags.login=0;
        }
    }

    if(!command_processed)
    {
        hshell_printf(context,"%s not found!\r\n",argv[0]);
        for(int i=0; i<argc; i++)
        {
            hshell_printf(context,"argv%d=%s\r\n",(int)i,argv[i]);
        }
    }

    return ret;
}

static int hshell_process_execute(hshell_context_t *ctx)
{
    int ret=0;
    hshell_context_t *context=hshell_context_check_context(ctx);
    int argc=0;
    /*
     * argv 前两项记录hshell上下文(幻数+指针)，最后一项为NULL
     */
    const char * argv[HSHELL_MAX_ARGC+3]= {0};
    argv[0]=(const char *)(uintptr_t)0x6873686C; // 幻数 hshl
    argv[1]=(const char *)context;
    {
        size_t current_ptr=0;
        argv[2]=(const char *)&context->buffer[current_ptr];
        while(current_ptr < context->buffer_ptr)
        {
            if(context->buffer[current_ptr]=='\0')
            {
                //到达字符串末尾
                break;
            }

            if(context->buffer[current_ptr]==' ')
            {
                current_ptr++;
                if(argc >= HSHELL_MAX_ARGC)
                {
                    //超过允许的参数
                    break;
                }
                if(context->buffer[current_ptr]!=' ' && context->buffer[current_ptr]!='\0')
                {
                    argv[2+argc]=(const char *)&context->buffer[current_ptr];
                }
                continue;
            }

            current_ptr++;

            if(context->buffer[current_ptr]==' ')
            {
                context->buffer[current_ptr]='\0';
                current_ptr++;
                argc++;
                if(argc >= HSHELL_MAX_ARGC)
                {
                    //超过允许的参数
                    break;
                }
                if(context->buffer[current_ptr]!=' ' && context->buffer[current_ptr]!='\0')
                {
                    argv[2+argc]=(const char *)&context->buffer[current_ptr];
                }
            }

            if(context->buffer[current_ptr]=='\0')
            {
                argc++;
                break;
            }

        }
    }

    if(argc > 0)
    {
        // 执行命令
        ret=hshell_process_execute_command(context,argc,&argv[2]);
    }

    if(context->flags.login!=0)
    {
        //清零提示符标志，显示提示符
        context->flags.prompt=0;
    }

    {
        //复位buffer
        memset(context->buffer,0,sizeof(context->buffer));
        context->buffer_ptr=0;
    }
    return ret;
}

static int hshell_process_input(hshell_context_t *ctx)
{
    int ret=0;
    hshell_context_t *context=hshell_context_check_context(ctx);
    int ch=hshell_getchar(context);
    bool need_echo=true;
    switch(ch)
    {
    case EOF:
    {
        ret=EOF;
        need_echo=false;
    }
    break;
    case '\n':
    {
        //处理字符串
        ret=hshell_process_execute(context);
    }
    break;
    case '\b':
    {
        if(context->buffer_ptr>0)
        {
            context->buffer_ptr--;
        }
    }
    break;
    case 0x7f:
    {
        if(context->buffer_ptr>0)
        {
            strcpy((char *)&context->buffer[context->buffer_ptr-1],(char *)&context->buffer[context->buffer_ptr]);
            context->buffer_ptr--;
        }
    }
    break;
    default:
    {
        if(context->buffer_ptr < (sizeof(context->buffer)-1))
        {
            context->buffer[context->buffer_ptr++]=(ch&0xFF);
        }
        if(context->buffer_ptr == (sizeof(context->buffer)-1))
        {
            context->buffer[context->buffer_ptr]='\0';
            ret=hshell_process_execute(context);
        }
    }
    break;
    }
    //回显字符
    if(context->flags.echo!=0 && need_echo)
    {
        hshell_putchar(context,ch);
    }

    return ret;
}

static int hshell_show_promtp_string(hshell_context_t *ctx)
{
    int ret=0;
    hshell_context_t *context=hshell_context_check_context(ctx);
    if(context->flags.login!=0 && context->flags.prompt==0)
    {
        context->flags.prompt=1;
        //打印提示符
        hshell_printf(context,"%s",(context->prompt!=NULL)?context->prompt:"");
    }
    return ret;
}

int hshell_loop(hshell_context_t *ctx)
{
    int ret=0;
    hshell_context_t *context=hshell_context_check_context(ctx);
    if((ret=hshell_login(context))<0)
    {
        return ret;
    }

    if((ret=hshell_show_promtp_string(context))<0)
    {
        return ret;
    }

    if((ret=hshell_process_input(context))<0)
    {
        return ret;
    }

    return ret;
}
