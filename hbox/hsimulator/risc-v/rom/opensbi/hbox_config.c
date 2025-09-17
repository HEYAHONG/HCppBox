#include "hbox_config.h"
#include "hbox.h"
#include "opensbi_port.h"
#ifdef HRC_ENABLED
#include "hrc.h"
#endif // HRC_ENABLED

hdefaults_tick_t hbox_tick_get(void)
{
    /*
     * TODO:确定系数,默认为aclint-mtimer @ 10000000Hz,
     */
#if defined(HDEFAULTS_ARCH_RISCV64)
    return sbi_rdtime()/10000;
#else
    {
        hdefaults_mutex_lock(NULL);
        static uint64_t mtime_base=0;
        uint64_t current_mtime=(mtime_base&(0xFFFFFFFF00000000))+sbi_rdtime();
        if(current_mtime < mtime_base)
        {
            current_mtime+=(1ULL<<32);
        }
        mtime_base=current_mtime;
        hdefaults_mutex_unlock(NULL);
        return current_mtime/10000;
    }
#endif
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
            hdefaults_tick_t start=hdefaults_tick_get();
            while(hdefaults_tick_get()-start < n*1000);
        }
    }
    return 0;
}
HSHELL_COMMAND_EXPORT(sleep,hbox_sleep_entry,sleep);



