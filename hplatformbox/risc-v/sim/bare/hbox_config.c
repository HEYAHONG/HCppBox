#include "hbox_config.h"
#include "hbox.h"
#ifdef HRC_ENABLED
#include "hrc.h"
#endif // HRC_ENABLED
#include "console.h"

int hbox_putchar( int ch )
{
    return console_putchar((uint8_t)ch);
}

int hbox_getchar(void)
{
    return console_getchar();
}

static int hbox_version_entry(int argc,const char *argv[])
{
    hshell_context_t * hshell_ctx=hshell_context_get_from_main_argv(argc,argv);
    hshell_printf(hshell_ctx,"0.0.0.1\r\n");
    return 0;
}
HSHELL_COMMAND_EXPORT(hbox_version,hbox_version_entry,show hbox_version);

static int hbox_sleep_entry(int argc,const char *argv[])
{
    hshell_context_t * hshell_ctx=hshell_context_get_from_main_argv(argc,argv);
    if(argc<2)
    {
        hshell_printf(hshell_ctx,"sleep [n]\r\n");
    }
    else
    {
        int n=atoi(argv[1]);
        if(n>0)
        {
            vTaskDelay((TickType_t)n*configTICK_RATE_HZ);
        }
    }
    return 0;
}
HSHELL_COMMAND_EXPORT(sleep,hbox_sleep_entry,sleep);

static int cmd_ps_entry(int argc,const char *argv[])
{
    hshell_context_t * hshell_ctx=hshell_context_get_from_main_argv(argc,argv);
    char strbuff[1024];
    strbuff[sizeof(strbuff)-1]='\0';
    vTaskList(strbuff);
    hshell_printf(hshell_ctx,"%s\r\n",strbuff);
    return 0;
}
HSHELL_COMMAND_EXPORT(ps,cmd_ps_entry,show process info);
