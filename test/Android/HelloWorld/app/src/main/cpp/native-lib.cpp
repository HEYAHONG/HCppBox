#include <jni.h>
#include <string>
#include "hrc.h"
#include "hbox.h"
#include "HCPPBox.h"
#include <android/log.h>

#define LOGI(...) \
  ((void)__android_log_print(ANDROID_LOG_INFO, "helloworld-libs::", __VA_ARGS__))

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
}

/*
 * 注意：毫秒定时器由定时器(在主活动实现)调用
 */
hdefaults_tick_t current_tick=0;
extern "C"
JNIEXPORT void JNICALL
Java_cn_hyhsystem_hcppbox_helloworld_MainActivity_MsTick(JNIEnv *env, jobject thiz) {
    current_tick++;
    if(current_tick%5000==0)
    {
        LOGI("Current Tick=%llu",(unsigned  long long)current_tick);
    }
}