#include "stdio.h"
#include "stdlib.h"
#include "hbox.h"
#include "HCPPBox.h"
#include <thread>
#include <chrono>
#include <dlfcn.h>
#include "android/log.h"

#define LOGI(...) \
  ((void)__android_log_print(ANDROID_LOG_INFO, "libexec_helloworld::", __VA_ARGS__))

static int daemonize(void)
{
    pid_t pid=0;
    pid = fork();
    if(pid <0)
    {
        exit(1);
    }
    if(pid >0)
    {
        exit(0);
    }

    setsid();

    pid = fork();
    if(pid <0)
    {
        exit(1);
    }
    if(pid >0)
    {
        exit(0);
    }

    return 0;
}

static bool isrunning=true;
static void signalHandler(int signum)
{
    if(!isrunning)
    {
        return;
    }
    isrunning=false;
}
static void init_signal()
{
    signal(SIGINT, signalHandler);
    signal(SIGFPE, signalHandler);
    signal(SIGILL, signalHandler);
    signal(SIGABRT, signalHandler);
    signal(SIGSEGV, signalHandler);
    signal(SIGTERM, signalHandler);
}

static std::string CurrentLibPATH;
static std::string GetCurrentLibPATH()
{
    std::string ret=CurrentLibPATH;
    if(!ret.empty())
    {
        return ret;
    }
    void * handle = dlopen(NULL,RTLD_LAZY);
    if(handle!=NULL)
    {
        Dl_info info= {0};
        if(dladdr((const void *)GetCurrentLibPATH,&info))
        {
            ret=info.dli_fname;
        }
        dlclose(handle);
    }

    CurrentLibPATH=ret;
    return ret;
}


static bool become_root()
{
    if(getuid()!=0)
    {
        setenv("PATH","/sbin:/system/sbin:/system/bin:/system/xbin:/bin",1);
        //通过su获取root，此方法要求su存在且能被当前软件包成功执行，对于模拟器用户需要先修改su的权限使得其可被一般用户执行
        if(system("su root true")!=0)
        {
            //不能正常执行su
            LOGI("try become_root failed!");
            LOGI("current_uid=%d,current_gid=%d",(int)getuid(),(int)getgid());
            return false;
        }
        FILE *file= popen("su","w");
        if(file!=NULL)
        {
            {
                //以root权限执行命令
                std::string exec_path=GetCurrentLibPATH()+"\r\n";
                fwrite(exec_path.c_str(),exec_path.length(),1,file);
            }
            {
                //成功执行
                LOGI("try become_root success!");
            }
            pclose(file);
            exit(0);
        }
        {
            LOGI("try become_root failed!");
            LOGI("current_uid=%d,current_gid=%d",(int)getuid(),(int)getgid());
            return false;
        }
    }
    LOGI("current_uid=%d,current_gid=%d",(int)getuid(),(int)getgid());
    return true;
}

int main()
{
    //尝试获取root
    become_root();
    //后台化
    daemonize();
    //初始化signal
    init_signal();

    //输出helloworld,注意：此信息可能不具有包名，使用LogCat查看时不要使用包名过滤。
    LOGI("Hello World!\r\n");

    //循环运行
    while(isrunning)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    return 0;
}
