#include "HCPPBox.h"
#include "stdint.h"
#include <thread>
#include <chrono>

void Timer_main()
{
    printf(__FILE__  " Start\r\n");

    new HCPPTimer(NULL,std::chrono::seconds(1),[](HCPPTimer *timer)
    {
        printf("OneShot Timer %08X\r\n",(uint32_t)reinterpret_cast<uintptr_t>(timer));
    },true);
    new HCPPTimer(NULL,std::chrono::seconds(1),[](HCPPTimer *timer)
    {
        printf("OneShot Timer %08X\r\n",(uint32_t)reinterpret_cast<uintptr_t>(timer));
    },true);
    new HCPPTimer(NULL,std::chrono::seconds(1),[](HCPPTimer *timer)
    {
        printf("OneShot Timer %08X\r\n",(uint32_t)reinterpret_cast<uintptr_t>(timer));
    },true);
    new HCPPTimer(NULL,std::chrono::seconds(1),[](HCPPTimer *timer)
    {
        printf("OneShot Timer %08X\r\n",(uint32_t)reinterpret_cast<uintptr_t>(timer));
    },true);

    std::this_thread::sleep_for(std::chrono::seconds(3));
    new HCPPTimer(NULL,std::chrono::seconds(1),[](HCPPTimer *timer)
    {
        static int cnt=0;
        printf("Period Timer %08X:cnt=%d\r\n",(uint32_t)reinterpret_cast<uintptr_t>(timer),cnt++);
        if(cnt>3)
        {
            //不使用定时器时直接删除定时器
            timer->DeleteTimer();
        }
    },false);
    std::this_thread::sleep_for(std::chrono::seconds(5));

    printf(__FILE__  " End\r\n");
}
