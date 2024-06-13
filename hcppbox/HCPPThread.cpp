#include "HCPPThread.h"

HCPPThread::HCPPThread():HCPPObject(NULL),std::thread(std::bind(&HCPPThread::RunThread,this)),m_idle_sleep_time(std::chrono::milliseconds(1))
{

}

HCPPThread::~HCPPThread()
{

}

void HCPPThread::RunThread()
{
    //HCPPOBject先于线程初始化，此时可使用HCPPObject的成员。
    //等待初始化
    while(!HasFlag(HCPPOBJECT_FLAG_RUN_INIT))
    {
        Run();
        if (m_idle_sleep_time != std::chrono::milliseconds(0))
        {
            std::this_thread::sleep_for(m_idle_sleep_time);
        }
        else
        {
            std::this_thread::yield();
        }
    }

    //HCPPThread类初始化完成,可使用类中的成员变量
    while(!HasFlag(HCPPOBJECT_FLAG_TO_BE_DELETED))
    {
        Run();
        if (m_idle_sleep_time != std::chrono::milliseconds(0))
        {
            std::this_thread::sleep_for(m_idle_sleep_time);
        }
        else
        {
            std::this_thread::yield();
        }
    }

    //分离线程与对象
    detach();

    //删除自身
    delete this;
}

//创建线程
HCPPThread *HCPPThread::New()
{
    HCPPThread *ret=new HCPPThread();
    if(ret!=NULL)
    {
        //设定线程信息为新创建的线程,表示线程对象的所有权归属新创建的线程
        ret->SetThreadId(ret->get_id(),true);
        //设定为可执行对象，此时Run函数开始生效
        ret->SetRunable();
    }
    return ret;
}

//删除线程
void HCPPThread::DeleteThread()
{
    //设置删除标志
    DeleteLatter();
}

void HCPPThread::SetChildDelete(bool childdelete)
{
    HCPPObject::SetChildDelete(childdelete);
}
