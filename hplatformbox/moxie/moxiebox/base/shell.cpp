/***************************************************************
 * Name:      shell.cpp
 * Purpose:   moxiebox基础固件HShell命令与注册
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-24
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hruntime.h"
#include "hshell.h"

static void sys_exit(int status)
{
    register long r0 __asm__("$r0")=status;
    __asm__ volatile("swi 1"::"r"(r0):"memory");
    for(;;)
    {
    }
}

static void sys_restart(void)
{
    __asm__ volatile("swi 0":::"memory");
    for(;;)
    {
    }
}

static int version_entry(int argc,const char *argv[])
{
    (void)argc;
    (void)argv;
    hshell_printf(NULL,"0.0.0.1\r\n");
    return 0;
}
HSHELL_COMMAND_EXPORT(version,version_entry,show version);

static int halt_entry(int argc,const char *argv[])
{
    (void)argc;
    (void)argv;
    sys_exit(0);
    return 0;
}
HSHELL_COMMAND_EXPORT(halt,halt_entry,halt);

static int reboot_entry(int argc,const char *argv[])
{
    (void)argc;
    (void)argv;
    sys_restart();
    return 0;
}
HSHELL_COMMAND_EXPORT(reboot,reboot_entry,reboot);

static void shell_init(const hruntime_function_t *func)
{
    (void)func;
    HSHELL_COMMANDS_REGISTER(NULL);
}
HRUNTIME_INIT_EXPORT(shell,255,shell_init,NULL);
HRUNTIME_SYMBOL_EXPORT(shell_init);

static void shell_loop(const hruntime_function_t *func)
{
    (void)func;
    hshell_loop(NULL);
}
HRUNTIME_LOOP_EXPORT(shell,255,shell_loop,NULL);
HRUNTIME_SYMBOL_EXPORT(shell_loop);
