#include "stdio.h"
#include "stdlib.h"
#include "hbox.h"
#include "HCPPBox.h"
#include <dlfcn.h>
#include "android/log.h"

#define LOGI(...) \
  ((void)__android_log_print(ANDROID_LOG_INFO, "libexec_helloworld::", __VA_ARGS__))

int main()
{
    //输出helloworld,注意：此信息可能不具有包名，使用LogCat查看时不要使用包名过滤。
    LOGI("Hello World!\r\n");
    return 0;
}
