#include "hbox_config.h"
#include "hbox.h"
#ifdef HRC_ENABLED
#include "hrc.h"
#endif // HRC_ENABLED
#include "serial8250.h"
#include "syscon.h"

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

int hbox_putchar( int ch )
{
    return serial8250_putchar((volatile serial8250_t *)CONSOLE_8250_BASE,ch);
}

int hbox_getchar(void)
{
    return serial8250_getchar((volatile serial8250_t *)CONSOLE_8250_BASE);
}

static int hbox_version_entry(int argc,const char *argv[])
{
    hshell_context_t * hshell_ctx=hshell_context_get_from_main_argv(argc,argv);
    hshell_printf(hshell_ctx,"0.0.0.1\r\n");
    return 0;
}
HSHELL_COMMAND_EXPORT(hbox_version,hbox_version_entry,show hbox_version);

static int hbox_halt_entry(int argc,const char *argv[])
{
    SYSCON_HALT();
}
HSHELL_COMMAND_EXPORT(halt,hbox_halt_entry,poweroff);

static int hbox_reboot_entry(int argc,const char *argv[])
{
    SYSCON_REBOOT();
}
HSHELL_COMMAND_EXPORT(reboot,hbox_reboot_entry,reboot);



