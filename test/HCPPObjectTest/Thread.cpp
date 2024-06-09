#include "HCPPBox.h"
#include "stdint.h"
#include <thread>
#include <chrono>
class Runable:public HCPPObject
{
    int8_t cnt;
    O_HCPPOBJECT
public:
    //注意：不可在构造函数里调用SetRunable,有可能导致调用不到子类重载的函数
    Runable(HCPPObject *parent):HCPPObject(parent),cnt(0)
    {
        printf("ctor %08X\r\n",(uint32_t)reinterpret_cast<uintptr_t>(this));
    }
    ~Runable()
    {
        printf("dtor %08X\r\n",(uint32_t)reinterpret_cast<uintptr_t>(this));
    }
    void InvokeInit() override
    {
        printf("init %08X\r\n",(uint32_t)reinterpret_cast<uintptr_t>(this));
    }
    void InvokeUpdate() override
    {
        //使用cnt确保只打印一次
        if(cnt<=0)
        {
            printf("update %08X\r\n",(uint32_t)reinterpret_cast<uintptr_t>(this));
        }

        cnt=1;
    }
};

void Thread_main()
{
    printf(__FILE__  " Start\r\n");
    printf("new Thread\r\n");
    //创建线程
    HCPPThread *thread_handle=HCPPThread::New();

    //不允许通过new创建线程
    //new HCPPThread();

    printf("new Runable\r\n");
    //创建一个对象
    (new Runable(thread_handle))->SetRunable();

    printf("new Runable\r\n");
    //创建一个对象
    (new Runable(thread_handle))->SetRunable();

    //等待1s
    std::this_thread::sleep_for(std::chrono::seconds(1));

    printf("delete Thread\r\n");
    //删除线程，删除线程后不要再使用thread_handle指针
    thread_handle->DeleteThread();

    //等待3s
    std::this_thread::sleep_for(std::chrono::seconds(3));
    printf(__FILE__  " End\r\n");
}
