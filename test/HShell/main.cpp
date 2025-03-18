#include "hbox.h"
#include "time.h"
#include <thread>
#ifdef __unix__
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#define ECHOFLAGS (ECHO | ECHOE | ECHOK | ECHONL)
int set_disp_mode(int fd,int option)
{
    int err;
    struct termios term;
    if(tcgetattr(fd,&term)==-1)
    {
        perror("Cannot get the attribution of the terminal");
        return 1;
    }
    if(option)
    {
        term.c_lflag|=ECHOFLAGS;
        term.c_lflag|= ICANON;
    }
    else
    {
        term.c_lflag &=~ECHOFLAGS;
        term.c_lflag &=(~ICANON);
    }
    err=tcsetattr(fd,TCSAFLUSH,&term);
    if(err==-1 && err==EINTR)
    {
        perror("Cannot set the attribution of the terminal");
        return 1;
    }
    return 0;
}
#endif

static int command_time_main(int argc,const char *argv[])
{
    hshell_context_t * hshell_ctx=hshell_context_get_from_main_argv(argc,argv);
    time_t time_now=time(NULL);
    hshell_printf(hshell_ctx,"%s",asctime(localtime(&time_now)));
    return 0;
};
static hshell_command_t commands[]=
{
    {
        command_time_main,
        "time",
        "show time"
    }
};

int main(int argc,const char *argv[])
{
    //关闭输出缓冲
    setbuf(stdout,NULL);
    setbuf(stdin,NULL);
#ifndef WIN32
    hshell_printf(NULL,"press Ctrl-D to exit!\r\n");
#endif
#if !defined(__unix__)
    //一般终端中无需开启回显，串口终端才开启回显
    hshell_echo_set(NULL,false);
#else
    //关闭输入回显，使用hshell内部的回显
    set_disp_mode(STDIN_FILENO,0);
#endif
    hshell_command_array_set(NULL,commands,sizeof(commands)/sizeof(commands[0]));
    while(hshell_loop(NULL)==0)
    {
        std::this_thread::yield();
    }
    hshell_printf(NULL,"\rhshell exit!\r\n");
    return 0;
}
