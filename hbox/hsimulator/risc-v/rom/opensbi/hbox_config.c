#include "hbox_config.h"
#include "hbox.h"
#include "opensbi_port.h"
#ifdef HRC_ENABLED
#include "hrc.h"
#endif // HRC_ENABLED

int hbox_putchar(int ch)
{
    if(ch>0)
    {
        sbi_ecall_putc(ch&0xFF);
    }
    return ch;
}

int hbox_getchar(void)
{
    int ch=EOF;
    {
        ch=sbi_ecall_getc();
        if((((ch != '\f') && (ch != '\r') && (ch != '\n') && (ch != '\b') && (ch != '\t')) && ch < ' ') || ch > 127)
        {
            /*
             * 空格字符当作失败
             */
            ch=EOF;
        }
    }
    return ch;
}



static int hbox_version_entry(int argc,const char *argv[])
{
    hshell_context_t * hshell_ctx=hshell_context_get_from_main_argv(argc,argv);
    hshell_printf(hshell_ctx,"0.0.0.1\r\n");
    return 0;
}
HSHELL_COMMAND_EXPORT(hbox_version,hbox_version_entry,show hbox_version);


static int hbox_reboot_entry(int argc,const char *argv[])
{
    hshell_context_t * hshell_ctx=hshell_context_get_from_main_argv(argc,argv);
    sbi_ecall_reboot();
    return 0;
}
HSHELL_COMMAND_EXPORT(reboot,hbox_reboot_entry,reboot system);

static int hbox_shutdown_entry(int argc,const char *argv[])
{
    hshell_context_t * hshell_ctx=hshell_context_get_from_main_argv(argc,argv);
    sbi_ecall_shutdown();
    return 0;
}
HSHELL_COMMAND_EXPORT(poweroff,hbox_shutdown_entry,poweroff system);

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
            vTaskDelay(n*1000);
        }
    }
    return 0;
}
HSHELL_COMMAND_EXPORT(sleep,hbox_sleep_entry,sleep);

#include "malloc.h"
static int cmd_free_entry(int argc,const char *argv[])
{
    hshell_context_t * hshell_ctx=hshell_context_get_from_main_argv(argc,argv);
    struct mallinfo info=mallinfo();
    size_t system_total=0;
    {
        extern char __heap_start[];
        extern char __heap_end[];
        system_total=(uintptr_t)__heap_end-(uintptr_t)__heap_start;
    }
    size_t free_size=(system_total-info.arena)+info.fordblks;
    size_t max_used_size=info.arena;
    size_t used_size=info.uordblks;
    size_t total_size=free_size+used_size;
    hshell_printf(hshell_ctx,"total:%d bytes,max_used:%d bytes,used:%d bytes,free:%d bytes\r\n",total_size,max_used_size,used_size,free_size);
    return 0;
};
HSHELL_COMMAND_EXPORT(free,cmd_free_entry,show memory info);

static int cmd_ps_entry(int argc,const char *argv[])
{
    hshell_context_t * hshell_ctx=hshell_context_get_from_main_argv(argc,argv);
    char strbuff[4096];
    strbuff[sizeof(strbuff)-1]='\0';
    vTaskList(strbuff);
    hshell_printf(hshell_ctx,"%s\r\n",strbuff);
    return 0;
};
HSHELL_COMMAND_EXPORT(ps,cmd_ps_entry,show process info);


