#include "HCPPBox.h"
#include "stdint.h"
#include "stdio.h"
#include <thread>
#include <chrono>

class A
{
private:
    int m_A[100];
};

class B
{
private:
    int m_B[100];
};

class C:public A,public B,public HCPPObject
{
    O_HCPPOBJECT
public:

};

C g_global;
C *g_global_heap=NULL;
void thread_func()
{
    {
        //全局变量测试
        printf("thread g_global(%08X) heap:%s thread:%s\n",(uint32_t)reinterpret_cast<uintptr_t>(&g_global),g_global.IsInHeap()?"true":"false",g_global.IsInThread()?"true":"false");
        printf("thread g_global_heap(%08X) heap:%s thread:%s\n",(uint32_t)reinterpret_cast<uintptr_t>(g_global_heap),g_global_heap->IsInHeap()?"true":"false",g_global_heap->IsInThread()?"true":"false");
    }
}

void IsInHeap_IsInThread_main()
{
    printf(__FILE__  " Start\r\n");

    //缺陷:不可在main之前分配内存
    g_global_heap=new C;
    {
        //全局变量测试
        printf("g_global(%08X) heap:%s thread:%s\n",(uint32_t)reinterpret_cast<uintptr_t>(&g_global),g_global.IsInHeap()?"true":"false",g_global.IsInThread()?"true":"false");
        printf("g_global_heap(%08X) heap:%s thread:%s\n",(uint32_t)reinterpret_cast<uintptr_t>(g_global_heap),g_global_heap->IsInHeap()?"true":"false",g_global_heap->IsInThread()?"true":"false");
    }

    //启动线程
    std::thread(thread_func).detach();
    std::this_thread::sleep_for(std::chrono::seconds(1));

    {
        //删除全局变量
        delete g_global_heap;
        g_global_heap=nullptr;
    }

    {
        //局部变量测试
        C m_local;
        C *m_local_heap=new C;
        printf("m_local(%08X) heap:%s thread:%s\n",(uint32_t)reinterpret_cast<uint64_t>(&m_local),m_local.IsInHeap()?"true":"false",m_local.IsInThread()?"true":"false");
        printf("m_local_heap(%08X) heap:%s thread:%s\n",(uint32_t)reinterpret_cast<uint64_t>(m_local_heap),m_local_heap->IsInHeap()?"true":"false",m_local_heap->IsInThread()?"true":"false");
        delete m_local_heap;
        m_local_heap=nullptr;
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));

    printf(__FILE__  " End\r\n");
}
