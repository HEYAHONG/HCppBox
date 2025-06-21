/***************************************************************
 * Name:      hshell_context.c
 * Purpose:   实现hshell_context接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-03-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hshell_context.h"

#ifdef putchar
#undef putchar
#endif

#ifdef getchar
#undef getchar
#endif

extern int hgetchar(void);
extern int hputchar(int ch);
hshell_context_external_api_t hshell_context_default_external_api(void)
{
    hshell_context_external_api_t api= {0};
    api.getchar=hgetchar;
    api.putchar=hputchar;
    api.invoke_command=NULL;
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
    real_context->flags.escape=0;
    real_context->flags.return_newline_compatible=0;
    real_context->flags.input_complete=0;
    real_context->flags.insert_mode=1;
    real_context->flags.echo=1;         //默认打开回显
    real_context->flags.show_banner=1;  //默认显示banner
    real_context->flags.command_name_shortcut=0;
    memset(real_context->buffer,0,sizeof(real_context->buffer));
    real_context->buffer_ptr=0;
    real_context->command.array_base=NULL;
    real_context->command.array_count=0;
    memset(real_context->escape_sequence,0,sizeof(real_context->escape_sequence));
    memset(&real_context->sub_context,0,sizeof(real_context->sub_context));
    memset(&real_context->history,0,sizeof(real_context->history));
}

hshell_context_t * __hshell_context_check_context(hshell_context_t *ctx)
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
    hshell_context_t *context=__hshell_context_check_context(ctx);
    const char *old_prompt_string=context->prompt;
    if(prompt!=NULL)
    {
        context->prompt=prompt;
    }
    return old_prompt_string;
}

const char *hshell_prompt_string_get(hshell_context_t *ctx)
{
    hshell_context_t *context=__hshell_context_check_context(ctx);
    return context->prompt;
}

hshell_context_external_api_t hshell_external_api_set(hshell_context_t *ctx,hshell_context_external_api_t api)
{
    hshell_context_t *context=__hshell_context_check_context(ctx);
    hshell_context_external_api_t old_api=context->api;
    context->api=api;
    return old_api;
}

hshell_context_external_api_t hshell_external_api_get(hshell_context_t *ctx)
{
    hshell_context_t *context=__hshell_context_check_context(ctx);
    return context->api;
}

bool hshell_echo_set(hshell_context_t *ctx,bool echo)
{
    hshell_context_t *context=__hshell_context_check_context(ctx);
    bool old_echo=(context->flags.echo!=0);
    context->flags.echo=(echo?0x1:0x0);
    return old_echo;
}

bool hshell_echo_get(hshell_context_t *ctx)
{
    hshell_context_t *context=__hshell_context_check_context(ctx);
    return context->flags.echo!=0;
}

bool hshell_command_name_shortcut_set(hshell_context_t *ctx,bool command_name_shortcut)
{
    hshell_context_t *context=__hshell_context_check_context(ctx);
    bool old_command_name_shortcut=(context->flags.command_name_shortcut!=0);
    context->flags.command_name_shortcut=(command_name_shortcut?0x1:0x0);
    return old_command_name_shortcut;
}

bool hshell_command_name_shortcut_get(hshell_context_t *ctx)
{
    hshell_context_t *context=__hshell_context_check_context(ctx);
    return context->flags.command_name_shortcut!=0;
}

bool hshell_show_banner_set(hshell_context_t *ctx,bool show_banner)
{
    hshell_context_t *context=__hshell_context_check_context(ctx);
    bool old_show_banner=(context->flags.show_banner!=0);
    context->flags.show_banner=(show_banner?0x1:0x0);
    return old_show_banner;
}

bool hshell_show_banner_get(hshell_context_t *ctx)
{
    hshell_context_t *context=__hshell_context_check_context(ctx);
    return context->flags.show_banner!=0;
}

void hshell_command_array_set(hshell_context_t *ctx,const hshell_command_t *array_base,size_t array_count)
{
    hshell_context_t *context=__hshell_context_check_context(ctx);
    if(array_base==NULL)
    {
        array_count=0;
    }
    context->command.array_count=array_count;
    context->command.array_base=array_base;
}

int hshell_context_getchar(hshell_context_t *ctx)
{
    hshell_context_t *context=__hshell_context_check_context(ctx);
    if(context->api.getchar!=NULL)
    {
        return context->api.getchar();
    }
    else
    {
        return hshell_context_default_external_api().getchar();
    }
}

int hshell_context_putchar(hshell_context_t *ctx,int ch)
{
    hshell_context_t *context=__hshell_context_check_context(ctx);
    if(context->api.putchar!=NULL)
    {
        return context->api.putchar(ch);
    }
    else
    {
        return hshell_context_default_external_api().putchar(ch);
    }
}

static void hshell_context_printf_out(char character, void* arg)
{
    hshell_context_putchar((hshell_context_t *)arg,(uint8_t)character);
}

int hshell_context_printf(hshell_context_t *ctx,const char *fmt,...)
{
    hshell_context_t *context=__hshell_context_check_context(ctx);
    int ret=0;
    va_list va;
    va_start(va, fmt);
    ret=hvfctprintf(hshell_context_printf_out,context,fmt,va);
    va_end(va);
    return ret;
}

void hshell_subcontext_enter(hshell_context_t *ctx,hshell_context_t *next_ctx)
{
    hshell_context_t *context=__hshell_context_check_context(ctx);
    if(next_ctx==NULL || next_ctx==context)
    {
        return;
    }
    context->sub_context.next=next_ctx;
}

void hshell_subcontext_exit(hshell_context_t *ctx)
{
    hshell_context_t *context=__hshell_context_check_context(ctx);
    if(context->sub_context.next!=NULL)
    {
        context->sub_context.next=NULL;
    }
}

void hshell_subcontext_exit_from_sub(hshell_context_t *sub_ctx)
{
    hshell_context_t *context=__hshell_context_check_context(sub_ctx);
    if(context->sub_context.prev!=NULL)
    {
        context->sub_context.prev->sub_context.next=NULL;
        context->sub_context.prev=NULL;
    }
}

hshell_context_t * hshell_context_get_from_main_argv(int argc,const char *argv[])
{
    hshell_context_t * ret=NULL;
    if(argc < 0 || argv == NULL)
    {
        return ret;
    }

    const char ** base=((const char **)(uintptr_t)(((uintptr_t)argv)-2*sizeof(const char *)));
    if(((uintptr_t)base[0])==0x6873686C)
    {
        //幻数检查成功
        ret=(hshell_context_t *)base[1];
    }
    return ret;
}
