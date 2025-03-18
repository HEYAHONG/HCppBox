#include "hbox.h"
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
    while(hshell_loop(NULL)==0)
    {
        std::this_thread::yield();
    }
    hshell_printf(NULL,"\rhshell exit!\r\n");
    return 0;
}
