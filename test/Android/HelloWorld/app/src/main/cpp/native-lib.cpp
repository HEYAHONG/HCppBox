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
