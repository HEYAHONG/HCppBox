
#include "stdlib.h"
#include "stdint.h"
#include "signal_helper.h"

static void (*sigint_handler_callback)(void)=NULL;

#if defined(WIN32) || defined(_WIN32)
#include <windows.h>
#include <stdio.h>

BOOL WINAPI CtrlHandler(DWORD fdwCtrlType)
{
    switch (fdwCtrlType)
    {
    case CTRL_C_EVENT:
    {
        auto cb=sigint_handler_callback;
        if(cb!=NULL)
        {
            cb();
        }
    }
    return TRUE;
    default:
    {

    }
    return FALSE;
    }
}
#else
#include <signal.h>
#include <pthread.h>
#include <unistd.h>
void signal_handler(int sig)
{
    {
        auto cb=sigint_handler_callback;
        if(cb!=NULL)
        {
            cb();
        }
    }
}
#endif

void sigint_init(void)
{
#if defined(WIN32) || defined(_WIN32)
    SetConsoleCtrlHandler(CtrlHandler, TRUE);
#else
    signal(SIGINT, signal_handler);
    signal(SIGFPE, signal_handler);
    signal(SIGILL, signal_handler);
    signal(SIGABRT, signal_handler);
    signal(SIGSEGV, signal_handler);
    signal(SIGTERM, signal_handler);
#endif
}

void sigint_reg(void (*sigint_handler)(void))
{
    sigint_handler_callback=sigint_handler;
}


