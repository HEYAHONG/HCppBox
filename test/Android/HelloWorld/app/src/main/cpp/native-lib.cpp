#include <jni.h>
#include <string>
#include "hrc.h"
#include "hbox.h"
#include "HCPPBox.h"

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