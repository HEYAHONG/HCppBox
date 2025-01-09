#include <jni.h>
#include <string>
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "hrc.h"
#include "hbox.h"
#include "HCPPBox.h"
#include <android/log.h>
#include <dlfcn.h>

#define LOGI(...) \
  ((void)__android_log_print(ANDROID_LOG_INFO, "helloworld-libs::", __VA_ARGS__))


static std::string GetCurrentLibPATH()
{
    std::string ret;
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
    return ret;
}

static std::string GetCurrentLibDir()
{
    std::string ret=GetCurrentLibPATH();
    for(size_t i=0; i<ret.length(); i++)
    {
        if(ret.c_str()[ret.length()-1-i]=='/')
        {
            ret=ret.substr(0,ret.length()-i);
            break;
        }
    }
    return ret;
}

static std::string GetLibexecPath(std::string libexec_name)
{
    std::string ret=GetCurrentLibDir()+libexec_name;
    return ret;
}


extern "C" JNIEXPORT jstring JNICALL
Java_cn_hyhsystem_hcppbox_helloworld_MainActivity_stringFromJNI(
    JNIEnv* env,
    jobject /* this */) {
    std::string hello = "Hello from C++\r\n";
    {
        //显示banner
        {
            const char * banner=(const char *)RCGetHandle((const char *)"banner");
            if(banner!=NULL)
            {
                hello+=banner;
            }
        }

    }
    return env->NewStringUTF(hello.c_str());
}

/*
 * 注意：初始化在主活动的构造函数中调用
 */
extern "C"
JNIEXPORT void JNICALL
Java_cn_hyhsystem_hcppbox_helloworld_MainActivity_Init(JNIEnv *env, jobject thiz) {
    LOGI("Init");
    LOGI("Lib_PATH:%s",GetCurrentLibPATH().c_str());
    LOGI("Lib_Dir:%s",GetCurrentLibDir().c_str());

    //执行helloworld
    {
        LOGI("libexec_helloworld.so:%s",GetLibexecPath("libexec_helloworld.so").c_str());
        system(GetLibexecPath("libexec_helloworld.so").c_str());
    }
}

/*
 * 注意：毫秒定时器由定时器(在主活动实现)调用
 */
static hdefaults_tick_t current_tick=0;
extern "C"
JNIEXPORT void JNICALL
Java_cn_hyhsystem_hcppbox_helloworld_MainActivity_MsTick(JNIEnv *env, jobject thiz) {
    current_tick++;
    if(current_tick%5000==0)
    {
        LOGI("Current Tick=%llu，SysTick=%llu",(unsigned  long long)current_tick,(unsigned  long long)hdefaults_tick_get());
    }
}