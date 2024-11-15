#include "HCPPBox.h"
#include "stdint.h"
#include <thread>
#include <chrono>

extern "C" void Init_c_early_init();
void Init_main()
{
    printf(__FILE__  " Start\r\n");
    {
        printf(__FILE__  " Early Init Start\r\n");
        Init_c_early_init();
        printf(__FILE__  " Early Init End\r\n");
    }
    {
        //TODO:初始化其它组件
    }
    {
        printf(__FILE__  " Init Start\r\n");
        HCPPInit();
        printf(__FILE__  " Init End\r\n");
    }

    printf(__FILE__  " End\r\n");
}
