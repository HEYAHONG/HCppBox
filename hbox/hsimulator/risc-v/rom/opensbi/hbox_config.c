#include "hbox_config.h"
#include "hbox.h"
#include "opensbi_port.h"
#ifdef HRC_ENABLED
#include "hrc.h"
#endif // HRC_ENABLED

hdefaults_tick_t hbox_tick_get(void)
{
    return 0;
}

void hbox_enter_critical()
{

}

void hbox_exit_critical()
{

}

void * hbox_malloc(size_t bytes)
{
    return malloc(bytes);
}

void hbox_free(void *ptr)
{
    free(ptr);
}


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



