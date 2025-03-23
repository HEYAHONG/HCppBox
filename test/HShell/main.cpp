#include "hbox.h"
#include "time.h"
#include <thread>
#include <string>
#include <vector>
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
        "datetime",
        "show date and time"
    }
};


#if defined(__unix__)
#include "unistd.h"
#include<sys/types.h>
#include<sys/wait.h>
static std::vector<std::string> get_path()
{
    std::vector<std::string> ret;
    {
        const char *path_str=getenv("PATH");
        if(path_str!=NULL)
        {
            std::string path(path_str);
            while(!path.empty())
            {
                size_t pos=path.find(":");
                if(pos==std::string::npos)
                {
                    ret.push_back(path);
                    path.clear();
                }
                else
                {
                    ret.push_back(path.substr(0,pos));
                    path=path.substr(pos+1);
                }
            }
        }
    }
    //增加一些默认路径
    ret.push_back("");
    ret.push_back(".");
    return ret;
}
static std::string find_program(std::string name)
{
    std::string ret=name;
    {
        std::vector<std::string> paths=get_path();
        for(size_t i=0; i<paths.size(); i++)
        {
            if(access((paths[i]+"/"+name).c_str(),R_OK|X_OK)==0)
            {
                ret=paths[i]+"/"+name;
                break;
            }
#ifdef __CYGWIN__
            if(access((paths[i]+"/"+name+".exe").c_str(),R_OK|X_OK)==0)
            {
                ret=paths[i]+"/"+name+".exe";
                break;
            }
#endif
        }
    }
    return ret;
}
#elif defined(WIN32) || defined(_WIN32)  || defined(_WIN64)
#include "windows.h"
#include "process.h"
#include "stdio.h"
#include "io.h"
static std::vector<std::string> get_path()
{
    std::vector<std::string> ret;
    {
        const char* path_str = getenv("PATH");
        if (path_str != NULL)
        {
            std::string path(path_str);
            while (!path.empty())
            {
                size_t pos = path.find(";");
                if (pos == std::string::npos)
                {
                    ret.push_back(path);
                    path.clear();
                }
                else
                {
                    ret.push_back(path.substr(0, pos));
                    path = path.substr(pos + 1);
                }
            }
        }
    }
    //增加一些默认路径
    ret.push_back("");
    ret.push_back(".");
    return ret;
}
static std::string find_program(std::string name)
{
    std::string ret = name;
    {
        std::vector<std::string> paths = get_path();
        for (size_t i = 0; i < paths.size(); i++)
        {
            if (_access((paths[i] + "\\" + name).c_str(), 04) == 0)
            {
                ret = paths[i] + "\\" + name;
                break;
            }
            if (_access((paths[i] + "\\" + name + ".exe").c_str(), 04) == 0)
            {
                ret = paths[i] + "\\" + name + ".exe";
                break;
            }

            if (_access((paths[i] + "\\" + name + ".bat").c_str(), 04) == 0)
            {
                ret = paths[i] + "\\" + name + ".bat";
                break;
            }

            if (_access((paths[i] + "\\" + name + ".cmd").c_str(), 04) == 0)
            {
                ret = paths[i] + "\\" + name + ".cmd";
                break;
            }
        }
    }
    return ret;
}

static std::string get_cmdline(int argc, const char* argv[])
{
    std::string ret;
    for (size_t i = 0; i < argc; i++)
    {
        if (argv[i] != NULL)
        {
            std::string arg(argv[i]);
            if (arg.find(" ") != std::string::npos)
            {
                arg = (std::string("\"") + arg + "\"");
            }
            ret += (arg + " ");
        }
    }
    return ret;
}

#endif
static int invoke_command(int argc, const char* argv[])
{
    hshell_context_t * hshell_ctx=hshell_context_get_from_main_argv(argc,argv);
    int ret = 0;
#if defined(__unix__)
    int pid = fork();
    if (pid < 0)
    {
        return -1;
    }
    if (pid == 0)
    {
        set_disp_mode(STDIN_FILENO,1);
        if(execv(find_program(argv[0]).c_str(), (char**)argv) < 0)
        {
            hshell_printf(hshell_ctx,"%s not found!\n",argv[0]);
            set_disp_mode(STDIN_FILENO,0);
            exit(-1);
        }
        set_disp_mode(STDIN_FILENO,0);
        exit(0);
    }
    else
    {
        int ret = 0;
        waitpid(pid, &ret, 0);
        set_disp_mode(STDIN_FILENO,0);
        return ret;
    }
#elif defined(WIN32) || defined(_WIN32)  || defined(_WIN64)
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    if (CreateProcessA(find_program(argv[0]).c_str(), (char *)get_cmdline(argc,argv).c_str(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
    {
        //等待子进程退出
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
    else
    {
        hshell_printf(hshell_ctx,"%s not found!\n",argv[0]);
        ret = -1;
    }

#endif
    return ret;
}

int main(int argc,const char *argv[])
{
    //关闭输出缓冲
    setbuf(stdout,NULL);
    setbuf(stdin,NULL);
#if !defined(__unix__)
    //一般终端中无需开启回显，串口终端才开启回显
    hshell_echo_set(NULL,false);
#else
    //关闭输入回显，使用hshell内部的回显
    set_disp_mode(STDIN_FILENO,0);
#endif
    hshell_command_array_set(NULL,commands,sizeof(commands)/sizeof(commands[0]));
    {
        hshell_context_external_api_t api=hshell_context_default_external_api();
        api.invoke_command=invoke_command;
        hshell_external_api_set(NULL,api);
    }
    while(hshell_loop(NULL)==0)
    {
        std::this_thread::yield();
    }
    hshell_printf(NULL,"\rhshell exit!\r\n");
#ifdef __unix__
    set_disp_mode(STDIN_FILENO,1);
#endif // __unix__
    return 0;
}
